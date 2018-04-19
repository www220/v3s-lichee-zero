#include "rthw.h"
#include <rtthread.h>
#include "rtdevice.h"

#include "board.h"
#include "interrupt.h"
#include <netif/ethernetif.h>
#include <lwipopts.h>

#define MAX_ADDR_LEN 		6
#define _EMAC_DEVICE(eth)	(struct emac_device*)(eth)

struct emac_device
{
	/* inherit from Ethernet device */
	struct eth_device parent;
    /* uboot dev_t */
    void * dev_ptr;
	/* interface address info. */
	rt_uint8_t  dev_addr[MAX_ADDR_LEN];			/* MAC address	*/
};
static struct emac_device _emac;

void _enet_isr(int vector, void *param)
{
    struct rt_device_t *dev = (struct rt_device_t *)param;
    struct emac_device *emac = _EMAC_DEVICE(dev);
    RT_ASSERT(emac != RT_NULL);

}

extern int _sun8i_emac_eth_init(void *priv, rt_uint8_t *enetaddr);
static rt_err_t _emac_init(rt_device_t dev)
{
    struct emac_device *emac = _EMAC_DEVICE(dev);
    RT_ASSERT(emac != RT_NULL);

	/* initialize enet */
    _sun8i_emac_eth_init(emac->dev_ptr, emac->dev_addr);
	return RT_EOK;
}

static rt_err_t _emac_open(rt_device_t dev, rt_uint16_t oflag)
{
	return RT_EOK;
}

static rt_err_t _emac_close(rt_device_t dev)
{
	return RT_EOK;
}

static rt_size_t _emac_read(rt_device_t dev, rt_off_t pos, void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_size_t _emac_write (rt_device_t dev, rt_off_t pos, const void* buffer, rt_size_t size)
{
	rt_set_errno(-RT_ENOSYS);
	return 0;
}

static rt_err_t _emac_control(rt_device_t dev, int cmd, void *args)
{
    struct emac_device *emac = _EMAC_DEVICE(dev);
    RT_ASSERT(emac != RT_NULL);

	switch(cmd)
	{
	case NIOCTL_GADDR:
		/* get MAC address */
		if(args) rt_memcpy(args, emac->dev_addr, MAX_ADDR_LEN);
		else return -RT_ERROR;
		break;

	default :
		break;
	}

	return RT_EOK;
}

/* Ethernet device interface */
/* transmit packet. */
extern int _sun8i_emac_eth_send(void *priv, void *packet, int len);
rt_err_t _emac_tx(rt_device_t dev, struct pbuf* p)
{
    struct pbuf* q;
    rt_err_t result = RT_EOK;
    struct emac_device *emac = _EMAC_DEVICE(dev);
    RT_ASSERT(emac != RT_NULL);

    for (q = p; q != NULL; q = q->next){
        _sun8i_emac_eth_send(emac->dev_addr, q->payload, q->len);
    }

    return result;
}

/* reception packet. */
extern int _sun8i_eth_recv(void *priv, rt_int8_t **packetp);
extern int _sun8i_free_pkt(void *priv);
struct pbuf *_emac_rx(rt_device_t dev)
{
    struct pbuf *q,*p = RT_NULL;
    struct emac_device *emac = _EMAC_DEVICE(dev);
    RT_ASSERT(emac != RT_NULL);

    do {
        rt_int8_t *framepack = RT_NULL;
        int framelength = _sun8i_eth_recv(emac->dev_ptr, &framepack);
        if (framelength <= 0){
            break;
        }
        p = pbuf_alloc(PBUF_LINK, framelength, PBUF_RAM);
        if (p == RT_NULL) {
            break;
        }
        int offset = 0;
        for (q = p; q != RT_NULL; q= q->next){
            rt_memcpy(q->payload,framepack+offset,q->len);
            offset += q->len;
        }
        _sun8i_free_pkt(emac->dev_ptr);
    }while (0);

    return p;
}

extern int sun8i_emac_eth_probe(const char* name, void **priv);
int rt_hw_eth_init(void)
{
    int ret = sun8i_emac_eth_probe("e0", &_emac.dev_ptr);
    if (ret != 0){
        rt_kprintf("failed to rt_hw_eth_init code:%d\n", ret);
        return ret;
    }

    /* test MAC address */
	_emac.dev_addr[0] = 0x00;
	_emac.dev_addr[1] = 0x11;
	_emac.dev_addr[2] = 0x22;
	_emac.dev_addr[3] = 0x33;
	_emac.dev_addr[4] = 0x44;
	_emac.dev_addr[5] = 0x55;

	_emac.parent.parent.init       = _emac_init;
	_emac.parent.parent.open       = _emac_open;
	_emac.parent.parent.close      = _emac_close;
	_emac.parent.parent.read       = _emac_read;
	_emac.parent.parent.write      = _emac_write;
	_emac.parent.parent.control    = _emac_control;
	_emac.parent.parent.user_data  = RT_NULL;

	_emac.parent.eth_rx     = _emac_rx;
	_emac.parent.eth_tx     = _emac_tx;

    /* register ETH device */
    eth_device_init(&(_emac.parent), "e0");

	return 0;
}
INIT_DEVICE_EXPORT(rt_hw_eth_init)


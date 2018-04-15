#include "gpio.h"
#include "sunxi_mmc.h"
#include "clock_sun6i.h"
#include "mmc.h"
#include "io.h"

#include "rtthread.h"
#include <rthw.h>
#include <drivers/mmcsd_core.h>

#define SUNXI_MMC0_BASE			0x01c0f000
#define SUNXI_MMC_CMD_START    (0x1 << 31)


#define __DEBUG__  
  
#ifdef __DEBUG__  
#define DEBUG(format,...) rt_kprintf("File: "__FILE__", Line: %05d: "format"\n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif  



#if 1

static int mmc_update_clk(void);

static int mmc_rint_wait(unsigned int timeout_msecs, unsigned int done_bit)
{
	unsigned int status;
    struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
	
	do {
		status = readl(&reg->rint);
		
		if (!timeout_msecs-- || (status & SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT)) 
		{
			DEBUG("status: %08x\n",status);
			return -110;
		}
		
		rt_thread_delay(1);
	} while (!(status & done_bit));
    
	return 0;
}

static int mmc_trans_data_by_cpu(struct rt_mmcsd_data * data)
{
	struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
	const int reading = !!(data->flags & DATA_DIR_READ);
	const unsigned int status_bit = reading ? SUNXI_MMC_STATUS_FIFO_EMPTY :
					      SUNXI_MMC_STATUS_FIFO_FULL;
	unsigned i;
	unsigned *buff = (unsigned *)data->buf;
	unsigned byte_cnt = data->blksize * data->blks;
	unsigned timeout_usecs = (byte_cnt >> 8) * 1000;
	if (timeout_usecs < 2000000)
		timeout_usecs = 2000000;
	
	setbits_le32(&reg->gctrl, SUNXI_MMC_GCTRL_ACCESS_BY_AHB);
	
	for (i = 0; i < (byte_cnt >> 2); i++) 
	{
		while (readl(&reg->status) & status_bit)
		{
			if (!timeout_usecs--)
				return -1;
		}

		if (reading)
		{
		    buff[i] = readl(&reg->fifo);
            //DEBUG("receive data: %d timeout_usecs: %d \n",buff[i],timeout_usecs);			
		}
		else
		{
			writel(buff[i],&reg->fifo);
		}
	}
	
	return 0;
}


static void sunxi_mmc_request(struct rt_mmcsd_host *host, struct rt_mmcsd_req *req)
{
	struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
	struct rt_mmcsd_data *data = req->data;
	struct rt_mmcsd_cmd  *cmd  = req->cmd;
	struct rt_mmcsd_cmd  *stop = req->stop;
	
	unsigned int cmdval = SUNXI_MMC_CMD_START;
	unsigned int timeout_msecs = 0;
	int error = 0;
	unsigned int status = 0;
	
	rt_uint32_t blocks = 0;
	rt_uint32_t length = 0;
	rt_uint32_t block_length = 0;
	
	
	DEBUG("sunxi_mmc_request is runing \n");
	if(stop)
	    DEBUG("stop not run \n");
	
	if (cmd->cmd_code == 12)
		return;
	
	if (!cmd->cmd_code)
		cmdval |= SUNXI_MMC_CMD_SEND_INIT_SEQ;
	if (resp_type(cmd) != RESP_NONE)
		cmdval |= SUNXI_MMC_CMD_RESP_EXPIRE;
	if (resp_type(cmd) == RESP_R2)
		cmdval |= SUNXI_MMC_CMD_LONG_RESPONSE;
	
	//cmdval |= SUNXI_MMC_CMD_CHK_RESPONSE_CRC;
	
	writel(0xffffffff,&reg->timeout);
	
	if (data) 
	{
		block_length = data->blksize;
		blocks = data->blks;
		length = block_length * blocks;
		
		cmdval |= SUNXI_MMC_CMD_DATA_EXPIRE|SUNXI_MMC_CMD_WAIT_PRE_OVER;
		
		if (data->flags & DATA_DIR_WRITE)
			cmdval |= SUNXI_MMC_CMD_WRITE;
		if (blocks > 1)
			cmdval |= SUNXI_MMC_CMD_AUTO_STOP;
		
		writel(block_length, &reg->blksz);
		writel(length, &reg->bytecnt);
		
		//cmdval |= SUNXI_MMC_CMD_CHK_RESPONSE_CRC;
		
		
	}
	
	writel(cmd->arg, &reg->arg);
	
	if (!data)
	{   		
		writel(cmdval | cmd->cmd_code, &reg->cmd);
	}	
		
	/*
	 * transfer data and check status
	 * STATREG[2] : FIFO empty
	 * STATREG[3] : FIFO full
	 */
	 
	 if(data)
	 {
		 int ret = 0;
        
		 writel(cmdval | cmd->cmd_code, &reg->cmd);
		 ret = mmc_trans_data_by_cpu(data);
		 if(ret)
		 {
			 error = readl(&reg->rint) & \
				SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT;
			error = -110;
            DEBUG("send data by cpu is err\n");
			goto out;
		 }
	 }
	
	DEBUG("cmd->type %08x  Sending command %d as %08X, arg = %08X, blocks = %d, length = %d \n",cmd->flags, cmd->cmd_code, cmdval, cmd->arg, blocks, block_length);
	
    error = mmc_rint_wait(1000, SUNXI_MMC_RINT_COMMAND_DONE);
	if (error)
		goto out;
	
	if (data) 
	{
		timeout_msecs = 120;
		error = mmc_rint_wait(timeout_msecs,
				      data->blks > 1 ?
				      SUNXI_MMC_RINT_AUTO_COMMAND_DONE :
				      SUNXI_MMC_RINT_DATA_OVER);
	    if (error)
		    goto out;
	}
	
	if (resp_type(cmd) == RESP_R1B)
	{
		timeout_msecs = 2000;
		
		DEBUG("R1B is run \n");
		
		do {
			status = readl(&reg->status);
			if (!timeout_msecs--) {
				error = -110;
				goto out;
			}
			rt_thread_delay(1);
		} while (status & SUNXI_MMC_STATUS_CARD_DATA_BUSY);
	}
	
	cmd->resp[0] = readl(&reg->resp0);
	cmd->resp[1] = readl(&reg->resp1);
	cmd->resp[2] = readl(&reg->resp2);
	cmd->resp[3] = readl(&reg->resp3);
	
	DEBUG("resp_0: %08x resp_1: %08x resp_2: %08x resp_3: %08x\n",cmd->resp[0],cmd->resp[1],cmd->resp[2],cmd->resp[3]);

out:
    
    if (error < 0) {
		writel(SUNXI_MMC_GCTRL_RESET, &reg->gctrl);
		mmc_update_clk();
		cmd->err = -RT_ETIMEOUT;
	}
	
    writel(0xffffffff, &reg->rint);
	writel(readl(&reg->gctrl) | SUNXI_MMC_GCTRL_FIFO_RESET,
	       &reg->gctrl);
	
	DEBUG("error:%d \n", error);
	
	mmcsd_req_complete(host);

}


static int mmc_set_mod_clk(unsigned int hz)
{
	struct sunxi_ccm_reg * mclkreg =  (struct sunxi_ccm_reg *)0x01C20088;
	unsigned int pll, pll_hz, div, n, oclk_dly, sclk_dly;

	if (hz <= 24000000) {
		pll = CCM_MMC_CTRL_OSCM24;
		pll_hz = 24000000;
	} else {
		pll = CCM_MMC_CTRL_PLL6;
		pll_hz = clock_get_pll6();
	}

	div = pll_hz / hz;
	if (pll_hz % hz)
		div++;

	n = 0;
	while (div > 16) {
		n++;
		div = (div + 1) / 2;
	}

	if (n > 3) {
		return -1;
	}

	/* determine delays */
	if (hz <= 400000) {
		oclk_dly = 0;
		sclk_dly = 0;
	} else if (hz <= 25000000) {
		oclk_dly = 0;
		sclk_dly = 5;
	} else if (hz <= 50000000) {
		oclk_dly = 3;
		sclk_dly = 4;
	} else {
		/* hz > 50000000 */
		oclk_dly = 1;
		sclk_dly = 4;
	}

	writel(CCM_MMC_CTRL_ENABLE | pll | CCM_MMC_CTRL_SCLK_DLY(sclk_dly) |
	       CCM_MMC_CTRL_N(n) | CCM_MMC_CTRL_OCLK_DLY(oclk_dly) |
	       CCM_MMC_CTRL_M(div), mclkreg);

    DEBUG("addr: %08x data: %08x \n",mclkreg, *mclkreg);

	return 0;
}



static int mmc_update_clk(void)
{
	struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
	unsigned int cmd;
	unsigned timeout_msecs = 2000;

	cmd = SUNXI_MMC_CMD_START |
	      SUNXI_MMC_CMD_UPCLK_ONLY |
	      SUNXI_MMC_CMD_WAIT_PRE_OVER;
	writel(cmd, &reg->cmd);
	
	DEBUG("addr: %08x data: %08x \n",&reg->cmd, reg->cmd);
	
	while (readl(&reg->cmd) & SUNXI_MMC_CMD_START) 
	{
		if (!timeout_msecs--)
			return -1;
		rt_thread_delay(1);
	}

	/* clock update sets various irq status bits, clear these */
	writel(readl(&reg->rint), &reg->rint);
    
	DEBUG("addr: %08x data: %08x \n",&reg->rint, reg->rint);
	
	return 0;
}


static int mmc_config_clock(unsigned int clock)
{
	struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
	unsigned rval = readl(&reg->clkcr);

	/* Disable Clock */
	rval &= ~SUNXI_MMC_CLK_ENABLE;
	writel(rval, &reg->clkcr);
	
	DEBUG("addr: %08x data: %08x \n",&reg->clkcr, reg->clkcr);
	
	if (mmc_update_clk())
		return -1;

	/* Set mod_clk to new rate */
	if (mmc_set_mod_clk(clock))
		return -1;

	/* Clear internal divider */
	rval &= ~SUNXI_MMC_CLK_DIVIDER_MASK;
	writel(rval, &reg->clkcr);

	DEBUG("addr: %08x data: %08x \n",&reg->clkcr, reg->clkcr);
	
	/* Re-enable Clock */
	rval |= SUNXI_MMC_CLK_ENABLE;
	writel(rval, &reg->clkcr);
	if (mmc_update_clk())
		return -1;
    
	DEBUG("addr: %08x data: %08x \n",&reg->clkcr, reg->clkcr);
	
	return 0;
}

static void sunxi_mmc_set_iocfg(struct rt_mmcsd_host *host, struct rt_mmcsd_io_cfg *io_cfg)
{
	struct sunxi_mmc * reg = (struct sunxi_mmc *)SUNXI_MMC0_BASE;
    unsigned int clk = io_cfg->clock;
	unsigned int width = io_cfg->bus_width;
	
	rt_kprintf("set ios: bus_width: %d, clock: %d\n",
	      width, clk);
    
	
	/* Change clock first */
	if (clk && mmc_config_clock(clk) != 0) 
    {
		// mmchost->fatal_err = 1;
		return;
	}
	
	/* Change bus width */
	if (width == MMCSD_BUS_WIDTH_8)
		writel(0x2, &reg->width);
	else if (width == MMCSD_BUS_WIDTH_4)
		writel(0x1, &reg->width);
	else
		writel(0x0, &reg->width);
	
	DEBUG("addr: %08x data: %08x \n",&reg->width, reg->width);
}

static int mmc_clk_io_on(int sdc_no)
{
	struct sunxi_ccm_reg *ccm = (struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	
	setbits_le32(&ccm->ahb_gate0, 1 << AHB_GATE_OFFSET_MMC(sdc_no));
	
	setbits_le32(&ccm->ahb_reset0_cfg, 1 << AHB_RESET_OFFSET_MMC(sdc_no));
	
	DEBUG("addr: %08x data: %08x \n",&ccm->ahb_gate0, ccm->ahb_gate0);
	DEBUG("addr: %08x data: %08x \n",&ccm->ahb_reset0_cfg, ccm->ahb_reset0_cfg);
	
	return mmc_set_mod_clk(24000000);
}




static const struct rt_mmcsd_host_ops ops = {
	sunxi_mmc_request,
	sunxi_mmc_set_iocfg,
    RT_NULL,
	RT_NULL,
};

void sunxi_mmc_init(void)
{
    struct rt_mmcsd_host *host;
	
	host = mmcsd_alloc_host();
	if (!host) 
	{
		return;
	}
	
	host->ops = &ops;
	host->freq_min = 400000;
	host->freq_max = 5000000;
	host->valid_ocr = VDD_32_33 | VDD_33_34;
	host->flags = MMCSD_BUSWIDTH_4 | MMCSD_MUTBLKWRITE | MMCSD_SUP_HIGHSPEED | MMCSD_SUP_SDIO_IRQ;
	host->max_seg_size = 65535;
	host->max_dma_segs = 2;
	host->max_blk_size = 512;
	host->max_blk_count = 4096;
	
	mmc_clk_io_on(0);
	
	mmcsd_change(host);
}

void mmc_pinmux_setup(void)
{
	unsigned int pin;
	
	/* SDC0: PF0-PF5 */
	for (pin = SUNXI_GPF(0); pin <= SUNXI_GPF(5); pin++) 
	{
		sunxi_gpio_set_cfgpin(pin, SUNXI_GPF_SDC0);
		sunxi_gpio_set_pull(pin, SUNXI_GPIO_PULL_UP);
		sunxi_gpio_set_drv(pin, 2);
	}
}

void my_mmc_init(void * parameter)
{
	mmc_pinmux_setup();
	
	sunxi_mmc_init();
}

#endif


void sdio_test___(void * parameter)
{
	rt_mmcsd_core_init();
	rt_mmcsd_blk_init();

	my_mmc_init(parameter);
}



















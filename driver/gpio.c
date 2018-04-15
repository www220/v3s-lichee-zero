
#include "rthw.h"
#include "rtdevice.h"
#include "board.h"


#define GPIO_BASE      (0x01C20800)
#define PG_CFG0_REG    (0xD8)
#define PG_CFG1_REG    (0xDC)
#define PG_CFG2_REG    (0xE0)
#define PG_CFG3_REG    (0xE4)
#define PG_DATA_REG    (0xE8)
#define PG_DRV0_REG    (0xEC)
#define PG_DRV1_REG    (0xF0)
#define PG_PULL0_REG   (0xF4)
#define PG_PULL1_REG   (0xF8)


#ifdef RT_USING_PIN

/* 后续制作一个.h文件 把地址和结构提打包 */
/* 然后写.c库 */

static void led_init(void)
{
    
    volatile unsigned int * p_temp = RT_NULL;

    /* 设置GPIO_G0 引脚 输出模式 */
    p_temp = GPIO_BASE + PG_CFG0_REG;
    *p_temp &= 0xfffffff9;
    *p_temp |= 0x00000001;

    /* 设置 输出低电平 */
    p_temp = GPIO_BASE + PG_DATA_REG;
    *p_temp &= 0xfffffffE;
}


void pin_write(rt_device_t dev, rt_base_t pin, rt_base_t value)
{
    volatile unsigned int * p_temp = RT_NULL;

    p_temp = GPIO_BASE + PG_DATA_REG;

    if(value == PIN_HIGH)
    {
        *p_temp |= (0x01 << pin); 
    }
    else
    {
        *p_temp &= ((0x01 << pin) ^ 0xffffffff);
    }



}

int pin_read(rt_device_t dev, rt_base_t pin)
{
    volatile unsigned int * p_temp = RT_NULL;

    p_temp = GPIO_BASE + PG_DATA_REG;
    
    
    if((*p_temp) & (0x01 << (pin * 4)))
    {
    	return PIN_HIGH;
    }
    
    return PIN_LOW;
}

void pin_mode(rt_device_t dev, rt_base_t pin, rt_base_t mode)
{
    volatile unsigned int * p_temp = RT_NULL;
    unsigned int test;
    p_temp = GPIO_BASE + PG_CFG0_REG;


    if(mode == PIN_MODE_OUTPUT)
    {
        *p_temp |= (0x01 << (pin * 4)); 
        *p_temp &= ((0x06 << (pin * 4)) ^ 0xffffffff);
    }
    else if(mode == PIN_MODE_INPUT)
    {

    }
    else if(mode == PIN_MODE_INPUT_PULLUP)
    {
    
    }
    else
    {

    }
    
}

const static struct rt_pin_ops _pin_ops = 
{
	pin_mode,
    pin_write,
    pin_read,
};

int _hw_pin_init(void)
{
    
    //led_init();

	rt_device_pin_register("pin", &_pin_ops, RT_NULL);
	return 0;
}


#endif
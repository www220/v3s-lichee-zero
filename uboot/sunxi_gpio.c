/*
 * (C) Copyright 2012 Henrik Nordstrom <henrik@henriknordstrom.net>
 *
 * Based on earlier arch/arm/cpu/armv7/sunxi/gpio.c:
 *
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <errno.h>
#include <fdtdec.h>
#include <malloc.h>
#include <asm/arch/gpio.h>
#include <asm/io.h>
#include <asm/gpio.h>
#include <dt-bindings/gpio/gpio.h>
#include <dt-bindings/pinctrl/sun4i-a10.h>

static int sunxi_gpio_output(u32 pin, u32 val)
{
	u32 dat;
	u32 bank = GPIO_BANK(pin);
	u32 num = GPIO_NUM(pin);
	struct sunxi_gpio *pio = BANK_TO_GPIO(bank);

	dat = readl(&pio->dat);
	if (val)
		dat |= 0x1 << num;
	else
		dat &= ~(0x1 << num);

	writel(dat, &pio->dat);

	return 0;
}

static int sunxi_gpio_input(u32 pin)
{
	u32 dat;
	u32 bank = GPIO_BANK(pin);
	u32 num = GPIO_NUM(pin);
	struct sunxi_gpio *pio = BANK_TO_GPIO(bank);

	dat = readl(&pio->dat);
	dat >>= num;

	return dat & 0x1;
}

int gpio_direction_input(unsigned gpio)
{
	sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_INPUT);

	return 0;
}

int gpio_direction_mode(unsigned gpio, int mode)
{
    if (mode == 0){
	    sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_INPUT);
    	sunxi_gpio_set_pull(gpio, SUN4I_PINCTRL_PULL_UP);
    	sunxi_gpio_set_drv(gpio, SUN4I_PINCTRL_10_MA);
    }else if (mode == 1){
        sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_OUTPUT);
        sunxi_gpio_set_pull(gpio, SUN4I_PINCTRL_PULL_UP);
    	sunxi_gpio_set_drv(gpio, SUN4I_PINCTRL_10_MA);
    }else if (mode == 2){
        sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_OUTPUT);
        sunxi_gpio_set_pull(gpio, SUN4I_PINCTRL_NO_PULL);
    	sunxi_gpio_set_drv(gpio, SUN4I_PINCTRL_10_MA);
    }else{
        sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_DISABLE);
        sunxi_gpio_set_pull(gpio, SUN4I_PINCTRL_NO_PULL);
    }

	return 0;
}

int gpio_direction_output(unsigned gpio, int value)
{
	sunxi_gpio_set_cfgpin(gpio, SUNXI_GPIO_OUTPUT);

	return sunxi_gpio_output(gpio, value);
}

int gpio_get_value(unsigned gpio)
{
	return sunxi_gpio_input(gpio);
}

int gpio_set_value(unsigned gpio, int value)
{
	return sunxi_gpio_output(gpio, value);
}

int sunxi_name_to_gpio(const char *name)
{
	int group = 0;
	int groupsize = 9 * 32;
	long pin;
	char *eptr;

	if (*name == 'P' || *name == 'p')
		name++;
	if (*name >= 'A') {
		group = *name - (*name > 'a' ? 'a' : 'A');
		groupsize = 32;
		name++;
	}

	pin = simple_strtol(name, &eptr, 10);
	if (!*name || *eptr)
		return -1;
	if (pin < 0 || pin > groupsize || group >= 9)
		return -1;
	return group * 32 + pin;
}


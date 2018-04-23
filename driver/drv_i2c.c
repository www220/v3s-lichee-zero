/*
 * File      : drv_uart.c
 * This file is part of RT-Thread RTOS
 * COPYRIGHT (C) 2017, RT-Thread Development Team
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 * Change Logs:
 * Date           Author       Notes
 * 2017-5-30      Bernard      the first version
 */

#include <rthw.h>
#include <rtthread.h>
#include <rtdevice.h>

#include "board.h"
#include "interrupt.h"

static void gpio_set_sda(void *data, rt_int32_t state)
{
    uint32_t *pin = (uint32_t *)data;
    gpio_direction_output(pin[0], state);
}
static void gpio_set_scl(void *data, rt_int32_t state)
{
    uint32_t *pin = (uint32_t *)data;
    gpio_direction_output(pin[1], state);
}
static rt_int32_t gpio_get_sda(void *data)
{
    uint32_t *pin = (uint32_t *)data;
    gpio_direction_input(pin[0]);
    return gpio_get_value(pin[0]);
}
static rt_int32_t gpio_get_scl(void *data)
{
    uint32_t *pin = (uint32_t *)data;
    gpio_direction_input(pin[1]);
    return gpio_get_value(pin[1]);
}
static void gpio_udelay(rt_uint32_t us)
{
    udelay(us);
}
static struct rt_i2c_bit_ops _i2c_bit_ops =
{
    NULL,
    gpio_set_sda,
    gpio_set_scl,
    gpio_get_sda,
    gpio_get_scl,
    gpio_udelay,
    10,
    10
};

uint32_t i2c0_pin[2] = {SUNXI_GPB(7), SUNXI_GPB(6)};
uint32_t i2c1_pin[2] = {SUNXI_GPB(3), SUNXI_GPB(2)};
struct rt_i2c_bus_device bit_i2c0;
struct rt_i2c_bus_device bit_i2c1;
int rt_hw_i2c_init(void)
{
    gpio_set_mode(i2c0_pin[0], PIN_TYPE(SUNXI_GPIO_OUTPUT)|PULL_NO);
    gpio_set_mode(i2c0_pin[1], PIN_TYPE(SUNXI_GPIO_OUTPUT)|PULL_NO);
    gpio_set_value(i2c0_pin[0], 1);
    gpio_set_value(i2c0_pin[1], 1);
    _i2c_bit_ops.data = i2c0_pin;
	bit_i2c0.priv = &_i2c_bit_ops;
	rt_i2c_bit_add_bus(&bit_i2c0, "i2c0");

    gpio_set_mode(i2c1_pin[0], PIN_TYPE(SUNXI_GPIO_OUTPUT)|PULL_NO);
    gpio_set_mode(i2c1_pin[1], PIN_TYPE(SUNXI_GPIO_OUTPUT)|PULL_NO);
    gpio_set_value(i2c1_pin[0], 1);
    gpio_set_value(i2c1_pin[1], 1);
   _i2c_bit_ops.data = i2c1_pin;
	bit_i2c1.priv = &_i2c_bit_ops;
	rt_i2c_bit_add_bus(&bit_i2c1, "i2c1");

    return 0;
}
INIT_APP_EXPORT(rt_hw_i2c_init);

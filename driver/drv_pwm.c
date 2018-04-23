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

extern int sun4i_pwm_probe(u32 base, u32 clk);
int rt_hw_pwm_init(void)
{
    gpio_set_mode(SUNXI_GPB(4), PIN_TYPE(SUNXI_GPB_PWM0)|PULL_UP);
    gpio_set_mode(SUNXI_GPB(5), PIN_TYPE(SUNXI_GPB_PWM1)|PULL_UP);
    sun4i_pwm_probe(0x1c21400, 24000000);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_pwm_init);

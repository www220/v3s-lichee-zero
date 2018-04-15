/*
 * File      : board.c
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

#include "board.h"
#include "drv_uart.h"

#include "interrupt.h"

#define TIMER_BASE             0x01C20C00
#define TMR_IRQ_EN_REG         0x00
#define TMR_IRQ_STA_REG        0x04
#define TMR0_CTRL_REG          0x10
#define TMR0_INTV_VALUE_REG    0x14
#define TMR0_CUR_VALUE_REG     0x18


void clock_irq(void)
{
     /* 中断使用说明：先清源头中断，在清GIC中断, */
    volatile unsigned int * p_temp = TIMER_BASE + TMR_IRQ_STA_REG;  //临时指针

    rt_interrupt_enter();

    *p_temp |= (*p_temp & 0x01);

    rt_hw_interrupt_clear(50);

    rt_tick_increase();

    rt_interrupt_leave();
}

void rt_hw_tick_init(void)
{
    volatile unsigned int * p_temp;  //临时指针

    /* 打开定时器0的中断 */
    p_temp = TIMER_BASE + TMR_IRQ_EN_REG;
    *p_temp |= 0x01;

    /*循环计数 24M时钟源  2分频  自动重装载值 不启动计数器*/
    p_temp = TIMER_BASE + TMR0_CTRL_REG;
    *p_temp &= 0x00; 
    *p_temp |= 0x16;

    /* 定时器间隔 0  */
    p_temp = TIMER_BASE + TMR0_INTV_VALUE_REG;
    *p_temp = 1000 * 12;

    /* 定时器计数个数 */
    p_temp = TIMER_BASE + TMR0_CUR_VALUE_REG;
    *p_temp = 1000 * 12;

    /* 打开定时器，开始工作 */
    p_temp = TIMER_BASE + TMR0_CTRL_REG;
    *p_temp |= 0x01;

    /* 使能中断 */
    enable_interrupt(50,0,3);
    register_interrupt_routine(50,clock_irq);
}

void rt_hw_board_init(void)
{
    /* MMU初始化 */
    rt_hw_mmu_init();

    /* 初始化中断控制器 */
    rt_hw_interrupt_init();

#ifdef RT_USING_HEAP
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

    /* 设置时钟 */
    rt_hw_tick_init();

    /* 设置串口 */
    rt_hw_uart_init();

#ifdef RT_USING_CONSOLE
    /* set console device */
    rt_console_set_device(RT_CONSOLE_DEVICE_NAME);
#endif /* RT_USING_CONSOLE */

#ifdef RT_USING_COMPONENTS_INIT
    rt_components_board_init();
#endif
}

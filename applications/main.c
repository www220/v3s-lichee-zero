/*
 * File      : main.c
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
#include <stdio.h>
#include <stdlib.h>

#include "rtthread.h"
#include "board.h"
#include "interrupt.h"
#include "rtconfig.h"
#include "rtdevice.h"




void rt_application_init(void)
{
    /* led测试函数 */
    extern void led_test(void * parameter);
	led_test(RT_NULL);
	
	/* LCD测试函数 */
	// extern void rt_init_thread_entry(void* parameter);
	// rt_init_thread_entry(RT_NULL);
	
	/* SDIO测试函数 */
	//extern void sdio_test(void * parameter);
	//sdio_test(RT_NULL);
	
}

int main()
{

    /* disable interrupt first */
    rt_hw_interrupt_disable();

    rt_hw_interrupt_init();

    rt_hw_board_init();

    rt_show_version();

	//rt_kprintf("rt_system_heap_init is run \n");
	
	
#ifdef RT_USING_HEAP
    /* init memory system */
    rt_system_heap_init(RT_HW_HEAP_BEGIN, RT_HW_HEAP_END);
#endif

    /* init scheduler system */
    rt_system_scheduler_init();

#ifdef RT_USING_FINSH
    // finsh_system_init();
    // finsh_set_device(RT_CONSOLE_DEVICE_NAME);
#endif 
    /* initialize timer */
    rt_system_timer_init();

    /* init timer thread */
    rt_system_timer_thread_init();

    /* init application */
    rt_application_init();

    /* init idle thread */
    rt_thread_idle_init();
	
    /* start scheduler */
    rt_system_scheduler_start();
    /* never reach here */
	
    return 0;
}

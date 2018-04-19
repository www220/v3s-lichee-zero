/*
 * File      : board.h
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

#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdint.h>

extern unsigned char __bss_start;
extern unsigned char __bss_end;

#define RT_HW_HEAP_BEGIN    (void*)&__bss_end
#define RT_HW_HEAP_END      (void*)0x43C00000

void rt_hw_board_init(void);
void rt_hw_mmu_init(void);
int rt_hw_pin_init(void);
int rt_hw_uart_init(void);

void udelay(unsigned long usec);
void mdelay(unsigned long msec);
ulong tick_read_timer(void);

/* GPIO bank sizes */
#define SUNXI_GPIO_A_NR		32
#define SUNXI_GPIO_B_NR		32
#define SUNXI_GPIO_C_NR		32
#define SUNXI_GPIO_D_NR		32
#define SUNXI_GPIO_E_NR		32
#define SUNXI_GPIO_F_NR		32
#define SUNXI_GPIO_G_NR		32
#define SUNXI_GPIO_H_NR		32
#define SUNXI_GPIO_I_NR		32
#define SUNXI_GPIO_L_NR		32
#define SUNXI_GPIO_M_NR		32

#define SUNXI_GPIO_NEXT(__gpio) \
	((__gpio##_START) + (__gpio##_NR) + 0)

enum sunxi_gpio_number {
	SUNXI_GPIO_A_START = 0,
	SUNXI_GPIO_B_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_A),
	SUNXI_GPIO_C_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_B),
	SUNXI_GPIO_D_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_C),
	SUNXI_GPIO_E_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_D),
	SUNXI_GPIO_F_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_E),
	SUNXI_GPIO_G_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_F),
	SUNXI_GPIO_H_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_G),
	SUNXI_GPIO_I_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_H),
	SUNXI_GPIO_L_START = 352,
	SUNXI_GPIO_M_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_L),
	SUNXI_GPIO_N_START = SUNXI_GPIO_NEXT(SUNXI_GPIO_M),
	SUNXI_GPIO_AXP0_START = 1024,
};

/* SUNXI GPIO number definitions */
#define SUNXI_GPA(_nr)	(SUNXI_GPIO_A_START + (_nr))
#define SUNXI_GPB(_nr)	(SUNXI_GPIO_B_START + (_nr))
#define SUNXI_GPC(_nr)	(SUNXI_GPIO_C_START + (_nr))
#define SUNXI_GPD(_nr)	(SUNXI_GPIO_D_START + (_nr))
#define SUNXI_GPE(_nr)	(SUNXI_GPIO_E_START + (_nr))
#define SUNXI_GPF(_nr)	(SUNXI_GPIO_F_START + (_nr))
#define SUNXI_GPG(_nr)	(SUNXI_GPIO_G_START + (_nr))
#define SUNXI_GPH(_nr)	(SUNXI_GPIO_H_START + (_nr))
#define SUNXI_GPI(_nr)	(SUNXI_GPIO_I_START + (_nr))
#define SUNXI_GPL(_nr)	(SUNXI_GPIO_L_START + (_nr))
#define SUNXI_GPM(_nr)	(SUNXI_GPIO_M_START + (_nr))
#define SUNXI_GPN(_nr)	(SUNXI_GPIO_N_START + (_nr))
#define SUNXI_GPAXP0(_nr)	(SUNXI_GPIO_AXP0_START + (_nr))

int gpio_direction_input(unsigned gpio);
int gpio_direction_output(unsigned gpio, int value);
int gpio_direction_mode(unsigned gpio, int mode);
int gpio_get_value(unsigned gpio);
int gpio_set_value(unsigned gpio, int value);

#define UART0_BASE 0x01c28000
#define UART1_BASE 0x01c28400
#define UART2_BASE 0x01c28800

void NS16550_puts(void *com_port, const char *s);
void NS16550_putc(void *com_port, char c);
char NS16550_getc(void *com_port);
int NS16550_tstc(void *com_port);

#endif

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
#include "drv_uart.h"

#include "interrupt.h"

struct device_uart
{
    rt_uint32_t hw_base;

    rt_uint32_t irqno;
    char name[RT_NAME_MAX];
};

static rt_err_t uart_configure          (struct rt_serial_device *serial, struct serial_configure *cfg);
static rt_err_t uart_control            (struct rt_serial_device *serial, int cmd, void *arg);
static int      uart_putc               (struct rt_serial_device *serial, char c);
static int      uart_getc               (struct rt_serial_device *serial);
static rt_size_t uart_dma_transmit      (struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction);

static void     uart_irq_handler        (int irqno, void *param);

const struct rt_uart_ops _uart_ops =
{
    uart_configure,
    uart_control,
    uart_putc,
    uart_getc,
    uart_dma_transmit
};


#define UART0_BASE      0x01c28000
#define UART_THR        0x00
#define UART_RHB        0x00
#define UART_USR        0x7c
#define UART_INTERRUPT  0X04
#define UART_INTT_STA   0x08

static struct rt_serial_device  serial1;

void uart_interrupt(void)
{
    rt_interrupt_enter();
    
    uart_irq_handler(0,&serial1);

    rt_interrupt_leave();
}



/*
 * UART Initiation
 */
int rt_hw_uart_init(void)
{
    volatile unsigned int * uart_addr; //串口中断开关地址

    struct rt_serial_device *serial;
    struct device_uart      *uart;
    struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;

    {
        
        static struct device_uart       uart1;

        serial  = &serial1;
        uart    = &uart1;

        serial->ops              = &_uart_ops;
        serial->config           = config;
        serial->config.baud_rate = 115200;

        uart->hw_base   = 0; // UART0_BASE;
        uart->irqno     = 0; // IRQ_UART0;

        rt_hw_serial_register(serial,
                              "uart",
                              RT_DEVICE_FLAG_RDWR | RT_DEVICE_FLAG_INT_RX,
                              uart);
    }

    /* 打开串口中断 */
    enable_interrupt(32,0,5);
    /* 设置串口中断服务函数 */
    register_interrupt_routine(32,uart_interrupt);

    /* 打开接收中断 */
    uart_addr = UART0_BASE + UART_INTERRUPT;
    *uart_addr |= 0x01;

    return 0;
}

/*
 * UART interface
 */
static rt_err_t uart_configure (struct rt_serial_device *serial, struct serial_configure *cfg)
{
     rt_uint32_t baud_div;
     struct device_uart * uart;

     RT_ASSERT(serial != RT_NULL);
     serial->config = *cfg;

     uart = serial->parent.user_data;
     RT_ASSERT(uart != RT_NULL);

     /* Init UART Hardware */

     /* Enable UART clock */

     /* Set both receiver and transmitter in UART mode (not SIR) */

     /* Set databits, stopbits and parity. (8-bit data, 1 stopbit, no parity) */

     /* set baudrate */

     return (RT_EOK);
}

static rt_err_t uart_control (struct rt_serial_device *serial, int cmd, void *arg)
{
    struct device_uart * uart;

    uart = serial->parent.user_data;

    RT_ASSERT(uart != RT_NULL);

    switch (cmd)
    {
    case RT_DEVICE_CTRL_CLR_INT:
        /* Disable the UART Interrupt */
        rt_hw_interrupt_mask(uart->irqno);
        break;

    case RT_DEVICE_CTRL_SET_INT:
        /* install interrupt */
        rt_hw_interrupt_install(uart->irqno, uart_irq_handler,
                                serial, uart->name);
        rt_hw_interrupt_umask(uart->irqno);

        /* Enable the UART Interrupt */
        break;
    }

    return (RT_EOK);
}

static int uart_putc (struct rt_serial_device *serial, char c)
{
    struct device_uart* uart;
    volatile unsigned char * sed_buf = UART0_BASE + UART_THR;
    volatile unsigned char * sta     = UART0_BASE + UART_USR;

    uart = serial->parent.user_data;

    /* FIFO status, contain valid data */
    while(!(*sta & 0x02)); //等待发送缓冲区非满
 //   while((*sta & 0x01));
    
    *sed_buf = c;

    /* write data */

    return (1);
}

static int uart_getc (struct rt_serial_device *serial)
{
    int ch;
    volatile unsigned int * p_temp;  //临时指针
    struct device_uart* uart = serial->parent.user_data;

    /* Receive Data Available */
    RT_ASSERT(serial != RT_NULL);

    ch = -1;
    p_temp = UART0_BASE + UART_USR;
    if(*p_temp & 0x08)
    {
            p_temp = UART0_BASE + UART_RHB;
            ch = *p_temp & 0xff;
    }

    return ch;
}

static rt_size_t uart_dma_transmit (struct rt_serial_device *serial, rt_uint8_t *buf, rt_size_t size, int direction)
{
    return (0);
}

/* UART ISR */
static void uart_irq_handler(int irqno, void *param)
{
    rt_ubase_t isr;
    struct rt_serial_device *serial = (struct rt_serial_device*)param;
    struct device_uart* uart = serial->parent.user_data;

    

    /* read interrupt status and clear it */
    if (1) /* rx ind */
    {
        rt_hw_serial_isr(serial,RT_SERIAL_EVENT_RX_IND);
    }

    if(0) /* tx done */
    {
        rt_hw_serial_isr(serial,RT_SERIAL_EVENT_TX_DONE);
    }
    
    rt_hw_interrupt_clear(32);

    

}

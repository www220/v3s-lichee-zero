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

struct lcdfb_device
{
    struct rt_device device; 
    struct rt_device_graphic_info info;
    int index;
    int bufsize;
    void* fb_address[2];
}; 

static rt_err_t _lcd_init(rt_device_t device)
{
    return RT_EOK; 
}

extern void sunxi_composer_fbbase_set(void* fbbase);
static rt_err_t _lcd_control(rt_device_t device, int cmd, void *args)
{
    struct lcdfb_device *lcdfb = (struct lcdfb_device *)device;
    RT_ASSERT(lcdfb != RT_NULL);

    switch(cmd)
    {
    case RTGRAPHIC_CTRL_GET_INFO:
        rt_memcpy(args, &lcdfb->info, sizeof(lcdfb->info)); 
        break;
    case RTGRAPHIC_CTRL_RECT_UPDATE:
        lcdfb->index = (lcdfb->index+1) % 2;
        rt_memcpy(lcdfb->fb_address[lcdfb->index], lcdfb->info.framebuffer, lcdfb->bufsize);
        sunxi_composer_fbbase_set(lcdfb->fb_address[lcdfb->index]);
        break;
    }

    return RT_EOK;
}

static struct lcdfb_device lcd; 
extern void *video_hw_init_probe(void* base, rt_uint8_t *deep, rt_uint16_t* width, rt_uint16_t* height);
int rt_hw_lcd_init(void)
{
    lcd.index = 0;
    lcd.fb_address[0] = RT_HW_HEAP_END;
    lcd.fb_address[1] = lcd.fb_address[0] + 0x400000;
    video_hw_init_probe(lcd.fb_address[0], &lcd.info.bits_per_pixel, &lcd.info.width, &lcd.info.height);
    lcd.info.pixel_format = (lcd.info.bits_per_pixel==16)?RTGRAPHIC_PIXEL_FORMAT_RGB565:RTGRAPHIC_PIXEL_FORMAT_RGB888;
    lcd.bufsize = RT_ALIGN(lcd.info.width * lcd.info.height * 4, 64);
    lcd.info.framebuffer = rt_malloc_align(lcd.bufsize, 64);

    lcd.device.type    = RT_Device_Class_Graphic; 
    lcd.device.init    = _lcd_init; 
    lcd.device.open    = RT_NULL;
    lcd.device.close   = RT_NULL;
    lcd.device.read    = RT_NULL;
    lcd.device.write   = RT_NULL;
    lcd.device.control = _lcd_control; 
    rt_device_register(&lcd.device, "lcd", RT_DEVICE_FLAG_RDWR); 

    return 0;
}
INIT_APP_EXPORT(rt_hw_lcd_init);

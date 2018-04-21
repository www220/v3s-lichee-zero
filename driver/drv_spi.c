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
#include "spi_flash.h"
#include "spi_flash_sfud.h"

struct hw_spi_bus
{
    uint32_t base;
    char *name;
    void * dev_ptr;
};
struct hw_spi_dev
{
    char *name;
    int cs;
};

extern int sunxi_spi_claim_bus(void *priv);
static rt_err_t spi_configure(struct rt_spi_device *device, struct rt_spi_configuration *configuration)
{
    struct hw_spi_bus *spi_bus = (struct hw_spi_bus *)device->bus->parent.user_data;
    struct hw_spi_dev *spi_dev = (struct hw_spi_dev *)device->parent.user_data;
    RT_ASSERT(spi_bus != RT_NULL);
    RT_ASSERT(spi_dev != RT_NULL);

    sunxi_spi_claim_bus(spi_bus->dev_ptr);
    return RT_EOK;
}

static rt_uint32_t spi_xfer(struct rt_spi_device *device, struct rt_spi_message *message)
{
    struct hw_spi_bus *spi_bus = (struct hw_spi_bus *)device->bus->parent.user_data;
    struct hw_spi_dev *spi_dev = (struct hw_spi_dev *)device->parent.user_data;
    RT_ASSERT(spi_bus != RT_NULL);
    RT_ASSERT(spi_dev != RT_NULL);

    return 0;
}

const struct rt_spi_ops _spi_ops =
{
    spi_configure,
    spi_xfer
};

static struct hw_spi_bus _spibus0_user =
{
    0x01c68000,
    "spibus",
};
static struct hw_spi_dev _spidev00_user =
{
    "spidev",
    23
};
static struct rt_spi_bus _spibus0;
static struct rt_spi_device _spidev00;

void spibus_pin_config(struct rt_spi_bus *bus, struct hw_spi_bus *spi)
{
    bus->parent.user_data = spi;
    rt_spi_bus_register(bus, spi->name, &_spi_ops);
}

void spidev_pin_config(const char *bus, struct rt_spi_device *dev, struct hw_spi_dev *spi)
{
    rt_spi_bus_attach_device(dev, spi->name, bus, spi);
}

extern int sunxi_spi_probe(const char *name, uint32_t reg, void **dev);
int rt_hw_spi_init(void)
{
    sunxi_spi_probe(_spibus0_user.name, _spibus0_user.base, &_spibus0_user.dev_ptr);
    spibus_pin_config(&_spibus0, &_spibus0_user);
    spidev_pin_config(_spibus0_user.name, &_spidev00, &_spidev00_user);

    return 0;
}
INIT_BOARD_EXPORT(rt_hw_spi_init);

int rt_hw_spi_flash_with_sfud_init(void)
{
    rt_spi_flash_device_t spi_device = rt_sfud_flash_probe("flash", _spidev00_user.name);
    if (spi_device == NULL)
    {
        rt_kprintf("failed to rt_hw_spi_flash_with_sfud_init\n");
        return -1;
    };

    return 0;
}
INIT_PREV_EXPORT(rt_hw_spi_flash_with_sfud_init);


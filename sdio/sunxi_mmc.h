/*
 * (C) Copyright 2007-2011
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Aaron <leafy.myeh@allwinnertech.com>
 *
 * MMC register definition for allwinner sunxi platform.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SUNXI_MMC_H
#define _SUNXI_MMC_H

struct sunxi_mmc {
	unsigned int gctrl;		/* 0x00 global control */
	unsigned int clkcr;		/* 0x04 clock control */
	unsigned int timeout;		/* 0x08 time out */
	unsigned int width;		/* 0x0c bus width */
	unsigned int blksz;		/* 0x10 block size */
	unsigned int bytecnt;		/* 0x14 byte count */
	unsigned int cmd;		/* 0x18 command */
	unsigned int arg;		/* 0x1c argument */
	unsigned int resp0;		/* 0x20 response 0 */
	unsigned int resp1;		/* 0x24 response 1 */
	unsigned int resp2;		/* 0x28 response 2 */
	unsigned int resp3;		/* 0x2c response 3 */
	unsigned int imask;		/* 0x30 interrupt mask */
	unsigned int mint;		/* 0x34 masked interrupt status */
	unsigned int rint;		/* 0x38 raw interrupt status */
	unsigned int status;		/* 0x3c status */
	unsigned int ftrglevel;		/* 0x40 FIFO threshold watermark*/
	unsigned int funcsel;		/* 0x44 function select */
	unsigned int cbcr;		/* 0x48 CIU byte count */
	unsigned int bbcr;		/* 0x4c BIU byte count */
	unsigned int dbgc;		/* 0x50 debug enable */
	unsigned int res0[11];
	unsigned int dmac;		/* 0x80 internal DMA control */
	unsigned int dlba;		/* 0x84 internal DMA descr list base address */
	unsigned int idst;		/* 0x88 internal DMA status */
	unsigned int idie;		/* 0x8c internal DMA interrupt enable */
	unsigned int chda;		/* 0x90 */
	unsigned int cbda;		/* 0x94 */
	unsigned int res1[26];
	unsigned int res2[64];
	unsigned int fifo;		/* 0x100 / 0x200 FIFO access address */
};

#define SUNXI_MMC_CLK_POWERSAVE		(0x1 << 17)
#define SUNXI_MMC_CLK_ENABLE		(0x1 << 16)
#define SUNXI_MMC_CLK_DIVIDER_MASK	(0xff)

#define SUNXI_MMC_GCTRL_SOFT_RESET	(0x1 << 0)
#define SUNXI_MMC_GCTRL_FIFO_RESET	(0x1 << 1)
#define SUNXI_MMC_GCTRL_DMA_RESET	(0x1 << 2)
#define SUNXI_MMC_GCTRL_RESET		(SUNXI_MMC_GCTRL_SOFT_RESET|\
					 SUNXI_MMC_GCTRL_FIFO_RESET|\
					 SUNXI_MMC_GCTRL_DMA_RESET)
#define SUNXI_MMC_GCTRL_DMA_ENABLE	(0x1 << 5)
#define SUNXI_MMC_GCTRL_ACCESS_BY_AHB   (0x1 << 31)

#define SUNXI_MMC_CMD_RESP_EXPIRE	(0x1 << 6)
#define SUNXI_MMC_CMD_LONG_RESPONSE	(0x1 << 7)
#define SUNXI_MMC_CMD_CHK_RESPONSE_CRC	(0x1 << 8)
#define SUNXI_MMC_CMD_DATA_EXPIRE	(0x1 << 9)
#define SUNXI_MMC_CMD_WRITE		(0x1 << 10)
#define SUNXI_MMC_CMD_AUTO_STOP		(0x1 << 12)
#define SUNXI_MMC_CMD_WAIT_PRE_OVER	(0x1 << 13)
#define SUNXI_MMC_CMD_SEND_INIT_SEQ	(0x1 << 15)
#define SUNXI_MMC_CMD_UPCLK_ONLY	(0x1 << 21)
#define SUNXI_MMC_CMD_START		(0x1 << 31)

#define SUNXI_MMC_RINT_RESP_ERROR		(0x1 << 1)
#define SUNXI_MMC_RINT_COMMAND_DONE		(0x1 << 2)
#define SUNXI_MMC_RINT_DATA_OVER		(0x1 << 3)
#define SUNXI_MMC_RINT_TX_DATA_REQUEST		(0x1 << 4)
#define SUNXI_MMC_RINT_RX_DATA_REQUEST		(0x1 << 5)
#define SUNXI_MMC_RINT_RESP_CRC_ERROR		(0x1 << 6)
#define SUNXI_MMC_RINT_DATA_CRC_ERROR		(0x1 << 7)
#define SUNXI_MMC_RINT_RESP_TIMEOUT		(0x1 << 8)
#define SUNXI_MMC_RINT_DATA_TIMEOUT		(0x1 << 9)
#define SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE	(0x1 << 10)
#define SUNXI_MMC_RINT_FIFO_RUN_ERROR		(0x1 << 11)
#define SUNXI_MMC_RINT_HARD_WARE_LOCKED		(0x1 << 12)
#define SUNXI_MMC_RINT_START_BIT_ERROR		(0x1 << 13)
#define SUNXI_MMC_RINT_AUTO_COMMAND_DONE	(0x1 << 14)
#define SUNXI_MMC_RINT_END_BIT_ERROR		(0x1 << 15)
#define SUNXI_MMC_RINT_SDIO_INTERRUPT		(0x1 << 16)
#define SUNXI_MMC_RINT_CARD_INSERT		(0x1 << 30)
#define SUNXI_MMC_RINT_CARD_REMOVE		(0x1 << 31)
#define SUNXI_MMC_RINT_INTERRUPT_ERROR_BIT      \
	(SUNXI_MMC_RINT_RESP_ERROR |		\
	 SUNXI_MMC_RINT_RESP_CRC_ERROR |	\
	 SUNXI_MMC_RINT_DATA_CRC_ERROR |	\
	 SUNXI_MMC_RINT_RESP_TIMEOUT |		\
	 SUNXI_MMC_RINT_DATA_TIMEOUT |		\
	 SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE |	\
	 SUNXI_MMC_RINT_FIFO_RUN_ERROR |	\
	 SUNXI_MMC_RINT_HARD_WARE_LOCKED |	\
	 SUNXI_MMC_RINT_START_BIT_ERROR |	\
	 SUNXI_MMC_RINT_END_BIT_ERROR) /* 0xbfc2 */
#define SUNXI_MMC_RINT_INTERRUPT_DONE_BIT	\
	(SUNXI_MMC_RINT_AUTO_COMMAND_DONE |	\
	 SUNXI_MMC_RINT_DATA_OVER |		\
	 SUNXI_MMC_RINT_COMMAND_DONE |		\
	 SUNXI_MMC_RINT_VOLTAGE_CHANGE_DONE)

#define SUNXI_MMC_STATUS_RXWL_FLAG		(0x1 << 0)
#define SUNXI_MMC_STATUS_TXWL_FLAG		(0x1 << 1)
#define SUNXI_MMC_STATUS_FIFO_EMPTY		(0x1 << 2)
#define SUNXI_MMC_STATUS_FIFO_FULL		(0x1 << 3)
#define SUNXI_MMC_STATUS_CARD_PRESENT		(0x1 << 8)
#define SUNXI_MMC_STATUS_CARD_DATA_BUSY		(0x1 << 9)
#define SUNXI_MMC_STATUS_DATA_FSM_BUSY		(0x1 << 10)

#define SUNXI_MMC_IDMAC_RESET		(0x1 << 0)
#define SUNXI_MMC_IDMAC_FIXBURST	(0x1 << 1)
#define SUNXI_MMC_IDMAC_ENABLE		(0x1 << 7)

#define SUNXI_MMC_IDIE_TXIRQ		(0x1 << 0)
#define SUNXI_MMC_IDIE_RXIRQ		(0x1 << 1)

#define SUNXI_MMC_COMMON_CLK_GATE		(1 << 16)
#define SUNXI_MMC_COMMON_RESET			(1 << 18)


void board_mmc_init(void);

#endif /* _SUNXI_MMC_H */

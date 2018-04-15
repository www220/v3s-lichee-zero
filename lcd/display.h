/*
 * Sunxi platform display controller register and constant defines
 *
 * (C) Copyright 2014 Hans de Goede <hdegoede@redhat.com>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _SUNXI_DISPLAY_H
#define _SUNXI_DISPLAY_H

struct sunxi_de_fe_reg {
	unsigned int enable;			/* 0x000 */
	unsigned int frame_ctrl;			/* 0x004 */
	unsigned int bypass;			/* 0x008 */
	unsigned int algorithm_sel;		/* 0x00c */
	unsigned int line_int_ctrl;		/* 0x010 */
	unsigned char res0[0x0c];			/* 0x014 */
	unsigned int ch0_addr;			/* 0x020 */
	unsigned int ch1_addr;			/* 0x024 */
	unsigned int ch2_addr;			/* 0x028 */
	unsigned int field_sequence;		/* 0x02c */
	unsigned int ch0_offset;			/* 0x030 */
	unsigned int ch1_offset;			/* 0x034 */
	unsigned int ch2_offset;			/* 0x038 */
	unsigned char res1[0x04];			/* 0x03c */
	unsigned int ch0_stride;			/* 0x040 */
	unsigned int ch1_stride;			/* 0x044 */
	unsigned int ch2_stride;			/* 0x048 */
	unsigned int input_fmt;			/* 0x04c */
	unsigned int ch3_addr;			/* 0x050 */
	unsigned int ch4_addr;			/* 0x054 */
	unsigned int ch5_addr;			/* 0x058 */
	unsigned int output_fmt;			/* 0x05c */
	unsigned int int_enable;			/* 0x060 */
	unsigned int int_status;			/* 0x064 */
	unsigned int status;			/* 0x068 */
	unsigned char res2[0x04];			/* 0x06c */
	unsigned int csc_coef00;			/* 0x070 */
	unsigned int csc_coef01;			/* 0x074 */
	unsigned int csc_coef02;			/* 0x078 */
	unsigned int csc_coef03;			/* 0x07c */
	unsigned int csc_coef10;			/* 0x080 */
	unsigned int csc_coef11;			/* 0x084 */
	unsigned int csc_coef12;			/* 0x088 */
	unsigned int csc_coef13;			/* 0x08c */
	unsigned int csc_coef20;			/* 0x090 */
	unsigned int csc_coef21;			/* 0x094 */
	unsigned int csc_coef22;			/* 0x098 */
	unsigned int csc_coef23;			/* 0x09c */
	unsigned int deinterlace_ctrl;		/* 0x0a0 */
	unsigned int deinterlace_diag;		/* 0x0a4 */
	unsigned int deinterlace_tempdiff;	/* 0x0a8 */
	unsigned int deinterlace_sawtooth;	/* 0x0ac */
	unsigned int deinterlace_spatcomp;	/* 0x0b0 */
	unsigned int deinterlace_burstlen;	/* 0x0b4 */
	unsigned int deinterlace_preluma;	/* 0x0b8 */
	unsigned int deinterlace_tile_addr;	/* 0x0bc */
	unsigned int deinterlace_tile_stride;	/* 0x0c0 */
	unsigned char res3[0x0c];			/* 0x0c4 */
	unsigned int wb_stride_enable;		/* 0x0d0 */
	unsigned int ch3_stride;			/* 0x0d4 */
	unsigned int ch4_stride;			/* 0x0d8 */
	unsigned int ch5_stride;			/* 0x0dc */
	unsigned int fe_3d_ctrl;			/* 0x0e0 */
	unsigned int fe_3d_ch0_addr;		/* 0x0e4 */
	unsigned int fe_3d_ch1_addr;		/* 0x0e8 */
	unsigned int fe_3d_ch2_addr;		/* 0x0ec */
	unsigned int fe_3d_ch0_offset;		/* 0x0f0 */
	unsigned int fe_3d_ch1_offset;		/* 0x0f4 */
	unsigned int fe_3d_ch2_offset;		/* 0x0f8 */
	unsigned char res4[0x04];			/* 0x0fc */
	unsigned int ch0_insize;			/* 0x100 */
	unsigned int ch0_outsize;		/* 0x104 */
	unsigned int ch0_horzfact;		/* 0x108 */
	unsigned int ch0_vertfact;		/* 0x10c */
	unsigned int ch0_horzphase;		/* 0x110 */
	unsigned int ch0_vertphase0;		/* 0x114 */
	unsigned int ch0_vertphase1;		/* 0x118 */
	unsigned char res5[0x04];			/* 0x11c */
	unsigned int ch0_horztapoffset0;		/* 0x120 */
	unsigned int ch0_horztapoffset1;		/* 0x124 */
	unsigned int ch0_verttapoffset;		/* 0x128 */
	unsigned char res6[0xd4];			/* 0x12c */
	unsigned int ch1_insize;			/* 0x200 */
	unsigned int ch1_outsize;		/* 0x204 */
	unsigned int ch1_horzfact;		/* 0x208 */
	unsigned int ch1_vertfact;		/* 0x20c */
	unsigned int ch1_horzphase;		/* 0x210 */
	unsigned int ch1_vertphase0;		/* 0x214 */
	unsigned int ch1_vertphase1;		/* 0x218 */
	unsigned char res7[0x04];			/* 0x21c */
	unsigned int ch1_horztapoffset0;		/* 0x220 */
	unsigned int ch1_horztapoffset1;		/* 0x224 */
	unsigned int ch1_verttapoffset;		/* 0x228 */
	unsigned char res8[0x1d4];			/* 0x22c */
	unsigned int ch0_horzcoef0[32];		/* 0x400 */
	unsigned int ch0_horzcoef1[32];		/* 0x480 */
	unsigned int ch0_vertcoef[32];		/* 0x500 */
	unsigned char res9[0x80];			/* 0x580 */
	unsigned int ch1_horzcoef0[32];		/* 0x600 */
	unsigned int ch1_horzcoef1[32];		/* 0x680 */
	unsigned int ch1_vertcoef[32];		/* 0x700 */
	unsigned char res10[0x280];		/* 0x780 */
	unsigned int vpp_enable;			/* 0xa00 */
	unsigned int vpp_dcti;			/* 0xa04 */
	unsigned int vpp_lp1;			/* 0xa08 */
	unsigned int vpp_lp2;			/* 0xa0c */
	unsigned int vpp_wle;			/* 0xa10 */
	unsigned int vpp_ble;			/* 0xa14 */
};

struct sunxi_de_be_reg {
	unsigned char res0[0x800];			/* 0x000 */
	unsigned int mode;			/* 0x800 */
	unsigned int backcolor;			/* 0x804 */
	unsigned int disp_size;			/* 0x808 */
	unsigned char res1[0x4];			/* 0x80c */
	unsigned int layer0_size;		/* 0x810 */
	unsigned int layer1_size;		/* 0x814 */
	unsigned int layer2_size;		/* 0x818 */
	unsigned int layer3_size;		/* 0x81c */
	unsigned int layer0_pos;			/* 0x820 */
	unsigned int layer1_pos;			/* 0x824 */
	unsigned int layer2_pos;			/* 0x828 */
	unsigned int layer3_pos;			/* 0x82c */
	unsigned char res2[0x10];			/* 0x830 */
	unsigned int layer0_stride;		/* 0x840 */
	unsigned int layer1_stride;		/* 0x844 */
	unsigned int layer2_stride;		/* 0x848 */
	unsigned int layer3_stride;		/* 0x84c */
	unsigned int layer0_addr_low32b;		/* 0x850 */
	unsigned int layer1_addr_low32b;		/* 0x854 */
	unsigned int layer2_addr_low32b;		/* 0x858 */
	unsigned int layer3_addr_low32b;		/* 0x85c */
	unsigned int layer0_addr_high4b;		/* 0x860 */
	unsigned int layer1_addr_high4b;		/* 0x864 */
	unsigned int layer2_addr_high4b;		/* 0x868 */
	unsigned int layer3_addr_high4b;		/* 0x86c */
	unsigned int reg_ctrl;			/* 0x870 */
	unsigned char res3[0xc];			/* 0x874 */
	unsigned int color_key_max;		/* 0x880 */
	unsigned int color_key_min;		/* 0x884 */
	unsigned int color_key_config;		/* 0x888 */
	unsigned char res4[0x4];			/* 0x88c */
	unsigned int layer0_attr0_ctrl;		/* 0x890 */
	unsigned int layer1_attr0_ctrl;		/* 0x894 */
	unsigned int layer2_attr0_ctrl;		/* 0x898 */
	unsigned int layer3_attr0_ctrl;		/* 0x89c */
	unsigned int layer0_attr1_ctrl;		/* 0x8a0 */
	unsigned int layer1_attr1_ctrl;		/* 0x8a4 */
	unsigned int layer2_attr1_ctrl;		/* 0x8a8 */
	unsigned int layer3_attr1_ctrl;		/* 0x8ac */
	unsigned char res5[0x110];			/* 0x8b0 */
	unsigned int output_color_ctrl;		/* 0x9c0 */
	unsigned char res6[0xc];			/* 0x9c4 */
	unsigned int output_color_coef[12];	/* 0x9d0 */
};

/*
 * The following structures are for DE2.
 * DE2 is a new generation of "composer" in some new Allwinner SoCs, such as
 * A83T, H3, V3s, A64.
 */

/* internal clock settings */
struct de_clk {
	unsigned int gate_cfg;
	unsigned int bus_cfg;
	unsigned int rst_cfg;
	unsigned int div_cfg;
	unsigned int sel_cfg;
};

/* global control */
struct de_glb {
	unsigned int ctl;
	unsigned int status;
	unsigned int dbuff;
	unsigned int size;
};

/* alpha blending */
struct de_bld {
	unsigned int fcolor_ctl;			/* 00 */
	struct {
		unsigned int fcolor;
		unsigned int insize;
		unsigned int offset;
		unsigned int dum;
	} attr[4];
	unsigned int dum0[15];			/* (end of clear offset) */
	unsigned int route;			/* 80 */
	unsigned int premultiply;
	unsigned int bkcolor;
	unsigned int output_size;
	unsigned int bld_mode[4];
	unsigned int dum1[4];
	unsigned int ck_ctl;			/* b0 */
	unsigned int ck_cfg;
	unsigned int dum2[2];
	unsigned int ck_max[4];			/* c0 */
	unsigned int dum3[4];
	unsigned int ck_min[4];			/* e0 */
	unsigned int dum4[3];
	unsigned int out_ctl;			/* fc */
};

/* VI channel */
struct de_vi {
	struct {
		unsigned int attr;
		unsigned int size;
		unsigned int coord;
		unsigned int pitch[3];
		unsigned int top_laddr[3];
		unsigned int bot_laddr[3];
	} cfg[4];
	unsigned int fcolor[4];			/* c0 */
	unsigned int top_haddr[3];		/* d0 */
	unsigned int bot_haddr[3];		/* dc */
	unsigned int ovl_size[2];		/* e8 */
	unsigned int hori[2];			/* f0 */
	unsigned int vert[2];			/* f8 */
};

struct de_ui {
	struct {
		unsigned int attr;
		unsigned int size;
		unsigned int coord;
		unsigned int pitch;
		unsigned int top_laddr;
		unsigned int bot_laddr;
		unsigned int fcolor;
		unsigned int dum;
	} cfg[4];			/* 00 */
	unsigned int top_haddr;			/* 80 */
	unsigned int bot_haddr;
	unsigned int ovl_size;			/* 88 */
};

struct sunxi_lcdc_reg {
	unsigned int ctrl;			/* 0x00 */
	unsigned int int0;			/* 0x04 */
	unsigned int int1;			/* 0x08 */
	unsigned char res0[0x04];			/* 0x0c */
	unsigned int tcon0_frm_ctrl;		/* 0x10 */
	unsigned int tcon0_frm_seed[6];		/* 0x14 */
	unsigned int tcon0_frm_table[4];		/* 0x2c */
	unsigned char res1[4];			/* 0x3c */
	unsigned int tcon0_ctrl;			/* 0x40 */
	unsigned int tcon0_dclk;			/* 0x44 */
	unsigned int tcon0_timing_active;	/* 0x48 */
	unsigned int tcon0_timing_h;		/* 0x4c */
	unsigned int tcon0_timing_v;		/* 0x50 */
	unsigned int tcon0_timing_sync;		/* 0x54 */
	unsigned int tcon0_hv_intf;		/* 0x58 */
	unsigned char res2[0x04];			/* 0x5c */
	unsigned int tcon0_cpu_intf;		/* 0x60 */
	unsigned int tcon0_cpu_wr_dat;		/* 0x64 */
	unsigned int tcon0_cpu_rd_dat0;		/* 0x68 */
	unsigned int tcon0_cpu_rd_dat1;		/* 0x6c */
	unsigned int tcon0_ttl_timing0;		/* 0x70 */
	unsigned int tcon0_ttl_timing1;		/* 0x74 */
	unsigned int tcon0_ttl_timing2;		/* 0x78 */
	unsigned int tcon0_ttl_timing3;		/* 0x7c */
	unsigned int tcon0_ttl_timing4;		/* 0x80 */
	unsigned int tcon0_lvds_intf;		/* 0x84 */
	unsigned int tcon0_io_polarity;		/* 0x88 */
	unsigned int tcon0_io_tristate;		/* 0x8c */
	unsigned int tcon1_ctrl;			/* 0x90 */
	unsigned int tcon1_timing_source;	/* 0x94 */
	unsigned int tcon1_timing_scale;		/* 0x98 */
	unsigned int tcon1_timing_out;		/* 0x9c */
	unsigned int tcon1_timing_h;		/* 0xa0 */
	unsigned int tcon1_timing_v;		/* 0xa4 */
	unsigned int tcon1_timing_sync;		/* 0xa8 */
	unsigned char res3[0x44];			/* 0xac */
	unsigned int tcon1_io_polarity;		/* 0xf0 */
	unsigned int tcon1_io_tristate;		/* 0xf4 */
	unsigned char res4[0x108];			/* 0xf8 */
	unsigned int mux_ctrl;			/* 0x200 */
	unsigned char res5[0x1c];			/* 0x204 */
	unsigned int lvds_ana0;			/* 0x220 */
	unsigned int lvds_ana1;			/* 0x224 */
};

struct sunxi_hdmi_reg {
	unsigned int version_id;			/* 0x000 */
	unsigned int ctrl;			/* 0x004 */
	unsigned int irq;			/* 0x008 */
	unsigned int hpd;			/* 0x00c */
	unsigned int video_ctrl;			/* 0x010 */
	unsigned int video_size;			/* 0x014 */
	unsigned int video_bp;			/* 0x018 */
	unsigned int video_fp;			/* 0x01c */
	unsigned int video_spw;			/* 0x020 */
	unsigned int video_polarity;		/* 0x024 */
	unsigned char res0[0x58];			/* 0x028 */
	unsigned char avi_info_frame[0x14];	/* 0x080 */
	unsigned char res1[0x4c];			/* 0x094 */
	unsigned int qcp_packet0;		/* 0x0e0 */
	unsigned int qcp_packet1;		/* 0x0e4 */
	unsigned char res2[0x118];			/* 0x0e8 */
	unsigned int pad_ctrl0;			/* 0x200 */
	unsigned int pad_ctrl1;			/* 0x204 */
	unsigned int pll_ctrl;			/* 0x208 */
	unsigned int pll_dbg0;			/* 0x20c */
	unsigned int pll_dbg1;			/* 0x210 */
	unsigned int hpd_cec;			/* 0x214 */
	unsigned char res3[0x28];			/* 0x218 */
	unsigned char vendor_info_frame[0x14];	/* 0x240 */
	unsigned char res4[0x9c];			/* 0x254 */
	unsigned int pkt_ctrl0;			/* 0x2f0 */
	unsigned int pkt_ctrl1;			/* 0x2f4 */
	unsigned char res5[0x8];			/* 0x2f8 */
	unsigned int unknown;			/* 0x300 */
	unsigned char res6[0xc];			/* 0x304 */
	unsigned int audio_sample_count;		/* 0x310 */
	unsigned char res7[0xec];			/* 0x314 */
	unsigned int audio_tx_fifo;		/* 0x400 */
	unsigned char res8[0xfc];			/* 0x404 */
#ifndef CONFIG_MACH_SUN6I
	unsigned int ddc_ctrl;			/* 0x500 */
	unsigned int ddc_addr;			/* 0x504 */
	unsigned int ddc_int_mask;		/* 0x508 */
	unsigned int ddc_int_status;		/* 0x50c */
	unsigned int ddc_fifo_ctrl;		/* 0x510 */
	unsigned int ddc_fifo_status;		/* 0x514 */
	unsigned int ddc_fifo_data;		/* 0x518 */
	unsigned int ddc_byte_count;		/* 0x51c */
	unsigned int ddc_cmnd;			/* 0x520 */
	unsigned int ddc_exreg;			/* 0x524 */
	unsigned int ddc_clock;			/* 0x528 */
	unsigned char res9[0x14];			/* 0x52c */
	unsigned int ddc_line_ctrl;		/* 0x540 */
#else
	unsigned int ddc_ctrl;			/* 0x500 */
	unsigned int ddc_exreg;			/* 0x504 */
	unsigned int ddc_cmnd;			/* 0x508 */
	unsigned int ddc_addr;			/* 0x50c */
	unsigned int ddc_int_mask;		/* 0x510 */
	unsigned int ddc_int_status;		/* 0x514 */
	unsigned int ddc_fifo_ctrl;		/* 0x518 */
	unsigned int ddc_fifo_status;		/* 0x51c */
	unsigned int ddc_clock;			/* 0x520 */
	unsigned int ddc_timeout;		/* 0x524 */
	unsigned char res9[0x18];			/* 0x528 */
	unsigned int ddc_dbg;			/* 0x540 */
	unsigned char res10[0x3c];			/* 0x544 */
	unsigned int ddc_fifo_data;		/* 0x580 */
#endif
};

/*
 * This is based on the A10s User Manual, and the A10s only supports
 * composite video and not vga like the A10 / A20 does, still other
 * than the removed vga out capability the tvencoder seems to be the same.
 * "unknown#" registers are registers which are used in the A10 kernel code,
 * but not documented in the A10s User Manual.
 */
struct sunxi_tve_reg {
	unsigned int gctrl;			/* 0x000 */
	unsigned int cfg0;			/* 0x004 */
	unsigned int dac_cfg0;			/* 0x008 */
	unsigned int filter;			/* 0x00c */
	unsigned int chroma_freq;		/* 0x010 */
	unsigned int porch_num;			/* 0x014 */
	unsigned int unknown0;			/* 0x018 */
	unsigned int line_num;			/* 0x01c */
	unsigned int blank_black_level;		/* 0x020 */
	unsigned int unknown1;			/* 0x024, seems to be 1 byte per dac */
	unsigned char res0[0x08];			/* 0x028 */
	unsigned int auto_detect_en;		/* 0x030 */
	unsigned int auto_detect_int_status;	/* 0x034 */
	unsigned int auto_detect_status;		/* 0x038 */
	unsigned int auto_detect_debounce;	/* 0x03c */
	unsigned int csc_reg0;			/* 0x040 */
	unsigned int csc_reg1;			/* 0x044 */
	unsigned int csc_reg2;			/* 0x048 */
	unsigned int csc_reg3;			/* 0x04c */
	unsigned char res1[0xb0];			/* 0x050 */
	unsigned int color_burst;		/* 0x100 */
	unsigned int vsync_num;			/* 0x104 */
	unsigned int notch_freq;			/* 0x108 */
	unsigned int cbr_level;			/* 0x10c */
	unsigned int burst_phase;		/* 0x110 */
	unsigned int burst_width;		/* 0x114 */
	unsigned int unknown2;			/* 0x118 */
	unsigned int sync_vbi_level;		/* 0x11c */
	unsigned int white_level;		/* 0x120 */
	unsigned int active_num;			/* 0x124 */
	unsigned int chroma_bw_gain;		/* 0x128 */
	unsigned int notch_width;		/* 0x12c */
	unsigned int resync_num;			/* 0x130 */
	unsigned int slave_para;			/* 0x134 */
	unsigned int cfg1;			/* 0x138 */
	unsigned int cfg2;			/* 0x13c */
};

/*
 * DE-FE register constants.
 */
#define SUNXI_DE_FE_WIDTH(x)			(((x) - 1) << 0)
#define SUNXI_DE_FE_HEIGHT(y)			(((y) - 1) << 16)
#define SUNXI_DE_FE_FACTOR_INT(n)		((n) << 16)
#define SUNXI_DE_FE_ENABLE_EN			(1 << 0)
#define SUNXI_DE_FE_FRAME_CTRL_REG_RDY		(1 << 0)
#define SUNXI_DE_FE_FRAME_CTRL_COEF_RDY		(1 << 1)
#define SUNXI_DE_FE_FRAME_CTRL_FRM_START	(1 << 16)
#define SUNXI_DE_FE_BYPASS_CSC_BYPASS		(1 << 1)
#define SUNXI_DE_FE_INPUT_FMT_ARGB8888		0x00000151
#define SUNXI_DE_FE_OUTPUT_FMT_ARGB8888		0x00000002

/*
 * DE-BE register constants.
 */
#define SUNXI_DE_BE_WIDTH(x)			(((x) - 1) << 0)
#define SUNXI_DE_BE_HEIGHT(y)			(((y) - 1) << 16)
#define SUNXI_DE_BE_MODE_ENABLE			(1 << 0)
#define SUNXI_DE_BE_MODE_START			(1 << 1)
#define SUNXI_DE_BE_MODE_DEFLICKER_ENABLE	(1 << 4)
#define SUNXI_DE_BE_MODE_LAYER0_ENABLE		(1 << 8)
#define SUNXI_DE_BE_MODE_INTERLACE_ENABLE	(1 << 28)
#define SUNXI_DE_BE_LAYER_STRIDE(x)		((x) << 5)
#define SUNXI_DE_BE_REG_CTRL_LOAD_REGS		(1 << 0)
#define SUNXI_DE_BE_LAYER_ATTR0_SRC_FE0		0x00000002
#define SUNXI_DE_BE_LAYER_ATTR1_FMT_XRGB8888	(0x09 << 8)
#define SUNXI_DE_BE_OUTPUT_COLOR_CTRL_ENABLE	1

/*
 * DE2 register constants.
 */
#define SUNXI_DE2_MUX0_BASE			(unsigned char *)(SUNXI_DE2_BASE + 0x100000)

#define SUNXI_DE2_MUX_GLB_REGS			0x00000
#define SUNXI_DE2_MUX_BLD_REGS			0x01000
#define SUNXI_DE2_MUX_CHAN_REGS			0x02000
#define	SUNXI_DE2_MUX_CHAN_SZ			0x1000
#define SUNXI_DE2_MUX_VSU_REGS			0x20000
#define SUNXI_DE2_MUX_GSU1_REGS			0x30000
#define SUNXI_DE2_MUX_GSU2_REGS			0x40000
#define SUNXI_DE2_MUX_GSU3_REGS			0x50000
#define SUNXI_DE2_MUX_FCE_REGS			0xa0000
#define SUNXI_DE2_MUX_BWS_REGS			0xa2000
#define SUNXI_DE2_MUX_LTI_REGS			0xa4000
#define SUNXI_DE2_MUX_PEAK_REGS			0xa6000
#define SUNXI_DE2_MUX_ASE_REGS			0xa8000
#define SUNXI_DE2_MUX_FCC_REGS			0xaa000
#define SUNXI_DE2_MUX_DCSC_REGS			0xb0000

#define SUNXI_DE2_FORMAT_ARGB_8888		0
#define SUNXI_DE2_FORMAT_BGRA_8888		3
#define SUNXI_DE2_FORMAT_XRGB_8888		4
#define SUNXI_DE2_FORMAT_RGB_888		8
#define SUNXI_DE2_FORMAT_BGR_888		9

#define SUNXI_DE2_MUX_GLB_CTL_RT_EN		(1 << 0)

#define SUNXI_DE2_UI_CFG_ATTR_EN		(1 << 0)
#define SUNXI_DE2_UI_CFG_ATTR_ALPMOD(m)		((m & 3) << 1)
#define SUNXI_DE2_UI_CFG_ATTR_FMT(f)		((f & 0xf) << 8)
#define SUNXI_DE2_UI_CFG_ATTR_ALPHA(a)		((a & 0xff) << 24)

#define SUNXI_DE2_WH(w, h)			(((h - 1) << 16) | (w - 1))

/*
 * LCDC register constants.
 */
#define SUNXI_LCDC_X(x)				(((x) - 1) << 16)
#define SUNXI_LCDC_Y(y)				(((y) - 1) << 0)
#define SUNXI_LCDC_TCON_VSYNC_MASK		(1 << 24)
#define SUNXI_LCDC_TCON_HSYNC_MASK		(1 << 25)
#define SUNXI_LCDC_CTRL_IO_MAP_MASK		(1 << 0)
#define SUNXI_LCDC_CTRL_IO_MAP_TCON0		(0 << 0)
#define SUNXI_LCDC_CTRL_IO_MAP_TCON1		(1 << 0)
#define SUNXI_LCDC_CTRL_TCON_ENABLE		(1 << 31)
#define SUNXI_LCDC_TCON0_FRM_CTRL_RGB666	((1 << 31) | (0 << 4))
#define SUNXI_LCDC_TCON0_FRM_CTRL_RGB565	((1 << 31) | (5 << 4))
#define SUNXI_LCDC_TCON0_FRM_SEED		0x11111111
#define SUNXI_LCDC_TCON0_FRM_TAB0		0x01010000
#define SUNXI_LCDC_TCON0_FRM_TAB1		0x15151111
#define SUNXI_LCDC_TCON0_FRM_TAB2		0x57575555
#define SUNXI_LCDC_TCON0_FRM_TAB3		0x7f7f7777
#define SUNXI_LCDC_TCON0_CTRL_CLK_DELAY(n)	(((n) & 0x1f) << 4)
#define SUNXI_LCDC_TCON0_CTRL_ENABLE		(1 << 31)
#define SUNXI_LCDC_TCON0_DCLK_DIV(n)		((n) << 0)
#define SUNXI_LCDC_TCON0_DCLK_ENABLE		(0xf << 28)
#define SUNXI_LCDC_TCON0_TIMING_H_BP(n)		(((n) - 1) << 0)
#define SUNXI_LCDC_TCON0_TIMING_H_TOTAL(n)	(((n) - 1) << 16)
#define SUNXI_LCDC_TCON0_TIMING_V_BP(n)		(((n) - 1) << 0)
#define SUNXI_LCDC_TCON0_TIMING_V_TOTAL(n)	(((n) * 2) << 16)
#ifdef CONFIG_SUNXI_GEN_SUN6I
#define SUNXI_LCDC_TCON0_LVDS_CLK_SEL_TCON0	(1 << 20)
#else
#define SUNXI_LCDC_TCON0_LVDS_CLK_SEL_TCON0	0 /* NA */
#endif
#define SUNXI_LCDC_TCON0_LVDS_INTF_BITWIDTH(n)	((n) << 26)
#define SUNXI_LCDC_TCON0_LVDS_INTF_ENABLE	(1 << 31)
#define SUNXI_LCDC_TCON0_IO_POL_DCLK_PHASE(x)	((x) << 28)
#define SUNXI_LCDC_TCON1_CTRL_CLK_DELAY(n)	(((n) & 0x1f) << 4)
#define SUNXI_LCDC_TCON1_CTRL_INTERLACE_ENABLE	(1 << 20)
#define SUNXI_LCDC_TCON1_CTRL_ENABLE		(1 << 31)
#define SUNXI_LCDC_TCON1_TIMING_H_BP(n)		(((n) - 1) << 0)
#define SUNXI_LCDC_TCON1_TIMING_H_TOTAL(n)	(((n) - 1) << 16)
#define SUNXI_LCDC_TCON1_TIMING_V_BP(n)		(((n) - 1) << 0)
#define SUNXI_LCDC_TCON1_TIMING_V_TOTAL(n)	((n) << 16)
#define SUNXI_LCDC_MUX_CTRL_SRC0_MASK		(0xf << 0)
#define SUNXI_LCDC_MUX_CTRL_SRC0(x)		((x) << 0)
#define SUNXI_LCDC_MUX_CTRL_SRC1_MASK		(0xf << 4)
#define SUNXI_LCDC_MUX_CTRL_SRC1(x)		((x) << 4)
#ifdef CONFIG_SUNXI_GEN_SUN6I
#define SUNXI_LCDC_LVDS_ANA0			0x40040320
#define SUNXI_LCDC_LVDS_ANA0_EN_MB		(1 << 31)
#define SUNXI_LCDC_LVDS_ANA0_DRVC		(1 << 24)
#define SUNXI_LCDC_LVDS_ANA0_DRVD(x)		((x) << 20)
#else
#define SUNXI_LCDC_LVDS_ANA0			0x3f310000
#define SUNXI_LCDC_LVDS_ANA0_UPDATE		(1 << 22)
#endif
#define SUNXI_LCDC_LVDS_ANA1_INIT1		(0x1f << 26 | 0x1f << 10)
#define SUNXI_LCDC_LVDS_ANA1_INIT2		(0x1f << 16 | 0x1f << 00)

/*
 * HDMI register constants.
 */
#define SUNXI_HDMI_X(x)				(((x) - 1) << 0)
#define SUNXI_HDMI_Y(y)				(((y) - 1) << 16)
#define SUNXI_HDMI_CTRL_ENABLE			(1 << 31)
#define SUNXI_HDMI_IRQ_STATUS_FIFO_UF		(1 << 0)
#define SUNXI_HDMI_IRQ_STATUS_FIFO_OF		(1 << 1)
#define SUNXI_HDMI_IRQ_STATUS_BITS		0x73
#define SUNXI_HDMI_HPD_DETECT			(1 << 0)
#define SUNXI_HDMI_VIDEO_CTRL_ENABLE		(1 << 31)
#define SUNXI_HDMI_VIDEO_CTRL_HDMI		(1 << 30)
#define SUNXI_HDMI_VIDEO_POL_HOR		(1 << 0)
#define SUNXI_HDMI_VIDEO_POL_VER		(1 << 1)
#define SUNXI_HDMI_VIDEO_POL_TX_CLK		(0x3e0 << 16)
#define SUNXI_HDMI_QCP_PACKET0			3
#define SUNXI_HDMI_QCP_PACKET1			0

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HDMI_PAD_CTRL0_HDP		0x7e80000f
#define SUNXI_HDMI_PAD_CTRL0_RUN		0x7e8000ff
#else
#define SUNXI_HDMI_PAD_CTRL0_HDP		0xfe800000
#define SUNXI_HDMI_PAD_CTRL0_RUN		0xfe800000
#endif

#ifdef CONFIG_MACH_SUN4I
#define SUNXI_HDMI_PAD_CTRL1			0x00d8c820
#elif defined CONFIG_MACH_SUN6I
#define SUNXI_HDMI_PAD_CTRL1			0x01ded030
#else
#define SUNXI_HDMI_PAD_CTRL1			0x00d8c830
#endif
#define SUNXI_HDMI_PAD_CTRL1_HALVE		(1 << 6)

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HDMI_PLL_CTRL			0xba48a308
#define SUNXI_HDMI_PLL_CTRL_DIV(n)		(((n) - 1) << 4)
#else
#define SUNXI_HDMI_PLL_CTRL			0xfa4ef708
#define SUNXI_HDMI_PLL_CTRL_DIV(n)		((n) << 4)
#endif
#define SUNXI_HDMI_PLL_CTRL_DIV_MASK		(0xf << 4)

#define SUNXI_HDMI_PLL_DBG0_PLL3		(0 << 21)
#define SUNXI_HDMI_PLL_DBG0_PLL7		(1 << 21)

#define SUNXI_HDMI_PKT_CTRL0			0x00000f21
#define SUNXI_HDMI_PKT_CTRL1			0x0000000f
#define SUNXI_HDMI_UNKNOWN_INPUT_SYNC		0x08000000

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HMDI_DDC_CTRL_ENABLE		(1 << 0)
#define SUNXI_HMDI_DDC_CTRL_SCL_ENABLE		(1 << 4)
#define SUNXI_HMDI_DDC_CTRL_SDA_ENABLE		(1 << 6)
#define SUNXI_HMDI_DDC_CTRL_START		(1 << 27)
#define SUNXI_HMDI_DDC_CTRL_RESET		(1 << 31)
#else
#define SUNXI_HMDI_DDC_CTRL_RESET		(1 << 0)
/* sun4i / sun5i / sun7i do not have a separate line_ctrl reg */
#define SUNXI_HMDI_DDC_CTRL_SDA_ENABLE		0
#define SUNXI_HMDI_DDC_CTRL_SCL_ENABLE		0
#define SUNXI_HMDI_DDC_CTRL_START		(1 << 30)
#define SUNXI_HMDI_DDC_CTRL_ENABLE		(1 << 31)
#endif

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HMDI_DDC_ADDR_SLAVE_ADDR		(0xa0 << 0)
#else
#define SUNXI_HMDI_DDC_ADDR_SLAVE_ADDR		(0x50 << 0)
#endif
#define SUNXI_HMDI_DDC_ADDR_OFFSET(n)		(((n) & 0xff) << 8)
#define SUNXI_HMDI_DDC_ADDR_EDDC_ADDR		(0x60 << 16)
#define SUNXI_HMDI_DDC_ADDR_EDDC_SEGMENT(n)	((n) << 24)

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HDMI_DDC_FIFO_CTRL_CLEAR		(1 << 15)
#else
#define SUNXI_HDMI_DDC_FIFO_CTRL_CLEAR		(1 << 31)
#endif

#define SUNXI_HDMI_DDC_CMND_EXPLICIT_EDDC_READ	6
#define SUNXI_HDMI_DDC_CMND_IMPLICIT_EDDC_READ	7

#ifdef CONFIG_MACH_SUN6I
#define SUNXI_HDMI_DDC_CLOCK			0x61
#else
/* N = 5,M=1 Fscl= Ftmds/2/10/2^N/(M+1) */
#define SUNXI_HDMI_DDC_CLOCK			0x0d
#endif

#define SUNXI_HMDI_DDC_LINE_CTRL_SCL_ENABLE	(1 << 8)
#define SUNXI_HMDI_DDC_LINE_CTRL_SDA_ENABLE	(1 << 9)

/*
 * TVE register constants.
 */
#define SUNXI_TVE_GCTRL_ENABLE			(1 << 0)
/*
 * Select input 0 to disable dac, 1 - 4 to feed dac from tve0, 5 - 8 to feed
 * dac from tve1. When using tve1 the mux value must be written to both tve0's
 * and tve1's gctrl reg.
 */
#define SUNXI_TVE_GCTRL_DAC_INPUT_MASK(dac)	(0xf << (((dac) + 1) * 4))
#define SUNXI_TVE_GCTRL_DAC_INPUT(dac, sel)	((sel) << (((dac) + 1) * 4))
#define SUNXI_TVE_CFG0_VGA			0x20000000
#define SUNXI_TVE_CFG0_PAL			0x07030001
#define SUNXI_TVE_CFG0_NTSC			0x07030000
#define SUNXI_TVE_DAC_CFG0_VGA			0x403e1ac7
#ifdef CONFIG_MACH_SUN5I
#define SUNXI_TVE_DAC_CFG0_COMPOSITE		0x433f0009
#else
#define SUNXI_TVE_DAC_CFG0_COMPOSITE		0x403f0008
#endif
#define SUNXI_TVE_FILTER_COMPOSITE		0x00000120
#define SUNXI_TVE_CHROMA_FREQ_PAL_M		0x21e6efe3
#define SUNXI_TVE_CHROMA_FREQ_PAL_NC		0x21f69446
#define SUNXI_TVE_PORCH_NUM_PAL			0x008a0018
#define SUNXI_TVE_PORCH_NUM_NTSC		0x00760020
#define SUNXI_TVE_LINE_NUM_PAL			0x00160271
#define SUNXI_TVE_LINE_NUM_NTSC			0x0016020d
#define SUNXI_TVE_BLANK_BLACK_LEVEL_PAL		0x00fc00fc
#define SUNXI_TVE_BLANK_BLACK_LEVEL_NTSC	0x00f0011a
#define SUNXI_TVE_UNKNOWN1_VGA			0x00000000
#define SUNXI_TVE_UNKNOWN1_COMPOSITE		0x18181818
#define SUNXI_TVE_AUTO_DETECT_EN_DET_EN(dac)	(1 << ((dac) + 0))
#define SUNXI_TVE_AUTO_DETECT_EN_INT_EN(dac)	(1 << ((dac) + 16))
#define SUNXI_TVE_AUTO_DETECT_INT_STATUS(dac)	(1 << ((dac) + 0))
#define SUNXI_TVE_AUTO_DETECT_STATUS_SHIFT(dac)	((dac) * 8)
#define SUNXI_TVE_AUTO_DETECT_STATUS_MASK(dac)	(3 << ((dac) * 8))
#define SUNXI_TVE_AUTO_DETECT_STATUS_NONE	0
#define SUNXI_TVE_AUTO_DETECT_STATUS_CONNECTED	1
#define SUNXI_TVE_AUTO_DETECT_STATUS_SHORT_GND	3
#define SUNXI_TVE_AUTO_DETECT_DEBOUNCE_SHIFT(d)	((d) * 8)
#define SUNXI_TVE_AUTO_DETECT_DEBOUNCE_MASK(d)	(0xf << ((d) * 8))
#define SUNXI_TVE_CSC_REG0_ENABLE		(1 << 31)
#define SUNXI_TVE_CSC_REG0			0x08440832
#define SUNXI_TVE_CSC_REG1			0x3b6dace1
#define SUNXI_TVE_CSC_REG2			0x0e1d13dc
#define SUNXI_TVE_CSC_REG3			0x00108080
#define SUNXI_TVE_COLOR_BURST_PAL_M		0x00000000
#define SUNXI_TVE_CBR_LEVEL_PAL			0x00002828
#define SUNXI_TVE_CBR_LEVEL_NTSC		0x0000004f
#define SUNXI_TVE_BURST_PHASE_NTSC		0x00000000
#define SUNXI_TVE_BURST_WIDTH_COMPOSITE		0x0016447e
#define SUNXI_TVE_UNKNOWN2_PAL			0x0000e0e0
#define SUNXI_TVE_UNKNOWN2_NTSC			0x0000a0a0
#define SUNXI_TVE_SYNC_VBI_LEVEL_NTSC		0x001000f0
#define SUNXI_TVE_ACTIVE_NUM_COMPOSITE		0x000005a0
#define SUNXI_TVE_CHROMA_BW_GAIN_COMP		0x00000002
#define SUNXI_TVE_NOTCH_WIDTH_COMPOSITE		0x00000101
#define SUNXI_TVE_RESYNC_NUM_PAL		0x800d000c
#define SUNXI_TVE_RESYNC_NUM_NTSC		0x000e000c
#define SUNXI_TVE_SLAVE_PARA_COMPOSITE		0x00000000

#define SUNXI_LCD0_BASE			0x01c0C000
#define SUNXI_DE2_BASE			0x01000000


int sunxi_simplefb_setup(void *blob);

#endif /* _SUNXI_DISPLAY_H */

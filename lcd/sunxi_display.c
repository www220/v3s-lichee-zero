#include "clock_sun6i.h"
#include "display.h"
#include "io.h"
#include "string.h"
#include "stdbool.h"

#include "rtthread.h"
#include <rtdevice.h>

#define PWM_ON 1
#define PWM_OFF 0

#define TYX_DEGUB_(v) rt_kprintf("test after addr=0x%08x value=0x%08x \n",&(v), (v));
#define GPIO_BASE      (0x01C20800)

struct ctfb_res_modes {
	int xres;		/* visible resolution		*/
	int yres;
	int refresh;		/* vertical refresh rate in hz  */
	/* Timing: All values in pixclocks, except pixclock (of course) */
	int pixclock;		/* pixel clock in ps (pico seconds) */
	int pixclock_khz;	/* pixel clock in kHz           */
	int left_margin;	/* time from sync to picture	*/
	int right_margin;	/* time from picture to sync	*/
	int upper_margin;	/* time from sync to picture	*/
	int lower_margin;
	int hsync_len;		/* length of horizontal sync	*/
	int vsync_len;		/* length of vertical sync	*/
	int sync;		/* see FB_SYNC_*		*/
	int vmode;		/* see FB_VMODE_*		*/
};

typedef struct graphic_device {
    unsigned int isaBase;
    unsigned int pciBase;
    unsigned int dprBase;
    unsigned int vprBase;
    unsigned int cprBase;
    unsigned int frameAdrs;
    unsigned int memSize;
    unsigned int mode;
    unsigned int gdfIndex;
    unsigned int gdfBytesPP;
    unsigned int fg;
    unsigned int bg;
    unsigned int plnSizeX;
    unsigned int plnSizeY;
    unsigned int winSizeX;
    unsigned int winSizeY;
    char modeIdent[80];
} GraphicDevice;

enum sunxi_monitor {
	sunxi_monitor_none,
	sunxi_monitor_dvi,
	sunxi_monitor_hdmi,
	sunxi_monitor_lcd,
	sunxi_monitor_vga,
	sunxi_monitor_composite_pal,
	sunxi_monitor_composite_ntsc,
	sunxi_monitor_composite_pal_m,
	sunxi_monitor_composite_pal_nc,
};

struct sunxi_display {
	GraphicDevice graphic_device;
	enum sunxi_monitor monitor;
	unsigned int depth;
	unsigned int fb_addr;
	unsigned int fb_size;
} sunxi_display;




static void sunxi_composer_init(void)
{
	//struct sunxi_ccm_reg 结构体在clock_sun6i.h中定义
    
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
    
    //配置104  DE寄存器
	clock_set_de2_mod_clock(&ccm->de_clk_cfg, 300000000);

	/* Set ahb gating to pass */
	//setbits_le32(&ccm->ahb_reset1_cfg, 1 << AHB_RESET_OFFSET_DE);
    //解除复位DE时钟开关
	setbits_le32(&ccm->ahb_reset1_cfg, 0x00001000);
    //打开DE时钟
	setbits_le32(&ccm->ahb_gate1, 1 << AHB_GATE_OFFSET_DE);
    
    //DE使能  1<<31
	/* Clock on */
	setbits_le32(&ccm->de_clk_cfg, CCM_DE2_CTRL_GATE);
}

static void sunxi_lcdc_init(void)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	struct sunxi_lcdc_reg * const lcdc =
		(struct sunxi_lcdc_reg *)SUNXI_LCD0_BASE;
	
    //lcd 解除复位
	setbits_le32(&ccm->ahb_reset1_cfg, 1 << AHB_RESET_OFFSET_LCD0);
    //打开lcd时钟开关
	setbits_le32(&ccm->ahb_gate1, 1 << AHB_GATE_OFFSET_LCD0);
	
    //先禁止TCON寄存器,等配置好了再打开
	writel(0, &lcdc->ctrl); /* Disable tcon */
    //禁止TCON所有中断
	writel(0, &lcdc->int0); /* Disable all interrupts */

    
	/* Disable tcon0 dot clock */
    //这个东西  暂时还不知道怎么处理 先放一手
	clrbits_le32(&lcdc->tcon0_dclk, SUNXI_LCDC_TCON0_DCLK_ENABLE);

	/* Set all io lines to tristate */
    //TCON0 IO Control 禁止
	writel(0xffffffff, &lcdc->tcon0_io_tristate);
    //TCON1 IO Control 禁止
	writel(0xffffffff, &lcdc->tcon1_io_tristate);
}

void sunxi_drc_init(void)
{
	
}


static void sunxi_engines_init(void)
{
	sunxi_composer_init();
	sunxi_lcdc_init();
	sunxi_drc_init();
}



/* 第二部分 */
void sunxi_lcdc_panel_enable(void)
{
	volatile unsigned int * p_temp = GPIO_BASE + 0x24;
    
    /* PB4输出模式 */
    *p_temp &= 0xfff8ffff; 
    *p_temp |= 0x00010000;
    
    p_temp = GPIO_BASE + 0x34;

    /* PB4输出0关闭背光 */
    *p_temp &= 0xFFFFFFEF; 
}

static void sunxi_composer_mode_set(const struct ctfb_res_modes *mode,
				    unsigned int address)
{
	struct de_clk * const de_clk_regs =
		(struct de_clk *)(SUNXI_DE2_BASE);
	struct de_glb * const de_glb_regs =
		(struct de_glb *)(SUNXI_DE2_MUX0_BASE +
				  SUNXI_DE2_MUX_GLB_REGS);
	struct de_bld * const de_bld_regs =
		(struct de_bld *)(SUNXI_DE2_MUX0_BASE +
				  SUNXI_DE2_MUX_BLD_REGS);
	struct de_ui * const de_ui_regs =
		(struct de_ui *)(SUNXI_DE2_MUX0_BASE +
				 SUNXI_DE2_MUX_CHAN_REGS +
				 SUNXI_DE2_MUX_CHAN_SZ * 2);
	unsigned int size = SUNXI_DE2_WH(mode->xres, mode->yres);
	int channel, i;
	unsigned int data;

	/* enable clock */
	setbits_le32(&de_clk_regs->rst_cfg, 1);
	TYX_DEGUB_(de_clk_regs->rst_cfg);
	setbits_le32(&de_clk_regs->gate_cfg, 1);
	TYX_DEGUB_(de_clk_regs->rst_cfg);
	setbits_le32(&de_clk_regs->bus_cfg, 1);
	TYX_DEGUB_(de_clk_regs->rst_cfg);

	clrbits_le32(&de_clk_regs->sel_cfg, 1);
	rt_kprintf("test clrbits_le32 test addr: 0x%08x  value: 0x%08x \n", &de_clk_regs->sel_cfg, de_clk_regs->sel_cfg);

	writel(SUNXI_DE2_MUX_GLB_CTL_RT_EN, &de_glb_regs->ctl);
	writel(0, &de_glb_regs->status);
	writel(1, &de_glb_regs->dbuff);
	writel(size, &de_glb_regs->size);

	for (channel = 0; channel < 4; channel++) {
		void *chan = SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_CHAN_REGS +
			SUNXI_DE2_MUX_CHAN_SZ * channel;
		memset(chan, 0, channel == 0 ?
			sizeof(struct de_vi) : sizeof(struct de_ui));
	}
	memset(de_bld_regs, 0, sizeof(struct de_bld));

	writel(0x00000101, &de_bld_regs->fcolor_ctl);

	writel(2, &de_bld_regs->route);

	writel(0, &de_bld_regs->premultiply);
	writel(0xff000000, &de_bld_regs->bkcolor);

	writel(0x03010301, &de_bld_regs->bld_mode[0]);
	writel(0x03010301, &de_bld_regs->bld_mode[1]);

	writel(size, &de_bld_regs->output_size);
	writel(mode->vmode & 1 ? 2 : 0,
	       &de_bld_regs->out_ctl);
	writel(0, &de_bld_regs->ck_ctl);

	for (i = 0; i < 4; i++) {
		writel(0xff000000, &de_bld_regs->attr[i].fcolor);
		writel(size, &de_bld_regs->attr[i].insize);
	}

	/* Disable all other units */
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_VSU_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_GSU1_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_GSU2_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_GSU3_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_FCE_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_BWS_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_LTI_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_PEAK_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_ASE_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_FCC_REGS);
	writel(0, SUNXI_DE2_MUX0_BASE + SUNXI_DE2_MUX_DCSC_REGS);

	data = SUNXI_DE2_UI_CFG_ATTR_EN |
	       SUNXI_DE2_UI_CFG_ATTR_FMT(SUNXI_DE2_FORMAT_XRGB_8888) |
	       SUNXI_DE2_UI_CFG_ATTR_ALPMOD(1) |
	       SUNXI_DE2_UI_CFG_ATTR_ALPHA(0xff);
	writel(data, &de_ui_regs->cfg[0].attr);
	writel(size, &de_ui_regs->cfg[0].size);
	writel(0, &de_ui_regs->cfg[0].coord);
	writel(4 * mode->xres, &de_ui_regs->cfg[0].pitch);
	writel(address, &de_ui_regs->cfg[0].top_laddr);
	writel(size, &de_ui_regs->ovl_size);
}

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


#define SUNXI_GPIO_A	0
#define SUNXI_GPIO_B	1
#define SUNXI_GPIO_C	2
#define SUNXI_GPIO_D	3
#define SUNXI_GPIO_E	4
#define SUNXI_GPIO_F	5
#define SUNXI_GPIO_G	6
#define SUNXI_GPIO_H	7
#define SUNXI_GPIO_I	8


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
};

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


static void sunxi_lcdc_pll_set(int tcon, int dotclock,
			       int *clk_div, int *clk_double)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	int value, n, m, min_m, max_m, diff;
	int best_n = 0, best_m = 0, best_diff = 0x0FFFFFFF;
	int best_double = 0;
	bool use_mipi_pll = false;

	if (tcon == 0) {
		min_m = 6;
		max_m = 127;
	} else {
		min_m = 1;
		max_m = 15;
	}

	/*
	 * Find the lowest divider resulting in a matching clock, if there
	 * is no match, pick the closest lower clock, as monitors tend to
	 * not sync to higher frequencies.
	 */
	for (m = min_m; m <= max_m; m++) {
		n = (m * dotclock) / 3000;

		if ((n >= 9) && (n <= 127)) {
			value = (3000 * n) / m;
			diff = dotclock - value;
			if (diff < best_diff) {
				best_diff = diff;
				best_m = m;
				best_n = n;
				best_double = 0;
			}
		}

		/* These are just duplicates */
		if (!(m & 1))
			continue;

		/* TCONs with DE2 do not support double clock */
	}

	{
		clock_set_pll3(best_n * 3000000);
	}

	if (tcon == 0) {
		unsigned int pll;

		if (use_mipi_pll)
			pll = CCM_LCD_CH0_CTRL_MIPI_PLL;
		else if (best_double)
			pll = CCM_LCD_CH0_CTRL_PLL3_2X;
		else
			pll = CCM_LCD_CH0_CTRL_PLL3;

		writel(CCM_LCD_CH0_CTRL_GATE | CCM_LCD_CH0_CTRL_RST | pll,
		       &ccm->lcd0_ch0_clk_cfg);
	} else {
		writel(CCM_LCD_CH1_CTRL_GATE |
		       (best_double ? CCM_LCD_CH1_CTRL_PLL3_2X :
				      CCM_LCD_CH1_CTRL_PLL3) |
		       CCM_LCD_CH1_CTRL_M(best_m), &ccm->lcd0_ch1_clk_cfg);
		if (false)
			setbits_le32(&ccm->lcd0_ch1_clk_cfg,
				     CCM_LCD_CH1_CTRL_HALF_SCLK1);
	}

	*clk_div = best_m;
	*clk_double = best_double;
}


static int sunxi_lcdc_get_clk_delay(const struct ctfb_res_modes *mode, int tcon)
{
	int delay;

	delay = mode->lower_margin + mode->vsync_len + mode->upper_margin;
	if (mode->vmode == 1)
		delay /= 2;
	if (tcon == 1)
		delay -= 2;

	return (delay > 30) ? 30 : delay;
}


static void sunxi_lcdc_tcon0_mode_set(const struct ctfb_res_modes *mode,
				      bool for_ext_vga_dac)
{
	struct sunxi_lcdc_reg * const lcdc =
		(struct sunxi_lcdc_reg *)SUNXI_LCD0_BASE;
	int bp, clk_delay, clk_div, clk_double, pin, total, val;

	for (pin = SUNXI_GPE(0); pin <= SUNXI_GPE(24); pin++) {
		if (pin >= SUNXI_GPE(20) && pin <= SUNXI_GPE(22))
			continue; /* These pins are not LCD */
		sunxi_gpio_set_cfgpin(pin, 3);
	}

	sunxi_lcdc_pll_set(0, mode->pixclock_khz, &clk_div, &clk_double);

	/* Use tcon0 */
	rt_kprintf("tyx before clrsetbits_le32 addr: 0x%08x value: 0x%08x \n",&lcdc->ctrl, lcdc->ctrl);
	clrsetbits_le32(&lcdc->ctrl, SUNXI_LCDC_CTRL_IO_MAP_MASK,
			SUNXI_LCDC_CTRL_IO_MAP_TCON0);

	rt_kprintf("tyx after clrsetbits_le32 addr: 0x%08x value: 0x%08x \n",&lcdc->ctrl, lcdc->ctrl);

	clk_delay = sunxi_lcdc_get_clk_delay(mode, 0);
	writel(SUNXI_LCDC_TCON0_CTRL_ENABLE |
	       SUNXI_LCDC_TCON0_CTRL_CLK_DELAY(clk_delay), &lcdc->tcon0_ctrl);

	writel(SUNXI_LCDC_TCON0_DCLK_ENABLE |
	       SUNXI_LCDC_TCON0_DCLK_DIV(clk_div), &lcdc->tcon0_dclk);

	writel(SUNXI_LCDC_X(mode->xres) | SUNXI_LCDC_Y(mode->yres),
	       &lcdc->tcon0_timing_active);

	bp = mode->hsync_len + mode->left_margin;
	total = mode->xres + mode->right_margin + bp;
	writel(SUNXI_LCDC_TCON0_TIMING_H_TOTAL(total) |
	       SUNXI_LCDC_TCON0_TIMING_H_BP(bp), &lcdc->tcon0_timing_h);

	bp = mode->vsync_len + mode->upper_margin;
	total = mode->yres + mode->lower_margin + bp;
	writel(SUNXI_LCDC_TCON0_TIMING_V_TOTAL(total) |
	       SUNXI_LCDC_TCON0_TIMING_V_BP(bp), &lcdc->tcon0_timing_v);

	writel(SUNXI_LCDC_X(mode->hsync_len) | SUNXI_LCDC_Y(mode->vsync_len),
	       &lcdc->tcon0_timing_sync);

	writel(0, &lcdc->tcon0_hv_intf);
	writel(0, &lcdc->tcon0_cpu_intf);

	if (sunxi_display.depth == 18 || sunxi_display.depth == 16) {
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[0]);
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[1]);
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[2]);
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[3]);
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[4]);
		writel(SUNXI_LCDC_TCON0_FRM_SEED, &lcdc->tcon0_frm_seed[5]);
		writel(SUNXI_LCDC_TCON0_FRM_TAB0, &lcdc->tcon0_frm_table[0]);
		writel(SUNXI_LCDC_TCON0_FRM_TAB1, &lcdc->tcon0_frm_table[1]);
		writel(SUNXI_LCDC_TCON0_FRM_TAB2, &lcdc->tcon0_frm_table[2]);
		writel(SUNXI_LCDC_TCON0_FRM_TAB3, &lcdc->tcon0_frm_table[3]);
		writel(((sunxi_display.depth == 18) ?
			SUNXI_LCDC_TCON0_FRM_CTRL_RGB666 :
			SUNXI_LCDC_TCON0_FRM_CTRL_RGB565),
		       &lcdc->tcon0_frm_ctrl);
	}

	val = SUNXI_LCDC_TCON0_IO_POL_DCLK_PHASE(1);
	if (!(mode->sync & 1))
		val |= SUNXI_LCDC_TCON_HSYNC_MASK;
	if (!(mode->sync & 2))
		val |= SUNXI_LCDC_TCON_VSYNC_MASK;

	writel(val, &lcdc->tcon0_io_polarity);

	writel(0, &lcdc->tcon0_io_tristate);
}

static void sunxi_lcdc_enable(void)
{
	struct sunxi_lcdc_reg * const lcdc =
		(struct sunxi_lcdc_reg *)SUNXI_LCD0_BASE;

	setbits_le32(&lcdc->ctrl, SUNXI_LCDC_CTRL_TCON_ENABLE);
}

static void sunxi_composer_enable(void)
{
	struct de_glb * const de_glb_regs =
		(struct de_glb *)(SUNXI_DE2_MUX0_BASE +
				  SUNXI_DE2_MUX_GLB_REGS);

	writel(1, &de_glb_regs->dbuff);
}

static sunxi_lcdc_backlight_enable(void)
{
    volatile unsigned int * p_temp = GPIO_BASE + 0x34;
    
    /* PB4输出1打开背光 */
    *p_temp |= 0x00000010; 
}


static void sunxi_mode_set(const struct ctfb_res_modes *mode,
			   unsigned int address)
{
	//背光相关 使能输出，但是拉低背光
	sunxi_lcdc_panel_enable();
	
    //好像是数据搬运有关，R6没有 暂时不管
	sunxi_composer_mode_set(mode, address);
    //配置TCON寄存器 重点
	sunxi_lcdc_tcon0_mode_set(mode, false);
    // DE2开始工作  暂时不管
	sunxi_composer_enable();
    //使能TCON寄存器
	sunxi_lcdc_enable();
    //打开背光
	sunxi_lcdc_backlight_enable();
}

static unsigned int _rt_framebuffer[480][800];

rt_err_t video_hw_init(rt_device_t dev)
{
	struct ctfb_res_modes test_mode;
	
	test_mode.xres = 800;                                                                
	test_mode.yres = 480;                                                                     
	test_mode.refresh = 60;                                                                    
	test_mode.pixclock = 39683;                                                                
	test_mode.pixclock_khz = 33000;                                                           
	test_mode.left_margin = 87;                                                                
	test_mode.right_margin = 40;                                                               
	test_mode.upper_margin = 31;                                                               
	test_mode.lower_margin = 13;                                                               
	test_mode.hsync_len = 1;                                                                   
	test_mode.vsync_len = 1;                                                                   
	test_mode.sync = 3;                                                                        
	test_mode.vmode = 0;

	rt_kprintf("LCD Init ...... \n");
	
	sunxi_engines_init();
	
	sunxi_mode_set(&test_mode, _rt_framebuffer);
	
	return RT_EOK;
}


void lcd_hw_set_pixel(const char * pixel, int x, int y)
{
    unsigned int * color = (unsigned int *)pixel;

    //rt_kprintf("color: %08x x: %d y: %d \n",*color, x, y);
    _rt_framebuffer[y][x] = *color;
    
}

void lcd_hw_get_pixel(char * pixel, int x, int y)
{
    *(unsigned int *)pixel = _rt_framebuffer[y][x];
}

static void lcd_hw_dram_hline(char * pixel, int x1, int x2, int y)
{
	unsigned int * color = (unsigned int *)pixel;

	//rt_kprintf("color: %08x x1: %d  x2: %d y: %d \n",*color, x1, x2, y);

	if(x1>x2)
	{
        for(;x1>x2;x2++)
        {
        	_rt_framebuffer[y][x2] = *color;
        }
	}
	else
	{
        for(;x2>x1;x1++)
        {
        	_rt_framebuffer[y][x1] = *color;
        }
	}
    
}

static void lcd_hw_dram_vline(char * pixel, int x, int y1, int y2)
{

    unsigned int * color = (unsigned int *)pixel;

	//rt_kprintf("color: %08x x: %d  y1: %d y2: %d \n",*color, x, y1, y2);

	if(y1>y2)
	{
        for(;y1>y2;y2++)
        {
        	_rt_framebuffer[y2][x] = *color;
        }
	}
	else
	{
        for(;y2>y1;y1++)
        {
        	_rt_framebuffer[y1][x] = *color;
        }
	}
    
}

void lcd_hw_draw_blit_hline(const char * pixel, int x, int y, rt_size_t size)
{
	volatile unsigned int * p =  &_rt_framebuffer[y][x];

	while(size)
	{
        *p = *(unsigned int *)pixel;
		size--;
	}
}


static struct rt_device_graphic_info _lcd_info;
static struct rt_device  lcd;


static rt_err_t rt_lcd_control(rt_device_t dev, rt_uint8_t cmd, void *args)
{
    switch (cmd)
    {
    case RTGRAPHIC_CTRL_RECT_UPDATE:
        rt_kprintf("lcd update!\n");
        break;
    case RTGRAPHIC_CTRL_POWERON:
        break;
    case RTGRAPHIC_CTRL_POWEROFF:
        break;
    case RTGRAPHIC_CTRL_GET_INFO:
        //memcpy(args, &_lcd_info, sizeof(_lcd_info));

        ((struct rt_device_graphic_info *)args)->bits_per_pixel = _lcd_info.bits_per_pixel;
        ((struct rt_device_graphic_info *)args)->pixel_format = _lcd_info.pixel_format;
        ((struct rt_device_graphic_info *)args)->framebuffer = RT_NULL;
        ((struct rt_device_graphic_info *)args)->width = _lcd_info.width;
        ((struct rt_device_graphic_info *)args)->height = _lcd_info.height;
        ((struct rt_device_graphic_info *)args)->reserved = _lcd_info.reserved;


        break;
    case RTGRAPHIC_CTRL_SET_MODE:
        break;
    }

    return RT_EOK;
}

struct rt_device_graphic_ops lcd_ill_ops = 
{
    lcd_hw_set_pixel,
    lcd_hw_get_pixel,
    lcd_hw_dram_hline,
    lcd_hw_dram_vline,
    lcd_hw_draw_blit_hline
};

void rt_hw_lcd_init(void)
{
	
    //_rt_framebuffer = rt_malloc_align(sizeof(rt_uint32_t) * 480 * 800, 32);
    //if (_rt_framebuffer == RT_NULL) return; /* no memory yet */
    //_rt_framebuffer = (rt_uint16_t *)FRAME_BUFFER;
	
    _lcd_info.bits_per_pixel = 16;
    _lcd_info.pixel_format = RTGRAPHIC_PIXEL_FORMAT_RGB888;
    _lcd_info.framebuffer = (void *)_rt_framebuffer;
    _lcd_info.width = 800;
    _lcd_info.height = 480;

    /* init device structure */
    lcd.type = RT_Device_Class_Graphic;
    lcd.init = video_hw_init;
    lcd.open = RT_NULL;
    lcd.close = RT_NULL;
    lcd.control = rt_lcd_control;
    lcd.user_data = (void *)&lcd_ill_ops;

    /* register lcd device to RT-Thread */
    rt_device_register(&lcd, "lcd", RT_DEVICE_FLAG_RDWR);
}


void lcd_clear(rt_uint32_t color)
{
    volatile rt_uint32_t *p = (rt_uint32_t *)_rt_framebuffer;
    int x, y;

    for (y = 0; y < 480; y++)
    {
        for (x = 0; x < 800; x++)
        {
            *p++ = color; /* red */
        }
    }
}


void lcd_tes__t(void)
{
	unsigned int i = 0;
    unsigned int color = 0;

    lcd_clear(0x00ff0000);
    rt_thread_delay(2000);
    lcd_clear(0x0000ff00);
    rt_thread_delay(2000);
    lcd_clear(0x000000ff);
    rt_thread_delay(200);
    lcd_clear(0x00ffffff);

    lcd_hw_dram_hline((char *)(&color),0,800,200);



    lcd_hw_dram_vline((char *)(&color),300,0,480);

    for(i = 0;i<200;i++)
    {
    	lcd_hw_set_pixel((char *)(&color), i, 240);
    }

}

#ifdef RT_USING_FINSH
#include <finsh.h>
FINSH_FUNCTION_EXPORT(lcd_clear, lcd_clear);
FINSH_FUNCTION_EXPORT(lcd_tes__t, lcd_tes__t);
#ifdef FINSH_USING_MSH
MSH_CMD_EXPORT(lcd_clear, lcd_clear);
MSH_CMD_EXPORT(lcd_tes__t, lcd_tes__t);
#endif
#endif

















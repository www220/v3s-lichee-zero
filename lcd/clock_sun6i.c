/*
 * sun6i specific clock code
 *
 * (C) Copyright 2007-2012
 * Allwinner Technology Co., Ltd. <www.allwinnertech.com>
 * Tom Cubie <tangliang@allwinnertech.com>
 *
 * (C) Copyright 2013 Luke Kenneth Casson Leighton <lkcl@lkcl.net>
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include "clock_sun6i.h"
#include "io.h"

void clock_set_pll3(unsigned int clk)
{
	struct sunxi_ccm_reg * const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	const int m = 8; /* 3 MHz steps just like sun4i, sun5i and sun7i */

	if (clk == 0) {
		clrbits_le32(&ccm->pll3_cfg, CCM_PLL3_CTRL_EN);
		return;
	}

	/* PLL3 rate = 24000000 * n / m */
	writel(CCM_PLL3_CTRL_EN | CCM_PLL3_CTRL_INTEGER_MODE |
	       CCM_PLL3_CTRL_N(clk / (24000000 / m)) | CCM_PLL3_CTRL_M(m),
	       &ccm->pll3_cfg);
}

 
unsigned int clock_get_pll3(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	unsigned int rval = readl(&ccm->pll3_cfg);
	int n = ((rval & CCM_PLL3_CTRL_N_MASK) >> CCM_PLL3_CTRL_N_SHIFT) + 1;
	int m = ((rval & CCM_PLL3_CTRL_M_MASK) >> CCM_PLL3_CTRL_M_SHIFT) + 1;

	/* Multiply by 1000 after dividing by m to avoid integer overflows */
	return (24000 * n / m) * 1000;
}

unsigned int clock_get_pll6(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	unsigned int rval = readl(&ccm->pll6_cfg);
	int n = ((rval & CCM_PLL6_CTRL_N_MASK) >> CCM_PLL6_CTRL_N_SHIFT) + 1;
	int k = ((rval & CCM_PLL6_CTRL_K_MASK) >> CCM_PLL6_CTRL_K_SHIFT) + 1;
	return 24000000 * n * k / 2;
}

unsigned int clock_get_mipi_pll(void)
{
	struct sunxi_ccm_reg *const ccm =
		(struct sunxi_ccm_reg *)SUNXI_CCM_BASE;
	unsigned int rval = readl(&ccm->mipi_pll_cfg);
	unsigned int n = ((rval & CCM_MIPI_PLL_CTRL_N_MASK) >> CCM_MIPI_PLL_CTRL_N_SHIFT) + 1;
	unsigned int k = ((rval & CCM_MIPI_PLL_CTRL_K_MASK) >> CCM_MIPI_PLL_CTRL_K_SHIFT) + 1;
	unsigned int m = ((rval & CCM_MIPI_PLL_CTRL_M_MASK) >> CCM_MIPI_PLL_CTRL_M_SHIFT) + 1;
	unsigned int src = clock_get_pll3();

	/* Multiply by 1000 after dividing by m to avoid integer overflows */
	return ((src / 1000) * n * k / m) * 1000;
}

void clock_set_de_mod_clock(unsigned int *clk_cfg, unsigned int hz)
{
	int pll = clock_get_pll6() * 2;
	int div = 1;

	while ((pll / div) > hz)
		div++;

	writel(CCM_DE_CTRL_GATE | CCM_DE_CTRL_PLL6_2X | CCM_DE_CTRL_M(div),
	       clk_cfg);
}

void clock_set_de2_mod_clock(unsigned int *clk_cfg, unsigned int hz)
{
	int pll = clock_get_pll6() * 2;
	int div = 1;

	while ((pll / div) > hz)
		div++;

	writel(CCM_DE2_CTRL_GATE | CCM_DE2_CTRL_PLL6_2X | CCM_DE2_CTRL_M(div),
	       clk_cfg);
}



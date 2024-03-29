Normally it should only be initialised in SPL as it is "time consuming".
Doing so however leaves cpus clocked to low frequencies
for Rockchip's DDR/loader/trust with mainline u-boot scenario
which does not involve SPL phase.

--- drivers/clk/rockchip/clk_rk3399.c.orig
+++ drivers/clk/rockchip/clk_rk3399.c
@@ -47,12 +47,9 @@ struct pll_div {
 	.fbdiv = (u32)((u64)hz * _refdiv * _postdiv1 * _postdiv2 / OSC_HZ),\
 	.postdiv1 = _postdiv1, .postdiv2 = _postdiv2};
 
-#if defined(CONFIG_SPL_BUILD)
 static const struct pll_div gpll_init_cfg = PLL_DIVISORS(GPLL_HZ, 2, 2, 1);
 static const struct pll_div cpll_init_cfg = PLL_DIVISORS(CPLL_HZ, 1, 2, 2);
-#else
 static const struct pll_div ppll_init_cfg = PLL_DIVISORS(PPLL_HZ, 2, 2, 1);
-#endif
 
 static const struct pll_div apll_l_1600_cfg = PLL_DIVISORS(1600 * MHz, 3, 1, 1);
 static const struct pll_div apll_l_600_cfg = PLL_DIVISORS(600 * MHz, 1, 2, 1);
@@ -1071,7 +1068,6 @@ static struct clk_ops rk3399_clk_ops = {
 #endif
 };
 
-#ifdef CONFIG_SPL_BUILD
 static void rkclk_init(struct rockchip_cru *cru)
 {
 	u32 aclk_div;
@@ -1149,11 +1145,9 @@ static void rkclk_init(struct rockchip_cru *cru)
 		     hclk_div << HCLK_PERILP1_DIV_CON_SHIFT |
 		     HCLK_PERILP1_PLL_SEL_GPLL << HCLK_PERILP1_PLL_SEL_SHIFT);
 }
-#endif
 
 static int rk3399_clk_probe(struct udevice *dev)
 {
-#ifdef CONFIG_SPL_BUILD
 	struct rk3399_clk_priv *priv = dev_get_priv(dev);
 
 #if CONFIG_IS_ENABLED(OF_PLATDATA)
@@ -1162,7 +1156,6 @@ static int rk3399_clk_probe(struct udevice *dev)
 	priv->cru = map_sysmem(plat->dtd.reg[0], plat->dtd.reg[1]);
 #endif
 	rkclk_init(priv->cru);
-#endif
 	return 0;
 }

--- drivers/clk/rockchip/clk_rk3399.c.orig	2020-07-06 22:22:53.000000000 +0300
+++ drivers/clk/rockchip/clk_rk3399.c	2021-04-17 08:22:36.436362000 +0300
@@ -1351,6 +1351,8 @@
 		     pclk_div << PCLK_PERILP1_DIV_CON_SHIFT |
 		     hclk_div << HCLK_PERILP1_DIV_CON_SHIFT |
 		     HCLK_PERILP1_PLL_SEL_GPLL << HCLK_PERILP1_PLL_SEL_SHIFT);
+
+	rk3399_saradc_set_clk(cru, 1000000);
 }
 #endif
 

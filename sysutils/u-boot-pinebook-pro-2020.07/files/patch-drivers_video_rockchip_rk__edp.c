--- drivers/video/rockchip/rk_edp.c.orig	2020-07-06 22:22:53.000000000 +0300
+++ drivers/video/rockchip/rk_edp.c	2020-07-08 09:53:12.971651000 +0300
@@ -17,10 +17,19 @@
 #include <asm/gpio.h>
 #include <asm/io.h>
 #include <asm/arch-rockchip/clock.h>
+#include <asm/arch-rockchip/hardware.h>
 #include <asm/arch-rockchip/edp_rk3288.h>
+
+#if defined(CONFIG_ROCKCHIP_RK3288)
 #include <asm/arch-rockchip/grf_rk3288.h>
-#include <asm/arch-rockchip/hardware.h>
 #include <dt-bindings/clock/rk3288-cru.h>
+#endif
+
+#if defined(CONFIG_ROCKCHIP_RK3399)
+#include <asm/arch-rockchip/grf_rk3399.h>
+#include <dt-bindings/clock/rk3399-cru.h>
+#endif
+
 #include <linux/delay.h>
 
 #define MAX_CR_LOOP 5
@@ -39,7 +48,12 @@
 
 struct rk_edp_priv {
 	struct rk3288_edp *regs;
+#if defined(CONFIG_ROCKCHIP_RK3288)
 	struct rk3288_grf *grf;
+#endif
+#if defined(CONFIG_ROCKCHIP_RK3399)
+	struct rk3399_grf_regs *grf;
+#endif
 	struct udevice *panel;
 	struct link_train link_train;
 	u8 train_set[4];
@@ -1024,6 +1038,7 @@
 	struct rk_edp_priv *priv = dev_get_priv(dev);
 	struct rk3288_edp *regs = priv->regs;
 	struct clk clk;
+	struct clk_bulk clocks;
 	int ret;
 
 	ret = uclass_get_device_by_phandle(UCLASS_PANEL, dev, "rockchip,panel",
@@ -1037,6 +1052,7 @@
 	int vop_id = uc_plat->source_id;
 	debug("%s, uc_plat=%p, vop_id=%u\n", __func__, uc_plat, vop_id);
 
+#if defined(CONFIG_ROCKCHIP_RK3288)
 	ret = clk_get_by_index(dev, 1, &clk);
 	if (ret >= 0) {
 		ret = clk_set_rate(&clk, 0);
@@ -1046,6 +1062,7 @@
 		debug("%s: Failed to set EDP clock: ret=%d\n", __func__, ret);
 		return ret;
 	}
+#endif
 
 	ret = clk_get_by_index(uc_plat->src_dev, 0, &clk);
 	if (ret >= 0) {
@@ -1058,13 +1075,21 @@
 		return ret;
 	}
 
+#if defined(CONFIG_ROCKCHIP_RK3288)
 	/* grf_edp_ref_clk_sel: from internal 24MHz or 27MHz clock */
 	rk_setreg(&priv->grf->soc_con12, 1 << 4);
 
 	/* select epd signal from vop0 or vop1 */
 	rk_clrsetreg(&priv->grf->soc_con6, (1 << 5),
 	    (vop_id == 1) ? (1 << 5) : (0 << 5));
+#endif
 
+#if defined(CONFIG_ROCKCHIP_RK3399)
+	/* select epd signal from vop0 or vop1 */
+	rk_clrsetreg(&priv->grf->soc_con20, (1 << 5),
+	    (vop_id == 1) ? (1 << 5) : (0 << 5));
+#endif
+
 	rockchip_edp_wait_hpd(priv);
 
 	rk_edp_init_refclk(regs);
@@ -1085,6 +1110,7 @@
 
 static const struct udevice_id rockchip_dp_ids[] = {
 	{ .compatible = "rockchip,rk3288-edp" },
+	{ .compatible = "rockchip,rk3399-edp" },
 	{ }
 };
 

--- drivers/ram/rockchip/sdram_rk3399.c.orig	2020-07-06 22:22:53.000000000 +0300
+++ drivers/ram/rockchip/sdram_rk3399.c	2021-04-17 08:23:09.107277000 +0300
@@ -1625,7 +1625,6 @@
 	rk_clrsetreg(&pmusgrf->soc_con4, 0x1f << 10, stride << 10);
 }
 
-#if !defined(CONFIG_RAM_RK3399_LPDDR4)
 static int data_training_first(struct dram_info *dram, u32 channel, u8 rank,
 			       struct rk3399_sdram_params *params)
 {
@@ -1715,8 +1714,8 @@
 	clrsetbits_le32(&denali_pi_params[76], 0x1 << 24, 0x1 << 24);
 	clrsetbits_le32(&denali_pi_params[77], 0x1, 0x1);
 }
-#else
 
+#if defined(CONFIG_RAM_RK3399_LPDDR4)
 struct rk3399_sdram_params dfs_cfgs_lpddr4[] = {
 #include "sdram-rk3399-lpddr4-400.inc"
 #include "sdram-rk3399-lpddr4-800.inc"
@@ -3032,20 +3031,40 @@
 	return 0;
 }
 
+__weak const char *rk3399_get_ddrtype(void)
+{
+	return NULL;
+}
+
 static int rk3399_dmc_ofdata_to_platdata(struct udevice *dev)
 {
 #if !CONFIG_IS_ENABLED(OF_PLATDATA)
 	struct rockchip_dmc_plat *plat = dev_get_platdata(dev);
+	ofnode node = { .np = NULL };
+	const char *name;
 	int ret;
 
-	ret = dev_read_u32_array(dev, "rockchip,sdram-params",
-				 (u32 *)&plat->sdram_params,
-				 sizeof(plat->sdram_params) / sizeof(u32));
+	name = rk3399_get_ddrtype();
+	if (name)
+		node = dev_read_subnode(dev, name);
+	if (!ofnode_valid(node)) {
+		debug("Failed to read subnode %s\n", name);
+		node = dev_read_first_subnode(dev);
+	}
+
+	/* fallback to current node */
+	if (!ofnode_valid(node))
+		node = dev_ofnode(dev);
+
+	ret = ofnode_read_u32_array(node, "rockchip,sdram-params",
+				    (u32 *)&plat->sdram_params,
+				    sizeof(plat->sdram_params) / sizeof(u32));
 	if (ret) {
 		printf("%s: Cannot read rockchip,sdram-params %d\n",
 		       __func__, ret);
 		return ret;
 	}
+
 	ret = regmap_init_mem(dev_ofnode(dev), &plat->map);
 	if (ret)
 		printf("%s: regmap failed %d\n", __func__, ret);
@@ -3072,18 +3091,20 @@
 #endif
 
 static const struct sdram_rk3399_ops rk3399_ops = {
-#if !defined(CONFIG_RAM_RK3399_LPDDR4)
 	.data_training_first = data_training_first,
 	.set_rate_index = switch_to_phy_index1,
 	.modify_param = modify_param,
 	.get_phy_index_params = get_phy_index_params,
-#else
+};
+
+#if defined(CONFIG_RAM_RK3399_LPDDR4)
+static const struct sdram_rk3399_ops lpddr4_ops = {
 	.data_training_first = lpddr4_mr_detect,
 	.set_rate_index = lpddr4_set_rate,
 	.modify_param = lpddr4_modify_param,
 	.get_phy_index_params = lpddr4_get_phy_index_params,
-#endif
 };
+#endif
 
 static int rk3399_dmc_init(struct udevice *dev)
 {
@@ -3102,7 +3123,17 @@
 		return ret;
 #endif
 
-	priv->ops = &rk3399_ops;
+	if (params->base.dramtype == LPDDR4) {
+#if defined(CONFIG_RAM_RK3399_LPDDR4)
+		priv->ops = &lpddr4_ops;
+#else
+		printf("LPDDR4 support is disable\n");
+		return -EINVAL;
+#endif
+	} else {
+		priv->ops = &rk3399_ops;
+	}
+
 	priv->cic = syscon_get_first_range(ROCKCHIP_SYSCON_CIC);
 	priv->grf = syscon_get_first_range(ROCKCHIP_SYSCON_GRF);
 	priv->pmu = syscon_get_first_range(ROCKCHIP_SYSCON_PMU);

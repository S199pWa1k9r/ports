--- drivers/video/rockchip/rk_hdmi.c.orig
+++ drivers/video/rockchip/rk_hdmi.c
@@ -113,8 +113,9 @@ int rk_hdmi_probe(struct udevice *dev)
 
 	ret = dw_hdmi_phy_wait_for_hpd(hdmi);
 	if (ret < 0) {
-		debug("hdmi can not get hpd signal\n");
-		return -1;
+		debug("*** hdmi can not get hpd signal *** \n");
+		debug("*** force hdmi activate  *** \n");
+	//	return -1;
 	}
 
 	dw_hdmi_init(hdmi);

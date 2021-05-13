--- drivers/video/rockchip/rk_vop.c.orig
+++ drivers/video/rockchip/rk_vop.c
@@ -299,9 +299,20 @@ static int rk_display_init(struct udevice *dev, ulong fbbase, ofnode ep_node)
 	}
 
 	ret = display_read_timing(disp, &timing);
-	if (ret) {
-		debug("%s: Failed to read timings\n", __func__);
-		return ret;
+
+	if (ret || timing.hactive.typ > 1920 ) {
+	    debug("%s: try failsave - full hd mode timing\n", __func__);
+	    timing.pixelclock.typ = 148500000;
+	    timing.hactive.typ = 1920;
+	    timing.hfront_porch.typ = 88;
+	    timing.hback_porch.typ = 148;
+	    timing.hsync_len.typ = 44;
+	    timing.vactive.typ = 1080;
+	    timing.vfront_porch.typ = 4;
+	    timing.vback_porch.typ = 36;
+	    timing.vsync_len.typ = 5;
+	    timing.flags = 10;
+	    timing.hdmi_monitor = true;
 	}
 
 	ret = clk_get_by_index(dev, 1, &clk);

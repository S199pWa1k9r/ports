From b0693aeb9ceab57ffc9d9f4ceca610bd82d5ca07 Mon Sep 17 00:00:00 2001
From: =?UTF-8?q?Kamil=20Trzci=C5=84ski?= <ayufan@ayufan.eu>
Date: Mon, 21 May 2018 02:00:19 +0200
Subject: [PATCH] ayufan: rock64: for SPL build always use fifo-mode

Change-Id: I9ac012ce4aaf03a151f7c5c818829d631efdd7ed
diff --git a/drivers/mmc/rockchip_dw_mmc.c b/drivers/mmc/rockchip_dw_mmc.c
index fc0f0fad76..d1f26e41fe 100644
--- drivers/mmc/rockchip_dw_mmc.c.orig
+++ drivers/mmc/rockchip_dw_mmc.c
@@ -70,7 +70,11 @@ static int rockchip_dwmmc_ofdata_to_platdata(struct udevice *dev)
 
 	if (priv->fifo_depth < 0)
 		return -EINVAL;
+#ifdef CONFIG_SPL_BUILD
+	priv->fifo_mode = true; // always force fifo mode
+#else
 	priv->fifo_mode = dev_read_bool(dev, "fifo-mode");
+#endif
 
 	/*
 	 * 'clock-freq-min-max' is deprecated

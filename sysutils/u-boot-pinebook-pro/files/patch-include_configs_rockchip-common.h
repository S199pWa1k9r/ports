--- include/configs/rockchip-common.h.orig	2020-07-16 13:55:46.148082000 +0300
+++ include/configs/rockchip-common.h	2020-07-16 13:56:54.959523000 +0300
@@ -17,8 +17,8 @@
 /* First try to boot from SD (index 0), then eMMC (index 1) */
 #if CONFIG_IS_ENABLED(CMD_MMC)
 	#define BOOT_TARGET_MMC(func) \
-		func(MMC, mmc, 0) \
-		func(MMC, mmc, 1)
+		func(MMC, mmc, 1) \
+		func(MMC, mmc, 0)
 #else
 	#define BOOT_TARGET_MMC(func)
 #endif

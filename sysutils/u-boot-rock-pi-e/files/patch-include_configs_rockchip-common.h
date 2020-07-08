--- include/configs/rockchip-common.h.orig	2020-05-16 17:58:54.786995000 +0300
+++ include/configs/rockchip-common.h	2020-05-16 18:01:56.480370000 +0300
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
@@ -42,8 +42,8 @@
 #endif
 
 #define BOOT_TARGET_DEVICES(func) \
-	BOOT_TARGET_MMC(func) \
 	BOOT_TARGET_USB(func) \
+	BOOT_TARGET_MMC(func) \
 	BOOT_TARGET_PXE(func) \
 	BOOT_TARGET_DHCP(func)
 

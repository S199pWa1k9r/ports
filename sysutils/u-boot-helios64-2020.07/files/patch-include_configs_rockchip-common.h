--- include/configs/rockchip-common.h.orig
+++ include/configs/rockchip-common.h
@@ -14,8 +14,8 @@
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

--- include/configs/rockchip-common.h.orig	2021-01-18 10:59:55.275700000 +0300
+++ include/configs/rockchip-common.h	2021-01-18 11:06:44.964842000 +0300
@@ -16,11 +16,10 @@
 
 /* First try to boot from SD (index 0), then eMMC (index 1) */
 #if CONFIG_IS_ENABLED(CMD_MMC)
-	#define BOOT_TARGET_MMC(func) \
-		func(MMC, mmc, 0) \
-		func(MMC, mmc, 1)
+	#define BOOT_TARGET_MMC(func, idx) \
+		func(MMC, mmc, idx) 
 #else
-	#define BOOT_TARGET_MMC(func)
+	#define BOOT_TARGET_MMC(func, idx)
 #endif
 
 #if CONFIG_IS_ENABLED(CMD_NVME)
@@ -55,16 +54,18 @@
 
 #ifdef CONFIG_ROCKCHIP_RK3399
 #define BOOT_TARGET_DEVICES(func) \
-	BOOT_TARGET_MMC(func) \
-	BOOT_TARGET_NVME(func) \
+	BOOT_TARGET_MMC(func, 1) \
 	BOOT_TARGET_USB(func) \
+	BOOT_TARGET_NVME(func) \
+	BOOT_TARGET_MMC(func, 0) \
 	BOOT_TARGET_PXE(func) \
 	BOOT_TARGET_DHCP(func) \
 	BOOT_TARGET_SF(func)
 #else
 #define BOOT_TARGET_DEVICES(func) \
-	BOOT_TARGET_MMC(func) \
+	BOOT_TARGET_MMC(func, 1) \
 	BOOT_TARGET_USB(func) \
+	BOOT_TARGET_MMC(func, 0) \
 	BOOT_TARGET_PXE(func) \
 	BOOT_TARGET_DHCP(func)
 #endif

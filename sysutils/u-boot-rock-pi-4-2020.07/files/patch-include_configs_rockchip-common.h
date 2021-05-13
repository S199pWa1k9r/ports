--- include/configs/rockchip-common.h.orig	2020-07-06 22:22:53.000000000 +0300
+++ include/configs/rockchip-common.h	2021-05-02 20:08:24.893368000 +0300
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
@@ -29,6 +28,17 @@
 	#define BOOT_TARGET_NVME(func)
 #endif
 
+#if CONFIG_IS_ENABLED(SCSI)
+	#define CONFIG_SYS_SCSI_MAX_SCSI_ID     5
+	#define CONFIG_SYS_SCSI_MAX_LUN         1
+	#define CONFIG_SYS_SCSI_MAX_DEVICE      (CONFIG_SYS_SCSI_MAX_SCSI_ID * \
+						CONFIG_SYS_SCSI_MAX_LUN)
+	#define BOOT_TARGET_SCSI(func) \
+		func(SCSI, scsi, 0)
+#else
+	#define BOOT_TARGET_SCSI(func)
+#endif
+
 #if CONFIG_IS_ENABLED(CMD_USB)
 	#define BOOT_TARGET_USB(func) func(USB, usb, 0)
 #else
@@ -55,16 +65,19 @@
 
 #ifdef CONFIG_ROCKCHIP_RK3399
 #define BOOT_TARGET_DEVICES(func) \
-	BOOT_TARGET_MMC(func) \
-	BOOT_TARGET_NVME(func) \
 	BOOT_TARGET_USB(func) \
+	BOOT_TARGET_MMC(func, 1) \
+	BOOT_TARGET_NVME(func) \
+	BOOT_TARGET_SCSI(func) \
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

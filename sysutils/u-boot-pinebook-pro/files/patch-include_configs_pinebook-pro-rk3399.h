--- include/configs/pinebook-pro-rk3399.h.orig	2020-07-06 22:22:53.000000000 +0300
+++ include/configs/pinebook-pro-rk3399.h	2020-07-16 19:10:11.297048000 +0300
@@ -8,20 +8,11 @@
 #define __PINEBOOK_PRO_RK3399_H
 
 #define ROCKCHIP_DEVICE_SETTINGS \
-		"stdin=serial,usbkbd\0" \
-		"stdout=serial,vidconsole\0" \
-		"stderr=serial,vidconsole\0"
+		"stdin=serial\0" \
+		"stdout=serial\0" \
+		"stderr=serial\0"
 
 #include <configs/rk3399_common.h>
-
-#if defined(CONFIG_ENV_IS_IN_MMC)
-#define CONFIG_SYS_MMC_ENV_DEV 0
-#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
-#define CONFIG_ENV_SECT_SIZE		(8 * 1024)
-#endif
-
-#undef CONFIG_SYS_SPI_U_BOOT_OFFS
-#define CONFIG_SYS_SPI_U_BOOT_OFFS	1024 * 512
 
 #define SDRAM_BANK_SIZE			(2UL << 30)
 

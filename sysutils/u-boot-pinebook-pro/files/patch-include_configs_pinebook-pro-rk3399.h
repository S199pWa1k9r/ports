--- include/configs/pinebook-pro-rk3399.h.orig	2020-06-13 15:09:07.966466000 +0000
+++ include/configs/pinebook-pro-rk3399.h	2020-06-13 15:10:02.298211000 +0000
@@ -14,15 +14,6 @@
 
 #include <configs/rk3399_common.h>
 
-#if defined(CONFIG_ENV_IS_IN_MMC)
-#define CONFIG_SYS_MMC_ENV_DEV 0
-#elif defined(CONFIG_ENV_IS_IN_SPI_FLASH)
-#define CONFIG_ENV_SECT_SIZE		(8 * 1024)
-#endif
-
-#undef CONFIG_SYS_SPI_U_BOOT_OFFS
-#define CONFIG_SYS_SPI_U_BOOT_OFFS	1024 * 512
-
 #define SDRAM_BANK_SIZE			(2UL << 30)
 
 #endif

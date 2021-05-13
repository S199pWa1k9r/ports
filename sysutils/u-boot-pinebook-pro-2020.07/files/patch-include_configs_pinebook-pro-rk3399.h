--- include/configs/pinebook-pro-rk3399.h.orig	2021-01-13 06:11:11.463691000 +0300
+++ include/configs/pinebook-pro-rk3399.h	2021-01-13 06:12:48.129467000 +0300
@@ -25,4 +25,7 @@
 
 #define SDRAM_BANK_SIZE			(2UL << 30)
 
+#define CONFIG_USB_OHCI_NEW
+#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	2
+
 #endif

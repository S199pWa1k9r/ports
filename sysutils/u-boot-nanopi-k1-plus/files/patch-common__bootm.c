--- common/bootm.c.orig	2018-08-11 20:55:57.188348000 +0300
+++ common/bootm.c	2018-08-11 20:57:06.025918000 +0300
@@ -31,7 +31,7 @@
 
 #ifndef CONFIG_SYS_BOOTM_LEN
 /* use 8MByte as default max gunzip size */
-#define CONFIG_SYS_BOOTM_LEN	0x800000
+#define CONFIG_SYS_BOOTM_LEN	0x2000000
 #endif
 
 #define IH_INITRD_ARCH IH_ARCH_DEFAULT

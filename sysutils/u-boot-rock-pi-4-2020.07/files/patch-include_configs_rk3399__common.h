--- include/configs/rk3399_common.h.orig	2020-07-08 08:46:14.768784000 +0300
+++ include/configs/rk3399_common.h	2020-07-08 08:57:09.716410000 +0300
@@ -58,9 +58,42 @@
 #ifndef ROCKCHIP_DEVICE_SETTINGS
 #define ROCKCHIP_DEVICE_SETTINGS
 #endif
+
+#define CONFIG_USB_OHCI_NEW
+#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	2
+
+#ifdef CONFIG_CMD_BMP
+#define CONFIG_VIDEO_BMP_RLE8
+#define CONFIG_BMP_16BPP
+#define CONFIG_BMP_24BPP
+#define CONFIG_BMP_32BPP
+#define CONFIG_SPLASH_SCREEN
+#define CONFIG_SPLASH_SCREEN_ALIGN
+#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE 8192000
+#define CONFIG_VIDEO_BMP_GZIP 1
 
+#ifdef ROCKCHIP_DEVICE_SETTINGS
+#undef ROCKCHIP_DEVICE_SETTINGS
+#define ROCKCHIP_DEVICE_SETTINGS \
+	"stdin=serial,usbkbd\0" 	/* stdin=serial,usbkbd */ \
+	"stdout=serial\0" 		/* stdout=serial,vidconsole */ \
+	"stderr=serial\0"
+#endif
+#endif
+
 #include <config_distro_bootcmd.h>
 #include <environment/distro/sf.h>
+
+#ifdef CONFIG_USE_PREBOOT
+#define PREBOOT_LOAD_LOGO \
+	"ll=1 && " \
+	"load mmc 0 $loadaddr splash.bmp || " \
+	"load mmc 1 $loadaddr splash.bmp || " \
+	"ll=0; " \
+	"test $ll = 1 && bmp display $loadaddr m m || ll=0 "
+
+#define PREBOOT_CMD "usb start; pci init; run load_logo;"
+
 #define CONFIG_EXTRA_ENV_SETTINGS \
 	ENV_MEM_LAYOUT_SETTINGS \
 	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
@@ -68,9 +98,24 @@
 	ROCKCHIP_DEVICE_SETTINGS \
 	BOOTENV \
 	BOOTENV_SF \
+	"loadaddr=0x40000000\0" \
+	"load_logo=" PREBOOT_LOAD_LOGO "\0" \
+	"preboot=" PREBOOT_CMD "\0" \
 	"altbootcmd=" \
 		"setenv boot_syslinux_conf extlinux/extlinux-rollback.conf;" \
 		"run distro_bootcmd\0"
+#else
+#define CONFIG_EXTRA_ENV_SETTINGS \
+	ENV_MEM_LAYOUT_SETTINGS \
+	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
+	"partitions=" PARTS_DEFAULT \
+	ROCKCHIP_DEVICE_SETTINGS \
+	BOOTENV \
+	BOOTENV_SF \
+	"altbootcmd=" \
+		"setenv boot_syslinux_conf extlinux/extlinux-rollback.conf;" \
+		"run distro_bootcmd\0"
+#endif 
 
 #endif
 

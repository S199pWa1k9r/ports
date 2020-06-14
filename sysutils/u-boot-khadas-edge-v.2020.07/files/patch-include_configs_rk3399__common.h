--- include/configs/rk3399_common.h.orig	2020-06-02 18:41:26.000000000 +0300
+++ include/configs/rk3399_common.h	2020-06-08 15:36:00.143595000 +0300
@@ -59,17 +59,54 @@
 #define ROCKCHIP_DEVICE_SETTINGS
 #endif
 
+#ifdef CONFIG_CMD_BMP
+#define CONFIG_VIDEO_BMP_RLE8
+#define CONFIG_BMP_16BPP
+#define CONFIG_BMP_24BPP
+#define CONFIG_BMP_32BPP
+#define CONFIG_SPLASH_SCREEN
+#define CONFIG_SPLASH_SCREEN_ALIGN
+#define CONFIG_SYS_VIDEO_LOGO_MAX_SIZE 8192000
+#define CONFIG_VIDEO_BMP_GZIP 1
+#define CONSOLE_FONT_COLOR 15
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
+#define PREBOOT_CMD "run load_logo;"
+
 #define CONFIG_EXTRA_ENV_SETTINGS \
 	ENV_MEM_LAYOUT_SETTINGS \
 	"fdtfile=" CONFIG_DEFAULT_FDT_FILE "\0" \
 	"partitions=" PARTS_DEFAULT \
 	ROCKCHIP_DEVICE_SETTINGS \
 	BOOTENV \
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
+	"altbootcmd=" \
+		"setenv boot_syslinux_conf extlinux/extlinux-rollback.conf;" \
+		"run distro_bootcmd\0"
+#endif 
 
 #endif
 

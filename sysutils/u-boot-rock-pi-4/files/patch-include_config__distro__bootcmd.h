--- include/config_distro_bootcmd.h.orig
+++ include/config_distro_bootcmd.h
@@ -131,11 +131,15 @@
 		"fi;"                                                     \
 		"load ${devtype} ${devnum}:${distro_bootpart} "           \
 			"${kernel_addr_r} efi/boot/"BOOTEFI_NAME"; "      \
+	"in=$stdin; out=$stdout; err=$stderr;"\
+	"setenv stdin serial;setenv stdout serial; setenv stderr serial; "\
 		"if fdt addr ${fdt_addr_r}; then "                        \
 			"bootefi ${kernel_addr_r} ${fdt_addr_r};"         \
 		"else "                                                   \
 			"bootefi ${kernel_addr_r} ${fdtcontroladdr};"     \
-		"fi\0"                                                    \
+		"fi; "                                                    \
+	"setenv stdin $in;setenv stdout $out; setenv stderr $err; "       \
+		" \0"                                                     \
 	\
 	"load_efi_dtb="                                                   \
 		"load ${devtype} ${devnum}:${distro_bootpart} "           \

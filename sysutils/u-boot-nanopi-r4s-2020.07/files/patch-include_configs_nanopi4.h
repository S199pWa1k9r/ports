--- /dev/null	2021-04-17 08:28:24.399162000 +0300
+++ include/configs/nanopi4.h	2021-04-17 08:22:36.437534000 +0300
@@ -0,0 +1,29 @@
+/* SPDX-License-Identifier: GPL-2.0+ */
+/*
+ * Copyright (C) Guangzhou FriendlyELEC Computer Tech. Co., Ltd.
+ * (http://www.friendlyarm.com)
+ *
+ * (C) Copyright 2016 Rockchip Electronics Co., Ltd
+ */
+
+#ifndef __CONFIG_NANOPI4_H__
+#define __CONFIG_NANOPI4_H__
+
+#define ROCKCHIP_DEVICE_SETTINGS \
+		"stdin=serial,usbkbd\0" \
+		"stdout=serial,vidconsole\0" \
+		"stderr=serial,vidconsole\0"
+
+#include <configs/rk3399_common.h>
+
+
+#if defined(CONFIG_ENV_IS_IN_MMC)
+# define CONFIG_SYS_MMC_ENV_DEV		0
+#endif
+
+#define SDRAM_BANK_SIZE			(2UL << 30)
+
+#define CONFIG_SERIAL_TAG
+#define CONFIG_REVISION_TAG
+
+#endif

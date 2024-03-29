From 91cbd5f4fb25281319034c33b6e0c0f9b7d1e12b Mon Sep 17 00:00:00 2001
Message-Id: <91cbd5f4fb25281319034c33b6e0c0f9b7d1e12b.1585676333.git.aditya@kobol.io>
In-Reply-To: <ef8793354dbb6bce3027a3492ffc1fee3588f41f.1585676333.git.aditya@kobol.io>
References: <ef8793354dbb6bce3027a3492ffc1fee3588f41f.1585676333.git.aditya@kobol.io>
From: Aditya Prayoga <aditya@kobol.io>
Date: Wed, 4 Mar 2020 22:10:31 +0700
Subject: [PATCH 4/4] arm:rockchip:rk3399: Populate child node of syscon

U-Boot only populate first level of node.
Scan child node so device such as PHY can be initialized.
---
 arch/arm/mach-rockchip/rk3399/syscon_rk3399.c | 3 +++
 1 file changed, 3 insertions(+)

diff --git a/arch/arm/mach-rockchip/rk3399/syscon_rk3399.c b/arch/arm/mach-rockchip/rk3399/syscon_rk3399.c
index 259ca44d68..81b04aa7f8 100644
--- arch/arm/mach-rockchip/rk3399/syscon_rk3399.c.orig
+++ arch/arm/mach-rockchip/rk3399/syscon_rk3399.c
@@ -21,6 +21,9 @@ U_BOOT_DRIVER(syscon_rk3399) = {
 	.name = "rk3399_syscon",
 	.id = UCLASS_SYSCON,
 	.of_match = rk3399_syscon_ids,
+#if !CONFIG_IS_ENABLED(OF_PLATDATA)
+	.bind = dm_scan_fdt_dev,
+#endif
 };
 
 #if CONFIG_IS_ENABLED(OF_PLATDATA)

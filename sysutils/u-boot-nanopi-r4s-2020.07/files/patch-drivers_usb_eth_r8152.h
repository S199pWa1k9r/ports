--- drivers/usb/eth/r8152.h.orig	2021-03-22 05:18:30.808169000 +0300
+++ drivers/usb/eth/r8152.h	2021-03-22 05:20:11.543570000 +0300
@@ -26,6 +26,8 @@
 #define PLA_TEREDO_TIMER	0xd2cc
 #define PLA_REALWOW_TIMER	0xd2e8
 #define PLA_EXTRA_STATUS	0xd398
+#define PLA_EFUSE_DATA		0xdd00
+#define PLA_EFUSE_CMD		0xdd02
 #define PLA_LEDSEL		0xdd90
 #define PLA_LED_FEATURE		0xdd92
 #define PLA_PHYAR		0xde00
@@ -76,8 +78,10 @@
 #define USB_CSR_DUMMY2		0xb466
 #define USB_DEV_STAT		0xb808
 #define USB_CONNECT_TIMER	0xcbf8
+#define USB_MSC_TIMER		0xcbfc
 #define USB_BURST_SIZE		0xcfc0
 #define USB_FW_FIX_EN1		0xcfcc
+#define USB_LPM_CONFIG		0xcfd8
 #define USB_USB_CTRL		0xd406
 #define USB_PHY_CTRL		0xd408
 #define USB_TX_AGG		0xd40a
@@ -85,25 +89,24 @@
 #define USB_USB_TIMER		0xd428
 #define USB_RX_EARLY_TIMEOUT	0xd42c
 #define USB_RX_EARLY_SIZE	0xd42e
-#define USB_PM_CTRL_STATUS	0xd432
+#define USB_PM_CTRL_STATUS	0xd432	/* RTL8153A */
+#define USB_RX_EXTRA_AGGR_TMR	0xd432	/* RTL8153B */
 #define USB_TX_DMA		0xd434
+#define USB_UPT_RXDMA_OWN	0xd437
 #define USB_TOLERANCE		0xd490
 #define USB_LPM_CTRL		0xd41a
+#define USB_BMU_RESET		0xd4b0
+#define USB_U1U2_TIMER		0xd4da
 #define USB_UPS_CTRL		0xd800
-#define USB_MISC_0		0xd81a
 #define USB_POWER_CUT		0xd80a
+#define USB_MISC_0		0xd81a
 #define USB_AFE_CTRL2		0xd824
+#define USB_UPS_CFG		0xd842
 #define USB_WDT11_CTRL		0xe43c
-#define USB_BP_BA		0xfc26
-#define USB_BP_0		0xfc28
-#define USB_BP_1		0xfc2a
-#define USB_BP_2		0xfc2c
-#define USB_BP_3		0xfc2e
-#define USB_BP_4		0xfc30
-#define USB_BP_5		0xfc32
-#define USB_BP_6		0xfc34
-#define USB_BP_7		0xfc36
-#define USB_BP_EN		0xfc38
+#define USB_BP_BA		PLA_BP_BA
+#define USB_BP(n)		(0xfc28 + 2 * (n))
+#define USB_BP_EN		PLA_BP_EN	/* RTL8153A */
+#define USB_BP2_EN		0xfc48
 
 /* OCP Registers */
 #define OCP_ALDPS_CONFIG	0x2010
@@ -114,6 +117,7 @@
 #define OCP_EEE_AR		0xa41a
 #define OCP_EEE_DATA		0xa41c
 #define OCP_PHY_STATUS		0xa420
+#define OCP_NCTL_CFG		0xa42c
 #define OCP_POWER_CFG		0xa430
 #define OCP_EEE_CFG		0xa432
 #define OCP_SRAM_ADDR		0xa436
@@ -123,9 +127,11 @@
 #define OCP_EEE_ADV		0xa5d0
 #define OCP_EEE_LPABLE		0xa5d2
 #define OCP_PHY_STATE		0xa708		/* nway state for 8153 */
+#define OCP_ADC_IOFFSET		0xbcfc
 #define OCP_ADC_CFG		0xbc06
 
 /* SRAM Register */
+#define SRAM_GREEN_CFG		0x8011
 #define SRAM_LPF_CFG		0x8012
 #define SRAM_10M_AMP1		0x8080
 #define SRAM_10M_AMP2		0x8082
@@ -207,6 +213,7 @@
 /* PLA_PHY_PWR */
 #define PLA_PHY_PWR_LLR	(LINK_LIST_READY << 24)
 #define PLA_PHY_PWR_TXEMP	(TXFIFO_EMPTY << 24)
+#define TEST_IO_OFF		BIT(4)
 
 /* PLA_MISC_1 */
 #define RXDY_GATED_EN		0x0008
@@ -230,6 +237,10 @@
 /* PLA_BDC_CR */
 #define ALDPS_PROXY_MODE	0x0001
 
+/* PLA_EFUSE_CMD */
+#define EFUSE_READ_CMD		BIT(15)
+#define EFUSE_DATA_BIT16	BIT(7)
+
 /* PLA_CONFIG34 */
 #define LINK_ON_WAKE_EN		0x0010
 #define LINK_OFF_WAKE_EN	0x0008
@@ -255,8 +266,10 @@
 
 /* PLA_MAC_PWR_CTRL2 */
 #define EEE_SPDWN_RATIO		0x8007
+#define MAC_CLK_SPDWN_EN	BIT(15)
 
 /* PLA_MAC_PWR_CTRL3 */
+#define PLA_MCU_SPDWN_EN	BIT(14)
 #define PKT_AVAIL_SPDWN_EN	0x0100
 #define SUSPEND_SPDWN_EN	0x0004
 #define U1U2_SPDWN_EN		0x0002
@@ -312,6 +325,9 @@
 /* USB_FW_FIX_EN1 */
 #define FW_IP_RESET_EN		BIT(9)
 
+/* USB_LPM_CONFIG */
+#define LPM_U1U2_EN		BIT(0)
+
 /* USB_TX_AGG */
 #define TX_AGG_MAX_THRESHOLD	0x03
 
@@ -320,10 +336,21 @@
 #define RX_THR_HIGH		0x7a120180
 #define RX_THR_SLOW		0xffff0180
 
+/* USB_RX_EARLY_TIMEOUT */
+#define RX_AUXILIARY_TIMER	1264
+
 /* USB_TX_DMA */
 #define TEST_MODE_DISABLE	0x00000001
 #define TX_SIZE_ADJUST1		0x00000100
 
+/* USB_BMU_RESET */
+#define BMU_RESET_EP_IN		0x01
+#define BMU_RESET_EP_OUT	0x02
+
+/* USB_UPT_RXDMA_OWN */
+#define OWN_UPDATE		BIT(0)
+#define OWN_CLEAR		BIT(1)
+
 /* USB_UPS_CTRL */
 #define POWER_CUT		0x0100
 
@@ -366,6 +393,9 @@
 #define SEN_VAL_NORMAL		0xa000
 #define SEL_RXIDLE		0x0100
 
+/* USB_UPS_CFG */
+#define SAW_CNT_1MS_MASK	0x0fff
+
 /* OCP_ALDPS_CONFIG */
 #define ENPWRSAVE		0x8000
 #define ENPDNPS			0x0200
@@ -377,6 +407,9 @@
 #define PHY_STAT_LAN_ON		3
 #define PHY_STAT_PWRDN		5
 
+/* OCP_NCTL_CFG */
+#define PGA_RETURN_EN		BIT(1)
+
 /* OCP_POWER_CFG */
 #define EEE_CLKDIV_EN		0x8000
 #define EN_ALDPS		0x0004
@@ -429,6 +462,10 @@
 #define ADC_EN			0x0080
 #define EN_EMI_L		0x0040
 
+/* SRAM_GREEN_CFG */
+#define GREEN_ETH_EN		BIT(15)
+#define R_TUNE_EN		BIT(11)
+
 /* SRAM_LPF_CFG */
 #define LPF_AUTO_TUNE		0x8000
 
@@ -573,6 +610,8 @@
 	RTL_VER_05,
 	RTL_VER_06,
 	RTL_VER_07,
+	RTL_VER_08,
+	RTL_VER_09,
 	RTL_VER_MAX
 };
 
@@ -640,4 +679,5 @@
 
 void r8152b_firmware(struct r8152 *tp);
 void r8153_firmware(struct r8152 *tp);
+void r8153b_firmware(struct r8152 *tp);
 #endif

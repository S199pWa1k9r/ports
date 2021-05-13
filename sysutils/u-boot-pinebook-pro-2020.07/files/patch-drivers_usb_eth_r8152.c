--- drivers/usb/eth/r8152.c.orig	2021-03-22 05:18:04.689434000 +0300
+++ drivers/usb/eth/r8152.c	2021-03-22 05:20:11.542977000 +0300
@@ -68,6 +68,8 @@
 	{ 0x5c20, RTL_VER_05, 1 },
 	{ 0x5c30, RTL_VER_06, 1 },
 	{ 0x4800, RTL_VER_07, 0 },
+	{ 0x6000, RTL_VER_08, 1 },
+	{ 0x6010, RTL_VER_09, 1 },
 };
 
 static
@@ -331,6 +333,12 @@
 	ocp_reg_write(tp, OCP_SRAM_DATA, data);
 }
 
+static u16 sram_read(struct r8152 *tp, u16 addr)
+{
+	ocp_reg_write(tp, OCP_SRAM_ADDR, addr);
+	return ocp_reg_read(tp, OCP_SRAM_DATA);
+}
+
 int r8152_wait_for_bit(struct r8152 *tp, bool ocp_reg, u16 type, u16 index,
 		       const u32 mask, bool set, unsigned int timeout)
 {
@@ -439,6 +447,12 @@
 	ocp_write_dword(tp, MCU_TYPE_PLA, PLA_RCR, ocp_data);
 }
 
+static inline void r8153b_rx_agg_chg_indicate(struct r8152 *tp)
+{
+	ocp_write_byte(tp, MCU_TYPE_USB, USB_UPT_RXDMA_OWN,
+		       OWN_UPDATE | OWN_CLEAR);
+}
+
 static int rtl_enable(struct r8152 *tp)
 {
 	u32 ocp_data;
@@ -449,6 +463,15 @@
 	ocp_data |= PLA_CR_RE | PLA_CR_TE;
 	ocp_write_byte(tp, MCU_TYPE_PLA, PLA_CR, ocp_data);
 
+	switch (tp->version) {
+	case RTL_VER_08:
+	case RTL_VER_09:
+		r8153b_rx_agg_chg_indicate(tp);
+		break;
+	default:
+		break;
+	}
+
 	rxdy_gated_en(tp, false);
 
 	rtl8152_set_rx_mode(tp);
@@ -467,14 +490,56 @@
 {
 	u32 ocp_data = tp->coalesce / 8;
 
-	ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_TIMEOUT, ocp_data);
+	switch (tp->version) {
+	case RTL_VER_03:
+	case RTL_VER_04:
+	case RTL_VER_05:
+	case RTL_VER_06:
+		ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_TIMEOUT,
+			       ocp_data);
+		break;
+
+	case RTL_VER_08:
+	case RTL_VER_09:
+		/* The RTL8153B uses USB_RX_EXTRA_AGGR_TMR for rx timeout
+		 * primarily. For USB_RX_EARLY_TIMEOUT, we fix it to 1264ns.
+		 */
+		ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_TIMEOUT,
+			       RX_AUXILIARY_TIMER / 8);
+		ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EXTRA_AGGR_TMR,
+			       ocp_data);
+		break;
+
+	default:
+		debug("** %s Invalid Device\n", __func__);
+		break;
+	}
 }
 
 static void r8153_set_rx_early_size(struct r8152 *tp)
 {
-	u32 ocp_data = (RTL8152_AGG_BUF_SZ - RTL8153_RMS) / 4;
+	u32 ocp_data = (RTL8152_AGG_BUF_SZ - RTL8153_RMS -
+			sizeof(struct rx_desc));
 
-	ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_SIZE, ocp_data);
+	switch (tp->version) {
+	case RTL_VER_03:
+	case RTL_VER_04:
+	case RTL_VER_05:
+	case RTL_VER_06:
+		ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_SIZE,
+			       ocp_data / 4);
+		break;
+
+	case RTL_VER_08:
+	case RTL_VER_09:
+		ocp_write_word(tp, MCU_TYPE_USB, USB_RX_EARLY_SIZE,
+			       ocp_data / 8);
+		break;
+
+	default:
+		debug("** %s Invalid Device\n", __func__);
+		break;
+	}
 }
 
 static int rtl8153_enable(struct r8152 *tp)
@@ -540,6 +605,19 @@
 	usb_ocp_write(tp, USB_TOLERANCE, BYTE_EN_SIX_BYTES, sizeof(u1u2), u1u2);
 }
 
+static void r8153b_u1u2en(struct r8152 *tp, bool enable)
+{
+	u16 ocp_data;
+
+	ocp_data = ocp_read_word(tp, MCU_TYPE_USB, USB_LPM_CONFIG);
+	if (enable)
+		ocp_data |= LPM_U1U2_EN;
+	else
+		ocp_data &= ~LPM_U1U2_EN;
+
+	ocp_write_word(tp, MCU_TYPE_USB, USB_LPM_CONFIG, ocp_data);
+}
+
 static void r8153_u2p3en(struct r8152 *tp, bool enable)
 {
 	u32 ocp_data;
@@ -568,6 +646,17 @@
 	ocp_write_word(tp, MCU_TYPE_USB, USB_MISC_0, ocp_data);
 }
 
+static void rtl_reset_bmu(struct r8152 *tp)
+{
+	u8 ocp_data;
+
+	ocp_data = ocp_read_byte(tp, MCU_TYPE_USB, USB_BMU_RESET);
+	ocp_data &= ~(BMU_RESET_EP_IN | BMU_RESET_EP_OUT);
+	ocp_write_byte(tp, MCU_TYPE_USB, USB_BMU_RESET, ocp_data);
+	ocp_data |= BMU_RESET_EP_IN | BMU_RESET_EP_OUT;
+	ocp_write_byte(tp, MCU_TYPE_USB, USB_BMU_RESET, ocp_data);
+}
+
 static int r8152_read_mac(struct r8152 *tp, unsigned char *macaddr)
 {
 	int ret;
@@ -773,6 +862,71 @@
 	sram_write(tp, SRAM_10M_AMP2, 0x0208);
 }
 
+static u32 r8152_efuse_read(struct r8152 *tp, u8 addr)
+{
+	u32 ocp_data;
+
+	ocp_write_word(tp, MCU_TYPE_PLA, PLA_EFUSE_CMD, EFUSE_READ_CMD | addr);
+	ocp_data = ocp_read_word(tp, MCU_TYPE_PLA, PLA_EFUSE_CMD);
+	ocp_data = (ocp_data & EFUSE_DATA_BIT16) << 9;	/* data of bit16 */
+	ocp_data |= ocp_read_word(tp, MCU_TYPE_PLA, PLA_EFUSE_DATA);
+
+	return ocp_data;
+}
+
+static void r8153b_hw_phy_cfg(struct r8152 *tp)
+{
+	u32 ocp_data;
+	u16 data;
+
+	data = r8152_mdio_read(tp, MII_BMCR);
+	if (data & BMCR_PDOWN) {
+		data &= ~BMCR_PDOWN;
+		r8152_mdio_write(tp, MII_BMCR, data);
+	}
+
+	/* U1/U2/L1 idle timer. 500 us */
+	ocp_write_word(tp, MCU_TYPE_USB, USB_U1U2_TIMER, 500);
+
+	r8153b_firmware(tp);
+
+	data = sram_read(tp, SRAM_GREEN_CFG);
+	data |= R_TUNE_EN;
+	sram_write(tp, SRAM_GREEN_CFG, data);
+	data = ocp_reg_read(tp, OCP_NCTL_CFG);
+	data |= PGA_RETURN_EN;
+	ocp_reg_write(tp, OCP_NCTL_CFG, data);
+
+	/* ADC Bias Calibration:
+	 * read efuse offset 0x7d to get a 17-bit data. Remove the dummy/fake
+	 * bit (bit3) to rebuild the real 16-bit data. Write the data to the
+	 * ADC ioffset.
+	 */
+	ocp_data = r8152_efuse_read(tp, 0x7d);
+	ocp_data = ((ocp_data & 0x1fff0) >> 1) | (ocp_data & 0x7);
+	if (ocp_data != 0xffff)
+		ocp_reg_write(tp, OCP_ADC_IOFFSET, ocp_data);
+
+	/* ups mode tx-link-pulse timing adjustment:
+	 * rg_saw_cnt = OCP reg 0xC426 Bit[13:0]
+	 * swr_cnt_1ms_ini = 16000000 / rg_saw_cnt
+	 */
+	ocp_data = ocp_reg_read(tp, 0xc426);
+	ocp_data &= 0x3fff;
+	if (ocp_data) {
+		u32 swr_cnt_1ms_ini;
+
+		swr_cnt_1ms_ini = (16000000 / ocp_data) & SAW_CNT_1MS_MASK;
+		ocp_data = ocp_read_word(tp, MCU_TYPE_USB, USB_UPS_CFG);
+		ocp_data = (ocp_data & ~SAW_CNT_1MS_MASK) | swr_cnt_1ms_ini;
+		ocp_write_word(tp, MCU_TYPE_USB, USB_UPS_CFG, ocp_data);
+	}
+
+	ocp_data = ocp_read_word(tp, MCU_TYPE_PLA, PLA_PHY_PWR);
+	ocp_data |= PFM_PWM_SWITCH;
+	ocp_write_word(tp, MCU_TYPE_PLA, PLA_PHY_PWR, ocp_data);
+}
+
 static void r8153_first_init(struct r8152 *tp)
 {
 	u32 ocp_data;
@@ -786,6 +940,7 @@
 	r8153_hw_phy_cfg(tp);
 
 	rtl8152_nic_reset(tp);
+	rtl_reset_bmu(tp);
 
 	ocp_data = ocp_read_byte(tp, MCU_TYPE_PLA, PLA_OOB_CTRL);
 	ocp_data &= ~NOW_IS_OOB;
@@ -832,6 +987,7 @@
 	ocp_write_byte(tp, MCU_TYPE_PLA, PLA_OOB_CTRL, ocp_data);
 
 	rtl_disable(tp);
+	rtl_reset_bmu(tp);
 
 	rtl8152_reinit_ll(tp);
 
@@ -873,6 +1029,7 @@
 {
 	r8153_disable_aldps(tp);
 	rtl_disable(tp);
+	rtl_reset_bmu(tp);
 }
 
 static int rtl8152_set_speed(struct r8152 *tp, u8 autoneg, u16 speed, u8 duplex)
@@ -933,7 +1090,7 @@
 			return -EINVAL;
 		}
 
-		bmcr = BMCR_ANENABLE | BMCR_ANRESTART;
+		bmcr = BMCR_ANENABLE | BMCR_ANRESTART | BMCR_RESET;
 	}
 
 	if (tp->supports_gmii)
@@ -977,6 +1134,16 @@
 	r8153_enter_oob(tp);
 }
 
+static void rtl8153b_up(struct r8152 *tp)
+{
+	r8153_first_init(tp);
+}
+
+static void rtl8153b_down(struct r8152 *tp)
+{
+	r8153_enter_oob(tp);
+}
+
 static void r8152b_get_version(struct r8152 *tp)
 {
 	u32 ocp_data;
@@ -1140,6 +1307,60 @@
 	rtl_tally_reset(tp);
 }
 
+static void r8153b_init(struct r8152 *tp)
+{
+	u32 ocp_data;
+	int i;
+
+	r8153_disable_aldps(tp);
+	r8153b_u1u2en(tp, false);
+
+	r8152_wait_for_bit(tp, 0, MCU_TYPE_PLA, PLA_BOOT_CTRL,
+			   AUTOLOAD_DONE, 1, R8152_WAIT_TIMEOUT);
+
+	for (i = 0; i < R8152_WAIT_TIMEOUT; i++) {
+		ocp_data = ocp_reg_read(tp, OCP_PHY_STATUS) & PHY_STAT_MASK;
+		if (ocp_data == PHY_STAT_LAN_ON || ocp_data == PHY_STAT_PWRDN)
+			break;
+
+		mdelay(1);
+	}
+
+	r8153_u2p3en(tp, false);
+
+	/* MSC timer = 0xfff * 8ms = 32760 ms */
+	ocp_write_word(tp, MCU_TYPE_USB, USB_MSC_TIMER, 0x0fff);
+
+	r8153_power_cut_en(tp, false);
+
+	/* MAC clock speed down */
+	ocp_data = ocp_read_word(tp, MCU_TYPE_PLA, PLA_MAC_PWR_CTRL2);
+	ocp_data |= MAC_CLK_SPDWN_EN;
+	ocp_write_word(tp, MCU_TYPE_PLA, PLA_MAC_PWR_CTRL2, ocp_data);
+
+	ocp_data = ocp_read_word(tp, MCU_TYPE_PLA, PLA_MAC_PWR_CTRL3);
+	ocp_data &= ~PLA_MCU_SPDWN_EN;
+	ocp_write_word(tp, MCU_TYPE_PLA, PLA_MAC_PWR_CTRL3, ocp_data);
+
+	if (tp->version == RTL_VER_09) {
+		/* Disable Test IO for 32QFN */
+		if (ocp_read_byte(tp, MCU_TYPE_PLA, 0xdc00) & BIT(5)) {
+			ocp_data = ocp_read_word(tp, MCU_TYPE_PLA, PLA_PHY_PWR);
+			ocp_data |= TEST_IO_OFF;
+			ocp_write_word(tp, MCU_TYPE_PLA, PLA_PHY_PWR, ocp_data);
+		}
+	}
+
+	/* rx aggregation */
+	ocp_data = ocp_read_word(tp, MCU_TYPE_USB, USB_USB_CTRL);
+	ocp_data &= ~(RX_AGG_DISABLE | RX_ZERO_EN);
+	ocp_write_word(tp, MCU_TYPE_USB, USB_USB_CTRL, ocp_data);
+
+	rtl_tally_reset(tp);
+	r8153b_hw_phy_cfg(tp);
+	r8152b_enable_fc(tp);
+}
+
 static void rtl8152_unload(struct r8152 *tp)
 {
 	if (tp->version != RTL_VER_01)
@@ -1180,6 +1401,15 @@
 		ops->unload		= rtl8153_unload;
 		break;
 
+	case RTL_VER_08:
+	case RTL_VER_09:
+		ops->init		= r8153b_init;
+		ops->enable		= rtl8153_enable;
+		ops->disable		= rtl8153_disable;
+		ops->up			= rtl8153b_up;
+		ops->down		= rtl8153b_down;
+		break;
+
 	default:
 		ret = -ENODEV;
 		printf("r8152 Unknown Device\n");
@@ -1249,7 +1479,7 @@
 }
 
 #ifndef CONFIG_DM_ETH
-static int r8152_init(struct eth_device *eth, bd_t *bd)
+static int r8152_init(struct eth_device *eth, struct bd_info *bd)
 {
 	struct ueth_data *dev = (struct ueth_data *)eth->priv;
 	struct r8152 *tp = (struct r8152 *)dev->dev_priv;
@@ -1430,7 +1660,7 @@
 	if (usb_set_interface(dev, iface_desc->bInterfaceNumber, 0) ||
 	    !ss->ep_in || !ss->ep_out || !ss->ep_int) {
 		debug("Problems with device\n");
-		return 0;
+		goto error;
 	}
 
 	dev->privptr = (void *)ss;
@@ -1442,7 +1672,7 @@
 	r8152b_get_version(tp);
 
 	if (rtl_ops_init(tp))
-		return 0;
+		goto error;
 
 	tp->rtl_ops.init(tp);
 	tp->rtl_ops.up(tp);
@@ -1452,6 +1682,11 @@
 			  DUPLEX_FULL);
 
 	return 1;
+
+error:
+	cfree(ss->dev_priv);
+	ss->dev_priv = 0;
+	return 0;
 }
 
 int r8152_eth_get_info(struct usb_device *dev, struct ueth_data *ss,

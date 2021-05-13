--- drivers/mtd/spi/spi-nor-ids.c.orig
+++ drivers/mtd/spi/spi-nor-ids.c
@@ -319,6 +319,11 @@ const struct flash_info spi_nor_ids[] = {
 	/* XMC (Wuhan Xinxin Semiconductor Manufacturing Corp.) */
 	{ INFO("XM25QH64A", 0x207017, 0, 64 * 1024, 128, SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
 	{ INFO("XM25QH128A", 0x207018, 0, 64 * 1024, 256, SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
+#endif
+#ifdef CONFIG_SPI_FLASH_XTX
+	/* XTX (Shenzhen Xin Tian Xia Tech) */
+	{ INFO("XT25F32B", 0x0b4016, 0, 64 * 1024, 64, SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
+	{ INFO("XT25F128B", 0x0b4018, 0, 64 * 1024, 256, SECT_4K | SPI_NOR_DUAL_READ | SPI_NOR_QUAD_READ) },
 #endif
 	{ },
 };

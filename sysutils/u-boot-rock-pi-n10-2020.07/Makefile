# $FreeBSD$

MASTERDIR=      ${.CURDIR}/../u-boot-2020.07

MODEL=		rock-pi-n10
BOARD_CONFIG=	rock-pi-n10-rk3399pro_defconfig
FAMILY=		rk3399

EXTRA_PATCHES=	${.CURDIR}/files/

UBOOT_PLIST_RK3399=	idbloader.img u-boot.itb splash.bmp ${UBOOT_SPI}
UBOOT_MOVE_RK3399=	${.CURDIR}/files/splash.bmp
UBOOT_SPI_PAYLOAD=	0x60000
UBOOT_SPI=		u-boot-spi.bin

.include "${MASTERDIR}/Makefile"

post-build:
	mkimage -n rk3399 -T rkspi -d ${WRKSRC}/tpl/u-boot-tpl.bin:${WRKSRC}/spl/u-boot-spl.bin \
		${WRKSRC}/${UBOOT_SPI} >/dev/null && cat /dev/null
	truncate -s ${UBOOT_SPI_PAYLOAD} ${WRKSRC}/${UBOOT_SPI}
	cat ${WRKSRC}/u-boot.itb >> ${WRKSRC}/${UBOOT_SPI}

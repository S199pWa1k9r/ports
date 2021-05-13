# $FreeBSD$

MASTERDIR=	${.CURDIR}/../u-boot-2020.07

MODEL=		nanopc-t4
BOARD_CONFIG=	nanopc-t4-rk3399_defconfig
FAMILY=		rk3399

UBOOT_PLIST_RK3399=	idbloader.img u-boot.itb splash.bmp
UBOOT_MOVE_RK3399=	${.CURDIR}/files/splash.bmp

EXTRA_PATCHES=  ${.CURDIR}/files/

.include "${MASTERDIR}/Makefile"

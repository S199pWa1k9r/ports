# $FreeBSD: head/sysutils/atf-rk3399/Makefile 492252 2019-02-05 15:45:52Z manu $

MASTERDIR=	${.CURDIR}/../atf-master-v2.3

PLAT=	rk3399
BL31=	bl31/bl31.elf

# ATF build a firmware that runs on the Cortex-M0
BUILD_DEPENDS+=	arm-none-eabi-gcc:devel/arm-none-eabi-gcc

.include "${MASTERDIR}/Makefile"

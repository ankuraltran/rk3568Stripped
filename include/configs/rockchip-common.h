/*
 * (C) Copyright 2016 Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:     GPL-2.0+
 */

#ifndef _ROCKCHIP_COMMON_H_
#define _ROCKCHIP_COMMON_H_
#include <linux/sizes.h>

#define COUNTER_FREQUENCY               24000000

#if CONFIG_IS_ENABLED(TINY_FRAMEWORK) && !defined(CONFIG_ARM64)
#undef CONFIG_SYS_ARCH_TIMER
#else
#define CONFIG_SYS_ARCH_TIMER
#endif

#define CONFIG_SYS_HZ_CLOCK	24000000

#define CONFIG_SYS_NS16550_MEM32

#ifdef CONFIG_SPL_BUILD
#define CONFIG_NR_DRAM_BANKS		2
#else
#define CONFIG_NR_DRAM_BANKS		12
#endif

#ifndef CONFIG_SPL_BUILD
#include <config_distro_defaults.h>

#ifdef CONFIG_CMD_RKNAND
#define BOOTENV_SHARED_RKNAND	BOOTENV_SHARED_BLKDEV(rknand)
#define BOOTENV_DEV_RKNAND		BOOTENV_DEV_BLKDEV
#define BOOTENV_DEV_NAME_RKNAND	BOOTENV_DEV_NAME_BLKDEV
#else
#define BOOTENV_SHARED_RKNAND
#define BOOTENV_DEV_RKNAND \
	BOOT_TARGET_DEVICES_references_RKNAND_without_CONFIG_CMD_RKNAND
#define BOOTENV_DEV_NAME_RKNAND \
	BOOT_TARGET_DEVICES_references_RKNAND_without_CONFIG_CMD_RKNAND
#endif

#ifdef CONFIG_CMD_NAND
#define BOOTENV_SHARED_NAND	BOOTENV_SHARED_BLKDEV(mtd)
#define BOOTENV_DEV_NAND(devtypeu, devtypel, instance) \
	"bootcmd_" #devtypel #instance "=" \
		"setenv devnum " #instance "; " \
		"run scan_dev_for_boot_part\0"
#define BOOTENV_DEV_NAME_NAND	BOOTENV_DEV_NAME_BLKDEV
#else
#define BOOTENV_SHARED_NAND
#define BOOTENV_DEV_NAND \
	BOOT_TARGET_DEVICES_references_NAND_without_CONFIG_CMD_NAND
#define BOOTENV_DEV_NAME_NAND \
	BOOT_TARGET_DEVICES_references_NAND_without_CONFIG_CMD_NAND
#endif

/* First try to boot from SD (index 1), then eMMC (index 0) */
#if CONFIG_IS_ENABLED(CMD_MMC)
	#define BOOT_TARGET_MMC(func) \
		func(MMC, mmc, 1) \
		func(MMC, mmc, 0)
#else
	#define BOOT_TARGET_MMC(func)
#endif

#if CONFIG_IS_ENABLED(CMD_RKNAND)
	#define BOOT_TARGET_RKNAND(func) func(RKNAND, rknand, 0)
#else
	#define BOOT_TARGET_RKNAND(func)
#endif

#if CONFIG_IS_ENABLED(CMD_NAND)
	#define BOOT_TARGET_NAND(func) \
		func(NAND, mtd, 1) \
		func(NAND, mtd, 0)
#else
	#define BOOT_TARGET_NAND(func)
#endif

#if CONFIG_IS_ENABLED(CMD_USB)
	#define BOOT_TARGET_USB(func) func(USB, usb, 0)
#else
	#define BOOT_TARGET_USB(func)
#endif

#if CONFIG_IS_ENABLED(CMD_PXE)
	#define BOOT_TARGET_PXE(func) func(PXE, pxe, na)
#else
	#define BOOT_TARGET_PXE(func)
#endif

#if CONFIG_IS_ENABLED(CMD_DHCP)
	#define BOOT_TARGET_DHCP(func) func(DHCP, dhcp, na)
#else
	#define BOOT_TARGET_DHCP(func)
#endif

#define BOOT_TARGET_DEVICES(func) \
	BOOT_TARGET_MMC(func) \
	BOOT_TARGET_RKNAND(func) \
	BOOT_TARGET_NAND(func) \
	BOOT_TARGET_USB(func) \
	BOOT_TARGET_PXE(func) \
	BOOT_TARGET_DHCP(func)

#ifdef CONFIG_ARM64
#define ROOT_UUID "B921B045-1DF0-41C3-AF44-4C6F280D3FAE;\0"
#else
#define ROOT_UUID "69DAD710-2CE4-4E3C-B16C-21A1D49ABED3;\0"
#endif
#define PARTS_DEFAULT \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=loader1,start=32K,size=4000K,uuid=${uuid_gpt_loader1};" \
	"name=loader2,start=8MB,size=4MB,uuid=${uuid_gpt_loader2};" \
	"name=trust,size=4M,uuid=${uuid_gpt_atf};" \
	"name=boot,size=112M,bootable,uuid=${uuid_gpt_boot};" \
	"name=rootfs,size=-,uuid="ROOT_UUID

#define PARTS_RKIMG \
	"uuid_disk=${uuid_gpt_disk};" \
	"name=uboot,start=8MB,size=4MB,uuid=${uuid_gpt_loader2};" \
	"name=trust,size=4M,uuid=${uuid_gpt_atf};" \
	"name=misc,size=4MB,uuid=${uuid_gpt_misc};" \
	"name=resource,size=16MB,uuid=${uuid_gpt_resource};" \
	"name=kernel,size=32M,uuid=${uuid_gpt_kernel};" \
	"name=boot,size=32M,bootable,uuid=${uuid_gpt_boot};" \
	"name=recovery,size=32M,uuid=${uuid_gpt_recovery};" \
	"name=backup,size=112M,uuid=${uuid_gpt_backup};" \
	"name=cache,size=512M,uuid=${uuid_gpt_cache};" \
	"name=system,size=2048M,uuid=${uuid_gpt_system};" \
	"name=metadata,size=16M,uuid=${uuid_gpt_metadata};" \
	"name=vendor,size=32M,uuid=${uuid_gpt_vendor};" \
	"name=oem,size=32M,uuid=${uuid_gpt_oem};" \
	"name=frp,size=512K,uuid=${uuid_gpt_frp};" \
	"name=security,size=2M,uuid=${uuid_gpt_security};" \
	"name=userdata,size=-,uuid=${uuid_gpt_userdata};\0"

#ifdef CONFIG_DM_RAMDISK
#define RKIMG_DET_BOOTDEV \
	"rkimg_bootdev=" \
	"setenv devtype ramdisk; setenv devnum 0; \0"
#else
#define RKIMG_DET_BOOTDEV \
	"rkimg_bootdev=" \
	"if mmc dev 1 && rkimgtest mmc 1; then " \
		"setenv devtype mmc; setenv devnum 1; echo Boot from SDcard;" \
	"elif mmc dev 0; then " \
		"setenv devtype mmc; setenv devnum 0;" \
	"elif mtd_blk dev 0; then " \
		"setenv devtype mtd; setenv devnum 0;" \
	"elif mtd_blk dev 1; then " \
		"setenv devtype mtd; setenv devnum 1;" \
	"elif mtd_blk dev 2; then " \
		"setenv devtype mtd; setenv devnum 2;" \
	"elif rknand dev 0; then " \
		"setenv devtype rknand; setenv devnum 0;" \
	"elif rksfc dev 0; then " \
		"setenv devtype spinand; setenv devnum 0;" \
	"elif rksfc dev 1; then " \
		"setenv devtype spinor; setenv devnum 1;" \
	"fi; \0"
#endif

#if defined(CONFIG_AVB_VBMETA_PUBLIC_KEY_VALIDATE)
#define RKIMG_BOOTCOMMAND			\
	"boot_android ${devtype} ${devnum};"
#else
#define RKIMG_BOOTCOMMAND			\
	"boot_android ${devtype} ${devnum};"	\
	"bootrkp;"				\
	"run distro_bootcmd;"
#endif

#endif /* CONFIG_SPL_BUILD */

#define CONFIG_DISPLAY_BOARDINFO_LATE

#endif /* _ROCKCHIP_COMMON_H_ */

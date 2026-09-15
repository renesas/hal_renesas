# SPDX-License-Identifier: Apache-2.0

zephyr_include_directories(drv/inc)

if(CONFIG_USE_RCARSOC_DRV_DMAC)
	file(GLOB rcarsoc_drv_dmac_srcs
			"drv/src/dmac/dmac_ctrl.c"
			"drv/src/dmac/dmac_reg.c"
			"drv/src/dmac/rdmac_ctrl.c"
			"drv/src/dmac/sysdmac_ctrl.c")
	zephyr_library_sources(${rcarsoc_drv_dmac_srcs})
	zephyr_include_directories(drv/src/dmac)
	zephyr_include_directories(drv/inc/dmac)
endif()

if(CONFIG_USE_RCARSOC_DRV_MFIS)
	file(GLOB rcarsoc_drv_mfis_srcs
			"drv/src/mfis/mfis.c")
	zephyr_library_sources(${rcarsoc_drv_mfis_srcs})
	zephyr_include_directories(drv/src/mfis)
	zephyr_include_directories(drv/inc/mfis)
endif()

if(CONFIG_USE_RCARSOC_DRV_ECM)
	file(GLOB rcarsoc_drv_ecm_srcs
			"drv/src/ecm/r_ecm.c")
	zephyr_library_sources(${rcarsoc_drv_ecm_srcs})
	zephyr_include_directories(drv/inc/ecm)
endif()

if(CONFIG_USE_RCARSOC_DRV_WATCHDOG)
	zephyr_include_directories(drv/inc/watchdog)
endif()

if(CONFIG_USE_RCARSOC_DRV_OSTM)
	zephyr_library_sources(
		fsp/src/r_ostm/r_ostm.c
		fsp/src/bsp/mcu/all/bsp_irq.c)

	zephyr_include_directories(
		fsp/inc
		fsp/inc/api
		fsp/inc/instances
		fsp/src/bsp/mcu/all
		fsp/src/bsp/mcu/x5h
		fsp/src/bsp/rcarsoc/Device/RENESAS/Include)

	zephyr_include_directories_ifdef(CONFIG_SOC_R8A78000_R52
		fsp/src/bsp/mcu/x5h/r8a78000)
endif()

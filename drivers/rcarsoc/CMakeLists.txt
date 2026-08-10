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
endif()


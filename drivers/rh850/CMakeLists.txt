# SPDX-License-Identifier: Apache-2.0

set(include_dirs
	fsp/inc
	fsp/inc/api
	fsp/inc/instances
	fsp/src/bsp/mcu/all
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}
	fsp/src/bsp/rh850/Device/RENESAS/Include
)

set(srcs
	fsp/src/bsp/mcu/all/bsp_barr.c
	fsp/src/bsp/mcu/all/bsp_clocks.c
	fsp/src/bsp/mcu/all/bsp_delay.c
	fsp/src/bsp/mcu/all/bsp_guard.c
	fsp/src/bsp/mcu/all/bsp_io.c
	fsp/src/bsp/mcu/all/bsp_irq.c
	fsp/src/bsp/mcu/all/bsp_mcu_reset.c
	fsp/src/bsp/mcu/all/bsp_mcu_standby_cyclicrun.c
	fsp/src/bsp/mcu/all/bsp_module_standby.c
	fsp/src/bsp/mcu/all/bsp_mpu.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_clocks_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_guard_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_module_standby_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_opbt.c
)

zephyr_library_sources(${srcs})
zephyr_include_directories(${include_dirs})

if(CONFIG_SOC_R7F70255xFABB_C 
	OR CONFIG_SOC_R7F70255xAFABB_C 
	OR CONFIG_SOC_R7F70255xBFABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2b6)

elseif(CONFIG_SOC_R7F70254xFABG_C 
	OR CONFIG_SOC_R7F70254xFABA_C 
	OR CONFIG_SOC_R7F70254xFABB_C
	OR CONFIG_SOC_R7F70254xAFABG_C 
	OR CONFIG_SOC_R7F70254xAFABA_C 
	OR CONFIG_SOC_R7F70254xAFABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2b10)

elseif(CONFIG_SOC_R7F702600AFABB_C 
	OR CONFIG_SOC_R7F702600FABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2c8)

endif()


# Optional build base on feature configuration
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_IOPORT
 	fsp/src/r_ioport/r_ioport.c)

zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RLIN3_UART
 	fsp/src/r_rlin3_uart/r_rlin3_uart.c)

zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_OSTM
 	fsp/src/r_ostm/r_ostm.c)	

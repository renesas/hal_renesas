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
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_opbt_driver.c
)

zephyr_library_sources(${srcs})
zephyr_include_directories(${include_dirs})

if(CONFIG_SOC_R7F702300AEBBC_C
	OR CONFIG_SOC_R7F702300BEBBC_C
	OR CONFIG_SOC_R7F702300EBBG_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2a16)

elseif(CONFIG_SOC_R7F70255XFABB_C 
	OR CONFIG_SOC_R7F70255XAFABB_C 
	OR CONFIG_SOC_R7F70255XBFABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2b6)

elseif(CONFIG_SOC_R7F70254XFABG_C 
	OR CONFIG_SOC_R7F70254XFABA_C 
	OR CONFIG_SOC_R7F70254XFABB_C
	OR CONFIG_SOC_R7F70254XAFABG_C 
	OR CONFIG_SOC_R7F70254XAFABA_C 
	OR CONFIG_SOC_R7F70254XAFABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2b10)

elseif(CONFIG_SOC_R7F702600AFABB_C 
	OR CONFIG_SOC_R7F702600FABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2c8)

endif()


# Optional build base on feature configuration

# ADCJ
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_ADCJ
	fsp/src/r_adcj/r_adcj.c)

# ADCK
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_ADCK
	fsp/src/r_adck/r_adck.c)

# CANFD
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_CANFD
	fsp/src/r_canfd/r_canfd.c)

# CGC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_CGC
	fsp/src/r_cgc/r_cgc.c)

# CRC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_CRC
	fsp/src/r_crc/r_crc.c)

# DMAC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_DMAC
	fsp/src/r_dmac/r_dmac.c)

# DTSC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_DTSC
	fsp/src/r_dtsc/r_dtsc.c)

# ETHER_T1S
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_ETHER_ETNF
	fsp/src/r_ether_phy/targets/CT25205_RTL/r_ether_phy_target_ct25205_rtl.c
	fsp/src/r_ether_phy/r_ether_phy.c
	fsp/src/r_ether_t1s/r_ether_t1s.c
)

# ICU
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_EXT_IRQ
	fsp/src/r_icu/r_icu.c)

# IIC_B
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_IIC_B
	fsp/src/r_iic_b_master/r_iic_b_master.c
	fsp/src/r_iic_b_slave/r_iic_b_slave.c
)

# IIC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_IIC
	fsp/src/r_iic_master/r_iic_master.c
	fsp/src/r_iic_slave/r_iic_slave.c
)

# IOPORT
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_IOPORT
	fsp/src/r_ioport/r_ioport.c)

# MSPI
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_MSPI
	fsp/src/r_mspi/r_mspi.c
)

# OSPI
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_OSPI
	fsp/src/r_ospi/r_ospi.c
)

# OSTM
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_OSTM
	fsp/src/r_ostm/r_ostm.c)

# PIC
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_PIC
	fsp/src/r_pic/r_pic.c
)

# RLIN3_UART
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RLIN3_UART
	fsp/src/r_rlin3_uart/r_rlin3_uart.c)

# RTCA
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RTCA
	fsp/src/r_rtca/r_rtca.c)

# TAUD
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_TAUD
	fsp/src/r_taud/r_taud.c)

# WDT
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_WDT
	fsp/src/r_wdt/r_wdt.c)

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
	fsp/src/bsp/mcu/all/bsp_common.c
	fsp/src/bsp/mcu/all/bsp_fault_trace.c
	fsp/src/bsp/mcu/all/bsp_sbrk.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_clocks_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_guard_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_module_standby_driver.c
	fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/bsp_opbt_driver.c
)

zephyr_library_sources(${srcs})
zephyr_include_directories(${include_dirs})

if(CONFIG_SOC_R7F702300AEBBC_C
	OR CONFIG_SOC_R7F702300BEBBC_C
	OR CONFIG_SOC_R7F702300EBBG_C
	OR CONFIG_SOC_R7F702300AEBBB_C
	OR CONFIG_SOC_R7F702300BEBBB_C
	OR CONFIG_SOC_R7F702300EBBB_C
	OR CONFIG_SOC_R7F702300AFABA_C
	OR CONFIG_SOC_R7F702300BFABA_C
	OR CONFIG_SOC_R7F702300EABA_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2a16)

elseif(CONFIG_SOC_R7F702301AEBBA_C
	OR CONFIG_SOC_R7F702301BEBBA_C
	OR CONFIG_SOC_R7F702301EBBA_C
	OR CONFIG_SOC_R7F702301AFABG_C
	OR CONFIG_SOC_R7F702301BFABG_C
	OR CONFIG_SOC_R7F702301EABG_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2a8)

elseif(CONFIG_SOC_R7F702302FABB_C
	OR CONFIG_SOC_R7F702302FAFK_C
	OR CONFIG_SOC_R7F702302FABD_C
	OR CONFIG_SOC_R7F702302FAFM_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2a6)

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

elseif(CONFIG_SOC_R7F702600AFABB_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2c8)

elseif(CONFIG_SOC_R7F702606AFABB_C
	OR CONFIG_SOC_R7F702616AFABB_C
	OR CONFIG_SOC_R7F702613AFABB_C
	OR CONFIG_SOC_R7F702606AFAFB_C
	OR CONFIG_SOC_R7F702606AFAFM_C
	OR CONFIG_SOC_R7F702616AFAFM_C
	OR CONFIG_SOC_R7F702616AFAFB_C
	OR CONFIG_SOC_R7F702614AFAFM_C
	OR CONFIG_SOC_R7F702614AFAFB_C
	OR CONFIG_SOC_R7F702606AFAFD_C
	OR CONFIG_SOC_R7F702606AFAFD_C
	OR CONFIG_SOC_R7F702613AFAFD_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2c4)

elseif(CONFIG_SOC_R7F702615AFAFM_C 
	OR CONFIG_SOC_R7F702615AFAFB_C
	OR CONFIG_SOC_R7F702608AFAFD_C)

	zephyr_include_directories(fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}/rh850u2c2)

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

# ETHER_PHY: r_ether_phy.c is shared by ETNF/T1S and ETND/TSN
if(CONFIG_USE_RH850_FSP_ETHER_ETNF OR CONFIG_USE_RH850_FSP_ETHER_ETND)
	zephyr_library_sources(fsp/src/r_ether_phy/r_ether_phy.c)
endif()

# CT25205 target: T1S PLCA PHY for ETNF
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_ETHER_ETNF
	fsp/src/r_ether_phy/targets/CT25205_RTL/r_ether_phy_target_ct25205_rtl.c
)

# I3C
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_I3C
	fsp/src/r_i3c/r_i3c.c)

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

# MMCA
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_MMCA
	fsp/src/r_mmca/r_mmca.c)

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

# PSI5
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_PSI5
	fsp/src/r_psi5/r_psi5.c
)

# TAUD_THREE_PHASE
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_TAUD_THREE_PHASE
	fsp/src/r_taud_three_phase/r_taud_three_phase.c
)

# RLIN3_LIN
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RLIN3_LIN
	fsp/src/r_rlin3_lin/r_rlin3_lin.c)

# RLIN3_UART
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RLIN3_UART
	fsp/src/r_rlin3_uart/r_rlin3_uart.c)

# RSENT_SENT
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RSENT_SENT
	fsp/src/r_rsent_sent/r_rsent_sent.c)

# RTCA
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_RTCA
	fsp/src/r_rtca/r_rtca.c)

# SFMA
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_SFMA
	fsp/src/r_sfma/r_sfma.c)

# SSI
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_SSI
	fsp/src/r_ssi/r_ssi.c)

# TAPA
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_TAPA
	fsp/src/r_tapa/r_tapa.c)

# TAUD
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_TAUD
	fsp/src/r_taud/r_taud.c)

# WDT
zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_WDT
	fsp/src/r_wdt/r_wdt.c)

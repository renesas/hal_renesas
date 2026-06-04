# SPDX-License-Identifier: Apache-2.0

if(CONFIG_HAS_RENESAS_RX_RDP)
	set(include_dirs
		rdp/src/r_bsp
		rdp/src/r_bsp/board/generic_${CONFIG_SOC_SERIES}
		rdp/src/r_bsp/mcu
		rdp/src/r_bsp/mcu/all
		rdp/src/r_bsp/mcu/${CONFIG_SOC_SERIES}
		rdp/src/r_bsp/mcu/${CONFIG_SOC_SERIES}/register_access/gnuc
	)

	set(srcs
		rdp/src/r_bsp/mcu/all/r_bsp_common.c
		rdp/src/r_bsp/mcu/all/r_bsp_cpu.c
		rdp/src/r_bsp/mcu/all/r_bsp_interrupts.c
		rdp/src/r_bsp/mcu/all/r_bsp_locking.c
		rdp/src/r_bsp/mcu/all/r_bsp_software_interrupt.c
		rdp/src/r_bsp/mcu/all/r_rx_intrinsic_functions.c
		rdp/src/r_bsp/mcu/all/mcu_locks.c
		rdp/src/r_bsp/mcu/${CONFIG_SOC_SERIES}/mcu_clocks.c
		rdp/src/r_bsp/mcu/${CONFIG_SOC_SERIES}/mcu_init.c
		rdp/src/r_bsp/mcu/${CONFIG_SOC_SERIES}/mcu_interrupts.c
	)

	zephyr_include_directories(${include_dirs})
	zephyr_library_sources(${srcs})

	# Optional build base on feature configuration
	if(CONFIG_USE_RX_RDP_GPIO)
		zephyr_library_sources(
			rdp/src/r_gpio_rx/src/r_gpio_rx.c
			rdp/src/r_gpio_rx/src/targets/${CONFIG_SOC_SERIES}/r_gpio_${CONFIG_SOC_SERIES}.c
		)
		zephyr_include_directories(rdp/src/r_gpio_rx)
	endif()

	if(CONFIG_USE_RX_RDP_MPC)
		zephyr_library_sources(
			rdp/src/r_mpc_rx/src/r_mpc_rx.c
		)
		zephyr_include_directories(rdp/src/r_mpc_rx)
	endif()

	if(CONFIG_USE_RX_RDP_SCI_UART)
		zephyr_include_directories(
			rdp/src/r_byteq
			rdp/src/r_byteq/src
			rdp/src/r_sci_rx
			rdp/src/r_sci_rx/src
			rdp/src/r_sci_rx/src/targets/${CONFIG_SOC_SERIES}
		)
		zephyr_library_sources(
			rdp/src/r_sci_rx/src/r_sci_rx.c
			rdp/src/r_sci_rx/src/targets/${CONFIG_SOC_SERIES}/r_sci_${CONFIG_SOC_SERIES}.c
			rdp/src/r_sci_rx/src/targets/${CONFIG_SOC_SERIES}/r_sci_${CONFIG_SOC_SERIES}_data.c
			rdp/src/r_byteq/src/r_byteq.c
		)
		zephyr_library_compile_options(
		"-Wno-missing-braces"
	)
	endif()

	if(CONFIG_USE_RX_RDP_FLASH)
		zephyr_library_sources(
			rdp/src/r_flash_rx/src/r_flash_fcu.c
			rdp/src/r_flash_rx/src/r_flash_group.c
			rdp/src/r_flash_rx/src/r_flash_nofcu.c
			rdp/src/r_flash_rx/src/r_flash_rx.c
		)
		zephyr_include_directories(
			rdp/src/r_flash_rx
			rdp/src/r_flash_rx/src
			rdp/src/r_flash_rx/src/targets/${CONFIG_SOC_SERIES}
		)

		if(CONFIG_RX_FLASH_TYPE1)
			zephyr_library_sources(
				rdp/src/r_flash_rx/src/flash_type_1/r_flash_type1.c
			)
			zephyr_include_directories(
				rdp/src/r_flash_rx/src/flash_type_1
			)
		endif()
	endif()

	if(CONFIG_USE_RX_RDP_RSPI)
		zephyr_library_sources(
			rdp/src/r_rspi_rx/src/r_rspi_rx.c
		)
		zephyr_include_directories(
			rdp/src/r_rspi_rx/src
			rdp/src/r_rspi_rx)
	endif()

	if(CONFIG_USE_RX_RDP_I2C)
		zephyr_include_directories(
			rdp/src/r_riic_rx
			rdp/src/r_riic_rx/src
			rdp/src/r_riic_rx/src/targets/${CONFIG_SOC_SERIES}
		)
		zephyr_library_sources(
			rdp/src/r_riic_rx/src/r_riic_rx.c
			rdp/src/r_riic_rx/src/targets/${CONFIG_SOC_SERIES}/r_riic_${CONFIG_SOC_SERIES}.c
		)
	endif()

	if(CONFIG_USE_RX_RDP_ADC)
		zephyr_include_directories(
			rdp/src/r_s12ad_rx
			rdp/src/r_s12ad_rx/src
			rdp/src/r_s12ad_rx/src/targets/${CONFIG_SOC_SERIES}
		)
		zephyr_library_sources(
			rdp/src/r_s12ad_rx/src/r_s12ad_rx.c
			rdp/src/r_s12ad_rx/src/targets/${CONFIG_SOC_SERIES}/r_s12ad_${CONFIG_SOC_SERIES}.c
		)
	endif()

	if(CONFIG_USE_RX_RDP_IWDT)
		zephyr_include_directories(
			rdp/src/r_iwdt_rx
			rdp/src/r_iwdt_rx/src
		)
		zephyr_library_sources(
			rdp/src/r_iwdt_rx/src/r_iwdt_rx.c
		)
	endif()

	if(CONFIG_USE_RX_RDP_LVD)
		zephyr_library_sources(
			rdp/src/r_lvd_rx/src/r_lvd_rx_hw.c
			rdp/src/r_lvd_rx/src/r_lvd_rx.c
		)
		zephyr_include_directories(
			rdp/src/r_lvd_rx
			rdp/src/r_lvd_rx/src
			rdp/src/r_lvd_rx/src/targets/${CONFIG_SOC_SERIES})
	endif()
endif()

if(CONFIG_HAS_RENESAS_RX_FSP)
	set(include_dirs
		fsp/inc
		fsp/inc/api
		fsp/inc/instances
		fsp/src/bsp/mcu/all
		fsp/src/bsp/mcu/${CONFIG_SOC_SERIES}
		fsp/src/bsp/cmsis/Device/RENESAS/Include
	)

	set(srcs
		fsp/src/bsp/mcu/all/bsp_clocks.c
		fsp/src/bsp/mcu/all/bsp_delay.c
		fsp/src/bsp/mcu/all/bsp_register_protection.c
		fsp/src/bsp/mcu/all/bsp_irq.c
		fsp/src/bsp/mcu/all/bsp_group_irq.c
	)

	zephyr_include_directories(${include_dirs})
	zephyr_library_sources(${srcs})

	if(CONFIG_SOC_SERIES_RX74M)
		zephyr_include_directories(
			fsp/src/bsp/cmsis/Device/RENESAS/Include/R5K574M
		)
	endif()

	if(CONFIG_USE_RX_FSP_SCI_B_UART)
		zephyr_library_sources(
			fsp/src/r_sci_b_uart/r_sci_b_uart.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_RMAC)
		zephyr_library_sources(
			fsp/src/r_layer3_switch/r_layer3_switch.c
			fsp/src/r_rmac/r_rmac.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_USB_DEVICE)
		zephyr_include_directories(
			fsp/src/r_usb_device
		)
		zephyr_library_sources(
			fsp/src/r_usb_device/r_usb_device.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_CANFDG)
		zephyr_library_sources(
			fsp/src/r_canfdg/r_canfdg.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SPI_B)
		zephyr_library_sources(
			fsp/src/r_spi_b/r_spi_b.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SCI_B_I2C)
		zephyr_library_sources(
			fsp/src/r_sci_b_i2c/r_sci_b_i2c.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SCI_B_SPI)
		zephyr_library_sources(
			fsp/src/r_sci_b_spi/r_sci_b_spi.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_MRAM)
		zephyr_library_sources(
			fsp/src/r_mram/r_mram.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_XSPI_CONTROLLER)
		zephyr_library_sources(
			fsp/src/r_xspi_controller/r_xspi_controller.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SCE)
		zephyr_include_directories(
			fsp/src/r_sce
			fsp/src/r_sce/common
		)
		if(CONFIG_HAS_RENESAS_RX_RSIP_E50D)
			zephyr_include_directories(
				fsp/src/r_sce/crypto_procedures/src/rsip_e50d/plainkey/public/inc
				fsp/src/r_sce/crypto_procedures/src/rsip_e50d/plainkey/private/inc)
			file(GLOB rsip_e50d_srcs
				"fsp/src/r_sce/crypto_procedures/src/rsip_e50d/plainkey/adaptors/*.c"
				"fsp/src/r_sce/crypto_procedures/src/rsip_e50d/plainkey/primitive/*.c")
			zephyr_library_sources(${rsip_e50d_srcs})
		endif()
	endif()

	if(CONFIG_USE_RX_FSP_DTC)
		zephyr_library_sources(
			fsp/src/r_dtc/r_dtc.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SDHI)
		zephyr_include_directories(
			fsp/src/r_sdhi
		)
		zephyr_library_sources(
			fsp/src/r_sdhi/r_sdhi.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_GPT)
		zephyr_library_sources(
			fsp/src/r_gpt/r_gpt.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_ADC_B)
		zephyr_library_sources(
			fsp/src/r_adc_b/r_adc_b.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_TMR)
		zephyr_library_sources(
			fsp/src/r_tmr/r_tmr.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_RTC)
		zephyr_library_sources(
			fsp/src/r_rtc/r_rtc.c
		)
	endif()

	if(CONFIG_USE_RX_FSP_SDRAM)
		zephyr_library_sources(
			fsp/src/bsp/mcu/all/bsp_sdram.c
		)
	endif()
endif()

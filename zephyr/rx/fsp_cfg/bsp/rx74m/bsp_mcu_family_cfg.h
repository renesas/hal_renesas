/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef BSP_MCU_FAMILY_CFG_H_
#define BSP_MCU_FAMILY_CFG_H_
#include "bsp_mcu_device_pn_cfg.h"
#include "bsp_mcu_device_cfg.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/bsp_override.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/bsp_mcu_info.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/r_adc_device_types.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/r_lpm_device_types.h"
#include "bsp_clock_cfg.h"
#define BSP_MCU_GROUP_RX74M (1)

#define BSP_LOCO_HZ                 (32768)
#define BSP_MOCO_HZ                 (8000000)
#define BSP_SUB_CLOCK_HZ            (32768)
#if   BSP_CFG_HOCO_FREQUENCY == 0
#define BSP_HOCO_HZ                 (16000000)
#elif BSP_CFG_HOCO_FREQUENCY == 1
            #define BSP_HOCO_HZ                 (18000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 2
            #define BSP_HOCO_HZ                 (20000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 4
            #define BSP_HOCO_HZ                 (32000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 7
            #define BSP_HOCO_HZ                 (48000000)
        #else
            #error "Invalid HOCO frequency chosen (BSP_CFG_HOCO_FREQUENCY) in bsp_clock_cfg.h"
        #endif

#define BSP_CFG_FLL_ENABLE                   (0)

#define BSP_CFG_CLOCK_SETTLING_DELAY_ENABLE  (1)
#define BSP_CFG_SLEEP_MODE_DELAY_ENABLE      (1)
#define BSP_CFG_RTOS_SLEEP_MODE_DELAY_ENABLE (1)
#define BSP_CFG_CLOCK_SETTLING_DELAY_US      (150)

/* Default settings for Option Function Select Register 0 */
#ifndef BSP_CFG_OPTION_SETTING_OFS0
#define BSP_CFG_OPTION_SETTING_OFS0 (0xFFFFFFFF)
#endif

/* Security Attribution for Bank Select Register */
#ifndef BSP_CFG_ROM_REG_BANKSEL_SEL
#define BSP_CFG_ROM_REG_BANKSEL_SEL (0x00000000U)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL
#define BSP_CFG_ROM_REG_BLSEL           (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL_SEC
#define BSP_CFG_ROM_REG_BLSEL_SEC       (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL_SEL
#define BSP_CFG_ROM_REG_BLSEL_SEL       (0x00000000)
#endif

/* Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_BPS0
#define BSP_CFG_ROM_REG_BPS0 (0xFFFFFFFF)
#endif
/* Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_BPS1
#define BSP_CFG_ROM_REG_BPS1 (0xFFFFFFFF)
#endif
/* Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_BPS2
#define BSP_CFG_ROM_REG_BPS2 (0xFFFFFFFF)
#endif
/* Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_BPS3
#define BSP_CFG_ROM_REG_BPS3 (0xFFFFFFFF)
#endif
/* Block Protection Register 4 */
#ifndef BSP_CFG_ROM_REG_BPS4
#define BSP_CFG_ROM_REG_BPS4 (0xFFFFFFFF)
#endif
/* Block Protection Register 5 */
#ifndef BSP_CFG_ROM_REG_BPS5
#define BSP_CFG_ROM_REG_BPS5 (0xFFFFFFFF)
#endif
/* Block Protection Register 6 */
#ifndef BSP_CFG_ROM_REG_BPS6
#define BSP_CFG_ROM_REG_BPS6 (0xFFFFFFFF)
#endif
/* Block Protection Register 7 */
#ifndef BSP_CFG_ROM_REG_BPS7
#define BSP_CFG_ROM_REG_BPS7 (0xFFFFFFFF)
#endif
/* Block Protection Register 8 */
#ifndef BSP_CFG_ROM_REG_BPS8
#define BSP_CFG_ROM_REG_BPS8 (0xFFFFFFFF)
#endif
/* Block Protection Register 9 */
#ifndef BSP_CFG_ROM_REG_BPS9
#define BSP_CFG_ROM_REG_BPS9 (0xFFFFFFFF)
#endif
/* Block Protection Register 10 */
#ifndef BSP_CFG_ROM_REG_BPS10
#define BSP_CFG_ROM_REG_BPS10 (0xFFFFFFFF)
#endif
/* Block Protection Register 11 */
#ifndef BSP_CFG_ROM_REG_BPS11
#define BSP_CFG_ROM_REG_BPS11 (0xFFFFFFFF)
#endif
/* Block Protection Register 12 */
#ifndef BSP_CFG_ROM_REG_BPS12
#define BSP_CFG_ROM_REG_BPS12 (0xFFFFFFFF)
#endif
/* Block Protection Register 13 */
#ifndef BSP_CFG_ROM_REG_BPS13
#define BSP_CFG_ROM_REG_BPS13 (0xFFFFFFFF)
#endif
/* Block Protection Register 14 */
#ifndef BSP_CFG_ROM_REG_BPS14
#define BSP_CFG_ROM_REG_BPS14 (0xFFFFFFFF)
#endif
/* Block Protection Register 15 */
#ifndef BSP_CFG_ROM_REG_BPS15
#define BSP_CFG_ROM_REG_BPS15 (0xFFFFFFFF)
#endif
/* Block Protection Register 64 */
#ifndef BSP_CFG_ROM_REG_BPS64
#define BSP_CFG_ROM_REG_BPS64 (0xFFFFFFFF)
#endif
/* Block Protection Register 65 */
#ifndef BSP_CFG_ROM_REG_BPS65
#define BSP_CFG_ROM_REG_BPS65 (0xFFFFFFFF)
#endif
/* Block Protection Register 66 */
#ifndef BSP_CFG_ROM_REG_BPS66
#define BSP_CFG_ROM_REG_BPS66 (0xFFFFFFFF)
#endif
/* Block Protection Register 67 */
#ifndef BSP_CFG_ROM_REG_BPS67
#define BSP_CFG_ROM_REG_BPS67 (0xFFFFFFFF)
#endif
/* Block Protection Register 68 */
#ifndef BSP_CFG_ROM_REG_BPS68
#define BSP_CFG_ROM_REG_BPS68 (0xFFFFFFFF)
#endif
/* Block Protection Register 69 */
#ifndef BSP_CFG_ROM_REG_BPS69
#define BSP_CFG_ROM_REG_BPS69 (0xFFFFFFFF)
#endif
/* Block Protection Register 70 */
#ifndef BSP_CFG_ROM_REG_BPS70
#define BSP_CFG_ROM_REG_BPS70 (0xFFFFFFFF)
#endif
/* Block Protection Register 71 */
#ifndef BSP_CFG_ROM_REG_BPS71
#define BSP_CFG_ROM_REG_BPS71 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC0
#define BSP_CFG_ROM_REG_BPS_SEC0 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC1
#define BSP_CFG_ROM_REG_BPS_SEC1 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC2
#define BSP_CFG_ROM_REG_BPS_SEC2 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC3
#define BSP_CFG_ROM_REG_BPS_SEC3 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 4 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC4
#define BSP_CFG_ROM_REG_BPS_SEC4 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 5 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC5
#define BSP_CFG_ROM_REG_BPS_SEC5 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 6 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC6
#define BSP_CFG_ROM_REG_BPS_SEC6 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 7 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC7
#define BSP_CFG_ROM_REG_BPS_SEC7 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 8 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC8
#define BSP_CFG_ROM_REG_BPS_SEC8 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 9 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC9
#define BSP_CFG_ROM_REG_BPS_SEC9 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 10 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC10
#define BSP_CFG_ROM_REG_BPS_SEC10 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 11 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC11
#define BSP_CFG_ROM_REG_BPS_SEC11 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 12 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC12
#define BSP_CFG_ROM_REG_BPS_SEC12 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 13 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC13
#define BSP_CFG_ROM_REG_BPS_SEC13 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 14 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC14
#define BSP_CFG_ROM_REG_BPS_SEC14 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 15 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC15
#define BSP_CFG_ROM_REG_BPS_SEC15 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 64 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC64
#define BSP_CFG_ROM_REG_BPS_SEC64 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 65 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC65
#define BSP_CFG_ROM_REG_BPS_SEC65 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 66 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC66
#define BSP_CFG_ROM_REG_BPS_SEC66 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 67 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC67
#define BSP_CFG_ROM_REG_BPS_SEC67 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 68 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC68
#define BSP_CFG_ROM_REG_BPS_SEC68 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 69 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC69
#define BSP_CFG_ROM_REG_BPS_SEC69 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 70 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC70
#define BSP_CFG_ROM_REG_BPS_SEC70 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 71 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC71
#define BSP_CFG_ROM_REG_BPS_SEC71 (0xFFFFFFFF)
#endif
/* Block Protection for Data MRAM Register  */
#ifndef BSP_CFG_ROM_REG_BPSDM
#define BSP_CFG_ROM_REG_BPSDM (0xFFFFFFFF)
#endif
/* Block Protection for Data MRAM Register  */
#ifndef BSP_CFG_ROM_REG_BPSDM_SEC
#define BSP_CFG_ROM_REG_BPSDM_SEC (0xFFFFFFFF)
#endif
/* Dual Mode Select Register */
#ifndef BSP_CFG_ROM_REG_DUALSEL
#define BSP_CFG_ROM_REG_DUALSEL (0xFFFFFFF8U | (0x7U))
#endif

#ifndef BSP_CFG_ROM_REG_SAS
#define BSP_CFG_ROM_REG_SAS             (0xFFFFFFFF)
#endif

#ifndef BSP_CLOCK_CFG_MAIN_OSC_WAIT
#define BSP_CLOCK_CFG_MAIN_OSC_WAIT (9)
#endif

/* Used to create IELS values for the interrupt initialization table g_interrupt_event_link_select. */
#define BSP_PRV_IELS_ENUM(vector)    CONCAT(ICU_, vector)

#define BSP_MAX_CLOCK_CHANGE_THRESHOLD (300000000U)

/* SDRAM controller configuration */
#if DT_NODE_HAS_STATUS_OKAY(DT_INST(0, renesas_rx_sdram))
#define BSP_CFG_SDRAM_ENABLED (1)
#define BSP_CFG_SDRAM_TRAS                                                     \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 0)
#define BSP_CFG_SDRAM_TRCD                                                     \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 1)
#define BSP_CFG_SDRAM_TRP                                                      \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 2)
#define BSP_CFG_SDRAM_TWR                                                      \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 3)
#define BSP_CFG_SDRAM_TCL                                                      \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 4)
#define BSP_CFG_SDRAM_TRFC                                                     \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 5)
#define BSP_CFG_SDRAM_TREFW                                                    \
  DT_PROP_BY_IDX(DT_CHILD(DT_INST(0, renesas_rx_sdram), bank_0),               \
                 renesas_rx_sdram_timing, 6)
#define BSP_CFG_SDRAM_INIT_ARFI                                                \
  DT_PROP(DT_INST(0, renesas_rx_sdram), auto_refresh_interval)
#define BSP_CFG_SDRAM_INIT_ARFC                                                \
  DT_PROP(DT_INST(0, renesas_rx_sdram), auto_refresh_count)
#define BSP_CFG_SDRAM_INIT_PRC                                                 \
  DT_PROP(DT_INST(0, renesas_rx_sdram), precharge_cycle_count)
#define BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT                                     \
  DT_ENUM_IDX(DT_INST(0, renesas_rx_sdram), multiplex_addr_shift)
#define BSP_CFG_SDRAM_ENDIAN_MODE                                              \
  DT_ENUM_IDX(DT_INST(0, renesas_rx_sdram), endian_mode)
#define BSP_CFG_SDRAM_ACCESS_MODE                                              \
  DT_PROP(DT_INST(0, renesas_rx_sdram), continuous_access)
#define BSP_CFG_SDRAM_BUS_WIDTH                                                \
  DT_ENUM_IDX(DT_INST(0, renesas_rx_sdram), bus_width)
#else
#define BSP_CFG_SDRAM_ENABLED (0)
#define BSP_CFG_SDRAM_TRAS (0)
#define BSP_CFG_SDRAM_TRCD (0)
#define BSP_CFG_SDRAM_TRP (0)
#define BSP_CFG_SDRAM_TWR (0)
#define BSP_CFG_SDRAM_TCL (0)
#define BSP_CFG_SDRAM_TRFC (0)
#define BSP_CFG_SDRAM_TREFW (0)
#define BSP_CFG_SDRAM_INIT_ARFI (0)
#define BSP_CFG_SDRAM_INIT_ARFC (0)
#define BSP_CFG_SDRAM_INIT_PRC (0)
#define BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT (0)
#define BSP_CFG_SDRAM_ENDIAN_MODE (0)
#define BSP_CFG_SDRAM_ACCESS_MODE (0)
#define BSP_CFG_SDRAM_BUS_WIDTH (0)
#endif /* DT_NODE_HAS_STATUS_OKAY(DT_INST(0, renesas_rx_sdram)) */

#endif /* BSP_MCU_FAMILY_CFG_H_ */

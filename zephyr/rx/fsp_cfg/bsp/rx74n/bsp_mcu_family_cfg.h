/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef BSP_MCU_FAMILY_CFG_H_
#define BSP_MCU_FAMILY_CFG_H_
#include "bsp_mcu_device_pn_cfg.h"
#include "bsp_mcu_device_cfg.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74n/bsp_override.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74n/bsp_mcu_info.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74n/r_adc_device_types.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74n/r_lpm_device_types.h"
#include "bsp_clock_cfg.h"
#define BSP_MCU_GROUP_RX74N (1)

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

#define BSP_CFG_INLINE_IRQ_FUNCTIONS         (1)

#define BSP_CFG_PREFETCH_BUFFER_ENABLE       (0)

#ifndef BSP_CLOCK_CFG_MAIN_OSC_WAIT
#define BSP_CLOCK_CFG_MAIN_OSC_WAIT (9)
#endif

#ifndef BSP_CFG_DCACHE_ENABLED
#define BSP_CFG_DCACHE_ENABLED (0)
#endif

/* Used to create IELS values for the interrupt initialization table g_interrupt_event_link_select. */
#define BSP_PRV_IELS_ENUM(vector)    (ICU_ ## vector)

#define BSP_MAX_CLOCK_CHANGE_THRESHOLD (300000000U)

#ifndef BSP_CFG_SDRAM_ENABLED
#define BSP_CFG_SDRAM_ENABLED  (0)
#endif

#ifndef BSP_CFG_SDRAM_TRAS
#define BSP_CFG_SDRAM_TRAS  (6)
#endif

#ifndef BSP_CFG_SDRAM_TRCD
#define BSP_CFG_SDRAM_TRCD  (2)
#endif

#ifndef BSP_CFG_SDRAM_TRP
#define BSP_CFG_SDRAM_TRP  (2)
#endif

#ifndef BSP_CFG_SDRAM_TWR
#define BSP_CFG_SDRAM_TWR  (2)
#endif

#ifndef BSP_CFG_SDRAM_TCL
#define BSP_CFG_SDRAM_TCL  (3)
#endif

#ifndef BSP_CFG_SDRAM_TRFC
#define BSP_CFG_SDRAM_TRFC  (1171)
#endif

#ifndef BSP_CFG_SDRAM_TREFW
#define BSP_CFG_SDRAM_TREFW  (5)
#endif

#ifndef BSP_CFG_SDRAM_INIT_ARFI
#define BSP_CFG_SDRAM_INIT_ARFI  (5)
#endif

#ifndef BSP_CFG_SDRAM_INIT_ARFC
#define BSP_CFG_SDRAM_INIT_ARFC  (2)
#endif

#ifndef BSP_CFG_SDRAM_INIT_PRC
#define BSP_CFG_SDRAM_INIT_PRC  (3)
#endif

#ifndef BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT
#define BSP_CFG_SDRAM_MULTIPLEX_ADDR_SHIFT  (0)
#endif

#ifndef BSP_CFG_SDRAM_ENDIAN_MODE
#define BSP_CFG_SDRAM_ENDIAN_MODE  (0)
#endif

#ifndef BSP_CFG_SDRAM_ACCESS_MODE
#define BSP_CFG_SDRAM_ACCESS_MODE  (1)
#endif

#ifndef BSP_CFG_SDRAM_BUS_WIDTH
#define BSP_CFG_SDRAM_BUS_WIDTH  (0)
#endif
#endif /* BSP_MCU_FAMILY_CFG_H_ */

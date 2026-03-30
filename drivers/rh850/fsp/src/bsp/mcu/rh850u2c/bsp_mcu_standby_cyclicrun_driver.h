/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_MCU_STANDBY_CYCLICRUN_DRIVER_H
#define BSP_MCU_STANDBY_CYCLICRUN_DRIVER_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED)

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Key code protection value */
 #define BSP_PRV_KEYCODE_DISABLE    0xA5A5A500 ///< Key code protection disable value
 #define BSP_PRV_KEYCODE_ENABLE     0xA5A5A501 ///< Key code protection enable value

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_STANDBY_CRUN_U2CX BSP MCU Standby Mode - Cyclic RUN mode
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Enum used in R_FSP_CyclicRunSystemClockHzGet. */
typedef enum e_fsp_priv_clock_cyclic
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC,    ///< CLK_LSIOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,    ///< CLK_HSIOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC,      ///< CLK_IOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC,  ///< CLK_SYS_IOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG,       ///< CLK_EMG Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,      ///< CLK_MOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC,      ///< CLK_SOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN, ///< CLK_SYS_CLEAN Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG,  ///< CLK_SYS_SSCG Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU,       ///< CLK_CPU Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS,      ///< CLK_SBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS,      ///< CLK_HBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB,      ///< CLK_UHSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,       ///< CLK_HSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB,       ///< CLK_LSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_SBUS,     ///< CLKC_SBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS,     ///< CLKC_HBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_UHSB,     ///< CLKC_UHSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB,      ///< CLKC_HSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB,      ///< CLKC_LSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS,      ///< CLKA_LPS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDTBA,    ///< CLKA_WDTBA Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ,     ///< CLKA_TAUJ Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC,      ///< CLKA_ADC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O,      ///< EXTCLK0O Clock
    FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O,      ///< EXTCLK1O Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI,      ///< CLK_MSPI Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN3,     ///< CLK_RLIN3 Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN323,   ///< CLK_RLIN323 Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDTB,      ///< CLK_WDTB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT,    ///< CLK_ECMCNT Clock
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
} fsp_priv_clock_cyclicrun_t;

/** Standby configuration for Cyclic-Run modules */
typedef enum e_bsp_cyclicrun_module_standby
{
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_MSPI,       ///< Standby module MSR_MSPI
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_RLIN3,      ///< Standby module MSR_RLIN3
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCK_AWO,   ///< Standby module MSR_ADCK_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_RTCA,       ///< Standby module MSR_RTCA
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_TAUJ_AWO,   ///< Standby module MSR_TAUJ_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_WDTB_AWO,   ///< Standby module MSR_WDTB_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_TOTAL_INDEX_END ///< Total number of standby module entries
} bsp_cyclicrun_module_standby_t;

/** @} (end addtogroup BSP_STANDBY_CRUN) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM];
extern const bsp_module_standby_info_t g_cyclicrun_msr_info_table[BSP_CYCLICRUN_MODULE_STANDBY_TOTAL_INDEX_END];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void bsp_cyclicrun_clock_driver_init(void);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_MCU_STANDBY_CYCLICRUN_DRIVER_H */
#endif /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */

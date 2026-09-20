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
#include "memory_regions.h"

#if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED)

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_STANDBY_CRUN_U2Ax BSP MCU Standby Mode - Cyclic RUN mode
 *
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Enum used in R_FSP_CyclicRunSystemClockHzGet. */
typedef enum e_fsp_priv_cyclicrun_clock   /* PRQA S 1536 # Advisory R-2.4 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,    ///< CLK_MOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,  ///< CLK_HSIOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC,  ///< CLK_LSIOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HVIOSC,  ///< CLK_HVIOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL,     ///< CLK_PLL Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLLO,    ///< CLK_PLLO Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC,    ///< CLK_IOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS,     ///< CLK_SYS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU,     ///< CLK_CPU Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS,    ///< CLK_SBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS,    ///< CLK_HBUS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB,    ///< CLK_UHSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,     ///< CLK_HSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB,     ///< CLK_LSB Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN,    ///< CLK_RLIN Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_RCANOSC, ///< CLK_RCANOSC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_ADC,     ///< CLK_ADC Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI,    ///< CLK_MSPI Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT,  ///< CLK_ECMCNT Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDT,     ///< CLK_WDT Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDT,    ///< CLKA_WDT Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ,   ///< CLKA_TAUJ Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA,   ///< CLKA_RTCA Clock
 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC,    ///< CLKA_ADC Clock
 #endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O,    ///< EXTCLK0O Clock
    FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O,    ///< EXTCLK1O Clock
    FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS,    ///< CLKA_LPS Clock
    FSP_PRIV_CLOCK_CYCLICRUN_NUM,
} fsp_priv_clock_cyclicrun_t;

/** Cyclic Standby Module */
typedef enum e_bsp_cyclicrun_module_standby      /* PRQA S 1536 # Advisory R-2.4 */
{
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_MSPI,       ///< MSR_MSPI
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_RLIN3,      ///< MSR_RLIN3
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCJ_AWO,   ///< MSR_ADCJ_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_RTCA,       ///< MSR_RTCA
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_TAUJ_AWO,   ///< MSR_TAUJ_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_MSR_WDTB_AWO,   ///< MSR_WDTB_AWO
    BSP_CYCLICRUN_MODULE_STANDBY_TOTAL_INDEX_END ///< TOTAL_INDEX_END
} bsp_cyclicrun_module_standby_t;                /* PRQA S 1535 # Advisory R-2.3 */

/** @} (end addtogroup BSP_STANDBY_CRUN) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA,
                            BSP_PRAGMA_SECTION_CRUN_CODE_DATA BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)

extern volatile uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM];
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_DEFAULT)

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA,
                            BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)
extern const bsp_module_standby_info_t g_cyclicrun_msr_info_table[
    BSP_CYCLICRUN_MODULE_STANDBY_TOTAL_INDEX_END];
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void     bsp_cyclicrun_clock_driver_init(void);
uint32_t r_bsp_cyclicrun_clock_hz_calculate(fsp_priv_clock_cyclicrun_t clock); /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */ /* PRQA S 2053 # Advisory D-4.4 */
#endif                                 /* BSP_MCU_STANDBY_CYCLICRUN_DRIVER_H */

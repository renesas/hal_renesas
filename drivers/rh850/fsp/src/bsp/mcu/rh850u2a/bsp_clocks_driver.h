/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_CLOCKS_DRIVER_H
#define BSP_CLOCKS_DRIVER_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Key code protection value */
#define BSP_PRV_KEYCODE_DISABLE          0xA5A5A500U  ///< Key code protection disable value
#define BSP_PRV_KEYCODE_ENABLE           0xA5A5A501U  ///< Key code protection enable value

/** Clock system divider */
#define BSP_CLOCK_DIVIDED_BY_2           (0x02U)      ///< Clock divided by 2
#define BSP_CLOCK_NO_DIVISION            (0x01U)      ///< No clock division

/** MOSC */
#define BSP_CLKCTL_CLK_MOSC_40MHZ        (40000000U)  ///< Main oscillator clock: 40 MHz
#define BSP_CLKCTL_CLK_MOSC_24MHZ        (24000000U)  ///< Main oscillator clock: 24 MHz
#define BSP_CLKCTL_CLK_MOSC_20MHZ        (20000000U)  ///< Main oscillator clock: 20 MHz
#define BSP_CLKCTL_CLK_MOSC_16MHZ        (16000000U)  ///< Main oscillator clock: 16 MHz

/** PLLCLK */
#define BSP_CKD_PLLS_PLLCLKDSYNC_MASK    (0x01U)      ///< PLL clock divide sync mask

/** PLLO */
#define BSP_CLKCTL_CLK_PLLO_480MHZ       (480000000U) ///< PLLO 480 MHz
#define BSP_CLKCTL_CLK_PLLO_640MHZ       (640000000U) ///< PLLO 640 MHz
#define BSP_CLKCTL_CLK_PLLO_800MHZ       (800000000U) ///< PLLO 800 MHz

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_CLOCK_U2AX BSP Clock
 *
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Enum used in R_FSP_SystemClockHzGet. */
/* PRQA S 1508, 1750 ++ # DP_MSR_R-5.6_UC1, DP_MSR_R-5.7_UC1 */
typedef enum e_fsp_priv_clock          /* PRQA S 1536 # Advisory R-2.4 */
{
    FSP_PRIV_CLOCK_CLK_MOSC,           ///< CLK_MOSC Clock
    FSP_PRIV_CLOCK_CLK_HSIOSC,         ///< CLK_HSIOSC Clock
    FSP_PRIV_CLOCK_CLK_LSIOSC,         ///< CLK_LSIOSC Clock
    FSP_PRIV_CLOCK_CLK_HVIOSC,         ///< CLK_HVIOSC Clock
    FSP_PRIV_CLOCK_CLK_PLL,            ///< CLK_PLL Clock
    FSP_PRIV_CLOCK_CLK_PLLO,           ///< CLK_PLLO Clock
    FSP_PRIV_CLOCK_CLK_IOSC,           ///< CLK_IOSC Clock
    FSP_PRIV_CLOCK_CLK_SYS,            ///< CLK_SYS Clock
    FSP_PRIV_CLOCK_CLK_CPU,            ///< CLK_CPU Clock
    FSP_PRIV_CLOCK_CLK_SBUS,           ///< CLK_SBUS Clock
    FSP_PRIV_CLOCK_CLK_HBUS,           ///< CLK_HBUS Clock
    FSP_PRIV_CLOCK_CLK_UHSB,           ///< CLK_UHSB Clock
    FSP_PRIV_CLOCK_CLK_HSB,            ///< CLK_HSB Clock
    FSP_PRIV_CLOCK_CLK_LSB,            ///< CLK_LSB Clock
    FSP_PRIV_CLOCK_CLK_RLIN,           ///< CLK_RLIN Clock
    FSP_PRIV_CLOCK_CLK_RCANOSC,        ///< CLK_RCANOSC Clock
    FSP_PRIV_CLOCK_CLK_ADC,            ///< CLK_ADC Clock
    FSP_PRIV_CLOCK_CLK_MSPI,           ///< CLK_MSPI Clock
    FSP_PRIV_CLOCK_CLK_ECMCNT,         ///< CLK_ECMCNT Clock
    FSP_PRIV_CLOCK_CLK_WDT,            ///< CLK_WDT Clock
    FSP_PRIV_CLOCK_CLKA_WDT,           ///< CLKA_WDT Clock
    FSP_PRIV_CLOCK_CLKA_TAUJ,          ///< CLKA_TAUJ Clock
    FSP_PRIV_CLOCK_CLKA_RTCA,          ///< CLKA_RTCA Clock
#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    FSP_PRIV_CLOCK_CLKA_ADC,           ///< CLKA_ADC Clock
#endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    FSP_PRIV_CLOCK_EXTCLK0O,           ///< EXTCLK0O Clock
    FSP_PRIV_CLOCK_EXTCLK1O,           ///< EXTCLK1O Clock
    FSP_PRIV_CLOCK_CLKA_LPS,           ///< CLKA_LPS Clock
    FSP_PRIV_CLOCK_NUM,
} fsp_priv_clock_t;

/* PRQA S 1508, 1750 -- */

/** @} (end addtogroup BSP_MCU_RH850U2AX) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/* PRQA S 1513 ++ # DP_MSR_R-8.5_UC1 */
extern uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM];

/* PRQA S 1513 -- */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
/* PRQA S 1513 ++ # DP_MSR_R-8.5_UC1 */
void     bsp_clock_driver_init(void);
void     bsp_clock_gear_down(void);
uint32_t r_bsp_clock_hz_calculate(fsp_priv_clock_t clock); /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */

/* PRQA S 1513 -- */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_CLOCKS_DRIVER_H */

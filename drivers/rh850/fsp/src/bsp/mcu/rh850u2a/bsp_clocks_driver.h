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
#define BSP_PRV_KEYCODE_DISABLE          0xA5A5A500 ///< Key code protection disable value
#define BSP_PRV_KEYCODE_ENABLE           0xA5A5A501 ///< Key code protection enable value

/** Clock system divider */
#define BSP_CLOCK_DIVIDED_BY_2           (0x02U)    ///< Clock divided by 2
#define BSP_CLOCK_NO_DIVISION            (0x01U)    ///< No clock division

/** MOSC */
#define BSP_CLKCTL_CLK_MOSC_40MHZ        (40000000) ///< Main oscillator clock: 40 MHz
#define BSP_CLKCTL_CLK_MOSC_24MHZ        (24000000) ///< Main oscillator clock: 24 MHz
#define BSP_CLKCTL_CLK_MOSC_20MHZ        (20000000) ///< Main oscillator clock: 20 MHz
#define BSP_CLKCTL_CLK_MOSC_16MHZ        (16000000) ///< Main oscillator clock: 16 MHz

/** PLLCLK */
#define BSP_CKD_PLLS_PLLCLKDSYNC_MASK    (0x01U)    ///< PLL clock divide sync mask

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_CLOCK_U2AX BSP Clock
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Enum used in R_FSP_SystemClockHzGet. */
typedef enum e_fsp_priv_clock
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
    FSP_PRIV_CLOCK_CLKA_ADC,           ///< CLKA_ADC Clock
    FSP_PRIV_CLOCK_EXTCLK0O,           ///< EXTCLK0O Clock
    FSP_PRIV_CLOCK_EXTCLK1O,           ///< EXTCLK1O Clock
    FSP_PRIV_CLOCK_CLKA_LPS,           ///< CLKA_LPS Clock
    FSP_PRIV_CLOCK_NUM,
} fsp_priv_clock_t;

/** @} (end addtogroup BSP_MCU_RH850U2AX) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
void bsp_clock_driver_init(void);
void bsp_clock_gear_down(void);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_CLOCKS_DRIVER_H */

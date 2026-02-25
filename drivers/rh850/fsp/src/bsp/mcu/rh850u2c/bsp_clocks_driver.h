/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
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
#define BSP_PRV_KEYCODE_DISABLE           0xA5A5A500  ///< Key code protection disable value
#define BSP_PRV_KEYCODE_ENABLE            0xA5A5A501  ///< Key code protection enable value

/** Clock system (Hz) */
#define BSP_CLOCK_SYSTEM_800MHz           (800000000) ///< System clock frequency: 800 MHz
#define BSP_CLOCK_SYSTEM_640MHz           (640000000) ///< System clock frequency: 640 MHz
#define BSP_CLOCK_SYSTEM_480MHz           (480000000) ///< System clock frequency: 480 MHz
#define BSP_CLOCK_SYSTEM_320MHz           (320000000) ///< System clock frequency: 320 MHz
#define BSP_CLOCK_SYSTEM_240MHz           (240000000) ///< System clock frequency: 240 MHz

/** Clock system divider (encoded as DIV/16) */
#define BSP_CLOCK_DIVIDED_BY_2_DIV_16     (0x02U)     ///< System clock divided by 2 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_3_DIV_16     (0x03U)     ///< System clock divided by 3 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_4_DIV_16     (0x04U)     ///< System clock divided by 4 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_5_DIV_16     (0x05U)     ///< System clock divided by 5 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_6_DIV_16     (0x06U)     ///< System clock divided by 6 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_7_DIV_16     (0x07U)     ///< System clock divided by 7 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_8_DIV_16     (0x08U)     ///< System clock divided by 8 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_9_DIV_16     (0x09U)     ///< System clock divided by 9 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_10_DIV_16    (0x0AU)     ///< System clock divided by 10 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_11_DIV_16    (0x0BU)     ///< System clock divided by 11 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_12_DIV_16    (0x0CU)     ///< System clock divided by 12 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_13_DIV_16    (0x0DU)     ///< System clock divided by 13 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_14_DIV_16    (0x0EU)     ///< System clock divided by 14 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_15_DIV_16    (0x0FU)     ///< System clock divided by 15 (DIV/16 encoding)
#define BSP_CLOCK_DIVIDED_BY_16_DIV_16    (0x00U)     ///< System clock divided by 16 (DIV/16 encoding)
#define BSP_CLOCK_NO_DIVISION             (0x00U)     ///< No clock division (alias of /16 setting)

/** Main oscillator (MOSC) nominal frequencies (Hz) */
#define BSP_CLKCTL_CLK_MOSC_40MHZ         (40000000)  ///< MOSC frequency: 40 MHz
#define BSP_CLKCTL_CLK_MOSC_25MHZ         (25000000)  ///< MOSC frequency: 25 MHz
#define BSP_CLKCTL_CLK_MOSC_24MHZ         (24000000)  ///< MOSC frequency: 24 MHz
#define BSP_CLKCTL_CLK_MOSC_20MHZ         (20000000)  ///< MOSC frequency: 20 MHz
#define BSP_CLKCTL_CLK_MOSC_16MHZ         (16000000)  ///< MOSC frequency: 16 MHz
#define BSP_CLKCTL_CLK_MOSC_DISABLE       (0)         ///< MOSC disabled

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_CLOCK_U2CX BSP Clock
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Enum used in R_FSP_SystemClockHzGet. */
typedef enum e_fsp_priv_clock
{
    FSP_PRIV_CLOCK_CLK_LSIOSC,         ///< CLK_LSIOSC Clock
    FSP_PRIV_CLOCK_CLK_HSIOSC,         ///< CLK_HSIOSC Clock
    FSP_PRIV_CLOCK_CLK_IOSC,           ///< CLK_IOSC Clock
    FSP_PRIV_CLOCK_CLK_SYS_IOSC,       ///< CLK_SYS_IOSC Clock
    FSP_PRIV_CLOCK_CLK_EMG,            ///< CLK_EMG Clock
    FSP_PRIV_CLOCK_CLK_MOSC,           ///< CLK_MOSC Clock
    FSP_PRIV_CLOCK_CLK_SOSC,           ///< CLK_SOSC Clock
    FSP_PRIV_CLOCK_CLK_PLL,            ///< CLK_PLL Clock
    FSP_PRIV_CLOCK_CLK_SSCG,           ///< CLK_SSCG Clock
    FSP_PRIV_CLOCK_CLK_SYS_CLEAN,      ///< CLK_SYS_CLEAN Clock
    FSP_PRIV_CLOCK_CLK_SYS_SSCG,       ///< CLK_SYS_SSCG Clock
    FSP_PRIV_CLOCK_CLK_CPU,            ///< CLK_CPU Clock
    FSP_PRIV_CLOCK_CLK_SBUS,           ///< CLK_SBUS Clock
    FSP_PRIV_CLOCK_CLK_HBUS,           ///< CLK_HBUS Clock
    FSP_PRIV_CLOCK_CLK_UHSB,           ///< CLK_UHSB Clock
    FSP_PRIV_CLOCK_CLK_HSB,            ///< CLK_HSB Clock
    FSP_PRIV_CLOCK_CLK_LSB,            ///< CLK_LSB Clock
    FSP_PRIV_CLOCK_CLKC_SBUS,          ///< CLKC_SBUS Clock
    FSP_PRIV_CLOCK_CLKC_HBUS,          ///< CLKC_HBUS Clock
    FSP_PRIV_CLOCK_CLKC_UHSB,          ///< CLKC_UHSB Clock
    FSP_PRIV_CLOCK_CLKC_HSB,           ///< CLKC_HSB Clock
    FSP_PRIV_CLOCK_CLKC_LSB,           ///< CLKC_LSB Clock
    FSP_PRIV_CLOCK_CLK_FLI,            ///< CLK_FLI Clock
    FSP_PRIV_CLOCK_CLK_FACI,           ///< CLK_FACI Clock
    FSP_PRIV_CLOCK_CLKA_LPS,           ///< CLKA_LPS Clock
    FSP_PRIV_CLOCK_CLKA_WDTBA,         ///< CLKA_WDTBA Clock
    FSP_PRIV_CLOCK_CLKA_TAUJ,          ///< CLKA_TAUJ Clock
    FSP_PRIV_CLOCK_CLKA_ADC,           ///< CLKA_ADC Clock
    FSP_PRIV_CLOCK_EXTCLK0O,           ///< EXTCLK0O Clock
    FSP_PRIV_CLOCK_EXTCLK1O,           ///< EXTCLK1O Clock
    FSP_PRIV_CLOCK_CLK_MSPI,           ///< CLK_MSPI Clock
    FSP_PRIV_CLOCK_CLK_RLIN3,          ///< CLK_RLIN3 Clock
    FSP_PRIV_CLOCK_CLK_RLIN323,        ///< CLK_RLIN323 Clock
    FSP_PRIV_CLOCK_CLK_CANFD_C,        ///< CLK_CANFD_C Clock
    FSP_PRIV_CLOCK_CLK_CANFD_XIN,      ///< CLK_CANFD_XIN Clock
    FSP_PRIV_CLOCK_CLKC_CANXL_CH0,     ///< CLKC_CANXL_CH0 Clock
    FSP_PRIV_CLOCK_CLKC_CANXL_CH1,     ///< CLKC_CANXL_CH1 Clock
    FSP_PRIV_CLOCK_CLK_WDTB,           ///< CLK_WDTB Clock
    FSP_PRIV_CLOCK_CLK_GTM,            ///< CLK_GTM Clock
    FSP_PRIV_CLOCK_CLK_ECMCNT,         ///< CLK_ECMCNT Clock
    FSP_PRIV_CLOCK_CLK_SWDT,           ///< CLK_SWDT Clock
    FSP_PRIV_CLOCK_NUM
} fsp_priv_clock_t;

/** @} (end addtogroup BSP_MCU_RH850U2CX) */

/** SSCG register */
typedef struct st_systemclock_sscg_ctl_t
{
    /** Selector Control Register */
    uint32_t CKS_SSCGC;

    /** Selector Status Register */
    uint32_t CKS_SSCGS;
    uint32_t Reserved0[6];

    /** Divider Control Register */
    uint32_t CKD_SSCGC;

    /** Divider Status Register */
    uint32_t CKD_SSCGS;
} st_systemclock_sscg_ctl;

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

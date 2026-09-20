/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "bsp_clocks_driver.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#define BSP_OPBT_MOSC_FREQ_POS                 (24U)                          ///< Position of MOSC Frequency selection bit in OPBT 10
#define BSP_OPBT_MOSC_FREQ_MSK                 (7U << BSP_OPBT_MOSC_FREQ_POS) ///< Mask of MOSC Frequency selection bit in OPBT 10

#define BSP_OPBT_CKDIVMD_POS                   (29U)                          ///< Position of CPU Frequency & CPU System Clock Setting bit in OPBT 11
#define BSP_OPBT_CKDIVMD_MSK                   (7U << BSP_OPBT_CKDIVMD_POS)   ///< Mask of CPU Frequency & CPU System Clock Setting bit in OPBT 11

#define BSP_OPBT_CKSEL_GTM_POS                 (23U)                          ///< Position of Clock source select for GTM bit in OPBT 8
#define BSP_OPBT_CKSEL_GTM_MSK                 (1U << BSP_OPBT_CKSEL_GTM_POS) ///< Mask of Clock source select for GTM bit in OPBT 8

#define BSP_CLOCK_RLIN3_MOSC_DIV_4_VALUE       (3U)                           ///< Clock source of RLIN3 is MOSC divided by 4
#define BSP_CLOCK_RLIN3_MOSC_DIV_8_VALUE       (4U)                           ///< Clock source of RLIN3 is MOSC divided by 8

#define BSP_CLOCK_CANFD_DIV_2_VALUE            (2U)                           ///< CANFD clock source divided by 2
#define BSP_CLOCK_CANFD_DIV_4_VALUE            (3U)                           ///< CANFD clock source divided by 4

#define BSP_CLOCK_ATAUJ_HSIOSC_DIV_20_VALUE    (2U)                           ///< Clock source of TAUJ AWO is HSIOSC divided by 20

#define BSP_CLOCK_AADC_HSIOSC_DIV_20_VALUE     (1U)                           ///< Clock source of ADC AWO is HSIOSC divided by 20

#define BSP_CLOCK_ARTCA_MOSC_DIV_16_VALUE      (2U)                           ///< Clock source of RTCA AWO is MOSC divided by 16

#define BSP_CLOCK_LSIOSC_VALUE                 (32768U)                       ///< LSIOSC clock value
#define BSP_CLOCK_HSIOSC_VALUE                 (200000000U)                   ///< HSIOSC clock value
#define BSP_CLOCK_IOSC_VALUE                   (200000000U)                   ///< IOSC clock value
#define BSP_CLOCK_PLL_VALUE                    (800000000U)                   ///< PLL clock value
#define BSP_CLOCK_SOSC_VALUE                   (32768U)                       ///< SOSC clock value

#define BSP_CLOCK_WDTB_NO_DIV_VALUE            (1U)                           ///< WDTB clock no division
#define BSP_CLOCK_WDTB_DIV_20_VALUE            (20U)                          ///< WDTB clock divided by 20
#define BSP_CLOCK_WDTB_DIV_640_VALUE           (640U)                         ///< WDTB clock divided by 640
#define BSP_CLOCK_WDTB_DIV_800_VALUE           (800U)                         ///< WDTB clock divided by 800

#define BSP_CLOCK_SWDT_NO_DIV_VALUE            (1U)                           ///< SWDT clock no division
#define BSP_CLOCK_SWDT_DIV_20_VALUE            (20U)                          ///< SWDT clock divided by 20
#define BSP_CLOCK_SWDT_DIV_800_VALUE           (800U)                         ///< SWDT clock divided by 800
#define BSP_CLOCK_SWDT_DIV_3060_VALUE          (3060U)                        ///< SWDT clock divided by 3060

#define BSP_CLOCK_WDTA_NO_DIV_VALUE            (1U)                           ///< WDTA clock no division
#define BSP_CLOCK_WDTA_DIV_128_VALUE           (128U)                         ///< WDTA clock divided by 128

#define BSP_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE    (1U)                           ///< SYS_IOSC clock no division
#define BSP_CLOCK_CLK_SYS_IOSC_DIV_2_VALUE     (2U)                           ///< SYS_IOSC clock divided by 2
#define BSP_CLOCK_CLK_SYS_IOSC_DIV_10_VALUE    (10U)                          ///< SYS_IOSC clock divided by 10

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/
static void bsp_clock_freq_var_init(void);
static void bsp_gear_up_down_sscg(uint8_t                            gear_up_down_select,
                                  volatile st_systemclock_sscg_ctl * sscg_register,
                                  uint32_t                           sscg_clock_hz);

static void bsp_gear_up_down_pllo(uint8_t gear_up_down_select, uint32_t pll_clock_hz);

static uint32_t bsp_source_clock_hz_get(fsp_priv_clock_t clock);     /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_system_clock_hz_get(fsp_priv_clock_t clock);     /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_bus_clock_hz_get(fsp_priv_clock_t clock);        /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_peripheral_clock_hz_get(fsp_priv_clock_t clock); /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_extclk_hz_get(fsp_priv_clock_t clock);           /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* This array stores the clock frequency of each bus clock. This section of RAM should not be initialized by the C
 * runtime environment. This is initialized and used in R_BSP_InitClock, which is called before the C runtime
 * environment is initialized. */
uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM] =                            /* PRQA S 1514 # Advisory R-8.7 & Advisory R-8.9 */
{
    [FSP_PRIV_CLOCK_CLK_LSIOSC]     = BSP_CFG_CLOCK_CLK_LSIOSC_HZ,     ///< Initial value of CLK_LSIOSC clock
    [FSP_PRIV_CLOCK_CLK_HSIOSC]     = BSP_CFG_CLOCK_CLK_HSIOSC_HZ,     ///< Initial value of CLK_HSIOSC clock
    [FSP_PRIV_CLOCK_CLK_IOSC]       = BSP_CFG_CLOCK_CLK_IOSC_HZ,       ///< Initial value of CLK_IOSC clock
    [FSP_PRIV_CLOCK_CLK_SYS_IOSC]   = BSP_CFG_CLOCK_CLK_SYS_IOSC_HZ,   ///< Initial value of CLK_SYS_IOSC clock
    [FSP_PRIV_CLOCK_CLK_EMG]        = BSP_CFG_CLOCK_CLK_EMG_HZ,        ///< Initial value of CLK_EMG clock
    [FSP_PRIV_CLOCK_CLK_MOSC]       = BSP_CFG_CLOCK_CLK_MOSC_HZ,       ///< Initial value of CLK_MOSC clock
    [FSP_PRIV_CLOCK_CLK_SOSC]       = BSP_CFG_CLOCK_CLK_SOSC_HZ,       ///< Initial value of CLK_SOSC clock
    [FSP_PRIV_CLOCK_CLK_PLL]        = BSP_CFG_CLOCK_CLK_PLL_HZ,        ///< Initial value of CLK_PLL clock
    [FSP_PRIV_CLOCK_CLK_SSCG]       = BSP_CFG_CLOCK_CLK_SSCG_HZ,       ///< Initial value of CLK_SSCG clock
    [FSP_PRIV_CLOCK_CLK_SYS_CLEAN]  = BSP_CFG_CLOCK_CLK_SYS_CLEAN_HZ,  ///< Initial value of CLK_SYS_CLEAN clock
    [FSP_PRIV_CLOCK_CLK_SYS_SSCG]   = BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ,   ///< Initial value of CLK_SYS_SSCG clock
    [FSP_PRIV_CLOCK_CLK_CPU]        = BSP_CFG_CLOCK_CLK_CPU_HZ,        ///< Initial value of CLK_CPU clock
    [FSP_PRIV_CLOCK_CLK_SBUS]       = BSP_CFG_CLOCK_CLK_SBUS_HZ,       ///< Initial value of CLK_SBUS clock
    [FSP_PRIV_CLOCK_CLK_HBUS]       = BSP_CFG_CLOCK_CLK_HBUS_HZ,       ///< Initial value of CLK_HBUS clock
    [FSP_PRIV_CLOCK_CLK_UHSB]       = BSP_CFG_CLOCK_CLK_UHSB_HZ,       ///< Initial value of CLK_UHSB clock
    [FSP_PRIV_CLOCK_CLK_HSB]        = BSP_CFG_CLOCK_CLK_HSB_HZ,        ///< Initial value of CLK_HSB clock
    [FSP_PRIV_CLOCK_CLK_LSB]        = BSP_CFG_CLOCK_CLK_LSB_HZ,        ///< Initial value of CLK_LSB clock
    [FSP_PRIV_CLOCK_CLKC_SBUS]      = BSP_CFG_CLOCK_CLKC_SBUS_HZ,      ///< Initial value of CLKC_SBUS clock
    [FSP_PRIV_CLOCK_CLKC_HBUS]      = BSP_CFG_CLOCK_CLKC_HBUS_HZ,      ///< Initial value of CLKC_HBUS clock
    [FSP_PRIV_CLOCK_CLKC_UHSB]      = BSP_CFG_CLOCK_CLKC_UHSB_HZ,      ///< Initial value of CLKC_UHSB clock
    [FSP_PRIV_CLOCK_CLKC_HSB]       = BSP_CFG_CLOCK_CLKC_HSB_HZ,       ///< Initial value of CLKC_HSB clock
    [FSP_PRIV_CLOCK_CLKC_LSB]       = BSP_CFG_CLOCK_CLKC_LSB_HZ,       ///< Initial value of CLKC_LSB clock
    [FSP_PRIV_CLOCK_CLK_FLI]        = BSP_CFG_CLOCK_CLK_FLI_HZ,        ///< Initial value of CLK_FLI clock
    [FSP_PRIV_CLOCK_CLK_FACI]       = BSP_CFG_CLOCK_CLK_FACI_HZ,       ///< Initial value of CLK_FACI clock
    [FSP_PRIV_CLOCK_CLKA_LPS]       = BSP_CFG_CLOCK_CLKA_LPS_HZ,       ///< Initial value of CLKA_LPS clock
    [FSP_PRIV_CLOCK_CLKA_WDTBA]     = BSP_CFG_CLOCK_CLKA_WDTBA_HZ,     ///< Initial value of CLKA_WDTBA clock
    [FSP_PRIV_CLOCK_CLKA_TAUJ]      = BSP_CFG_CLOCK_CLKA_TAUJ_HZ,      ///< Initial value of CLKA_TAUJ clock
    [FSP_PRIV_CLOCK_CLKA_ADC]       = BSP_CFG_CLOCK_CLKA_ADC_HZ,       ///< Initial value of CLKA_ADC clock
    [FSP_PRIV_CLOCK_CLKA_RTCA]      = BSP_CFG_CLOCK_CLKA_RTCA_HZ,      ///< Initial value of CLKA_RTCA clock
    [FSP_PRIV_CLOCK_EXTCLK0O]       = BSP_CFG_CLOCK_EXTCLK0O_HZ,       ///< Initial value of EXTCLK0O clock
    [FSP_PRIV_CLOCK_EXTCLK1O]       = BSP_CFG_CLOCK_EXTCLK1O_HZ,       ///< Initial value of EXTCLK1O clock
    [FSP_PRIV_CLOCK_CLK_MSPI]       = BSP_CFG_CLOCK_CLK_MSPI_HZ,       ///< Initial value of CLK_MSPI clock
    [FSP_PRIV_CLOCK_CLK_RLIN3]      = BSP_CFG_CLOCK_CLK_RLIN3_HZ,      ///< Initial value of CLK_RLIN3 clock
    [FSP_PRIV_CLOCK_CLK_RLIN323]    = BSP_CFG_CLOCK_CLK_RLIN323_HZ,    ///< Initial value of CLK_RLIN323 clock
    [FSP_PRIV_CLOCK_CLK_CANFD_C]    = BSP_CFG_CLOCK_CLK_CANFD_C_HZ,    ///< Initial value of CLK_CANFD_C clock
    [FSP_PRIV_CLOCK_CLK_CANFD_XIN]  = BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ,  ///< Initial value of CLK_CANFD_XIN clock
    [FSP_PRIV_CLOCK_CLKC_CANXL_CH0] = BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ, ///< Initial value of CLKC_CANXL_CH0 clock
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
    [FSP_PRIV_CLOCK_CLKC_CANXL_CH1] = BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ, ///< Initial value of CLKC_CANXL_CH1 clock
#endif /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [FSP_PRIV_CLOCK_CLK_WDTB]   = BSP_CFG_CLOCK_CLK_WDTB_HZ,           ///< Initial value of CLK_WDTB clock
    [FSP_PRIV_CLOCK_CLK_GTM]    = BSP_CFG_CLOCK_CLK_GTM_HZ,            ///< Initial value of CLK_GTM clock
    [FSP_PRIV_CLOCK_CLK_ECMCNT] = BSP_CFG_CLOCK_CLK_ECMCNT_HZ,         ///< Initial value of CLK_ECMCNT clock
    [FSP_PRIV_CLOCK_CLK_SWDT]   = BSP_CFG_CLOCK_CLK_SWDT_HZ,           ///< Initial value of CLK_SWDT clock
};

/** PLL gear up sequence */
static const uint32_t g_pll_gear_up_repetitions_arr[] =                /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_4_DIV_16,                                     ///< Clock divided by 4
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,                                     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,                                     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,                                    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 640 MHz gear up sequence */
static const uint32_t g_sscg_640_gear_up_repetitions_arr[] =           /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_5_DIV_16,                                     ///< Clock divided by 5
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,                                     ///< Clock divided by 7
    BSP_CLOCK_DIVIDED_BY_9_DIV_16,                                     ///< Clock divided by 9
    BSP_CLOCK_DIVIDED_BY_11_DIV_16,                                    ///< Clock divided by 11
    BSP_CLOCK_DIVIDED_BY_13_DIV_16,                                    ///< Clock divided by 13
    BSP_CLOCK_DIVIDED_BY_15_DIV_16,                                    ///< Clock divided by 15
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,                                    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 480 MHz gear up sequence */
static const uint32_t g_sscg_480_gear_up_repetitions_arr[] =           /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,                                     ///< Clock divided by 7
    BSP_CLOCK_DIVIDED_BY_9_DIV_16,                                     ///< Clock divided by 9
    BSP_CLOCK_DIVIDED_BY_11_DIV_16,                                    ///< Clock divided by 11
    BSP_CLOCK_DIVIDED_BY_13_DIV_16,                                    ///< Clock divided by 13
    BSP_CLOCK_DIVIDED_BY_15_DIV_16,                                    ///< Clock divided by 15
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,                                    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 320 MHz gear up sequence */
static const uint32_t g_sscg_320_gear_up_repetitions_arr[] =           /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,                                    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 240 MHz gear up sequence */
static const uint32_t g_sscg_240_gear_up_repetitions_arr[] =           /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,                                    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** PLL gear down sequence */
static const uint32_t g_pll_gear_down_repetitions_arr[] =              /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,                                     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,                                     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_4_DIV_16,                                     ///< Clock divided by 4
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 640 MHz gear down sequence */
static const uint32_t g_sscg_640_gear_down_repetitions_arr[] =         /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,                                     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,                                     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_5_DIV_16,                                     ///< Clock divided by 5
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 480 MHz gear down sequence */
static const uint32_t g_sscg_480_gear_down_repetitions_arr[] =         /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,                                     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,                                     ///< Clock divided by 7
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 320 MHz gear down sequence */
static const uint32_t g_sscg_320_gear_down_repetitions_arr[] =         /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,                                    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,                                    ///< Clock divided by 10
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/** SSCG 240 MHz gear down sequence */
static const uint32_t g_sscg_240_gear_down_repetitions_arr[] =         /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,                                    ///< Clock divided by 12
    BSP_CLOCK_NO_DIVISION                                              ///< No division
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes variable to store system clock frequencies.
 **********************************************************************************************************************/
static void bsp_clock_freq_var_init (void)
{
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSIOSC]     = BSP_CFG_CLOCK_CLK_LSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSIOSC]     = BSP_CFG_CLOCK_CLK_HSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_IOSC]       = BSP_CFG_CLOCK_CLK_IOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS_IOSC]   = BSP_CFG_CLOCK_CLK_SYS_IOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_EMG]        = BSP_CFG_CLOCK_CLK_EMG_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MOSC]       = BSP_CFG_CLOCK_CLK_MOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SOSC]       = BSP_CFG_CLOCK_CLK_SOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_PLL]        = BSP_CFG_CLOCK_CLK_PLL_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SSCG]       = BSP_CFG_CLOCK_CLK_SSCG_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS_CLEAN]  = BSP_CFG_CLOCK_CLK_SYS_CLEAN_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS_SSCG]   = BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CPU]        = BSP_CFG_CLOCK_CLK_CPU_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SBUS]       = BSP_CFG_CLOCK_CLK_SBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HBUS]       = BSP_CFG_CLOCK_CLK_HBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_UHSB]       = BSP_CFG_CLOCK_CLK_UHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSB]        = BSP_CFG_CLOCK_CLK_HSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSB]        = BSP_CFG_CLOCK_CLK_LSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_SBUS]      = BSP_CFG_CLOCK_CLKC_SBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_HBUS]      = BSP_CFG_CLOCK_CLKC_HBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_UHSB]      = BSP_CFG_CLOCK_CLKC_UHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_HSB]       = BSP_CFG_CLOCK_CLKC_HSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_LSB]       = BSP_CFG_CLOCK_CLKC_LSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_FLI]        = BSP_CFG_CLOCK_CLK_FLI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_FACI]       = BSP_CFG_CLOCK_CLK_FACI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_LPS]       = BSP_CFG_CLOCK_CLKA_LPS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_WDTBA]     = BSP_CFG_CLOCK_CLKA_WDTBA_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_TAUJ]      = BSP_CFG_CLOCK_CLKA_TAUJ_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_ADC]       = BSP_CFG_CLOCK_CLKA_ADC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_RTCA]      = BSP_CFG_CLOCK_CLKA_RTCA_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK0O]       = BSP_CFG_CLOCK_EXTCLK0O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK1O]       = BSP_CFG_CLOCK_EXTCLK1O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MSPI]       = BSP_CFG_CLOCK_CLK_MSPI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RLIN3]      = BSP_CFG_CLOCK_CLK_RLIN3_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RLIN323]    = BSP_CFG_CLOCK_CLK_RLIN323_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_C]    = BSP_CFG_CLOCK_CLK_CANFD_C_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_XIN]  = BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_CANXL_CH0] = BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ;
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_CANXL_CH1] = BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ;
#endif                                 /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    g_clock_freq[FSP_PRIV_CLOCK_CLK_WDTB]   = BSP_CFG_CLOCK_CLK_WDTB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_GTM]    = BSP_CFG_CLOCK_CLK_GTM_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_ECMCNT] = BSP_CFG_CLOCK_CLK_ECMCNT_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SWDT]   = BSP_CFG_CLOCK_CLK_SWDT_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 **********************************************************************************************************************/
void bsp_clock_driver_init (void)
{
    /* Disable clock register write protection to allow configuration */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE;                                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm HS IntOSC is stable*/
    R_SYS0_A_CLOCK->HSOSCSTPM = BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE;                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->HSOSCS, (uint32_t) R_SYS0_A_CLOCK_HSOSCS_HSOSCSTAB_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* If Main OSC have been configured */
    R_SYS0_A_CLOCK->MOSCSTPM = BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE;                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm Main OSC is stopped */
    if (0U == R_SYS0_A_CLOCK->MOSCS)                                                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Enable MOSC */
        R_SYS0_A_CLOCK->MOSCE_b.MOSCENTRG = (uint32_t) R_SYS0_A_CLOCK_MOSCE_MOSCENTRG_Msk;              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Confirm that the Main OSC have been stable (MOSCS.MOSCEN = 1 & MOSCS.MOSCSTAB = 1) */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS,                                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCSTAB_Msk));

#if (BSP_CFG_CLOCK_CLK_SOSC_HZ > 0)

    /* Confirm Sub OSC is stopped */
    if (0U == R_SYS0_A_CLOCK->SOSCS)                                           /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        R_SYS0_A_CLOCK->SOSCE = (uint32_t) R_SYS0_A_CLOCK_SOSCE_SOSCENTRG_Msk; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that the Sub OSC has been stable
         * (SOSCS.SOSCEN = 1 & SOSCS.SOSCSTAB = 1)*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x03U);              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

#else

    /* Confirm that the Sub OSC is stable */
    if (1U == R_SYS0_A_CLOCK->SOSCS_b.SOSCSTAB)                                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        R_SYS0_A_CLOCK->SOSCE = (uint32_t) R_SYS0_A_CLOCK_SOSCE_SOSCDISTRG_Msk;        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that the Sub OSC has been stopped (SOSCS.SOSCEN = 0 & SOSCS.SOSCSTAB = 0).*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x00U);                      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
#endif                                                                                 /* End of (BSP_CFG_CLOCK_CLK_SOSC_HZ > 0) */ /* PRQA S 2053 # Advisory D-4.4 */

    /* Check if the system clock is selected as PLLO/SSCGO */
    if (0 != BSP_CFG_CLOCK_CLK_PLL_HZ)                                                 /* PRQA S 2741 # DP_MSR_R-14.3_UC1 */
    {
        R_SYS0_CLOCK->PLLSTPM = BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE;                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm the status of PLL */
        if (0U == R_SYS0_CLOCK->PLLS)                                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* The status of PLL is stopped (PLLS.PLLCLKEN = 0 & PLLS.PLLCLKSTAB = 0) */
            /* Start the PLL: PLLE.PLLENTRG = 1 */
            R_SYS0_CLOCK->PLLE_b.PLLENTRG = (uint32_t) R_SYS0_CLOCK_PLLE_PLLENTRG_Msk; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Confirm that the PLL has been stable (PLLS.PLLCLKEN = 1 & PLLS.PLLCLKSTAB = 1) */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS,                                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                   ((uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk | \
                                    (uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk));
    }

    /* Check if the configured system clock is PLLO/SSCGO/SSCG1O */
    /* Gear Up/Down the PLLO */
    bsp_gear_up_down_pllo((uint8_t) BSP_CFG_CLOCK_CKS_CLEANC_SRC, (uint32_t) BSP_CFG_CLOCK_CLK_PLL_HZ);

    /* PRQA S 0306, 0662 ++ # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    R_SYS0_CLOCK->SSCGC = (((uint32_t) BSP_CFG_CLK_SSCG_MODULATION_MODE << R_SYS0_CLOCK_SSCGC_SSMODE1_Pos) |   \
                           ((uint32_t) BSP_CFG_CLK_SSCG_MODULATION_VALUE << R_SYS0_CLOCK_SSCGC_SELMFREQ_Pos) | \
                           ((uint32_t) BSP_CFG_CLK_SSCG_MODULATION_DITHERED_RANGE));

    /* PRQA S 0306, 0662 -- */

    /* Gear Up/Down the SSCGO */
    bsp_gear_up_down_sscg((uint8_t) BSP_CFG_CLOCK_CKS_SSCGC_SRC,
                          (volatile st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC), /* PRQA S 0310, 0306, 0662 # DP_MSR_R-11.3_UC1, Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                          (uint32_t) BSP_CFG_CLOCK_CLK_SSCG_HZ);

    /* CLK_RLIN3 divider setting and CLK_RLIN3_CH23 divider setting */
    R_SYS0_CLOCK->CKS_RLINC =                                                              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                              (((uint32_t) BSP_CFG_CLOCK_CLK_RLIN323_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CLK_RLIN3_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS,                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) BSP_CFG_CLOCK_CLK_RLIN323_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CLK_RLIN3_SRC));

    /* CLK_CANFD_C selector setting */
    R_SYS0_CLOCK->CKS_RCANC =                           /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
                              (((uint32_t) BSP_CFG_CLOCK_CLK_CANFD_C_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RCANS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) BSP_CFG_CLOCK_CLK_CANFD_C_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC));

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = (uint32_t) BSP_CFG_CLOCK_CLK_MSPI_SRC;                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, (uint32_t) BSP_CFG_CLOCK_CLK_MSPI_SRC);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLK_SWDT selector setting */
    R_SYS0_CLOCK->CKS_SWDTC = (uint32_t) BSP_CFG_CLOCK_CLK_SWDT_SRC;                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_SWDTS, (uint32_t) BSP_CFG_CLOCK_CLK_SWDT_SRC);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKC_CANXL_CH0 selector setting */
    R_SYS0_CLOCK->CKS_CANXL0C = (uint32_t) BSP_CFG_CLOCK_CLKC_CANXL_CH0_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CANXL0S, (uint32_t) BSP_CFG_CLOCK_CLKC_CANXL_CH0_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)

    /* CLKC_CANXL_CH1 selector setting */
    R_SYS0_CLOCK->CKS_CANXL1C = (uint32_t) BSP_CFG_CLOCK_CLKC_CANXL_CH1_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CANXL1S, (uint32_t) BSP_CFG_CLOCK_CLKC_CANXL_CH1_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif                                                                                                  /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

    /* CLKA_WDTBA selector setting */
    R_SYS0_A_CLOCK->CKS_AWDTC = (uint32_t) BSP_CFG_CLOCK_CLKA_WDTBA_SRC;                                /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AWDTS, (uint32_t) BSP_CFG_CLOCK_CLKA_WDTBA_SRC);     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKS_ATAUJC = (uint32_t) BSP_CFG_CLOCK_CLKA_TAUJ_SRC;                                /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS, (uint32_t) BSP_CFG_CLOCK_CLKA_TAUJ_SRC);     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKS_ARTCAC = (uint32_t) BSP_CFG_CLOCK_CLKA_RTCA_SRC;                                /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, (uint32_t) BSP_CFG_CLOCK_CLKA_RTCA_SRC);     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKS_AADCC = (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_SRC;                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS, (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_SRC);       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0U;                                                                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk)); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK0C = (uint32_t) BSP_CFG_CLOCK_EXTCLK0O_SRC;                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, (uint32_t) BSP_CFG_CLOCK_EXTCLK0O_SRC);                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S_b.EXTCLK0SYNC,                                              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = (uint32_t) BSP_CFG_CLOCK_EXTCLK0O_DIV;                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S,                                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0CLKACT_Msk));
#endif                                                                                                                  /* #if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

#if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0U;                                                                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk)); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK1C = (uint32_t) BSP_CFG_CLOCK_EXTCLK1O_SRC;                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, (uint32_t) BSP_CFG_CLOCK_EXTCLK1O_SRC);                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S_b.EXTCLK1SYNC,                                              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = (uint32_t) BSP_CFG_CLOCK_EXTCLK1O_DIV;                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S,                                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1CLKACT_Msk));
#endif                                                                                                                  /* #if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

    /* CLK_WDTB selector setting */
    R_SYS0_A_CLOCK->CKS_WDTC = (uint32_t) BSP_CFG_CLOCK_CLK_WDTB_SRC;                                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_WDTS, (uint32_t) BSP_CFG_CLOCK_CLK_WDTB_SRC);                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* If the PLL is not used */
    if (0 == BSP_CFG_CLOCK_CLK_PLL_HZ)                                                                                  /* PRQA S 2742 # DP_MSR_R-14.3_UC1 */
    {
        /* Confirm that the PLL is stable */
        if (((uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk | (uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk) ==
            R_SYS0_CLOCK->PLLS)                                                                                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Stop the PLL (PLLE.PLLDISTRG = 1). */
            R_SYS0_CLOCK->PLLE_b.PLLDISTRG = 1U;                                                                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Confirm that the PLL has been stopped (PLLS.PLLCLKEN = 0 & PLLS.PLLCLKSTAB = 0) */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS, 0U);                                                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Disable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;                                                                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    bsp_clock_freq_var_init();
    __SYNCP();                                                                                                          /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Gear Down Clock.
 **********************************************************************************************************************/
void bsp_clock_gear_down (void)
{
    /* Disable write protection of clock controller (enable register write access) */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Gear Down the PLLO */
    uint32_t pll_clock_freq = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_PLL);
    bsp_gear_up_down_pllo(1U, (uint32_t) pll_clock_freq);

    /* Gear Down the SSCGO */
    uint32_t sscg_clock_freq = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_SSCG);
    bsp_gear_up_down_sscg(1U,
                          (volatile st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC), /* PRQA S 0306, 0310, 0662 # Advisory R-11.4, DP_MSR_R-11.3_UC1, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                          (uint32_t) sscg_clock_freq);

    /* Enable write protection of clock controller (lock registers) */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Flush pipeline after clock source change */
    __SYNCP();                                                                             /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * System Clock (PLLO) Gear Up.
 **********************************************************************************************************************/
static void bsp_gear_up_down_pllo (uint8_t gear_up_down_select, uint32_t pll_clock_hz)
{
    uint8_t repetition_count     = 0U;
    uint8_t repetition_start     = 1U;
    uint8_t total_of_repetitions = 0U;

    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(pll_clock_hz);

    if (0U != gear_up_down_select)
    {
        /* Get the total of repetitions */
        total_of_repetitions =
            (uint8_t) (sizeof(g_pll_gear_down_repetitions_arr) / sizeof(g_pll_gear_down_repetitions_arr[0]));

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        /* PRQA S 0306, 0662 ++ # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        if ((0U == R_SYS0_CLOCK->CKS_CLEANS) && (0U == R_SYS0_CLOCK->CKD_PLLC) && \
            ((uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK == R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC))

        /* PRQA S 0306, 0662 -- */
        {
            for (repetition_count = 0U; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to PLL0 */
                R_SYS0_CLOCK->CKD_PLLC = g_pll_gear_down_repetitions_arr[repetition_count]; /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */

                /* Confirm that PLL0 dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC,            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                           (uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK);
            }

            /* Division ratio of clock source HSIOSC is changed to 1/1 */
            /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
            if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)     /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
            {
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;      /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                           (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* Change the clock source for the system clock to CLK_IOSC */
            R_SYS0_CLOCK->CKS_CLEANC = 1U;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CLEANS, 1U); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }
    else
    {
        /* Get the total of repetitions */
        total_of_repetitions =
            (uint8_t) (sizeof(g_pll_gear_up_repetitions_arr) / sizeof(g_pll_gear_up_repetitions_arr[0]));

        /* Shifting the System Clock Gear Up for System Clock */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_4_DIV_16;                                          /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, BSP_CKD_PLLS_PLLCLKDSYNC_MASK); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
        if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            if (2U == R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)                                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /* 01B if current CKD_HSIOSC setting is 10B (DIV10) */
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 1U;                                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS,                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                           (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* 00B if current CKD_HSIOSC setting is 01B (DIV2) */
            R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Verify that the value of HSIOSCCLKDSYNC is 1B */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                       (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
        }

        /* Change the clock source to CLK_PLLO */
        R_SYS0_CLOCK->CKS_CLEANC = 0U;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CLEANS, 0U); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to PLL0 */
            R_SYS0_CLOCK->CKD_PLLC = g_pll_gear_up_repetitions_arr[repetition_count];                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Confirm that PLL0 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, (uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100U, BSP_DELAY_UNITS_MICROSECONDS);
}

/*******************************************************************************************************************//**
 * System Clock (SSCGO/SSCG1O) Gear Up/Down.
 **********************************************************************************************************************/
static void bsp_gear_up_down_sscg (uint8_t                            gear_up_down_select,
                                   volatile st_systemclock_sscg_ctl * sscg_register,
                                   uint32_t                           sscg_clock_hz)
{
    uint8_t          repetition_count     = 0U;
    uint8_t          repetition_start     = 1U;
    uint8_t          total_of_repetitions = 0U;
    const uint32_t * repetitions_arr      = NULL;

    if (0U != gear_up_down_select)
    {
        /* Get the total of repetitions */
        switch (sscg_clock_hz)
        {
            case BSP_CLOCK_SYSTEM_640MHz:
            {
                repetitions_arr      = &g_sscg_640_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_640_gear_down_repetitions_arr) /
                               sizeof(g_sscg_640_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_480MHz:
            {
                repetitions_arr      = &g_sscg_480_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_480_gear_down_repetitions_arr) /
                               sizeof(g_sscg_480_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_320MHz:
            {
                repetitions_arr      = &g_sscg_320_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_320_gear_down_repetitions_arr) /
                               sizeof(g_sscg_320_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_240MHz:
            {
                repetitions_arr      = &g_sscg_240_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_240_gear_down_repetitions_arr) /
                               sizeof(g_sscg_240_gear_down_repetitions_arr[0]));
                break;
            }

            default:                   /* PRQA S 2016 # DP_MSR_R-16.4_UC1 */
            {
                break;
            }
        }

        /* After the switch block in the gear-down (if) branch */
        if (0U == total_of_repetitions)
        {
            return;                    /* Unsupported SSCG frequency - skip gear-down */
        }

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0U == sscg_register->CKS_SSCGS) && \
            (0U == sscg_register->CKD_SSCGC) && \
            ((uint32_t) R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk == sscg_register->CKD_SSCGS))
        {
            for (repetition_count = 0U; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to SSCGO */
                sscg_register->CKD_SSCGC = repetitions_arr[repetition_count];

                /* Confirm that SSCGO/SSCG1O dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, (uint32_t) R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
            }

            /* Division ratio of clock source HSIOSC is changed to 1/1 */
            /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
            if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                           (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* Change the clock source for the system clock to CLK_IOSC */
            sscg_register->CKS_SSCGC = 1U;

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKS_SSCGS, 1U);
        }
    }
    else
    {
        /* Get the total of repetitions */
        switch (sscg_clock_hz)
        {
            case BSP_CLOCK_SYSTEM_640MHz:
            {
                repetitions_arr      = &g_sscg_640_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_640_gear_up_repetitions_arr) /
                               sizeof(g_sscg_640_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_480MHz:
            {
                repetitions_arr      = &g_sscg_480_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_480_gear_up_repetitions_arr) /
                               sizeof(g_sscg_480_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_320MHz:
            {
                repetitions_arr      = &g_sscg_320_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_320_gear_up_repetitions_arr) /
                               sizeof(g_sscg_320_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_240MHz:
            {
                repetitions_arr      = &g_sscg_240_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (uint8_t) (sizeof(g_sscg_240_gear_up_repetitions_arr) /
                               sizeof(g_sscg_240_gear_up_repetitions_arr[0]));
                break;
            }

            default:                   /* PRQA S 2016 # DP_MSR_R-16.4_UC1 */
            {
                break;
            }
        }

        /* After the switch block in the gear-up (else) branch */
        if (0U == total_of_repetitions)
        {
            return;                    /* Unsupported SSCG frequency - skip gear-up */
        }

        /* Shifting the System Clock Gear Up for System Clock */
        sscg_register->CKD_SSCGC = repetitions_arr[0];

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);

        /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
        if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            if (2U == R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                /* 01B if current CKD_HSIOSC setting is 10B (DIV10) */
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 1U;      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                           (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* 00B if current CKD_HSIOSC setting is 01B (DIV2) */
            R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Verify that the value of HSIOSCCLKDSYNC is 1B */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                       (uint32_t) R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
        }

        /* Setting clock source to CLK_SSCGO */
        sscg_register->CKS_SSCGC = 0U;

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKS_SSCGS, 0U);

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to SSCGO/SSCG1O */
            sscg_register->CKD_SSCGC = repetitions_arr[repetition_count];

            /* Confirm that SSCGO/SSCG1O dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, (uint32_t) R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100U, BSP_DELAY_UNITS_MICROSECONDS);
}

/*******************************************************************************************************************//**
 * Get the specific clock's frequency.
 **********************************************************************************************************************/
uint32_t r_bsp_clock_hz_calculate (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_PLL:
        case FSP_PRIV_CLOCK_CLK_SSCG:
        case FSP_PRIV_CLOCK_CLK_LSIOSC:
        case FSP_PRIV_CLOCK_CLK_HSIOSC:
        case FSP_PRIV_CLOCK_CLK_IOSC:
        case FSP_PRIV_CLOCK_CLK_SYS_IOSC:
        case FSP_PRIV_CLOCK_CLK_MOSC:
        case FSP_PRIV_CLOCK_CLK_SOSC:
        case FSP_PRIV_CLOCK_CLK_EMG:
        {
            frequency = bsp_source_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SYS_SSCG:
        case FSP_PRIV_CLOCK_CLK_SYS_CLEAN:
        {
            frequency = bsp_system_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CLKC_SBUS:
        case FSP_PRIV_CLOCK_CLKC_HBUS:
        case FSP_PRIV_CLOCK_CLKC_UHSB:
        case FSP_PRIV_CLOCK_CLKC_HSB:
        case FSP_PRIV_CLOCK_CLKC_LSB:
        case FSP_PRIV_CLOCK_CLK_CPU:
        case FSP_PRIV_CLOCK_CLK_SBUS:
        case FSP_PRIV_CLOCK_CLK_HBUS:
        case FSP_PRIV_CLOCK_CLK_UHSB:
        case FSP_PRIV_CLOCK_CLK_HSB:
        case FSP_PRIV_CLOCK_CLK_LSB:
        {
            frequency = bsp_bus_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_MSPI:
        case FSP_PRIV_CLOCK_CLK_RLIN3:
        case FSP_PRIV_CLOCK_CLK_RLIN323:
        case FSP_PRIV_CLOCK_CLK_GTM:
        case FSP_PRIV_CLOCK_CLK_WDTB:
        case FSP_PRIV_CLOCK_CLK_CANFD_C:
        case FSP_PRIV_CLOCK_CLK_CANFD_XIN:
        case FSP_PRIV_CLOCK_CLKC_CANXL_CH0:
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CLKC_CANXL_CH1:
#endif                                 /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
        case FSP_PRIV_CLOCK_CLK_SWDT:
        case FSP_PRIV_CLOCK_CLK_FLI:
        case FSP_PRIV_CLOCK_CLK_FACI:
        case FSP_PRIV_CLOCK_CLKA_LPS:
        case FSP_PRIV_CLOCK_CLKA_WDTBA:
        case FSP_PRIV_CLOCK_CLKA_TAUJ:
        case FSP_PRIV_CLOCK_CLKA_ADC:
        case FSP_PRIV_CLOCK_CLKA_RTCA:
        case FSP_PRIV_CLOCK_CLK_ECMCNT:
        {
            /* PRQA S 1520 ++ # DP_MSR_R-17.2_UC1 */
            frequency = bsp_peripheral_clock_hz_get(clock);

            /* PRQA S 1520 -- */
            break;
        }

        case FSP_PRIV_CLOCK_EXTCLK0O:
        case FSP_PRIV_CLOCK_EXTCLK1O:
        {
            frequency = bsp_extclk_hz_get(clock);
            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*******************************************************************************************************************//**
 * Get the clock source frequency.
 **********************************************************************************************************************/
static uint32_t bsp_source_clock_hz_get (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t             frequency            = 0U;
    static const uint8_t clock_sys_dividers[] =
    {
        BSP_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE, BSP_CLOCK_CLK_SYS_IOSC_DIV_2_VALUE, BSP_CLOCK_CLK_SYS_IOSC_DIV_10_VALUE,
        BSP_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE
    };
    uint8_t               clock_sys_divider = clock_sys_dividers[R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID]; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t               opbt_mosc_freq_value;
    static const uint32_t main_os_clocks[] =
    {
        BSP_CLKCTL_CLK_MOSC_8MHZ,    BSP_CLKCTL_CLK_MOSC_16MHZ,    BSP_CLKCTL_CLK_MOSC_20MHZ,
        BSP_CLKCTL_CLK_MOSC_24MHZ,
        BSP_CLKCTL_CLK_MOSC_DISABLE, BSP_CLKCTL_CLK_MOSC_DISABLE,  BSP_CLKCTL_CLK_MOSC_DISABLE,
        BSP_CLKCTL_CLK_MOSC_DISABLE
    };
    uint8_t               opbt_ckdivmd_value;
    static const uint32_t sscg_clocks[] =
    {
        BSP_CLKCTL_CLK_SSCG_240MHZ, BSP_CLKCTL_CLK_SSCG_240MHZ, BSP_CLKCTL_CLK_SSCG_320MHZ, BSP_CLKCTL_CLK_SSCG_320MHZ,
        BSP_CLKCTL_CLK_SSCG_480MHZ, BSP_CLKCTL_CLK_SSCG_480MHZ, BSP_CLKCTL_CLK_SSCG_640MHZ, BSP_CLKCTL_CLK_SSCG_640MHZ
    };
    uint8_t               sosc_enable   = 0U;
    static const uint32_t sosc_clocks[] = {0U, BSP_CLOCK_SOSC_VALUE};

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_LSIOSC:
        {
            frequency = BSP_CLOCK_LSIOSC_VALUE; /* LS IntOSC clock = 32.768 kHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_HSIOSC:
        {
            frequency = BSP_CLOCK_HSIOSC_VALUE; /* HS IntOSC clock = 200 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_IOSC:
        {
            frequency = BSP_CLOCK_IOSC_VALUE; /* Internal OSC clock = HS IntOSC clock = 200 MHz,
                                               * Internal OSC clock = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                               * stops in chip standby mode (Stop / DeepStop mode)*/
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SYS_IOSC:
        {
            frequency = BSP_CLOCK_HSIOSC_VALUE / clock_sys_divider; /* Internal OSC clock for CLK_SYS Domain = HS IntOSC clock (/1, /2, /10) = 200/100/20 MHz
                                                                     * Internal OSC clock for CLK_SYS Domain = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                                                     * stops in chip standby mode (Stop / DeepStop mode) */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_EMG:
        {
            frequency = BSP_CLOCK_HSIOSC_VALUE / 20U; /* Emergency clock = HS IntOSC clock / 20 = 10 MHz
                                                       * Emergency clock = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                                       * stops in chip standby mode (Stop / DeepStop mode) */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_MOSC:
        {
            opbt_mosc_freq_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(10U) & BSP_OPBT_MOSC_FREQ_MSK) >> /* PRQA S 0303, 0499, 3432 # Advisory R-11.4, DP_MSR_R-12.2_UC1, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_MOSC_FREQ_POS);
            frequency = main_os_clocks[opbt_mosc_freq_value];                           /* Main OSC clock = 8 MHz / 16 MHz / 20 MHz / 24 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SOSC:
        {
            sosc_enable = R_SYS0_A_CLOCK->SOSCS & R_SYS0_A_CLOCK_SOSCS_SOSCEN_Msk; /* PRQA S 0306, 0662, 4461 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, DP_MSR_R-10.3_UC1 */
            frequency   = sosc_clocks[sosc_enable];                                /* Sub OSC clock = 0 Hz / 32.768 kHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_PLL:
        {
            frequency = BSP_CLOCK_PLL_VALUE; /* PLL clock clock = 800 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SSCG:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency = sscg_clocks[opbt_ckdivmd_value];                              /* SSCG clock = 640 MHz / 480 MHz / 320 MHz / 240 MHz */
            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*******************************************************************************************************************//**
 * Get the system clock's frequency.
 **********************************************************************************************************************/
static uint32_t bsp_system_clock_hz_get (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t             frequency = 0U;
    static const uint8_t pllo_sscgo_divider_numerators[] =
    {
        1U, 0U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 9U, 10U, 11U, 12U, 13U, 14U, 15U
    };
    static const uint8_t pllo_sscgo_divider_denominators[] =
    {
        1U, 1U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U, 16U
    };

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_SYS_SSCG:
        {
            /* Check the source clock of System clock for SSCG Domain */
            if (1U == R_SYS0_CLOCK->CKS_SSCGS) /* CLK_SYS_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_IOSC);
            }
            else /* CLK_SSCGO */
            {
                uint8_t sscgo_divider_option = R_SYS0_CLOCK->CKD_SSCGC & R_SYS0_CLOCK_CKD_SSCGC_PLLCLKDCSID_Msk; /* PRQA S 4461, 0306, 0662 # DP_MSR_R-10.3_UC1, Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_SSCG);

                frequency = (frequency * pllo_sscgo_divider_numerators[sscgo_divider_option]) /
                            pllo_sscgo_divider_denominators[sscgo_divider_option];
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLK_SYS_CLEAN:
        {
            /* Check the source clock of System clock for Clean Domain */
            if (1U == R_SYS0_CLOCK->CKS_CLEANS) /* CLK_SYS_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_IOSC);
            }
            else /* CLK_PLLO */
            {
                uint8_t pllo_divider_option = R_SYS0_CLOCK->CKD_PLLC & R_SYS0_CLOCK_CKD_PLLC_PLLCLKDCSID_Msk; /* PRQA S 4461, 0306, 0662 # DP_MSR_R-10.3_UC1, Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_PLL);

                frequency = (frequency * pllo_sscgo_divider_numerators[pllo_divider_option]) /
                            pllo_sscgo_divider_denominators[pllo_divider_option];
            }

            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*******************************************************************************************************************//**
 * Get the bus clock's frequency.
 **********************************************************************************************************************/
static uint32_t bsp_bus_clock_hz_get (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency;

    uint8_t              opbt_ckdivmd_value;
    static const uint8_t clk_sbus_dividers[] = {4U, 4U, 4U, 4U, 4U, 6U, 4U, 4U};
    static const uint8_t clk_hbus_dividers[] = {4U, 4U, 4U, 4U, 8U, 6U, 8U, 8U};
    static const uint8_t clk_uhsb_dividers[] = {2U, 2U, 2U, 2U, 4U, 3U, 4U, 4U};
    static const uint8_t clk_hsb_dividers[]  = {4U, 4U, 4U, 4U, 8U, 6U, 8U, 8U};
    static const uint8_t clk_lsb_dividers[]  = {8U, 8U, 8U, 8U, 16U, 12U, 16U, 16U};

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLKC_SBUS:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_CLEAN);
            frequency /= 4U;
            break;
        }

        case FSP_PRIV_CLOCK_CLKC_HBUS:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_CLEAN);
            frequency /= 8U;
            break;
        }

        case FSP_PRIV_CLOCK_CLKC_UHSB:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_CLEAN);
            frequency /= 5U;
            break;
        }

        case FSP_PRIV_CLOCK_CLKC_HSB:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_CLEAN);
            frequency /= 10U;
            break;
        }

        case FSP_PRIV_CLOCK_CLKC_LSB:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_CLEAN);
            frequency /= 20U;
            break;
        }

        case FSP_PRIV_CLOCK_CLK_CPU:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= 2U;
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= clk_sbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_HBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= clk_hbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_UHSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= clk_uhsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_HSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= clk_hsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_LSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0303, 0499, 3432 # Advisory R-11.4, DP_MSR_R-12.2_UC1, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS_SSCG);
            frequency /= clk_lsb_dividers[opbt_ckdivmd_value];
            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*******************************************************************************************************************//**
 * Get the peripheral clock's frequency.
 **********************************************************************************************************************/
static uint32_t bsp_peripheral_clock_hz_get (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency;
    uint8_t  clock_src;

    static const fsp_priv_clock_t mspi_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLKC_HSB, FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t rlin3_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG,  FSP_PRIV_CLOCK_CLKC_HSB,  FSP_PRIV_CLOCK_CLK_MOSC,  FSP_PRIV_CLOCK_CLK_MOSC,
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_NUM,       FSP_PRIV_CLOCK_NUM,       FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t rlin3_23_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG,  FSP_PRIV_CLOCK_CLKC_HBUS,  FSP_PRIV_CLOCK_CLK_MOSC,   FSP_PRIV_CLOCK_CLK_MOSC,
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_NUM,        FSP_PRIV_CLOCK_NUM,        FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t gtm_clock_srcs[] = {FSP_PRIV_CLOCK_CLKC_SBUS, FSP_PRIV_CLOCK_CLKC_UHSB};
    static const uint16_t         wdtb_clock_div[] =
    {
        BSP_CLOCK_WDTB_DIV_20_VALUE, BSP_CLOCK_WDTB_DIV_640_VALUE, BSP_CLOCK_WDTB_DIV_800_VALUE,
        BSP_CLOCK_WDTB_NO_DIV_VALUE
    };
    static const fsp_priv_clock_t canfd_c_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG, FSP_PRIV_CLOCK_CLKC_HSB, FSP_PRIV_CLOCK_CLKC_HSB, FSP_PRIV_CLOCK_CLKC_HSB
    };
    static const fsp_priv_clock_t canfd_xin_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_MOSC
    };
    static const fsp_priv_clock_t canxl_clock_srcs[] =
    {
        [0] = FSP_PRIV_CLOCK_CLK_EMG,  /* PRQA S 3676 # DP_MSR_R-9.5_UC1 */
        [1] = FSP_PRIV_CLOCK_CLKC_HSB,
        [2] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [3] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [4] = FSP_PRIV_CLOCK_CLKC_UHSB,
        [5] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [6] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [7] = FSP_PRIV_CLOCK_NUM       /* Setting prohibited, will return 0 value */
    };
    static const uint16_t swdt_clock_div[] =
    {
        BSP_CLOCK_SWDT_DIV_20_VALUE, BSP_CLOCK_SWDT_DIV_800_VALUE, BSP_CLOCK_SWDT_DIV_3060_VALUE,
        BSP_CLOCK_SWDT_NO_DIV_VALUE
    };
    static const uint16_t         wdta_clock_div[]      = {BSP_CLOCK_WDTA_NO_DIV_VALUE, BSP_CLOCK_WDTA_DIV_128_VALUE};
    static const fsp_priv_clock_t tauj_awo_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG,  FSP_PRIV_CLOCK_CLK_LSIOSC,  FSP_PRIV_CLOCK_CLK_HSIOSC,  FSP_PRIV_CLOCK_CLK_MOSC,
        FSP_PRIV_CLOCK_CLKC_HSB, FSP_PRIV_CLOCK_NUM,         FSP_PRIV_CLOCK_NUM,         FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t adc_awo_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG, FSP_PRIV_CLOCK_CLK_HSIOSC, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLKC_LSB
    };
    static const fsp_priv_clock_t rtca_awo_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_EMG, FSP_PRIV_CLOCK_CLK_LSIOSC, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_SOSC
    };

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_MSPI:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_MSPIS & R_SYS0_CLOCK_CKS_MSPIS_MSPISACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(mspi_clock_srcs[clock_src]);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_RLIN3:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_RLINS & R_SYS0_CLOCK_CKS_RLINS_RLINSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(rlin3_clock_srcs[clock_src]);

            if (BSP_CLOCK_RLIN3_MOSC_DIV_4_VALUE == clock_src)
            {
                frequency /= 4U;
            }
            else if (BSP_CLOCK_RLIN3_MOSC_DIV_8_VALUE == clock_src)
            {
                frequency /= 8U;
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLK_RLIN323:
        {
            clock_src =
                (uint8_t) ((R_SYS0_CLOCK->CKS_RLINS & R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Msk) >> /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                           R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos);
            frequency = r_bsp_clock_hz_calculate(rlin3_23_clock_srcs[clock_src]);

            if (BSP_CLOCK_RLIN3_MOSC_DIV_4_VALUE == clock_src)
            {
                frequency /= 4U;
            }
            else if (BSP_CLOCK_RLIN3_MOSC_DIV_8_VALUE == clock_src)
            {
                frequency /= 8U;
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLK_GTM:
        {
            clock_src =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(8U) & BSP_OPBT_CKSEL_GTM_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKSEL_GTM_POS);
            frequency = r_bsp_clock_hz_calculate(gtm_clock_srcs[clock_src]);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_WDTB:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_WDTS & R_SYS0_A_CLOCK_CKS_WDTS_WDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_HSIOSC) / wdtb_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_CANFD_C:
        {
            clock_src =
                (uint8_t) ((R_SYS0_CLOCK->CKS_RCANS & R_SYS0_CLOCK_CKS_RCANS_RCANCSACT_Msk) >> /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                           R_SYS0_CLOCK_CKS_RCANS_RCANCSACT_Pos);
            frequency = r_bsp_clock_hz_calculate(canfd_c_clock_srcs[clock_src]);

            if (BSP_CLOCK_CANFD_DIV_2_VALUE == clock_src)
            {
                frequency /= 2U;
            }
            else if (BSP_CLOCK_CANFD_DIV_4_VALUE == clock_src)
            {
                frequency /= 4U;
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLK_CANFD_XIN:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_RCANS & R_SYS0_CLOCK_CKS_RCANS_RCANXINSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(canfd_xin_clock_srcs[clock_src]);

            if (BSP_CLOCK_CANFD_DIV_2_VALUE == clock_src)
            {
                frequency /= 2U;
            }
            else if (BSP_CLOCK_CANFD_DIV_4_VALUE == clock_src)
            {
                frequency /= 4U;
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLKC_CANXL_CH0:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_CANXL0S & R_SYS0_CLOCK_CKS_CANXL0S_CANXLSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(canxl_clock_srcs[clock_src]);
            break;
        }

#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CLKC_CANXL_CH1:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_CANXL1S & R_SYS0_CLOCK_CKS_CANXL1S_CANXLSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(canxl_clock_srcs[clock_src]);

            break;
        }
#endif                                                                                             /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

        case FSP_PRIV_CLOCK_CLK_SWDT:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_SWDTS & R_SYS0_CLOCK_CKS_SWDTS_SWDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_HSIOSC) / swdt_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_FLI:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_CPU);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_FACI:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_LSB);
            break;
        }

        case FSP_PRIV_CLOCK_CLKA_LPS:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_EMG);
            break;
        }

        case FSP_PRIV_CLOCK_CLKA_WDTBA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_AWDTS & R_SYS0_A_CLOCK_CKS_AWDTS_AWDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_LSIOSC) / wdta_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLKA_TAUJ:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_ATAUJS & R_SYS0_A_CLOCK_CKS_ATAUJS_ATAUJSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(tauj_awo_clock_srcs[clock_src]);

            if (BSP_CLOCK_ATAUJ_HSIOSC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLKA_ADC:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_AADCS & R_SYS0_A_CLOCK_CKS_AADCS_AADCSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(adc_awo_clock_srcs[clock_src]);

            if (BSP_CLOCK_AADC_HSIOSC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLKA_RTCA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_ARTCAS & R_SYS0_A_CLOCK_CKS_ARTCAS_ARTCASACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(rtca_awo_clock_srcs[clock_src]);

            if (BSP_CLOCK_ARTCA_MOSC_DIV_16_VALUE == clock_src)
            {
                frequency /= 16U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLK_ECMCNT:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_EMG);
            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*******************************************************************************************************************//**
 * Get the external clock's frequency.
 **********************************************************************************************************************/
static uint32_t bsp_extclk_hz_get (fsp_priv_clock_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency;
    uint8_t  clock_src;
    uint16_t clock_div;
    static const fsp_priv_clock_t extclk0_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_LSIOSC, FSP_PRIV_CLOCK_CLK_HSIOSC, FSP_PRIV_CLOCK_CLK_SOSC,
        FSP_PRIV_CLOCK_CLK_HSB,  FSP_PRIV_CLOCK_CLKC_HSB,   FSP_PRIV_CLOCK_NUM,        FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t extclk1_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_LSIOSC, FSP_PRIV_CLOCK_CLK_HSIOSC, FSP_PRIV_CLOCK_CLK_SOSC,
        FSP_PRIV_CLOCK_CLK_HSB,  FSP_PRIV_CLOCK_CLKC_HSB,   FSP_PRIV_CLOCK_NUM,        FSP_PRIV_CLOCK_NUM
    };

    switch (clock)
    {
        case FSP_PRIV_CLOCK_EXTCLK0O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_EXTCLK0S & R_SYS0_A_CLOCK_CKS_EXTCLK0S_EXTCLK0SACT_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CKD_EXTCLK0C & R_SYS0_A_CLOCK_CKD_EXTCLK0C_EXTCLK0DIV_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            frequency = (0U != clock_div) ? (r_bsp_clock_hz_calculate(extclk0_clock_srcs[clock_src]) / clock_div) : 0U; /* PRQA S 1520 # DP_MSR_R-17.2_UC1 */
            frequency = (FSP_PRIV_CLOCK_CLK_HSIOSC == extclk0_clock_srcs[clock_src]) ? (frequency / 20U) : frequency;
            break;
        }

        case FSP_PRIV_CLOCK_EXTCLK1O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_EXTCLK1S & R_SYS0_A_CLOCK_CKS_EXTCLK1S_EXTCLK1SACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CKD_EXTCLK1C & R_SYS0_A_CLOCK_CKD_EXTCLK1C_EXTCLK1DIV_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            frequency = (0U != clock_div) ? (r_bsp_clock_hz_calculate(extclk1_clock_srcs[clock_src]) / clock_div) : 0U;
            frequency = (FSP_PRIV_CLOCK_CLK_HSIOSC == extclk1_clock_srcs[clock_src]) ? (frequency / 20U) : frequency;
            break;
        }

        default:
        {
            frequency = 0U;
            break;
        }
    }

    return frequency;
}

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "bsp_mcu_standby_cyclicrun_driver.h"

#if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED)

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

 #define BSP_CYCLICRUN_OPBT_MOSC_FREQ_POS              (24U)        ///< Position of MOSC Frequency selection bit in OPBT 10
 #define BSP_CYCLICRUN_OPBT_MOSC_FREQ_MSK              (3U << 24)   ///< Mask of MOSC Frequency selection bit in OPBT 10

 #define BSP_CYCLICRUN_OPBT_CKDIVMD_POS                (30U)        ///< Position of CPU Frequency & CPU System Clock Setting bit in OPBT 11
 #define BSP_CYCLICRUN_OPBT_CKDIVMD_MSK                (3U << 30)   ///< Mask of CPU Frequency & CPU System Clock Setting bit in OPBT 11

 #define BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_4_VALUE    (2U)         ///< Clock source of RLIN3 is MOSC divided by 4
 #define BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_8_VALUE    (3U)         ///< Clock source of RLIN3 is MOSC divided by 8

 #define BSP_CYCLICRUN_CLOCK_ATAUJ_DIV_20_VALUE        (1U)         ///< ATAUJ clock source divided by 20

 #define BSP_CYCLICRUN_CLOCK_RTCA_DIV_16_VALUE         (0U)         ///< RTCA clock source divided by 16

 #define BSP_CYCLICRUN_CLOCK_AADC_DIV_20_VALUE         (1U)         ///< AADC clock source divided by 20

 #define BSP_CYCLICRUN_CLOCK_HVIOSC_VALUE              (16000000U)  ///< HVIOSC clock value
 #define BSP_CYCLICRUN_CLOCK_LSIOSC_VALUE              (240000U)    ///< LSIOSC clock value
 #define BSP_CYCLICRUN_CLOCK_HSIOSC_VALUE              (200000000U) ///< HSIOSC clock value
 #define BSP_CYCLICRUN_CLOCK_IOSC_VALUE                (200000000U) ///< IOSC clock value

 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_40MHZ           (40000000U)  ///< Main oscillator clock: 40 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_24MHZ           (24000000U)  ///< Main oscillator clock: 24 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_20MHZ           (20000000U)  ///< Main oscillator clock: 20 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_16MHZ           (16000000U)  ///< Main oscillator clock: 16 MHz

 #define BSP_CYCLICRUN_CLOCK_AADC_NO_DIV_VALUE         (1U)         ///< ADC AWO clock no division
 #define BSP_CYCLICRUN_CLOCK_AADC_DIV_2_VALUE          (2U)         ///< ADC AWO clock divided by 2

 #define BSP_CYCLICRUN_CLOCK_WDT_DIV_20_VALUE          (20U)        ///< WDT clock divided by 20
 #define BSP_CYCLICRUN_CLOCK_WDT_DIV_640_VALUE         (640U)       ///< WDT clock divided by 640

 #define BSP_CYCLICRUN_CLOCK_WDTA_NO_DIV_VALUE         (1U)         ///< WDT AWO clock no division
 #define BSP_CYCLICRUN_CLOCK_WDTA_DIV_128_VALUE        (128U)       ///< WDT AWO clock divided by 128

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/
static void bsp_cyclicrun_clock_freq_var_init(void);

static uint32_t bsp_cyclicrun_source_clock_hz_get(fsp_priv_clock_cyclicrun_t clock);     /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_cyclicrun_system_clock_hz_get(fsp_priv_clock_cyclicrun_t clock);     /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_cyclicrun_bus_clock_hz_get(fsp_priv_clock_cyclicrun_t clock);        /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_cyclicrun_peripheral_clock_hz_get(fsp_priv_clock_cyclicrun_t clock); /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */
static uint32_t bsp_cyclicrun_extclk_hz_get(fsp_priv_clock_cyclicrun_t clock);           /* PRQA S 4604 # DP_MSR_R-21.2_UC1 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* DATA SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA,
                            BSP_PRAGMA_SECTION_CRUN_CODE_DATA BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)

/***********************************************************************************************************************
 * CLOCK CONTROLLER
 **********************************************************************************************************************/

/* This array stores the clock frequency of each bus clock. This section of RAM should not be initialized by the C
 * runtime environment. This is initialized and used in R_BSP_CyclicRunInitClock, which is called before the C runtime
 * environment is initialized. */
volatile uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM] =            /* PRQA S 1514 # Advisory R-8.7 & Advisory R-8.9 */
{
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ,   ///< Initial value of CLK_MOSC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ, ///< Initial value of CLK_HSIOSC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ, ///< Initial value of CLK_CLK_LSIOSCCPU clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HVIOSC]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_HVIOSC_HZ, ///< Initial value of CLK_HVIOSC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL]     = 0U,                                    ///< Initial value of CLK_PLL clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLLO]    = 0U,                                    ///< Initial value of CLK_PLLO clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ,   ///< Initial value of CLK_IOSC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_HZ,    ///< Initial value of CLK_SYS clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ,    ///< Initial value of CLK_CPU clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ,   ///< Initial value of CLK_SBUS clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ,   ///< Initial value of CLK_HBUS clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ,   ///< Initial value of CLK_UHSB clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ,    ///< Initial value of CLK_HSB clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ,    ///< Initial value of CLK_LSB clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ,  ///< Initial value of CLK_RLIN clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_RCANOSC] = 0U,                                    ///< Initial value of CLK_RCANOSC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_ADC]     = 0U,                                    ///< Initial value of CLK_ADC clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ,   ///< Initial value of CLK_MSPI clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT]  = 0U,                                    ///< Initial value of CLK_ECMCNT clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDT]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_HZ,    ///< Initial value of CLK_WDT clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDT]    = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_HZ,   ///< Initial value of CLKA_WDT clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ,  ///< Initial value of CLKA_TAUJ clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ,  ///< Initial value of CLKA_RTCA clock
 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC] = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ,      ///< Initial value of CLKA_ADC clock
 #endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O] = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ,      ///< Initial value of EXTCLK0O clock
    [FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O] = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ,      ///< Initial value of EXTCLK1O clock
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS] = BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ,      ///< Initial value of CLKA_LPS clock
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/* ROM DATA SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA,
                            BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)

/***********************************************************************************************************************
 * STANDBY CONTROLLER
 **********************************************************************************************************************/

/** Array contains all Module standby informations that is available in Cyclic RUN Mode  */
const bsp_module_standby_info_t g_cyclicrun_msr_info_table[] =
{
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_MSPI] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_MSPI),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MSPI_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_RLIN3] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_RLIN3),      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE,           /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RLIN3_ENABLE                   /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCJ_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_ADCJ_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCJ_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCJ_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_RTCA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_RTCA),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RTCA_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_TAUJ_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_TAUJ_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_WDTB_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_WDTB_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_WDTB_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    }
};

/***********************************************************************************************************************
 * CLOCK
 **********************************************************************************************************************/

static const uint32_t g_main_os_clocks[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLKCTL_CLK_MOSC_16MHZ, BSP_CYCLICRUN_CLKCTL_CLK_MOSC_20MHZ, BSP_CYCLICRUN_CLKCTL_CLK_MOSC_24MHZ,
    BSP_CYCLICRUN_CLKCTL_CLK_MOSC_40MHZ
};

static const uint8_t g_clk_sbus_dividers[] = {6U, 6U, 4U, 4U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_hbus_dividers[] = {6U, 6U, 8U, 8U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_uhsb_dividers[] = {3U, 3U, 4U, 5U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_hsb_dividers[]  = {6U, 6U, 8U, 10U};    /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_lsb_dividers[]  = {12U, 12U, 16U, 20U}; /* PRQA S 3218 # Advisory R-8.9 */

static const fsp_priv_clock_cyclicrun_t g_mspi_clock_srcs[] =      /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB, FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const fsp_priv_clock_cyclicrun_t g_rlin3_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB, FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC
};
static const uint8_t g_aadc_clock_div[] =                      /* PRQA S 3218 # Advisory R-8.9 */
{BSP_CYCLICRUN_CLOCK_AADC_DIV_2_VALUE, BSP_CYCLICRUN_CLOCK_AADC_NO_DIV_VALUE};
static const uint16_t g_wdt_clock_div[] =                      /* PRQA S 3218 # Advisory R-8.9 */
{BSP_CYCLICRUN_CLOCK_WDT_DIV_20_VALUE, BSP_CYCLICRUN_CLOCK_WDT_DIV_640_VALUE};
static const uint16_t g_wdta_clock_div[] =                     /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLOCK_WDTA_NO_DIV_VALUE, BSP_CYCLICRUN_CLOCK_WDTA_DIV_128_VALUE
};
static const fsp_priv_clock_cyclicrun_t g_atauj_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB
};
static const fsp_priv_clock_cyclicrun_t g_rtca_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC
};
static const fsp_priv_clock_cyclicrun_t g_aadc_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};

static const fsp_priv_clock_cyclicrun_t g_extclk0_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    [0] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,                     /* PRQA S 3676 # DP_MSR_R-9.5_UC1 */
    [1] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,
    [2] = FSP_PRIV_CLOCK_CYCLICRUN_NUM,                          /* Setting prohibited, will return 0 value */
    [3] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC,
    [4] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,
    [5] = FSP_PRIV_CLOCK_CYCLICRUN_NUM,                          /* Setting prohibited, will return 0 value */
    [6] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    [7] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC
};
static const fsp_priv_clock_cyclicrun_t g_extclk1_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    [0] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,                     /* PRQA S 3676 # DP_MSR_R-9.5_UC1 */
    [1] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,
    [2] = FSP_PRIV_CLOCK_CYCLICRUN_NUM,                          /* Setting prohibited, will return 0 value */
    [3] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC,
    [4] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,
    [5] = FSP_PRIV_CLOCK_CYCLICRUN_NUM,                          /* Setting prohibited, will return 0 value */
    [6] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    [7] = FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/* BSS SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_BSS,
                            BSP_PRAGMA_SECTION_CRUN_CODE_BSS BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)
extern uint32_t g_cyclicrun_opbt_value[BSP_OPBT_NUMBER_OF_OPBT_REGS];

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_BSS, BSP_PRAGMA_SECTION_DEFAULT)

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/* TEXT SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_CRUN_CODE_TEXT)

/*******************************************************************************************************************//**
 * Initializes variable to store system clock frequencies.
 **********************************************************************************************************************/
static void bsp_cyclicrun_clock_freq_var_init (void)
{
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC] = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC] = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HVIOSC] = BSP_CFG_CLOCK_CYCLICRUN_CLK_HVIOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT] = BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDT]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDT]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ]  = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA]  = BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ;
 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC] = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ;
 #endif                                /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O] = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O] = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS] = BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 * This function can only be used in Cyclic RUN Mode.
 **********************************************************************************************************************/
void bsp_cyclicrun_clock_driver_init (void)
{
    /* Unlock clock controller write protection to allow register configuration */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE;                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm HS IntOSC is stable*/
    R_SYS0_A_CLOCK->HSOSCSTPM = BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->HSOSCS,                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) R_SYS0_A_CLOCK_HSOSCS_HSOSCSTAB_Msk);

    /* If Main OSC have been configured */
    R_SYS0_A_CLOCK->MOSCSTPM = BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE;   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm Main OSC is stopped */
    if (0U == R_SYS0_A_CLOCK->MOSCS)                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Enable MOSC */
        R_SYS0_A_CLOCK->MOSCE_b.MOSCENTRG =                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                                            (uint32_t) R_SYS0_A_CLOCK_MOSCE_MOSCENTRG_Msk;
    }

    /* Confirm that the Main OSC have been stable (MOSCS.MOSCEN = 1 & MOSCS.MOSCSTAB = 1) */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS, /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCSTAB_Msk));

    /* CLK_RLIN3 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC;                                /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS,                                                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC;                                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS,                                                        /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC);

    /* CLKA_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_AWDTC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC;                              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AWDTS,                                                     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKSC_ATAUJC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKSC_ARTCAC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS,                                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC);

 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKSC_AADCC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS,                                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC);

    R_SYS0_A_CLOCK->CLKD_AADCC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_DIV;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_AADCS, (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_DIV); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
 #endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT0C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, (uint32_t) (R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT0C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S_b.FOUT0SYNC, (uint32_t) 1U);
    R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S,
                               ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0CLKACT_Msk));
 #endif                                /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK1O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT1C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT1C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT1S, (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S_b.FOUT1SYNC, (uint32_t) 1U);
    R_SYS0_A_CLOCK->CLKD_FOUT1C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S,
                               ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1CLKACT_Msk));
 #endif                                                                         /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */
    /* CLK_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_WDTC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_WDTS,                       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC);

    /* Re-lock clock controller write protection */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    bsp_cyclicrun_clock_freq_var_init();
    __SYNCP();                                                                  /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Get the specific clock's frequency.
 **********************************************************************************************************************/
uint32_t r_bsp_cyclicrun_clock_hz_calculate (fsp_priv_clock_cyclicrun_t clock)  /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLLO:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HVIOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC:
        {
            frequency = bsp_cyclicrun_source_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS:
        {
            frequency = bsp_cyclicrun_system_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB:
        {
            frequency = bsp_cyclicrun_bus_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ADC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RCANOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDT:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDT:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA:
 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC:
 #endif                                /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS:
        {
            /* PRQA S 1520 ++ # DP_MSR_R-17.2_UC1 */
            frequency = bsp_cyclicrun_peripheral_clock_hz_get(clock);

            /* PRQA S 1520 -- */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O:
        case FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O:
        {
            frequency = bsp_cyclicrun_extclk_hz_get(clock);
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
static uint32_t bsp_cyclicrun_source_clock_hz_get (fsp_priv_clock_cyclicrun_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    volatile uint32_t frequency = 0U;
    uint8_t           opbt_mosc_freq_value;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HVIOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_HVIOSC_VALUE; /* HV IntOSC clock = 16 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_LSIOSC_VALUE; /* LS IntOSC clock = 240 kHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_HSIOSC_VALUE; /* HS IntOSC clock = 200 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_IOSC_VALUE; /* Internal OSC clock = HS IntOSC clock = 200 MHz,
                                                         * Internal OSC clock = LS IntOSC clock = 240 kHz only when HS IntOSC clock
                                                         * stops in chip standby mode (Stop / DeepStop mode)*/
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC:
        {
            opbt_mosc_freq_value =
                (uint8_t) ((g_cyclicrun_opbt_value[10U + BSP_OPBT_REGISTER_OFFSET] &
                            BSP_CYCLICRUN_OPBT_MOSC_FREQ_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_MOSC_FREQ_POS);
            frequency = g_main_os_clocks[opbt_mosc_freq_value];  /* Main OSC clock = 16 MHz / 20 MHz / 24 MHz / 40 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL:
        {
            frequency = 0U;            /* PLL clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLLO:
        {
            frequency = 0U;            /* PLLO clock clock doesn't operate in Cyclic Run Mode */
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
static uint32_t bsp_cyclicrun_system_clock_hz_get (fsp_priv_clock_cyclicrun_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS:
        {
            /* Check the source clock of System clock for SSCG Domain */
            if (1U == R_SYS0_CLOCK->CKSC_CPUS) /* CLK_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_cyclicrun_source_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC);
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
static uint32_t bsp_cyclicrun_bus_clock_hz_get (fsp_priv_clock_cyclicrun_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;

    uint8_t opbt_ckdivmd_value;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= 2U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= g_clk_sbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= g_clk_hbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= g_clk_uhsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= g_clk_hsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS);
            frequency /= g_clk_lsb_dividers[opbt_ckdivmd_value];
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
static uint32_t bsp_cyclicrun_peripheral_clock_hz_get (fsp_priv_clock_cyclicrun_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;
    uint8_t  clock_src;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_MSPIS & R_SYS0_CLOCK_CKS_MSPIS_MSPISACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_mspi_clock_srcs[clock_src]);          /* PRQA S 1520 # DP_MSR_R-17.2_UC1 */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_RLINS & R_SYS0_CLOCK_CKS_RLINS_RLINSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_rlin3_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_4_VALUE == clock_src)
            {
                frequency /= 4U;
            }
            else if (BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_8_VALUE == clock_src)
            {
                frequency /= 8U;
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ADC:
        {
            frequency = 0U;            /* ADC clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDT:
        {
            /* Only WDTB0 can operate in Cyclic Run Mode */
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_WDTS & R_SYS0_A_CLOCK_CKSC_WDTS_WDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC) /
                        g_wdt_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RCANOSC:
        {
            frequency = 0U;            /* RCANOSC clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT:
        {
            frequency = 0U;            /* ECMCNT clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDT:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_AWDTS & R_SYS0_A_CLOCK_CKSC_AWDTS_AWDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC) /
                        g_wdta_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_ATAUJS & R_SYS0_A_CLOCK_CKSC_ATAUJS_ATAUJSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_atauj_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_ATAUJ_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_ARTCAS & R_SYS0_A_CLOCK_CKSC_ARTCAS_ARTCASACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_rtca_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_RTCA_DIV_16_VALUE == clock_src)
            {
                frequency /= 16U;
            }

            break;
        }

 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC:
        {
            uint8_t aadc_div = (uint8_t) (R_SYS0_A_CLOCK->CLKD_AADCC & R_SYS0_A_CLOCK_CLKD_AADCC_AADCDCSID_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_AADCS & R_SYS0_A_CLOCK_CKSC_AADCS_AADCSACT_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_aadc_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_AADC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            frequency /= g_aadc_clock_div[aadc_div];

            break;
        }
 #endif                                /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS:
        {
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC) / 20U;
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
static uint32_t bsp_cyclicrun_extclk_hz_get (fsp_priv_clock_cyclicrun_t clock) /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;
    uint8_t  clock_src;
    uint16_t clock_div;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_FOUT0S & R_SYS0_A_CLOCK_CKSC_FOUT0S_FOUT0SACT_Msk);             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CLKD_FOUT0C & R_SYS0_A_CLOCK_CLKD_FOUT0C_FOUT0DIV_Msk);             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            frequency =
                (0U !=
                 clock_div) ? r_bsp_cyclicrun_clock_hz_calculate(g_extclk0_clock_srcs[clock_src]) / clock_div : 0U;     /* PRQA S 3396 # Advisory R-12.1 */
            frequency =
                (FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC == g_extclk0_clock_srcs[clock_src]) ? frequency / 20U : frequency; /* PRQA S 3396 # Advisory R-12.1 */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_FOUT1S & R_SYS0_A_CLOCK_CKSC_FOUT1S_FOUT1SACT_Msk);             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CLKD_FOUT1C & R_SYS0_A_CLOCK_CLKD_FOUT1C_FOUT1DIV_Msk);             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            frequency =
                (0U !=
                 clock_div) ? r_bsp_cyclicrun_clock_hz_calculate(g_extclk1_clock_srcs[clock_src]) / clock_div : 0U;     /* PRQA S 3396 # Advisory R-12.1 */
            frequency =
                (FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC == g_extclk1_clock_srcs[clock_src]) ? frequency / 20U : frequency; /* PRQA S 3396 # Advisory R-12.1 */
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

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_DEFAULT)

#endif                                 /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */ /* PRQA S 2053 # Advisory D-4.4 */

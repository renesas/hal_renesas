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

 #define BSP_CYCLICRUN_OPBT_MOSC_FREQ_POS                 (24U)        ///< Position of MOSC Frequency selection bit in OPBT 10
 #define BSP_CYCLICRUN_OPBT_MOSC_FREQ_MSK                 (7U << 24)   ///< Mask of MOSC Frequency selection bit in OPBT 10

 #define BSP_CYCLICRUN_OPBT_CKDIVMD_POS                   (29U)        ///< Position of CPU Frequency & CPU System Clock Setting bit in OPBT 11
 #define BSP_CYCLICRUN_OPBT_CKDIVMD_MSK                   (7U << 29)   ///< Mask of CPU Frequency & CPU System Clock Setting bit in OPBT 11

 #define BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_4_VALUE       (3U)         ///< Clock source of RLIN3 is MOSC divided by 4
 #define BSP_CYCLICRUN_CLOCK_RLIN3_MOSC_DIV_8_VALUE       (4U)         ///< Clock source of RLIN3 is MOSC divided by 8

 #define BSP_CYCLICRUN_CLOCK_ATAUJ_HSIOSC_DIV_20_VALUE    (2U)         ///< Clock source of TAUJ AWO is HSIOSC divided by 20

 #define BSP_CYCLICRUN_CLOCK_AADC_HSIOSC_DIV_20_VALUE     (1U)         ///< Clock source of ADC AWO is HSIOSC divided by 20

 #define BSP_CYCLICRUN_CLOCK_ARTCA_MOSC_DIV_16_VALUE      (2U)         ///< Clock source of RTCA AWO is MOSC divided by 16

 #define BSP_CYCLICRUN_CLOCK_LSIOSC_VALUE                 (32768U)     ///< LSIOSC clock value
 #define BSP_CYCLICRUN_CLOCK_HSIOSC_VALUE                 (200000000U) ///< HSIOSC clock value
 #define BSP_CYCLICRUN_CLOCK_IOSC_VALUE                   (200000000U) ///< IOSC clock value

 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_24MHZ              (24000000U)  ///< MOSC frequency: 24 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_20MHZ              (20000000U)  ///< MOSC frequency: 20 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_16MHZ              (16000000U)  ///< MOSC frequency: 16 MHz
 #define BSP_CYCLICRUN_CLKCTL_CLK_MOSC_8MHZ               (8000000U)   ///< MOSC frequency: 8 MHz

 #define BSP_CYCLICRUN_CLOCK_WDTB_NO_DIV_VALUE            (1U)         ///< WDTB clock no division
 #define BSP_CYCLICRUN_CLOCK_WDTB_DIV_20_VALUE            (20U)        ///< WDTB clock divided by 20
 #define BSP_CYCLICRUN_CLOCK_WDTB_DIV_640_VALUE           (640U)       ///< WDTB clock divided by 640
 #define BSP_CYCLICRUN_CLOCK_WDTB_DIV_800_VALUE           (800U)       ///< WDTB clock divided by 800

 #define BSP_CYCLICRUN_CLOCK_WDTA_NO_DIV_VALUE            (1U)         ///< WDTA clock no division
 #define BSP_CYCLICRUN_CLOCK_WDTA_DIV_128_VALUE           (128U)       ///< WDTA clock divided by 128

 #define BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE    (1U)         ///< SYS_IOSC clock no division
 #define BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_DIV_2_VALUE     (2U)         ///< SYS_IOSC clock divided by 2
 #define BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_DIV_10_VALUE    (10U)        ///< SYS_IOSC clock divided by 10

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
volatile uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM] =                 /* PRQA S 1514 # Advisory R-8.7 & Advisory R-8.9 */
{
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ,    /* Initial value of CLK_LSIOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ,    /* Initial value of CLK_HSIOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ,      /* Initial value of CLK_IOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_IOSC_HZ,  /* Initial value of CLK_SYS_IOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG]       = BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_HZ,       /* Initial value of CLK_EMG clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ,      /* Initial value of CLK_MOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_SOSC_HZ,      /* Initial value of CLK_SOSC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL]       = 0U,                                       /* Initial value of CLK_PLL clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SSCG]      = 0U,                                       /* Initial value of CLK_SSCG clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN] = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_CLEAN_HZ, /* Initial value of CLK_SYS_CLEAN clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_SSCG_HZ,  /* Initial value of CLK_SYS_SSCG clock */

    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ,           /* Initial value of CLK_CPU clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ,          /* Initial value of CLK_SBUS clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ,          /* Initial value of CLK_HBUS clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ,          /* Initial value of CLK_UHSB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ,           /* Initial value of CLK_HSB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ,           /* Initial value of CLK_LSB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_SBUS] = BSP_CFG_CLOCK_CYCLICRUN_CLKC_SBUS_HZ,         /* Initial value of CLKC_SBUS clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS] = BSP_CFG_CLOCK_CYCLICRUN_CLKC_HBUS_HZ,         /* Initial value of CLKC_HBUS clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_UHSB] = BSP_CFG_CLOCK_CYCLICRUN_CLKC_UHSB_HZ,         /* Initial value of CLKC_UHSB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB]  = BSP_CFG_CLOCK_CYCLICRUN_CLKC_HSB_HZ,          /* Initial value of CLKC_HSB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB]  = BSP_CFG_CLOCK_CYCLICRUN_CLKC_LSB_HZ,          /* Initial value of CLKC_LSB clock */

    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_FLI]     = 0U,                                         /* Initial value of CLK_FLI clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_FACI]    = 0U,                                         /* Initial value of CLK_FACI clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS]    = BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ,        /* Initial value of CLKA_LPS clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDTBA]  = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_HZ,      /* Initial value of CLKA_WDTBA clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ,       /* Initial value of CLKA_TAUJ clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC]    = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ,        /* Initial value of CLKA_ADC clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ,       /* Initial value of CLKA_RTCA clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O]    = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ,        /* Initial value of EXTCLK0O clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O]    = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ,        /* Initial value of EXTCLK1O clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ,        /* Initial value of CLK_MSPI clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN3]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ,       /* Initial value of CLK_RLIN3 clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN323] = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_HZ,     /* Initial value of CLK_RLIN323 clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDTB]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_HZ,        /* Initial value of CLK_WDTB clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ,      /* Initial value of CLK_ECMCNT clock */

    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_C]    = 0U,                                      /* Initial value of CLK_CANFD_C clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_XIN]  = 0U,                                      /* Initial value of CLK_CANFD_XIN clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH0] = 0U,                                      /* Initial value of CLKC_CANXL_CH0 clock */
 #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
    [FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH1] = 0U,                                      /* Initial value of CLKC_CANXL_CH1 clock */
 #endif
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_GTM]  = 0U,                                            /* Initial value of CLK_GTM clock */
    [FSP_PRIV_CLOCK_CYCLICRUN_CLK_SWDT] = 0U,                                            /* Initial value of CLK_SWDT clock */
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
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCK_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_ADCK_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
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

static const uint8_t g_clock_sys_dividers[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE, BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_DIV_2_VALUE,
    BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_DIV_10_VALUE, BSP_CYCLICRUN_CLOCK_CLK_SYS_IOSC_NO_DIV_VALUE
};
static const uint32_t g_main_os_clocks[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLKCTL_CLK_MOSC_8MHZ,  BSP_CYCLICRUN_CLKCTL_CLK_MOSC_16MHZ,  BSP_CYCLICRUN_CLKCTL_CLK_MOSC_20MHZ,
    BSP_CYCLICRUN_CLKCTL_CLK_MOSC_24MHZ, 0U,                                   0U,                                  0U,
    0U
};
static const uint32_t g_sosc_clocks[] = {0U, 32768U};                              /* PRQA S 3218 # Advisory R-8.9 */

static const uint8_t g_clk_sbus_dividers[] = {4U, 4U, 4U, 4U, 4U, 6U, 4U, 4U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_hbus_dividers[] = {4U, 4U, 4U, 4U, 8U, 6U, 8U, 8U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_uhsb_dividers[] = {2U, 2U, 2U, 2U, 4U, 3U, 4U, 4U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_hsb_dividers[]  = {4U, 4U, 4U, 4U, 8U, 6U, 8U, 8U};     /* PRQA S 3218 # Advisory R-8.9 */
static const uint8_t g_clk_lsb_dividers[]  = {8U, 8U, 8U, 8U, 16U, 12U, 16U, 16U}; /* PRQA S 3218 # Advisory R-8.9 */

static const fsp_priv_clock_cyclicrun_t g_mspi_clock_srcs[] =                      /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG, FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const fsp_priv_clock_cyclicrun_t g_rlin3_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG,  FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB,  FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_NUM,       FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const fsp_priv_clock_cyclicrun_t g_rlin3_23_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG,  FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS,  FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_NUM,        FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const uint16_t g_wdtb_clock_div[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLOCK_WDTB_DIV_20_VALUE,  BSP_CYCLICRUN_CLOCK_WDTB_DIV_640_VALUE,
    BSP_CYCLICRUN_CLOCK_WDTB_DIV_800_VALUE, BSP_CYCLICRUN_CLOCK_WDTB_NO_DIV_VALUE
};
static const uint16_t g_wdta_clock_div[] =                        /* PRQA S 3218 # Advisory R-8.9 */
{
    BSP_CYCLICRUN_CLOCK_WDTA_NO_DIV_VALUE, BSP_CYCLICRUN_CLOCK_WDTA_DIV_128_VALUE
};
static const fsp_priv_clock_cyclicrun_t g_tauj_awo_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG,  FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC,  FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB, FSP_PRIV_CLOCK_CYCLICRUN_NUM,         FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const fsp_priv_clock_cyclicrun_t g_adc_awo_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB
};
static const fsp_priv_clock_cyclicrun_t g_rtca_awo_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG, FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC
};

static const fsp_priv_clock_cyclicrun_t g_extclk0_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,  FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB,   FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
};
static const fsp_priv_clock_cyclicrun_t g_extclk1_clock_srcs[] = /* PRQA S 3218 # Advisory R-8.9 */
{
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC, FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC,
    FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB,  FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB,   FSP_PRIV_CLOCK_CYCLICRUN_NUM,
    FSP_PRIV_CLOCK_CYCLICRUN_NUM
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

/***********************************************************************************************************************
 * CLOCK CONTROLLER
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes variable to store system clock frequencies.in Cyclic RUN Mode
 **********************************************************************************************************************/
static void bsp_cyclicrun_clock_freq_var_init (void)
{
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_IOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG]       = BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_SOSC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN] = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_CLEAN_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG]  = BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_SSCG_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU]       = BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB]       = BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB]       = BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKC_SBUS]     = BSP_CFG_CLOCK_CYCLICRUN_CLKC_SBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS]     = BSP_CFG_CLOCK_CYCLICRUN_CLKC_HBUS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKC_UHSB]     = BSP_CFG_CLOCK_CYCLICRUN_CLKC_UHSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB]      = BSP_CFG_CLOCK_CYCLICRUN_CLKC_HSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB]      = BSP_CFG_CLOCK_CYCLICRUN_CLKC_LSB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS]      = BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDTBA]    = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ]     = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC]      = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O]      = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O]      = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN3]     = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN323]   = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDTB]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT]    = BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 **********************************************************************************************************************/
void bsp_cyclicrun_clock_driver_init (void)
{
    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE;                                     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm HS IntOSC is stable*/
    R_SYS0_A_CLOCK->HSOSCSTPM = BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE;                     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->HSOSCS,                                     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) R_SYS0_A_CLOCK_HSOSCS_HSOSCSTAB_Msk);

    /* If Main OSC have been configured */
    R_SYS0_A_CLOCK->MOSCSTPM = BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE;                       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm Main OSC is stopped */
    if (0U == R_SYS0_A_CLOCK->MOSCS)                                                       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        /* Enable MOSC */
        R_SYS0_A_CLOCK->MOSCE_b.MOSCENTRG = (uint32_t) R_SYS0_A_CLOCK_MOSCE_MOSCENTRG_Msk; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Confirm that the Main OSC have been stable (MOSCS.MOSCEN = 1 & MOSCS.MOSCSTAB = 1) */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS,                                      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_MOSCS_MOSCSTAB_Msk));

 #if (BSP_CFG_CLOCK_CYCLICRUN_SUBOSC_HZ > 0)                                               /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    /* Confirm Sub OSC is stopped */
    if (0U == R_SYS0_A_CLOCK->SOSCS)                                                       /* PRQA S 0306 # Advisory R-11.4 */
    {
        R_SYS0_A_CLOCK->SOSCE = (uint32_t) R_SYS0_A_CLOCK_SOSCE_SOSCENTRG_Msk;             /* PRQA S 0306 # Advisory R-11.4 */

        /* Confirm that the Sub OSC has been stable
         * (SOSCS.SOSCEN = 1 & SOSCS.SOSCSTAB = 1)*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x03U);                          /* PRQA S 0306 # Advisory R-11.4 */
    }

 #else

    /* Confirm that the Sub OSC is stable */
    if (1U == R_SYS0_A_CLOCK->SOSCS_b.SOSCSTAB)                                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    {
        R_SYS0_A_CLOCK->SOSCE = (uint32_t) R_SYS0_A_CLOCK_SOSCE_SOSCDISTRG_Msk; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that the Sub OSC has been stopped (SOSCS.SOSCEN = 0 & SOSCS.SOSCSTAB = 0).*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x00U);               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }
 #endif                                                                         /* End of (BSP_CFG_CLOCK_CYCLICRUN_SUBOSC_HZ > 0) */ /* PRQA S 2053 # Advisory D-4.4 */

    /* CLK_RLIN3 divider setting and CLK_RLIN3_CH23 divider setting */
    R_SYS0_CLOCK->CKS_RLINC =                                                   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                              (((uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS,                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_SRC << 16) |
                               ((uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC));

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC;     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS,                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC);

    /* CLKA_WDTBA selector setting */
    R_SYS0_A_CLOCK->CKS_AWDTC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_SRC; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AWDTS,                          /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_SRC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKS_ATAUJC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS,                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC);

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKS_ARTCAC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS,                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC);

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKS_AADCC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC;   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS,                          /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC);

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0U;                                              /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK0C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC; /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S_b.EXTCLK0SYNC,          /* PRQA S 0306 # Advisory R-11.4 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV; /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0CLKACT_Msk));
 #endif                                                                             /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0U;                                              /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK1C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC; /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S_b.EXTCLK1SYNC,          /* PRQA S 0306 # Advisory R-11.4 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV; /* PRQA S 0306 # Advisory R-11.4 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S,                        /* PRQA S 0306 # Advisory R-11.4 */
                               ((uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1CLKACT_Msk));
 #endif                                                                             /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

    /* CLK_WDTB selector setting */
    R_SYS0_A_CLOCK->CKS_WDTC = (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_SRC;     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_WDTS,                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_SRC);

    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    bsp_cyclicrun_clock_freq_var_init();
    __SYNCP();                                                                      /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Get the specific clock's frequency.
 **********************************************************************************************************************/
uint32_t r_bsp_cyclicrun_clock_hz_calculate (fsp_priv_clock_cyclicrun_t clock)      /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    uint32_t frequency = 0U;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SSCG:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_IOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG:
        {
            frequency = bsp_cyclicrun_source_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN:
        {
            frequency = bsp_cyclicrun_system_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_SBUS:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_UHSB:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB:
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

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN3:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN323:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_GTM:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDTB:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_C:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_XIN:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH0:
 #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH1:
 #endif                                /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SWDT:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_FLI:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_FACI:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDTBA:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA:
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT:
        {
            frequency = bsp_cyclicrun_peripheral_clock_hz_get(clock);
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
static uint32_t bsp_cyclicrun_source_clock_hz_get (fsp_priv_clock_cyclicrun_t clock)                        /* PRQA S 4603 # DP_MSR_R-21.2_UC1 */
{
    volatile uint32_t frequency         = 0U;
    uint8_t           clock_sys_divider = g_clock_sys_dividers[R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID]; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    uint8_t           opbt_mosc_freq_value;
    uint8_t           sosc_enable = 0U;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_LSIOSC_VALUE; /* LS IntOSC clock = 32.768 kHz */
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
                                                         * Internal OSC clock = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                                         * stops in chip standby mode (Stop / DeepStop mode)*/
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC:
        {
            frequency = BSP_CYCLICRUN_CLOCK_HSIOSC_VALUE / clock_sys_divider; /* Internal OSC clock for CLK_SYS Domain = HS IntOSC clock (/1, /2, /10) = 200/100/20 MHz
                                                                               * Internal OSC clock for CLK_SYS Domain = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                                                               * stops in chip standby mode (Stop / DeepStop mode) */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG:
        {
            frequency = BSP_CYCLICRUN_CLOCK_HSIOSC_VALUE / 20U; /* Emergency clock = HS IntOSC clock / 20 = 10 MHz
                                                                 * Emergency clock = LS IntOSC clock = 32.768 kHz only when HS IntOSC clock
                                                                 * stops in chip standby mode (Stop / DeepStop mode) */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_MOSC:
        {
            opbt_mosc_freq_value =
                (uint8_t) ((g_cyclicrun_opbt_value[10U + BSP_OPBT_REGISTER_OFFSET] &
                            BSP_CYCLICRUN_OPBT_MOSC_FREQ_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_MOSC_FREQ_POS);
            frequency = g_main_os_clocks[opbt_mosc_freq_value];  /* Main OSC clock = 8 MHz / 16 MHz / 20 MHz / 24 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SOSC:
        {
            sosc_enable = R_SYS0_A_CLOCK->SOSCS & R_SYS0_A_CLOCK_SOSCS_SOSCEN_Msk; /* PRQA S 0306, 0662, 4461 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, DP_MSR_R-10.3_UC1 */
            frequency   = g_sosc_clocks[sosc_enable];                              /* Sub OSC clock = 0 Hz / 32.768 kHz */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_PLL:
        {
            frequency = 0U;            /* PLL clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SSCG:
        {
            frequency = 0U;            /* SSCG clock clock doesn't operate in Cyclic Run Mode */
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
        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG:
        {
            /* Check the source clock of System clock for SSCG Domain */
            if (1U == R_SYS0_CLOCK->CKS_SSCGS) /* CLK_SYS_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_cyclicrun_source_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC);
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN:
        {
            /* Check the source clock of System clock for Clean Domain */
            if (1U == R_SYS0_CLOCK->CKS_CLEANS) /* CLK_SYS_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_cyclicrun_source_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_IOSC);
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
    uint8_t  opbt_ckdivmd_value;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_SBUS:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN);
            frequency /= 4U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HBUS:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN);
            frequency /= 8U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_UHSB:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN);
            frequency /= 5U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_HSB:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN);
            frequency /= 10U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_LSB:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_CLEAN);
            frequency /= 20U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU:
        {
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
            frequency /= 2U;
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
            frequency /= g_clk_sbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
            frequency /= g_clk_hbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_UHSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
            frequency /= g_clk_uhsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
            frequency /= g_clk_hsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((g_cyclicrun_opbt_value[11U + BSP_OPBT_REGISTER_OFFSET] & BSP_CYCLICRUN_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499 # DP_MSR_R-12.2_UC1 */
                           BSP_CYCLICRUN_OPBT_CKDIVMD_POS);
            frequency  = bsp_cyclicrun_system_clock_hz_get(FSP_PRIV_CLOCK_CYCLICRUN_CLK_SYS_SSCG);
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
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_mspi_clock_srcs[clock_src]);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN3:
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

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_RLIN323:
        {
            clock_src =
                (uint8_t) ((R_SYS0_CLOCK->CKS_RLINS & R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Msk) >> /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                           R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos);
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_rlin3_23_clock_srcs[clock_src]);

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

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_GTM:
        {
            frequency = 0U;            /* GTM clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_WDTB:
        {
            /* Only WDTB0 can operate in Cyclic Run Mode */
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_WDTS & R_SYS0_A_CLOCK_CKS_WDTS_WDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC) /
                        g_wdtb_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_C:
        {
            frequency = 0U;            /* CANFD_C clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_CANFD_XIN:
        {
            frequency = 0U;            /* CANFD_XIN clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH0:
        {
            frequency = 0U;            /* CANXL_CH0 clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

 #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CYCLICRUN_CLKC_CANXL_CH1:
        {
            frequency = 0U;            /* CANXL_CH1 clock clock doesn't operate in Cyclic Run Mode */
            break;
        }
 #endif                                /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_SWDT:
        {
            frequency = 0U;            /* CLK_SWDT clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_FLI:
        {
            frequency = 0U;            /* CLK_FLI clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_FACI:
        {
            frequency = 0U;            /* CLK_FACI clock clock doesn't operate in Cyclic Run Mode */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS:
        {
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG);
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_WDTBA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_AWDTS & R_SYS0_A_CLOCK_CKS_AWDTS_AWDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_LSIOSC) /
                        g_wdta_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_TAUJ:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_ATAUJS & R_SYS0_A_CLOCK_CKS_ATAUJS_ATAUJSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_tauj_awo_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_ATAUJ_HSIOSC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_AADCS & R_SYS0_A_CLOCK_CKS_AADCS_AADCSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_adc_awo_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_AADC_HSIOSC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLKA_RTCA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_ARTCAS & R_SYS0_A_CLOCK_CKS_ARTCAS_ARTCASACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_cyclicrun_clock_hz_calculate(g_rtca_awo_clock_srcs[clock_src]);

            if (BSP_CYCLICRUN_CLOCK_ARTCA_MOSC_DIV_16_VALUE == clock_src)
            {
                frequency /= 16U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_CLK_ECMCNT:
        {
            frequency = r_bsp_cyclicrun_clock_hz_calculate(FSP_PRIV_CLOCK_CYCLICRUN_CLK_EMG);
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
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_EXTCLK0S & R_SYS0_A_CLOCK_CKS_EXTCLK0S_EXTCLK0SACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CKD_EXTCLK0C & R_SYS0_A_CLOCK_CKD_EXTCLK0C_EXTCLK0DIV_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            frequency =
                (0U !=

                 /* PRQA S 1520 ++ # DP_MSR_R-17.2_UC1 */
                 clock_div) ? r_bsp_cyclicrun_clock_hz_calculate(g_extclk0_clock_srcs[clock_src]) / clock_div : 0U;     /* PRQA S 3396 # Advisory R-12.1 */
            /* PRQA S 1520 -- */
            frequency =
                (FSP_PRIV_CLOCK_CYCLICRUN_CLK_HSIOSC == g_extclk0_clock_srcs[clock_src]) ? frequency / 20U : frequency; /* PRQA S 3396 # Advisory R-12.1 */
            break;
        }

        case FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKS_EXTCLK1S & R_SYS0_A_CLOCK_CKS_EXTCLK1S_EXTCLK1SACT_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CKD_EXTCLK1C & R_SYS0_A_CLOCK_CKD_EXTCLK1C_EXTCLK1DIV_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

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

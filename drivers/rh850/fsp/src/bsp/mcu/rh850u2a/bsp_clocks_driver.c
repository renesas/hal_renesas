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

#define BSP_OPBT_MOSC_FREQ_POS              (24U)                          ///< Position of MOSC Frequency selection bit in OPBT 10
#define BSP_OPBT_MOSC_FREQ_MSK              (3U << BSP_OPBT_MOSC_FREQ_POS) ///< Mask of MOSC Frequency selection bit in OPBT 10

#define BSP_OPBT_CKDIVMD_POS                (30U)                          ///< Position of CPU Frequency & CPU System Clock Setting bit in OPBT 11
#define BSP_OPBT_CKDIVMD_MSK                (3U << BSP_OPBT_CKDIVMD_POS)   ///< Mask of CPU Frequency & CPU System Clock Setting bit in OPBT 11

#define BSP_CLOCK_RLIN3_MOSC_DIV_4_VALUE    (2U)                           ///< Clock source of RLIN3 is MOSC divided by 4
#define BSP_CLOCK_RLIN3_MOSC_DIV_8_VALUE    (3U)                           ///< Clock source of RLIN3 is MOSC divided by 8

#define BSP_CLOCK_CANFD_DIV_2_VALUE         (2U)                           ///< CANFD clock source divided by 2
#define BSP_CLOCK_CANFD_DIV_4_VALUE         (3U)                           ///< CANFD clock source divided by 4

#define BSP_CLOCK_ATAUJ_DIV_20_VALUE        (1U)                           ///< ATAUJ clock source divided by 20

#define BSP_CLOCK_RTCA_DIV_16_VALUE         (0U)                           ///< RTCA clock source divided by 16

#define BSP_CLOCK_AADC_DIV_20_VALUE         (1U)                           ///< AADC clock source divided by 20

#define BSP_CLOCK_HVIOSC_VALUE              (16000000U)                    ///< HVIOSC clock value
#define BSP_CLOCK_LSIOSC_VALUE              (240000U)                      ///< LSIOSC clock value
#define BSP_CLOCK_HSIOSC_VALUE              (200000000U)                   ///< HSIOSC clock value
#define BSP_CLOCK_IOSC_VALUE                (200000000U)                   ///< IOSC clock value

#define BSP_CLOCK_ADC_NO_DIV_VALUE          (1U)                           ///< ADC clock no division
#define BSP_CLOCK_ADC_DIV_2_VALUE           (2U)                           ///< ADC clock divided by 2

#define BSP_CLOCK_AADC_NO_DIV_VALUE         (1U)                           ///< ADC AWO clock no division
#define BSP_CLOCK_AADC_DIV_2_VALUE          (2U)                           ///< ADC AWO clock divided by 2

#define BSP_CLOCK_WDT_DIV_20_VALUE          (20U)                          ///< WDT clock divided by 20
#define BSP_CLOCK_WDT_DIV_640_VALUE         (640U)                         ///< WDT clock divided by 640

#define BSP_CLOCK_WDTA_NO_DIV_VALUE         (1U)                           ///< WDT AWO clock no division
#define BSP_CLOCK_WDTA_DIV_128_VALUE        (128U)                         ///< WDT AWO clock divided by 128

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/
static void bsp_clock_freq_var_init(void);
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

/* PRQA S 1510 ++ # DP_MSR_R-1.3_UC1 */
uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM] =                      /* PRQA S 1514 # Advisory R-8.7 & Advisory R-8.9 */
{
    [FSP_PRIV_CLOCK_CLK_MOSC]    = BSP_CFG_CLOCK_CLK_MOSC_HZ,    ///< Initial value of CLK_MOSC clock
    [FSP_PRIV_CLOCK_CLK_HSIOSC]  = BSP_CFG_CLOCK_CLK_HSIOSC_HZ,  ///< Initial value of CLK_HSIOSC clock
    [FSP_PRIV_CLOCK_CLK_LSIOSC]  = BSP_CFG_CLOCK_CLK_LSIOSC_HZ,  ///< Initial value of CLK_LSIOSC clock
    [FSP_PRIV_CLOCK_CLK_HVIOSC]  = BSP_CFG_CLOCK_CLK_HVIOSC_HZ,  ///< Initial value of CLK_HVIOSC clock
    [FSP_PRIV_CLOCK_CLK_PLL]     = BSP_CFG_CLOCK_CLK_PLL_HZ,     ///< Initial value of CLK_PLL clock
    [FSP_PRIV_CLOCK_CLK_PLLO]    = BSP_CFG_CLOCK_CLK_PLLO_HZ,    ///< Initial value of CLK_PLLO clock
    [FSP_PRIV_CLOCK_CLK_IOSC]    = BSP_CFG_CLOCK_CLK_IOSC_HZ,    ///< Initial value of CLK_IOSC clock
    [FSP_PRIV_CLOCK_CLK_SYS]     = BSP_CFG_CLOCK_CLK_SYS_HZ,     ///< Initial value of CLK_SYS clock
    [FSP_PRIV_CLOCK_CLK_CPU]     = BSP_CFG_CLOCK_CLK_CPU_HZ,     ///< Initial value of CLK_CPU clock
    [FSP_PRIV_CLOCK_CLK_SBUS]    = BSP_CFG_CLOCK_CLK_SBUS_HZ,    ///< Initial value of CLK_SBUS clock
    [FSP_PRIV_CLOCK_CLK_HBUS]    = BSP_CFG_CLOCK_CLK_HBUS_HZ,    ///< Initial value of CLK_HBUS clock
    [FSP_PRIV_CLOCK_CLK_UHSB]    = BSP_CFG_CLOCK_CLK_UHSB_HZ,    ///< Initial value of CLK_UHSB clock
    [FSP_PRIV_CLOCK_CLK_HSB]     = BSP_CFG_CLOCK_CLK_HSB_HZ,     ///< Initial value of CLK_HSB clock
    [FSP_PRIV_CLOCK_CLK_LSB]     = BSP_CFG_CLOCK_CLK_LSB_HZ,     ///< Initial value of CLK_LSB clock
    [FSP_PRIV_CLOCK_CLK_RLIN]    = BSP_CFG_CLOCK_CLK_RLIN3_HZ,   ///< Initial value of CLK_RLIN clock
    [FSP_PRIV_CLOCK_CLK_RCANOSC] = BSP_CFG_CLOCK_CLK_RCANOSC_HZ, ///< Initial value of CLK_RCANOSC clock
    [FSP_PRIV_CLOCK_CLK_ADC]     = BSP_CFG_CLOCK_CLK_ADC_HZ,     ///< Initial value of CLK_ADC clock
    [FSP_PRIV_CLOCK_CLK_MSPI]    = BSP_CFG_CLOCK_CLK_MSPI_HZ,    ///< Initial value of CLK_MSPI clock
    [FSP_PRIV_CLOCK_CLK_ECMCNT]  = BSP_CFG_CLOCK_CLK_ECMCNT_HZ,  ///< Initial value of CLK_ECMCNT clock
    [FSP_PRIV_CLOCK_CLK_WDT]     = BSP_CFG_CLOCK_CLK_WDT_HZ,     ///< Initial value of CLK_WDT clock
    [FSP_PRIV_CLOCK_CLKA_WDT]    = BSP_CFG_CLOCK_CLKA_WDT_HZ,    ///< Initial value of CLKA_WDT clock
    [FSP_PRIV_CLOCK_CLKA_TAUJ]   = BSP_CFG_CLOCK_CLKA_TAUJ_HZ,   ///< Initial value of CLKA_TAUJ clock
    [FSP_PRIV_CLOCK_CLKA_RTCA]   = BSP_CFG_CLOCK_CLKA_RTCA_HZ,   ///< Initial value of CLKA_RTCA clock
#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    [FSP_PRIV_CLOCK_CLKA_ADC] = BSP_CFG_CLOCK_CLKA_ADC_HZ,       ///< Initial value of CLKA_ADC clock
#endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [FSP_PRIV_CLOCK_EXTCLK0O] = BSP_CFG_CLOCK_EXTCLK0O_HZ,       ///< Initial value of EXTCLK0O clock
    [FSP_PRIV_CLOCK_EXTCLK1O] = BSP_CFG_CLOCK_EXTCLK1O_HZ,       ///< Initial value of EXTCLK1O clock
    [FSP_PRIV_CLOCK_CLKA_LPS] = BSP_CFG_CLOCK_CLKA_LPS_HZ,       ///< Initial value of CLKA_LPS clock
};

/* PRQA S 1510 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes variable to store system clock frequencies.
 **********************************************************************************************************************/
static void bsp_clock_freq_var_init (void)
{
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MOSC]    = BSP_CFG_CLOCK_CLK_MOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSIOSC]  = BSP_CFG_CLOCK_CLK_HSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSIOSC]  = BSP_CFG_CLOCK_CLK_LSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HVIOSC]  = BSP_CFG_CLOCK_CLK_HVIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_PLL]     = BSP_CFG_CLOCK_CLK_PLL_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_PLLO]    = BSP_CFG_CLOCK_CLK_PLLO_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_IOSC]    = BSP_CFG_CLOCK_CLK_IOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS]     = BSP_CFG_CLOCK_CLK_SYS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CPU]     = BSP_CFG_CLOCK_CLK_CPU_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SBUS]    = BSP_CFG_CLOCK_CLK_SBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HBUS]    = BSP_CFG_CLOCK_CLK_HBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_UHSB]    = BSP_CFG_CLOCK_CLK_UHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSB]     = BSP_CFG_CLOCK_CLK_HSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSB]     = BSP_CFG_CLOCK_CLK_LSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RLIN]    = BSP_CFG_CLOCK_CLK_RLIN3_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RCANOSC] = BSP_CFG_CLOCK_CLK_RCANOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_ADC]     = BSP_CFG_CLOCK_CLK_ADC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MSPI]    = BSP_CFG_CLOCK_CLK_MSPI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_ECMCNT]  = BSP_CFG_CLOCK_CLK_ECMCNT_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_WDT]     = BSP_CFG_CLOCK_CLK_WDT_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_WDT]    = BSP_CFG_CLOCK_CLKA_WDT_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_TAUJ]   = BSP_CFG_CLOCK_CLKA_TAUJ_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_RTCA]   = BSP_CFG_CLOCK_CLKA_RTCA_HZ;
#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_ADC] = BSP_CFG_CLOCK_CLKA_ADC_HZ;
#endif                                 /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK0O] = BSP_CFG_CLOCK_EXTCLK0O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK1O] = BSP_CFG_CLOCK_EXTCLK1O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_LPS] = BSP_CFG_CLOCK_CLKA_LPS_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 **********************************************************************************************************************/
void bsp_clock_driver_init (void)
{
    /* Disable write protection of clock controller (enable register write access) */
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

    /* Check if the system clock is selected as PLLO */
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

        /* Wait 1.4us */
        R_BSP_SoftwareDelay(2U, BSP_DELAY_UNITS_MICROSECONDS);
    }

    /* Check if the configured system clock is PLLO */
    /* Gear Up/Down the PLLO */
    bsp_gear_up_down_pllo((uint8_t) BSP_CFG_CLOCK_CKSC_CPUC_SRC, (uint32_t) BSP_CFG_CLOCK_CLK_PLL_HZ);

    /* CLK_RLIN3 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = (uint32_t) BSP_CFG_CLOCK_CLK_RLIN3_SRC;                                /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, (uint32_t) BSP_CFG_CLOCK_CLK_RLIN3_SRC);     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLK_RCANOSC selector setting */
    R_SYS0_CLOCK->CKSC_RCANC = (uint32_t) BSP_CFG_CLOCK_CLK_RCANOSC_SRC;                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_RCANS, (uint32_t) BSP_CFG_CLOCK_CLK_RCANOSC_SRC);  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLK_ADC selector setting */
    R_SYS0_CLOCK->CKSC_ADCC = (uint32_t) BSP_CFG_CLOCK_CLK_ADC_SRC;                                  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_ADCS, (uint32_t) BSP_CFG_CLOCK_CLK_ADC_SRC);       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = (uint32_t) BSP_CFG_CLOCK_CLK_MSPI_SRC;                                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, (uint32_t) BSP_CFG_CLOCK_CLK_MSPI_SRC);      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_AWDTC = (uint32_t) BSP_CFG_CLOCK_CLKA_WDT_SRC;                              /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AWDTS, (uint32_t) BSP_CFG_CLOCK_CLKA_WDT_SRC);   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKSC_ATAUJC = (uint32_t) BSP_CFG_CLOCK_CLKA_TAUJ_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, (uint32_t) BSP_CFG_CLOCK_CLKA_TAUJ_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKSC_ARTCAC = (uint32_t) BSP_CFG_CLOCK_CLKA_RTCA_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS, (uint32_t) BSP_CFG_CLOCK_CLKA_RTCA_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKSC_AADCC = (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_SRC;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS, (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_SRC); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* CLKA_ADC divider setting */
    R_SYS0_A_CLOCK->CLKD_AADCC = (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_DIV;                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_AADCS, (uint32_t) BSP_CFG_CLOCK_CLKA_ADC_DIV); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
#endif                                                                                             /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

#if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT0C = 0U;                                                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk)); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT0C = (uint32_t) BSP_CFG_CLOCK_EXTCLK0O_SRC;                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, (uint32_t) BSP_CFG_CLOCK_EXTCLK0O_SRC);                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S_b.FOUT0SYNC,                                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;                                                       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S,                                                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0CLKACT_Msk));
#endif                                                                                                              /* #if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

#if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK1O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT1C = 0U;                                                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk)); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT1C = (uint32_t) BSP_CFG_CLOCK_EXTCLK1O_SRC;                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT1S, (uint32_t) BSP_CFG_CLOCK_EXTCLK1O_SRC);                 /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S_b.FOUT1SYNC,                                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               (uint32_t) 1U);
    R_SYS0_A_CLOCK->CLKD_FOUT1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;                                                       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S,                                                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                               ((uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk | \
                                (uint32_t) R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1CLKACT_Msk));
#endif                                                                                                              /* #if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0 */ /* PRQA S 2053 # Advisory D-4.4 */

    /* CLK_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_WDTC = (uint32_t) BSP_CFG_CLOCK_CLK_WDT_SRC;                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_WDTS, (uint32_t) BSP_CFG_CLOCK_CLK_WDT_SRC);                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* If the PLL is not used */
    if (0 == BSP_CFG_CLOCK_CLK_PLL_HZ)                                                                              /* PRQA S 2742 # DP_MSR_R-14.3_UC1 */
    {
        /* Confirm that the PLL is stable */
        if (((uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk | (uint32_t) R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk) ==
            R_SYS0_CLOCK->PLLS)                                                                                     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        {
            /* Stop the PLL (PLLE.PLLDISTRG = 1). */
            R_SYS0_CLOCK->PLLE_b.PLLDISTRG = 1U;                                                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }

        /* Confirm that the PLL has been stopped (PLLS.PLLCLKEN = 0 & PLLS.PLLCLKSTAB = 0) */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS, 0U);                                                         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Enable write protection of clock controller (lock registers) */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;                                                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    bsp_clock_freq_var_init();
    __SYNCP();                                                                                                      /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * Gear Down Clock.
 **********************************************************************************************************************/
void bsp_clock_gear_down (void)
{
    /* Disable write protection of clock controller (enable register write access) */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    uint32_t pll_clock_freq = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_PLL);
    bsp_gear_up_down_pllo(1U, (uint32_t) pll_clock_freq);

    /* Enable write protection of clock controller (lock registers) */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE; /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Flush pipeline after clock source change */
    __SYNCP();                                          /* PRQA S 3469 # Advisory D-4.9 */
}

/*******************************************************************************************************************//**
 * System Clock (PLLO) Gear Up.
 **********************************************************************************************************************/
static void bsp_gear_up_down_pllo (uint8_t gear_up_down_select, uint32_t pll_clock_hz)
{
    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(pll_clock_hz);

    if (0U != gear_up_down_select)
    {
        /* Check if Clock source is other than CLK_IOSC, Clock divider is no division, and clock divider synchronous is stable */
        /* PRQA S 0306, 0662 ++ # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        if ((0U == R_SYS0_CLOCK->CKSC_CPUS) && (1U == R_SYS0_CLOCK->CKD_PLLC) && \
            ((uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK == R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC))

        /* PRQA S 0306, 0662 -- */
        {
            /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
            R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_2;                                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Confirm that PLL0 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, (uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Change the clock source for the system clock to CLK_IOSC */
            R_SYS0_CLOCK->CKSC_CPUC = 1U;                                                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_CPUS, 1U);                                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }
    else
    {
        /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_2;                                                            /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that PLL0 dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, (uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Change the clock source for the system clock to CLK_PLLO */
        R_SYS0_CLOCK->CKSC_CPUC = 0U;                                                                               /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that CLK_PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_CPUS, 0U);                                                    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_NO_DIVISION;                                                             /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

        /* Confirm that PLL0 dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, (uint32_t) BSP_CKD_PLLS_PLLCLKDSYNC_MASK); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Wait 100 us after verifying */
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
        case FSP_PRIV_CLOCK_CLK_PLLO:
        case FSP_PRIV_CLOCK_CLK_HVIOSC:
        case FSP_PRIV_CLOCK_CLK_LSIOSC:
        case FSP_PRIV_CLOCK_CLK_HSIOSC:
        case FSP_PRIV_CLOCK_CLK_IOSC:
        case FSP_PRIV_CLOCK_CLK_MOSC:
        {
            frequency = bsp_source_clock_hz_get(clock);
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SYS:
        {
            frequency = bsp_system_clock_hz_get(clock);
            break;
        }

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

        case FSP_PRIV_CLOCK_CLK_RLIN:
        case FSP_PRIV_CLOCK_CLK_ADC:
        case FSP_PRIV_CLOCK_CLK_MSPI:
        case FSP_PRIV_CLOCK_CLK_RCANOSC:
        case FSP_PRIV_CLOCK_CLK_ECMCNT:
        case FSP_PRIV_CLOCK_CLK_WDT:
        case FSP_PRIV_CLOCK_CLKA_WDT:
        case FSP_PRIV_CLOCK_CLKA_TAUJ:
        case FSP_PRIV_CLOCK_CLKA_RTCA:
#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CLKA_ADC:
#endif                                 /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
        case FSP_PRIV_CLOCK_CLKA_LPS:
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
    uint32_t              frequency = 0U;
    uint8_t               opbt_mosc_freq_value;
    uint8_t               opbt_pllo_freq_value;
    static const uint32_t main_os_clocks[] =
    {
        BSP_CLKCTL_CLK_MOSC_16MHZ, BSP_CLKCTL_CLK_MOSC_20MHZ, BSP_CLKCTL_CLK_MOSC_24MHZ, BSP_CLKCTL_CLK_MOSC_40MHZ
    };
    static const uint32_t pllo_clocks[] =
    {
        BSP_CLKCTL_CLK_PLLO_480MHZ, BSP_CLKCTL_CLK_PLLO_480MHZ, BSP_CLKCTL_CLK_PLLO_640MHZ, BSP_CLKCTL_CLK_PLLO_800MHZ
    };
    static const uint8_t pllo_divider_numerators[] =
    {0U, 1U, 1U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U, 0U};
    static const uint8_t pllo_divider_denominators[] =
    {1U, 1U, 2U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U};

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_HVIOSC:
        {
            frequency = BSP_CLOCK_HVIOSC_VALUE; /* HV IntOSC clock = 16 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_LSIOSC:
        {
            frequency = BSP_CLOCK_LSIOSC_VALUE; /* LS IntOSC clock = 240 kHz */
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
                                               * Internal OSC clock = LS IntOSC clock = 240 kHz only when HS IntOSC clock
                                               * stops in chip standby mode (Stop / DeepStop mode)*/
            break;
        }

        case FSP_PRIV_CLOCK_CLK_MOSC:
        {
            opbt_mosc_freq_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(10U) & BSP_OPBT_MOSC_FREQ_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_MOSC_FREQ_POS);
            frequency = main_os_clocks[opbt_mosc_freq_value];                           /* Main OSC clock = 16 MHz / 20 MHz / 24 MHz / 40 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_PLL:
        {
            opbt_pllo_freq_value = (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                                              BSP_OPBT_CKDIVMD_POS);
            frequency = pllo_clocks[opbt_pllo_freq_value];                                               /* PLL clock clock = 800 MHz / 640 MHz / 480 MHz */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_PLLO:
        {
            uint8_t pllo_divider_option = R_SYS0_CLOCK->CKD_PLLC & R_SYS0_CLOCK_CKD_PLLC_PLLCLKDCSID_Msk; /* PRQA S 0306, 0662, 4461 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, DP_MSR_R-10.3_UC1 */
            frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_PLL);                                  /* PRQA S 3670 # DP_MSR_R-17.2_UC1 */

            frequency = (frequency * pllo_divider_numerators[pllo_divider_option]) /
                        pllo_divider_denominators[pllo_divider_option];
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
    uint32_t frequency;

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_SYS:
        {
            /* Check the source clock of System clock for SSCG Domain */
            if (1U == R_SYS0_CLOCK->CKSC_CPUS) /* CLK_IOSC */ /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            {
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_IOSC);
            }
            else /* CLK_PLLO */
            {
                frequency = bsp_source_clock_hz_get(FSP_PRIV_CLOCK_CLK_PLLO);
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
    static const uint8_t clk_sbus_dividers[] = {6U, 6U, 4U, 4U};
    static const uint8_t clk_hbus_dividers[] = {6U, 6U, 8U, 8U};
    static const uint8_t clk_uhsb_dividers[] = {3U, 3U, 4U, 5U};
    static const uint8_t clk_hsb_dividers[]  = {6U, 6U, 8U, 10U};
    static const uint8_t clk_lsb_dividers[]  = {12U, 12U, 16U, 20U};

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_CPU:
        {
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
            frequency /= 2U;
            break;
        }

        case FSP_PRIV_CLOCK_CLK_SBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
            frequency /= clk_sbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_HBUS:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
            frequency /= clk_hbus_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_UHSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
            frequency /= clk_uhsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_HSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0499, 0303, 3432 # DP_MSR_R-12.2_UC1, Advisory R-11.4, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
            frequency /= clk_hsb_dividers[opbt_ckdivmd_value];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_LSB:
        {
            opbt_ckdivmd_value =
                (uint8_t) ((BSP_OPBT_VALID_SIDE_OPBTn(11U) & BSP_OPBT_CKDIVMD_MSK) >> /* PRQA S 0303, 0499, 3432 # Advisory R-11.4, DP_MSR_R-12.2_UC1, DP_MSR_R-20.7_UC1 */
                           BSP_OPBT_CKDIVMD_POS);
            frequency  = bsp_system_clock_hz_get(FSP_PRIV_CLOCK_CLK_SYS);
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
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_HSB, FSP_PRIV_CLOCK_NUM, FSP_PRIV_CLOCK_NUM
    };
    static const fsp_priv_clock_t rlin3_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_HSB, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_MOSC
    };
    static const uint8_t          adc_clock_div[]    = {BSP_CLOCK_ADC_NO_DIV_VALUE, BSP_CLOCK_ADC_DIV_2_VALUE};
    static const uint8_t          aadc_clock_div[]   = {BSP_CLOCK_AADC_DIV_2_VALUE, BSP_CLOCK_AADC_NO_DIV_VALUE};
    static const uint16_t         wdt_clock_div[]    = {BSP_CLOCK_WDT_DIV_20_VALUE, BSP_CLOCK_WDT_DIV_640_VALUE};
    static const uint16_t         wdta_clock_div[]   = {BSP_CLOCK_WDTA_NO_DIV_VALUE, BSP_CLOCK_WDTA_DIV_128_VALUE};
    static const fsp_priv_clock_t atauj_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_LSIOSC, FSP_PRIV_CLOCK_CLK_HSIOSC, FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_HSB
    };
    static const fsp_priv_clock_t rtca_clock_srcs[] = {FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_LSIOSC};
    static const fsp_priv_clock_t aadc_clock_srcs[] =
    {
        FSP_PRIV_CLOCK_CLK_MOSC, FSP_PRIV_CLOCK_CLK_HSIOSC, FSP_PRIV_CLOCK_CLK_LSB, FSP_PRIV_CLOCK_NUM
    };

    switch (clock)
    {
        case FSP_PRIV_CLOCK_CLK_MSPI:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_MSPIS & R_SYS0_CLOCK_CKS_MSPIS_MSPISACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(mspi_clock_srcs[clock_src]);                      /* PRQA S 1520 # DP_MSR_R-17.2_UC1 */
            break;
        }

        case FSP_PRIV_CLOCK_CLK_RLIN:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKS_RLINS & R_SYS0_CLOCK_CKS_RLINS_RLINSACT_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
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

        case FSP_PRIV_CLOCK_CLK_ADC:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKSC_ADCS & R_SYS0_CLOCK_CKSC_ADCS_ADCSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_LSB) / adc_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_WDT:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_WDTS & R_SYS0_A_CLOCK_CKSC_WDTS_WDTSACT_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_HSIOSC) / wdt_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLK_RCANOSC:
        {
            clock_src = (uint8_t) (R_SYS0_CLOCK->CKSC_RCANS & R_SYS0_CLOCK_CKSC_RCANS_RCANSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_MOSC);

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

        case FSP_PRIV_CLOCK_CLK_ECMCNT:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_HSIOSC) / 20U;
            break;
        }

        case FSP_PRIV_CLOCK_CLKA_WDT:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_AWDTS & R_SYS0_A_CLOCK_CKSC_AWDTS_AWDTSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_LSIOSC) / wdta_clock_div[clock_src];
            break;
        }

        case FSP_PRIV_CLOCK_CLKA_TAUJ:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_ATAUJS & R_SYS0_A_CLOCK_CKSC_ATAUJS_ATAUJSACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(atauj_clock_srcs[clock_src]);

            if (BSP_CLOCK_ATAUJ_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            break;
        }

        case FSP_PRIV_CLOCK_CLKA_RTCA:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_ARTCAS & R_SYS0_A_CLOCK_CKSC_ARTCAS_ARTCASACT_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(rtca_clock_srcs[clock_src]);

            if (BSP_CLOCK_RTCA_DIV_16_VALUE == clock_src)
            {
                frequency /= 16U;
            }

            break;
        }

#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
        case FSP_PRIV_CLOCK_CLKA_ADC:
        {
            uint8_t aadc_div = (uint8_t) (R_SYS0_A_CLOCK->CLKD_AADCC & R_SYS0_A_CLOCK_CLKD_AADCC_AADCDCSID_Msk); /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_AADCS & R_SYS0_A_CLOCK_CKSC_AADCS_AADCSACT_Msk);         /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
            frequency = r_bsp_clock_hz_calculate(aadc_clock_srcs[clock_src]);

            if (BSP_CLOCK_AADC_DIV_20_VALUE == clock_src)
            {
                frequency /= 20U;
            }

            frequency /= aadc_clock_div[aadc_div];

            break;
        }
#endif                                 /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */

        case FSP_PRIV_CLOCK_CLKA_LPS:
        {
            frequency = r_bsp_clock_hz_calculate(FSP_PRIV_CLOCK_CLK_HSIOSC) / 20U;
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
        [0] = FSP_PRIV_CLOCK_CLK_MOSC, /* PRQA S 3676 # DP_MSR_R-9.5_UC1 */
        [1] = FSP_PRIV_CLOCK_CLK_HSB,
        [2] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [3] = FSP_PRIV_CLOCK_CLK_LSIOSC,
        [4] = FSP_PRIV_CLOCK_CLK_HSIOSC,
        [5] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [6] = FSP_PRIV_CLOCK_CLK_MOSC,
        [7] = FSP_PRIV_CLOCK_CLK_MOSC
    };
    static const fsp_priv_clock_t extclk1_clock_srcs[] =
    {
        [0] = FSP_PRIV_CLOCK_CLK_MOSC, /* PRQA S 3676 # DP_MSR_R-9.5_UC1 */
        [1] = FSP_PRIV_CLOCK_CLK_HSB,
        [2] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [3] = FSP_PRIV_CLOCK_CLK_LSIOSC,
        [4] = FSP_PRIV_CLOCK_CLK_HSIOSC,
        [5] = FSP_PRIV_CLOCK_NUM,      /* Setting prohibited, will return 0 value */
        [6] = FSP_PRIV_CLOCK_CLK_MOSC,
        [7] = FSP_PRIV_CLOCK_CLK_MOSC
    };

    switch (clock)
    {
        case FSP_PRIV_CLOCK_EXTCLK0O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_FOUT0S & R_SYS0_A_CLOCK_CKSC_FOUT0S_FOUT0SACT_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CLKD_FOUT0C & R_SYS0_A_CLOCK_CLKD_FOUT0C_FOUT0DIV_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */

            frequency = (0U != clock_div) ? (r_bsp_clock_hz_calculate(extclk0_clock_srcs[clock_src]) / clock_div) : 0U;
            frequency = (FSP_PRIV_CLOCK_CLK_HSIOSC == extclk0_clock_srcs[clock_src]) ? (frequency / 20U) : frequency;
            break;
        }

        case FSP_PRIV_CLOCK_EXTCLK1O:
        {
            clock_src = (uint8_t) (R_SYS0_A_CLOCK->CKSC_FOUT1S & R_SYS0_A_CLOCK_CKSC_FOUT1S_FOUT1SACT_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */
            clock_div = (uint16_t) (R_SYS0_A_CLOCK->CLKD_FOUT1C & R_SYS0_A_CLOCK_CLKD_FOUT1C_FOUT1DIV_Msk); /* PRQA S 0662, 0306 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1, Advisory R-11.4 */

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

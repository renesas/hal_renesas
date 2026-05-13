/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_clocks_driver.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

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
static void bsp_gear_up_down_pllo(bool gear_up_down_select, uint32_t pll_clock_hz);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* This array stores the clock frequency of each bus clock. This section of RAM should not be initialized by the C
 * runtime environment. This is initialized and used in R_BSP_InitClock, which is called before the C runtime
 * environment is initialized. */
uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM] =
{
    BSP_CFG_CLOCK_CLK_MOSC_HZ,         ///< Initial value of CLK_MOSC clock
    BSP_CFG_CLOCK_CLK_HSIOSC_HZ,       ///< Initial value of CLK_HSIOSC clock
    BSP_CFG_CLOCK_CLK_LSIOSC_HZ,       ///< Initial value of CLK_LSIOSC clock
    BSP_CFG_CLOCK_CLK_HVIOSC_HZ,       ///< Initial value of CLK_HVIOSC clock
    BSP_CFG_CLOCK_CLK_PLL_HZ,          ///< Initial value of CLK_PLL clock
    BSP_CFG_CLOCK_CLK_PLLO_HZ,         ///< Initial value of CLK_PLLO clock
    BSP_CFG_CLOCK_CLK_IOSC_HZ,         ///< Initial value of CLK_IOSC clock
    BSP_CFG_CLOCK_CLK_SYS_HZ,          ///< Initial value of CLK_SYS clock
    BSP_CFG_CLOCK_CLK_CPU_HZ,          ///< Initial value of CLK_CPU clock
    BSP_CFG_CLOCK_CLK_SBUS_HZ,         ///< Initial value of CLK_SBUS clock
    BSP_CFG_CLOCK_CLK_HBUS_HZ,         ///< Initial value of CLK_HBUS clock
    BSP_CFG_CLOCK_CLK_HSB_HZ,          ///< Initial value of CLK_HSB clock
    BSP_CFG_CLOCK_CLK_LSB_HZ,          ///< Initial value of CLK_LSB clock
    BSP_CFG_CLOCK_CLK_RLIN3_HZ,        ///< Initial value of CLK_RLIN clock
    BSP_CFG_CLOCK_CLK_RCANOSC_HZ,      ///< Initial value of CLK_RCANOSC clock
    BSP_CFG_CLOCK_CLK_ADC_HZ,          ///< Initial value of CLK_ADC clock
    BSP_CFG_CLOCK_CLK_MSPI_HZ,         ///< Initial value of CLK_MSPI clock
    BSP_CFG_CLOCK_CLK_ECMCNT_HZ,       ///< Initial value of CLK_ECMCNT clock
    BSP_CFG_CLOCK_CLK_WDT_HZ,          ///< Initial value of CLK_WDT clock
    BSP_CFG_CLOCK_CLKA_WDT_HZ,         ///< Initial value of CLKA_WDT clock
    BSP_CFG_CLOCK_CLKA_TAUJ_HZ,        ///< Initial value of CLKA_TAUJ clock
    BSP_CFG_CLOCK_CLKA_RTCA_HZ,        ///< Initial value of CLKA_RTCA clock
#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    BSP_CFG_CLOCK_CLKA_ADC_HZ,         ///< Initial value of CLKA_ADC clock
#endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */
    BSP_CFG_CLOCK_EXTCLK0O_HZ,         ///< Initial value of EXTCLK0O clock
    BSP_CFG_CLOCK_EXTCLK1O_HZ,         ///< Initial value of EXTCLK1O clock
    BSP_CFG_CLOCK_CLKA_LPS_HZ,         ///< Initial value of CLKA_LPS clock
};

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
#endif                                 /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK0O] = BSP_CFG_CLOCK_EXTCLK0O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK1O] = BSP_CFG_CLOCK_EXTCLK1O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKA_LPS] = BSP_CFG_CLOCK_CLKA_LPS_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 **********************************************************************************************************************/
void bsp_clock_driver_init (void)
{
    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE;

    /* Confirm HS IntOSC is stable*/
    R_SYS0_A_CLOCK->HSOSCSTPM = BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->HSOSCS, R_SYS0_A_CLOCK_HSOSCS_HSOSCSTAB_Msk);

    /* If Main OSC have been configured */
    R_SYS0_A_CLOCK->MOSCSTPM = BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE;

    /* Confirm Main OSC is stopped */
    if (0UL == R_SYS0_A_CLOCK->MOSCS)
    {
        /* Enable MOSC */
        R_SYS0_A_CLOCK->MOSCE_b.MOSCENTRG = R_SYS0_A_CLOCK_MOSCE_MOSCENTRG_Msk;
    }

    /* Confirm that the Main OSC have been stable (MOSCS.MOSCEN = 1 & MOSCS.MOSCSTAB = 1) */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS,
                               (R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk | \
                                R_SYS0_A_CLOCK_MOSCS_MOSCSTAB_Msk));

    /* Check if the system clock is selected as PLLO */
    if (0UL != BSP_CFG_CLOCK_CLK_PLL_HZ)
    {
        R_SYS0_CLOCK->PLLSTPM = BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE;

        /* Confirm the status of PLL */
        if (0UL == R_SYS0_CLOCK->PLLS)
        {
            /* The status of PLL is stopped (PLLS.PLLCLKEN = 0 & PLLS.PLLCLKSTAB = 0) */
            /* Start the PLL: PLLE.PLLENTRG = 1 */
            R_SYS0_CLOCK->PLLE_b.PLLENTRG = R_SYS0_CLOCK_PLLE_PLLENTRG_Msk;
        }

        /* Confirm that the PLL has been stable (PLLS.PLLCLKEN = 1 & PLLS.PLLCLKSTAB = 1) */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS,
                                   (R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk | \
                                    R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk));

        /* Wait 1.4us */
        R_BSP_SoftwareDelay(2, BSP_DELAY_UNITS_MICROSECONDS);
    }

    /* Check if the configured system clock is PLLO */
    /* Gear Up/Down the PLLO */
    bsp_gear_up_down_pllo(BSP_CFG_CLOCK_CKSC_CPUC_SRC, BSP_CFG_CLOCK_CLK_PLL_HZ);

    /* CLK_RLIN3 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = BSP_CFG_CLOCK_CLK_RLIN3_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, BSP_CFG_CLOCK_CLK_RLIN3_SRC);

    /* CLK_RCANOSC selector setting */
    R_SYS0_CLOCK->CKSC_RCANC = BSP_CFG_CLOCK_CLK_RCANOSC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_RCANS, BSP_CFG_CLOCK_CLK_RCANOSC_SRC);

    /* CLK_ADC selector setting */
    R_SYS0_CLOCK->CKSC_ADCC = BSP_CFG_CLOCK_CLK_ADC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_ADCS, BSP_CFG_CLOCK_CLK_ADC_SRC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = BSP_CFG_CLOCK_CLK_MSPI_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, BSP_CFG_CLOCK_CLK_MSPI_SRC);

    /* CLKA_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_AWDTC = BSP_CFG_CLOCK_CLKA_WDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AWDTS, BSP_CFG_CLOCK_CLKA_WDT_SRC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKSC_ATAUJC = BSP_CFG_CLOCK_CLKA_TAUJ_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, BSP_CFG_CLOCK_CLKA_TAUJ_SRC);

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKSC_ARTCAC = BSP_CFG_CLOCK_CLKA_RTCA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS, BSP_CFG_CLOCK_CLKA_RTCA_SRC);

#if (BSP_FEATURE_ADCKA_IS_AVAILABLE)

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKSC_AADCC = BSP_CFG_CLOCK_CLKA_ADC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS, BSP_CFG_CLOCK_CLKA_ADC_SRC);
#endif                                 /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */

#if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT0C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, (R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT0C = BSP_CFG_CLOCK_EXTCLK0O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, BSP_CFG_CLOCK_EXTCLK0O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S_b.FOUT0SYNC, R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk);
    R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S,
                               (R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk | \
                                R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0CLKACT_Msk));
#endif                                 /* #if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0 */

#if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT1C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, (R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT1C = BSP_CFG_CLOCK_EXTCLK1O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT1S, BSP_CFG_CLOCK_EXTCLK1O_SRC);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S_b.FOUT1SYNC, R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk);
    R_SYS0_A_CLOCK->CLKD_FOUT1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S,
                               (R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk | \
                                R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1CLKACT_Msk));
#endif                                 /* #if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0 */

    /* CLK_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_WDTC = BSP_CFG_CLOCK_CLK_WDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_WDTS, BSP_CFG_CLOCK_CLK_WDT_SRC);

    /* If the PLL is not used */
    if (0UL == BSP_CFG_CLOCK_CLK_PLL_HZ)
    {
        /* Confirm that the PLL is stable */
        if ((R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk | R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk) == R_SYS0_CLOCK->PLLS)
        {
            /* Stop the PLL (PLLE.PLLDISTRG = 1). */
            R_SYS0_CLOCK->PLLE_b.PLLDISTRG = 1U;
        }

        /* Confirm that the PLL has been stopped (PLLS.PLLCLKEN = 0 & PLLS.PLLCLKSTAB = 0) */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS, 0UL);
    }

    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;

    bsp_clock_freq_var_init();
    __SYNCP();
}

/*******************************************************************************************************************//**
 * Gear Down Clock.
 **********************************************************************************************************************/
void bsp_clock_gear_down (void)
{
    bsp_gear_up_down_pllo(1U, BSP_CFG_CLOCK_CLK_PLL_HZ);
}

/*******************************************************************************************************************//**
 * System Clock (PPLO) Gear Up.
 **********************************************************************************************************************/
static void bsp_gear_up_down_pllo (bool gear_up_down_select, uint32_t pll_clock_hz)
{
    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(pll_clock_hz);

    if (gear_up_down_select)
    {
        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0UL == R_SYS0_CLOCK->CKSC_CPUS) && (0UL == R_SYS0_CLOCK->CKD_PLLC) && \
            (R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk == R_SYS0_CLOCK->CKD_PLLS))
        {
            /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
            R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_2;

            /* Confirm that PLL0 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, BSP_CKD_PLLS_PLLCLKDSYNC_MASK);

            /* Change the clock source for the system clock to CLK_IOSC */
            R_SYS0_CLOCK->CKSC_CPUC = BSP_CFG_CLOCK_CKSC_CPUC_SRC;

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_CPUS, BSP_CFG_CLOCK_CKSC_CPUC_SRC);
        }
    }
    else
    {
        /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_2;

        /* Confirm that PLL0 dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS_b.PLLCLKDSYNC, BSP_CKD_PLLS_PLLCLKDSYNC_MASK);

        /* Change the clock source for the system clock to CLK_PLLO */
        R_SYS0_CLOCK->CKSC_CPUC = BSP_CFG_CLOCK_CKSC_CPUC_SRC;

        /* Confirm that CLK_PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_CPUS, BSP_CFG_CLOCK_CKSC_CPUC_SRC);

        /*  Division ratio of clock source PLL is changed from 1/1 to 1/2 */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_NO_DIVISION;
    }

    /* Wait 100 μs after verifying */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
}

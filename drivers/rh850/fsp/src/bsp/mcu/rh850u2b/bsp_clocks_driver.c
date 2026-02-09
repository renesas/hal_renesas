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
static void bsp_gear_up_down_sscg(bool                      gear_up_down_select,
                                  st_systemclock_sscg_ctl * sscg_register,
                                  uint32_t                  sscg_clock_hz);

static void bsp_gear_up_down_pllo(bool gear_up_down_select, uint32_t pll_clock_hz);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* This array stores the clock frequency of each bus clock. This section of RAM should not be initialized by the C
 * runtime environment. This is initialized and used in R_BSP_InitClock, which is called before the C runtime
 * environment is initialized. */
uint32_t g_clock_freq[FSP_PRIV_CLOCK_NUM] =
{
    BSP_CFG_CLOCK_CLK_PLL_HZ,          ///< Initial value of PLL clock
    BSP_CFG_CLOCK_CLK_SSCG_HZ,         ///< Initial value of SSCG clock
#if BSP_FEATURE_DFP_IS_AVAILABLE
    BSP_CFG_CLOCK_CLK_SSCG1_HZ,        ///< Initial value of SSCG1 clock
#endif /* #if BSP_FEATURE_DFP_IS_AVAILABLE */
    BSP_CFG_CLOCK_CLK_HVIOSC_HZ,       ///< Initial value of HVIOSC clock
    BSP_CFG_CLOCK_CLK_LSIOSC_HZ,       ///< Initial value of LSIOSC clock
    BSP_CFG_CLOCK_CLK_HSIOSC_HZ,       ///< Initial value of HSIOSC clock
    BSP_CFG_CLOCK_CLK_IOSC_HZ,         ///< Initial value of IOSC clock
    BSP_CFG_CLOCK_CLK_MOSC_HZ,         ///< Initial value of MOSC clock
    BSP_CFG_CLOCK_CLK_EMG_HZ,          ///< Initial value of EMG clock
    BSP_CFG_CLOCK_CLK_ECMCNT_HZ,       ///< Initial value of EMCCNT clock
    BSP_CFG_CLOCK_EXTCLK0O_HZ,         ///< Initial value of EXTCLK0O clock
    BSP_CFG_CLOCK_EXTCLK1O_HZ,         ///< Initial value of EXTCLK1O clock
    BSP_CFG_CLOCK_CLK_MSPI_HZ,         ///< Initial value of MSPI clock
    BSP_CFG_CLOCK_CLK_RLIN3_HZ,        ///< Initial value of RLIN3 clock
    BSP_CFG_CLOCK_CLK_CH23_RLIN3_HZ,   ///< Initial value of RLIN3 channel 23 clock
    BSP_CFG_CLOCK_CLK_GTM_HZ,          ///< Initial value of GTM clock
    BSP_CFG_CLOCK_CLK_WDTB_HZ,         ///< Initial value of WDTB clock
    BSP_CFG_CLOCK_CLK_CANFD_C_HZ,      ///< Initial value of CANFD C clock
    BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ,    ///< Initial value of CANFD XIN clock
    BSP_CFG_CLOCK_CLKC_CPU_HZ,         ///< Initial value of CLKC CPU clock
    BSP_CFG_CLOCK_CLKC_SBUS_HZ,        ///< Initial value of CLKC SBUS clock
    BSP_CFG_CLOCK_CLKC_HBUS_HZ,        ///< Initial value of CLKC HBUS clock
    BSP_CFG_CLOCK_CLKC_UHSB_HZ,        ///< Initial value of CLKC UHSB clock
    BSP_CFG_CLOCK_CLKC_SHSB_HZ,        ///< Initial value of CLKC SHSB clock
    BSP_CFG_CLOCK_CLKC_HSB_HZ,         ///< Initial value of CLKC HSB clock
    BSP_CFG_CLOCK_CLKC_LSB_HZ,         ///< Initial value of CLKC LSB clock
    BSP_CFG_CLOCK_CLK_CPU_HZ,          ///< Initial value of CLK CPU clock
    BSP_CFG_CLOCK_CLK_SBUS_HZ,         ///< Initial value of CLK SBUS clock
    BSP_CFG_CLOCK_CLK_HBUS_HZ,         ///< Initial value of CLK HBUS clock
    BSP_CFG_CLOCK_CLK_UHSB_HZ,         ///< Initial value of CLK UHSB clock
    BSP_CFG_CLOCK_CLK_HSB_HZ,          ///< Initial value of CLK HSB clock
    BSP_CFG_CLOCK_CLK_LSB_HZ,          ///< Initial value of CLK LSB clock
    BSP_CFG_CLOCK_CLKC_SWDT_HZ,        ///< Initial value of CLKC SWDT clock
#if BSP_FEATURE_DFP_IS_AVAILABLE
    BSP_CFG_CLOCK_CLK_DFP_HZ,          ///< Initial value of CLK DFP clock
    BSP_CFG_CLOCK_CLK_DFP_DIV_2_HZ     ///< Initial value of CLK DFP / 2 clock
#endif /* #if BSP_FEATURE_DFP_IS_AVAILABLE */
};

/** Gear up clock divider sequence */
static const uint32_t g_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** Gear down clock divider sequence */
static const uint32_t g_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,     ///< Clock divided by 6
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes variable to store system clock frequencies.
 **********************************************************************************************************************/
static void bsp_clock_freq_var_init (void)
{
    g_clock_freq[FSP_PRIV_CLOCK_CLK_PLL]      = BSP_CFG_CLOCK_CLK_PLL_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SSCG]     = BSP_CFG_CLOCK_CLK_SSCG_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS_SSCG] = BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ;
#if BSP_FEATURE_DFP_IS_AVAILABLE
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SSCG1]     = BSP_CFG_CLOCK_CLK_SSCG1_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SYS_SSCG1] = BSP_CFG_CLOCK_CLK_SYS_SSCG1_HZ;
#endif                                 /* #if BSP_FEATURE_DFP_IS_AVAILABLE */
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HVIOSC]     = BSP_CFG_CLOCK_CLK_HVIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSIOSC]     = BSP_CFG_CLOCK_CLK_LSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSIOSC]     = BSP_CFG_CLOCK_CLK_HSIOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_IOSC]       = BSP_CFG_CLOCK_CLK_IOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MOSC]       = BSP_CFG_CLOCK_CLK_MOSC_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_EMG]        = BSP_CFG_CLOCK_CLK_EMG_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_EMCCNT]     = BSP_CFG_CLOCK_CLK_ECMCNT_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK0O]       = BSP_CFG_CLOCK_EXTCLK0O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK1O]       = BSP_CFG_CLOCK_EXTCLK1O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MSPI]       = BSP_CFG_CLOCK_CLK_MSPI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RLIN3]      = BSP_CFG_CLOCK_CLK_RLIN3_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CH23_RLIN3] = BSP_CFG_CLOCK_CLK_CH23_RLIN3_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_GTM]        = BSP_CFG_CLOCK_CLK_GTM_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_WDTB]       = BSP_CFG_CLOCK_CLK_WDTB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_C]    = BSP_CFG_CLOCK_CLK_CANFD_C_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_XIN]  = BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_CPU]       = BSP_CFG_CLOCK_CLKC_CPU_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_SBUS]      = BSP_CFG_CLOCK_CLKC_SBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_HBUS]      = BSP_CFG_CLOCK_CLKC_HBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_UHSB]      = BSP_CFG_CLOCK_CLKC_UHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_SHSB]      = BSP_CFG_CLOCK_CLKC_SHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_HSB]       = BSP_CFG_CLOCK_CLKC_HSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_LSB]       = BSP_CFG_CLOCK_CLKC_LSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CPU]        = BSP_CFG_CLOCK_CLK_CPU_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_SBUS]       = BSP_CFG_CLOCK_CLK_SBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HBUS]       = BSP_CFG_CLOCK_CLK_HBUS_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_UHSB]       = BSP_CFG_CLOCK_CLK_UHSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_HSB]        = BSP_CFG_CLOCK_CLK_HSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_LSB]        = BSP_CFG_CLOCK_CLK_LSB_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_SWDT]      = BSP_CFG_CLOCK_CLKC_SWDT_HZ;
#if BSP_FEATURE_DFP_IS_AVAILABLE
    g_clock_freq[FSP_PRIV_CLOCK_CLK_DFP]       = BSP_CFG_CLOCK_CLK_DFP_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_DFP_DIV_2] = BSP_CFG_CLOCK_CLK_DFP_DIV_2_HZ;
#endif                                 /* #if BSP_FEATURE_DFP_IS_AVAILABLE */
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

    /* Check if the system clock is selected as PLLO/SSCGO/SSCG10 */
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
    }

    /* Check if the configured system clock is PLLO/SSCGO/SSCG1O */
    /* Gear Up/Down the PLLO */
    bsp_gear_up_down_pllo(BSP_CFG_CLOCK_CKS_CLEANC_SRC, BSP_CFG_CLOCK_CLK_PLL_HZ);

    R_SYS0_CLOCK->SSCGC = ((BSP_CFG_CLK_SSCG_MODULATION_MODE << R_SYS0_CLOCK_SSCGC_SSMODE1_Pos) |   \
                           (BSP_CFG_CLK_SSCG_MODULATION_VALUE << R_SYS0_CLOCK_SSCGC_SELMFREQ_Pos) | \
                           (BSP_CFG_CLK_SSCG_MODULATION_DITHERED_RANGE));

    /* Gear Up/Down the SSCGO/SSCG1O */
    bsp_gear_up_down_sscg(BSP_CFG_CLOCK_CKS_SSCGC_SRC,
                          (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC),
                          BSP_CFG_CLOCK_CLK_SSCG_HZ);

#if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ
    R_SYS0_CLOCK->SSCG1C = ((BSP_CFG_CLK_SSCG1_MODULATION_MODE << R_SYS0_CLOCK_SSCG1C_SSMODE1_Pos) |   \
                            (BSP_CFG_CLK_SSCG1_MODULATION_VALUE << R_SYS0_CLOCK_SSCG1C_SELMFREQ_Pos) | \
                            (BSP_CFG_CLK_SSCG1_MODULATION_DITHERED_RANGE));

    bsp_gear_up_down_sscg(BSP_CFG_CLOCK_CKS_SSCG1C_SRC,
                          (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCG1C),
                          BSP_CFG_CLOCK_CLK_SSCG1_HZ);
#endif                                 /* #if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ */

    /* CLK_RLIN3 divider setting and CLK_RLIN3_CH23 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = ((BSP_CFG_CLOCK_CLK_CH23_RLIN3_SRC << 16) | (BSP_CFG_CLOCK_CLK_RLIN3_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, R_SYS0_CLOCK->CKS_RLINC);

    /* CLK_CANDF_C selector setting */
    R_SYS0_CLOCK->CKS_RCANC = ((BSP_CFG_CLOCK_CLK_CANFD_C_SRC << 16) | (BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RCANS, R_SYS0_CLOCK->CKS_RCANC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = BSP_CFG_CLOCK_CLK_MSPI_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, R_SYS0_CLOCK->CKS_MSPIC);

    /* CLK_SWDT selector setting */
    R_SYS0_CLOCK->CKS_SWDTC = BSP_CFG_CLOCK_CLKC_SWDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_SWDTS, R_SYS0_CLOCK->CKS_SWDTC);

#if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, (R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, R_SYS0_A_CLOCK->CKS_EXTCLK0C);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S_b.EXTCLK0SYNC, R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk);
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S,
                               (R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk | \
                                R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0CLKACT_Msk));
#endif                                 /* #if BSP_CFG_CLOCK_EXTCLK0O_DIV > 0 */

#if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, (R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, R_SYS0_A_CLOCK->CKS_EXTCLK1C);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S_b.EXTCLK1SYNC, R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk);
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S,
                               (R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk | \
                                R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1CLKACT_Msk));
#endif                                 /* #if BSP_CFG_CLOCK_EXTCLK1O_DIV > 0 */

    /* CLK_WDTB selector setting */
    R_SYS0_A_CLOCK->CKS_WDTC = BSP_CFG_CLOCK_CLK_WDTB_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_WDTS, R_SYS0_A_CLOCK->CKS_WDTC);

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
    SYNCP();
}

/*******************************************************************************************************************//**
 * Gear Down Clock.
 **********************************************************************************************************************/
void bsp_clock_gear_down (void)
{
    /* Gear Down the PLLO */
    bsp_gear_up_down_pllo(1U, BSP_CFG_CLOCK_CLK_PLL_HZ);

    /* Gear Down the SSCGO/SSCG1O */
    bsp_gear_up_down_sscg(1U, (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC), BSP_CFG_CLOCK_CLK_SSCG_HZ);

#if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ
    bsp_gear_up_down_sscg(1U, (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCG1C), BSP_CFG_CLOCK_CLK_SSCG1_HZ);
#endif                                 /* #if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ */
}

/*******************************************************************************************************************//**
 * System Clock (PPLO) Gear Up.
 **********************************************************************************************************************/
static void bsp_gear_up_down_pllo (bool gear_up_down_select, uint32_t pll_clock_hz)
{
    uint8_t repetition_count     = 0;
    uint8_t repetition_start     = 0;
    uint8_t total_of_repetitions = 0;

    if (gear_up_down_select)
    {
        /* Get the total of repetitions */
        total_of_repetitions = (sizeof(g_gear_down_repetitions_arr) / sizeof(g_gear_down_repetitions_arr[0]));

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0UL == R_SYS0_CLOCK->CKS_CLEANS) && (0UL == R_SYS0_CLOCK->CKD_PLLC) && \
            (R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk == R_SYS0_CLOCK->CKD_PLLS))
        {
            /* Start the repetitions 5 repetitions (800/640MHz)/ 4 repetitions (480MHz) */
            if (BSP_CLOCK_SYSTEM_480MHz != pll_clock_hz)
            {
                total_of_repetitions = (uint8_t) (total_of_repetitions - 1U);
            }

            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to PLL0 */
                R_SYS0_CLOCK->CKD_PLLC = g_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL0 dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS, R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            /* Change the clock source for the system clock to CLK_IOSC */
            R_SYS0_CLOCK->CKS_CLEANC = BSP_CFG_CLOCK_CKS_CLEANC_SRC;

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CLEANS, R_SYS0_CLOCK->CKS_CLEANC);
        }
    }
    else
    {
        /* Get the total of repetitions */
        total_of_repetitions = (sizeof(g_gear_up_repetitions_arr) / sizeof(g_gear_up_repetitions_arr[0]));

        /* Shifting the System Clock Gear Up for System Clock */
        /* Division ratio of clock source CLK_SSCGO are changed from 1 to 3/8 (800/640MHz) or 4/8 (480MHz)*/
        if (BSP_CLOCK_SYSTEM_480MHz != pll_clock_hz)
        {
            R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_6_DIV_16;
        }
        else
        {
            R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_8_DIV_16;
        }

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS, 0x02U);

        /* Setting clock source for */
        R_SYS0_CLOCK->CKS_CLEANC = BSP_CFG_CLOCK_CKS_CLEANC_SRC;

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CLEANS, R_SYS0_CLOCK->CKS_CLEANC);

        /* Start the repetitions 5 repetitions (800/640MHz)/ 4 repetitions (480MHz) */
        if (BSP_CLOCK_SYSTEM_480MHz != pll_clock_hz)
        {
            repetition_start = 1U;
        }

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to PLL0 */
            R_SYS0_CLOCK->CKD_PLLC = g_gear_up_repetitions_arr[repetition_count];

            /* Confirm that PLL0 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS, R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
}

/*******************************************************************************************************************//**
 * System Clock (SSCGO/SSCG1O) Gear Up/Down.
 **********************************************************************************************************************/
static void bsp_gear_up_down_sscg (bool                      gear_up_down_select,
                                   st_systemclock_sscg_ctl * sscg_register,
                                   uint32_t                  sscg_clock_hz)
{
    uint8_t repetition_count     = 0;
    uint8_t repetition_start     = 0;
    uint8_t total_of_repetitions = 0;

    if (gear_up_down_select)
    {
        /* Get the total of repetitions */
        total_of_repetitions = (sizeof(g_gear_down_repetitions_arr) / sizeof(g_gear_down_repetitions_arr[0]));

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0UL == sscg_register->CKS_SSCGS) && \
            (0UL == sscg_register->CKD_SSCGC) && \
            (R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk == sscg_register->CKD_SSCGS))
        {
            /* Start the repetitions 5 repetitions (800/640MHz)/ 4 repetitions (480MHz) */
            if (BSP_CLOCK_SYSTEM_480MHz != sscg_clock_hz)
            {
                total_of_repetitions = (uint8_t) (total_of_repetitions - 1U);
            }

            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to SSCGO/SSCG1O */
                sscg_register->CKD_SSCGC = g_gear_down_repetitions_arr[repetition_count];

                /* Confirm that SSCGO/SSCG1O dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
            }

            /* Change the clock source for the system clock to CLK_IOSC */
            sscg_register->CKS_SSCGC = BSP_CFG_CLOCK_CKS_CLEANC_SRC;

            /* Confirm that CLK_IOSC have been selected */
            FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKS_SSCGS, sscg_register->CKS_SSCGC);
        }
    }
    else
    {
        /* Get the total of repetitions */
        total_of_repetitions = (sizeof(g_gear_up_repetitions_arr) / sizeof(g_gear_up_repetitions_arr[0]));

        /* Shifting the System Clock Gear Up for System Clock */
        /* Division ratio of clock source CLK_SSCGO are changed from 1 to 3/8 (800/640MHz) or 4/8 (480MHz)*/
        if (BSP_CLOCK_SYSTEM_480MHz != sscg_clock_hz)
        {
            sscg_register->CKD_SSCGC = BSP_CLOCK_DIVIDED_BY_6_DIV_16;
        }
        else
        {
            sscg_register->CKD_SSCGC = BSP_CLOCK_DIVIDED_BY_8_DIV_16;
        }

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, 0x02U);

        /* Setting clock source for */
        sscg_register->CKS_SSCGC = BSP_CFG_CLOCK_CKS_SSCGC_SRC;

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKS_SSCGS, sscg_register->CKS_SSCGC);

        /* Start the repetitions 5 repetitions (800/640MHz)/ 4 repetitions (480MHz) */
        if (BSP_CLOCK_SYSTEM_480MHz != sscg_clock_hz)
        {
            repetition_start = 1U;
        }

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to SSCGO/SSCG1O */
            sscg_register->CKD_SSCGC = g_gear_up_repetitions_arr[repetition_count];

            /* Confirm that SSCGO/SSCG1O dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
}

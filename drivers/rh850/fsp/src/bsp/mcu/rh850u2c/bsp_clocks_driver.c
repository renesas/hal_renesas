/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
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
    BSP_CFG_CLOCK_CLK_LSIOSC_HZ,       ///< Initial value of CLK_LSIOSC clock
    BSP_CFG_CLOCK_CLK_HSIOSC_HZ,       ///< Initial value of CLK_HSIOSC clock
    BSP_CFG_CLOCK_CLK_IOSC_HZ,         ///< Initial value of CLK_IOSC clock
    BSP_CFG_CLOCK_CLK_SYS_IOSC_HZ,     ///< Initial value of CLK_SYS_IOSC clock
    BSP_CFG_CLOCK_CLK_EMG_HZ,          ///< Initial value of CLK_EMG clock
    BSP_CFG_CLOCK_CLK_MOSC_HZ,         ///< Initial value of CLK_MOSC clock
    BSP_CFG_CLOCK_CLK_SOSC_HZ,         ///< Initial value of CLK_SOSC clock
    BSP_CFG_CLOCK_CLK_PLL_HZ,          ///< Initial value of CLK_PLL clock
    BSP_CFG_CLOCK_CLK_SSCG_HZ,         ///< Initial value of CLK_SSCG clock
    BSP_CFG_CLOCK_CLK_SYS_CLEAN_HZ,    ///< Initial value of CLK_SYS_CLEAN clock
    BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ,     ///< Initial value of CLK_SYS_SSCG clock
    BSP_CFG_CLOCK_CLK_CPU_HZ,          ///< Initial value of CLK_CPU clock
    BSP_CFG_CLOCK_CLK_SBUS_HZ,         ///< Initial value of CLK_SBUS clock
    BSP_CFG_CLOCK_CLK_HBUS_HZ,         ///< Initial value of CLK_HBUS clock
    BSP_CFG_CLOCK_CLK_UHSB_HZ,         ///< Initial value of CLK_UHSB clock
    BSP_CFG_CLOCK_CLK_HSB_HZ,          ///< Initial value of CLK_HSB clock
    BSP_CFG_CLOCK_CLK_LSB_HZ,          ///< Initial value of CLK_LSB clock
    BSP_CFG_CLOCK_CLKC_SBUS_HZ,        ///< Initial value of CLKC_SBUS clock
    BSP_CFG_CLOCK_CLKC_HBUS_HZ,        ///< Initial value of CLKC_HBUS clock
    BSP_CFG_CLOCK_CLKC_UHSB_HZ,        ///< Initial value of CLKC_UHSB clock
    BSP_CFG_CLOCK_CLKC_HSB_HZ,         ///< Initial value of CLKC_HSB clock
    BSP_CFG_CLOCK_CLKC_LSB_HZ,         ///< Initial value of CLKC_LSB clock
    BSP_CFG_CLOCK_CLK_FLI_HZ,          ///< Initial value of CLK_FLI clock
    BSP_CFG_CLOCK_CLK_FACI_HZ,         ///< Initial value of CLK_FACI clock
    BSP_CFG_CLOCK_CLKA_LPS_HZ,         ///< Initial value of CLKA_LPS clock
    BSP_CFG_CLOCK_CLKA_WDTBA_HZ,       ///< Initial value of CLKA_WDTBA clock
    BSP_CFG_CLOCK_CLKA_TAUJ_HZ,        ///< Initial value of CLKA_TAUJ clock
    BSP_CFG_CLOCK_CLKA_ADC_HZ,         ///< Initial value of CLKA_ADC clock
    BSP_CFG_CLOCK_EXTCLK0O_HZ,         ///< Initial value of EXTCLK0O clock
    BSP_CFG_CLOCK_EXTCLK1O_HZ,         ///< Initial value of EXTCLK1O clock
    BSP_CFG_CLOCK_CLK_MSPI_HZ,         ///< Initial value of CLK_MSPI clock
    BSP_CFG_CLOCK_CLK_RLIN3_HZ,        ///< Initial value of CLK_RLIN3 clock
    BSP_CFG_CLOCK_CLK_RLIN323_HZ,      ///< Initial value of CLK_RLIN323 clock
    BSP_CFG_CLOCK_CLK_CANFD_C_HZ,      ///< Initial value of CLK_CANFD_C clock
    BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ,    ///< Initial value of CLK_CANFD_XIN clock
    BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ,   ///< Initial value of CLKC_CANXL_CH0 clock
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
    BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ,   ///< Initial value of CLKC_CANXL_CH1 clock
#endif /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */
    BSP_CFG_CLOCK_CLK_WDTB_HZ,         ///< Initial value of CLK_WDTB clock
    BSP_CFG_CLOCK_CLK_GTM_HZ,          ///< Initial value of CLK_GTM clock
    BSP_CFG_CLOCK_CLK_ECMCNT_HZ,       ///< Initial value of CLK_ECMCNT clock
    BSP_CFG_CLOCK_CLK_SWDT_HZ,         ///< Initial value of CLK_SWDT clock
};

/** PLL gear up sequence */
static const uint32_t g_pll_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_4_DIV_16,     ///< Clock divided by 4
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 640 MHz gear up sequence */
static const uint32_t g_sscg_640_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_5_DIV_16,     ///< Clock divided by 5
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,     ///< Clock divided by 7
    BSP_CLOCK_DIVIDED_BY_9_DIV_16,     ///< Clock divided by 9
    BSP_CLOCK_DIVIDED_BY_11_DIV_16,    ///< Clock divided by 11
    BSP_CLOCK_DIVIDED_BY_13_DIV_16,    ///< Clock divided by 13
    BSP_CLOCK_DIVIDED_BY_15_DIV_16,    ///< Clock divided by 15
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 480 MHz gear up sequence */
static const uint32_t g_sscg_480_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,     ///< Clock divided by 7
    BSP_CLOCK_DIVIDED_BY_9_DIV_16,     ///< Clock divided by 9
    BSP_CLOCK_DIVIDED_BY_11_DIV_16,    ///< Clock divided by 11
    BSP_CLOCK_DIVIDED_BY_13_DIV_16,    ///< Clock divided by 13
    BSP_CLOCK_DIVIDED_BY_15_DIV_16,    ///< Clock divided by 15
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 320 MHz gear up sequence */
static const uint32_t g_sscg_320_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 240 MHz gear up sequence */
static const uint32_t g_sscg_240_gear_up_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_16_DIV_16,    ///< Clock divided by 16
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** PLL gear down sequence */
static const uint32_t g_pll_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_4_DIV_16,     ///< Clock divided by 4
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 640 MHz gear down sequence */
static const uint32_t g_sscg_640_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_6_DIV_16,     ///< Clock divided by 6
    BSP_CLOCK_DIVIDED_BY_5_DIV_16,     ///< Clock divided by 5
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 480 MHz gear down sequence */
static const uint32_t g_sscg_480_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_DIVIDED_BY_8_DIV_16,     ///< Clock divided by 8
    BSP_CLOCK_DIVIDED_BY_7_DIV_16,     ///< Clock divided by 7
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 320 MHz gear down sequence */
static const uint32_t g_sscg_320_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_14_DIV_16,    ///< Clock divided by 14
    BSP_CLOCK_DIVIDED_BY_10_DIV_16,    ///< Clock divided by 10
    BSP_CLOCK_NO_DIVISION              ///< No division
};

/** SSCG 240 MHz gear down sequence */
static const uint32_t g_sscg_240_gear_down_repetitions_arr[] =
{
    BSP_CLOCK_DIVIDED_BY_12_DIV_16,    ///< Clock divided by 12
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
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK0O]       = BSP_CFG_CLOCK_EXTCLK0O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_EXTCLK1O]       = BSP_CFG_CLOCK_EXTCLK1O_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MSPI]       = BSP_CFG_CLOCK_CLK_MSPI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_MSPI]       = BSP_CFG_CLOCK_CLK_MSPI_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_RLIN323]    = BSP_CFG_CLOCK_CLK_RLIN323_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_C]    = BSP_CFG_CLOCK_CLK_CANFD_C_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLK_CANFD_XIN]  = BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ;
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_CANXL_CH0] = BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ;
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
    g_clock_freq[FSP_PRIV_CLOCK_CLKC_CANXL_CH1] = BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ;
#endif                                 /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */
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

#if (BSP_CFG_CLOCK_CLK_SOSC_HZ > 0)

    /* Confirm Sub OSC is stopped */
    if (0UL == R_SYS0_A_CLOCK->SOSCS)
    {
        R_SYS0_A_CLOCK->SOSCE = R_SYS0_A_CLOCK_SOSCE_SOSCENTRG_Msk;

        /* Confirm that the Sub OSC has been stable
         * (SOSCS.SOSCEN = 1 & SOSCS.SOSCSTAB = 1)*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x03);
    }

#else

    /* Confirm that the Sub OSC is stable */
    if (1UL == R_SYS0_A_CLOCK->SOSCS_b.SOSCSTAB)
    {
        R_SYS0_A_CLOCK->SOSCE = R_SYS0_A_CLOCK_SOSCE_SOSCDISTRG_Msk;

        /* Confirm that the Sub OSC has been stopped (SOSCS.SOSCEN = 0 & SOSCS.SOSCSTAB = 0).*/
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x00);
    }
#endif                                 /* End of (BSP_CFG_CLOCK_CLK_SOSC_HZ > 0) */

    /* Check if the system clock is selected as PLLO/SSCGO */
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

    /* Gear Up/Down the SSCGO */
    bsp_gear_up_down_sscg(BSP_CFG_CLOCK_CKS_SSCGC_SRC,
                          (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC),
                          BSP_CFG_CLOCK_CLK_SSCG_HZ);

    /* CLK_RLIN3 divider setting and CLK_RLIN3_CH23 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = ((BSP_CFG_CLOCK_CLK_RLIN323_SRC << 16) | (BSP_CFG_CLOCK_CLK_RLIN3_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, R_SYS0_CLOCK->CKS_RLINC);

    /* CLK_CANDF_C selector setting */
    R_SYS0_CLOCK->CKS_RCANC = ((BSP_CFG_CLOCK_CLK_CANFD_C_SRC << 16) | (BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RCANS, R_SYS0_CLOCK->CKS_RCANC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = BSP_CFG_CLOCK_CLK_MSPI_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, R_SYS0_CLOCK->CKS_MSPIC);

    /* CLK_SWDT selector setting */
    R_SYS0_CLOCK->CKS_SWDTC = BSP_CFG_CLOCK_CLK_SWDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_SWDTS, R_SYS0_CLOCK->CKS_SWDTC);

    /* CLKC_CANXL_CH0 selector setting */
    R_SYS0_CLOCK->CKS_CANXL0C = BSP_CFG_CLOCK_CLKC_CANXL_CH0_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CANXL0S, R_SYS0_CLOCK->CKS_CANXL0C);

#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)

    /* CLKC_CANXL_CH1 selector setting */
    R_SYS0_CLOCK->CKS_CANXL1C = BSP_CFG_CLOCK_CLKC_CANXL_CH1_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CANXL1S, R_SYS0_CLOCK->CKS_CANXL1C);
#endif                                 /* #if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE) */

    /* CLKA_WDTBA selector setting */
    R_SYS0_A_CLOCK->CKS_AWDTC = BSP_CFG_CLOCK_CLKA_WDTBA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AWDTS, R_SYS0_A_CLOCK->CKS_AWDTC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKS_ATAUJC = BSP_CFG_CLOCK_CLKA_TAUJ_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS, R_SYS0_A_CLOCK->CKS_ATAUJC);

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKS_ARTCAC = BSP_CFG_CLOCK_CLKA_RTCA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, R_SYS0_A_CLOCK->CKS_ARTCAC);

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKS_AADCC = BSP_CFG_CLOCK_CLKA_ADC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS, R_SYS0_A_CLOCK->CKS_AADCC);

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

    /* Gear Down the SSCGO */
    bsp_gear_up_down_sscg(1U, (st_systemclock_sscg_ctl *) &(R_SYS0_CLOCK->CKS_SSCGC), BSP_CFG_CLOCK_CLK_SSCG_HZ);
}

/*******************************************************************************************************************//**
 * System Clock (PPLO) Gear Up.
 **********************************************************************************************************************/
static void bsp_gear_up_down_pllo (bool gear_up_down_select, uint32_t pll_clock_hz)
{
    uint8_t repetition_count     = 0;
    uint8_t repetition_start     = 1;
    uint8_t total_of_repetitions = 0;

    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(pll_clock_hz);

    if (gear_up_down_select)
    {
        /* Get the total of repetitions */
        total_of_repetitions = (sizeof(g_pll_gear_down_repetitions_arr) / sizeof(g_pll_gear_down_repetitions_arr[0]));

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0UL == R_SYS0_CLOCK->CKS_CLEANS) && (0UL == R_SYS0_CLOCK->CKD_PLLC) && \
            (R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk == R_SYS0_CLOCK->CKD_PLLS))
        {
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to PLL0 */
                R_SYS0_CLOCK->CKD_PLLC = g_pll_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL0 dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS, R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            /* Division ratio of clock source HSIOSC is changed to 1/1 */
            /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
            if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
            {
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
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
        total_of_repetitions = (sizeof(g_pll_gear_up_repetitions_arr) / sizeof(g_pll_gear_up_repetitions_arr[0]));

        /* Shifting the System Clock Gear Up for System Clock */
        R_SYS0_CLOCK->CKD_PLLC = BSP_CLOCK_DIVIDED_BY_4_DIV_16;

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_PLLS, 0x02U);

        /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
        if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
        {
            if (2U == R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
            {
                /* 01B if current CKD_HSIOSC setting is 10B (DIV10) */
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 1U;

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* 00B if current CKD_HSIOSC setting is 01B (DIV2) */
            R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;

            /* Verify that the value of HSIOSCCLKDSYNC is 1B */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
        }

        /* Setting clock source for */
        R_SYS0_CLOCK->CKS_CLEANC = BSP_CFG_CLOCK_CKS_CLEANC_SRC;

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_CLEANS, R_SYS0_CLOCK->CKS_CLEANC);

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to PLL0 */
            R_SYS0_CLOCK->CKD_PLLC = g_pll_gear_up_repetitions_arr[repetition_count];

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
    uint8_t          repetition_count     = 0;
    uint8_t          repetition_start     = 1;
    uint8_t          total_of_repetitions = 0;
    const uint32_t * repetitions_arr;

    if (gear_up_down_select)
    {
        /* Get the total of repetitions */
        switch (sscg_clock_hz)
        {
            case BSP_CLOCK_SYSTEM_640MHz:
            {
                repetitions_arr      = &g_sscg_640_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_640_gear_down_repetitions_arr) / sizeof(g_sscg_640_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_480MHz:
            {
                repetitions_arr      = &g_sscg_480_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_480_gear_down_repetitions_arr) / sizeof(g_sscg_480_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_320MHz:
            {
                repetitions_arr      = &g_sscg_320_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_320_gear_down_repetitions_arr) / sizeof(g_sscg_320_gear_down_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_240MHz:
            {
                repetitions_arr      = &g_sscg_240_gear_down_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_240_gear_down_repetitions_arr) / sizeof(g_sscg_240_gear_down_repetitions_arr[0]));
                break;
            }

            default:
            {
                break;
            }
        }

        /* Check if Clock source is other than CLK_IOSC, Clock divider is 0x00, and clock divider synchronous is stable */
        if ((0UL == sscg_register->CKS_SSCGS) && \
            (0UL == sscg_register->CKD_SSCGC) && \
            (R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk == sscg_register->CKD_SSCGS))
        {
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the Divider value to SSCGO */
                sscg_register->CKD_SSCGC = repetitions_arr[repetition_count];

                /* Confirm that SSCGO/SSCG1O dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
            }

            /* Division ratio of clock source HSIOSC is changed to 1/1 */
            /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
            if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
            {
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
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
        switch (sscg_clock_hz)
        {
            case BSP_CLOCK_SYSTEM_640MHz:
            {
                repetitions_arr      = &g_sscg_640_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_640_gear_up_repetitions_arr) / sizeof(g_sscg_640_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_480MHz:
            {
                repetitions_arr      = &g_sscg_480_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_480_gear_up_repetitions_arr) / sizeof(g_sscg_480_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_320MHz:
            {
                repetitions_arr      = &g_sscg_320_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_320_gear_up_repetitions_arr) / sizeof(g_sscg_320_gear_up_repetitions_arr[0]));
                break;
            }

            case BSP_CLOCK_SYSTEM_240MHz:
            {
                repetitions_arr      = &g_sscg_240_gear_up_repetitions_arr[0];
                total_of_repetitions =
                    (sizeof(g_sscg_240_gear_up_repetitions_arr) / sizeof(g_sscg_240_gear_up_repetitions_arr[0]));
                break;
            }

            default:
            {
                break;
            }
        }

        /* Shifting the System Clock Gear Up for System Clock */
        sscg_register->CKD_SSCGC = repetitions_arr[0];

        /* Confirm that CLK_SSCGO dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, 0x02U);

        /* Verify that the value of HSIOSCCLKDCSID[1:0] is not 00 */
        if (0U != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
        {
            if (2U == R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
            {
                /* 01B if current CKD_HSIOSC setting is 10B (DIV10) */
                R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 1U;

                /* Verify that the value of HSIOSCCLKDSYNC is 1B */
                FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
            }

            /* 00B if current CKD_HSIOSC setting is 01B (DIV2) */
            R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;

            /* Verify that the value of HSIOSCCLKDSYNC is 1B */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
        }

        /* Setting clock source for */
        sscg_register->CKS_SSCGC = BSP_CFG_CLOCK_CKS_SSCGC_SRC;

        /* Confirm that PLLO have been selected */
        FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKS_SSCGS, sscg_register->CKS_SSCGC);

        for (repetition_count = repetition_start; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the Divider value to SSCGO/SSCG1O */
            sscg_register->CKD_SSCGC = repetitions_arr[repetition_count];

            /* Confirm that SSCGO/SSCG1O dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(sscg_register->CKD_SSCGS, R_SYS0_CLOCK_CKD_SSCGS_PLLCLKDSYNC_Msk);
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
}

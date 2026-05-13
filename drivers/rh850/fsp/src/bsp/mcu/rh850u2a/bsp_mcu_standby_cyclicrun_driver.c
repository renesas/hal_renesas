/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_mcu_standby_cyclicrun_driver.h"

#if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED)

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
static void bsp_cyclicrun_clock_freq_var_init(void);

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
uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM] =
{
    BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ,   ///< Initial value of CLK_MOSC clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ, ///< Initial value of CLK_HSIOSC clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ, ///< Initial value of CLK_LSIOSC clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HVIOSC_HZ, ///< Initial value of CLK_HVIOSC clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ,   ///< Initial value of CLK_IOSC clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_HZ,    ///< Initial value of CLK_SYS clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ,    ///< Initial value of CLK_CPU clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ,   ///< Initial value of CLK_SBUS clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ,   ///< Initial value of CLK_HBUS clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ,   ///< Initial value of CLK_HBUS clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ,    ///< Initial value of CLK_HSB clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ,    ///< Initial value of CLK_LSB clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ,  ///< Initial value of CLK_RLIN clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ,   ///< Initial value of CLK_MSPI clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ, ///< Initial value of CLK_ECMCNT clock
    BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_HZ,    ///< Initial value of CLKA_WDT clock
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_HZ,   ///< Initial value of CLKA_WDT clock
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ,  ///< Initial value of CLKA_TAUJ clock
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ,  ///< Initial value of CLKA_RTCA clock
 #if (BSP_FEATURE_ADCKA_IS_AVAILABLE)
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ,   ///< Initial value of CLKA_ADC clock
 #endif /* #if (BSP_FEATURE_ADCKA_IS_AVAILABLE) */
    BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ,   ///< Initial value of EXTCLK0O clock
    BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ,   ///< Initial value of EXTCLK1O clock
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ,   ///< Initial value of CLKA_LPS clock
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/* ROM DATA SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA,
                            BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE)

/***********************************************************************************************************************
 * STANDBY CONTROLLER
 **********************************************************************************************************************/

/** Array contains all Module standby informations that is availalbe in Cyclic RUN Mode  */
const bsp_module_standby_info_t g_cyclicrun_msr_info_table[] =
{
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_MSPI] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_MSPI),
    .default_value     = BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MSPI_ENABLE
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_RLIN3] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RLIN3),
    .default_value     = BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RLIN3_ENABLE
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCJ_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_ADCJ_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCJ_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCJ_AWO_ENABLE
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_RTCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_RTCA),
    .default_value     = BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RTCA_ENABLE
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_TAUJ_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_TAUJ_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_ENABLE
    },
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_WDTB_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_WDTB_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_WDTB_AWO_ENABLE
    }
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA, BSP_PRAGMA_SECTION_DEFAULT)

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
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_ADC]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK0O]   = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_EXTCLK1O]   = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ;
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLKA_LPS]   = BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ;
}

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 * This function can only be used in Cyclic RUN Mode.
 **********************************************************************************************************************/
void bsp_cyclicrun_clock_driver_init (void)
{
    volatile uint32_t timeout;

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

    /* CLK_RLIN3 divider setting */
    R_SYS0_CLOCK->CKS_RLINC = BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC);

    /* CLKA_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_AWDTC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AWDTS, BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKSC_ATAUJC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC);

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKSC_ARTCAC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS, BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC);

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKSC_AADCC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS, BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC);

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT0C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, (R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT0C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, R_SYS0_A_CLOCK->CKSC_FOUT0C);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S_b.FOUT0SYNC, R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk);
    R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S,
                               (R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0SYNC_Msk | \
                                R_SYS0_A_CLOCK_CLKD_FOUT0S_FOUT0CLKACT_Msk));
 #endif                                /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0 */

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CLKD_FOUT1C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, (R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKSC_FOUT1C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC, R_SYS0_A_CLOCK->CKSC_FOUT1S);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S_b.FOUT1SYNC, R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk);
    R_SYS0_A_CLOCK->CLKD_FOUT1C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S,
                               (R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1SYNC_Msk | \
                                R_SYS0_A_CLOCK_CLKD_FOUT1S_FOUT1CLKACT_Msk));
 #endif                                /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0 */
    /* CLK_WDT selector setting */
    R_SYS0_A_CLOCK->CKSC_WDTC = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_WDTS, BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC);

    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;

    bsp_cyclicrun_clock_freq_var_init();
    __SYNCP();
}

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_DEFAULT)

#endif                                 /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */

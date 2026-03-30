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
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_CRUN_CODE_DATA)

/***********************************************************************************************************************
 * CLOCK CONTROLLER
 **********************************************************************************************************************/

/* This array stores the clock frequency of each bus clock. This section of RAM should not be initialized by the C
 * runtime environment. This is initialized and used in R_BSP_CyclicRunInitClock, which is called before the C runtime
 * environment is initialized. */
uint32_t g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_NUM] =
{
    BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ,    /* Initial value of CLK_LSIOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ,    /* Initial value of CLK_HSIOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ,      /* Initial value of CLK_IOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_IOSC_HZ,  /* Initial value of CLK_SYS_IOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_HZ,       /* Initial value of CLK_EMG clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ,      /* Initial value of CLK_MOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SOSC_HZ,      /* Initial value of CLK_SOSC clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_CLEAN_HZ, /* Initial value of CLK_SYS_CLEAN clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_SSCG_HZ,  /* Initial value of CLK_SYS_SSCG clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ,       /* Initial value of CLK_CPU clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ,      /* Initial value of CLK_SBUS clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ,      /* Initial value of CLK_HBUS clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ,      /* Initial value of CLK_UHSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ,       /* Initial value of CLK_HSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ,       /* Initial value of CLK_LSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKC_SBUS_HZ,     /* Initial value of CLKC_SBUS clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKC_HBUS_HZ,     /* Initial value of CLKC_HBUS clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKC_UHSB_HZ,     /* Initial value of CLKC_UHSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKC_HSB_HZ,      /* Initial value of CLKC_HSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKC_LSB_HZ,      /* Initial value of CLKC_LSB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ,      /* Initial value of CLKA_LPS clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_HZ,    /* Initial value of CLKA_WDTBA clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ,     /* Initial value of CLKA_TAUJ clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ,      /* Initial value of CLKA_ADC clock */
    BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ,      /* Initial value of EXTCLK0O clock */
    BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ,      /* Initial value of EXTCLK1O clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ,      /* Initial value of CLK_MSPI clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ,     /* Initial value of CLK_RLIN3 clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_HZ,   /* Initial value of CLK_RLIN323 clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_HZ,      /* Initial value of CLK_WDTB clock */
    BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ,    /* Initial value of CLK_ECMCNT clock */
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/* ROM DATA SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_ROM_DATA, BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA)

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
    [BSP_CYCLICRUN_MODULE_STANDBY_MSR_ADCK_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_ADCK_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_AWO_ENABLE
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
    g_clock_cyclicrun_freq[FSP_PRIV_CLOCK_CYCLICRUN_CLK_MSPI]      = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ;
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

 #if (BSP_CFG_CLOCK_CYCLICRUN_SUBOSC_HZ > 0)

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
 #endif                                /* End of (BSP_CFG_CLOCK_CYCLICRUN_SUBOSC_HZ > 0) */

    /* CLK_RLIN3 divider setting and CLK_RLIN3_CH23 divider setting */
    R_SYS0_CLOCK->CKS_RLINC =
        ((BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_SRC << 16) | (BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC));
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, R_SYS0_CLOCK->CKS_RLINC);

    /* CLK_MSPI selector setting */
    R_SYS0_CLOCK->CKS_MSPIC = BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, R_SYS0_CLOCK->CKS_MSPIC);

    /* CLKA_WDTBA selector setting */
    R_SYS0_A_CLOCK->CKS_AWDTC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AWDTS, R_SYS0_A_CLOCK->CKS_AWDTC);

    /* CLKA_TAUJ selector setting */
    R_SYS0_A_CLOCK->CKS_ATAUJC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS, R_SYS0_A_CLOCK->CKS_ATAUJC);

    /* CLKA_RTCA selector setting */
    R_SYS0_A_CLOCK->CKS_ARTCAC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, R_SYS0_A_CLOCK->CKS_ARTCAC);

    /* CLKA_ADC selector setting */
    R_SYS0_A_CLOCK->CKS_AADCC = BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS, R_SYS0_A_CLOCK->CKS_AADCC);

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, (R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk));

    /* EXTCLK0 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK0C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, R_SYS0_A_CLOCK->CKS_EXTCLK0C);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S_b.EXTCLK0SYNC, R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk);
    R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S,
                               (R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0SYNC_Msk | \
                                R_SYS0_A_CLOCK_CKD_EXTCLK0S_EXTCLK0CLKACT_Msk));
 #endif                                /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV > 0 */

 #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0

    /* Make sure the EXTCLK0O is stopped before setting the new clock source */
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0U;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, (R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk));

    /* EXTCLK1 selector and divider setting */
    R_SYS0_A_CLOCK->CKS_EXTCLK1C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, R_SYS0_A_CLOCK->CKS_EXTCLK1C);

    /* Confirm clock divider is stable */
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S_b.EXTCLK1SYNC, R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk);
    R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S,
                               (R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1SYNC_Msk | \
                                R_SYS0_A_CLOCK_CKD_EXTCLK1S_EXTCLK1CLKACT_Msk));
 #endif                                /* #if BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV > 0 */

    /* CLK_WDTB selector setting */
    R_SYS0_A_CLOCK->CKS_WDTC = BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_SRC;
    FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_WDTS, R_SYS0_A_CLOCK->CKS_WDTC);

    /* Enable write protect of clock controller */
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;

    bsp_cyclicrun_clock_freq_var_init();
    SYNCP();
}

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_DEFAULT)

#endif                                 /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */

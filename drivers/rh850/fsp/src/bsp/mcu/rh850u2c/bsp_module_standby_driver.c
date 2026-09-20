/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "bsp_module_standby_driver.h"

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
 * Private global variables and functions
 **********************************************************************************************************************/

/** Array contains all Module standby information */
const bsp_module_standby_info_t g_msr_info_table[] =
{
    [BSP_MODULE_STANDBY_MSR_RSCFD] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_RSCFD), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RSCFD_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSCFD_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_FLXA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_FLXA), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_FLXA_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_FLXA_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#endif                                                                   /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [BSP_MODULE_STANDBY_MSR_GTM] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_GTM),  /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_GTM_DEFAULT_VALUE,       /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_GTM_ENABLE               /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_ETND] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_ETND),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ETND_DEFAULT_VALUE,          /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ETND_ENABLE                  /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#endif                                                                       /* #if (BSP_FEATURE_ETND_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [BSP_MODULE_STANDBY_MSR_ETNF] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_ETNF),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ETNF_DEFAULT_VALUE,          /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ETNF_ENABLE                  /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_RSENT] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_RSENT),    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RSENT_DEFAULT_VALUE,         /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSENT_ENABLE                 /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_MSPI] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_MSPI),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE,          /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MSPI_ENABLE                  /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_RLIN3] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_RLIN3),    /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE,         /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RLIN3_ENABLE                 /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_ADCK_ISO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_ADCK_ISO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_ISO_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_ISO_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_CXPI] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_CXPI),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_CXPI_DEFAULT_VALUE,          /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_CXPI_ENABLE                  /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_MMCA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_MMCA), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_MMCA_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MMCA_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#endif                                                                   /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */ /* PRQA S 2053 # Advisory D-4.4 */
    [BSP_MODULE_STANDBY_MSR_ENCA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_ENCA), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ENCA_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ENCA_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_PSI5] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_PSI5), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_PSI5_DEFAULT_VALUE,      /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5_ENABLE              /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_PSI5S] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_PSI5S),      /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_PSI5S_DEFAULT_VALUE,           /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5S_ENABLE                   /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
#endif                                                                         /* BSP_FEATURE_PSI5S_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_PWMD] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_PWMD),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_PWMD_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PWMD_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_SFMA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_SFMA),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_SFMA_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_SFMA_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_TAPA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_TAPA),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TAPA_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAPA_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_TAUD] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_TAUD),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUD_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUD_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_TAUJ_ISO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_TAUJ_ISO),   /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_ISO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_ISO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_TSG3] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_TSG3),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TSG3_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TSG3_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_OSTM] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_OSTM),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_OSTM_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_OSTM_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_RI3C] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_RI3C),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RI3C_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RI3C_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_SSIF] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_STBC->MSR_SSIF),       /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_SSIF_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_SSIF_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_ADCK_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_ADCK_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_RTCA] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_RTCA),     /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE,            /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RTCA_ENABLE                    /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_TAUJ_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_TAUJ_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    },
    [BSP_MODULE_STANDBY_MSR_WDTB_AWO] =
    {
    .msr_reg_address   = (volatile uint32_t *) &(R_SYS0_A_STBC->MSR_WDTB_AWO), /* PRQA S 0306, 0662 # Advisory R-11.4, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    .default_value     = BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE,        /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_WDTB_AWO_ENABLE                /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */
    }
};

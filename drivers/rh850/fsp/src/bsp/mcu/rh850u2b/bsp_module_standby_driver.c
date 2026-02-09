/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
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

/** Array contains all Module standby informations */
const bsp_module_standby_info_t g_msr_info_table[] =
{
    [BSP_MODULE_STANDBY_MSR_RSCFD] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RSCFD),
    .default_value     = BSP_MODULE_STANDBY_RSCFD_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSCFD_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_FLXA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_FLXA),
    .default_value     = BSP_MODULE_STANDBY_FLXA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_FLXA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_GTM] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_GTM),
    .default_value     = BSP_MODULE_STANDBY_GTM_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_GTM_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_ETN] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ETN),
    .default_value     = BSP_MODULE_STANDBY_ETN_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ETHTSN_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RSENT] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RSENT),
    .default_value     = BSP_MODULE_STANDBY_RSENT_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSENT_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_MSPI] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_MSPI),
    .default_value     = BSP_MODULE_STANDBY_MSPI_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MSPI_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RLIN3] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RLIN3),
    .default_value     = BSP_MODULE_STANDBY_RLIN3_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RLIN3_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_DSADC_CADC] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_DSADC_CADC),
    .default_value     = BSP_MODULE_STANDBY_DSADC_CADC_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_DS_ADC_CADC_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_ADCK_ISO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ADCK_ISO),
    .default_value     = BSP_MODULE_STANDBY_ADCK_ISO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_ISO_ENABLE
    },
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_MMCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_MMCA),
    .default_value     = BSP_MODULE_STANDBY_MMCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MMCA_ENABLE
    },
#endif                                 /* BSP_FEATURE_MMCA_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_ENCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ENCA),
    .default_value     = BSP_MODULE_STANDBY_ENCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ENCA_ENABLE
    },
#if (BSP_FEATURE_PSI5_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_PSI5] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_PSI5),
    .default_value     = BSP_MODULE_STANDBY_PSI5_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5_ENABLE
    },
#endif                                 /* BSP_FEATURE_PSI5_IS_AVAILABLE */
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_PSI5S] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_PSI5S),
    .default_value     = BSP_MODULE_STANDBY_PSI5S_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5S_ENABLE
    },
#endif                                 /* BSP_FEATURE_PSI5S_IS_AVAILABLE */
#if (BSP_FEATURE_RHSIF_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_RHSIF] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RHSIF),
    .default_value     = BSP_MODULE_STANDBY_RHSIF_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RHSIF_ENABLE
    },
#endif                                 /* BSP_FEATURE_RHSIF_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_RHSB] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RHSB),
    .default_value     = BSP_MODULE_STANDBY_RHSB_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RHSB_ENABLE
    },
#if (BSP_FEATURE_RIIC_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_RIIC] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RIIC),
    .default_value     = BSP_MODULE_STANDBY_RIIC_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RIIC_ENABLE
    },
#endif                                 /* BSP_FEATURE_RIIC_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_SFMA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_SFMA),
    .default_value     = BSP_MODULE_STANDBY_SFMA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_SFMA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAPA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TAPA),
    .default_value     = BSP_MODULE_STANDBY_TAPA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAPA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAUD] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TAUD),
    .default_value     = BSP_MODULE_STANDBY_TAUD_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUD_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TPBA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TPBA),
    .default_value     = BSP_MODULE_STANDBY_TPBA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TPBA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TSG3] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TSG3),
    .default_value     = BSP_MODULE_STANDBY_TSG3_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TSG3_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_OSTM] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_OSTM),
    .default_value     = BSP_MODULE_STANDBY_OSTM_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_OSTM_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_ATU] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ATU),
    .default_value     = BSP_MODULE_STANDBY_ATU_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ATU_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_DFE] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_DFE),
    .default_value     = BSP_MODULE_STANDBY_DFE_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_DFE_ENABLE
    },
#if (BSP_FEATURE_DFP_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_DFP] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_DFP),
    .default_value     = BSP_MODULE_STANDBY_DFP_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_DFP_ENABLE
    },
#endif                                 /* BSP_FEATURE_DFP_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_RDC] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RDC),
    .default_value     = BSP_MODULE_STANDBY_RDC_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RDC_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_FCOMP] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_FCOMP),
    .default_value     = BSP_MODULE_STANDBY_FCOMP_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_FCOMP_ENABLE
    },
#if (BSP_FEATURE_EMU_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_EMU] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_EMU),
    .default_value     = BSP_MODULE_STANDBY_EMU_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_EMU_ENABLE
    },
#endif                                 /* #if BSP_FEATURE_EMU_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_HRPWM] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_HRPWM),
    .default_value     = BSP_MODULE_STANDBY_HRPWM_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_HRPWM_ENABLE
    },
#if (BSP_FEATURE_DSMIF_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_DSMIF] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_DSMIF),
    .default_value     = BSP_MODULE_STANDBY_DSMIF_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_DSMIF_ENABLE
    }
#endif                                 /* BSP_FEATURE_DSMIF_IS_AVAILABLE */
};

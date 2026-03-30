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
    .default_value     = BSP_MODULE_STANDBY_MSR_RSCFD_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSCFD_ENABLE
    },
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_FLXA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_FLXA),
    .default_value     = BSP_MODULE_STANDBY_MSR_FLXA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_FLXA_ENABLE
    },
#endif                                 /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE) */
    [BSP_MODULE_STANDBY_MSR_GTM] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_GTM),
    .default_value     = BSP_MODULE_STANDBY_MSR_GTM_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_GTM_ENABLE
    },
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_ETND] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ETND),
    .default_value     = BSP_MODULE_STANDBY_MSR_ETND_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ETND_ENABLE
    },
#endif                                 /* #if (BSP_FEATURE_ETND_IS_AVAILABLE) */
    [BSP_MODULE_STANDBY_MSR_ETNF] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ETNF),
    .default_value     = BSP_MODULE_STANDBY_MSR_ETNF_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ETNF_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RSENT] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RSENT),
    .default_value     = BSP_MODULE_STANDBY_MSR_RSENT_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RSENT_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_MSPI] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_MSPI),
    .default_value     = BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MSPI_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RLIN3] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RLIN3),
    .default_value     = BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RLIN3_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_ADCK_ISO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ADCK_ISO),
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_ISO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_ISO_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_CXPI] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_CXPI),
    .default_value     = BSP_MODULE_STANDBY_MSR_CXPI_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_CXPI_ENABLE
    },
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_MMCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_MMCA),
    .default_value     = BSP_MODULE_STANDBY_MSR_MMCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_MMCA_ENABLE
    },
#endif                                 /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */
    [BSP_MODULE_STANDBY_MSR_ENCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_ENCA),
    .default_value     = BSP_MODULE_STANDBY_MSR_ENCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ENCA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_PSI5] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_PSI5),
    .default_value     = BSP_MODULE_STANDBY_MSR_PSI5_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5_ENABLE
    },
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    [BSP_MODULE_STANDBY_MSR_PSI5S] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_PSI5S),
    .default_value     = BSP_MODULE_STANDBY_MSR_PSI5S_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PSI5S_ENABLE
    },
#endif                                 /* BSP_FEATURE_PSI5S_IS_AVAILABLE */
    [BSP_MODULE_STANDBY_MSR_PWMD] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_PWMD),
    .default_value     = BSP_MODULE_STANDBY_MSR_PWMD_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_PWMD_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_SFMA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_SFMA),
    .default_value     = BSP_MODULE_STANDBY_MSR_SFMA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_SFMA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAPA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TAPA),
    .default_value     = BSP_MODULE_STANDBY_MSR_TAPA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAPA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAUD] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TAUD),
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUD_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUD_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAUJ_ISO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TAUJ_ISO),
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_ISO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_ISO_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TSG3] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_TSG3),
    .default_value     = BSP_MODULE_STANDBY_MSR_TSG3_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TSG3_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_OSTM] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_OSTM),
    .default_value     = BSP_MODULE_STANDBY_MSR_OSTM_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_OSTM_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RI3C] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_RI3C),
    .default_value     = BSP_MODULE_STANDBY_MSR_RI3C_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RI3C_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_SSIF] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_STBC->MSR_SSIF),
    .default_value     = BSP_MODULE_STANDBY_MSR_SSIF_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_SSIF_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_ADCK_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_ADCK_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_ADCK_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_ADCK_AWO_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_RTCA] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_RTCA),
    .default_value     = BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_RTCA_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_TAUJ_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_TAUJ_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_TAUJ_AWO_ENABLE
    },
    [BSP_MODULE_STANDBY_MSR_WDTB_AWO] =
    {
    .msr_reg_address   = (uint32_t *) &(R_SYS0_A_STBC->MSR_WDTB_AWO),
    .default_value     = BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE,
    .initial_cfg_value = BSP_MODULE_STANDBY_MSR_WDTB_AWO_ENABLE
    }
};

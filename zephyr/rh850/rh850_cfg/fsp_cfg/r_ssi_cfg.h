/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup R_SSI
 * @defgroup R_SSI_CFG Build Time Configurations
 * @{
 **********************************************************************************************************************/

#ifndef R_SSI_CFG_H
#define R_SSI_CFG_H

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#ifndef SSI_CFG_PARAM_CHECKING_ENABLE
 #define SSI_CFG_PARAM_CHECKING_ENABLE    (BSP_CFG_PARAM_CHECKING_ENABLE)
#endif

#ifndef SSI_CFG_TRANSFER_ENABLE
 #define SSI_CFG_TRANSFER_ENABLE          (1)
#endif

#endif

/*******************************************************************************************************************//**
 * @} (end defgroup R_SSI_CFG)
 **********************************************************************************************************************/

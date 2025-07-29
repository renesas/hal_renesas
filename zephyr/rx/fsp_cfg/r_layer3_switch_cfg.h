/*
 * Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its
 * affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*******************************************************************************************************************//**
 * @addtogroup LAYER3_SWITCH
 * @{
 **********************************************************************************************************************/

/* Guards against multiple inclusion */
#ifndef R_LAYER3_SWITCH_CFG_H
#define R_LAYER3_SWITCH_CFG_H

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/** SPECIFY WHLAYER3_SWITCH TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * 1 : includes parameter checking
 * 0 : compiles out parameter checking
 */
#ifndef LAYER3_SWITCH_CFG_PARAM_CHECKING_ENABLE
 #define LAYER3_SWITCH_CFG_PARAM_CHECKING_ENABLE    (BSP_CFG_PARAM_CHECKING_ENABLE)
#endif

/** SPECIFY WHLAYER3_SWITCH TO INCLUDE CODE FOR API PARAMETER CHECKING
 * Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting
 * 1 : includes parameter checking
 * 0 : compiles out parameter checking
 */
#ifndef LAYER3_SWITCH_CFG_AVAILABLE_QUEUE_NUM
 #define LAYER3_SWITCH_CFG_AVAILABLE_QUEUE_NUM      (8)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

#endif                                 /* R_LAYER3_SWITCH_CFG_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup LAYER3_SWITCH)
 **********************************************************************************************************************/

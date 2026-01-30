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

#include <zephyr/devicetree.h>

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

#define LAYER3_SWITCH_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)

#define LAYER3_SWITCH_GET_QUEUE_NUM(id, compat, prop)                                  \
		    COND_CODE_1(DT_NODE_HAS_COMPAT(id,compat),(DT_PROP(id,prop)),(0))

#define LAYER3_SWITCH_CFG_AVAILABLE_QUEUE_NUM                                          \
		    (DT_FOREACH_CHILD_SEP_VARGS(DT_NODELABEL(eswm), LAYER3_SWITCH_GET_QUEUE_NUM,   \
						(+), renesas_rx_rmac, txq_num) +          \
		     DT_FOREACH_CHILD_SEP_VARGS(DT_NODELABEL(eswm), LAYER3_SWITCH_GET_QUEUE_NUM,   \
						(+), renesas_rx_rmac, rxq_num))


#endif                                 /* R_LAYER3_SWITCH_CFG_H */

/*******************************************************************************************************************//**
 * @} (end addtogroup LAYER3_SWITCH)
 **********************************************************************************************************************/

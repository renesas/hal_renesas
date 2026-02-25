/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850
 * @defgroup BSP_MCU_RH850U2CX BSP U2CX MCU
 * @includedoc config_bsp_rh850u2cx_fsp.html
 *
 * @{
 **********************************************************************************************************************/

#ifndef BSP_MCU_INFO_H
#define BSP_MCU_INFO_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/* BSP MCU Specific Includes. */
#include "bsp_feature.h"
#include "bsp_irq_info.h"
#include "bsp_dmac.h"
#include "bsp_dtsc.h"
#include "bsp_ecm.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

#ifndef BSP_OVERRIDE_GROUP_IRQ_T

/** Which interrupts can have callbacks registered. */
typedef enum e_bsp_grp_irq
{
    BSP_GRP_IRQ_EXTERNAL_NMI = 0,              ///< External NMI interrupt

    BSP_GRP_IRQ_SECURE_WDT_OVERFLOW_FEINT = 1, ///< Secure WDT overflow FEINT
 #if (BSP_FEATURE_FEINT_CPU0)
    BSP_GRP_IRQ_TPTM_PE0_FEINT = 2,            ///< TPTM PE0 FEINT
    BSP_GRP_IRQ_ECM_PE0_FEINT  = 3,            ///< ECM PE0 FEINT
 #endif
 #if (BSP_FEATURE_FEINT_CPU1)
    BSP_GRP_IRQ_TPTM_PE1_FEINT = 4,            ///< TPTM PE1 FEINT
    BSP_GRP_IRQ_ECM_PE1_FEINT  = 5,            ///< ECM PE1 FEINT
 #endif
    BSP_GRP_IRQ_MAX_INDEX
} bsp_grp_irq_t;

#endif

/* Callback type. */
typedef void (* bsp_grp_irq_cb_t)(bsp_grp_irq_t irq);

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif

/** @} (end defgroup BSP_MCU_RH850U2CX) */

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
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

 #if (BSP_CONFIG_USE_SMP_MODE)
  #define BSP_GRP_IRQ_MAX    (4U)
 #else
  #define BSP_GRP_IRQ_MAX    (6U)
 #endif

/** Which interrupts can have callbacks registered. */
typedef enum e_bsp_grp_irq
{
    BSP_GRP_IRQ_EXTERNAL_NMI = 0,              ///< External NMI interrupt

    BSP_GRP_IRQ_SECURE_WDT_OVERFLOW_FEINT = 1, ///< Secure WDT overflow FEINT
    BSP_GRP_IRQ_TPTM_FEINT                = 2, ///< TPTM PEx FEINT
    BSP_GRP_IRQ_ECM_FEINT                 = 3, ///< ECM PEx FEINT

    BSP_GRP_IRQ_MAX_INDEX = BSP_GRP_IRQ_MAX
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

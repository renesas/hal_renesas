/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * BSP entry header of the Minimal FSP/BSP implementation for R-Car SoC
 * (X5H/R8A78000, Cortex-R52).
 *
 * It provides only the subset required by the supported FSP peripheral
 * drivers (currently: OSTM). Vector tables, clock trees, IO port setup and
 * module stop are not handled by this BSP.
 */

#ifndef BSP_API_H
#define BSP_API_H

/* FSP Common Includes. */
#include "fsp_common_api.h"

/* Gets MCU configuration information. Also defines BSP_API_OVERRIDE. */
#include "bsp_cfg.h"

/* IRQn_Type and interrupt attributes for Cortex-R52 / GIC. */
#include "bsp_exceptions.h"

/* Device register definitions. */
#include "r8a78000.h"

#if defined(BSP_API_OVERRIDE)
 #include BSP_API_OVERRIDE
#else

/* BSP Common Includes. */
 #include "bsp_common.h"

/* BSP SOC Specific Includes. */
 #include "bsp_irq.h"

/* Factory SOC information. */
 #include "bsp_feature.h"
#endif

/* Common macro for FSP header files. */
FSP_HEADER

/* Common macro for FSP header files. */
FSP_FOOTER

#endif                                 /* BSP_API_H */

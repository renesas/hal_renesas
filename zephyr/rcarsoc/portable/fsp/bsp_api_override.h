/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * Replaces the default FSP BSP include set (see bsp_api.h).
 *
 * Keep this list as small as the supported FSP drivers allow.
 */

#ifndef BSP_API_OVERRIDE_H_
#define BSP_API_OVERRIDE_H_

/* BSP Common Includes */
#include "bsp_common.h"

/* Interrupt management. */
#include "bsp_irq.h"

/* Per-device feature macros (valid channel masks, instance counts). */
#include "bsp_feature.h"

#endif                                 /* BSP_API_OVERRIDE_H_ */

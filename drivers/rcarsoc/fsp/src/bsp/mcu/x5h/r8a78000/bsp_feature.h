/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * Per-device feature macros for R-Car Gen5 X5H (R8A78000).
 *
 * Only the entries referenced by the supported FSP drivers are defined here.
 */

#ifndef BSP_FEATURE_H
#define BSP_FEATURE_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * OSTM
 **********************************************************************************************************************/

/*
 * Bitmask of OSTM channels present on this device: bit n set == channel n
 * exists. Checked by r_ostm_open_param_checking().
 */
#define BSP_FEATURE_OSTM_VALID_CHANNEL_MASK    (0x3FU)

/** Number of OSTM channels. Must agree with the mask above. */
#define BSP_FEATURE_OSTM_CHANNEL_COUNT         (6U) 

#ifdef __cplusplus
}
#endif

#endif                                 /* BSP_FEATURE_H */

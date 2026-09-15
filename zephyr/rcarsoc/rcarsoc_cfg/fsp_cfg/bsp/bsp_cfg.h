/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*
 * BSP configuration for R-Car SoC.
 */

#ifndef BSP_CFG_H
#define BSP_CFG_H

#ifndef BSP_API_OVERRIDE
 #define BSP_API_OVERRIDE                 "bsp_api_override.h"
#endif

/* FSP_ASSERT behaviour in bsp_common.h: 0 = return FSP_ERR_ASSERTION, 2 = assert(), 3 = compiled out. */
#ifndef BSP_CFG_ASSERT
 #define BSP_CFG_ASSERT                   (0)
#endif

/* 1 = call fsp_error_log() before returning an error code. */
#ifndef BSP_CFG_ERROR_LOG
 #define BSP_CFG_ERROR_LOG                (0)
#endif

#endif                                 /* BSP_CFG_H */

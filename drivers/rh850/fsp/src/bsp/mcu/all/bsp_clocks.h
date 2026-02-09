/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_CLOCKS_H
#define BSP_CLOCKS_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_clock_cfg.h"
#include "bsp_api.h"
#include "bsp_clocks_driver.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Key code protection value */
#define BSP_PRV_KEYCODE_DISABLE    0xA5A5A500 ///< Key code protection disable value
#define BSP_PRV_KEYCODE_ENABLE     0xA5A5A501 ///< Key code protection enable value

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
void R_BSP_InitClock(void);

/* Get a clock frequency */
uint32_t R_FSP_SystemClockHzGet(fsp_priv_clock_t clock);

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_CLOCKS_H */

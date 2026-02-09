/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BOARD_CFG_H_
#define BOARD_CFG_H_

#if defined(CONFIG_SOC_R7F70255xFABB_C) || defined(CONFIG_SOC_R7F70255xAFABB_C) || defined(CONFIG_SOC_R7F70255xBFABB_C)
 #include "board_cfg_u2b6.h"
#elif defined(CONFIG_SOC_R7F70254xFABG_C) || defined(CONFIG_SOC_R7F70254xFABA_C) || defined(CONFIG_SOC_R7F70254xFABB_C)
    || defined(CONFIG_SOC_R7F70254xAFABG_C)  || defined(CONFIG_SOC_R7F70254xAFABA_C) || defined(CONFIG_SOC_R7F70254xAFABB_C)
 #include "board_cfg_u2b10.h"
#endif

#endif                                 /* BOARD_CFG_H_ */

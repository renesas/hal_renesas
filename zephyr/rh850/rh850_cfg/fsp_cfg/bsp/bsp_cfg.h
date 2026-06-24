/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_CFG_H_
#define BSP_CFG_H_

#if defined(CONFIG_SOC_R7F702300AEBBC_C) || \
    defined(CONFIG_SOC_R7F702300BEBBC_C) || \
    defined(CONFIG_SOC_R7F702300EBBG_C)
 #include "bsp_cfg_u2a16.h"
#elif defined(CONFIG_SOC_R7F70255XFABB_C) || \
    defined(CONFIG_SOC_R7F70255XAFABB_C) || \
    defined(CONFIG_SOC_R7F70255XBFABB_C)
 #include "bsp_cfg_u2b6.h"
#elif defined(CONFIG_SOC_R7F70254XFABG_C) || \
    defined(CONFIG_SOC_R7F70254XFABA_C) || \
    defined(CONFIG_SOC_R7F70254XFABB_C) || \
    defined(CONFIG_SOC_R7F70254XAFABG_C) || \
    defined(CONFIG_SOC_R7F70254XAFABA_C) || \
    defined(CONFIG_SOC_R7F70254XAFABB_C)
 #include "bsp_cfg_u2b10.h"
#elif defined(CONFIG_SOC_R7F702600AFABB_C) || \
	defined(CONFIG_SOC_R7F702600FABB_C)
 #include "bsp_cfg_u2c8.h"
#endif

#ifndef BSP_PE0_USED
 #define BSP_PE0_USED                              (0x1)
#endif

#endif                                 /* BSP_CFG_H_ */

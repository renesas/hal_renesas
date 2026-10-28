/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_TAUD_CFG_H_
#define R_TAUD_CFG_H_

#if defined(CONFIG_SOC_SERIES_RH850U2A)
 #define TAUD_CFG_CLOCK                (BSP_CFG_CLOCK_CLK_HSB_HZ)
#else
 #define TAUD_CFG_CLOCK                (BSP_CFG_CLOCK_CLKC_HSB_HZ)
#endif
#define TAUD_CFG_MULTI_SLAVE_ENABLE    (1)
#endif                                 /* R_TAUD_CFG_H_ */

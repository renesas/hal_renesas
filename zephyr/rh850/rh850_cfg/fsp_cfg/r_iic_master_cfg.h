/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_IIC_MASTER_CFG_H_
#define R_IIC_MASTER_CFG_H_

#if defined(CONFIG_SOC_SERIES_RH850U2A)
 #define IIC_MASTER_CFG_CLOCK    (BSP_CFG_CLOCK_CLK_LSB_HZ)
#else
 #define IIC_MASTER_CFG_CLOCK    (BSP_CFG_CLOCK_CLKC_LSB_HZ)
#endif

#endif                                 /* R_IIC_MASTER_CFG_H_ */

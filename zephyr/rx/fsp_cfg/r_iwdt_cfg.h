/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_IWDT_CFG_H_
#define R_IWDT_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define IWDT_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#define IWDT_CFG_NMI_SUPPORTED ((0))
#define IWDT_CFG_IRQ_SUPPORTED ((CONFIG_WDT_RENESAS_RX_IWDT_IRQ))

#ifdef __cplusplus
}
#endif
#endif /* R_IWDT_CFG_H_ */

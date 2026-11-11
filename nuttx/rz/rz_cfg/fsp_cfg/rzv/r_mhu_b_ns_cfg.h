/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_MHU_B_NS_CFG_H_
#define R_MHU_B_NS_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define MHU_B_NS_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (FSP_NOT_DEFINED) != (FSP_NOT_DEFINED)
      #define BSP_CFG_OPENAMP (1)
      #endif

#ifdef __cplusplus
}
#endif
#endif /* R_MHU_B_NS_CFG_H_ */

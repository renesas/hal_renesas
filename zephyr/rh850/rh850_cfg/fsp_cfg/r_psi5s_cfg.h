/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_PSI5S_CFG_H_
 #define R_PSI5S_CFG_H_
 #ifdef __cplusplus
extern "C" {
 #endif
 #ifndef PSI5S_CFG_PARAM_CHECKING_ENABLE
  #define PSI5S_CFG_PARAM_CHECKING_ENABLE    (BSP_CFG_PARAM_CHECKING_ENABLE)
 #endif

 #ifndef PSI5S_CFG_HSB_CLOCK
  #define PSI5S_CFG_HSB_CLOCK                (80000000U)
 #endif

 #ifndef PSI5S_CFG_USHB_CLOCK
  #define PSI5S_CFG_USHB_CLOCK               (160000000U)
 #endif

 #ifdef __cplusplus
}
 #endif
#endif                                 /* R_PSI5S_CFG_H_ */

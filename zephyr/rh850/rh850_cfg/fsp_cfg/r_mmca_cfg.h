/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_MMCA_CFG_H_
 #define R_MMCA_CFG_H_
 #ifdef __cplusplus
extern "C" {
 #endif

 #ifndef MMCA_CFG_PARAM_CHECKING_ENABLE
  #define MMCA_CFG_PARAM_CHECKING_ENABLE    (BSP_CFG_PARAM_CHECKING_ENABLE)
 #endif

 #ifndef MMCA_CFG_DMAC_ENABLE
  #define MMCA_CFG_DMAC_ENABLE              (0)
 #endif

 #ifndef MMCA_CFG_NON_BLOCK_ENABLE
  #define MMCA_CFG_NON_BLOCK_ENABLE         (0)
 #endif

 #ifndef MMCA_CFG_BOOT_ENABLE
  #define MMCA_CFG_BOOT_ENABLE              (0)
 #endif

 #ifdef __cplusplus
}
 #endif
#endif                                 /* R_MMCA_CFG_H_ */

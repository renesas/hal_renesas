/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_I3C_CFG_H_
 #define R_I3C_CFG_H_
 #ifdef __cplusplus
extern "C" {
 #endif

 #ifndef I3C_CFG_PARAM_CHECKING_ENABLE
  #define I3C_CFG_PARAM_CHECKING_ENABLE       (1)
 #endif

 #ifndef I3C_CFG_UNALIGNED_BUFFER_SUPPORT
  #define I3C_CFG_UNALIGNED_BUFFER_SUPPORT    (1)
 #endif

 #ifndef I3C_CFG_MASTER_SUPPORT
  #define I3C_CFG_MASTER_SUPPORT              (1)
 #endif

 #ifndef I3C_CFG_SLAVE_SUPPORT
  #define I3C_CFG_SLAVE_SUPPORT               (1)
 #endif

 #ifndef I3C_TCLK
  #define I3C_TCLK                            200000000
 #endif

 #ifdef __cplusplus
}
 #endif
#endif                                 /* R_I3C_CFG_H_ */

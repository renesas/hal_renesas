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
  #if defined(CONFIG_SOC_SERIES_RH850U2A)
   #define PSI5S_CFG_HSB_CLOCK               (BSP_CFG_CLOCK_CLK_HSB_HZ)
  #else
   #define PSI5S_CFG_HSB_CLOCK               (BSP_CFG_CLOCK_CLKC_HSB_HZ)
  #endif
 #endif

 #ifndef PSI5S_CFG_USHB_CLOCK
  #if defined(CONFIG_SOC_SERIES_RH850U2A)
   #define PSI5S_CFG_USHB_CLOCK              (BSP_CFG_CLOCK_CLK_UHSB_HZ)
  #else
   #define PSI5S_CFG_USHB_CLOCK              (BSP_CFG_CLOCK_CLKC_UHSB_HZ)
  #endif
 #endif

 #ifdef __cplusplus
}
 #endif
#endif                                 /* R_PSI5S_CFG_H_ */

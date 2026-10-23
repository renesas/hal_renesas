/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_ETHER_PHY_CFG_H_
#define R_ETHER_PHY_CFG_H_

#define ETHER_PHY_CFG_PARAM_CHECKING_ENABLE (0)
#define ETHER_PHY_CFG_INIT_PHY_LSI_AUTOMATIC (0)
#define ETHER_PHY_CFG_BYPASS_ENABLE (0)

#ifndef ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE
 #if defined(CONFIG_USE_RH850_FSP_ETHER_ETNF)
  #define ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE (1)
 #else
  #define ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE (0)
 #endif
#endif

#define ETHER_PHY_CFG_TARGET_LAN8700_ENABLE (0)

#endif                                 /* R_ETHER_PHY_CFG_H_ */

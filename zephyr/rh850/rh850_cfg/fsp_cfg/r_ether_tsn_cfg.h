/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef R_ETHER_CFG_H_
#define R_ETHER_CFG_H_

#ifdef __cplusplus
extern "C" {
#endif

#define ETHER_TSN_CFG_PARAM_CHECKING_ENABLE (0)

#define ETHER_TSN_CFG_GLOBAL_TIME_SUPPORT (0)

#if defined(CONFIG_ETH_RENESAS_RH850_ETND_ADDR_FILTER)
 #define ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER (1)
#else
 #define ETHER_TSN_CFG_UPDATE_PHYS_ADDR_FILTER (0)
#endif

#define ETHER_TSN_CFG_READ_BUFFER_SIZE_CHECK (0)

#ifdef __cplusplus
}
#endif

#endif                                 /* R_ETHER_CFG_H_ */

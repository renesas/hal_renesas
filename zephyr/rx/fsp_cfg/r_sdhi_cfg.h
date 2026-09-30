/*
 * Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef R_SDHI_CFG_H_
#define R_SDHI_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define SDHI_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#if (2 == (0))
#define SDMMC_CFG_UNALIGNED_ACCESS_ENABLE (1)
#define SDMMC_CFG_DTC_SUPPORT_ENABLE (1)
#else
#define SDMMC_CFG_UNALIGNED_ACCESS_ENABLE (0)
#define SDMMC_CFG_DTC_SUPPORT_ENABLE (0)
#endif
#ifndef SDHI_CFG_SD_SUPPORT_ENABLE
#define SDHI_CFG_SD_SUPPORT_ENABLE ((1))
#endif
#ifndef SDHI_CFG_EMMC_SUPPORT_ENABLE
#define SDHI_CFG_EMMC_SUPPORT_ENABLE ((0))
#endif

#ifdef __cplusplus
}
#endif
#endif /* R_SDHI_CFG_H_ */

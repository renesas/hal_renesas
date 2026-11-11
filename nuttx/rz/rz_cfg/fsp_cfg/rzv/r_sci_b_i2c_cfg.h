/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_SCI_B_I2C_CFG_H_
#define R_SCI_B_I2C_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define SCI_SIIC_CFG_PREREQUISITE_CHECKING_ENABLE 1
#define SCI_B_I2C_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#define SCI_B_I2C_CFG_DMAC_ENABLE (0)
#define SCI_B_I2C_CFG_ADDR_MODE_10_BIT_ENABLE (1)

#ifdef __cplusplus
}
#endif
#endif /* R_SCI_B_I2C_CFG_H_ */

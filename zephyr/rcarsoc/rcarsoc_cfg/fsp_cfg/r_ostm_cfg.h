/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_OSTM_CFG_H_
#define R_OSTM_CFG_H_

/* Disabled by default to reduce code size. */
#ifndef OSTM_CFG_PARAM_CHECKING_ENABLE
 #define OSTM_CFG_PARAM_CHECKING_ENABLE   (0)
#endif

#ifndef OSTM_CFG_CLOCK
 #define OSTM_CFG_CLOCK                   (66666666)
#endif

#endif                                 /* R_OSTM_CFG_H_ */

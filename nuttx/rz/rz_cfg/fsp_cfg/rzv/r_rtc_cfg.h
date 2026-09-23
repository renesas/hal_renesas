/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* Hand-authored in the house style of the sibling rzv *_cfg.h headers.
 * Not a byte copy of any vendor file, but the macro names and their default
 * values are taken from the FSP-generated fixture
 * fsp-rz test_files/rzv_cfg/fsp_cfg/r_rtc_cfg.h (INTERNAL v4.2.0-beta.0),
 * which carries the placeholder disclaimer above. The placeholder is retained
 * deliberately so that a single grep for REA_DISCLAIMER_PLACEHOLDER finds every
 * file in this tree whose licence state is unresolved.
 *
 * RTC_CFG_OPEN_SET_CLOCK_SOURCE is defined here although the rzv module
 * description never emits it (the property is referenced but not declared;
 * upstream defect noted in .claude/state/artifacts/rtc/FSP-API.md section 11).
 * Defining it to (0) keeps r_rtc.c:263 well-defined instead of relying on
 * -Wno-undef. The value is correct for rzv2h regardless, because
 * BSP_FEATURE_RTC_CLOCK_SOURCE_UNSELECTABLE is defined for this device.
 */

#ifndef R_RTC_CFG_H_
#define R_RTC_CFG_H_
#ifdef __cplusplus
extern "C" {
#endif

#define RTC_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#define RTC_CFG_OPEN_SET_CLOCK_SOURCE (0)

#ifdef __cplusplus
}
#endif
#endif /* R_RTC_CFG_H_ */

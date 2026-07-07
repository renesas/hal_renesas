/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RP_BSP_CLOCK_CONTROL_H_
#define RP_BSP_CLOCK_CONTROL_H_

#include <stdbool.h>
#include <stdint.h>

#include "bsp_api.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Apply a RH850 U2x peripheral clock source/divider configuration.
 *
 * This is a Zephyr integration helper. The upstream RH850 U2x FSP CGC API
 * exposes BSP boot-time clock selector setup, but it does not expose a generic
 * public runtime API for peripheral selector/divider changes. This helper keeps
 * the register writes and SoC-specific selector mapping in the HAL layer so the
 * Zephyr driver does not duplicate SoC register layout details.
 *
 * @param[in] clock_id        RH850_CLOCK_* identifier from Zephyr dt-bindings.
 * @param[in] source_clock_id Logical RH850_CLOCK_* source parsed from the DTS
 *                            clock-source phandle.
 * @param[in] div             Raw SoC-specific divider encoding. Only clocks
 *                            with a hardware divider use this value; other
 *                            clocks use zero (RLIN also accepts one).
 *
 * @retval FSP_SUCCESS              The selector/divider was applied or the
 *                                  requested clock needs no runtime register
 *                                  update on this SoC.
 * @retval FSP_ERR_UNSUPPORTED      The clock/source pair is unsupported by
 *                                  this SoC/HAL.
 * @retval FSP_ERR_INVALID_ARGUMENT The divider value is invalid.
 */
fsp_err_t RP_BSP_PeripheralClockConfigure(uint32_t clock_id,
					 uint32_t source_clock_id,
					 uint32_t div);

/**
 * Convert a physical clock division factor to the peripheral HAL encoding.
 *
 * The factor includes fixed source prescalers. Source and divider validation
 * completes without accessing hardware; the output is written only on success.
 *
 * @param[in] clock_id        Logical destination RH850_CLOCK_* identifier.
 * @param[in] source_clock_id Logical source RH850_CLOCK_* identifier.
 * @param[in] factor          Physical division factor; zero stops EXTCLK only.
 * @param[out] div            Divider encoding for RP_BSP_PeripheralClockConfigure().
 *
 * @retval FSP_SUCCESS              The source and factor have a valid encoding.
 * @retval FSP_ERR_INVALID_ARGUMENT The output pointer or division factor is invalid.
 * @retval FSP_ERR_UNSUPPORTED      The clock/source pair or encoding is unsupported.
 */
fsp_err_t RP_BSP_PeripheralClockDividerEncode(uint32_t clock_id, uint32_t source_clock_id,
					   uint32_t factor, uint32_t *div);

/** Enable a module standby channel using a logical RH850 module ID. */
fsp_err_t RP_BSP_ModuleStandbyEnableById(uint32_t module_id, uint32_t channel);

/** Disable a module standby channel using a logical RH850 module ID. */
fsp_err_t RP_BSP_ModuleStandbyDisableById(uint32_t module_id, uint32_t channel);

/** Read a module standby channel using a logical RH850 module ID. */
fsp_err_t RP_BSP_ModuleStandbyStatusGetById(uint32_t module_id, uint32_t channel, bool *standby);

/** Read the active peripheral selector and divider from hardware status registers. */
fsp_err_t RP_BSP_PeripheralClockConfigGet(uint32_t clock_id, uint32_t *source_clock_id, uint32_t *div);

/** Return the effective active clock rate for a logical RH850 clock ID. */
fsp_err_t RP_BSP_ClockHzGet(uint32_t clock_id, uint32_t *rate_hz);

#ifdef __cplusplus
}
#endif

#endif /* RP_BSP_CLOCK_CONTROL_H_ */

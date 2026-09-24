/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*
*/

#ifndef R_ECM_H
#define R_ECM_H

/**
* @defgroup      ECM_Module Error Control Module (ECM)
* @{
* @brief         This module provides APIs to configure and manage the ECM hardware.
*
* The ECM (Error Control Module) provides mechanisms for detecting, counting,
* notifying, and handling hardware errors across multiple subsystems.
* This module offers enums, structures, and functions to configure error
* detection, pin/reset outputs, interrupt notification, and pseudo error insertion.
*/

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include <stddef.h>
#include "r_error_domain_id.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ECM_GET_ERROR_DOMAIN_ID(register_number, bit_positive)  (((register_number) * 32) + (bit_positive))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 *  @struct STag_EcmIrqCfg
 *  @brief  Handle structure for DMAC.
 *  @details This structure is used to manage the DMAC Irq handle.
 */
typedef void (*IrqErrorHandlerFn)(e_ecm_error_id_t id);

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
/**
 * @brief Register a callback function for merged ECM error interrupt.
 *
 * This function sets the user-provided callback that will be invoked whenever
 * any ECM error event triggers the merged ECM interrupt.
 *
 * @param[in] irq_handler  Pointer to the user callback function.
 *                         The callback receives the detected ECM error ID.
 *
 * @retval 0  Callback registered successfully.
 */
uint8_t R_ECM_SetInterruptCallback(IrqErrorHandlerFn irq_handler);

/**
 * @brief Enable or disable error detection for a specific ECM error ID.
 *
 * @param[in] id        ECM error ID to configure.
 * @param[in] isEnable  Set to 1 to enable detection, or 0 to disable.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_SetDetection(e_ecm_error_id_t id, int8_t isEnable);

/**
 * @brief Enable or disable the ECM pin output for a specific error ID.
 *
 * When enabled, the selected error can assert the ECMERR pin.
 *
 * @param[in] id        ECM error ID to configure.
 * @param[in] isEnable  Set to 1 to enable pin output, or 0 to disable.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_SetPinOut(e_ecm_error_id_t id, int8_t isEnable);

/**
 * @brief Enable or disable system reset output for a specific ECM error ID.
 *
 * When enabled, the ECM will generate a system reset when this error occurs.
 *
 * @param[in] id        ECM error ID to configure.
 * @param[in] isEnable  Set to 1 to enable reset output, or 0 to disable.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_SetReset(e_ecm_error_id_t id, int8_t isEnable);

/**
 * @brief Enable or disable interrupt notification for a specific ECM error ID.
 *
 * This controls whether the selected error contributes to the merged ECM interrupt.
 *
 * @param[in] id        ECM error ID to configure.
 * @param[in] isEnable  Set to 1 to enable interrupt notification, or 0 to disable.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_SetInterruptNotification(e_ecm_error_id_t id, int8_t isEnable);

/**
 * @brief Disable all ECM detection, pin output, reset output, and interrupt notifications.
 *
 * This function clears all enable bits across ECM control registers,
 * effectively disabling the ECM error monitoring system.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_DisableAll();

/**
 * @brief Check whether a specific ECM error ID is currently active.
 *
 * Reads the corresponding ECM error status register and returns
 * whether the error bit is asserted.
 *
 * @param[in] id  ECM error ID to check.
 *
 * @retval 1  Error active.
 * @retval 0  Error not active.
 */
uint8_t R_ECM_CheckErrorStatus(e_ecm_error_id_t id);

/**
 * @brief Inject a pseudo error for a specific ECM error ID.
 *
 * This function forces a test error condition by setting the
 * corresponding pseudo-error bit provided by the hardware.
 *
 * @param[in] id  ECM error ID to trigger as pseudo error.
 *
 * @retval 0  Operation successful.
 */
uint8_t R_ECM_PseudoError(e_ecm_error_id_t id);

#ifdef __cplusplus
}
#endif

/** @} */ // end

#endif /* R_ECM_H */
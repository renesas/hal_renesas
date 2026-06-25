/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RP_BSP_MODULE_STANDBY_H_
#define RP_BSP_MODULE_STANDBY_H_

#include <stdbool.h>
#include <stdint.h>

#include "bsp_api.h"
#include "bsp_module_standby_driver.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Read the module standby state from the target MSR register.
 *
 * @param[in]  module          Module standby register index.
 * @param[in]  channel         Module channel bit in the MSR register.
 * @param[out] module_standby  true when the module is in standby,
 *                             false when its clock is supplied.
 *
 * @retval FSP_SUCCESS              State was read successfully.
 * @retval FSP_ERR_ASSERTION        module_standby is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT Invalid module, channel, or MSR address.
 */
fsp_err_t RP_BSP_ModuleStandbyGet(bsp_module_standby_t module,
				 uint32_t channel,
				 bool *module_standby);

#ifdef __cplusplus
}
#endif

#endif /* RP_BSP_MODULE_STANDBY_H_ */

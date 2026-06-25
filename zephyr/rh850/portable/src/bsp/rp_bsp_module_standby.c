/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "rp_bsp_module_standby.h"

#define RP_BSP_MODULE_STANDBY_CHANNEL_MAX    32U

fsp_err_t RP_BSP_ModuleStandbyGet(bsp_module_standby_t module,
				 uint32_t channel,
				 bool *module_standby)
{
	volatile uint32_t *msr;
	uint32_t value;

	FSP_ASSERT(module_standby != NULL);

	if (((uint32_t)module >= (uint32_t)BSP_MODULE_STANDBY_TOTAL_INDEX_END) ||
	    (channel >= RP_BSP_MODULE_STANDBY_CHANNEL_MAX)) {
		return FSP_ERR_INVALID_ARGUMENT;
	}

	msr = g_msr_info_table[module].msr_reg_address;
	if (msr == NULL) {
		return FSP_ERR_INVALID_ARGUMENT;
	}

	value = *msr;

	/*
	 * RH850 module standby register convention:
	 *   bit = 0: clock supplied, module released from standby
	 *   bit = 1: clock stopped, module in standby
	 */
	*module_standby = ((value & (1UL << channel)) != 0U);

	return FSP_SUCCESS;
}

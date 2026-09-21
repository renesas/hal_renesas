/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stddef.h>
#include "mfis/rp_mfis_scp_mbox.h"

#define MFIS_SCP_BASE 0x18840000

#define MFIS_RT_SCP_MAX_CHANNELS 12

/* MFIS CPU communication control register SCP core to Realtime core[m] */
#define MFISRSIICR(m) (MFIS_SCP_BASE + 0x1000 * (m) + 0x20000)
/* MFIS CPU communication control register Realtime core[m] to SCP core */
#define MFISRSEICR(m) (MFIS_SCP_BASE + 0x1000 * (m) + 0x4 + 0x20000)
/* MFIS CPU communication message register SCP core to Realtime core[m] */
#define MFISRSIMBR(m) (MFIS_SCP_BASE + 0x1000 * (m) + 0x40 + 0x20000)
/* MFIS CPU communication message register Realtime core[m] to SCP core */
#define MFISRSEMBR(m) (MFIS_SCP_BASE + 0x1000 * (m) + 0x44 + 0x20000)

/** @brief Write a message into the RT to SCP message register */
int mfis_scp_send_message(uint32_t channel, uint32_t value)
{
	if (channel >= MFIS_RT_SCP_MAX_CHANNELS)
		return -EINVAL;

	*(volatile uint32_t *) MFISRSEMBR(channel) = value;

	return 0;
}

/** @brief Read a message inside the SCP to RT message register */
int mfis_scp_get_message(uint32_t channel, uint32_t *msg)
{
	if ((channel >= MFIS_RT_SCP_MAX_CHANNELS) || (msg == NULL)) {
		return -EINVAL;
	}

	*msg = *(volatile uint32_t *) MFISRSIMBR(channel);

	return 0;
}

/** @brief Trigger an interrupt from RT to SCP */
int mfis_scp_trigger_interrupt(uint32_t channel)
{
	if (channel >= MFIS_RT_SCP_MAX_CHANNELS)
		return -EINVAL;

	*(volatile uint32_t *) MFISRSEICR(channel) = 0x1;

	return 0;
}

/** @brief Clear an already triggered interrupt from SCP to RT */
int mfis_scp_clear_interrupt(uint32_t channel)
{
	if (channel >= MFIS_RT_SCP_MAX_CHANNELS)
		return -EINVAL;

	*(volatile uint32_t *) MFISRSIICR(channel) = 0x0;

	return 0;
}

/** @brief Check whether the TX (RT-to-SCP) interrupt request bit is still pending */
int mfis_scp_check_tx_pending(uint32_t channel, bool *pending)
{
	uint32_t reg;

	if (channel >= MFIS_RT_SCP_MAX_CHANNELS || pending == NULL) {
		return -EINVAL;
	}

	reg = *(volatile uint32_t *)MFISRSEICR(channel);
	*pending = (reg & 0x1U) != 0U;

	return 0;
}

/** @brief Check whether the RX (SCP-to-RT) interrupt request bit is still pending */
int mfis_scp_check_rx_pending(uint32_t channel, bool *pending)
{
	uint32_t reg;

	if (channel >= MFIS_RT_SCP_MAX_CHANNELS || pending == NULL) {
		return -EINVAL;
	}

	reg = *(volatile uint32_t *)MFISRSIICR(channel);
	*pending = (reg & 0x1U) != 0U;

	return 0;
}

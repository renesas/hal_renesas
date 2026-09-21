/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#include "mfis/rp_mfis_mbox.h"
#include <errno.h>
#include <stddef.h>

/* AP-RT common MFIS helper */
/* Write a message into the MFIS common message register */
int rp_mfis_common_send(struct mfis_channel *ch, uint32_t value)
{
	if (ch == NULL) {
		return -EINVAL;
	}

	return mfis_send_message(ch, value);
}

/* Read a message from the MFIS common message register */
int rp_mfis_common_get(struct mfis_channel *ch, uint32_t *msg)
{
	if ((ch == NULL) || (msg == NULL) || (ch->ch > 63)) {
		return -EINVAL;
	}

	*msg = mfis_get_message(ch);

	return 0;
}

/* Trigger interrupt to another core */
int rp_mfis_common_trigger(struct mfis_channel *ch, uint16_t source)
{
	if (ch == NULL) {
		return -EINVAL;
	}

	return mfis_trigger_interrupt(ch, source);
}

/* Clear incoming interrupt */
int rp_mfis_common_clear(struct mfis_channel *ch)
{
	if (ch == NULL) {
		return -EINVAL;
	}

	return mfis_clear_interrupt(ch);
}

/* Check whether TX interrupt request bit is still pending */
int rp_mfis_common_check_tx_pending(struct mfis_channel *ch, bool *pending)
{
	if ((ch == NULL) || (pending == NULL)) {
		return -EINVAL;
	}

	return mfis_check_tx_pending(ch, pending);
}

/* Check whether RX interrupt request bit is still pending */
int rp_mfis_common_check_rx_pending(struct mfis_channel *ch, bool *pending)
{
	if ((ch == NULL) || (pending == NULL)) {
		return -EINVAL;
	}

	return mfis_check_rx_pending(ch, pending);
}

/* Unlock write protection */
void rp_mfis_common_unlock_write(void)
{
	mfis_unlock_write();
}

/* RT-SCP MFIS helper */
/* Write a message into the MFIS RT-SCP message register */
int rp_mfis_scp_send(uint32_t channel, uint32_t value)
{
	return mfis_scp_send_message(channel, value);
}

/* Read a message from the MFIS SCP-RT message register */
int rp_mfis_scp_get(uint32_t channel, uint32_t *msg)
{
	return mfis_scp_get_message(channel, msg);
}

/* Trigger interrupt from RT-SCP */
int rp_mfis_scp_trigger(uint32_t channel)
{
	return mfis_scp_trigger_interrupt(channel);
}

/* Clear interrupt from SCP-RT */
int rp_mfis_scp_clear(uint32_t channel)
{
	return mfis_scp_clear_interrupt(channel);
}

/* Check whether the TX (RT-to-SCP) interrupt request bit is still pending */
int rp_mfis_scp_check_tx_pending(uint32_t channel, bool *pending)
{
	return mfis_scp_check_tx_pending(channel, pending);
}

/* Check whether the RX (SCP-to-RT) interrupt request bit is still pending */
int rp_mfis_scp_check_rx_pending(uint32_t channel, bool *pending)
{
	return mfis_scp_check_rx_pending(channel, pending);
}

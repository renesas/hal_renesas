/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_MFIS_MBOX_H
#define RP_MFIS_MBOX_H

#include "mfis/rp_mfis_scp_mbox.h"
#include "mfis/mfis.h"
#include <stdbool.h>
#include <stdint.h>

/* AP-RT common MFIS helper */
int rp_mfis_common_send(struct mfis_channel *ch, uint32_t value);
int rp_mfis_common_get(struct mfis_channel *ch, uint32_t *msg);
int rp_mfis_common_trigger(struct mfis_channel *ch, uint16_t source);
int rp_mfis_common_clear(struct mfis_channel *ch);
int rp_mfis_common_check_tx_pending(struct mfis_channel *ch, bool *pending);
int rp_mfis_common_check_rx_pending(struct mfis_channel *ch, bool *pending);
void rp_mfis_common_unlock_write(void);

/* RT-SCP MFIS helper */
int rp_mfis_scp_send(uint32_t channel, uint32_t value);
int rp_mfis_scp_get(uint32_t channel, uint32_t *msg);
int rp_mfis_scp_trigger(uint32_t channel);
int rp_mfis_scp_clear(uint32_t channel);
int rp_mfis_scp_check_tx_pending(uint32_t channel, bool *pending);
int rp_mfis_scp_check_rx_pending(uint32_t channel, bool *pending);

#endif /* RP_MFIS_MBOX_H */

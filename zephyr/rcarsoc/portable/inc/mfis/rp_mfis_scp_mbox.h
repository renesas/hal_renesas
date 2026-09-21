/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef RP_MFIS_SCP_MBOX_H
#define RP_MFIS_SCP_MBOX_H

#include <stdint.h>
#include <errno.h>
#include <stdbool.h>

int mfis_scp_send_message(uint32_t channel, uint32_t value);
int mfis_scp_get_message(uint32_t channel, uint32_t *msg);
int mfis_scp_trigger_interrupt(uint32_t channel);
int mfis_scp_clear_interrupt(uint32_t channel);
int mfis_scp_check_tx_pending(uint32_t channel, bool *pending);
int mfis_scp_check_rx_pending(uint32_t channel, bool *pending);

#endif /* RP_MFIS_SCP_MBOX_H */

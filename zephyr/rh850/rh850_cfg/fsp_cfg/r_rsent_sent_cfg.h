/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#include <zephyr/devicetree.h>

#ifndef R_RSENT_SENT_CFG_H_
#define R_RSENT_SENT_CFG_H_
#define RSENT_SENT_CFG_PARAM_CHECKING_ENABLE (BSP_CFG_PARAM_CHECKING_ENABLE)
#define RSENT_SENT_CFG_FAST_MSG_TIMESTAMP     (1)
#define RSENT_SENT_CFG_SLOW_MSG_TIMESTAMP     (1)

/*
 * Produce an arithmetic expression that counts enabled RSENT child nodes
 * containing a pause-pulse-detect property.
 */
#define RSENT_SENT_DT_CHILD_PAUSE_PULSE_DETECT_TERM(node_id) \
    + DT_NODE_HAS_PROP(node_id, pause_pulse_detect)

#define RSENT_SENT_DT_PARENT_PAUSE_PULSE_DETECT_TERM(node_id) \
    + (0 DT_FOREACH_CHILD_STATUS_OKAY(                            \
           node_id, RSENT_SENT_DT_CHILD_PAUSE_PULSE_DETECT_TERM))

#define RSENT_SENT_DT_PAUSE_PULSE_DETECT_CHANNEL_COUNT \
    (0 DT_FOREACH_STATUS_OKAY(                           \
           renesas_rh850_sent,                           \
           RSENT_SENT_DT_PARENT_PAUSE_PULSE_DETECT_TERM))

#if (RSENT_SENT_DT_PAUSE_PULSE_DETECT_CHANNEL_COUNT > 0)
 #define RSENT_SENT_CFG_FIXED_MSG_LENGTH       (1)
#else
 #define RSENT_SENT_CFG_FIXED_MSG_LENGTH       (0)
#endif


/*
* Produce an arithmetic expression that counts enabled RSENT child nodes
* containing a dmas property.
*/
#define RSENT_SENT_DT_CHILD_DMA_TERM(node_id) \
        + DT_NODE_HAS_PROP(node_id, dmas)

#define RSENT_SENT_DT_PARENT_DMA_TERM(node_id) \
        + (0 DT_FOREACH_CHILD_STATUS_OKAY( \
            node_id, RSENT_SENT_DT_CHILD_DMA_TERM))

#define RSENT_SENT_DT_DMA_CHANNEL_COUNT \
        (0 DT_FOREACH_STATUS_OKAY( \
            renesas_rh850_sent, \
                RSENT_SENT_DT_PARENT_DMA_TERM))

#if (RSENT_SENT_DT_DMA_CHANNEL_COUNT > 0)
 #if !defined(CONFIG_DMA)
  #error "An enabled RH850 RSENT channel contains dmas, but CONFIG_DMA is disabled"
 #endif
 #define RSENT_SENT_TRANSFER_SUPPORT_ENABLE (1)
#else
 #define RSENT_SENT_TRANSFER_SUPPORT_ENABLE (0)
#endif

#endif                                 /* R_RSENT_SENT_CFG_H_ */

/*
 * Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its
 * affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef RP_RMAC_H
#define RP_RMAC_H

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_api.h"
#include "r_rmac_cfg.h"
#include "r_rmac.h"
#include "r_layer3_switch.h"
#include "r_rmac_phy.h"

FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

 /**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_api_t g_ether_on_rmac;

fsp_err_t rpmac_do_link (rmac_instance_ctrl_t * const                 p_instance_ctrl,
                               const layer3_switch_magic_packet_detection_t mode);

/** @endcond */


/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* RP_RMAC_H */
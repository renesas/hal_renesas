/*
 * Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "rp_crc.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "CRC" in ASCII, used to determine if channel is open. */
#define CRC_OPEN (0x00435243ULL)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Reconfigure the CRC driver module
 *
 * Initialize the driver control block according to the passed-in
 * configuration structure.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_ctrl or p_cfg is NULL.
 * @retval FSP_ERR_ALREADY_OPEN    Module already open
 **********************************************************************************************************************/
fsp_err_t RP_CRC_Reconfigure (crc_ctrl_t * const p_ctrl, crc_cfg_t const * const p_cfg)
{
    crc_instance_ctrl_t * p_instance_ctrl = (crc_instance_ctrl_t *) p_ctrl;
    uint32_t              crc_ctl         = 0;
#if CRC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
#endif

    /* Pointer to CRC extended configuration. */
    crc_extended_cfg_t * p_extend = (crc_extended_cfg_t *) p_cfg->p_extend;
    crc_ctl_reg_t * p_ctl = (crc_ctl_reg_t *) p_extend->control_reg;

    /* Setting the control register values */
    crc_ctl  = (uint32_t) p_ctl->input_data_size << R_KCRC0_CTL_DW_Pos;
    crc_ctl |= (uint32_t) p_cfg->bit_order << R_KCRC0_CTL_CMD2_Pos;
    crc_ctl |= (uint32_t) p_ctl->input_mode << R_KCRC0_CTL_CMD1_Pos;
    crc_ctl |= (uint32_t) p_ctl->output_mode << R_KCRC0_CTL_CMD0_Pos;
    crc_ctl |= (uint32_t) p_ctl->poly_size << R_KCRC0_CTL_PSIZE_Pos;
    p_instance_ctrl->p_reg->CTL   = (uint32_t) crc_ctl;
    p_instance_ctrl->p_reg->POLY0 = (uint32_t) (p_extend->poly0);
    p_instance_ctrl->p_reg->POLY1 = (uint32_t) (p_extend->poly1);
    p_instance_ctrl->p_reg->XOR0  = (uint32_t) (p_extend->xor0);
    p_instance_ctrl->p_reg->XOR1  = (uint32_t) (p_extend->xor1);

    /* Setting the KCRCnDOUT0 and KCRCnDOUT1 registers before the first write access
     *                             to the KCRC input register (KCRCnDIN) is performed.*/
    p_instance_ctrl->p_reg->DOUT0 = (uint32_t) (p_extend->dout0);
    p_instance_ctrl->p_reg->DOUT1 = (uint32_t) (p_extend->dout1);

    return FSP_SUCCESS;
}

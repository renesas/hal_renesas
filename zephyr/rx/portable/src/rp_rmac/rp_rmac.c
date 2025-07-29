/*
 * Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its
 * affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rmac_cfg.h"
#include "r_rmac.h"
#include "r_layer3_switch.h"
#include "r_rmac_phy.h"
#include "rp_rmac.h"
#include <stdio.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define RMAC_OPEN                                   (('R' << 24U) | ('M' << 16U) | ('A' << 8U) | ('C' << 0U))

#define RMAC_ETHA_REG_SIZE                          (R_ETHA1_BASE - R_ETHA0_BASE)
#define RMAC_RMAC_REG_SIZE                          (R_RMAC1_BASE - R_RMAC0_BASE)

/* Register values */
#define RMAC_REG_MRAFC_PROMISCUOUS_VALUE            (R_RMAC0_MRAFC_UCENE_Msk | R_RMAC0_MRAFC_MCENE_Msk |    \
                                                     R_RMAC0_MRAFC_BCENE_Msk | R_RMAC0_MRAFC_BCACE_Msk |    \
                                                     R_RMAC0_MRAFC_NDAREE_Msk | R_RMAC0_MRAFC_SDSFREE_Msk | \
                                                     R_RMAC0_MRAFC_NSAREE_Msk | R_RMAC0_MRAFC_UCENP_Msk |   \
                                                     R_RMAC0_MRAFC_MCENP_Msk | R_RMAC0_MRAFC_BCENP_Msk |    \
                                                     R_RMAC0_MRAFC_BCACP_Msk | R_RMAC0_MRAFC_NDAREP_Msk |   \
                                                     R_RMAC0_MRAFC_SDSFREP_Msk | R_RMAC0_MRAFC_NSAREP_Msk)

/* For descriptor fields */
#define RMAC_DESCRIPTOR_FIELD_DS_UPPER_MASK         (0xF00)
#define RMAC_DESCRIPTOR_FIELD_DS_LOWER_MASK         (0x00FF)
#define RMAC_DESCRIPTOR_FIELD_DS_UPPER_POSITION     (0x8)
#define RMAC_DESCRIPTOR_FIELD_DV_MASK               (0x7F)
#define RMAC_DESCRIPTOR_FIELD_PTR_UPPER_MASK        (0xFF00000000)
#define RMAC_DESCRIPTOR_FIELD_PTR_LOWER_MASK        (0xFFFFFFFF)
#define RMAC_DESCRIPTOR_FIELD_PTR_UPPER_POSITION    (32)

#define LAYER3_SWITCH_FWPBFC_REGISTER_OFFSET          (0x10)
#define LAYER3_SWITCH_FWPBFCSDC0_REGISTER_OFFSET      (0x10)
#define LAYER3_SWITCH_INTERRUPT_REGISTER_OFFSET       (0x10)
#define LAYER3_SWITCH_FWPC_REGISTER_OFFSET            (0x10)
#define LAYER3_SWITCH_PORT_CONFIG_REGISTER_OFFSET     (0x10)

/* Bitmask for the CPU port (GWCA).  */
#define LAYER3_SWITCH_PORT_CPU_BITMASK                (1 << BSP_FEATURE_ETHER_MAX_CHANNELS)

/* Definition of the maximum / minimum number of data that can be sent at one time in the Ethernet */
#define RMAC_MAXIMUM_FRAME_SIZE                     (1514U) /* Maximum number of transmitted data */
#define RMAC_MINIMUM_FRAME_SIZE                     (60U)   /* Minimum number of transmitted data */
#define RMAC_NO_DATA                                (0)

/* Invalid value that mean all queue is not running or not available. */
#define RMAC_INVALID_QUEUE_INDEX                    (0xFFFFFFFF)

/* Increments a index and wraps around to 0 if it exceeds the maximum value */
#define RMAC_INCREMENT_INDEX(x, max)    (x = (x + 1) % max)

extern void      r_rmac_switch_interrupt_callback(ether_switch_callback_args_t * p_args);

/*******************************************************************************************************************//**
 * Configure Ethernet port features, including callback function for each port.
 *
 * @retval  FSP_SUCCESS                  Port configured successfully.
 * @retval  FSP_ERR_ASSERTION            Pointer to control block is NULL..
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval  FSP_ERR_INVALID_POINTER      Pointer to a argument is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT     Port number is invalid.
 **********************************************************************************************************************/
static fsp_err_t RP_LAYER3_SWITCH_ConfigurePort (ether_switch_ctrl_t * const p_ctrl,
                                         uint8_t                     port,
                                         layer3_switch_port_cfg_t  * p_port_cfg)
{
    layer3_switch_instance_ctrl_t * p_instance_ctrl = (layer3_switch_instance_ctrl_t *) p_ctrl;
    volatile uint32_t             * p_mfwd_fwpbfc_reg;

#if LAYER3_SWITCH_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(LAYER3_SWITCH_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(port < BSP_FEATURE_ETHER_MAX_CHANNELS, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(p_port_cfg, FSP_ERR_INVALID_POINTER);
#endif
    /* Copy callback settings to the control member. */
    p_instance_ctrl->p_port_cfg_list[port].p_callback        = p_port_cfg->p_callback;
    p_instance_ctrl->p_port_cfg_list[port].p_context         = p_port_cfg->p_context;
    p_instance_ctrl->p_port_cfg_list[port].p_callback_memory = p_port_cfg->p_callback_memory;

    p_mfwd_fwpbfc_reg =
        (uint32_t *) ((uintptr_t) &(R_MFWD->FWPBFC0) + (uint32_t) (port * LAYER3_SWITCH_FWPBFC_REGISTER_OFFSET));
    if (p_port_cfg->forwarding_to_cpu_enable)
    {
        *p_mfwd_fwpbfc_reg |= (R_MFWD_FWPBFC0_PBDV_Msk & (uint32_t) (LAYER3_SWITCH_PORT_CPU_BITMASK));
    }
    else
    {
        /* Disable forwarding to CPU. But forwarding from the LAN port to the LAN port is still enabled. */
        *p_mfwd_fwpbfc_reg &= (R_MFWD_FWPBFC0_PBDV_Msk & (uint32_t) (~LAYER3_SWITCH_PORT_CPU_BITMASK));
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************
 * Disable reception on this port.
 **********************************************************************************************************************/
static void r_rmac_disable_reception (rmac_instance_ctrl_t * p_instance_ctrl)
{
    rmac_extended_cfg_t      * p_extend           = (rmac_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    layer3_switch_port_cfg_t   port_cfg           = {0};
    layer3_switch_descriptor_t descriptor         = {0};
    rmac_queue_info_t          running_queue_info = p_extend->p_rx_queue_list[p_instance_ctrl->rx_running_queue_index];

    /* When magic packet detection is enabled, disable data interrupt. */
    port_cfg.p_callback = NULL;

    /* Disable CPU reception from this port. */
    port_cfg.forwarding_to_cpu_enable = false;
    RP_LAYER3_SWITCH_ConfigurePort(p_extend->p_ether_switch->p_ctrl, p_instance_ctrl->p_cfg->channel, &port_cfg);

    /* Set descriptors in RX running queue to stop state. */
    descriptor.basic.dt = LAYER3_SWITCH_DESCRIPTOR_TYPE_LEMPTY;
    for (size_t i = 0; i < running_queue_info.queue_cfg.array_length; i++)
    {
        R_LAYER3_SWITCH_SetDescriptor(p_extend->p_ether_switch->p_ctrl, running_queue_info.index, &descriptor);
    }

    /* Perform starting reception to initialize queue status. */
    R_LAYER3_SWITCH_StartDescriptorQueue(p_extend->p_ether_switch->p_ctrl, running_queue_info.index);
}

 /********************************************************************************************************************//**
 * @brief Determines the partner PHY capability through auto-negotiation process. The link abilities
 *        are handled to determine duplex, speed and flow control (PAUSE frames).
 *
 * @param[in] p_instance_ctrl           Pointer to the control block for the channel
 * @param[in] mode                      The operational mode is specified.
 *                                      NO_USE_MAGIC_PACKET_DETECT (0) - Communicate mode usually
 *                                      USE_MAGIC_PACKET_DETECT    (1) - Magic packet detection mode
 * @retval  FSP_SUCCESS                 Processing completed successfully.
 * @retval  FSP_ERR_ASSERTION           Pointer to ETHER control block or configuration structure is NULL.
 * @retval  FSP_ERR_NOT_OPEN            The control block has not been opened.
 * @retval  FSP_ERR_ETHER_ERROR_LINK    Auto-negotiation of PHY-LSI is not completed
 *                                      or result of Auto-negotiation is abnormal.
 *
 ***********************************************************************************************************************/
fsp_err_t rpmac_do_link (rmac_instance_ctrl_t * const                 p_instance_ctrl,
                               const layer3_switch_magic_packet_detection_t mode)
{
    fsp_err_t err = FSP_SUCCESS;

    rmac_extended_cfg_t          * p_rmac_extended_cfg;
    layer3_switch_extended_cfg_t * p_switch_extended_cfg;

    layer3_switch_port_cfg_t port_cfg;
    fsp_err_t                link_result=FSP_SUCCESS;

#if (RMAC_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(RMAC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_rmac_extended_cfg   = (rmac_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    p_switch_extended_cfg = (layer3_switch_extended_cfg_t *) p_rmac_extended_cfg->p_ether_switch->p_cfg->p_extend;

    if (FSP_SUCCESS == link_result)
    {
        /* Set MAC address. */
        port_cfg.p_mac_address = p_instance_ctrl->p_cfg->p_mac_address;

        if (LAYER3_SWITCH_MAGIC_PACKET_DETECTION_DISABLE == mode)
        {
            /*
             * To prevent reception from starting at an invalid descriptor, the queue must be reloaded first.
             * Until then, reception is kept disabled.
             */
            port_cfg.forwarding_to_cpu_enable = false;
            R_LAYER3_SWITCH_ConfigurePort(p_rmac_extended_cfg->p_ether_switch->p_ctrl,
                                          p_instance_ctrl->p_cfg->channel,
                                          &port_cfg);

            /* Reload RX queue. */
            p_instance_ctrl->rx_running_queue_index = 0;
            err = R_LAYER3_SWITCH_StartDescriptorQueue(p_rmac_extended_cfg->p_ether_switch->p_ctrl,
                                                       p_rmac_extended_cfg->p_rx_queue_list[0].index);

            /* Set callback for switch data interrupt. */
            port_cfg.p_callback        = r_rmac_switch_interrupt_callback;
            port_cfg.p_context         = p_instance_ctrl;
            port_cfg.p_callback_memory = NULL;

            /* Enable reception. */
            port_cfg.forwarding_to_cpu_enable = true;
            R_LAYER3_SWITCH_ConfigurePort(p_rmac_extended_cfg->p_ether_switch->p_ctrl,
                                          p_instance_ctrl->p_cfg->channel,
                                          &port_cfg);
        }
        else
        {
            /* Disable reception. */
            r_rmac_disable_reception(p_instance_ctrl);

            /* Enable a magic packet interrupt. */
            R_BSP_IrqCfgEnable(p_rmac_extended_cfg->rmpi_irq, p_rmac_extended_cfg->rmpi_ipl, p_instance_ctrl);
            p_instance_ctrl->p_reg_rmac->MMIE2_b.MPDIE = 1;
        }
    }
    else
    {
        err = FSP_ERR_ETHER_ERROR_LINK;
    }

    return err;
}                                      /* End of function rmac_do_link() */

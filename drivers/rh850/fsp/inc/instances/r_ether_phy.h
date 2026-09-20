/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */
#ifndef R_ETHER_PHY_H
#define R_ETHER_PHY_H

#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_ether_phy_cfg.h"
#include "r_ether_phy_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* OS Tick reference type */
typedef uint32_t ether_phy_tick_type;

/* OS Tick reference type */
typedef ether_phy_tick_type ether_phy_tick_ref_type;                                                                     /* PRQA S 1535 # Advisory R-2.3 */

/** Initialization state for read/write */
typedef enum e_ether_phy_interface_status                                                                                /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_PHY_INTERFACE_STATUS_UNINITIALIZED = 0,                                                                        ///< ETHER PHY interface is uninitialized
    ETHER_PHY_INTERFACE_STATUS_INITIALIZED   = 1                                                                         ///< ETHER PHY interface is initialized
} ether_phy_interface_status_t;

/** Media-independent interface */
typedef enum e_ether_phy_eth_reg                                                                                         /* PRQA S 1536 # Advisory R-2.4 */
{
    ETHER_PHY_ETH_REG_TSN    = 0,                                                                                        ///< ETH TSN
    ETHER_PHY_ETH_REG_T1S    = 1,                                                                                        ///< ETH T1S
    ETHER_PHY_ETH_REG_AVB    = 2,                                                                                        ///< ETH AVB
    ETHER_PHY_ETH_REG_TSNSWA = 3,                                                                                        ///< ETH TSNSWA
} ether_phy_eth_reg_t;

/** ETHER PHY control block. DO NOT INITIALIZE.  Initialization occurs when @ref ether_phy_api_t::open is called. */
typedef struct st_ether_phy_instance_ctrl                                                                                /* PRQA S 3630, 1536 # Advisory D-4.8, Advisory R-2.4 */
{
    uint32_t open;                                                                                                       ///< Used to determine if the channel is configured

    /* Configuration of Ethernet PHY-LSI module. */
    ether_phy_cfg_t const * p_ether_phy_cfg;                                                                             ///< Pointer to initial configurations.

    /* Interface for PHY-LSI chip. */
    volatile uint32_t * p_reg_mpsm;                                                                                      ///< Pointer to RMACn/RMACAn peripheral registers.
    volatile uint32_t * p_reg_mmis0;                                                                                     ///< Pointer to RMACn peripheral registers.
    volatile uint32_t * p_reg_mmis1;                                                                                     ///< Pointer to RMACAn peripheral registers.
    volatile uint32_t * p_reg_mmie0;                                                                                     ///< Pointer to RMACn peripheral registers
    volatile uint32_t * p_reg_mpic;                                                                                      ///< Pointer to RMACn peripheral registers.
    volatile uint32_t * p_reg_mlvc;                                                                                      ///< Pointer to RMACn peripheral registers.
    volatile uint32_t * p_reg_mioc;                                                                                      ///< Pointer to RMACn peripheral registers.
    volatile uint32_t * p_reg_pir;                                                                                       ///< Pointer to ETNFn peripheral registers.

    /* The capabilities of the local link as PHY data */
    uint32_t local_advertise;                                                                                            ///< Capabilities bitmap for local advertising.
    ether_phy_interface_status_t interface_status;                                                                       ///< Initialized status of ETHER PHY interface.
} ether_phy_instance_ctrl_t;

/** ETHER PHY extended configuration. */
typedef struct st_ether_phy_extended_cfg                                                                                 /* PRQA S 1536 # Advisory R-2.4 */
{
    void (* p_target_init)(ether_phy_instance_ctrl_t * p_instance_ctrl);                                                 ///< Pointer to callback that is called to initialize the target.
    bool (* p_target_link_partner_ability_get)(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t line_speed_duplex); ///< Pointer to callback that is called to get the link partner ability.
    ether_phy_lsi_cfg_t const * p_phy_lsi_cfg_list;                                                                      ///< Pointer PHY LSI configurations.
    ether_phy_eth_reg_t         eth_reg;                                                                                 ///< Interface type is ETH IP
    void (* p_mdio_read)(uint8_t phy_addr, uint32_t reg_addr, uint32_t * p_data);                                        ///< Custom MDIO read callback. NULL uses the hardware MDIO controller.
    void (* p_mdio_write)(uint8_t phy_addr, uint32_t reg_addr, uint32_t data);                                           ///< Custom MDIO write callback. NULL uses the hardware MDIO controller.
} ether_phy_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const ether_phy_api_t g_ether_phy_on_ether_phy;

/** @endcond */

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 * Public Function Prototypes
 **********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open(ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_PHY_Close(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_ChipInit(ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg);

fsp_err_t R_ETHER_PHY_Read(ether_phy_ctrl_t * const p_ctrl, uint32_t reg_addr, uint32_t * const p_data);

fsp_err_t R_ETHER_PHY_Write(ether_phy_ctrl_t * const p_ctrl, uint32_t reg_addr, uint32_t data);

fsp_err_t R_ETHER_PHY_StartAutoNegotiate(ether_phy_ctrl_t * const p_ctrl);

fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet(ether_phy_ctrl_t * const p_ctrl,
                                            uint32_t * const         p_line_speed_duplex,
                                            uint32_t * const         p_local_pause,
                                            uint32_t * const         p_partner_pause);

fsp_err_t R_ETHER_PHY_LinkStatusGet(ether_phy_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_ETHER_PHY_H */

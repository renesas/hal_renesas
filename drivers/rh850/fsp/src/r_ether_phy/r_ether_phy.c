/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "r_ether_phy.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

#ifndef ETHER_PHY_ERROR_RETURN

 #define ETHER_PHY_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif

#define ETHER_TSNAn_OFFSET                        (0x00002000UL) /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_RMACn_OFFSET                        (0x00002000UL) /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_RMACAn_OFFSET                       (0x00010000UL)
#define ETHER_AVBn_OFFSET                         (0x00002000UL) /* PRQA S 1534 # Advisory R-2.5 */

/** "RPHY" in ASCII.  Used to determine if the control block is open. */
#define ETHER_PHY_OPEN                            (0x52504859U)

/* Standard PHY Registers */
#define ETHER_PHY_REG_CONTROL                     (0U)
#define ETHER_PHY_REG_STATUS                      (1U)
#define ETHER_PHY_REG_AN_ADVERTISEMENT            (4U)
#define ETHER_PHY_REG_AN_LINK_PARTNER             (5U)
#define ETHER_PHY_REG_1000BASET_CONTROL           (9U)  /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_REG_1000BASET_STATUS            (10U)
#define ETHER_PHY_REG_PAGE_ADDRESS                (22U) /* PRQA S 1534 # Advisory R-2.5 */

/* Basic Mode Control Register Bit Definitions */
#define ETHER_PHY_CONTROL_RESET                   (1UL << 15U)
#define ETHER_PHY_CONTROL_AN_ENABLE               (1UL << 12U)
#define ETHER_PHY_CONTROL_AN_RESTART              (1UL << 9U)

/* Basic Mode Status Register Bit Definitions */
#define ETHER_PHY_STATUS_AN_COMPLETE              (1U << 5U)
#define ETHER_PHY_STATUS_LINK_UP                  (1U << 2U)

/* Auto Negotiation Advertisement Bit Definitions */
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR        (1UL << 11U)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE          (1UL << 10U)
#define ETHER_PHY_AN_ADVERTISEMENT_1000F          (1UL << 9U) /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_AN_ADVERTISEMENT_1000H          (1UL << 8U) /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_AN_ADVERTISEMENT_100F           (1UL << 8U)
#define ETHER_PHY_AN_ADVERTISEMENT_100H           (1U << 7U)
#define ETHER_PHY_AN_ADVERTISEMENT_10F            (1U << 6U)
#define ETHER_PHY_AN_ADVERTISEMENT_10H            (1U << 5U)
#define ETHER_PHY_AN_ADVERTISEMENT_SELECTOR       (1U << 0U)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR         (1UL << 11U)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE           (1UL << 10U)
#define ETHER_PHY_AN_LINK_PARTNER_1000F           (1UL << 11U)
#define ETHER_PHY_AN_LINK_PARTNER_1000H           (1UL << 10U)
#define ETHER_PHY_AN_LINK_PARTNER_100F            (1UL << 8U)
#define ETHER_PHY_AN_LINK_PARTNER_100H            (1U << 7U)
#define ETHER_PHY_AN_LINK_PARTNER_10F             (1U << 6U)
#define ETHER_PHY_AN_LINK_PARTNER_10H             (1U << 5U)

#define ETHER_PHY_ADDRESS_SIZE                    (0x1fU)
#define ETHER_PHY_REGISTER_DATA_SIZE              (0xffffU)

/* ETH TSN PHY */
#define ETHER_TSN_MPSM_WRITE_DATA_SET             (0x2UL)
#define ETHER_TSN_MPSM_WRITE_OPERATION            (0x1UL)
#define ETHER_TSN_PHY_CONFIG_REGISTER_MASK        (0x1UL)
#define ETHER_TSN_MMIS1_PWACS_MASK                (0x02UL)
#define ETHER_TSN_MPSM_READ_OPERATION             (0x1UL)

/* ETH TSNSWA PHY */
#define TSNSWA_RMAC_MPSM_MFF_MDIO                 (0x00000000UL)
#define TSNSWA_RMAC_MPSM_POP_READ                 (0x00004000UL)
#define TSNSWA_RMAC_MPSM_POP_WRITE                (0x00002000UL)
#define TSNSWA_RMAC_MPSM_PSME                     (0x00000001UL)
#define TSNSWA_RMAC_MPSM_PRD_POS                  (16UL)
#define TSNSWA_RMAC_MPSM_PRA_POS                  (8UL)
#define TSNSWA_RMAC_MPSM_PDA_POS                  (3UL)

#define TSNSWA_RMAC_MPIC_PIS_POS                  (0UL)          /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PIS_MSK                  (0x7UL)        /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_LSC_POS                  (3UL)          /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_LSC_MSK                  (0x38UL)       /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_FHDM_POS                 (7UL)          /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_FHDM_MSK                 (0x80UL)       /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMCS_POS                (16UL)         /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMCS_MSK                (0x7f0000UL)   /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMCT_POS                (28UL)         /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMCT_MSK                (0x70000000UL) /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMHT_POS                (24UL)         /* PRQA S 1534 # Advisory R-2.5 */
#define TSNSWA_RMAC_MPIC_PSMHT_MSK                (0x7000000UL)  /* PRQA S 1534 # Advisory R-2.5 */

#define TSNSWA_TSNA_EAMC_OPC_MSK                  (0x3UL)        /* PRQA S 1534 # Advisory R-2.5 */

#define ETH_PHY_MMD_BIT                           (16U)
#define ETH_PHY_MMD_MASK                          (0x001FU)
#define ETH_PHY_REG_ADDR_BIT                      (0U)
#define ETH_PHY_REG_ADDR_MASK                     (0xFFFFU)
#define ETH_PHY_MMD_ACCESS_CONTROL_REG            (13U)
#define ETH_PHY_MMD_ACCESS_DATA_REG               (14U)
#define ETH_PHY_MMD_ACCESS_BIT                    (0x4000U)

#define ETH_CPUCLK_MHZ                            (240UL)
#define ETH_GET_MDI(regval)        (((uint32_t) (regval) >> 3UL) & 1UL)     /* PRQA S 3472 # Advisory D-4.9 */
#define ETH_NS2HZ_AVB(nano_sec)    ((ETH_CPUCLK_MHZ * (nano_sec)) / 1000UL) /* PRQA S 3472 # Advisory D-4.9 */

#define ETH_PIR_MMD                               (0x00000002UL)
#define ETH_MDIO_SETUP_TIME                       (10UL)
#define ETH_WORST_SLEW_RATE                       (90U)
#define ETH_PIR_MDC                               (0x00000001U)
#define ETH_PIR_MDO                               (0x00000004UL)
#define ETH_PHY_MAX_PHYAD_IDX                     (31U)
#define ETH_PHY_MAX_REGAD_IDX                     (31U)
#define ETH_PHY_PREAMBLE_SIZE                     (32UL)
#define ETH_PHY_HEADER_SIZE                       (14UL)
#define ETH_PHY_DATA_SIZE                         (16UL)
#define ETH_PHY_MAX_CLAUSE22_REGISTER             (32U)

#define ETHER_PHY_AN_ADVERTISEMENT_SUPPORT        (1U)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE_POS      (1U)
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR_POS    (0U)

#define ETHER_PHY_AN_LINK_PARTNER_SUPPORT         (1U)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE_POS       (1U)
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR_POS     (0U)
#define ETHER_PHY_CT25205_RTL_INVALID_STATE1      (0x404U) /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_CT25205_RTL_INVALID_STATE2      (0x4U)   /* PRQA S 1534 # Advisory R-2.5 */

/* Operation mode of ETHA*/
#define ETHER_PHY_ETHA_DISABLE_MODE               (1UL)    /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_ETHA_CONFIG_MODE                (2UL)    /* PRQA S 1534 # Advisory R-2.5 */
#define ETHER_PHY_ETHA_OPERATION_MODE             (3UL)    /* PRQA S 1534 # Advisory R-2.5 */

/* Create a header for read operation */
#define ETH_PHY_RHEADER(phyad, regad)                      /* PRQA S 3472 # Advisory D-4.9 */ \
    (0x1800UL | ((uint32_t) (phyad) << 5UL) | ((uint32_t) (regad)))

/* Create a header for write operation */
#define ETH_PHY_WHEADER(phyad, regad)                      /* PRQA S 3472 # Advisory D-4.9 */ \
    (0x1400UL | ((uint32_t) (phyad) << 5UL) | ((uint32_t) (regad)))

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

#if (ETHER_PHY_CFG_TARGET_LAN8700_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
/* PRQA S 1513 ++ # DP_MSR_R-8.5_UC1 */
extern void ether_phy_target_lan8700_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
extern bool ether_phy_target_lan8700_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                     uint32_t                    line_speed_duplex);

/* PRQA S 1513 -- # DP_MSR_R-8.5_UC1 */
#endif

#if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
/* PRQA S 1513 ++ # DP_MSR_R-8.5_UC1 */
extern void ether_phy_target_ct25205_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);

/* PRQA S 1513 -- */
#endif

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
extern void ether_phy_target_88e1112_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
extern bool ether_phy_target_88e1112_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                     uint32_t                    line_speed_duplex);

#endif

#if (!ETHER_PHY_CFG_BYPASS_ENABLE)
static void ether_phy_targets_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
static bool ether_phy_targets_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                              uint32_t                    line_speed_duplex);

#endif
static bool r_ether_phy_tsn_hw_write_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                         uint32_t                    trcv_idx,
                                         uint32_t                    reg_idx,
                                         uint32_t                    reg_val);
static bool r_ether_phy_tsn_hw_read_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                        uint32_t                    trcv_idx,
                                        uint32_t                    reg_idx,
                                        uint32_t * const            p_reg_val);

static bool r_ether_phy_tsnswa_hw_write_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                            uint32_t                    trcv_idx,
                                            uint32_t                    reg_idx,
                                            uint32_t                    reg_val);
static bool r_ether_phy_tsnswa_hw_read_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                           uint32_t                    trcv_idx,
                                           uint32_t                    reg_idx,
                                           uint32_t * const            p_reg_val);

static uint32_t r_ether_phy_read_45(ether_phy_ctrl_t * p_ctrl, uint8_t ctrl_idx, uint32_t trcv_idx,
                                    uint32_t reg_addr_45);
static void r_ether_phy_write_45(ether_phy_ctrl_t * p_ctrl,
                                 uint8_t            ctrl_idx,
                                 uint32_t           trcv_idx,
                                 uint32_t           reg_addr_45,
                                 uint16_t           reg_val);
static bool r_ether_pir_write_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                  uint8_t                     ctrl_idx,
                                  uint32_t                    trcv_idx,
                                  uint32_t                    reg_idx,
                                  uint32_t                    reg_val);
static bool r_ether_pir_read_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                 uint8_t                     ctrl_idx,
                                 uint32_t                    trcv_idx,
                                 uint32_t                    reg_idx,
                                 uint32_t                  * reg_val_ptr);
static void     r_ether_pir_hw_write_mii_bit(ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx, const uint32_t bit);
static uint32_t r_ether_pir_hw_read_mii_bit(ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx);
static void     r_ether_pir_wait_nano_second(uint32_t nano_sec);

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
static uint8_t r_ether_phy_get_operation_mode(ether_phy_instance_ctrl_t * p_instance_ctrl);
static void    r_ether_phy_set_operation_mode(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                              uint32_t                    channel,
                                              uint8_t                     mode);
static uint32_t r_ether_phy_calculate_mpic(ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t line_speed_duplex);

#endif

/** ETHER_PHY HAL API mapping for Ethernet PHY Controller interface */
const ether_phy_api_t g_ether_phy_on_ether_phy =                /* PRQA S 1502 # Advisory R-2.8 */
{
    .open                  = R_ETHER_PHY_Open,                  /* PRQA S 3635 # Advisory R-17.12 */
    .close                 = R_ETHER_PHY_Close,                 /* PRQA S 3635 # Advisory R-17.12 */
    .startAutoNegotiate    = R_ETHER_PHY_StartAutoNegotiate,    /* PRQA S 3635 # Advisory R-17.12 */
    .linkPartnerAbilityGet = R_ETHER_PHY_LinkPartnerAbilityGet, /* PRQA S 3635 # Advisory R-17.12 */
    .linkStatusGet         = R_ETHER_PHY_LinkStatusGet,         /* PRQA S 3635 # Advisory R-17.12 */
    .chipInit              = R_ETHER_PHY_ChipInit,              /* PRQA S 3635 # Advisory R-17.12 */
    .read                  = R_ETHER_PHY_Read,                  /* PRQA S 3635 # Advisory R-17.12 */
    .write                 = R_ETHER_PHY_Write                  /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/********************************************************************************************************************//**
 * @brief Resets Ethernet PHY device. Implements @ref ether_phy_api_t::open.
 *
 * @retval  FSP_SUCCESS                             Channel opened successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_PHY control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN                    Control block has already been opened or channel is being used by another
 *                                                  instance. Call close() then open() to reconfigure.
 * @retval  FSP_ERR_INVALID_CHANNEL                 Invalid channel number is given.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer to p_cfg is NULL.
 * @retval  FSP_ERR_TIMEOUT                         PHY-LSI Reset wait timeout.
 * @retval  FSP_ERR_INVALID_ARGUMENT                Register address is incorrect
 * @retval  FSP_ERR_NOT_INITIALIZED                 The control block has not been initialized.
 * @retval  FSP_ERR_INVALID_MODE                    Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open (ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;           /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN((ETHER_PHY_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
#endif

    /* Initialize configuration of Ethernet PHY module. */
    p_instance_ctrl->p_ether_phy_cfg = p_cfg;
    ether_phy_extended_cfg_t const * p_extend =
        ((ether_phy_extended_cfg_t const *) p_instance_ctrl->p_ether_phy_cfg->p_extend); /* PRQA S 0316 # Advisory R-11.5 */

    if (ETHER_PHY_ETH_REG_TSN == p_extend->eth_reg)
    {
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
        R_RMACA0_Type const * p_reg_rmaca;

        /* Make sure this channel exists. */
        p_reg_rmaca =
            (R_RMACA0_Type *) (R_RMACA0_BASE + (ETHER_RMACAn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel)); /* PRQA S 0306 # Advisory R-11.4 */

        p_instance_ctrl->p_reg_mpsm  = (uint32_t *) &p_reg_rmaca->MPSM;                                            /* PRQA S 0311, 3305, 0312,0662 # Advisory R-11.8, Advisory R-11.3, DP_MSR_R-11.8_UC1,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg_mmis1 = (uint32_t *) &p_reg_rmaca->MMIS1;                                           /* PRQA S 0310, 0311, 0312, 3305,0662 # DP_MSR_R-11.3_UC1, Advisory R-11.8, DP_MSR_R-11.8_UC1, Advisory R-11.3,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */
#else

        return FSP_ERR_INVALID_MODE;
#endif                                                                                                             /* BSP_FEATURE_ETND_IS_AVAILABLE */
    }
    else if (ETHER_PHY_ETH_REG_T1S == p_extend->eth_reg)
    {
#if (BSP_FEATURE_ETNF_IS_AVAILABLE)
        R_ETNF0_Type const * p_reg_etnf;

        p_reg_etnf = (R_ETNF0_Type *) R_ETNF0_BASE;                 /* PRQA S 0306 # Advisory R-11.4 */

        p_instance_ctrl->p_reg_pir = (uint32_t *) &p_reg_etnf->PIR; /* PRQA S 0311, 0312,0662 # Advisory R-11.8, DP_MSR_R-11.8_UC1,DP_MSR_D-1.1_UC1,DP_MSR_R-1.2_UC1 */
#else

        return FSP_ERR_INVALID_MODE;
#endif                                                              /* BSP_FEATURE_ETNF_IS_AVAILABLE */
    }
    else if (ETHER_PHY_ETH_REG_AVB == p_extend->eth_reg)
    {
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)                                 /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        R_ETNB0_Type * p_reg_etnb;

        p_reg_etnb = (R_ETNB0_Type *) (R_ETNB0_BASE + (ETHER_AVBn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel));

        p_instance_ctrl->p_reg_pir = (uint32_t *) &p_reg_etnb->PIR;
#else

        return FSP_ERR_INVALID_MODE;
#endif                                 /* BSP_FEATURE_ETNB_IS_AVAILABLE */
    }
    else if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        R_ETNE0_RMAC0_Type * p_reg_rmac;
        uint32_t             link_speed = ETHER_PHY_LINK_SPEED_10F;

        /* ETHA IP should be CONFIG mode */
        FSP_ERROR_RETURN(ETHER_PHY_ETHA_CONFIG_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                         FSP_ERR_INVALID_MODE);

        p_reg_rmac =
            (R_ETNE0_RMAC0_Type *) (R_ETNE0_RMAC0_BASE +
                                    (ETHER_RMACn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel));

        p_instance_ctrl->p_reg_mpsm  = (uint32_t *) &p_reg_rmac->MPSM;
        p_instance_ctrl->p_reg_mmis0 = (uint32_t *) &p_reg_rmac->MMIS0;
        p_instance_ctrl->p_reg_mmie0 = (uint32_t *) &p_reg_rmac->MMIE0;
        p_instance_ctrl->p_reg_mpic  = (uint32_t *) &p_reg_rmac->MPIC;
        p_instance_ctrl->p_reg_mlvc  = (uint32_t *) &p_reg_rmac->MLVC;
        p_instance_ctrl->p_reg_mioc  = (uint32_t *) &p_reg_rmac->MIOC;

        /* Initialize local advertise bitmap. */
        p_instance_ctrl->local_advertise = 0;

        /* Configure maximum link speed for each interface. */
        if ((ETHER_PHY_MII_TYPE_MII == p_instance_ctrl->p_ether_phy_cfg->mii_type) ||
            (ETHER_PHY_MII_TYPE_RMII == p_instance_ctrl->p_ether_phy_cfg->mii_type))
        {
            link_speed = ETHER_PHY_LINK_SPEED_100F;
        }
        else if ((ETHER_PHY_MII_TYPE_GMII == p_instance_ctrl->p_ether_phy_cfg->mii_type) ||
                 (ETHER_PHY_MII_TYPE_RGMII == p_instance_ctrl->p_ether_phy_cfg->mii_type))
        {
            link_speed = ETHER_PHY_LINK_SPEED_1000F;
        }
        else                           /* PRQA S 2013 # DP_MSR_R-15.7_UC1 */
        {
            /* Do nothing */
        }

        /* PHY Interface configuration */
        *p_instance_ctrl->p_reg_mpic = r_ether_phy_calculate_mpic(p_instance_ctrl, link_speed);
#else

        return FSP_ERR_INVALID_MODE;
#endif                                 /* BSP_FEATURE_ETNE_IS_AVAILABLE */
    }
    else
    {
        /* Do nothing */
        ;
    }

#if ETHER_PHY_CFG_BYPASS_ENABLE

    /* Bypass the PHY-LSI reset/initialization process entirely. Only read()/write() remain functional;
     * the application (or a user-provided MDIO transport) is responsible for the PHY-LSI, if any. */
    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_INITIALIZED;
    p_instance_ctrl->open             = ETHER_PHY_OPEN;

    err = FSP_SUCCESS;
#elif ETHER_PHY_CFG_INIT_PHY_LSI_AUTOMATIC
 #if (BSP_FEATURE_ETNE_IS_AVAILABLE)   /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
 #endif

    uint32_t reg   = 0;
    uint32_t count = 0;

    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_INITIALIZED;

    if (ETHER_PHY_LSI_TYPE_CUSTOM == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    else if (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }
 #endif
    else
    {
        /* Reset PHY */
        err = R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Reset completion waiting */
        do
        {
            err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
            count++;
        } while ((reg & ETHER_PHY_CONTROL_RESET) && (count < p_cfg->phy_reset_wait_time)); /* PRQA S 4558 # DP_MSR_R-10.1_UC1 */

        if (count < p_cfg->phy_reset_wait_time)
        {
            ether_phy_targets_initialize(p_instance_ctrl);

            p_instance_ctrl->open = ETHER_PHY_OPEN;

            err = FSP_SUCCESS;
        }
        else
        {
            err = FSP_ERR_TIMEOUT;
        }
    }
#else
    p_instance_ctrl->open = ETHER_PHY_OPEN;

    err = FSP_SUCCESS;
#endif

    return err;
}                                                                                       /* End of function R_ETHER_PHY_Open() */

/********************************************************************************************************************//**
 * @brief Close Ethernet PHY device. Implements @ref ether_phy_api_t::close.
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_INVALID_MODE                        Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Close (ether_phy_ctrl_t * const p_ctrl)                           /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSNSWA)
    {
        /* ETHA IP should be DISABLE mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_DISABLE_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

    /** Clear configure block parameters. */
    p_instance_ctrl->p_ether_phy_cfg = NULL;
    p_instance_ctrl->local_advertise = 0;

#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    p_instance_ctrl->p_reg_mpsm  = NULL;
    p_instance_ctrl->p_reg_mmis1 = NULL;
#endif                                 /* BSP_FEATURE_ETND_IS_AVAILABLE */

#if (BSP_FEATURE_ETNF_IS_AVAILABLE || BSP_FEATURE_ETNB_IS_AVAILABLE)
    p_instance_ctrl->p_reg_pir = NULL;
#endif                                 /* BSP_FEATURE_ETNF_IS_AVAILABLE || BSP_FEATURE_ETNB_IS_AVAILABLE */

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    p_instance_ctrl->p_reg_mpsm  = NULL;
    p_instance_ctrl->p_reg_mmis0 = NULL;
    p_instance_ctrl->p_reg_mmie0 = NULL;
    p_instance_ctrl->p_reg_mpic  = NULL;
    p_instance_ctrl->p_reg_mlvc  = NULL;
    p_instance_ctrl->p_reg_mioc  = NULL;
#endif                                 /* BSP_FEATURE_ETNE_IS_AVAILABLE */

    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_UNINITIALIZED;
    p_instance_ctrl->open             = 0;

    return err;
}                                                                                       /* End of function R_ETHER_PHY_Close() */

/********************************************************************************************************************//**
 * @brief Starts auto-negotiate. Implements @ref ether_phy_api_t::startAutoNegotiate.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully starts auto-negotiate.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Register address is incorrect
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to read buffer is NULL.
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 * @retval  FSP_ERR_INVALID_MODE                        Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_StartAutoNegotiate (ether_phy_ctrl_t * const p_ctrl)              /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t  reg = 0;
    fsp_err_t err = FSP_SUCCESS;
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

#if ETHER_PHY_CFG_BYPASS_ENABLE

    /* Bypass auto-negotiation entirely. Only read()/write() remain functional; the application (or a
     * user-provided MDIO transport) is responsible for the PHY-LSI, if any. */
    FSP_PARAMETER_NOT_USED(reg);
    FSP_PARAMETER_NOT_USED(err);
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);

    return FSP_SUCCESS;
#else
    if (ETHER_PHY_LSI_TYPE_CUSTOM == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;
    }

 #if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
    else if (ETHER_PHY_LSI_TYPE_CT25205_RTL == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        /* ANAB (Auto-Negotiation Ability), the CT25205 PHY does NOT support the Auto-Negotiation feature.
         * However, this bit is enabled to allow the PHY to be managed by standard software drivers with no changes. */
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg); /* Bit 3 ANAB (Auto-Negotiation Ability) is enabled*/
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        return err;
    }
 #endif                                /* ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE */
    else
    {
        /* Set local ability */
        /* When pause frame is not used */
        if (ETHER_PHY_FLOW_CONTROL_DISABLE == p_instance_ctrl->p_ether_phy_cfg->flow_control)
        {
            p_instance_ctrl->local_advertise = ((((ETHER_PHY_AN_ADVERTISEMENT_100F |
                                                   ETHER_PHY_AN_ADVERTISEMENT_100H) |
                                                  ETHER_PHY_AN_ADVERTISEMENT_10F) |
                                                 ETHER_PHY_AN_ADVERTISEMENT_10H) |
                                                ETHER_PHY_AN_ADVERTISEMENT_SELECTOR);
        }
        /* When pause frame is used */
        else
        {
            p_instance_ctrl->local_advertise = ((((((ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR |
                                                     ETHER_PHY_AN_ADVERTISEMENT_PAUSE) |
                                                    ETHER_PHY_AN_ADVERTISEMENT_100F) |
                                                   ETHER_PHY_AN_ADVERTISEMENT_100H) |
                                                  ETHER_PHY_AN_ADVERTISEMENT_10F) |
                                                 ETHER_PHY_AN_ADVERTISEMENT_10H) |
                                                ETHER_PHY_AN_ADVERTISEMENT_SELECTOR);
        }

        /* Configure what the PHY and the Ethernet controller on this board supports */
        err = R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, p_instance_ctrl->local_advertise);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Set local ability for SGMII */
 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        if (

            (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type) ||

            (ETHER_PHY_MII_TYPE_RGMII == p_instance_ctrl->p_ether_phy_cfg->mii_type) ||
            (ETHER_PHY_MII_TYPE_GMII == p_instance_ctrl->p_ether_phy_cfg->mii_type))
        {
            /* Select Copper Page */
            err = R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_PAGE_ADDRESS, 0U);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
            err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_1000BASET_CONTROL, &reg);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
            err =
                R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_1000BASET_CONTROL,
                                  (reg | ETHER_PHY_AN_ADVERTISEMENT_1000F | ETHER_PHY_AN_ADVERTISEMENT_1000H));
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
        }
 #endif                                /* ETHER_PHY_CFG_TARGET_88E1112_ENABLE */
        err = R_ETHER_PHY_Write(p_instance_ctrl,
                                ETHER_PHY_REG_CONTROL,
                                (ETHER_PHY_CONTROL_AN_ENABLE |
                                 ETHER_PHY_CONTROL_AN_RESTART));
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Read register to confirm data just write above is correct  */
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
    }

    return FSP_SUCCESS;
#endif                                                                        /* ETHER_PHY_CFG_BYPASS_ENABLE */
}                                                                             /* End of function R_ETHER_PHY_StartAutoNegotiate() */

/********************************************************************************************************************//**
 * @brief Reports the other side's physical capability. Implements @ref ether_phy_api_t::linkPartnerAbilityGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to arguments are NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 * @retval  FSP_ERR_ETHER_PHY_NOT_READY                 The auto-negotiation isn't completed
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Status register address is incorrect
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 * @retval  FSP_ERR_INVALID_MODE                        Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet (ether_phy_ctrl_t * const p_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                                             uint32_t * const         p_line_speed_duplex,
                                             uint32_t * const         p_local_pause,
                                             uint32_t * const         p_partner_pause)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t reg               = 0;                                                     /* PRQA S 1710 # Advisory D-4.5 */
    uint32_t line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_NO_LINK;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(NULL != p_line_speed_duplex, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_local_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_partner_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

#if ETHER_PHY_CFG_BYPASS_ENABLE

    /* Bypass the link partner ability decoding entirely and report a fixed default ability. Only
     * read()/write() remain functional; the application (or a user-provided MDIO transport) is
     * responsible for the PHY-LSI, if any. line_speed_duplex is still assigned here (rather than
     * just the output pointer) because it also feeds r_ether_phy_calculate_mpic() below when
     * BSP_FEATURE_ETNE_IS_AVAILABLE is set. */
    line_speed_duplex      = (uint32_t) ETHER_PHY_LINK_SPEED_100F;
    (*p_line_speed_duplex) = (uint32_t) ETHER_PHY_LINK_SPEED_100F;
    (*p_local_pause)       = 0U;
    (*p_partner_pause)     = 0U;
    FSP_PARAMETER_NOT_USED(reg);
    FSP_PARAMETER_NOT_USED(line_speed_duplex);
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#else
    if (ETHER_PHY_LSI_TYPE_CUSTOM == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

 #if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
    else if (ETHER_PHY_LSI_TYPE_CT25205_RTL == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        /* Only support T1S mode (Half-duplex mode) in CT25205-RTL */
        (*p_line_speed_duplex) = (uint32_t) ETHER_PHY_LINK_SPEED_10H;

        err = FSP_SUCCESS;
    }
 #endif
    else
    {
        /* Because reading the first time shows the previous state, the Link status bit is read twice. */
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* When the link isn't up, return error */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_LINK_UP == (reg & ETHER_PHY_STATUS_LINK_UP),
                               FSP_ERR_ETHER_PHY_ERROR_LINK);

        /* Establish local pause capability */
        if (ETHER_PHY_AN_ADVERTISEMENT_PAUSE ==
            (p_instance_ctrl->local_advertise & ETHER_PHY_AN_ADVERTISEMENT_PAUSE))
        {
            (*p_local_pause) |= (ETHER_PHY_AN_ADVERTISEMENT_SUPPORT << ETHER_PHY_AN_ADVERTISEMENT_PAUSE_POS);
        }

        if (ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR ==
            (p_instance_ctrl->local_advertise & ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR))
        {
            (*p_local_pause) |= (ETHER_PHY_AN_ADVERTISEMENT_SUPPORT << ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR_POS);
        }

        /* When the auto-negotiation isn't completed, return error */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_STATUS_AN_COMPLETE == (reg & ETHER_PHY_STATUS_AN_COMPLETE),
                               FSP_ERR_ETHER_PHY_NOT_READY);

        /* Get the link partner response */
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_AN_LINK_PARTNER, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Establish partner pause capability */
        if (ETHER_PHY_AN_LINK_PARTNER_PAUSE == (reg & ETHER_PHY_AN_LINK_PARTNER_PAUSE))
        {
            (*p_partner_pause) = (ETHER_PHY_AN_LINK_PARTNER_SUPPORT << ETHER_PHY_AN_LINK_PARTNER_PAUSE_POS);
        }

        if (ETHER_PHY_AN_LINK_PARTNER_ASM_DIR == (reg & ETHER_PHY_AN_LINK_PARTNER_ASM_DIR))
        {
            (*p_partner_pause) |= (ETHER_PHY_AN_LINK_PARTNER_SUPPORT << ETHER_PHY_AN_LINK_PARTNER_ASM_DIR_POS);
        }

        /* Establish the line speed and the duplex */
        if ((ETHER_PHY_AN_LINK_PARTNER_10H == (reg & ETHER_PHY_AN_LINK_PARTNER_10H)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, (uint32_t) ETHER_PHY_LINK_SPEED_10H)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
        {
            line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_10H;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_10F == (reg & ETHER_PHY_AN_LINK_PARTNER_10F)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, (uint32_t) ETHER_PHY_LINK_SPEED_10F)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
        {
            line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_10F;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_100H == (reg & ETHER_PHY_AN_LINK_PARTNER_100H)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, (uint32_t) ETHER_PHY_LINK_SPEED_100H)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
        {
            line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_100H;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_100F == (reg & ETHER_PHY_AN_LINK_PARTNER_100F)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, (uint32_t) ETHER_PHY_LINK_SPEED_100F)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
        {
            line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_100F;
        }

        if (
 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
            (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type) ||
 #endif

            /* When MII type is RGMII or GMII, check also Gigabit Ethernet ability. */
            (ETHER_PHY_MII_TYPE_RGMII == p_instance_ctrl->p_ether_phy_cfg->mii_type) ||
            (ETHER_PHY_MII_TYPE_GMII == p_instance_ctrl->p_ether_phy_cfg->mii_type))
        {
 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
            /* Select Copper Page */
            err = R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_PAGE_ADDRESS, 0U);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
 #endif

            /* Get the link partner response */
            err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_1000BASET_STATUS, &reg);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

            if ((ETHER_PHY_AN_LINK_PARTNER_1000H == (reg & ETHER_PHY_AN_LINK_PARTNER_1000H)) &&
                ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl,                        /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
                                                                  (uint32_t) ETHER_PHY_LINK_SPEED_1000H)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
            {
                line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_1000H;
            }

            if ((ETHER_PHY_AN_LINK_PARTNER_1000F == (reg & ETHER_PHY_AN_LINK_PARTNER_1000F)) &&
                ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl,                        /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
                                                                  (uint32_t) ETHER_PHY_LINK_SPEED_1000F)) /* PRQA S 3415 # DP_MSR_R-13.5_UC1 */
            {
                line_speed_duplex = (uint32_t) ETHER_PHY_LINK_SPEED_1000F;
            }
        }

        if ((uint32_t) ETHER_PHY_LINK_SPEED_NO_LINK == line_speed_duplex)
        {
            err = FSP_ERR_ETHER_PHY_ERROR_LINK;
        }
        else
        {
            (*p_line_speed_duplex) = line_speed_duplex;
        }
    }
#endif                                 /* ETHER_PHY_CFG_BYPASS_ENABLE */

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        uint32_t mpic = r_ether_phy_calculate_mpic(p_instance_ctrl, line_speed_duplex);
        if (mpic != *p_instance_ctrl->p_reg_mpic)
        {
            /* Set ETHA to CONFIG mode */
            r_ether_phy_set_operation_mode(p_instance_ctrl,
                                           p_instance_ctrl->p_ether_phy_cfg->channel,
                                           ETHER_PHY_ETHA_DISABLE_MODE);
            r_ether_phy_set_operation_mode(p_instance_ctrl,
                                           p_instance_ctrl->p_ether_phy_cfg->channel,
                                           ETHER_PHY_ETHA_CONFIG_MODE);

            *p_instance_ctrl->p_reg_mpic = mpic;

            /* Set ETHA to OPERATION mode */
            r_ether_phy_set_operation_mode(p_instance_ctrl,
                                           p_instance_ctrl->p_ether_phy_cfg->channel,
                                           ETHER_PHY_ETHA_DISABLE_MODE);
            r_ether_phy_set_operation_mode(p_instance_ctrl,
                                           p_instance_ctrl->p_ether_phy_cfg->channel,
                                           ETHER_PHY_ETHA_OPERATION_MODE);
        }
    }
#endif

    return err;
}                                                                                       /* End of function R_ETHER_PHY_LinkPartnerAbilityGet() */

/********************************************************************************************************************//**
 * @brief Returns the status of the physical link. Implements @ref ether_phy_api_t::linkStatusGet.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully get link partner ability.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_ETHER_PHY_ERROR_LINK                PHY-LSI is not link up.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Status register address is incorrect
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to read buffer is NULL.
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 * @retval  FSP_ERR_INVALID_MODE                        Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkStatusGet (ether_phy_ctrl_t * const p_ctrl)                   /* PRQA S 1505 # Advisory R-8.7 */
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t  reg = 0;
    fsp_err_t err = FSP_SUCCESS;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

#if ETHER_PHY_CFG_BYPASS_ENABLE

    /* Bypass the link status check entirely and always report link up. Only read()/write() remain
     * functional; the application (or a user-provided MDIO transport) is responsible for the
     * PHY-LSI, if any. */
    FSP_PARAMETER_NOT_USED(reg);
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#else
    if (ETHER_PHY_LSI_TYPE_CUSTOM != p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        /* Because reading the first time shows the previous state, the Link status bit is read twice. */
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
        err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* When the link isn't up, return error */
        if (ETHER_PHY_STATUS_LINK_UP != (reg & ETHER_PHY_STATUS_LINK_UP))
        {
            /* Link is down */
            err = FSP_ERR_ETHER_PHY_ERROR_LINK;
        }
        else
        {
 #if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
            if (ETHER_PHY_LSI_TYPE_CT25205_RTL == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
            {
                if ((reg == ETHER_PHY_CT25205_RTL_INVALID_STATE1) ||
                    (reg == ETHER_PHY_CT25205_RTL_INVALID_STATE2))
                {
                    err = FSP_ERR_ETHER_PHY_ERROR_LINK;
                }
                else
                {
                    /* Link is up */
                    err = FSP_SUCCESS;
                }
            }
            else
            {
                /* Link is up */
                err = FSP_SUCCESS;
            }

 #else

            /* Link is up */
            err = FSP_SUCCESS;
 #endif                                /* ETHER_PHY_LSI_TYPE_CT25205_RTL */
        }
    }
    else
    {
        /* Custom PHY need to be design and confirm by User */
        /* Link is up */
        err = FSP_SUCCESS;
    }
#endif                                                                                                /* ETHER_PHY_CFG_BYPASS_ENABLE */

    return err;
}                                                                                                     /* End of function R_ETHER_PHY_LinkStatusGet() */

/********************************************************************************************************************//**
 * @brief Initialize Ethernet PHY device. Implements @ref ether_phy_api_t::chipInit.
 *
 * @retval  FSP_SUCCESS                             PHY device initialized successfully.
 * @retval  FSP_ERR_ASSERTION                       Pointer to ETHER_PHY control block or configuration structure is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                Address or data is not a valid size.
 * @retval  FSP_ERR_INVALID_POINTER                 Pointer to p_cfg is NULL.
 * @retval  FSP_ERR_NOT_INITIALIZED                 The control block has not been initialized.
 * @retval  FSP_ERR_NOT_OPEN                        The control block has not been opened.
 * @retval  FSP_ERR_TIMEOUT                         PHY-LSI Reset wait timeout.
 * @retval  FSP_ERR_INVALID_MODE                    Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_ChipInit (ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;               /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t reg   = 0;
    uint32_t count = 0;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_INITIALIZED;

#if ETHER_PHY_CFG_BYPASS_ENABLE

    /* Bypass the PHY-LSI reset/initialization process entirely. Only read()/write() remain functional;
     * the application (or a user-provided MDIO transport) is responsible for the PHY-LSI, if any. */
    FSP_PARAMETER_NOT_USED(reg);
    FSP_PARAMETER_NOT_USED(count);

    err = FSP_SUCCESS;
#else
    if (ETHER_PHY_LSI_TYPE_CUSTOM == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    else if (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }
 #endif
    else
    {
        /* Reset PHY */
        err = R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);
        ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);

        /* Reset completion waiting */
        do
        {
            err = R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg);
            ETHER_PHY_ERROR_RETURN(FSP_SUCCESS == err, err);
            count++;
        } while ((reg & ETHER_PHY_CONTROL_RESET) && (count < p_cfg->phy_reset_wait_time)); /* PRQA S 4558 # DP_MSR_R-10.1_UC1 */

        if (count < p_cfg->phy_reset_wait_time)
        {
            ether_phy_targets_initialize(p_instance_ctrl);

            err = FSP_SUCCESS;
        }
        else
        {
            err = FSP_ERR_TIMEOUT;
        }
    }
#endif                                 /* ETHER_PHY_CFG_BYPASS_ENABLE */

    return err;
}                                      /* End of function R_ETHER_PHY_ChipInit() */

/********************************************************************************************************************//**
 * @brief Read data from register of PHY-LSI . Implements @ref ether_phy_api_t::read.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully read data.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to read buffer is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Address is not a valid size
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Read (ether_phy_ctrl_t * const p_ctrl, uint32_t reg_addr, uint32_t * const p_data)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_data, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_phy_cfg->channel;
    ether_phy_extended_cfg_t const * p_extend =                                                                        /* PRQA S 0316 # Advisory R-11.5 */
                                                ((ether_phy_extended_cfg_t const *) p_instance_ctrl->p_ether_phy_cfg-> /* PRQA S 0316 # Advisory R-11.5 */
                                                 p_extend);
    uint32_t trcv_idx = p_extend->p_phy_lsi_cfg_list->address;

    if (NULL != p_extend->p_mdio_read)
    {
        /* User-provided MDIO transport (for example, bit-banged MDIO/MDC on GPIO pins) bypasses the hardware
         * MDIO controller entirely. */
        p_extend->p_mdio_read((uint8_t) trcv_idx, reg_addr, p_data);

        return FSP_SUCCESS;
    }

    if (reg_addr < ETH_PHY_MAX_CLAUSE22_REGISTER)
    {
        if (ETHER_PHY_ETH_REG_TSN == p_extend->eth_reg)
        {
            ETHER_PHY_ERROR_RETURN(r_ether_phy_tsn_hw_read_mii(p_instance_ctrl, trcv_idx, reg_addr, p_data),
                                   FSP_ERR_TIMEOUT);
        }
        else if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
        {
            ETHER_PHY_ERROR_RETURN(r_ether_phy_tsnswa_hw_read_mii(p_instance_ctrl, trcv_idx, reg_addr, p_data),
                                   FSP_ERR_TIMEOUT);
        }
        else
        {
            r_ether_pir_read_mii(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr, p_data); /* PRQA S 3200 # Required R-17.7 */
        }
    }
    else
    {
        if (ETHER_PHY_ETH_REG_TSN == p_extend->eth_reg)
        {
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
            ETHER_PHY_ERROR_RETURN(ETHER_PHY_ADDRESS_SIZE >= reg_addr, FSP_ERR_INVALID_ARGUMENT);
#endif
        }
        else if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
        {
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
            ETHER_PHY_ERROR_RETURN(ETHER_PHY_ADDRESS_SIZE >= reg_addr, FSP_ERR_INVALID_ARGUMENT);
#endif
        }
        else
        {
            *p_data = r_ether_phy_read_45(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr);
        }
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_Read() */

/********************************************************************************************************************//**
 * @brief Write data to register of PHY-LSI . Implements @ref ether_phy_api_t::write.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully write data.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Address or data is not a valid size
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 * @retval  FSP_ERR_INVALID_MODE                        Invalid mode to use Ethernet register.
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Write (ether_phy_ctrl_t * const p_ctrl, uint32_t reg_addr, uint32_t data)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_REGISTER_DATA_SIZE >= data, FSP_ERR_INVALID_ARGUMENT);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_phy_cfg->channel;
    ether_phy_extended_cfg_t const * p_extend =                                                                        /* PRQA S 0316 # Advisory R-11.5 */
                                                ((ether_phy_extended_cfg_t const *) p_instance_ctrl->p_ether_phy_cfg-> /* PRQA S 0316 # Advisory R-11.5 */
                                                 p_extend);

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)                                                                                    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
    if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
    {
        /* ETHA IP should be OPERATION mode */
        ETHER_PHY_ERROR_RETURN(ETHER_PHY_ETHA_OPERATION_MODE == r_ether_phy_get_operation_mode(p_instance_ctrl),
                               FSP_ERR_INVALID_MODE);
    }
#endif

    uint32_t trcv_idx = p_extend->p_phy_lsi_cfg_list->address;

    if (NULL != p_extend->p_mdio_write)
    {
        /* User-provided MDIO transport (for example, bit-banged MDIO/MDC on GPIO pins) bypasses the hardware
         * MDIO controller entirely. */
        p_extend->p_mdio_write((uint8_t) trcv_idx, reg_addr, data);

        return FSP_SUCCESS;
    }

    if (reg_addr < ETH_PHY_MAX_CLAUSE22_REGISTER)
    {
        if (ETHER_PHY_ETH_REG_TSN == p_extend->eth_reg)
        {
            ETHER_PHY_ERROR_RETURN(r_ether_phy_tsn_hw_write_mii(p_instance_ctrl, trcv_idx, reg_addr, data),
                                   FSP_ERR_TIMEOUT);
        }
        else if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
        {
            ETHER_PHY_ERROR_RETURN(r_ether_phy_tsnswa_hw_write_mii(p_instance_ctrl, trcv_idx, reg_addr, data),
                                   FSP_ERR_TIMEOUT);
        }
        else
        {
            r_ether_pir_write_mii(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr, data); /* PRQA S 3200 # Required R-17.7 */
        }
    }
    else
    {
        if (ETHER_PHY_ETH_REG_TSN == p_extend->eth_reg)
        {
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
            ETHER_PHY_ERROR_RETURN(ETHER_PHY_ADDRESS_SIZE >= reg_addr, FSP_ERR_INVALID_ARGUMENT);
#endif
        }
        else if (ETHER_PHY_ETH_REG_TSNSWA == p_extend->eth_reg)
        {
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
            ETHER_PHY_ERROR_RETURN(ETHER_PHY_ADDRESS_SIZE >= reg_addr, FSP_ERR_INVALID_ARGUMENT);
#endif
        }
        else
        {
            r_ether_phy_write_45(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr, (uint16_t) data);
        }
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_Write() */

/*******************************************************************************************************************//**
 * @} (end addtogroup ETHER_PHY)
 **********************************************************************************************************************/

/**
 * Private functions
 */

#if (!ETHER_PHY_CFG_BYPASS_ENABLE)

/***********************************************************************************************************************
 * Function Name: ether_phy_targets_initialize
 * Description  : PHY-LSI specific initialization processing
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 * Return Value : none
 ***********************************************************************************************************************/
static void ether_phy_targets_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    switch (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
 #if (ETHER_PHY_CFG_TARGET_LAN8700_ENABLE)

        /* Use LAN8700 */
        case ETHER_PHY_LSI_TYPE_LAN8700:
        {
            ether_phy_target_lan8700_initialize(p_instance_ctrl);
            break;
        }
 #endif

 #if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)

        /* Use CT25205-RTL */
        case ETHER_PHY_LSI_TYPE_CT25205_RTL:
        {
            ether_phy_target_ct25205_initialize(p_instance_ctrl);
            break;
        }
 #endif

 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        /* Use 88E1112 */
        case ETHER_PHY_LSI_TYPE_88E1112:
        {
            ether_phy_target_88e1112_initialize(p_instance_ctrl);
            break;
        }
 #endif

        /* User custom */
 #if (ETHER_PHY_CFG_USE_CUSTOM_PHY_LSI_ENABLE)
        case ETHER_PHY_LSI_TYPE_CUSTOM:
        {
            if (NULL != p_instance_ctrl->p_ether_phy_cfg->p_extend)
            {
                ether_phy_extended_cfg_t const * p_callback = p_instance_ctrl->p_ether_phy_cfg->p_extend; /* PRQA S 0317 # Advisory R-11.5 */
                if (NULL != p_callback->p_target_init)
                {
                    p_callback->p_target_init(p_instance_ctrl);
                }
            }

            break;
        }
 #endif

        /* If module is configured for default LSI */
        default:                       /* PRQA S 2016 # DP_MSR_R-16.4_UC1 */
        {
            break;
        }
    }
}                                      /* End of function ether_phy_targets_initialize() */

/***********************************************************************************************************************
 * Function Name: ether_phy_targets_is_support_link_partner_ability
 *
 * Description  : Check if the PHY-LSI connected Ethernet controller supports link ability
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                line_speed_duplex -
 *                    Line speed duplex of link partner PHY-LSI
 * Return Value : Boolean
 ***********************************************************************************************************************/
static bool ether_phy_targets_is_support_link_partner_ability (ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                               uint32_t                    line_speed_duplex)
{
    bool result = false;
    FSP_PARAMETER_NOT_USED(line_speed_duplex);
    switch (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
 #if (ETHER_PHY_CFG_TARGET_LAN8700_ENABLE)

        /* Use LAN8700 */
        case ETHER_PHY_LSI_TYPE_LAN8700:
        {
            result = ether_phy_target_lan8700_is_support_link_partner_ability(p_instance_ctrl, line_speed_duplex);
            break;
        }
 #endif

 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE) /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */
        /* Use 88E1112 */
        case ETHER_PHY_LSI_TYPE_88E1112:
        {
            result = ether_phy_target_88e1112_is_support_link_partner_ability(p_instance_ctrl, line_speed_duplex);
            break;
        }
 #endif

        /* User custom */
 #if (ETHER_PHY_CFG_USE_CUSTOM_PHY_LSI_ENABLE)
        case ETHER_PHY_LSI_TYPE_CUSTOM:
        {
            if (NULL != p_instance_ctrl->p_ether_phy_cfg->p_extend)
            {
                ether_phy_extended_cfg_t const * p_callback = p_instance_ctrl->p_ether_phy_cfg->p_extend; /* PRQA S 0317 # Advisory R-11.5 */
                if (NULL != p_callback->p_target_link_partner_ability_get)
                {
                    result = p_callback->p_target_link_partner_ability_get(p_instance_ctrl, line_speed_duplex);
                }
            }

            break;
        }
 #endif

        /* If module is configured for default LSI, always return true */
        default:
        {
            result = true;
            break;
        }
    }

    return result;
}                                                                             /* End of function ether_phy_targets_is_support_link_partner_ability() */

#endif                                                                        /* !ETHER_PHY_CFG_BYPASS_ENABLE */

/*******************************************************************************************************************//**
 * Function Name                 r_ether_phy_tsn_hw_write_mii
 * Description                   Write data to the PHY management interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] trcv_idx           Physical init address
 * @param[in] reg_idx            register index
 * @param[in] reg_val            register value
 ***********************************************************************************************************************/
static bool r_ether_phy_tsn_hw_write_mii (ether_phy_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Rule-8.13 */
                                          uint32_t                    trcv_idx,
                                          uint32_t                    reg_idx,
                                          uint32_t                    reg_val)
{
    bool     result;
    uint32_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 3678 # Advisory R-8.13 */

    /* Write PRA, PDA to the MPSM register and and set write access direction */
    (*p_instance_ctrl->p_reg_mpsm) =
        (uint32_t) (((uint32_t) reg_val << 16UL) | ((uint32_t) trcv_idx << 3UL) | ((uint32_t) reg_idx << 8UL) |
                    ETHER_TSN_MPSM_WRITE_DATA_SET);

    /* Enable PHY register access bit */
    (*p_instance_ctrl->p_reg_mpsm) |= (uint32_t) ETHER_TSN_MPSM_WRITE_OPERATION;

    /* Wait until MPSM.PSME returns to 0 */
    do
    {
        count++;
    } while ((((*p_instance_ctrl->p_reg_mpsm) & ETHER_TSN_PHY_CONFIG_REGISTER_MASK) != 0UL) &&
             (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    if (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    (*p_instance_ctrl->p_reg_mmis1) |= ETHER_TSN_MMIS1_PWACS_MASK;

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_phy_tsn_hw_read_mii
 * Description                   Read data from the PHY management interface.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] trcv_idx           Physical Initialize address
 * @param[in] reg_idx            Register index
 * @param[in] p_reg_val          Pointer to check register value
 ***********************************************************************************************************************/
static bool r_ether_phy_tsn_hw_read_mii (ether_phy_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Rule-8.13 */
                                         uint32_t                    trcv_idx,
                                         uint32_t                    reg_idx,
                                         uint32_t * const            p_reg_val)
{
    bool     result;
    uint32_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 3678 # Advisory R-8.13 */

    (*p_instance_ctrl->p_reg_mpsm) = (uint32_t) (((uint32_t) trcv_idx << 3UL) | ((uint32_t) reg_idx << 8UL));

    /* Enable PHY register access bit */
    (*p_instance_ctrl->p_reg_mpsm) |= ETHER_TSN_MPSM_READ_OPERATION;

    /* Wait until MPSM.PSME returns to 0 */
    do
    {
        count++;
    } while ((((*p_instance_ctrl->p_reg_mpsm) & ETHER_TSN_PHY_CONFIG_REGISTER_MASK) != 0UL) &&
             (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    if (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time)
    {
        /* Get read data as return value */
        *p_reg_val = (uint16_t) ((*p_instance_ctrl->p_reg_mpsm) >> 16UL);
        result     = true;
    }
    else
    {
        *p_reg_val = (uint16_t) 0x0UL;
        result     = false;
    }

    /* Clear interrupt (MMIS1.PRACS = 1) */
    (*p_instance_ctrl->p_reg_mmis1) |= ETHER_TSN_PHY_CONFIG_REGISTER_MASK;

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_phy_tsnswa_hw_write_mii
 * Description                   Write data to the PHY management interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] trcv_idx           Physical init address
 * @param[in] reg_idx            register index
 * @param[in] reg_val            register value
 ***********************************************************************************************************************/
static bool r_ether_phy_tsnswa_hw_write_mii (ether_phy_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Rule-8.13 */
                                             uint32_t                    trcv_idx,
                                             uint32_t                    reg_idx,
                                             uint32_t                    reg_val)
{
    bool     result;
    uint32_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 3678 # Advisory R-8.13 */

    /* Write PRA, PDA to the MPSM register and and set write access direction */
    (*p_instance_ctrl->p_reg_mpsm) = (uint32_t) (((uint32_t) reg_val << TSNSWA_RMAC_MPSM_PRD_POS) |
                                                 ((uint32_t) reg_idx << TSNSWA_RMAC_MPSM_PRA_POS) |
                                                 ((uint32_t) trcv_idx << TSNSWA_RMAC_MPSM_PDA_POS) |
                                                 TSNSWA_RMAC_MPSM_POP_WRITE |
                                                 TSNSWA_RMAC_MPSM_MFF_MDIO |
                                                 TSNSWA_RMAC_MPSM_PSME);

    /* Wait until MPSM.PSME returns to 0 */
    do
    {
        count++;
    } while ((((*p_instance_ctrl->p_reg_mpsm) & TSNSWA_RMAC_MPSM_PSME) != 0UL) &&
             (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    if (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time)
    {
        result = true;
    }
    else
    {
        result = false;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_phy_tsnswa_hw_read_mii
 * Description                   Read data from the PHY management interface.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] trcv_idx           Physical Initialize address
 * @param[in] reg_idx            Register index
 * @param[in] p_reg_val          Pointer to check register value
 ***********************************************************************************************************************/
static bool r_ether_phy_tsnswa_hw_read_mii (ether_phy_instance_ctrl_t * p_ctrl, /* PRQA S 3673 # Rule-8.13 */
                                            uint32_t                    trcv_idx,
                                            uint32_t                    reg_idx,
                                            uint32_t * const            p_reg_val)
{
    bool     result = true;
    uint32_t count  = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl; /* PRQA S 3678 # Advisory R-8.13 */

    (*p_instance_ctrl->p_reg_mpsm) = (uint32_t) (((uint32_t) 0U << 16UL) |
                                                 ((uint32_t) reg_idx << 8UL) |
                                                 ((uint32_t) trcv_idx << 3UL) |
                                                 TSNSWA_RMAC_MPSM_POP_READ |
                                                 TSNSWA_RMAC_MPSM_MFF_MDIO |
                                                 TSNSWA_RMAC_MPSM_PSME);

    /* Wait until MPSM.PSME returns to 0 */
    do
    {
        count++;
    } while ((((*p_instance_ctrl->p_reg_mpsm) & TSNSWA_RMAC_MPSM_PSME) != 0UL) &&
             (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    if (count < p_ctrl->p_ether_phy_cfg->phy_reset_wait_time)
    {
        /* Get read data as return value */
        *p_reg_val = (uint16_t) ((*p_instance_ctrl->p_reg_mpsm) >> TSNSWA_RMAC_MPSM_PRD_POS);
    }
    else
    {
        *p_reg_val = (uint16_t) 0x0UL;
        result     = false;
    }

    return result;
}

/*******************************************************************************************************************//**
 * Function Name              r_ether_phy_read_45
 * Description                This function read PHY's register follow clause 45.
 * @param[in]  p_ctrl         Pointer to the control block for the channel
 * @param[in]  ctrl_idx       Index of the controller
 * @param[in]  trcv_idx       Transceiver of the controller
 * @param[in]  reg_addr_45    Register index
 ***********************************************************************************************************************/
static uint32_t r_ether_phy_read_45 (ether_phy_ctrl_t * p_ctrl,
                                     uint8_t            ctrl_idx,
                                     uint32_t           trcv_idx,
                                     uint32_t           reg_addr_45)
{
    /* Initial MMD and PHY register address based on address of parameter in */
    uint16_t mmd      = (uint16_t) (((uint16_t) (reg_addr_45 >> ETH_PHY_MMD_BIT)) & ETH_PHY_MMD_MASK);
    uint32_t reg_addr = ((uint32_t) (reg_addr_45 >> ETH_PHY_REG_ADDR_BIT)) & ETH_PHY_REG_ADDR_MASK;
    uint32_t reg_val;

    /* Select MMD Register Space */
    ETHER_PHY_ERROR_RETURN(r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, mmd),   /* PRQA S 4424,0317 # DP_MSR_R-10.3_UC1,Advisory R-11.5 */
                           FSP_ERR_INVALID_ARGUMENT);

    /* Set Clause 45 Register Address */
    ETHER_PHY_ERROR_RETURN(r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_addr), /* PRQA S 4424,0317 # DP_MSR_R-10.3_UC1,Advisory R-11.5 */
                           FSP_ERR_INVALID_ARGUMENT);

    /* Switch to Data Mode */
    ETHER_PHY_ERROR_RETURN(r_ether_pir_write_mii(p_ctrl,                                                             /* PRQA S 0317,4391,4424 # Advisory R-11.5, DP_MSR_R-10.8,DP_MSR_R-10.3_UC1 */
                                                 ctrl_idx,
                                                 trcv_idx,
                                                 ETH_PHY_MMD_ACCESS_CONTROL_REG,                                     /* PRQA S 0317 # Advisory R-11.5 */
                                                 (uint32_t) (ETH_PHY_MMD_ACCESS_BIT | mmd)),
                           FSP_ERR_INVALID_ARGUMENT);

    /* Read data from data register */
    r_ether_pir_read_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, &reg_val); /* PRQA S 3200,0317 # Required R-17.7,Advisory R-11.5 */

    return reg_val;
}

/*******************************************************************************************************************//**
 * Function Name             r_ether_phy_write_45
 * Description               This function write PHY's register follow clause 45.
 * @param[in] p_ctrl         Pointer to the control block for the channel
 * @param[in] ctrl_idx       Index of the controller
 * @param[in] trcv_idx       Transceiver of the controller
 * @param[in] reg_addr_45    Register index
 * @param[in] reg_val        Register value
 ***********************************************************************************************************************/
static void r_ether_phy_write_45 (ether_phy_ctrl_t * p_ctrl,
                                  uint8_t            ctrl_idx,
                                  uint32_t           trcv_idx,
                                  uint32_t           reg_addr_45,
                                  uint16_t           reg_val)
{
    /* Initial MMD and PHY register address based on address of parameter in */
    uint16_t mmd      = (uint16_t) (((uint16_t) (reg_addr_45 >> ETH_PHY_MMD_BIT)) & ETH_PHY_MMD_MASK);
    uint16_t reg_addr = (uint16_t) ((uint16_t) (reg_addr_45 >> ETH_PHY_REG_ADDR_BIT) & ETH_PHY_REG_ADDR_MASK);

    /* Select MMD Register Space */
    (void) r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, mmd);   /* PRQA S 0317 # Advisory R-11.5 */

    /* Set Clause 45 Register Address */
    (void) r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_addr); /* PRQA S 0317 # Advisory R-11.5 */

    /* Switch to Data Mode */
    (void) r_ether_pir_write_mii(p_ctrl,                                                             /* PRQA S 0317 # Advisory R-11.5 */
                                 ctrl_idx,
                                 trcv_idx,
                                 ETH_PHY_MMD_ACCESS_CONTROL_REG,                                     /* PRQA S 0317 # Advisory R-11.5 */
                                 (uint32_t) (ETH_PHY_MMD_ACCESS_BIT | mmd));                         /* PRQA S 4391 # DP_MSR_R-10.8 */

    /* Write data to data register */
    (void) r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_val);  /* PRQA S 0317 # Advisory R-11.5 */
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_pir_write_mii
 * Description                   This API Configures a transceiver register or triggers a function offered by the receiver by
 *                               writing the specified transceiver register through the MII of the indexed controller.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] trcv_idx           Physical init address
 * @param[in] reg_idx            register index
 * @param[in] reg_val            register value
 ***********************************************************************************************************************/
static bool r_ether_pir_write_mii (ether_phy_instance_ctrl_t * p_ctrl,
                                   uint8_t                     ctrl_idx,
                                   uint32_t                    trcv_idx,
                                   uint32_t                    reg_idx,
                                   uint32_t                    reg_val)
{
    uint32_t bit_count;
    bool     return_value;
    uint32_t header;
    uint32_t bit;
    return_value = true;
    if (ETH_PHY_MAX_PHYAD_IDX < trcv_idx)
    {
        return_value = false;
    }

    if (ETH_PHY_MAX_REGAD_IDX < reg_idx)
    {
        return_value = false;
    }

    if (return_value == true)
    {
        for (bit_count = (uint32_t) ETH_PHY_PREAMBLE_SIZE; bit_count > 0UL; bit_count--)
        {
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1U);
        }

        /* ST, OP, PHYAD, REGAD */
        header = ETH_PHY_WHEADER(trcv_idx, reg_idx); /* PRQA S 3469 # Advisory D-4.9 */
        for (bit_count = (uint32_t) ETH_PHY_HEADER_SIZE; bit_count > 0UL; bit_count--)
        {
            /* Pickup the bit which should be output next, MSB first */
            bit = (header >> (bit_count - 1UL)) & 1UL;
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
        }

        /* TA: 10 */
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1U);
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 0U);

        /* DATA */
        for (bit_count = (uint32_t) ETH_PHY_DATA_SIZE; bit_count > 0UL; bit_count--)
        {
            /* Pickup the bit which should be output next, MSB first */
            bit = ((uint32_t) reg_val >> (bit_count - 1UL)) & 1UL;
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
        }

        r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx); /* PRQA S 3200 # Required R-17.7 */
    }

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_pir_read_mii
 * Description                   This API Reads the specified transceiver register through the MII of the indexed controller.
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx   Index of the controller
 * @param[in] trcv_idx           Physical init address
 * @param[in] reg_idx            Register index
 * @param[in] reg_val_ptr        Filled with the register content of the indexed register
 ***********************************************************************************************************************/
static bool r_ether_pir_read_mii (ether_phy_instance_ctrl_t * p_ctrl,
                                  uint8_t                     ctrl_idx,
                                  uint32_t                    trcv_idx,
                                  uint32_t                    reg_idx,
                                  uint32_t                  * reg_val_ptr)
{
    uint32_t bit_count;
    uint32_t header;
    uint32_t bit;
    uint32_t reg_val;

    /* Local variable to hold the DET return value */
    bool return_value;

    /* PRE */
    for (bit_count = (uint32_t) ETH_PHY_PREAMBLE_SIZE; bit_count > 0UL; bit_count--)
    {
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1U);
    }

    /* ST, OP, PHYAD, REGAD */
    header = ETH_PHY_RHEADER(trcv_idx, reg_idx); /* PRQA S 3469 # Advisory D-4.9 */
    for (bit_count = (uint32_t) ETH_PHY_HEADER_SIZE; bit_count > 0UL; bit_count--)
    {
        /* Pickup the MSB which should be output next, MSB first */
        bit = (header >> (bit_count - 1UL)) & 1UL;
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
    }

    /* TA[0]: Turn MDIO to Z, it is same as Read cycle */
    r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx); /* PRQA S 3200 # Required R-17.7 */

    /* TA[1]: Read 0 from PHY, ignore it */
    r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx); /* PRQA S 3200 # Required R-17.7 */

    /* DATA */
    reg_val = 0U;
    for (bit_count = (uint32_t) ETH_PHY_DATA_SIZE; bit_count > 0UL; bit_count--)
    {
        /* Since read data is MSB first, shift left the result */
        reg_val = reg_val << 1UL;

        /* Read data from PHY */
        reg_val |= r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx);
    }

    *reg_val_ptr = (uint16_t) reg_val;

    return_value = true;

    return return_value;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_pir_hw_write_mii_bit
 * Description                   Write one bit to the PHY management interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of the controller
 * @param[in] bit                A bit to be written (0 or 1)
 ***********************************************************************************************************************/
static void r_ether_pir_hw_write_mii_bit (ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx, const uint32_t bit) /* PRQA S 3673 # Rule-8.13 */
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;                              /* PRQA S 3678 # Advisory R-8.13 */
    const uint32_t              mdo             = ETH_PIR_MMD | (ETH_PIR_MDO * bit);
    (void) ctrl_idx;

    (*p_instance_ctrl->p_reg_pir) = mdo;

    r_ether_pir_wait_nano_second(ETH_MDIO_SETUP_TIME + ETH_WORST_SLEW_RATE);

    (*p_instance_ctrl->p_reg_pir) = mdo | ETH_PIR_MDC;

    r_ether_pir_wait_nano_second(ETH_MDIO_SETUP_TIME + ETH_WORST_SLEW_RATE);

    (*p_instance_ctrl->p_reg_pir) = mdo;

    r_ether_pir_wait_nano_second(ETH_WORST_SLEW_RATE);
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_pir_hw_read_mii_bit
 * Description                   Read one bit from the PHY management interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] ctrl_idx           Index of a controller
 ***********************************************************************************************************************/
static uint32_t r_ether_pir_hw_read_mii_bit (ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx) /* PRQA S 3673 # Rule-8.13 */
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;             /* PRQA S 3678 # Advisory R-8.13 */
    uint32_t bit;
    (void) ctrl_idx;
    bit = ETH_GET_MDI(*p_instance_ctrl->p_reg_pir);                                                 /* PRQA S 3469 # Advisory D-4.9 */

    (*p_instance_ctrl->p_reg_pir) = ETH_PIR_MDC;

    r_ether_pir_wait_nano_second(ETH_MDIO_SETUP_TIME + ETH_WORST_SLEW_RATE);
    (*p_instance_ctrl->p_reg_pir) = 0U;

    r_ether_pir_wait_nano_second(ETH_MDIO_SETUP_TIME + ETH_WORST_SLEW_RATE);

    return bit;
}

/*******************************************************************************************************************//**
 * Function Name                 r_ether_pir_wait_nano_second
 * Description                   Wait for the specified nanosecond.
 * @param[in] nano_sec           Waiting time (nano sec)
 ***********************************************************************************************************************/
static void r_ether_pir_wait_nano_second (uint32_t nano_sec)
{
    uint32_t          cycle = ETH_NS2HZ_AVB(nano_sec) + 1U; /* PRQA S 3469 # Advisory D-4.9 */
    volatile uint32_t count;

    for (count = 0U; count < cycle; count++)                /* PRQA S 3387 # Advisory R-13.3 */
    {
        /* No action required */
    }
}

#if (BSP_FEATURE_ETNE_IS_AVAILABLE)    /* PRQA S 3332 # DP_MSR_R-20.9_UC1 */

/***********************************************************************************************************************
 * Function Name: r_ether_phy_get_operation_mode
 * Description  : Get operation mode of ETHA.
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 * Return Value : uint8_t
 ***********************************************************************************************************************/
static uint8_t r_ether_phy_get_operation_mode (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    R_ETNE0_TSNA0_Type * p_etha_reg =
        (R_ETNE0_TSNA0_Type *) (R_ETNE0_TSNA0_BASE + (ETHER_TSNAn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel));

    /* Return operation mode of ETHA IP. */
    return p_etha_reg->EAMS_b.OPS;
}

/***********************************************************************************************************************
 * Function Name: r_ether_phy_set_operation_mode
 * Description  : Change operation mode of ETHA.
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                channel -
 *                    Channel to set
 *                mode -
 *                    New operation mode
 * Return Value : void
 ***********************************************************************************************************************/
static void r_ether_phy_set_operation_mode (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t channel, uint8_t mode)
{
    volatile uint32_t count = p_instance_ctrl->p_ether_phy_cfg->phy_reset_wait_time;

    R_ETNE0_TSNA0_Type * p_etha_reg = (R_ETNE0_TSNA0_Type *) (R_ETNE0_TSNA0_BASE + (ETHER_TSNAn_OFFSET * channel));

    /* Mode transition */
    p_etha_reg->EAMC_b.OPC = TSNSWA_TSNA_EAMC_OPC_MSK & mode;

    /* Wait to change operation mode. */
    while (0 < count)
    {
        count--;
    }

    /* Emergency clock recovery if timeout occur. */
    if (mode != p_etha_reg->EAMS_b.OPS)
    {
        *p_instance_ctrl->p_reg_mioc = 0x01;
        FSP_HARDWARE_REGISTER_WAIT(p_etha_reg->EAMS_b.OPS, mode);
        *p_instance_ctrl->p_reg_mioc = 0x00;
    }
}

/***********************************************************************************************************************
 * Function Name: r_ether_phy_calculate_mpic
 * Description  : Calculate MPIC register value.
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                line_speed_duplex -
 *                    Line speed duplex of link partner PHY-LSI
 * Return Value : uint32_t
 ***********************************************************************************************************************/
static uint32_t r_ether_phy_calculate_mpic (ether_phy_instance_ctrl_t * p_instance_ctrl, uint32_t line_speed_duplex)
{
    uint32_t mpic;
    uint32_t mpic_pis;
    uint32_t mpic_lsc;

    /* Configure interface. */
    if ((ETHER_PHY_MII_TYPE_MII == p_instance_ctrl->p_ether_phy_cfg->mii_type) ||
        (ETHER_PHY_MII_TYPE_RMII == p_instance_ctrl->p_ether_phy_cfg->mii_type))
    {
        /* MII, 100Mbps */
        mpic_pis = 0;
    }
    else
    {
        /* GMII, 1000Mbps */
        mpic_pis = 2;
    }

    /* Configure link speed. */
    switch (line_speed_duplex)
    {
        case ETHER_PHY_LINK_SPEED_10H:
        case ETHER_PHY_LINK_SPEED_10F:
        {
            mpic_lsc = 0;
            break;
        }

        case ETHER_PHY_LINK_SPEED_100H:
        case ETHER_PHY_LINK_SPEED_100F:
        {
            mpic_lsc = 1;
            break;
        }

        case ETHER_PHY_LINK_SPEED_1000H:
        case ETHER_PHY_LINK_SPEED_1000F:
        {
            mpic_lsc = 2;
            break;
        }

        default:
        {
            mpic_lsc = 0;
            break;
        }
    }

    mpic = (TSNSWA_RMAC_MPIC_PIS_MSK & (mpic_pis << TSNSWA_RMAC_MPIC_PIS_POS)) |
           (TSNSWA_RMAC_MPIC_LSC_MSK & (mpic_lsc << TSNSWA_RMAC_MPIC_LSC_POS)) |
           (TSNSWA_RMAC_MPIC_FHDM_MSK & (0x0UL << TSNSWA_RMAC_MPIC_FHDM_POS)) |
           (TSNSWA_RMAC_MPIC_PSMCS_MSK & (0x13UL << TSNSWA_RMAC_MPIC_PSMCS_POS)) |
           (TSNSWA_RMAC_MPIC_PSMCT_MSK & (0x1UL << TSNSWA_RMAC_MPIC_PSMCT_POS)) |
           (TSNSWA_RMAC_MPIC_PSMHT_MSK & (0x2UL << TSNSWA_RMAC_MPIC_PSMHT_POS));

    return mpic;
}

#endif                                 /* BSP_FEATURE_ETNE_IS_AVAILABLE */

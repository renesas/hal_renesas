/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"
#include "r_ether_phy.h"

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

#ifndef ETHER_PHY_ERROR_RETURN

 #define ETHER_PHY_ERROR_RETURN(a, err)    FSP_ERROR_RETURN((a), (err))
#endif

#define ETHER_RMACAn_OFFSET                       (0x10000UL)
#define ETHER_AVBn_OFFSET                         (0x2000UL)

/** "RPHY" in ASCII.  Used to determine if the control block is open. */
#define ETHER_PHY_OPEN                            (0x52504859U)

/* Media Independent Interface */
#define ETHER_PHY_MII_ST                          (1)
#define ETHER_PHY_MII_READ                        (2)
#define ETHER_PHY_MII_WRITE                       (1)

/* Standard PHY Registers */
#define ETHER_PHY_REG_CONTROL                     (0)
#define ETHER_PHY_REG_STATUS                      (1)
#define ETHER_PHY_REG_IDENTIFIER1                 (2)
#define ETHER_PHY_REG_IDENTIFIER2                 (3)
#define ETHER_PHY_REG_AN_ADVERTISEMENT            (4)
#define ETHER_PHY_REG_AN_LINK_PARTNER             (5)
#define ETHER_PHY_REG_AN_EXPANSION                (6)
#define ETHER_PHY_REG_1000BASET_CONTROL           (9)
#define ETHER_PHY_REG_1000BASET_STATUS            (10)
#define ETHER_PHY_REG_PAGE_ADDRESS                (22)

/* Basic Mode Control Register Bit Definitions */
#define ETHER_PHY_CONTROL_RESET                   (1 << 15)
#define ETHER_PHY_CONTROL_LOOPBACK                (1 << 14)
#define ETHER_PHY_CONTROL_100_MBPS                (1 << 13)
#define ETHER_PHY_CONTROL_AN_ENABLE               (1 << 12)
#define ETHER_PHY_CONTROL_POWER_DOWN              (1 << 11)
#define ETHER_PHY_CONTROL_ISOLATE                 (1 << 10)
#define ETHER_PHY_CONTROL_AN_RESTART              (1 << 9)
#define ETHER_PHY_CONTROL_FULL_DUPLEX             (1 << 8)
#define ETHER_PHY_CONTROL_COLLISION               (1 << 7)

/* Basic Mode Status Register Bit Definitions */
#define ETHER_PHY_STATUS_100_T4                   (1 << 15)
#define ETHER_PHY_STATUS_100F                     (1 << 14)
#define ETHER_PHY_STATUS_100H                     (1 << 13)
#define ETHER_PHY_STATUS_10F                      (1 << 12)
#define ETHER_PHY_STATUS_10H                      (1 << 11)
#define ETHER_PHY_STATUS_AN_COMPLETE              (1 << 5)
#define ETHER_PHY_STATUS_RM_FAULT                 (1 << 4)
#define ETHER_PHY_STATUS_AN_ABILITY               (1 << 3)
#define ETHER_PHY_STATUS_LINK_UP                  (1 << 2)
#define ETHER_PHY_STATUS_JABBER                   (1 << 1)
#define ETHER_PHY_STATUS_EX_CAPABILITY            (1 << 0)

/* Auto Negotiation Advertisement Bit Definitions */
#define ETHER_PHY_AN_ADVERTISEMENT_NEXT_PAGE      (1 << 15)
#define ETHER_PHY_AN_ADVERTISEMENT_RM_FAULT       (1 << 13)
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR        (1 << 11)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE          (1 << 10)
#define ETHER_PHY_AN_ADVERTISEMENT_1000F          (1 << 9)
#define ETHER_PHY_AN_ADVERTISEMENT_1000H          (1 << 8)
#define ETHER_PHY_AN_ADVERTISEMENT_100_T4         (1 << 9)
#define ETHER_PHY_AN_ADVERTISEMENT_100F           (1 << 8)
#define ETHER_PHY_AN_ADVERTISEMENT_100H           (1 << 7)
#define ETHER_PHY_AN_ADVERTISEMENT_10F            (1 << 6)
#define ETHER_PHY_AN_ADVERTISEMENT_10H            (1 << 5)
#define ETHER_PHY_AN_ADVERTISEMENT_SELECTOR       (1 << 0)

/* Auto Negotiate Link Partner Ability Bit Definitions */
#define ETHER_PHY_AN_LINK_PARTNER_NEXT_PAGE       (1 << 15)
#define ETHER_PHY_AN_LINK_PARTNER_ACK             (1 << 14)
#define ETHER_PHY_AN_LINK_PARTNER_RM_FAULT        (1 << 13)
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR         (1 << 11)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE           (1 << 10)
#define ETHER_PHY_AN_LINK_PARTNER_1000F           (1 << 11)
#define ETHER_PHY_AN_LINK_PARTNER_1000H           (1 << 10)
#define ETHER_PHY_AN_LINK_PARTNER_100_T4          (1 << 9)
#define ETHER_PHY_AN_LINK_PARTNER_100F            (1 << 8)
#define ETHER_PHY_AN_LINK_PARTNER_100H            (1 << 7)
#define ETHER_PHY_AN_LINK_PARTNER_10F             (1 << 6)
#define ETHER_PHY_AN_LINK_PARTNER_10H             (1 << 5)
#define ETHER_PHY_AN_LINK_PARTNER_SELECTOR        (1 << 0)

#define ETHER_PHY_PIR_MDI_MASK                    (1 << 3)
#define ETHER_PHY_PIR_MDO_HIGH                    (0x04)
#define ETHER_PHY_PIR_MDO_LOW                     (0x00)
#define ETHER_PHY_PIR_MMD_WRITE                   (0x02)
#define ETHER_PHY_PIR_MMD_READ                    (0x00)
#define ETHER_PHY_PIR_MDC_HIGH                    (0x01)
#define ETHER_PHY_PIR_MDC_LOW                     (0x00)

#define ETHER_PHY_ADDRESS_SIZE                    (0x1fU)
#define ETHER_PHY_REGISTER_DATA_SIZE              (0xffffU)

#define ETHER_PHY_PREAMBLE_LENGTH                 (32U)
#define ETHER_PHY_WRITE_DATA_BIT_MASK             (0x8000)

/* ETH TSN PHY */
#define ETHER_TSN_MPSM_WRITE_DATA_SET             (0x2UL)
#define ETHER_TSN_MPSM_WRITE_OPERATION            (0x1UL)
#define ETHER_TSN_PHY_CONFIG_REGISTER_MASK        (0x1UL)
#define ETHER_TSN_TIMEOUT_COUNT                   (63158UL)
#define ETHER_TSN_MMIS1_PWACS_MASK                (0x02UL)
#define ETHER_TSN_MPSM_READ_OPERATION             (0x1UL)

#define ETH_PHY_MMD_BIT                           (16U)
#define ETH_PHY_MMD_MASK                          (0x001F)
#define ETH_PHY_REG_ADDR_BIT                      (0U)
#define ETH_PHY_REG_ADDR_MASK                     (0xFFFF)
#define ETH_PHY_MMD_ACCESS_CONTROL_REG            (13U)
#define ETH_PHY_MMD_ACCESS_DATA_REG               (14U)
#define ETH_PHY_MMD_ACCESS_BIT                    (0x4000)

#define ETH_CPUCLK_MHZ                            (240UL)
#define ETH_GET_MDI(regval)        (((uint32_t) (regval) >> 3UL) & 1UL)
#define ETH_NS2HZ_AVB(nano_sec)    ((ETH_CPUCLK_MHZ * (nano_sec)) / 1000UL)

#define ETH_PIR_MMD                               (0x00000002UL)
#define ETH_MDIO_SETUP_TIME                       (10UL)
#define ETH_WORST_SLEW_RATE                       (90UL)
#define ETH_PIR_MDC                               (0x00000001UL)
#define ETH_PIR_MDO                               (0x00000004UL)
#define ETH_PHY_MAX_PHYAD_IDX                     (31U)
#define ETH_PHY_MAX_REGAD_IDX                     (31U)
#define ETH_PHY_PREAMBLE_SIZE                     (32UL)
#define ETH_PHY_HEADER_SIZE                       (14UL)
#define ETH_PHY_DATA_SIZE                         (16UL)
#define ETH_PHY_MAX_CLAUSE22_REGISTER             (32U)

#define ETHER_PHY_AN_ADVERTISEMENT_SUPPORT        (1)
#define ETHER_PHY_AN_ADVERTISEMENT_PAUSE_POS      (1)
#define ETHER_PHY_AN_ADVERTISEMENT_ASM_DIR_POS    (0)

#define ETHER_PHY_AN_LINK_PARTNER_SUPPORT         (1)
#define ETHER_PHY_AN_LINK_PARTNER_PAUSE_POS       (1)
#define ETHER_PHY_AN_LINK_PARTNER_ASM_DIR_POS     (0)
#define ETHER_PHY_CT25205_RTL_INVALID_STATE1      (0x404)
#define ETHER_PHY_CT25205_RTL_INVALID_STATE2      (0x4)

#define ETHER_CHANNEL_0                           (0UL)
#define ETHER_CHANNEL_1                           (1UL)

/* Create a header for read operation */
#define ETH_PHY_RHEADER(phyad, regad) \
    (0x1800UL | ((uint32_t) (phyad) << 5UL) | ((uint32_t) (regad)))

/* Create a header for write operation */
#define ETH_PHY_WHEADER(phyad, regad) \
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
#if (ETHER_PHY_CFG_TARGET_LAN8700_ENABLE)
extern void ether_phy_target_lan8700_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
extern bool ether_phy_target_lan8700_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                     uint32_t                    line_speed_duplex);

#endif

#if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
extern void ether_phy_target_ct25205_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);

#endif

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)
extern void ether_phy_target_88e1112_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
extern bool ether_phy_target_88e1112_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                                     uint32_t                    line_speed_duplex);

#endif

static void ether_phy_targets_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
static bool ether_phy_targets_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                              uint32_t                    line_speed_duplex);
static bool r_ether_phy_tsn_hw_write_mii(ether_phy_instance_ctrl_t * p_ctrl,
                                         uint32_t                    trcv_idx,
                                         uint32_t                    reg_idx,
                                         uint32_t                    reg_val);
static bool r_ether_phy_tsn_hw_read_mii(ether_phy_instance_ctrl_t * p_ctrl,
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

/** ETHER_PHY HAL API mapping for Ethernet PHY Controller interface */
const ether_phy_api_t g_ether_phy_on_ether_phy =
{
    .open                  = R_ETHER_PHY_Open,
    .close                 = R_ETHER_PHY_Close,
    .startAutoNegotiate    = R_ETHER_PHY_StartAutoNegotiate,
    .linkPartnerAbilityGet = R_ETHER_PHY_LinkPartnerAbilityGet,
    .linkStatusGet         = R_ETHER_PHY_LinkStatusGet,
    .chipInit              = R_ETHER_PHY_ChipInit,
    .read                  = R_ETHER_PHY_Read,
    .write                 = R_ETHER_PHY_Write
};

/*******************************************************************************************************************//**
 * @addtogroup ETHER_PHY
 * @{
 **********************************************************************************************************************/

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
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Open (ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN((ETHER_PHY_OPEN != p_instance_ctrl->open), FSP_ERR_ALREADY_OPEN);
#endif

    /* Initialize configuration of Ethernet PHY module. */
    p_instance_ctrl->p_ether_phy_cfg = p_cfg;
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);

    if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSN)
    {
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
        R_RMACA0_Type * p_reg_rmaca;

        /* Make sure this channel exists. */
        p_reg_rmaca =
            (R_RMACA0_Type *) (R_RMACA0_BASE + (ETHER_RMACAn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel));

        p_instance_ctrl->p_reg_mpsm  = (uint32_t *) &p_reg_rmaca->MPSM;
        p_instance_ctrl->p_reg_mmis1 = (uint32_t *) &p_reg_rmaca->MMIS1;
#endif                                 /* BSP_FEATURE_ETND_IS_AVAILABLE */
    }
    else if (p_extend->eth_reg == ETHER_PHY_ETH_REG_T1S)
    {
#if (BSP_FEATURE_ETNF_IS_AVAILABLE)
        R_ETNF0_Type * p_reg_etnf;

        p_reg_etnf = (R_ETNF0_Type *) R_ETNF0_BASE;

        p_instance_ctrl->p_reg_pir = (uint32_t *) &p_reg_etnf->PIR;
#endif                                 /* BSP_FEATURE_ETNF_IS_AVAILABLE */
    }
    else if (p_extend->eth_reg == ETHER_PHY_ETH_REG_AVB)
    {
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)
        R_ETNB0_Type * p_reg_etnb;

        p_reg_etnb = (R_ETNB0_Type *) (R_ETNB0_BASE + (ETHER_AVBn_OFFSET * p_instance_ctrl->p_ether_phy_cfg->channel));

        p_instance_ctrl->p_reg_pir = (uint32_t *) &p_reg_etnb->PIR;
#endif                                 /* BSP_FEATURE_ETNB_IS_AVAILABLE */
    }
    else
    {
        /* Do nothing */
        ;
    }

#if ETHER_PHY_CFG_INIT_PHY_LSI_AUTOMATIC
    uint32_t reg   = 0;
    uint32_t count = 0;

    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_INITIALIZED;

    if (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type == ETHER_PHY_LSI_TYPE_CUSTOM)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

 #if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)
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
        R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);

        /* Reset completion waiting */
        do
        {
            R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg);
            count++;
        } while ((reg & ETHER_PHY_CONTROL_RESET) && (count < p_cfg->phy_reset_wait_time));

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
}                                      /* End of function R_ETHER_PHY_Open() */

/********************************************************************************************************************//**
 * @brief Close Ethernet PHY device. Implements @ref ether_phy_api_t::close.
 *
 * @retval  FSP_SUCCESS                                 Channel successfully closed.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 *
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Close (ether_phy_ctrl_t * const p_ctrl)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
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

    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_UNINITIALIZED;
    p_instance_ctrl->open             = 0;

    return err;
}                                      /* End of function R_ETHER_PHY_Close() */

/********************************************************************************************************************//**
 * @brief Starts auto-negotiate. Implements @ref ether_phy_api_t::startAutoNegotiate.
 *
 * @retval  FSP_SUCCESS                                 ETHER_PHY successfully starts auto-negotiate.
 * @retval  FSP_ERR_ASSERTION                           Pointer to ETHER_PHY control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN                            The control block has not been opened
 * @retval  FSP_ERR_INVALID_ARGUMENT                    Register address is incorrect
 * @retval  FSP_ERR_INVALID_POINTER                     Pointer to read buffer is NULL.
 * @retval  FSP_ERR_NOT_INITIALIZED                     The control block has not been initialized
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_StartAutoNegotiate (ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t reg = 0;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

    if (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type == ETHER_PHY_LSI_TYPE_CUSTOM)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;
    }

#if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
    else if (ETHER_PHY_LSI_TYPE_CT25205_RTL == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        /* ANAB (Auto-Negotiation Ability), the CT25205 PHY does NOT support the Auto-Negotiation feature.
         * However, this bit is enabled to allow the PHY to be managed by standard software drivers with no changes. */
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg); /* Bit 3 ANAB (Auto-Negotiation Ability) is enabled*/
    }
#endif /* ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE */
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
        R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, p_instance_ctrl->local_advertise);

        /* Set local ability for SGMII */
#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)
        if (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
        {
            /* Select Copper Page */
            R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_PAGE_ADDRESS, 0U);

            R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_1000BASET_CONTROL, &reg);

            R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_1000BASET_CONTROL,
                              (reg | ETHER_PHY_AN_ADVERTISEMENT_1000F | ETHER_PHY_AN_ADVERTISEMENT_1000H));
        }
#endif                                 /* ETHER_PHY_CFG_TARGET_88E1112_ENABLE */

        R_ETHER_PHY_Write(p_instance_ctrl,
                          ETHER_PHY_REG_CONTROL,
                          (ETHER_PHY_CONTROL_AN_ENABLE |
                           ETHER_PHY_CONTROL_AN_RESTART));

        /* Read register to confirm data just write above is correct  */
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_AN_ADVERTISEMENT, &reg);
    }

    return FSP_SUCCESS;
}                                      /* End of function R_ETHER_PHY_StartAutoNegotiate() */

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
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkPartnerAbilityGet (ether_phy_ctrl_t * const p_ctrl,
                                             uint32_t * const         p_line_speed_duplex,
                                             uint32_t * const         p_local_pause,
                                             uint32_t * const         p_partner_pause)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t reg               = 0;
    uint32_t line_speed_duplex = ETHER_PHY_LINK_SPEED_NO_LINK;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(NULL != p_line_speed_duplex, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_local_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(NULL != p_partner_pause, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

    if (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type == ETHER_PHY_LSI_TYPE_CUSTOM)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

#if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)
    else if (ETHER_PHY_LSI_TYPE_CT25205_RTL == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
    {
        /* Only support T1S mode (Half-duplex mode) in CT25205-RTL */
        (*p_line_speed_duplex) = ETHER_PHY_LINK_SPEED_10H;

        err = FSP_SUCCESS;
    }
#endif
    else
    {
        /* Because reading the first time shows the previous state, the Link status bit is read twice. */
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);

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
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_AN_LINK_PARTNER, &reg);

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
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_10H))
        {
            line_speed_duplex = ETHER_PHY_LINK_SPEED_10H;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_10F == (reg & ETHER_PHY_AN_LINK_PARTNER_10F)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_10F))
        {
            line_speed_duplex = ETHER_PHY_LINK_SPEED_10F;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_100H == (reg & ETHER_PHY_AN_LINK_PARTNER_100H)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_100H))
        {
            line_speed_duplex = ETHER_PHY_LINK_SPEED_100H;
        }

        if ((ETHER_PHY_AN_LINK_PARTNER_100F == (reg & ETHER_PHY_AN_LINK_PARTNER_100F)) &&
            ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_100F))
        {
            line_speed_duplex = ETHER_PHY_LINK_SPEED_100F;
        }

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)
        if (ETHER_PHY_LSI_TYPE_88E1112 == p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type)
        {
            /* Select Copper Page */
            R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_PAGE_ADDRESS, 0U);

            /* Get the link partner response */
            R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_1000BASET_STATUS, &reg);

            if ((ETHER_PHY_AN_LINK_PARTNER_1000H == (reg & ETHER_PHY_AN_LINK_PARTNER_1000H)) &&
                ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_1000H))
            {
                line_speed_duplex = ETHER_PHY_LINK_SPEED_1000H;
            }

            if ((ETHER_PHY_AN_LINK_PARTNER_1000F == (reg & ETHER_PHY_AN_LINK_PARTNER_1000F)) &&
                ether_phy_targets_is_support_link_partner_ability(p_instance_ctrl, ETHER_PHY_LINK_SPEED_1000F))
            {
                line_speed_duplex = ETHER_PHY_LINK_SPEED_1000F;
            }
        }
#endif

        if (ETHER_PHY_LINK_SPEED_NO_LINK == line_speed_duplex)
        {
            err = FSP_ERR_ETHER_PHY_ERROR_LINK;
        }
        else
        {
            (*p_line_speed_duplex) = line_speed_duplex;
        }
    }

    return err;
}                                      /* End of function R_ETHER_PHY_LinkPartnerAbilityGet() */

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
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_LinkStatusGet (ether_phy_ctrl_t * const p_ctrl)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t  reg;
    fsp_err_t err = FSP_SUCCESS;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif

    if (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type != ETHER_PHY_LSI_TYPE_CUSTOM)
    {
        /* Because reading the first time shows the previous state, the Link status bit is read twice. */
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);
        R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_STATUS, &reg);

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
#endif                                 /* ETHER_PHY_LSI_TYPE_CT25205_RTL */
        }
    }
    else
    {
        /* Custom PHY need to be design and confirm by User */
        /* Link is up */
        err = FSP_SUCCESS;
    }

    return err;
}                                      /* End of function R_ETHER_PHY_LinkStatusGet() */

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
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_ChipInit (ether_phy_ctrl_t * const p_ctrl, ether_phy_cfg_t const * const p_cfg)
{
    fsp_err_t err = FSP_SUCCESS;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t reg   = 0;
    uint32_t count = 0;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_cfg, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
    p_instance_ctrl->interface_status = ETHER_PHY_INTERFACE_STATUS_INITIALIZED;

    if (p_instance_ctrl->p_ether_phy_cfg->phy_lsi_type == ETHER_PHY_LSI_TYPE_CUSTOM)
    {
        ether_phy_targets_initialize(p_instance_ctrl);

        p_instance_ctrl->open = ETHER_PHY_OPEN;

        err = FSP_SUCCESS;
    }

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)
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
        R_ETHER_PHY_Write(p_instance_ctrl, ETHER_PHY_REG_CONTROL, ETHER_PHY_CONTROL_RESET);

        /* Reset completion waiting */
        do
        {
            R_ETHER_PHY_Read(p_instance_ctrl, ETHER_PHY_REG_CONTROL, &reg);
            count++;
        } while ((reg & ETHER_PHY_CONTROL_RESET) && (count < p_cfg->phy_reset_wait_time));

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
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(NULL != p_data, FSP_ERR_INVALID_POINTER);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_phy_cfg->channel;
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    uint32_t trcv_idx = p_extend->p_phy_lsi_cfg_list->address;
    if (reg_addr < ETH_PHY_MAX_CLAUSE22_REGISTER)
    {
        if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSN)
        {
            r_ether_phy_tsn_hw_read_mii(p_instance_ctrl, trcv_idx, reg_addr, p_data);
        }
        else
        {
            r_ether_pir_read_mii(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr, p_data);
        }
    }
    else
    {
        if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSN)
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
 ***********************************************************************************************************************/
fsp_err_t R_ETHER_PHY_Write (ether_phy_ctrl_t * const p_ctrl, uint32_t reg_addr, uint32_t data)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

#if (ETHER_PHY_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_REGISTER_DATA_SIZE >= data, FSP_ERR_INVALID_ARGUMENT);
    ETHER_PHY_ERROR_RETURN(ETHER_PHY_INTERFACE_STATUS_INITIALIZED == p_instance_ctrl->interface_status,
                           FSP_ERR_NOT_INITIALIZED);
#endif
    uint8_t ctrl_idx = p_instance_ctrl->p_ether_phy_cfg->channel;
    ether_phy_extended_cfg_t * p_extend = ((ether_phy_extended_cfg_t *) p_instance_ctrl->p_ether_phy_cfg->p_extend);
    uint32_t trcv_idx = p_extend->p_phy_lsi_cfg_list->address;
    if (reg_addr < ETH_PHY_MAX_CLAUSE22_REGISTER)
    {
        if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSN)
        {
            r_ether_phy_tsn_hw_write_mii(p_instance_ctrl, trcv_idx, reg_addr, data);
        }
        else
        {
            r_ether_pir_write_mii(p_instance_ctrl, ctrl_idx, trcv_idx, reg_addr, data);
        }
    }
    else
    {
        if (p_extend->eth_reg == ETHER_PHY_ETH_REG_TSN)
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

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)

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
                ether_phy_extended_cfg_t const * p_callback = p_instance_ctrl->p_ether_phy_cfg->p_extend;
                if (NULL != p_callback->p_target_init)
                {
                    p_callback->p_target_init(p_instance_ctrl);
                }
            }

            break;
        }
#endif

        /* If module is configured for default LSI */
        default:
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

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)

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
                ether_phy_extended_cfg_t const * p_callback = p_instance_ctrl->p_ether_phy_cfg->p_extend;
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
}                                      /* End of function ether_phy_targets_is_support_link_partner_ability() */

/*******************************************************************************************************************//**
 * Function Name                 r_ether_phy_tsn_hw_write_mii
 * Description                   Write data to the PHY management interface
 * @param[in] p_ctrl             Pointer to the control block for the channel
 * @param[in] trcv_idx           Physical init address
 * @param[in] reg_idx            register index
 * @param[in] reg_val            register value
 ***********************************************************************************************************************/
static bool r_ether_phy_tsn_hw_write_mii (ether_phy_instance_ctrl_t * p_ctrl,
                                          uint32_t                    trcv_idx,
                                          uint32_t                    reg_idx,
                                          uint32_t                    reg_val)
{
    bool     result;
    uint32_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

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
static bool r_ether_phy_tsn_hw_read_mii (ether_phy_instance_ctrl_t * p_ctrl,
                                         uint32_t                    trcv_idx,
                                         uint32_t                    reg_idx,
                                         uint32_t * const            p_reg_val)
{
    bool     result;
    uint32_t count = 0;
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;

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
    uint32_t mmd      = ((uint16_t) (reg_addr_45 >> ETH_PHY_MMD_BIT) & ETH_PHY_MMD_MASK);
    uint32_t reg_addr = ((uint16_t) (reg_addr_45 >> ETH_PHY_REG_ADDR_BIT) & ETH_PHY_REG_ADDR_MASK);
    uint32_t reg_val;

    /* Select MMD Register Space */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, mmd);

    /* Set Clause 45 Register Address */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_addr);

    /* Switch to Data Mode */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, ETH_PHY_MMD_ACCESS_BIT | mmd);

    /* Read data from data register */
    r_ether_pir_read_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, &reg_val);

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
    uint16_t mmd      = ((uint16_t) (reg_addr_45 >> ETH_PHY_MMD_BIT) & ETH_PHY_MMD_MASK);
    uint16_t reg_addr = ((uint16_t) (reg_addr_45 >> ETH_PHY_REG_ADDR_BIT) & ETH_PHY_REG_ADDR_MASK);

    /* Select MMD Register Space */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, mmd);

    /* Set Clause 45 Register Address */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_addr);

    /* Switch to Data Mode */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_CONTROL_REG, ETH_PHY_MMD_ACCESS_BIT | mmd);

    /* Write data to data register */
    r_ether_pir_write_mii(p_ctrl, ctrl_idx, trcv_idx, ETH_PHY_MMD_ACCESS_DATA_REG, reg_val);
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
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1UL);
        }

        /* ST, OP, PHYAD, REGAD */
        header = ETH_PHY_WHEADER(trcv_idx, reg_idx);
        for (bit_count = (uint32_t) ETH_PHY_HEADER_SIZE; bit_count > 0UL; bit_count--)
        {
            /* Pickup the bit which should be output next, MSB first */
            bit = (header >> (bit_count - 1UL)) & 1UL;
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
        }

        /* TA: 10 */
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1UL);
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 0UL);

        /* DATA */
        for (bit_count = (uint32_t) ETH_PHY_DATA_SIZE; bit_count > 0UL; bit_count--)
        {
            /* Pickup the bit which should be output next, MSB first */
            bit = ((uint32_t) reg_val >> (bit_count - 1UL)) & 1UL;
            r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
        }

        r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx);
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
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, 1UL);
    }

    /* ST, OP, PHYAD, REGAD */
    header = ETH_PHY_RHEADER(trcv_idx, reg_idx);
    for (bit_count = (uint32_t) ETH_PHY_HEADER_SIZE; bit_count > 0UL; bit_count--)
    {
        /* Pickup the MSB which should be output next, MSB first */
        bit = (header >> (bit_count - 1UL)) & 1UL;
        r_ether_pir_hw_write_mii_bit(p_ctrl, (uint32_t) ctrl_idx, bit);
    }

    /* TA[0]: Turn MDIO to Z, it is same as Read cycle */
    r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx);

    /* TA[1]: Read 0 from PHY, ignore it */
    r_ether_pir_hw_read_mii_bit(p_ctrl, (uint32_t) ctrl_idx);

    /* DATA */
    reg_val = 0UL;
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
static void r_ether_pir_hw_write_mii_bit (ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx, const uint32_t bit)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
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
static uint32_t r_ether_pir_hw_read_mii_bit (ether_phy_instance_ctrl_t * p_ctrl, uint32_t ctrl_idx)
{
    ether_phy_instance_ctrl_t * p_instance_ctrl = (ether_phy_instance_ctrl_t *) p_ctrl;
    uint32_t bit;
    (void) ctrl_idx;
    bit = ETH_GET_MDI(*p_instance_ctrl->p_reg_pir);

    (*p_instance_ctrl->p_reg_pir) = ETH_PIR_MDC;

    r_ether_pir_wait_nano_second(ETH_MDIO_SETUP_TIME + ETH_WORST_SLEW_RATE);
    (*p_instance_ctrl->p_reg_pir) = 0UL;

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
    uint32_t          cycle = ETH_NS2HZ_AVB(nano_sec) + 1UL;
    volatile uint32_t count;

    for (count = 0UL; count < cycle; count++)
    {
        /* No action required */
    }
}

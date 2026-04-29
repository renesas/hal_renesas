/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"
#include "r_ether_phy.h"

#if (ETHER_PHY_CFG_TARGET_LAN8700_ENABLE)

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
 #define ETHTSN_PHY_ADDR                (0x1FU)
 #define ETH_REG0_NEAR_LOOPBACK_CFG     ETH_REG0_NEAR_LOOPBACK_OFF  /* Near loop back mode. [ON/OFF]                 */
 #define ETH_REG0_SPEED_CFG             ETH_REG0_SPEED_100MBPS      /* Ethernet network speed. [10MBPS/100MBPS]     */
 #define ETH_REG0_AUTONEG_PROC_CFG      ETH_REG0_AUTONEG_PROC_ON    /* Auto negotiation. [ON/OFF]                   */
 #define ETH_REG0_POWERDOWN_MODE_CFG    ETH_REG0_POWERDOWN_MODE_OFF /* General Power down mode. [ON/OFF]             */
 #define ETH_REG0_ELEC_ISOLATE_CFG      ETH_REG0_ELEC_ISOLATE_OFF   /* PHY Electrical Isolation. [ON/OFF]          */
 #define ETH_REG0_DUPLEX_CFG            ETH_REG0_DUPLEX_FULL        /* Duplex mode. [HALF/FULL]                     */
 #define ETH_REG0_COLTEST_CFG           ETH_REG0_COLTEST_OFF        /* Collision Test. [ON/OFF]                      */
 #define ETH_REG0_SW_RESET              (0x8000U)
 #define ETH_REG0_BASIC_CONTROL_REG     (0U)
 #define ETH_REG1_BASIC_STATUS_REG      (1U)
 #define ETHER_DELAY_TIME_100_MI_SEC    (100UL)
 #define ETH_REG0_SW_RESET              (0x8000U)
 #define ETH_REG0_NEAR_LOOPBACK_ON      (0x4000U)
 #define ETH_REG0_NEAR_LOOPBACK_OFF     (0x0000U)
 #define ETH_REG0_SPEED_100MBPS         (0x2000U)
 #define ETH_REG0_SPEED_10MBPS          (0x0000U)
 #define ETH_REG0_AUTONEG_PROC_ON       (0x1000U)
 #define ETH_REG0_AUTONEG_PROC_OFF      (0x0000U)
 #define ETH_REG0_POWERDOWN_MODE_ON     (0x0800U)
 #define ETH_REG0_POWERDOWN_MODE_OFF    (0x0000U)
 #define ETH_REG0_ELEC_ISOLATE_ON       (0x0400U)
 #define ETH_REG0_ELEC_ISOLATE_OFF      (0x0000U)
 #define ETH_REG0_RESTART_AUTONEG       (0x0200U)
 #define ETH_REG0_DUPLEX_FULL           (0x0100U)
 #define ETH_REG0_DUPLEX_HALF           (0x0000U)
 #define ETH_REG0_COLTEST_ON            (0x0008U)
 #define ETH_REG0_COLTEST_OFF           (0x0000U)

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/
void ether_phy_target_lan8700_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
bool ether_phy_target_lan8700_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                              uint32_t                    line_speed_duplex);

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name             ether_phy_target_lan8700_initialize
 * Description               Initialize physical init
 * @param[in] p_ctrl         Pointer to the control block for the channel
 ***********************************************************************************************************************/
void ether_phy_target_lan8700_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    /* Software reset. */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG0_BASIC_CONTROL_REG, ETH_REG0_SW_RESET);

    /* Waiting physical Initialize value*/
    R_BSP_SoftwareDelay(ETHER_DELAY_TIME_100_MI_SEC, BSP_DELAY_UNITS_MILLISECONDS);

    /* Setting PHY to auto-negotiate 100Mbps*/
    R_ETHER_PHY_Write(p_instance_ctrl,
                      ETH_REG0_BASIC_CONTROL_REG,
                      ETH_REG0_NEAR_LOOPBACK_CFG |
                      ETH_REG0_SPEED_CFG |
                      ETH_REG0_AUTONEG_PROC_CFG |
                      ETH_REG0_POWERDOWN_MODE_CFG |
                      ETH_REG0_ELEC_ISOLATE_CFG |
                      ETH_REG0_DUPLEX_CFG |
                      ETH_REG0_COLTEST_CFG);
}                                      /* End of function ether_phy_targets_initialize() */

/***********************************************************************************************************************
 * Function Name: ether_phy_target_lan8700_is_support_link_partner_ability
 * Description  : Check if the PHY-LSI connected Ethernet controller supports link ability
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                line_speed_duplex -
 *                    Line speed duplex of link partner PHY-LSI
 * Return Value : bool
 ***********************************************************************************************************************/
bool ether_phy_target_lan8700_is_support_link_partner_ability (ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                               uint32_t                    line_speed_duplex)
{
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    bool ret = false;

    /* This PHY-LSI only supports full Duplex mode. */
    switch (line_speed_duplex)
    {
        /* Lan8700 support Full Duplex for 10Mbps and 100Mbps */
        case ETHER_PHY_LINK_SPEED_10F:
        {
            ret = true;
            break;
        }

        case ETHER_PHY_LINK_SPEED_100F:
        {
            ret = true;
            break;
        }

        default:
        {
            break;
        }
    }

    return ret;
}                                      /* End of function ether_phy_target_lan8700_is_support_link_partner_ability() */

#endif /* ETHER_PHY_CFG_TARGET_LAN8700_ENABLE */

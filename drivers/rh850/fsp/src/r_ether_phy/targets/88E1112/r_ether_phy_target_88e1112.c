/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"
#include "r_ether_phy.h"

#if (ETHER_PHY_CFG_TARGET_88E1112_ENABLE)

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/
 #define ETH_PHY_88E1112_TIMEOUT                                                         (5000U)

 #if ((BSP_MCU_R7F702301x) || (BSP_MCU_R7F702302x))
  #define ETH_PHY_88E1112_HARD_RESET_PIN                                                 BSP_IO_PORT_AP00_PIN_8
 #else
  #define ETH_PHY_88E1112_HARD_RESET_PIN                                                 BSP_IO_PORT_P09_PIN_0
 #endif

 #define ETH_REG0_CONTROL_REGISTER                                                       (0U)
 #define ETH_REG0_CONTROL_REGISTER_AUTONEGO_EN                                           (1 << 12)
 #define ETH_REG0_CONTROL_REGISTER_RESTART_COPPER_AN_PROC                                (1 << 9)
 #define ETH_REG0_CONTROL_REGISTER_COPPER_FULL_DUPLEX_MODE                               (1 << 8)
 #define ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_MSB                             (1 << 6)
 #define ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_LSB                             (0 << 13)

 #define ETH_REG0_CONTROL_REGISTER_PHY_SOFTWARE_RESET                                    (1 << 15)
 #define ETH_REG0_CONTROL_REGISTER_SGMII_MAC_INTERFACE_EN_AN_PROCESS                     (1 << 12)
 #define ETH_REG0_CONTROL_REGISTER_DEFAULT_MAC_SGMII_SPEED_MSB                           (1 << 6)
 #define ETH_REG0_CONTROL_REGISTER_DEFAULT_MAC_SGMII_SPEED_LSB                           (0 << 13)

 #define ETH_REG1_STATUS_REGISTER                                                        (1U)
 #define ETH_REG1_STATUS_REGISTER_LINK_STATUS_MSK                                        (0x0004U)

 #define ETH_REG16_SPECIFIC_CONTROL_REGISTER_1                                           (16U)
 #define ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_PREFER_COPPER_MEDIUM                      (2 << 10)
 #define ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_AUTO_COPPER_SGMII_MEDIA_INTERRFACE        (2 << 7)
 #define ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_SGMII_MAC_INTERFACE_ALWAYS_POWER_UP       (1 << 3)
 #define ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_PASS_FLOW_CONTROL_BIT_THROUGH_SGMII_AN    (1 << 2)

 #define ETH_REG22_PAGE_ADDRESS                                                          (22U)
 #define ETH_REG22_PAGE_ADDRESS_COPPER                                                   (0U)
 #define ETH_REG22_PAGE_ADDRESS_FIBER                                                    (1U)
 #define ETH_REG22_PAGE_ADDRESS_MAC                                                      (2U)

 #define ETH_REG26_SPECIFIC_CONTROL_REGISTER_2                                           (26U)
 #define ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_AN_BYPASS             (1 << 15)
 #define ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_AN_TIMER              (1 << 13) /* 0.50 to 0.60us */
 #define ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_CLOCK_EN              (1 << 4)
 #define ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_OUTPUT_AMP            (4 << 0)  /* Amplitude peak-to-peak 0.90V */

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/
void ether_phy_target_88e1112_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);
bool ether_phy_target_88e1112_is_support_link_partner_ability(ether_phy_instance_ctrl_t * p_instance_ctrl,
                                                              uint32_t                    line_speed_duplex);

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name             ether_phy_target_88e1112_initialize
 * Description               Initialize physical init
 * @param[in] p_ctrl         Pointer to the control block for the channel
 ***********************************************************************************************************************/
void ether_phy_target_88e1112_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t phy_sts = 0;
    uint32_t count   = 0;
    uint32_t reg_val = 0;

    /*======================================================*/
    /* RESET of Ether PHY asserted and de-asserted to reset */
    /*======================================================*/

    R_BSP_PinAccessEnable();
    R_BSP_PortWriteEnable((bsp_io_port_t) ETH_PHY_88E1112_HARD_RESET_PIN);

 #if ((BSP_MCU_R7F702301x) || (BSP_MCU_R7F702302x))

    /* RESET de-asserted (Normal mode) via ETH1RESET */
    R_BSP_PinWrite(ETH_PHY_88E1112_HARD_RESET_PIN, BSP_IO_LEVEL_LOW);
    R_BSP_PinWrite(ETH_PHY_88E1112_HARD_RESET_PIN, BSP_IO_LEVEL_HIGH);
 #else

    /* RESET de-asserted (Normal mode) via ETH1RESET(P9_0=LO) */
    R_BSP_PinWrite(ETH_PHY_88E1112_HARD_RESET_PIN, BSP_IO_LEVEL_LOW);
 #endif

    /* Wait for TRESET(Minimum reset pulse width during normal operation: 10ms) */
    R_BSP_SoftwareDelay(10, BSP_DELAY_UNITS_MICROSECONDS);

    R_BSP_PortWriteDisable((bsp_io_port_t) ETH_PHY_88E1112_HARD_RESET_PIN);
    R_BSP_PinAccessDisable();

    /* Release power down mode of PHY */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG22_PAGE_ADDRESS, ETH_REG22_PAGE_ADDRESS_COPPER);
    reg_val = ETH_REG0_CONTROL_REGISTER_AUTONEGO_EN |
              ETH_REG0_CONTROL_REGISTER_COPPER_FULL_DUPLEX_MODE |
              ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_MSB |
              ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_LSB;
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG0_CONTROL_REGISTER, reg_val); /* 0x1140 */

    /* Activate Enhanced SGMII (flow control) with default 100 Mbit/s speed in PHY, enable SGMII CLK */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG22_PAGE_ADDRESS, ETH_REG22_PAGE_ADDRESS_MAC);
    reg_val = ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_PREFER_COPPER_MEDIUM |
              ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_AUTO_COPPER_SGMII_MEDIA_INTERRFACE |
              ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_SGMII_MAC_INTERFACE_ALWAYS_POWER_UP |
              ETH_REG16_SPECIFIC_CONTROL_REGISTER_1_PASS_FLOW_CONTROL_BIT_THROUGH_SGMII_AN;
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG16_SPECIFIC_CONTROL_REGISTER_1, reg_val); /* 0x090C */

    reg_val = ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_AN_TIMER |
              ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_CLOCK_EN |
              ETH_REG26_SPECIFIC_CONTROL_REGISTER_2_SGMII_MAC_INTERFACE_OUTPUT_AMP;
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG26_SPECIFIC_CONTROL_REGISTER_2, reg_val); /* 0x2014 */

    reg_val = ETH_REG0_CONTROL_REGISTER_PHY_SOFTWARE_RESET |
              ETH_REG0_CONTROL_REGISTER_SGMII_MAC_INTERFACE_EN_AN_PROCESS |
              ETH_REG0_CONTROL_REGISTER_DEFAULT_MAC_SGMII_SPEED_MSB |
              ETH_REG0_CONTROL_REGISTER_DEFAULT_MAC_SGMII_SPEED_LSB;
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG0_CONTROL_REGISTER, reg_val); /* 0x9040 */

    /* No Loopback, Soft Reset PHY to start, so that also auto negotiation starts */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG22_PAGE_ADDRESS, ETH_REG22_PAGE_ADDRESS_COPPER);
    reg_val = ETH_REG0_CONTROL_REGISTER_PHY_SOFTWARE_RESET |
              ETH_REG0_CONTROL_REGISTER_AUTONEGO_EN |
              ETH_REG0_CONTROL_REGISTER_RESTART_COPPER_AN_PROC |
              ETH_REG0_CONTROL_REGISTER_COPPER_FULL_DUPLEX_MODE |
              ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_MSB |
              ETH_REG0_CONTROL_REGISTER_SPEED_SELECTION_SGMII_LSB;
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_REG0_CONTROL_REGISTER, reg_val); /* 0x9340 */

    /* wait until link is up */
    do
    {
        R_ETHER_PHY_Read(p_instance_ctrl, ETH_REG1_STATUS_REGISTER, &phy_sts);
        count++;
    } while (((phy_sts & ETH_REG1_STATUS_REGISTER_LINK_STATUS_MSK) != ETH_REG1_STATUS_REGISTER_LINK_STATUS_MSK) &&
             (count <= ETH_PHY_88E1112_TIMEOUT)); /* Wait until link is up */
}                                                 /* End of function ether_phy_target_88e1112_initialize() */

/***********************************************************************************************************************
 * Function Name: ether_phy_target_88e1112_is_support_link_partner_ability
 * Description  : Check if the PHY-LSI connected Ethernet controller supports link ability
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                line_speed_duplex -
 *                    Line speed duplex of link partner PHY-LSI
 * Return Value : bool
 ***********************************************************************************************************************/
bool ether_phy_target_88e1112_is_support_link_partner_ability (ether_phy_instance_ctrl_t * p_instance_ctrl,
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

        case ETHER_PHY_LINK_SPEED_1000F:
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
}                                      /* End of function ether_phy_target_88e1112_is_support_link_partner_ability() */

#endif /* ETHER_PHY_CFG_TARGET_88E1112_ENABLE */

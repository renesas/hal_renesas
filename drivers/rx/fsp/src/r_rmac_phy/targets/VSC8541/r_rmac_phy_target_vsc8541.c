/*
 * Copyright (c) 2020 - 2025 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"
#include "r_rmac_phy.h"

#if (ETHER_PHY_CFG_TARGET_VSC8541_ENABLE)

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/* Vendor Specific PHY Registers */
 #define RMAC_PHY_REG_LED_MODE_SELECT                             (0x1D)
 #define RMAC_PHY_REG_LED_BEHAVIOR                                (0x1E)
 #define RMAC_PHY_REG_INTERRUPT_MASK                              (0x19)
 #define RMAC_PHY_REG_EXT_PHY_CONTROL1                            (0x17)
 #define RMAC_PHY_REG_MODE_CONTROL                                (0x00)
 #define RMAC_PHY_REG_1000BASET_CONTROL                           (0x09)
 #define RMAC_PHY_REG_RGMII_CONTROL                               (0x14)
 #define RMAC_PHY_REG_PAGE_REG_ACCESS                             (0x1F)

 #define RMAC_PHY_REG_LEDMODE_ANYSPEED_LINK_ACTIVITY              (0x0)
 #define RMAC_PHY_REG_LED0_MODE_SELECT_OFFSET                     (0x0)
 #define RMAC_PHY_REG_LED1_MODE_SELECT_OFFSET                     (0x4)
 #define RMAC_PHY_REG_LED0_FEATURE_DISABLE_OFFSET                 (0x0)
 #define RMAC_PHY_REG_LED1_FEATURE_DISABLE_OFFSET                 (0x1)
 #define RMAC_PHY_REG_INTERRUPT_MASK_MDINT_OFFSET                 (0xF)

 #define RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_OFFSET        (0xB)
 #define RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_VALUE_MASK    (0x3)
 #define RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_MII_GMII      (0x0)
 #define RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_RMII          (0x1)
 #define RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_RGMII         (0x2)

 #define RMAC_PHY_REG_MODE_CONTROL_SOFTWARE_RESET_OFFSET          (0xF)

 #define RMAC_PHY_REG_1000BASET_CONTROL_FDX_CAPABLITY_OFFSET      (0x09)
 #define RMAC_PHY_REG_1000BASET_CONTROL_HDX_CAPABLITY_OFFSET      (0x08)

 #define RMAC_PHY_REG_RGMII_CONTROL_RX_CLK_DELAY_OFFSET           (0x4)
 #define RMAC_PHY_REG_RGMII_CONTROL_TX_CLK_DELAY_OFFSET           (0x0)
 #define RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_MASK       (0x07)
 #define RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_2NS        (0x04)

 #define RMAC_PHY_REG_PAGE_REG_ACCESS_MAIN_REG                    (0x00)
 #define RMAC_PHY_REG_PAGE_REG_ACCESS_PAGE2                       (0x02)

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global funtion
 ***********************************************************************************************************************/
void rmac_phy_target_vsc8541_initialize(rmac_phy_instance_ctrl_t * p_instance_ctrl);
bool rmac_phy_target_vsc8541_is_support_link_partner_ability(rmac_phy_instance_ctrl_t * p_instance_ctrl,
                                                             uint32_t                   line_speed_duplex);

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: rmac_phy_targets_initialize
 * Description  : PHY-LSI specific initialization processing
 * Arguments    : p_api_ctrl -
 *                    Ethernet channel number
 * Return Value : none
 ***********************************************************************************************************************/
void rmac_phy_target_vsc8541_initialize (rmac_phy_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t reg;
    uint32_t count = 0;
    uint32_t phy_reg_value;
    uint32_t expect_reg_value;

    /* When VSC8541 of the Microsemi Corporation. is used.
     * Enable access to main register.
     */
    do
    {
        R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, RMAC_PHY_REG_PAGE_REG_ACCESS_MAIN_REG);
        R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, &reg);
        count++;
    } while ((reg != RMAC_PHY_REG_PAGE_REG_ACCESS_MAIN_REG) &&
             (count < p_instance_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    /* Check MAC interface mode is set as expected. If not, set register */
    R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_EXT_PHY_CONTROL1, &reg);
    phy_reg_value =
        (reg >>
         RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_OFFSET) & RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_VALUE_MASK;

    switch (p_instance_ctrl->p_ether_phy_cfg->mii_type)
    {
        case ETHER_PHY_MII_TYPE_MII:
        case ETHER_PHY_MII_TYPE_GMII:
        {
            expect_reg_value = RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_MII_GMII;
            break;
        }

        case ETHER_PHY_MII_TYPE_RMII:
        {
            expect_reg_value = RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_RMII;
            break;
        }

        case ETHER_PHY_MII_TYPE_RGMII:
        {
            expect_reg_value = RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_RGMII;
            break;
        }

        default:
        {
            expect_reg_value = RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_MII_GMII;
            break;
        }
    }

    if (phy_reg_value != expect_reg_value)
    {
        reg =
            (reg &
             ~(uint32_t) (RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_VALUE_MASK <<
                          RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_OFFSET)) |
            (expect_reg_value << RMAC_PHY_REG_EXT_PHY_CONTROL_MAC_INTERFACE_OFFSET);

        R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_EXT_PHY_CONTROL1, reg);

        /* Software reset after MAC interface mode is changed
         * b15=1:Software reset asserted
         * */
        do
        {
            R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_MODE_CONTROL,
                             (1 << RMAC_PHY_REG_MODE_CONTROL_SOFTWARE_RESET_OFFSET));
            R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_MODE_CONTROL, &reg);
            count++;
        } while ((reg & (1 << RMAC_PHY_REG_MODE_CONTROL_SOFTWARE_RESET_OFFSET)) &&
                 (count < p_instance_ctrl->p_ether_phy_cfg->phy_reset_wait_time));
    }

    /* Disable 1000BASE-T capability in MII or RMII mode
     * b9=0:1000BASE-T FDX capability disabled
     * b8=0:1000BASE-T HDX capability disabled
     * */
    if ((p_instance_ctrl->p_ether_phy_cfg->mii_type == ETHER_PHY_MII_TYPE_MII) ||
        (p_instance_ctrl->p_ether_phy_cfg->mii_type == ETHER_PHY_MII_TYPE_RMII))
    {
        R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_1000BASET_CONTROL, &reg);

        reg = reg &
              ~(uint32_t) ((0x1 << RMAC_PHY_REG_1000BASET_CONTROL_FDX_CAPABLITY_OFFSET) |
                           (0x1 << RMAC_PHY_REG_1000BASET_CONTROL_HDX_CAPABLITY_OFFSET));

        R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_1000BASET_CONTROL, reg);
    }

    /* Enable Extended register Page 2 access
     * 0x0002: Registers 16–30 access extended register space 2
     */
    count = 0;                         /* Reset count for new register setting */
    do
    {
        R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, RMAC_PHY_REG_PAGE_REG_ACCESS_PAGE2);
        R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, &reg);
        count++;
    } while ((reg != RMAC_PHY_REG_PAGE_REG_ACCESS_PAGE2) &&
             (count < p_instance_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    /* Set RX_CLK and TX_CLK to 2.0 ns
     * b6:4=0x4:RX_CLK 2.0 ns delay
     * b2:0=0x4:TX_CLK 2.0 ns delay
     */
    R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_RGMII_CONTROL, &reg);
    reg =
        (reg &
         ~(uint32_t) ((RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_MASK <<
                       RMAC_PHY_REG_RGMII_CONTROL_RX_CLK_DELAY_OFFSET) +
                      (RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_MASK <<
                       RMAC_PHY_REG_RGMII_CONTROL_TX_CLK_DELAY_OFFSET))) |
        ((RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_2NS <<
            RMAC_PHY_REG_RGMII_CONTROL_RX_CLK_DELAY_OFFSET) |
         (RMAC_PHY_REG_RGMII_CONTROL_XX_CLK_DELAY_VALUE_2NS << RMAC_PHY_REG_RGMII_CONTROL_RX_CLK_DELAY_OFFSET));
    R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_RGMII_CONTROL, reg);

    /* Enable main register access
     * 0x0000: Register 16–30 accesses main register space
     */
    count = 0;                         /* Reset count for new register setting */
    do
    {
        R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, RMAC_PHY_REG_PAGE_REG_ACCESS_MAIN_REG);
        R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_PAGE_REG_ACCESS, &reg);
        count++;
    } while ((reg != RMAC_PHY_REG_PAGE_REG_ACCESS_MAIN_REG) &&
             (count < p_instance_ctrl->p_ether_phy_cfg->phy_reset_wait_time));

    /*
     * When VSC8541-05 of the Microsemi Corporation is used,
     * the pin that outputs the state of LINK is used combinedly with LINK/ACTIVITY in default.
     * The setting of the pin is changed so that only the state of LINK is output.
     * LED Mode Select (0x1D)
     * b7:4 = 0000b : LED1 mode any speed LINK/ACTIVITY
     * b3:0 = 0000b : LED0 mode any speed LINK/ACTIVITY
     * LED Behavior (0x1E)
     * b1 = 1 : LED1 combination feature enable  (link/activity)
     * b0 = 0 : LED0 combination feature disable (link only, duplex only)
     *//* LED Mode Select */
    R_RMAC_PHY_Write(p_instance_ctrl,
                     RMAC_PHY_REG_LED_MODE_SELECT,
                     (RMAC_PHY_REG_LEDMODE_ANYSPEED_LINK_ACTIVITY << RMAC_PHY_REG_LED1_MODE_SELECT_OFFSET) |
                     (RMAC_PHY_REG_LEDMODE_ANYSPEED_LINK_ACTIVITY << RMAC_PHY_REG_LED0_MODE_SELECT_OFFSET));

    /* LED Behavior */
    R_RMAC_PHY_Read(p_instance_ctrl, RMAC_PHY_REG_LED_BEHAVIOR, &reg);
    reg &= ~(1U << RMAC_PHY_REG_LED1_FEATURE_DISABLE_OFFSET);
    reg |= 1U << RMAC_PHY_REG_LED0_FEATURE_DISABLE_OFFSET;
    R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_LED_BEHAVIOR, reg);

    /* Enable interrupt of MDINT
     * b15=1:MDINT interrupt status enable */
    R_RMAC_PHY_Write(p_instance_ctrl, RMAC_PHY_REG_INTERRUPT_MASK, 0x1 << RMAC_PHY_REG_INTERRUPT_MASK_MDINT_OFFSET);
}                                      /* End of function rmac_phy_targets_initialize() */

/***********************************************************************************************************************
 * Function Name: rmac_phy_targets_is_support_link_partner_ability
 * Description  : Check if the PHY-LSI connected Ethernet controller supports link ability
 * Arguments    : p_instance_ctrl -
 *                    Ethernet control block
 *                line_speed_duplex -
 *                    Line speed duplex of link partner PHY-LSI
 * Return Value : bool
 ***********************************************************************************************************************/
bool rmac_phy_target_vsc8541_is_support_link_partner_ability (rmac_phy_instance_ctrl_t * p_instance_ctrl,
                                                              uint32_t                   line_speed_duplex)
{
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    FSP_PARAMETER_NOT_USED(line_speed_duplex);

    /* This PHY-LSI supports half and full duplex mode. */
    return true;
}                                      /* End of function rmac_phy_targets_is_support_link_partner_ability() */

#endif /* ETHER_PHY_CFG_TARGET_VSC8541_ENABLE */

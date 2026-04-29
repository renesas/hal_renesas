/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 ***********************************************************************************************************************/

/* Access to peripherals and board defines. */
#include "bsp_api.h"
#include "r_ether_phy.h"

#if (ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE)

/***********************************************************************************************************************
 * Macro definitions
 ***********************************************************************************************************************/

/* PHY REGISTER NAME */
 #define ETH_PHY_LINK_UP                      (0x2CU)
 #define ETH_PHY_CT25205_REG_CONTROL          (0x00U)
 #define ETH_PHY_CT25205_REG_STATUS           (0x01U)
 #define ETH_PHY_CT25205_REG_PHYID0           (0x02U)
 #define ETH_PHY_CT25205_REG_PHYID1           (0x03U)
 #define ETH_PHY_CT25205_REG_T1STWEAKS        (0x3F8001)
 #define ETH_PHY_CT25205_REG_PLCACTRL1        (0x3FCA02)
 #define ETH_PHY_CT25205_REG_PLCAST           (0x3FCA03)
 #define ETH_PHY_CT25205_REG_CTIPVER          (0x3F8000)
 #define ETH_PHY_CT25205_REG_T1STXCST         (0x3F8006)
 #define ETH_PHY_CT25205_REG_T1STXCCTL        (0x3F8005)
 #define ETH_PHY_CT25205_REG_PLCACTRL0        (0x3FCA01)

/* PHY REGISTER VALUE */
 #define ETH_PHY_CT25205_T1STWEAKS_PKTLOOP    (0x0003)
 #define ETH_PHY_CT25205_CONTROL_LCTL         (0x1000)
 #define ETH_PHY_CT25205_CONTROL_RESET        (0x8000)
 #define ETH_PHY_CT25205_PLCACTRL1_CONFIG     (0x0800)
 #define ETH_PHY_CT25205_T1STXCCTL_CONFIG     (0x0002)
 #define ETH_PHY_CT25205_T1STXCCTL_RESET      (0x0000)
 #define ETH_PHY_CT25205_T1STXCST_FAIL        (0x07)
 #define ETH_PHY_CT25205_PLCACTRL0_PLCA       (0x8000)
 #define ETH_PHY_CT25205_PLCACTRL0_EN         (0x8000)
 #define ETH_PHY_CT25205_TIMEOUT              (5000U)

/***********************************************************************************************************************
 * Typedef definitions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global function
 ***********************************************************************************************************************/
void ether_phy_target_ct25205_initialize(ether_phy_instance_ctrl_t * p_instance_ctrl);

/***********************************************************************************************************************
 * Private global variables and functions
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name             ether_phy_target_ct25205_initialize
 * Description               Initialize physical init
 * @param[in] p_ctrl         Pointer to the control block for the channel
 ***********************************************************************************************************************/
void ether_phy_target_ct25205_initialize (ether_phy_instance_ctrl_t * p_instance_ctrl)
{
    uint32_t reg_val;
    uint32_t count = 0;

    /* Software reset */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_CONTROL, ETH_PHY_CT25205_CONTROL_RESET);

    /* Configure PLCA for CT25205 */
    /* Check content T1S PHY register */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_CONTROL, &reg_val);                             /* CONTROL     -> 0x0000 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_STATUS, &reg_val);                              /* STATUS      -> 0x0809 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_PHYID0, &reg_val);                              /* PHYID0      -> 0xB824 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_PHYID1, &reg_val);                              /* PHYID1      -> 0x2B01 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_CTIPVER, &reg_val);                             /* CTIPVER     -> 0x20C2 / 0x21c8 */

    /* Setup T1S-PHY CT25205 */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STWEAKS, ETH_PHY_CT25205_T1STWEAKS_PKTLOOP); /* T1STWEAKS, set PKTLOOP */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STWEAKS, &reg_val);                           /* T1STWEAKS, -> 0x8003/0x0003 */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_CONTROL, ETH_PHY_CT25205_CONTROL_LCTL);        /* CONTROL  , set LCTL */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_CONTROL, &reg_val);                             /* CONTROL  , -> 0x1000 */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_PLCACTRL1, ETH_PHY_CT25205_PLCACTRL1_CONFIG);  /* PLCACTRL1, set NCNT=8, ID=0 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_PLCACTRL1, &reg_val);                           /* PLCACTRL1, -> 0x0800 */
    R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_PLCAST, &reg_val);                              /* PLCAST, check PST -> 0x0000 */

    /* Setup CT25205 */
    do
    {
        R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STXCST, &reg_val);                       /* T1STXCST, check TXCST status NORMAL */
        count++;
    } while (((reg_val & ETH_PHY_CT25205_T1STXCST_FAIL) != 0x00) && (count <= ETH_PHY_CT25205_TIMEOUT));

    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STXCCTL, ETH_PHY_CT25205_T1STXCCTL_CONFIG); /* T1STXCCTL: PHY CONFIG Request */

    /* Set Normal Mode */
    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STXCCTL, ETH_PHY_CT25205_T1STXCCTL_RESET);  /* T1STXCCTL: PHY RESET/NORMAL Request */
    do
    {
        R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_T1STXCST, &reg_val);                       /* T1STXCST, check TXCST status NORMAL */
        count++;
    } while (((reg_val & ETH_PHY_CT25205_T1STXCST_FAIL) != 0x00) && (count <= ETH_PHY_CT25205_TIMEOUT));

    R_ETHER_PHY_Write(p_instance_ctrl, ETH_PHY_CT25205_REG_PLCACTRL0, ETH_PHY_CT25205_PLCACTRL0_PLCA);   /* PLCACTRL0, set PLCA EN */
    do
    {
        R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_PLCACTRL0, &reg_val);                      /* PLCACTRL0, -> 0x8000 */
        count++;
    } while (((reg_val & ETH_PHY_CT25205_PLCACTRL0_EN) != ETH_PHY_CT25205_PLCACTRL0_EN) &&
             (count <= ETH_PHY_CT25205_TIMEOUT));

    do
    {
        /* Read the state register of the PHY(CT25205). */
        R_ETHER_PHY_Read(p_instance_ctrl, ETH_PHY_CT25205_REG_STATUS, &reg_val);
        count++;
    } while (((reg_val & ETH_PHY_LINK_UP) != ETH_PHY_LINK_UP) && (count <= ETH_PHY_CT25205_TIMEOUT));
}                                      /* End of function ether_phy_target_ct25205_initialize() */

#endif /* ETHER_PHY_CFG_TARGET_CT25205_RTL_ENABLE */

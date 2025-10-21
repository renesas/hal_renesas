/***********************************************************************************************************************
* Copyright (c) 2016 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_lpc_rx.c
 * Description  : The LPC module configures the MCU for the different operating and low power modes.
 *                This file is the LPC module's interface header file and should be included by the application that
 *                intends to use the API.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version Description
 *         : 01.10.2016 1.00    First Release
 *         : 04.22.2019 1.41    Delete inline definition of GetVersion function
 *         : 14.11.2019 2.00    Added support for GNUC and ICCRX.
 *         : 10.06.2020 2.01    Modified comment of API function to Doxygen style.
 *         : 05.03.2021 2.02    Added RX671 to Doxygen comment.
 *         : 31.07.2021 2.03    Added support for snooze mode in Low Power Consumption.
 *         : 31.12.2021 2.04    Removes the lower operating power consumption related functions of RX660.
 *         : 15.08.2022 2.20    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *         : 29.05.2023 2.30    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *         : 28.06.2024 2.40    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
 *                              Modified comment of API R_LPC_OperatingModeSet (), R_LPC_ReturnClockSwitch (),
 *                              and R_LPC_LowPowerModeActivate () functions.
 *         : 15.03.2025 2.51    Updated disclaimer.
 ***********************************************************************************************************************/
/***********************************************************************************************************************
  Includes <System Includes> , "Project Includes"
************************************************************************************************************************/
/* Defines for LPC support */
#include "r_lpc_rx_if.h"

/* Include specifics for chosen MCU. */
#include "r_lpc_rx_private.h"

#ifndef LPC_INVALID_OPERATING_MODE
/***********************************************************************************************************************
 * Function Name: R_LPC_OperatingModeSet
 ******************************************************************************************************************/ /**
 * @brief This function configures the MCU for the supported Operating Power Control modes(See Table "1.1 Operating 
 *        Power Control modes" in the application note).
 * @param[in] e_mode The modes for all supported MCUs are specified in enum \e lpc_operating_mode_t in section 2.9.1 of 
 *                   the application note.
 * @retval    LPC_SUCCESS 
 * @retval    LPC_ERR_CLOCK_EXCEEDED: Clock exceeds the limit of the operating power control mode.
 * @retval    LPC_ERR_P_E_MODE: The operating power control mode cannot be switched while the flash memory is being 
 *                              programmed or erased (P/E).
 * @details   Depending upon the mode chosen and the MCU, the maximum speed of the internal clocks ICLK, PCLKB, PCLKD 
 *            and FCLK is limited. For example, in Low-Speed operating mode on the RX110, RX111, RX113, RX130, RX140,
 *            RX230, RX231, RX23E-B, RX23W, RX260, RX261, only the sub-clock can be used as the system clock. See 
 *            Table 11.3 (RX64M, RX65N, RX66N, RX671, RX71M, RX72M, RX72N) and Table 11.4 (RX110, RX111, RX113, RX130,
 *            RX140, RX230, RX231, RX23E-B, RX23W, RX260, RX261) in the Hardware manual for clock limitations. If the 
 *            argument to this function cannot support the current internal clock frequencies, then an error is 
 *            returned. When switching the clock source from a lower frequency to a higher frequency, make certain that
 *            the operating power control mode is configured to support the range. Failure to do so will result in 
 *            improper CPU operation.
 * @note      When switching operating power control modes and internal clock frequencies, it is important to first make
 *            sure that the frequencies of internal clocks are set within the range supported by the operating power 
 *            control mode. When moving between operating power control modes/frequencies, use the following sequence: 
 *            -# Moving from low power and low internal frequencies to higher power and higher clock frequencies:
 *            \li Use R_LPC_OperatingModeSet() to move to higher power operating mode.
 *            \li Increase internal clock frequencies
 *
 *            -# Moving from high power and high internal clock frequencies to low power and low internal frequencies:
 *            \li Decrease internal clock frequencies
 *            \li Use R_LPC_OperatingModeSet() to move to lower power operating mode.
 */
lpc_err_t R_LPC_OperatingModeSet(lpc_operating_mode_t e_mode)
{
    return (lpc_operating_mode_set(e_mode));

}

/***********************************************************************************************************************
 End of function R_LPC_OperatingModeSet
 ***********************************************************************************************************************/
#endif /* LPC_INVALID_OPERATING_MODE */

/***********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeConfigure
 ******************************************************************************************************************/ /**
 * @brief This function configures the low power consumption modes(see Table "1.2 Low Power Consumption modes" in 
 *        the application note) when the WAIT instruction is executed.
 * @param[in] e_mode The modes for all supported MCUs are specified in enum \e lpc_low_power_mode_t.
 * @retval    LPC_SUCCESS
 * @details   This function configures the MCU for the different Low Power Consumption modes shown in Table "1.2 Low
 *            Power Consumption modes" in the application note. Note that this function does not activate the low power 
 *            mode, but configures the registers for the specified mode. To activate the low power mode, use the
 *            R_LPC_LowPowerModeActivate() function.\n
 *            The CPU will be stopped once any of these modes are activated; however a few of the peripherals and clocks
 *            can operate in these modes. For more details refer to the User's Manual: Hardware.
 * @note      None 
 */
lpc_err_t R_LPC_LowPowerModeConfigure(lpc_low_power_mode_t e_mode)
{
    return (lpc_low_power_mode_configure(e_mode));
}
/***********************************************************************************************************************
 End of function R_LPC_LowPowerModeConfigure
 ***********************************************************************************************************************/

#ifdef LPC_VALID_SNOOZE_MODE
/***********************************************************************************************************************
 * Function Name: R_LPC_SnoozeModeConfigure
 ******************************************************************************************************************/ /**
 * @brief This function configures the conditions for entering the snooze mode, returning to the software standby mode
 *        or releasing from the snooze mode.
 *        (See section "3.4" in the application note)
 * @param[in] snooze_mode  The snooze_mode  for all supported MCUs are specified in enum \e lpc_snooze_mode_t.
 * @retval    LPC_SUCCESS
 * @retval    LPC_ERR_ILLEGAL
 * @details   For the peripheral functions that can be operated in snooze mode, refer to the user's manual hardware.
 * @note      In order for each peripheral function to operate in snooze mode, it is necessary to meet the conditions for
 *            each peripheral function to be used before switching to software standby.
 */
lpc_err_t R_LPC_SnoozeModeConfigure(lpc_snooze_mode_t * snooze_mode)
{
    return (lpc_snooze_mode_configure(snooze_mode));
}
/***********************************************************************************************************************
 End of function R_LPC_SnoozeModeConfigure
 ***********************************************************************************************************************/
#endif


/***********************************************************************************************************************
 * Function Name: R_LPC_LowPowerModeActivate
 ******************************************************************************************************************/ /**
 * @brief This function activates the Low Power Consumption mode configured in R_LPC_LowPowerModeConfigure().
 * @param[in] pcallback
 *            Function to be called before activating low power mode.
 * @retval    LPC_SUCCESS
 * @retval    LPC_ERR_OSC_STOP_ENABLED: Cannot enter software standby if oscillation stop detection is enabled.
 * @retval    LPC_ERR_CLOCK_EXCEEDED: Clock exceeds the limit of the operating power control mode.
 * @retval    LPC_ERR_ILL_CLOCK_SOURCE: Illegal clock when sleep mode return clock switching is enabled
 * @retval    LPC_ERR_ILL_MAIN_CLK_FREQ: Clock freq. exceeds the limit of the sleep return clock.
 * @retval    LPC_ERR_DEEP_SLEEP_STATUS: The condition error for a deep sleep mode
 * @retval    LPC_ERR_ILLEGAL: Illegal operation other than above
 * @details   This function activates the low power mode by calling the wait() function. The hardware manual specifies 
 *            the sequence for entering low power mode as follows:\n
 *            \- Disable interrupts.\n
 *            \- Configure the interrupt source to wake the MCU up from the low power mode.\n
 *            \- Ensure that the last IO register write is successful.\n
 *            \- Execute the wait instruction to enter the low power mode. The wait instruction will internally enable
 *               interrupts.\n\n
 *            This function implements the sequence as follows:\n
 *            \- Disable interrupts.\n
 *            \- Call the callback function specified by the argument. The callback function should configure the
 *               wake-up interrupt source and make sure that the last IO register write is complete before returning.
 *               The user can pass a FIT_NO_FUNC pointer if the interrupt has already been configured.\n
 *            \- Execute the wait instruction.\n\n
 *            When this function is executed, an error is returned should be refereed from
 *            "R_LPC_LowPowerModeActivate()" in the application note.
 * @note      When parameter checking is enabled for the module, this function checks for a variety of conditions that
 *            prevent the MCU from entering a low power mode. While it is important to have this feature enabled in the
 *            development phase, it can be disabled during release to allow for a more rapid entry into low power 
 *            modes.\n
 *            Before entering Deep Sleep or Software Standby mode, ensure that DTC transactions are not pending and
 *            the DTC module is stopped.
 */
lpc_err_t R_LPC_LowPowerModeActivate(lpc_callback_set_t pcallback)
{
    return (lpc_lowpower_activate(pcallback));

}
/***********************************************************************************************************************
 End of function R_LPC_LowPowerModeActivate
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LPC_ReturnClockSwitch
 ******************************************************************************************************************/ /**
 * @brief This function configures the MCU to switch clock sources on waking up from Sleep mode.
 * @param[in] e_clock_source
 *            This parameter selects the clock source to be used at the time of release from sleep mode. The supported
 *            clock sources are specified in the enum \e lpc_clock_switch_t in section 2.9.3 in the application note.
 * @param[in] enable
 *            Enables or disables clock source switching at the time of release from sleep mode. The clock source
 *            selected by e_clock_source is enabled only when enable = 1.
 * @retval    LPC_SUCCESS
 * @details   This function will configure the return clock switching parameter that allows the clock source to be 
 *            switched on returning from Sleep Mode to the HOCO, LOCO or Main Clock. The following items have to be 
 *            followed to allow for Return Clock Switching:
 *            -# RX110, RX111, RX113 MCUs :\n
 *            \- When entering Sleep Mode, the system clock should be the Sub-Clock oscillator. On exiting sleep, the 
 *               operating mode will return to whatever the operating power control mode was before entering sleep.\n
 *            \- If the Main OSC is chosen as the Sleep Return clock source, middle speed mode is the return mode. Make 
 *               sure the internal clock after returning from sleep mode does not exceed the limits 
 *               of middle speed mode.
 *
 *            -# RX130, RX140, RX230, RX231, RX23E-B, RX23W, RX260, RX261 MCUs :\n
 *            \- When entering Sleep Mode, the system clock should be the Sub-Clock oscillator. On exiting sleep, the 
 *               operating mode will return to whatever the operating power control mode was before entering sleep.\n
 *            \- If Middle Speed mode is the return mode and the Main OSC is chosen as the Sleep Return clock source, 
 *               make sure the internal clock after returning from sleep mode does not exceed the limits of middle speed
 *               mode.
 *
 *            -# RX26T, RX64M, RX65N, RX66N, RX671, RX71M, RX72M, RX72N MCUs:\n
 *            \- When entering sleep mode, select LOCO or the sub-clock as the clock source.
 *
 * @note      None 
 */
lpc_err_t R_LPC_ReturnClockSwitch(lpc_clock_switch_t e_clock_source, bool enable)
{
    return (lpc_return_clock_switch(e_clock_source, enable));

}
/***********************************************************************************************************************
 End of function R_LPC_ReturnClockSwitch
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * Function Name: R_LPC_GetVersion
 ******************************************************************************************************************/ /**
 * @brief This function returns the driver version number at runtime.
 * @returns   Version number.
 * @details   Returns the version of this module. The version number is encoded such that the top 2 bytes are the major
 *            version number and the bottom 2 bytes are the minor version number.
 * @note      None 
 */
uint32_t R_LPC_GetVersion(void)
{
    uint32_t version;

    version = (LPC_RX_VERSION_MAJOR << 16) | LPC_RX_VERSION_MINOR;

    return version;
}
/***********************************************************************************************************************
 End of function R_LPC_GetVersion
 ***********************************************************************************************************************/

/***********************************************************************************************************************
 * End of File
 ***********************************************************************************************************************/

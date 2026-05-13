/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_opbt.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_OPBT_VALID_SIDE_OPBTn(n)      *((volatile uint32_t *) (BSP_OPBT_CSAF_BASE + BSP_OPBT_OPTION_BYTE_OFFSET + \
                                                                   (4U * n)))                                     ///< OPBT value on the valid side
#define BSP_OPBT_INVALID_SIDE_OPBTn(n)    *((volatile uint32_t *) (BSP_OPBT_CSAB_BASE + BSP_OPBT_OPTION_BYTE_OFFSET + \
                                                                   (4U * n)))                                     ///< OPBT value on the invalid side

#define BSP_OPBT_FACI_COMMAND_FORCE_STOP                         (0xB3U)                                          ///< FACI Command Force Stop
#define BSP_OPBT_FACI_COMMAND_SWITCH_ERASURE_1ST_ACCESS          (0x87U)                                          ///< FACI Command Switch Erasure for 1st access
#define BSP_OPBT_FACI_COMMAND_SWITCH_ERASURE_2ND_ACCESS          (0xD0U)                                          ///< FACI Command Switch Erasure for 2nd access
#define BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_1ST_ACCESS      (0x85U)                                          ///< FACI Command Switch Programming for 1st access
#define BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_2ND_ACCESS      (0x08U)                                          ///< FACI Command Switch Programming for 2nd access
#define BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_3RD_ACCESS      (0xD0U)                                          ///< FACI Command Switch Programming for 3rd access
#define BSP_OPBT_FACI_COMMAND_TAG_ERASURE_1ST_ACCESS             (0x89U)                                          ///< FACI Command TAG Erasure for 1st access
#define BSP_OPBT_FACI_COMMAND_TAG_ERASURE_2ND_ACCESS             (0xD0U)                                          ///< FACI Command TAG Erasure for 2nd access
#define BSP_OPBT_FACI_COMMAND_TAG_UPDATE_1ST_ACCESS              (0x83U)                                          ///< FACI Command TAG Update for 1st access
#define BSP_OPBT_FACI_COMMAND_TAG_UPDATE_2ND_ACCESS              (0xD0U)                                          ///< FACI Command TAG Update for 2nd access
#define BSP_OPBT_FACI_COMMAND_SHIFT_TO_READ_MODE                 (0xAA00U)                                        ///< FACI Command shift to Read mode
#define BSP_OPBT_FACI_COMMAND_SHIFT_TO_PROGRAMMING_MODE          (0xAA80U)                                        ///< FACI Command shift to Programming mode
#define BSP_OPBT_FACI_COMMAND_PROPERTY_ERASURE_1ST_ACCESS        (0x47U)                                          ///< FACI Command Property Erasure for 1st access
#define BSP_OPBT_FACI_COMMAND_PROPERTY_ERASURE_2ND_ACCESS        (0xD0U)                                          ///< FACI Command Property Erasure for 2nd access
#define BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_1ST_ACCESS    (0x45U)                                          ///< FACI Command Property Programming for 1st access
#define BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_2ND_ACCESS    (0x08U)                                          ///< FACI Command Property Programming for 2nd access
#define BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_3RD_ACCESS    (0xD0U)                                          ///< FACI Command Property Programming for 3rd access

#define BSP_OPBT_VALUE_RETURN_FROM_COMMAND_LOCK_TIMEOUT          (374U)                                           ///< Return from command lock timeout value
#define BSP_OPBT_VALUE_PROPERTY_PROGRAMMING_TIMEOUT              (1U)                                             ///< Property programming command timeout value
#define BSP_OPBT_VALUE_PROPERTY_ERASURE_TIMEOUT                  (105U)                                           ///< Property erasure command timeout value
#define BSP_OPBT_VALUE_SHIFT_TO_PROGRAM_TIMEOUT                  (105U)                                           ///< Shift to Program Mode command timeout value
#define BSP_OPBT_VALUE_SHIFT_TO_READ_TIMEOUT                     (374U)                                           ///< Shift to Read Mode timeout value
#define BSP_OPBT_VALUE_FORCE_STOP_TIMEOUT                        (28U)                                            ///< Force stop command timeout value
#define BSP_OPBT_VALUE_TAG_ERASE_TIMEOUT                         (107U)                                           ///< TAG erasure command timeout value
#define BSP_OPBT_VALUE_TAG_UPDATE_TIMEOUT                        (2U)                                             ///< TAG update command timeout value
#define BSP_OPBT_VALUE_SWITCH_PROGRAMMING_TIMEOUT                (2U)                                             ///< Switch Area programming command timeout value
#define BSP_OPBT_VALUE_SWITCH_ERASURE_TIMEOUT                    (107U)                                           ///< Switch Area erasure command timeout value

#define BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T                   *((volatile uint8_t *) ( \
                                                                       BSP_OPBT_FACI0_COMMAND_ISSUING_AREA_BASE)) ///< Macro helper performs a 1-byte access to FACI0_COMMAND_ISSUING_AREA address
#define BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T                  *((volatile uint32_t *) ( \
                                                                       BSP_OPBT_FACI0_COMMAND_ISSUING_AREA_BASE)) ///< Macro helper performs a 4-bytes access to FACI0_COMMAND_ISSUING_AREA address

/** Macro helper for checking status */
#define BSP_OPBT_BREAK_IF_ERROR(_status)    if (BSP_OPBT_STATUS_SUCCESS != (_status)) {break;}

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
static void              r_bsp_issue_force_stop_command(void);
static bsp_opbt_status_t r_bsp_issue_switch_erasure_command(void);
static bsp_opbt_status_t r_bsp_issue_switch_programming_command(uint32_t cva_value);
static bsp_opbt_status_t r_bsp_issue_tag_check_flag(void);
static bsp_opbt_status_t r_bsp_issue_tag_earse_command(void);
static bsp_opbt_status_t r_bsp_issue_tag_update_command(void);
static bsp_opbt_status_t r_bsp_shift_to_read_mode(void);
static bsp_opbt_status_t r_bsp_shift_to_programming_mode(void);
static bsp_opbt_status_t r_bsp_configuration_setting_erase(void);
static bsp_opbt_status_t r_bsp_configuration_setting_flash(const uint32_t * p_src,
                                                           const uint32_t   nums_of_regs,
                                                           const uint32_t   base_address);
static bsp_opbt_status_t r_bsp_shift_to_program_erase_flash(void);
static bsp_opbt_status_t r_bsp_exit_program_erase_flash(void);

static void              r_bsp_opbt_read(uint32_t base_address, uint32_t offset, uint32_t * p_dest, uint32_t length);
static bsp_opbt_status_t r_bsp_opbt_switch_area(void);
static bsp_opbt_status_t r_bsp_opbt_programming(void);
static bsp_opbt_status_t bsp_opbt_wait_flash_ready(volatile uint32_t timeout_ms);
static void              r_bsp_return_from_command_lock_state(void);

/** Array contains the OPT value of the valid side */
static uint32_t g_otp_buff[BSP_OPBT_NUMBER_OF_OTP_REGS];

/** Array contains the OPBT value of the valid side */
static uint32_t g_opbt_buff[BSP_OPBT_NUMBER_OF_OPBT_REGS];

/** Variable contains the CVA value */
static uint32_t g_cva_value;

/** Variable contains the SVA value */
static uint32_t g_sva_value;

/** Variable contains the BVA0 value */
static uint32_t g_bva0_value;

#if (BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE)

/** Variable contains the BVA1 value */
static uint32_t g_bva1_value;
#endif

/*******************************************************************************************************************//**
 * This function changes the valid bank of the devices when in double map mode. In single map mode, this function
 * has no effect.
 *
 * @param[in]  bank_select                          .
 *
 * @retval  BSP_OPBT_STATUS_SUCCESS                 The Switch Area operation was success.
 * @retval  BSP_OPBT_STATUS_TIMEOUT                 One of the command was timeout.
 * @retval  BSP_OPBT_STATUS_FAIL                    The Switch Area operation was fail.
 * @retval  BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED     One of the Area was not programmed correctly.
 * @retval  BSP_OPBT_STATUS_VERIFY_MISMATCH         The OPBT values don't match the input data.
 **********************************************************************************************************************/
bsp_opbt_status_t R_BSP_DoubleMapValidBankSelect (bsp_opbt_double_map_valid_area_select_t bank_select)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    /* Read the registers' value from offset 0x200 to 0x21C of Configuration Setting Area */
    r_bsp_opbt_read(BSP_OPBT_CSAF_BASE, BSP_OPBT_OTP_SETTING_OFFSET, g_otp_buff, BSP_OPBT_NUMBER_OF_OTP_REGS);

    /* Read OPBT value from the valid side */
    r_bsp_opbt_read(BSP_OPBT_CSAF_BASE,
                    BSP_OPBT_SOFTWARE_CONFIGURATION_OPTION_BYTE_OFFSET,
                    g_opbt_buff,
                    BSP_OPBT_NUMBER_OF_OPBT_REGS);

    /* Disable Instruction Cache to make sure the value will be written correctly */
    R_BSP_InstructionCacheDisable();

    /* Change the valid bank in OPBT13 */
    if (1U == (((uint8_t) bank_select) % 2U))
    {
        g_opbt_buff[13U + BSP_OPBT_REGISTER_OFFSET] = (g_opbt_buff[13U + BSP_OPBT_REGISTER_OFFSET]) |
                                                      (1U << (((uint8_t) bank_select) / 2U));
    }
    else
    {
        g_opbt_buff[13U + BSP_OPBT_REGISTER_OFFSET] = (g_opbt_buff[13U + BSP_OPBT_REGISTER_OFFSET]) &
                                                      ~(1U << (((uint8_t) bank_select) / 2U));
    }

    do
    {
        /* Enter Program/ Erase Mode for flash memory */
        status = r_bsp_shift_to_program_erase_flash();

        /* Verify that the MCU has entered the Programming Mode */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Perform OPBT Program */
        status = r_bsp_opbt_programming();

        /* Verify that OPBT Programming command was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Issue OPBT switch area */
        status = r_bsp_opbt_switch_area();

        /* Verify that OPBT Switch Area command was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Exit Program/ Erase Mode for flash memory */
        status = r_bsp_exit_program_erase_flash();
    } while (0);

    /* Enable Instruction Cache again*/
    R_BSP_InstructionCacheEnable();

    return status;
}

#if (BSP_OPBT_API_USED_ENABLE)

/*******************************************************************************************************************//**
 * This function programms the custom OPBT values to invalid side of Configuration Setting Area.
 *
 * @param[in]  p_src       Pointer to array containing OPBT value need to be programmed.
 *
 * @retval  BSP_OPBT_STATUS_SUCCESS                 The OPBT Program operation was success.
 * @retval  BSP_OPBT_STATUS_TIMEOUT                 One of the command was timeout.
 * @retval  BSP_OPBT_STATUS_FAIL                    The OPBT Program operation was fail.
 **********************************************************************************************************************/
bsp_opbt_status_t R_BSP_OPBTProgram (const uint32_t * p_src)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    /* Read the registers' value from offset 0x200 to 0x21C of Configuration Setting Area */
    r_bsp_opbt_read(BSP_OPBT_CSAF_BASE, BSP_OPBT_OTP_SETTING_OFFSET, g_otp_buff, BSP_OPBT_NUMBER_OF_OTP_REGS);

    /* Read the input data and store it into g_opbt_buff */
    r_bsp_opbt_read((uint32_t) p_src, 0x00U, g_opbt_buff, BSP_OPBT_NUMBER_OF_OPBT_REGS);

    /* Disable Instruction Cache to make sure the value will be written correctly */
    R_BSP_InstructionCacheDisable();

    do
    {
        /* Enter Program/ Erase Mode for flash memory */
        status = r_bsp_shift_to_program_erase_flash();

        /* Verify that the MCU has entered the Programming Mode */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Perform OPBT Program */
        status = r_bsp_opbt_programming();

        /* Verify that OPBT Programming command was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Issue OPBT switch area */
        status = r_bsp_opbt_switch_area();

        /* Verify that OPBT Switch Area command was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Exit Program/ Erase Mode for flash memory */
        status = r_bsp_exit_program_erase_flash();
    } while (0);

    /* Enable Instruction Cache again*/
    R_BSP_InstructionCacheEnable();

    return status;
}

#endif

/*******************************************************************************************************************//**
 * This function reads the valid side of Configuration Setting Area.
 *
 * @retval  None
 **********************************************************************************************************************/
void R_BSP_OPBTRead (uint32_t * p_dest, uint32_t length)
{
    r_bsp_opbt_read(BSP_OPBT_CSAF_BASE, BSP_OPBT_SOFTWARE_CONFIGURATION_OPTION_BYTE_OFFSET, p_dest, length);
}

/*******************************************************************************************************************//**
 * This function verifies the OPBT values on the valid side with the input data.
 *
 * This function reads the specified Software Configuration Option Byte (OPBT)
 * from the valid configuration area and compares it with the expected value.
 *
 * @param[in]  configuration_name   Software configuration OPBT identifier.
 *                                  This value specifies which OPBT index to verify.
 *
 * @param[in]  expected_value       Expected value of the specified OPBT.
 *
 * @retval  BSP_OPBT_STATUS_VERIFY_OK                 The OPBT values match the input data.
 * @retval  BSP_OPBT_STATUS_VERIFY_MISMATCH           The OPBT values don't match the input data.
 **********************************************************************************************************************/
bsp_opbt_status_t R_BSP_OPBTVerify (bsp_configuration_area_t configuration_name, uint32_t expected_value)
{
    uint32_t opbt_index;
    uint32_t actual_value;

    /* Convert configuration identifier to OPBT index */
    opbt_index = (uint32_t) configuration_name;

    /* Read OPBT value from valid configuration area */
    actual_value = *(volatile uint32_t *)
                   (BSP_OPBT_CSAF_BASE + BSP_OPBT_SOFTWARE_CONFIGURATION_OPTION_BYTE_OFFSET + (4U * opbt_index));

    if (actual_value != expected_value)
    {
        return BSP_OPBT_STATUS_VERIFY_MISMATCH;
    }

    return BSP_OPBT_STATUS_VERIFY_OK;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Execute the Forced Stop Command. The Forced Stop Command is used to abort the command being processed by the
 * flash sequencer.
 **********************************************************************************************************************/
static void r_bsp_issue_force_stop_command (void)
{
    volatile uint8_t timeout_us;

    do
    {
        timeout_us = BSP_OPBT_VALUE_FORCE_STOP_TIMEOUT;

        /* Issue Force Stop Command */
        BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_FORCE_STOP;

        /* Wait for FRDY bit in FSTATR_0 register set */
        while ((0U == (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_FRDY_BIT_MSK)) && (timeout_us))
        {
            R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MICROSECONDS);
            timeout_us--;
        }
    } while (BSP_OPBT_FASTAT_0 & BSP_OPBT_FASTAT_0_CMDLK_BIT_MSK);
}

/*******************************************************************************************************************//**
 * Execute the Switch Erasure Command. The Switch Erasure Command is used to erase the back side (invalid) of Switch
 * Area.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_issue_switch_erasure_command (void)
{
    bsp_opbt_status_t status;

    /* Set the start address of Switch Area */
    BSP_OPBT_FSADDR_0 = (BSP_OPBT_SWAB_BASE + BSP_OPBT_CVA_OFFSET);

    /* Issue the Switch Erasure Command */
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_SWITCH_ERASURE_1ST_ACCESS;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_SWITCH_ERASURE_2ND_ACCESS;

    status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_SWITCH_ERASURE_TIMEOUT);

    return status;
}

/*******************************************************************************************************************//**
 * Execute the Switch Programming Command. The Switch Programming Command is used to write to the back side (invalid)
 * of Switch Area. This function only changes the CVA flag to execute the switch operation of Configration Setting Area.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_issue_switch_programming_command (uint32_t cva_value)
{
    bsp_opbt_status_t status;
    uint8_t           repetition_count;

    /* Set the start address of Switch Area */
    BSP_OPBT_FSADDR_0 = (BSP_OPBT_SWAB_BASE + BSP_OPBT_CVA_OFFSET);

    /* Issue the Switch Programming Command */
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_1ST_ACCESS;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_2ND_ACCESS;

    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = g_cva_value;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = g_sva_value;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = g_bva0_value;

#if (BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE)
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = g_bva1_value;
#else
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = 0xFFFFFFFFU;
#endif
    for (repetition_count = 0U; repetition_count < 4U; repetition_count++)
    {
        BSP_OPBT_FACI0_COMMAND_ISSUING_UINT32_T = 0xFFFFFFFFU;
    }

    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_SWITCH_PROGRAMMING_3RD_ACCESS;

    status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_SWITCH_PROGRAMMING_TIMEOUT);

    return status;
}

/*******************************************************************************************************************//**
 * Perform AnPC, BSP_OPBT_CSAVOFCn flag checking.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_issue_tag_check_flag (void)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    /* Check whether Switch Area is programmed */
    if (BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED == BSP_OPBT_A0PC_B)
    {
        if (BSP_OPBT_BSP_CONFIGURATION_SETTING_AREA_1_VALID == BSP_OPBT_CVA_INVALID_SIDE)
        {
            /* Check whether the Switch Area value is valid */
            if (0U == (BSP_OPBT_FSWASTAT_0 & BSP_OPBT_FSWASTAT_0_CFGVA_BIT_MSK))
            {
                /* Check BSP_OPBT_CSAVOFCn (n = 0, 8 to 19). If one of these register is not programmed, issue Switch Erasure Command */
                if (BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED != BSP_OPBT_CSAVOFCn(0))
                {
                    r_bsp_issue_switch_erasure_command();

                    status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
                }
                else
                {
                    uint8_t not_programmed_flag = 0U;
                    uint8_t reg_index;
                    for (reg_index = 8U; reg_index <= 19U; reg_index++)
                    {
                        if (BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED != BSP_OPBT_CSAVOFCn(reg_index))
                        {
                            not_programmed_flag = 1U;
                            break;
                        }
                    }

                    if (not_programmed_flag)
                    {
                        r_bsp_issue_switch_erasure_command();

                        status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
                    }
                }
            }
        }
        else if (BSP_OPBT_BSP_CONFIGURATION_SETTING_AREA_0_VALID == BSP_OPBT_CVA_INVALID_SIDE)
        {
            /* Check whether the Switch Area value is valid */
            if (0U != (BSP_OPBT_FSWASTAT_0 & BSP_OPBT_FSWASTAT_0_CFGVA_BIT_MSK))
            {
                /* Check BSP_OPBT_CSAVOFCn (n = 0, 8 to 19). If one of these register is not programmed, issue Switch Erasure Command */
                if (BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED != BSP_OPBT_CSAVOFCn(0))
                {
                    r_bsp_issue_switch_erasure_command();

                    status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
                }
                else
                {
                    uint8_t not_programmed_flag = 0U;
                    uint8_t reg_index;
                    for (reg_index = 8; reg_index <= 19; reg_index++)
                    {
                        if (BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED != BSP_OPBT_CSAVOFCn(reg_index))
                        {
                            not_programmed_flag = 1U;
                            break;
                        }
                    }

                    if (not_programmed_flag)
                    {
                        r_bsp_issue_switch_erasure_command();

                        status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
                    }
                }
            }
        }
        else
        {
            /* If CVA is neither Area 0 nor Area 1, issue Switch Erasure Command */
            r_bsp_issue_switch_erasure_command();

            status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
        }
    }
    else
    {
        /* If AnPC is not programmed, issue Switch Erasure Command */
        r_bsp_issue_switch_erasure_command();

        status = BSP_OPBT_STATUS_AREA_NOT_PROGRAMMED;
    }

    return status;
}

/*******************************************************************************************************************//**
 * Execute the TAG Erasure Command. The TAG Erasure Command and TAG Update Command are used for the change of the
 * data in TAG Area.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_issue_tag_earse_command (void)
{
    bsp_opbt_status_t status;

    /* Set the start address of TAG Area */
    BSP_OPBT_FSADDR_0 = (BSP_OPBT_TAG_BASE + BSP_OPBT_VAF_OFFSET);

    /* Issue the TAG Erasure Command */
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_TAG_ERASURE_1ST_ACCESS;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_TAG_ERASURE_2ND_ACCESS;

    status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_TAG_ERASE_TIMEOUT);

    return status;
}

/*******************************************************************************************************************//**
 * Execute the TAG Update Command. The TAG Erasure Command and TAG Update Command are used for the change of the
 * data in TAG Area.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_issue_tag_update_command (void)
{
    bsp_opbt_status_t status;

    /* Set the start address of TAG Area */
    BSP_OPBT_FSADDR_0 = (BSP_OPBT_TAG_BASE + BSP_OPBT_VAF_OFFSET);

    /* Issue the TAG Update Command */
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_TAG_UPDATE_1ST_ACCESS;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_TAG_UPDATE_2ND_ACCESS;

    status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_TAG_UPDATE_TIMEOUT);

    return status;
}

/*******************************************************************************************************************//**
 * Shift the MCU to read mode to read the flash memory.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_shift_to_read_mode (void)
{
    bsp_opbt_status_t status     = BSP_OPBT_STATUS_SUCCESS;
    volatile uint16_t timeout_ms = BSP_OPBT_VALUE_SHIFT_TO_READ_TIMEOUT;

    /* Write 0xAA00 to BSP_OPBT_FENTRYR_0 register */
    BSP_OPBT_FENTRYR_0 = BSP_OPBT_FACI_COMMAND_SHIFT_TO_READ_MODE;

    /* Wait for FENTRYD bit to be cleared */
    while ((0U != (BSP_OPBT_FENTRYR_0 & BSP_OPBT_FENTRYR_0_FENTRYD_BIT_MSK)) && (timeout_ms))
    {
        R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
    }

    if (0U == timeout_ms)
    {
        status = BSP_OPBT_STATUS_TIMEOUT;
    }

    return status;
}

/*******************************************************************************************************************//**
 * Shift the MCU to data flash programming mode to execute operations related to Data Flash erase/program, Switch
 * erase/program and TAG earse/update.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_shift_to_programming_mode (void)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    /* Write 0xAA80 to BSP_OPBT_FENTRYR_0 register */
    BSP_OPBT_FENTRYR_0 = BSP_OPBT_FACI_COMMAND_SHIFT_TO_PROGRAMMING_MODE;

    volatile uint16_t timeout_ms = BSP_OPBT_VALUE_SHIFT_TO_PROGRAM_TIMEOUT;

    /* Wait for FENTRYD bit to be set */
    while ((0U == (BSP_OPBT_FENTRYR_0 & BSP_OPBT_FENTRYR_0_FENTRYD_BIT_MSK)) && (timeout_ms))
    {
        R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
    }

    if ((0U == timeout_ms) && (0U == (BSP_OPBT_FENTRYR_0 & BSP_OPBT_FENTRYR_0_FENTRYD_BIT_MSK)))
    {
        status = BSP_OPBT_STATUS_TIMEOUT;

        /* If it's timeout, shift to read mode */
        r_bsp_shift_to_read_mode();
    }

    return status;
}

/*******************************************************************************************************************//**
 * Perform the Configuration Setting Area erase.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_configuration_setting_erase (void)
{
    bsp_opbt_status_t status;

    /* Set the start address of Configuration Setting Area */
    BSP_OPBT_FSADDR_0 = (BSP_OPBT_CSAB_BASE + BSP_OPBT_OTP_SETTING_OFFSET);

    /* Issue the Configuration Setting Area Erasure Command */
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_PROPERTY_ERASURE_1ST_ACCESS;
    BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
        BSP_OPBT_FACI_COMMAND_PROPERTY_ERASURE_2ND_ACCESS;

    status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_PROPERTY_ERASURE_TIMEOUT);

    return status;
}

/*******************************************************************************************************************//**
 * Perform the Configuration Setting Area programming.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_configuration_setting_flash (const uint32_t * p_src,
                                                            const uint32_t   nums_of_regs,
                                                            const uint32_t   base_address)
{
    uint8_t           repetition_count;
    uint8_t           byte_count;
    volatile uint8_t  timeout;
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    /* Issue the Configuration Setting Area programming. Can only program 32 bytes / command */
    for (repetition_count = 0U; repetition_count < (nums_of_regs / 8U); repetition_count++)
    {
        BSP_OPBT_FSADDR_0 = base_address + (repetition_count * 32U);

        /* Issue the Configuration Setting Area programming command */
        BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
            BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_1ST_ACCESS;
        BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T =
            BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_2ND_ACCESS;

        for (byte_count = 0U; byte_count < 8U; byte_count++)
        {
            /* Write the value from the source to the Configuration Setting Area */
            uint32_t write_value = *p_src++;
            *(volatile uint32_t *) BSP_OPBT_FACI0_COMMAND_ISSUING_AREA_BASE = write_value;
        }

        BSP_OPBT_FACI0_COMMAND_ISSUING_UINT8_T = BSP_OPBT_FACI_COMMAND_PROPERTY_PROGRAMMING_3RD_ACCESS;

        status = bsp_opbt_wait_flash_ready(BSP_OPBT_VALUE_PROPERTY_PROGRAMMING_TIMEOUT);
    }

    return status;
}

/*******************************************************************************************************************//**
 * Enter Program / Erase Data Flash Mode.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_shift_to_program_erase_flash (void)
{
    bsp_opbt_status_t status;

    BSP_OPBT_FHVE3FP_0  |= 1U;
    BSP_OPBT_FHVE15FP_0 |= 1U;

    status = r_bsp_shift_to_programming_mode();

    return status;
}

/*******************************************************************************************************************//**
 * Exit Program / Erase Data Flash Mode.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_exit_program_erase_flash (void)
{
    bsp_opbt_status_t status;
    status = r_bsp_shift_to_read_mode();

    BSP_OPBT_FHVE3FP_0  &= 0U;
    BSP_OPBT_FHVE15FP_0 &= 0U;

    return status;
}

/*******************************************************************************************************************//**
 * Perform reading operation from (base_address + offset) to (base_address + offset + legth * 4U).
 **********************************************************************************************************************/
static void r_bsp_opbt_read (uint32_t base_address, uint32_t offset, uint32_t * p_dest, uint32_t length)
{
    uint32_t reg_index;

    for (reg_index = 0U; reg_index < length; reg_index++)
    {
        p_dest[reg_index] = *(volatile uint32_t *) (base_address + offset + (4U * reg_index));
    }
}

/*******************************************************************************************************************//**
 * Issue Switch Area command.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_opbt_switch_area (void)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    do
    {
        /* Switch the CVA value */
        if (BSP_OPBT_VAF == BSP_OPBT_BSP_CONFIGURATION_SETTING_AREA_0_VALID)
        {
            g_cva_value = BSP_OPBT_BSP_CONFIGURATION_SETTING_AREA_1_VALID;
        }
        else
        {
            g_cva_value = BSP_OPBT_BSP_CONFIGURATION_SETTING_AREA_0_VALID;
        }

        /* Read value of SVA, BVAn of the valid side */
        g_sva_value  = BSP_OPBT_SVA_F;
        g_bva0_value = BSP_OPBT_BVA0_F;

#if (BSP_FEATURE_OPBT_BVA1_AREA_IS_AVAILABLE)
        g_bva1_value = BSP_OPBT_BVA1_F;
#endif

        if (BSP_OPBT_VAPC == BSP_OPBT_SWITCH_AREA_IS_PROGRAMMED)
        {
            /* Perform Switch Erasure Command for Switch Area */
            status = r_bsp_issue_switch_erasure_command();

            /* Verify the Erasure Command for Switch Area was success */
            BSP_OPBT_BREAK_IF_ERROR(status);

            /* Perform Switch Programming Command for Switch Area*/
            status = r_bsp_issue_switch_programming_command(g_cva_value);

            /* Verify the Switch Programming Command for Switch Area was success */
            BSP_OPBT_BREAK_IF_ERROR(status);
        }

        /* Perform flag checking for TAG Area */
        status = r_bsp_issue_tag_check_flag();

        /* Verify the flag checking for TAG Area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Perform TAG Erasure Command for TAG Area*/
        status = r_bsp_issue_tag_earse_command();

        /* Verify the Perform TAG Erasure Command for TAG Area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Perform flag checking for TAG Area */
        status = r_bsp_issue_tag_check_flag();

        /* Verify the flag checking for TAG Area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Perform TAG Update Command for TAG Area*/
        status = r_bsp_issue_tag_update_command();

        /* Verify the Perform TAG Update Command for TAG Area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);
    } while (0);

    return status;
}

/*******************************************************************************************************************//**
 * Issue Configuration Setting Area Program command.
 **********************************************************************************************************************/
static bsp_opbt_status_t r_bsp_opbt_programming (void)
{
    bsp_opbt_status_t status = BSP_OPBT_STATUS_SUCCESS;

    do
    {
        /* Force Stop */
        r_bsp_issue_force_stop_command();

        /* Perform Erasure Command for Configuration Setting Area */
        status = r_bsp_configuration_setting_erase();

        /* Verify the Erase the Configuration Setting Area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Flash the OPT area */
        status =
            r_bsp_configuration_setting_flash((const uint32_t *) g_otp_buff, BSP_OPBT_NUMBER_OF_OTP_REGS,
                                              (BSP_OPBT_CSAB_BASE + BSP_OPBT_OTP_SETTING_OFFSET));

        /* Verify the Flash the OPT area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);

        /* Force Stop */
        r_bsp_issue_force_stop_command();

        /* Flash the OPBT area */
        r_bsp_configuration_setting_flash((const uint32_t *) g_opbt_buff, BSP_OPBT_NUMBER_OF_OPBT_REGS,
                                          (BSP_OPBT_CSAB_BASE + BSP_OPBT_SOFTWARE_CONFIGURATION_OPTION_BYTE_OFFSET));

        /* Verify the Flash the OPBT area was success */
        BSP_OPBT_BREAK_IF_ERROR(status);
    } while (0);

    return status;
}

/*******************************************************************************************************************//**
 * Wait for the FACI command processing.
 **********************************************************************************************************************/
static bsp_opbt_status_t bsp_opbt_wait_flash_ready (volatile uint32_t timeout_ms)
{
    while (0U == (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_FRDY_BIT_MSK))
    {
        if (0U == timeout_ms)
        {
            /* Timeout -> Abort current operation */
            r_bsp_issue_force_stop_command();

            return BSP_OPBT_STATUS_TIMEOUT;
        }

        R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);

        timeout_ms--;
    }

    /* FRDY == 1 -> Sequencer idle */
    if (BSP_OPBT_FASTAT_0 & BSP_OPBT_FASTAT_0_CMDLK_BIT_MSK)
    {
        r_bsp_return_from_command_lock_state();

        return BSP_OPBT_STATUS_FAIL;
    }

    return BSP_OPBT_STATUS_SUCCESS;
}

/*******************************************************************************************************************//**
 * Release the command lock state.
 **********************************************************************************************************************/
static void r_bsp_return_from_command_lock_state (void)
{
    volatile uint16_t timeout_ms = BSP_OPBT_VALUE_RETURN_FROM_COMMAND_LOCK_TIMEOUT;

    /* Wait for FRDY bit in FSTATR_0 register set */
    while ((0U == (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_FRDY_BIT_MSK)) && (timeout_ms))
    {
        R_BSP_SoftwareDelay(1U, BSP_DELAY_UNITS_MILLISECONDS);
        timeout_ms--;
    }

    /* If FRDY bit is not set, issue force stop */
    if ((0U == timeout_ms) && (0U == (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_FRDY_BIT_MSK)))
    {
        r_bsp_issue_force_stop_command();
    }
    else if (0U != (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_FRDY_BIT_MSK))
    {
        /* Check ILGLERR bit */
        if (0U == (BSP_OPBT_FSTATR_0 & BSP_OPBT_FSTATR_0_ILGLERR_BIT_MSK))
        {
            if ((BSP_OPBT_FASTAT_0 & BSP_OPBT_FASTAT_0_CFAE_BIT_MSK) ||
                (BSP_OPBT_FASTAT_0 & BSP_OPBT_FASTAT_0_DFAE_BIT_MSK))
            {
                BSP_OPBT_FASTAT_0 &= ~(BSP_OPBT_FASTAT_0_CFAE_BIT_MSK | BSP_OPBT_FASTAT_0_DFAE_BIT_MSK);
            }
        }

        r_bsp_issue_force_stop_command();
    }
    else
    {
        /* Do nothing */
    }
}

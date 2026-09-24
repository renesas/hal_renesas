/*
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
*
*/


/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdio.h>
#include "ecm/r_ecm.h"
#include "r_ecm_reg.h"
#include "interrupts.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void ECM_RcarInterruptHandler (void* p_context);
static uint32_t ECM_GetErrorID(void);

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/
uint8_t R_ECM_SetInterruptCallback(IrqErrorHandlerFn irq_handler)
{
    uint8_t result = 0;
    /* Set Handler for Irq */
    Irq_SetupEntry(INTID_ECMERR_MERGE, ECM_RcarInterruptHandler,(Context_t*) irq_handler);

    /* Set priority for Irq */
    Irq_SetPriority(INTID_ECMERR_MERGE, IPRIORITY(2));

    /* Enable Irq */
    Irq_Enable(INTID_ECMERR_MERGE);

    return result;
}

uint8_t R_ECM_SetDetection(e_ecm_error_id_t id, int8_t isEnable)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERRCTLR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    if (isEnable == 0)
    {
        value = READ_REGISTER_32(reg_addr);
        value &= ~ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }
    else
    {
        value = READ_REGISTER_32(reg_addr);
        value |= ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }

    value = READ_REGISTER_32(reg_addr);

    if (isEnable == 0)
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) != 0)
        {
            result = 1;
        }
    }
    else
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) == 0)
        {
            result = 1;
        }
    }

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);
    return result;
}

uint8_t R_ECM_SetPinOut(e_ecm_error_id_t id, int8_t isEnable)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERROMKR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    if (isEnable == 0)
    {
        value = READ_REGISTER_32(reg_addr);
        value &= ~ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }
    else
    {
        value = READ_REGISTER_32(reg_addr);
        value |= ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }

    value = READ_REGISTER_32(reg_addr);

    if (isEnable == 0)
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) != 0)
        {
            result = 1;
        }
    }
    else
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) == 0)
        {
            result = 1;
        }
    }

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);
    return result;
}

uint8_t R_ECM_SetReset(e_ecm_error_id_t id, int8_t isEnable)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERRRSTR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    if (isEnable == 0)
    {
        value = READ_REGISTER_32(reg_addr);
        value &= ~ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }
    else
    {
        value = READ_REGISTER_32(reg_addr);
        value |= ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }

    value = READ_REGISTER_32(reg_addr);

    if (isEnable == 0)
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) != 0)
        {
            result = 1;
        }
    }
    else
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) == 0)
        {
            result = 1;
        }
    }

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);
    return result;    
}

uint8_t R_ECM_SetInterruptNotification(e_ecm_error_id_t id, int8_t isEnable)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERRINCR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    if (isEnable == 0)
    {
        value = READ_REGISTER_32(reg_addr);
        value &= ~ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }
    else
    {
        value = READ_REGISTER_32(reg_addr);
        value |= ECM_SET_BIT(ecm_reg_bit);
        WRITE_REGISTER_32(reg_addr, value);
    }

    value = READ_REGISTER_32(reg_addr);

    if (isEnable == 0)
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) != 0)
        {
            result = 1;
        }
    }
    else
    {
        if ((value & ECM_SET_BIT(ecm_reg_bit)) == 0)
        {
            result = 1;
        }
    }

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);
    return result;
}

uint8_t R_ECM_DisableAll()
{
    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    for (int ecm_register_index = 0; ecm_register_index < ECM_REGISTER_NUMBER; ecm_register_index++)
    {
        WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRINCR(ecm_register_index), ECM_ECMERRINCR_CLEAR);
        WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRCTLR(ecm_register_index), ECM_ECMERRCTLR_CLEAR);
        WRITE_REGISTER_32(DRV_REG_ADDR_ECM_SAFCLERRENR,   ECM_SAFCLERRENR_CLEAR);
        WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_register_index), ECM_ECMERRSTSR_CLEAR);
    }

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);
}

uint8_t R_ECM_CheckErrorStatus(e_ecm_error_id_t id)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    value = READ_REGISTER_32(reg_addr);
    if ((value & ECM_SET_BIT(ecm_reg_bit)) == 0)
    {
        result = 1;
    }

    return result;
}

uint8_t R_ECM_PseudoError(e_ecm_error_id_t id)
{
    uint32_t ecm_reg_num = (uint32_t)id / 32;
    uint32_t ecm_reg_bit = (uint32_t)id % 32;
    uint32_t reg_addr = DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_reg_num);
    uint32_t value;
    uint32_t result = 0;

    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    value = ECM_PUSEDO_ERROR_ENABLE + ecm_reg_num;
    /* Enable Pseudo error */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_SAFCTLR, value);
    /* Inject pseudo error ECM0 bit29 */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_SAFSTERRENR, ECM_SET_BIT(ecm_reg_bit));

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);

    return result;
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/
static void ECM_RcarInterruptHandler (void* p_context)
{
    IrqErrorHandlerFn ecm_handler = (IrqErrorHandlerFn)p_context;
    uint32_t ecm_reg_num, reg_addr, value;
    uint32_t ecm_reg_bit;

    uint32_t ecm_id = ECM_GetErrorID();
    ecm_reg_num = (uint32_t)ecm_id / 32;
    ecm_reg_bit = (uint32_t)ecm_id % 32;
    reg_addr = DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_reg_num);
    
    value = ECM_SET_BIT(ecm_reg_bit);
    /* Disable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_DISABLE);

    WRITE_REGISTER_32(reg_addr, value);
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_SAFCLERRENR,   ECM_SAFCLERRENR_CLEAR);

    /* Enable Write Protection ECM */
    WRITE_REGISTER_32(DRV_REG_ADDR_ECM_ECMWPCNTR, ECM_WRITE_PROTECTION_ENABLE);

    ecm_handler(ecm_id);
}

static uint32_t ECM_GetErrorID(void)
{
    uint32_t ecm_reg_num = 0xFFFFFFFFU;
    uint32_t ecm_reg_bit = 0xFFFFFFFFU;
    uint32_t value;
    uint32_t result;

    if(READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRINTSTSR0) != ECM_ECMERRINTSTSR0_CLEAR)
    {
        value = READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRINTSTSR0);
        for (uint32_t bit = 0; bit < 32; bit++)
        {
            if ((value & ((uint32_t)1 << bit)) != 0U)
            {
                ecm_reg_num = bit;     
                break;
            }
        }

        if (ecm_reg_num != 0xFFFFFFFFU)
        {
            value = READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_reg_num));
            for (uint32_t bit = 0; bit < 32; bit++)
            {
                if ((value & ((uint32_t)1 << bit)) != 0U)
                {
                    ecm_reg_bit = bit;     
                    break;
                }
            }

            if (ecm_reg_bit == 0xFFFFFFFFU)
            {
                result = 0xFFFFFFFFU;
            }

            result = ecm_reg_num *32 + ecm_reg_bit;
        }
        else
        {
            result = 0xFFFFFFFFU;
        }
        
    }
    else if(READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRINTSTSR1) != ECM_ECMERRINTSTSR1_CLEAR)
    {
        value = READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRINTSTSR1);
        for (uint32_t bit = 0; bit < 32; bit++)
        {
            if ((value & ((uint32_t)1 << bit)) != 0U)
            {
                ecm_reg_num = bit + 32;     
                break;
            }
        }

        if (ecm_reg_num != 0xFFFFFFFFU)
        {
            value = READ_REGISTER_32(DRV_REG_ADDR_ECM_ECMERRSTSR(ecm_reg_num));
            for (uint32_t bit = 0; bit < 32; bit++)
            {
                if ((value & ((uint32_t)1 << bit)) != 0U)
                {
                    ecm_reg_bit = bit;     
                    break;
                }
            }

            if (ecm_reg_bit == 0xFFFFFFFFU)
            {
                result = 0xFFFFFFFFU;
            }

            result = ecm_reg_num *32 + ecm_reg_bit;
        }
        else
        {
            result = 0xFFFFFFFFU;
        }
    }
    else
    {
        result = 0xFFFFFFFFU;
    }

    return result;
}

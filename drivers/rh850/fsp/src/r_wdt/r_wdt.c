/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_wdt.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define WDT_OPEN                   (0X00574454ULL) // "WDT" in ASCII

/** Enables/disables the WDT */
#define WDT_PRV_START_CODE         (0xAC)

#define WDT_PRV_ACTIVATION_CODE    (0xAC)

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void wdt_isr(void);

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

/** Watchdog implementation of WDT Driver  */
const wdt_api_t g_wdt_on_wdt =
{
    .open        = R_WDT_Open,
    .refresh     = R_WDT_Refresh,
    .statusGet   = R_WDT_StatusGet,
    .statusClear = R_WDT_StatusClear,
    .counterGet  = R_WDT_CounterGet,
    .timeoutGet  = R_WDT_TimeoutGet,
    .callbackSet = R_WDT_CallbackSet,
};

/* WDTB Unit Base Addresses */
const uint32_t g_wdtb_unit_base_addr[] =
{
#if defined(R_WDTBA)
    (uint32_t) R_WDTBA,
#endif
#if defined(R_WDTB0)
    (uint32_t) R_WDTB0,
#endif
#if defined(R_WDTB1)
    (uint32_t) R_WDTB1,
#endif
#if defined(R_WDTB2)
    (uint32_t) R_WDTB2,
#endif
#if defined(R_WDTB3)
    (uint32_t) R_WDTB3,
#endif
#if defined(R_WDTB4)
    (uint32_t) R_WDTB4,
#endif
#if defined(R_WDTB5)
    (uint32_t) R_WDTB5,
#endif
#if defined(R_WDTB6)
    (uint32_t) R_WDTB6
#endif
};

/*******************************************************************************************************************//**
 * @addtogroup WDT WDT
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure the WDT in register start mode. Implements
 *
 * This function should only be called once as WDT configuration registers can only be written to once so subsequent
 * calls will have no effect.
 *
 * @retval FSP_SUCCESS              WDT successfully configured.
 * @retval FSP_ERR_ASSERTION        Null pointer, one or more configuration options is invalid.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.  This module can only be opened once.
 **********************************************************************************************************************/
fsp_err_t R_WDT_Open (wdt_ctrl_t * const p_ctrl, wdt_cfg_t const * const p_cfg)
{
    wdt_instance_ctrl_t * p_instance_ctrl = (wdt_instance_ctrl_t *) p_ctrl;

#if WDT_CFG_PARAM_CHECKING_ENABLE

    /* Check that control and config structure pointers are valid. */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    wdt_extended_cfg_t * p_extend = (wdt_extended_cfg_t *) p_cfg->p_extend;
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(WDT_OPEN != p_instance_ctrl->wdt_open, FSP_ERR_ALREADY_OPEN);
#else
    wdt_extended_cfg_t * p_extend = (wdt_extended_cfg_t *) p_cfg->p_extend;
#endif

    uint8_t wdt_mode_setup = 0U;

    p_instance_ctrl->p_cfg = p_cfg;
    p_instance_ctrl->p_reg = (R_WDTB1_Type *) g_wdtb_unit_base_addr[p_extend->unit];

    /* Check validity of the parameters */
    if ((FSP_INVALID_VECTOR != p_extend->wdt_irq) && (0 <= p_extend->wdt_irq))
    {
        R_BSP_IrqCfgEnable(p_extend->wdt_irq, p_extend->ipl, p_ctrl);
    }

    /* Register callback. */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_context         = p_cfg->p_context;
    p_instance_ctrl->p_callback_memory = NULL;

    /* Set the window-open mode select*/
    wdt_mode_setup |= (uint8_t) p_extend->window_open_mode_select << R_WDTB1_WDTBnMD_WDTBnWMS_Pos;

    if (WDT_WINDOW_OPEN_FUNCTION_MODE_SELECT_BY_WS == (uint8_t) p_extend->window_open_mode_select)
    {
        /* Set the window-open period*/
        wdt_mode_setup |= (uint8_t) p_extend->window_open << R_WDTB1_WDTBnMD_WDTBnWS_Pos;
    }
    else
    {
        if (WDT_INTERRUPT_ENABLED == p_extend->isr_request)
        {
            /* Configures the timing of the WDTB interrupt (WDTBnTIT/WDTBATIT) generation */
            p_instance_ctrl->p_reg->WDTBnWIS = p_extend->window_open_interrupt_output_timimg;
        }

        /* Set the Window-open Start for setting start timing of the window open */
        p_instance_ctrl->p_reg->WDTBnWOST = p_extend->window_open_start;
    }

    if (WDT_UNIT_A != p_extend->unit)
    {
        /* Configure the WDT to use NMI mode when an error is detected by clearing the WDTBnERM bit */
        if (WDT_RESET_CONTROL_NMI == p_cfg->reset_control)
        {
            wdt_mode_setup &= (uint8_t) (~(1U << R_WDTB1_WDTBnMD_WDTBnERM_Pos));
        }
        /* Configure the WDT to use reset mode when an error is detected by setting the WDTBnERM bit */
        else if (WDT_RESET_CONTROL_RESET == p_cfg->reset_control)
        {
            wdt_mode_setup |= (uint8_t) (1U << R_WDTB1_WDTBnMD_WDTBnERM_Pos);
        }
        else
        {
            /* Do nothing */
        }
    }

    /* Enables/disables the 75% interrupt */
    wdt_mode_setup |= (uint8_t) p_extend->isr_request << R_WDTB1_WDTBnMD_WDTBnWIE_Pos;

    /* Set the overflow interval time */
    wdt_mode_setup |= (uint8_t) p_extend->overflow_time << R_WDTB1_WDTBnMD_WDTBnOVF_Pos;

    /* Set the mode of the watchdog */
    p_instance_ctrl->p_reg->WDTBnMD = wdt_mode_setup;

    p_instance_ctrl->wdt_open = WDT_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read timeout information for the watchdog timer.
 *
 * @retval FSP_ERR_UNSUPPORTED              Function not supported in this implementation
 *
 * @note This function is not supported in this implementation.
 *
 **********************************************************************************************************************/
fsp_err_t R_WDT_TimeoutGet (wdt_ctrl_t * const p_ctrl, wdt_timeout_values_t * const p_timeout)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_timeout);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Refresh the watchdog timer.
 *
 * In addition to refreshing the watchdog counter this function can be used to start the counter in register start mode.
 *
 *
 * @retval FSP_SUCCESS              WDT successfully refreshed.
 * @retval FSP_ERR_ASSERTION        p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN         Instance control block is not initialized.
 *
 * @note This function only returns FSP_SUCCESS. If the refresh fails due to being performed outside of the
 *       permitted refresh period the device will either reset or trigger an NMI ISR to run.
 **********************************************************************************************************************/
fsp_err_t R_WDT_Refresh (wdt_ctrl_t * const p_ctrl)
{
    wdt_instance_ctrl_t * p_instance_ctrl = (wdt_instance_ctrl_t *) p_ctrl;

#if WDT_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    wdt_extended_cfg_t * p_extend = (wdt_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    FSP_ASSERT(NULL != p_extend);
    FSP_ERROR_RETURN(WDT_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#else
    wdt_extended_cfg_t * p_extend = (wdt_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
#endif
    uint8_t  wdt_activation_code = (uint8_t) p_extend->activation_code;
    uint16_t wdt_ref             = 0U;

    if (WDT_ACTIVATION_CODE_FIX == wdt_activation_code)
    {
        p_instance_ctrl->p_reg->WDTBnWDTE = WDT_PRV_START_CODE;
    }
    else if (WDT_ACTIVATION_CODE_VARIABLE == wdt_activation_code)
    {
        wdt_ref = (uint16_t) (p_instance_ctrl->p_reg->WDTBnREF);
        p_instance_ctrl->p_reg->WDTBnEVAC = (uint8_t) (WDT_PRV_ACTIVATION_CODE - wdt_ref);
    }
    else
    {
        /* Do nothing */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read the WDT status flags. Implements .
 *
 * Indicates both status and error conditions.
 *
 * @retval FSP_ERR_UNSUPPORTED              Function not supported in this implementation
 *
 * @note This function is not supported in this implementation.
 *
 **********************************************************************************************************************/
fsp_err_t R_WDT_StatusGet (wdt_ctrl_t * const p_ctrl, wdt_status_t * const p_status)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_status);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Clear the WDT status and error flags. Implements
 *
 * @retval FSP_ERR_UNSUPPORTED              Function not supported in this implementation
 *
 * @note This function is not supported in this implementation.
 *
 **********************************************************************************************************************/
fsp_err_t R_WDT_StatusClear (wdt_ctrl_t * const p_ctrl, const wdt_status_t status)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(status);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Read the current count value of the WDT. Implement.
 *
 * @retval FSP_ERR_UNSUPPORTED              Function not supported in this implementation
 *
 * @note This function is not supported in this implementation.
 *
 **********************************************************************************************************************/
fsp_err_t R_WDT_CounterGet (wdt_ctrl_t * const p_ctrl, uint32_t * const p_count)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_count);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements wdt_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_WDT_CallbackSet (wdt_ctrl_t * const          p_ctrl,
                             void (                    * p_callback)(wdt_callback_args_t *),
                             void * const                p_context,
                             wdt_callback_args_t * const p_callback_memory)
{
    wdt_instance_ctrl_t * p_instance_ctrl = (wdt_instance_ctrl_t *) p_ctrl;

#if WDT_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(WDT_OPEN == p_instance_ctrl->wdt_open, FSP_ERR_NOT_OPEN);
#endif

    /* Store callback and context */
    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup R_WDT)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This function implements the 75% interrupt handler.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void wdt_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear the IR bit after calling the user callback so that if the condition is cleared the ISR will not
     * be called again. */
    R_BSP_IrqStatusClear(irq);

    wdt_instance_ctrl_t * p_ctrl = (wdt_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if ((NULL != p_ctrl) && (NULL != p_ctrl->p_callback))
    {
        /* Set data to identify callback to user, then call user callback. */
        wdt_callback_args_t args;

        /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
         * stored in non-secure memory so they can be accessed by a non-secure callback function. */
        wdt_callback_args_t * p_args = p_ctrl->p_callback_memory;

        if (NULL == p_args)
        {
            /* Store on stack */
            p_args = &args;
        }
        else
        {
            /* Save current arguments on the stack in case this is a nested interrupt. */
            args = *p_args;
        }

        p_args->p_context = p_ctrl->p_context;
        p_ctrl->p_callback(p_args);

        if (NULL != p_ctrl->p_callback_memory)
        {
            /* Restore callback memory in case this is a nested interrupt. */
            *p_ctrl->p_callback_memory = args;
        }
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

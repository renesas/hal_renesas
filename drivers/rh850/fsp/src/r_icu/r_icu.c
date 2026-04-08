/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_icu_cfg.h"
#include "r_icu.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "ICU" in ASCII, used to determine if channel is open. */
#define ICU_OPEN                 (0x00494355U)

#define FCLACTLm_nGAP            (0x00000004UL)

#define FCLACTL_0_MAX_ENTRIES    (8U)
#define FCLACTL_1_MAX_ENTRIES    (16U)
#define FCLACTL_2_MAX_ENTRIES    (24U)
#define FCLACTL_3_MAX_ENTRIES    (32U)
#define FCLACTL_4_MAX_ENTRIES    (40U)
#define FCLACTL_5_MAX_ENTRIES    (46U)

#if defined(R_FCLA_IRQ0_BASE)

/* IRQ-based interrupt configuration */
 #define FCLACTL                 FCLACTL0_IRQ_0
 #define R_FCLA_BASE(n)    R_FCLA_IRQ ## n ## _BASE
 #define R_FCLA_NMI_BASE         R_FCLA_IRQ5_BASE // NMI maps to R_FCLA_IRQ5_BASE
#else

/* INTP-based interrupt configuration */
 #define FCLACTL                 FCLACTL0_INTP_0
 #define R_FCLA_BASE(n)    R_FCLA_INTP ## n ## _BASE
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_icu_isr(void);
static void                  r_icu_nmi(bsp_grp_irq_t irq);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

static volatile icu_instance_ctrl_t * gp_icu_ctrl = NULL;

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

/* ICU implementation of External IRQ API. */
const external_irq_api_t g_external_irq_on_icu =
{
    .open        = R_ICU_ExternalIrqOpen,
    .enable      = R_ICU_ExternalIrqEnable,
    .disable     = R_ICU_ExternalIrqDisable,
    .callbackSet = R_ICU_ExternalIrqCallbackSet,
    .close       = R_ICU_ExternalIrqClose,
};

/*******************************************************************************************************************//**
 * @addtogroup ICU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Configure an IRQ input pin for use with the external interrupt interface.  Implements @ref external_irq_api_t::open.
 *
 * The Open function is responsible for preparing an external IRQ pin for operation.
 *
 * @retval FSP_SUCCESS                    Open successful.
 * @retval FSP_ERR_ASSERTION              One of the following is invalid:
 *                                          - p_ctrl or p_cfg is NULL
 * @retval FSP_ERR_ALREADY_OPEN           The channel specified has already been opened. No configurations were changed.
 *                                        Call the associated Close function to reconfigure the channel.
 * @retval FSP_ERR_INVALID_CHANNEL The channel requested in p_cfg is not available on the device selected in
 *                                        r_bsp_cfg.h.
 * @retval FSP_ERR_INVALID_ARGUMENT       p_cfg->p_callback is not NULL, but ISR is not enabled. ISR must be enabled to
 *                                        use callback function.
 *
 * @note This function is reentrant for different channels.  It is not reentrant for the same channel.
 **********************************************************************************************************************/
fsp_err_t R_ICU_ExternalIrqOpen (external_irq_ctrl_t * const p_api_ctrl, external_irq_cfg_t const * const p_cfg)
{
    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) p_api_ctrl;

#if ICU_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(ICU_OPEN != p_ctrl->open, FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);

    /* Callback must be used with a valid interrupt priority otherwise it will never be called. */
    FSP_ASSERT(p_cfg->p_callback);
    FSP_ERROR_RETURN(BSP_IRQ_DISABLED != p_cfg->ipl, FSP_ERR_INVALID_ARGUMENT);
#endif

    p_ctrl->channel = (e_icu_channel_t) p_cfg->channel;
    p_ctrl->irq     = p_cfg->irq;

    /* Initialize control block. */
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /**
     * Determine FCLACTL register base address based on channel number
     * FCLACTL channels are grouped into blocks, each block contains up to 8 channels
     */
    if (FCLACTL_0_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(0) + (p_ctrl->channel % 8U) * FCLACTLm_nGAP));
    }
    else if (FCLACTL_1_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(1) + (p_ctrl->channel % 8U) * FCLACTLm_nGAP));
    }
    else if (FCLACTL_2_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(2) + (p_ctrl->channel % 8U) * FCLACTLm_nGAP));
    }
    else if (FCLACTL_3_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(3) + (p_ctrl->channel % 8U) * FCLACTLm_nGAP));
    }
    else if (FCLACTL_4_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(4) + (p_ctrl->channel % 8U) * FCLACTLm_nGAP));
    }

#if (1 == BSP_FEATURE_FCLA5_IS_AVAILABLE)
    else if (FCLACTL_5_MAX_ENTRIES > p_ctrl->channel)
    {
        p_ctrl->p_reg = ((R_FCLA_Type *) (R_FCLA_BASE(5) + ((p_ctrl->channel % 8U) + 1) * FCLACTLm_nGAP));
    }
#endif

    /* Setting detection mode for NMI external pin */
    else if (ICU_CHANNEL_NMI == p_ctrl->channel)
    {
        /* Initialize global pointer to ICU for NMI callback use.  */
        gp_icu_ctrl = (icu_instance_ctrl_t *) p_ctrl;

        gp_icu_ctrl->p_callback = p_cfg->p_callback;
        gp_icu_ctrl->p_context  = p_cfg->p_context;

        /* Set NMI callback function */
        R_BSP_GroupIrqWrite(BSP_GRP_IRQ_EXTERNAL_NMI, r_icu_nmi);

        /* Get the NMI base address */
        p_ctrl->p_reg = ((R_FCLA_Type *) R_FCLA_NMI_BASE);
    }
    else
    {
        /* Do nothing */
    }

    /* Configure Interrupt only if valid vector number is provided */
    if ((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR))
    {
        /* Configure interrupt controller */
        R_BSP_IrqCfg(p_ctrl->irq, p_cfg->ipl, p_ctrl);
    }

    /* Configure trigger detection mode */
    p_ctrl->p_reg->FCLACTL = p_cfg->trigger;

    /* Dummy read */
    p_ctrl->p_reg->FCLACTL;

    /* Synchronization after setting registers */
    SYNCP();

    /* Mark the control block as open */
    p_ctrl->open = ICU_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enable external interrupt for specified channel. Implements @ref external_irq_api_t::enable.
 *
 * @retval FSP_SUCCESS                 Interrupt Enabled successfully.
 * @retval FSP_ERR_ASSERTION           The p_ctrl parameter was null.
 * @retval FSP_ERR_NOT_OPEN            The channel is not opened.
 * @retval FSP_ERR_IRQ_BSP_DISABLED    Requested IRQ is not defined in this system
 **********************************************************************************************************************/
fsp_err_t R_ICU_ExternalIrqEnable (external_irq_ctrl_t * const p_api_ctrl)
{
    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) p_api_ctrl;

#if ICU_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(ICU_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR), FSP_ERR_IRQ_BSP_DISABLED);
#endif

    /* Configure Interrupt only if valid vector number is provided */
    if ((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR))
    {
        /* Clear the interrupt status and Pending bits, before the interrupt is enabled. */
        R_BSP_IrqEnable(p_ctrl->irq);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Disable external interrupt for specified channel. Implements @ref external_irq_api_t::disable.
 *
 * @retval FSP_SUCCESS                 Interrupt disabled successfully.
 * @retval FSP_ERR_ASSERTION           The p_ctrl parameter was null.
 * @retval FSP_ERR_NOT_OPEN            The channel is not opened.
 * @retval FSP_ERR_IRQ_BSP_DISABLED    Requested IRQ is not defined in this system
 **********************************************************************************************************************/
fsp_err_t R_ICU_ExternalIrqDisable (external_irq_ctrl_t * const p_api_ctrl)
{
    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) p_api_ctrl;

#if ICU_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(ICU_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR), FSP_ERR_IRQ_BSP_DISABLED);
#endif

    /* Configure Interrupt only if valid vector number is provided */
    if ((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR))
    {
        /* Disable the interrupt, and then clear the interrupt pending bits and interrupt status. */
        R_BSP_IrqDisable(p_ctrl->irq);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Updates the user callback and has option of providing memory for callback structure.
 * Implements external_irq_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            A required pointer is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_ICU_ExternalIrqCallbackSet (external_irq_ctrl_t * const p_api_ctrl,
                                        void (                    * p_callback)(
                                            external_irq_callback_args_t *),
                                        void * const                         p_context,
                                        external_irq_callback_args_t * const p_callback_memory)
{
    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) p_api_ctrl;

#if ICU_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_callback);
    FSP_ERROR_RETURN(ICU_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Configure callback function and context parameters */
    p_ctrl->p_callback        = p_callback;
    p_ctrl->p_callback_memory = p_callback_memory;
    p_ctrl->p_context         = p_context;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close the external interrupt channel. Implements @ref external_irq_api_t::close.
 *
 * @retval     FSP_SUCCESS          Successfully closed.
 * @retval     FSP_ERR_ASSERTION    The parameter p_ctrl is NULL.
 * @retval     FSP_ERR_NOT_OPEN     The channel is not opened.
 **********************************************************************************************************************/
fsp_err_t R_ICU_ExternalIrqClose (external_irq_ctrl_t * const p_api_ctrl)
{
    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) p_api_ctrl;

#if ICU_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(ICU_OPEN == p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Configure Interrupt only if valid vector number is provided */
    if ((p_ctrl->irq >= 0) && (p_ctrl->irq < FSP_INVALID_VECTOR))
    {
        /* Disable the interrupt, and then clear the interrupt pending bits and interrupt status. */
        R_BSP_IrqDisable(p_ctrl->irq);
        R_FSP_IsrContextSet(p_ctrl->irq, NULL);
    }

    /* Mark the control block as close */
    p_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ICU)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * ICU External Interrupt ISR.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void r_icu_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Clear the IR bit after calling the user callback so that if the condition is cleared the ISR will not
     * be called again. */
    R_BSP_IrqStatusClear(irq);

    icu_instance_ctrl_t * p_ctrl = (icu_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    if ((NULL != p_ctrl) && (NULL != p_ctrl->p_callback))
    {
        /* Set data to identify callback to user, then call user callback. */
        external_irq_callback_args_t args;

        /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
         * stored in non-secure memory so they can be accessed by a non-secure callback function. */
        external_irq_callback_args_t * p_args = p_ctrl->p_callback_memory;

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

        p_args->channel   = p_ctrl->channel;
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

/*******************************************************************************************************************//**
 * ICU External interrupt NMI.
 **********************************************************************************************************************/
static void r_icu_nmi (bsp_grp_irq_t irq)
{
    FSP_PARAMETER_NOT_USED(irq);

    external_irq_callback_args_t args;

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    external_irq_callback_args_t * p_args = gp_icu_ctrl->p_callback_memory;
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

    p_args->channel   = gp_icu_ctrl->channel;
    p_args->p_context = (void *) gp_icu_ctrl->p_context;

    if (NULL != gp_icu_ctrl->p_callback)
    {
        gp_icu_ctrl->p_callback(p_args);
    }

    if (NULL != gp_icu_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *gp_icu_ctrl->p_callback_memory = args;
    }
}

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_irq.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define BSP_EEIC_EITB_SET            ((uint16_t) (BSP_INTERRUPT_TABLE_REFERENCE_METHOD)) ///< Value used to set EITB bit in EEIC
#define BSP_PRV_INTERRUPTABLE_NUM    (15U)                                               ///< Number of interrupt table entries supported
#define BSP_EIINT_MASK               (0x07FFU)                                           ///< Mask to extract EIINT source index
#define BSP_FEINC_PE_OFFSET          (0x100U)                                            ///< Offset applied for FEINC mapping per PE
#define BSP_FEINT_PE_SRC_OFFSET      (2U)                                                ///< Offset shift for FEINT source per PE
#define BSP_FEINT_NON_PE_SRC         (1U)                                                ///< FEINT source index for non‑PE sources

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/** This array holds callback functions. */
bsp_grp_irq_cb_t g_bsp_group_isr_sources[BSP_GRP_IRQ_MAX_INDEX];

/* This table is used to store the context in the ISR. */
void * gp_renesas_isr_context[BSP_INT_VECTOR_MAX_ENTRIES];

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
static void bsp_group_isr_call(bsp_grp_irq_t irq);

static void bsp_feint_enable(void);

/*******************************************************************************************************************//**
 * @internal
 * @addtogroup FENMI function
 * @ingroup RENESAS_INTERNAL
 * @{
 **********************************************************************************************************************/
#if defined(__CCRH__)
 #pragma section FENMI
#elif defined(__ghs__)
 #pragma ghs interrupt (FE)
#endif
void FENMI (void)
{
    uint32_t            channel;
    volatile uint32_t * p_fenmif;

    p_fenmif = (uint32_t *) (&R_FENC->FENMIF);
    channel  = SCH1R(*p_fenmif) - 1;   // Define macro for this Intrinsic functions in bsp_compiler_support.h

    /* call the irq callback */
    bsp_group_isr_call((bsp_grp_irq_t) channel);
}

/*******************************************************************************************************************//**
 * @internal
 * @addtogroup FEINT function
 * @ingroup RENESAS_INTERNAL
 * @{
 **********************************************************************************************************************/
#if defined(__CCRH__)
 #pragma section FEINT
#elif defined(__ghs__)
 #pragma ghs interrupt (FE)
#endif
void FEINT (void)
{
    uint8_t             coreID;
    uint32_t            channel;
    volatile uint32_t * pfeintf;

    coreID  = (uint8_t) STSR_REGSEL(0, 2);
    pfeintf = (uint32_t *) &((R_FEINC_Type *) (R_FEINC_BASE + (coreID * BSP_FEINC_PE_OFFSET)))->FEINTF;

    channel = SCH1R(*pfeintf);         // Define macro for this Intrinsic functions in bsp_compiler_support.h

    if (BSP_FEINT_NON_PE_SRC != channel)
    {
        channel += coreID * BSP_FEINT_PE_SRC_OFFSET;
    }

    /* call the irq callback */
    bsp_group_isr_call((bsp_grp_irq_t) channel);
}

/*******************************************************************************************************************//**
 * @internal
 * @addtogroup EI TRAP0 function
 * @ingroup RENESAS_INTERNAL
 * @{
 **********************************************************************************************************************/
#if (0 == BSP_CFG_RTOS)                /* NO_RTOS */
void vTRAP0_Handler (void)
{
    while (1)
    {
        ;
    }
}

#endif

/*******************************************************************************************************************//**
 * Calls the callback function for an interrupt if a callback has been registered.
 *
 * @param[in]   irq         Which interrupt to check and possibly call.
 *
 * @retval FSP_SUCCESS              Callback was called.
 * @retval FSP_ERR_INVALID_ARGUMENT No valid callback has been registered for this interrupt source.
 *
 * @warning This function is called from within an interrupt
 **********************************************************************************************************************/
static void bsp_group_isr_call (bsp_grp_irq_t irq)
{
    /** Check for valid callback */
    if (NULL != g_bsp_group_isr_sources[irq])
    {
        /** Callback has been found. Call it. */
        g_bsp_group_isr_sources[irq](irq);
    }
    else
    {
        /* Hang MCU */
        while (1)
        {
            ;
        }
    }
}

/*******************************************************************************************************************//**
 * @addtogroup BSP_COMMON
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Register a callback function for supported interrupts. If NULL is passed for the callback argument then any
 * previously registered callbacks are unregistered.
 *
 * @param[in]  irq          Interrupt for which  to register a callback.
 * @param[in]  p_callback   Pointer to function to call when interrupt occurs.
 *
 * @retval     FSP_SUCCESS                  Callback registered
 * @retval     FSP_ERR_ASSERTION            Callback pointer is NULL
 **********************************************************************************************************************/
fsp_err_t R_BSP_GroupIrqWrite (bsp_grp_irq_t irq, void (* p_callback)(bsp_grp_irq_t irq))
{
#if BSP_CFG_PARAM_CHECKING_ENABLE

    /* Check pointer for NULL value. */
    FSP_ASSERT(p_callback);
#endif

    /* Register callback. */
    g_bsp_group_isr_sources[irq] = p_callback;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Using the vector table information section that has been built by the linker and placed into ROM in the.
 * vector_info. section, this function will initialize the INTC1, INTC2 and FEINT, so that configured interrupts
 * will trigger interrupts in the INTC1, INTC2 and FEINT.
 **********************************************************************************************************************/
void bsp_irq_cfg (void)
{
    R_INTC1_PE0_Type * R_INTC1_PE;
    uint16_t           irq;
    uint8_t            coreID = R_BSP_GetCoreID();

    R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE + coreID * BSP_INT_INTC1_OFFSET);
    for (irq = 0; irq < BSP_INTC1_VECTOR_MAX_ENTRIES; irq++)
    {
        /* INTC1 */
        R_INTC1_PE->EEIC_b[irq].EITB |= (uint16_t) (BSP_EEIC_EITB_SET);

        /* Dummy read and sync */
        BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC1_PE->EEIC[irq]);
    }

#if (BSP_PE0_USED)

    /* To avoid unexpected settings on other cores, INTC2 is initialized only once on CPU0 during startup */
    if (!coreID)
    {
        for (irq = BSP_INTC1_VECTOR_MAX_ENTRIES; irq < BSP_INT_VECTOR_MAX_ENTRIES; irq++)
        {
            /* INTC2 */
            if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
            {
                R_INTC2->EEIC_b[irq].EITB |= (uint16_t) (BSP_EEIC_EITB_SET);

                /* Dummy read and sync */
                BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EEIC[irq]);
            }
            else
            {
                /* Nothing to do */
            }
        }

 #if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)

        /* Initialize OR-ed interrupt configuration if supported by the device */
        R_BSP_ORed_IrqInit();
 #endif                                /* #if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ) */
    }
    else
    {
        /* Nothing to do */
    }
#endif

    /*  Enable FEINT interrupts  */
    bsp_feint_enable();

    /* Enable interrupt */
    EI();
}

/*******************************************************************************************************************//**
 * Unmask FEINT for the current CPU core if enabled.
 **********************************************************************************************************************/
static void bsp_feint_enable (void)
{
    uint8_t        coreID;
    uint8_t        feint_enable;
    R_FEINC_Type * R_FEINC_PE;

    coreID = (uint8_t) STSR_REGSEL(0, 2);

    /* Check if FEINT enabled for this core */
    feint_enable = (BSP_FE_LEVEL_MASKABLE_INTERRUPT & (1U << coreID));

    if (0U != feint_enable)
    {
        /* Get FEINTMSK register address for this core */
        R_FEINC_PE = (R_FEINC_Type *) (R_FEINC_BASE + coreID * BSP_FEINC_PE_OFFSET);

        /* Unmask FEINT */
        R_FEINC_PE->FEINTMSK = 0U;
    }
}

/*******************************************************************************************************************//**
 * Dummy ISR.
 **********************************************************************************************************************/
void DummyIsr (void)
{
    while (1)
    {
        ;
    }
}

/*******************************************************************************************************************//**
 * @brief Default interrupt handler for direct vector method.
 * @details Dispatches to actual handler via INTBP vector table based on EIIC channel.
 **********************************************************************************************************************/
void INTP_Default_Handler (void)
{
    /* Read interrupt channel number (EI Level exception cause)*/
    uint16_t irq_number = (uint16_t) ((STSR_REGSEL(13, 0) & BSP_EIINT_MASK));

    /* Read base address of the interrupt handler “address” table */
    uint32_t intbp = STSR_REGSEL(4, 1);

    /* Fetch handler address from vector table */
    uint32_t handler_addr = *(volatile uint32_t *) (intbp + (irq_number << 2));

    /* Call the actual interrupt handler */
    ((void (*)(void))handler_addr)();
}

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_COMMON)
 **********************************************************************************************************************/

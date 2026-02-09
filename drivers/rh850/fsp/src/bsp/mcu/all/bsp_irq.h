/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_IRQ_H
#define BSP_IRQ_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "fsp_common_api.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Interrupt Vector Setting */
#define BSP_EITB                       ((uint16_t) 0x0040)                                           ///< Bit mask for EITB (Enable Interrupt Table Branch)
#define BSP_EIMK                       ((uint16_t) 0x0080)                                           ///< Bit mask for EIMK (Interrupt Mask)
#define BSP_EICT                       ((uint16_t) 0x8000)                                           ///< Bit mask for EICT (Interrupt Control)
#define BSP_EIIC_MSK                   ((uint32_t) 0x00000FFF)                                       ///< Mask to extract EIIC index bits (lower 12 bits)
#define BSP_ENABLE_EIIC                ((uint8_t) 0)                                                 ///< Enable value for EIIC
#define BSP_DISABLE_EIIC               ((uint8_t) 1)                                                 ///< Disable value for EIIC
#define BSP_INT_INTC1_OFFSET           ((uint32_t) 0x4000U)                                          ///< Offset applied for INTC1 interrupt vector base
#define BSP_INTC_VECTOR_MAX_ENTRIES    (BSP_INTC1_VECTOR_MAX_ENTRIES + BSP_INTC2_VECTOR_MAX_ENTRIES) ///< Total number of INTC1 + INTC2 vector entries

#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)

/** Total interrupt vector entries including OR-ed IRQ and VIRQ */
 #define BSP_INT_VECTOR_MAX_ENTRIES    (BSP_INTC1_VECTOR_MAX_ENTRIES + BSP_INTC2_VECTOR_MAX_ENTRIES + \
                                        BSP_FEINT_VECTOR_MAX_ENTRIES + BSP_FENMI_VECTOR_MAX_ENTRIES + \
                                        BSP_FEATURE_VIRQ_MAX_ENTRIES)
#else

/** Total interrupt vector entries excluding VIRQ */
 #define BSP_INT_VECTOR_MAX_ENTRIES    (BSP_INTC1_VECTOR_MAX_ENTRIES + BSP_INTC2_VECTOR_MAX_ENTRIES + \
                                        BSP_FEINT_VECTOR_MAX_ENTRIES + BSP_FENMI_VECTOR_MAX_ENTRIES)
#endif

/**
 * @brief   Execute dummy read and syncp for EIC register
 * @details Dummy read on the register and SYNCP
 * @param [in]    reg   Pointer to register address
 */
#define BSP_INT_IOREG_DUMMY_READ_SYNCP(reg) \
    do {                                    \
        volatile uint32_t dummy = (reg);    \
        SYNCP();                            \
        (void) dummy;                       \
    } while (0)

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern void * gp_renesas_isr_context[BSP_INT_VECTOR_MAX_ENTRIES];

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU
 * @defgroup BSP_COMMON BSP Common
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Finds the ISR context associated with the requested IRQ.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @return  ISR context for IRQ.
 **********************************************************************************************************************/
__STATIC_INLINE void * R_FSP_IsrContextGet (IRQn_Type const irq)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    return gp_renesas_isr_context[irq];
}

/*******************************************************************************************************************//**
 * @brief      Sets the ISR context associated with the requested IRQ.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @param[in]  p_context      ISR context for IRQ.
 **********************************************************************************************************************/
__STATIC_INLINE void R_FSP_IsrContextSet (IRQn_Type const irq, void * p_context)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    gp_renesas_isr_context[irq] = p_context;
}

/*******************************************************************************************************************//**
 * Clears pending interrupts in INTC1, INTC2 and FEINT, then enables the interrupt.
 *
 * @param[in] irq            Interrupt for which to clear the IR bit and enable in the INTC1, INTC2 and FEINT.
 *                           Note that the enums listed
 *                           for IRQn_Type are only those for the G3KH Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqEnable (IRQn_Type const irq)
{
    uint8_t            coreID;
    R_INTC1_PE0_Type * R_INTC1_PE;

    /* Get the current coreID */
    coreID = R_BSP_GetCoreID();

    /* INTC1 */
    if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
    {
        R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE + coreID * BSP_INT_INTC1_OFFSET);
        R_INTC1_PE->EEIC_b[irq].EIMK &= BSP_ENABLE_EIIC;

        /* Dummy read and sync */
        BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC1_PE->EEIC[irq]);
    }

    /* INTC2 */
    else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
    {
        R_INTC2->EEIC_b[irq].EIMK &= BSP_ENABLE_EIIC;
        R_INTC2->EIBD_b[irq].PEID  = (uint8_t) (coreID & R_INTC2_EIBD_PEID_Msk);

        /* Dummy read and sync */
        BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EIBD[irq]);
    }

    /* FEINT */
    else if (irq < BSP_INT_VECTOR_MAX_ENTRIES)
    {
        R_FEINC->FEINTMSK &= (uint32_t) (~(1 << (irq - BSP_INTC_VECTOR_MAX_ENTRIES)));
    }
    else
    {
        /* Nothing to do */
    }
}

/*******************************************************************************************************************//**
 * Disables interrupts in the INTC1, INTC2 and FEINT.
 *
 * @param[in] irq            The IRQ to disable in the INTC1, INTC2 and FEINT. Note that the enums listed for IRQn_Type
 *                           are only those for the G3KH Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqDisable (IRQn_Type const irq)
{
    uint8_t            coreID;
    R_INTC1_PE0_Type * R_INTC1_PE;

    /* INTC1 */
    if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
    {
        /* Get the current coreID */
        coreID     = R_BSP_GetCoreID();
        R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE + coreID * BSP_INT_INTC1_OFFSET);
        R_INTC1_PE->EEIC_b[irq].EIMK = BSP_DISABLE_EIIC;

        /* Dummy read and sync */
        BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC1_PE->EEIC[irq]);
    }

    /* INTC2 */
    else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
    {
        R_INTC2->EEIC_b[irq].EIMK = BSP_DISABLE_EIIC;

        /* Dummy read and sync*/
        BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EEIC[irq]);
    }

    /* FEINT */
    else if (irq < BSP_INT_VECTOR_MAX_ENTRIES)
    {
        R_FEINC->FEINTMSK |= (uint32_t) (1 << (irq - BSP_INTC_VECTOR_MAX_ENTRIES));
    }
    else
    {
        /* Nothing to do */
    }
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context.
 *
 * @param[in] irq            The IRQ number to configure.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqCfg (IRQn_Type const irq, uint32_t priority, void * p_context)
{
    uint8_t            coreID;
    R_INTC1_PE0_Type * R_INTC1_PE;

    if (irq < BSP_INT_VECTOR_MAX_ENTRIES)
    {
        /* INTC1 */
        if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
        {
            /* Get the current coreID */
            coreID     = R_BSP_GetCoreID();
            R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE + coreID * BSP_INT_INTC1_OFFSET);
            R_INTC1_PE->EEIC_b[irq].EIP = (uint16_t) (priority & R_INTC1_PE0_EEIC_EIP_Msk);

            /* Dummy read and sync */
            BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC1_PE->EEIC[irq]);
        }

        /* INTC2 */
        else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
        {
            R_INTC2->EEIC_b[irq].EIP = (uint16_t) (priority & R_INTC2_EEIC_EIP_Msk);

            /* Dummy read and syncp */
            BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EEIC[irq]);
        }

        /* FEINT */
        else
        {
            /* Nothing to do */
        }

        /* Store the context. The context is recovered in the ISR. */
        R_FSP_IsrContextSet(irq, p_context);
    }
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context, clears pending interrupts, then enables the interrupt.
 *
 * @param[in] irq            Interrupt number.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqCfgEnable (IRQn_Type const irq, uint16_t priority, void * p_context)
{
    R_BSP_IrqCfg(irq, priority, p_context);

    if (BSP_IRQ_DISABLED == priority)
    {
        R_BSP_IrqDisable(irq);
    }
    else
    {
        R_BSP_IrqEnable(irq);
    }
}

/*******************************************************************************************************************//**
 * Return active interrupt vector number value
 *
 * @return     Active interrupt vector number value
 **********************************************************************************************************************/
__STATIC_INLINE IRQn_Type R_FSP_CurrentIrqGet (void)
{
    /* Initial interrupt id */
    uint32_t   intid;
    uint32_t * pfeintf;
    pfeintf = (uint32_t *) (&R_FEINC->FEINTF);

    if (*pfeintf != 0)                 /* FE interrupt */
    {
        intid  = SCH1R(*pfeintf);
        intid += BSP_INTC_VECTOR_MAX_ENTRIES;
    }
    else                               /* EI interrupt */
    {
        intid = STSR(EIIC);
        intid = intid & BSP_EIIC_MSK;
    }

    /* Return the current interrupt number. */
    return (IRQn_Type) intid;
}

/*******************************************************************************************************************//**
 * Clear the interrupt status flag for a given interrupt. There is no processing to be performed by this function in
 * this BSP.
 *
 * @param[in] irq            Interrupt for which to clear the status flag. Note that the enums listed for IRQn_Type are
 *                           only those for the EIC Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_IrqStatusClear (IRQn_Type irq)
{
    FSP_PARAMETER_NOT_USED(irq);

    /* Do nothing */
}

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_COMMON)
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
void                               bsp_irq_cfg(void); // Used internally by BSP
void                               bsp_common_interrupt_handler(uint32_t id);
BSP_FENMI_INTERRUPT_ATTRIBUTE void FENMI(void);
BSP_FEINT_INTERRUPT_ATTRIBUTE void FEINT(void);
BSP_INTERRUPT_ATTRIBUTE void       DummyIsr(void);
void                               vTRAP0_Handler(void);
void                               INTP_Default_Handler(void);
fsp_err_t                          R_BSP_GroupIrqWrite(bsp_grp_irq_t irq, void (* p_callback)(bsp_grp_irq_t irq));

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* BSP_IRQ_H */

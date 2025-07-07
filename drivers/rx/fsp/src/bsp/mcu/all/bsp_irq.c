/* ${REA_DISCLAIMER_PLACEHOLDER} */

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/** ELC event definitions. */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/
uint32_t g_current_isr_number;

/* This table is used to store the context in the ISR. */
void * gp_renesas_isr_context[BSP_ICU_VECTOR_MAX_ENTRIES];

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/
#if defined(__GNUC__) || defined(__ICCRX__)
static void break_event_hook(void);

#else

void break_event_hook(void);

#endif

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 *
 * @{
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU) */

/*******************************************************************************************************************//**
 *        Using the vector table information section that has been built by the linker and placed into ROM in the
 * .vector_info. section, this function will initialize the ICU so that configured ELC events will trigger interrupts
 * in the NVIC.
 *
 **********************************************************************************************************************/
void bsp_irq_cfg (void)
{
    /* Enable selectable interrupts */
    bsp_selectable_irq_cfg();

    /* Enable all group interrupts */
    bsp_group_irq_cfg();

    g_current_isr_number = BSP_IRQ_DISABLED;

    /* Enable interrupts */
    __SETPSW_I();
}

/* This function is called at Brk instruction*/
#if defined(__GNUC__) || defined(__ICCRX__)
BSP_ATTRIBUTE_STACKLESS void brk_isr (void)
{
    /* Push All register. */
    __asm volatile ("pushm  r1-r15\n");

    /* jump to break event hook. */
    __asm volatile ("jsr	%0\n"::"r" (break_event_hook));

    /* Pop All register. */
    __asm volatile ("popm   r1-r15\n");

    /* return. */
    __asm volatile ("rte\n");
}

#endif

/* This function calls user defined function when unconditional trap is generated. */
#if defined(__GNUC__) || defined(__ICCRX__)
static void break_event_hook (void)
#else

void break_event_hook (void)
#endif
{
    /** When the BRK instruction is executed, an unconditional trap is generated.
     */
    BSP_CFG_HANDLE_BREAK_EVENT_HOOK(0);
}

/*******************************************************************************************************************//**
 * Default interrupt handler.
 **********************************************************************************************************************/
void Reserved_Isr (void)
{
    while (1)
    {
        ;
    }
}

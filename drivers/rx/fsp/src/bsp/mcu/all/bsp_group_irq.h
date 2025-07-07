/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef BSP_GROUP_IRQ_H
#define BSP_GROUP_IRQ_H

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const icu_event_t g_group_irq_table[];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/* Public functions defined in bsp.h */
void     R_BSP_GroupIrqContextSet(uint8_t irq, uint8_t factor, void * p_context);
void     R_BSP_GroupIrqStatusClear(uint8_t irq, uint8_t factor);
void     R_BSP_GroupIrqEnableNoClear(uint8_t irq, uint8_t factor);
void     R_BSP_GroupIrqDisable(uint8_t irq, uint8_t factor);
uint32_t R_BSP_GroupIrqStatusGet(uint8_t irq);
uint32_t R_BSP_GroupIrqCfgGet(uint8_t irq);
uint32_t R_BSP_GroupIrqIsEnabled(uint8_t irq, uint8_t factor);
void   * R_BSP_GroupIrqContextGet(uint8_t irq, uint8_t factor);

void bsp_group_irq_cfg(void);
void group_irq_isr(void);

__STATIC_INLINE bool R_BSP_GroupIrqCheck (uint8_t irq) {
    bool result = false;

    for (uint8_t group = 0; group < BSP_FEATURE_GROUP_IRQ_NUM_INTERRUPTS; group++)
    {
        /* Check if the interrupt is a group interrupt. */
        if (irq == g_group_irq_table[group])
        {
            result = true;
            break;
        }
    }

    return result;
}

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/*******************************************************************************************************************//**
 * @addtogroup BSP_STANDBY_CRUN
 *
 * @{
 **********************************************************************************************************************/

#if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED)
 #include "bsp_mcu_standby_cyclicrun.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Interrupt
 **********************************************************************************************************************/
 #define BSP_EIC_EITB_SET                     ((uint16_t) (1))

/***********************************************************************************************************************
 * IO Port
 **********************************************************************************************************************/

/** Shift to get port in bsp_io_port_t and bsp_io_port_pin_t enums. */
 #define BSP_IO_PORT_GROUP_POS                (8UL)          ///< Bit position of port group field within packed IO value
 #define BSP_IO_PORT_TYPE_POS                 (16UL)         ///< Bit position of port type field within packed IO value
 #define BSP_IO_PORT_PWE_POS                  (24UL)         ///< Bit position of PWE (Port Write Enable) field within packed IO value

 #define BSP_IO_PORT_PWE_MASK                 (0xFF000000UL) ///< Mask for PWE (bits 31..24)
 #define BSP_IO_PORT_TYPE_MASK                (0x00FF0000UL) ///< Mask for port type (bits 23..16)
 #define BSP_IO_PORT_GROUP_MASK               (0x0000FF00UL) ///< Mask for port group/index (bits 15..8)
 #define BSP_IO_PORT_PIN_MASK                 (0x000000FFUL) ///< Mask for pin number within the group (bits 7..0)

 #define BSP_IO_SR_MASK_POS                   (16UL)         ///< Bit position used when forming SR (set/reset) masks
 #define BSP_IO_PORT_PWE_UNSED                (0xFFUL)       ///< PWE value when not used / all-subfields set (as provided)

 #define BSP_IO_PORT_KEYCODE_ENABLE           0xA5A5A501U    ///< Keycode required to enable protected port write access
 #define BSP_IO_PORT_KEYCODE_DISABLE          0xA5A5A500U    ///< Keycode required to disable protected port write access

 #define R_BSP_PORT                           R_PORT         ///< Alias to the hardware PORT instance for BSP usage

/***********************************************************************************************************************
 * Software Delay
 **********************************************************************************************************************/
 #define BSP_CYCLICRUN_DELAY_NS_PER_US        (1000)         ///< Number of nanoseconds in one microsecond
 #define BSP_CYCLICRUN_DELAY_NS_PER_SECOND    (1000000000)   ///< Number of nanoseconds in one second

/** The number of cycles required per software delay loop in cyclic mode. */
 #define BSP_CYCLICRUN_DELAY_LOOP_CYCLES      (55)

/* Calculates the number of delay loops to pass to bsp_prv_software_delay_loop to achieve at least the requested cycle
 * count delay. This is 1 loop longer than optimal if cycles is a multiple of BSP_CYCLICRUN_DELAY_LOOP_CYCLES, but it ensures
 * the requested number of loops is at least 1 since bsp_prv_software_delay_loop cannot be called with a loop count
 * of 0. */
 #define BSP_CYCLICRUN_DELAY_LOOPS_CALCULATE(cycles)    (((cycles) / BSP_CYCLICRUN_DELAY_LOOP_CYCLES) + 1U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

/* Software Delay */
void        bsp_cyclicrun_prv_software_delay_loop(uint32_t loop_cnt);
static void bsp_cyclicrun_software_delay_loop(uint64_t loop_cnt);

/* Standby Controller */
static void bsp_cyclicrun_group_isr_call(bsp_grp_irq_t irq);
static void bsp_cyclicrun_halt(void);
static void bsp_cyclicrun_stop_mode(void);
static void bsp_cyclicrun_deep_stop_mode(void);
static void bsp_cyclicrun_stbc_set_channel(bsp_module_standby_t module, uint32_t channel, bool enable);
static void bsp_cyclicrun_stbc_set_channels(bsp_module_standby_t module, bool enable);
static void bsp_cyclicrun_stbc_write_with_protection(volatile uint32_t * p_msr_address, uint32_t value);

extern uint32_t r_bsp_run_mode_wakeup_reg[];
extern uint32_t r_bsp_cyclicrun_mode_wakeup_reg[];

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/* DATA SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_CRUN_CODE_DATA)

R_PORT_Type * R_BSP_CyclicRun_PORT_TYPE[] =
{
    R_BSP_PORT,                        // PORT
    R_BSP_JPORT,                       // JPORT
    R_BSP_APORT                        // APORT
};

/** Array contains all functions' pointer used in CYCLIC Mode */
void (* r_bsp_cyclicrun_mcu_standby_mode_funcs[BSP_MCU_STANDBY_MODE_INDEX_END])(void) =
{
    [BSP_MCU_STANDBY_MODE_HALT]       = bsp_cyclicrun_halt,
    [BSP_MCU_STANDBY_MODE_CYCLICSTOP] = bsp_cyclicrun_stop_mode,
    [BSP_MCU_STANDBY_MODE_DEEPSTOP]   = bsp_cyclicrun_deep_stop_mode
};

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_DATA, BSP_PRAGMA_SECTION_DEFAULT)

/* BSS SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_BSS, BSP_PRAGMA_SECTION_CRUN_CODE_BSS)

/* Interrupt */
/** This array holds callback functions. Used in Cyclic RUN Mode */
bsp_grp_irq_cb_t g_bsp_cyclicrun_group_isr_sources[BSP_GRP_IRQ_MAX_INDEX];

/* This table is used to store the context in the ISR. Used in Cyclic RUN Mode */
void * gp_renesas_cyclicrun_isr_context[BSP_INT_VECTOR_MAX_ENTRIES];

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_CRUN_BSS, BSP_PRAGMA_SECTION_DEFAULT)

/* TEXT SECTION */
BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_CRUN_CODE_TEXT)

/***********************************************************************************************************************
 * Software Delay
 **********************************************************************************************************************/
void R_BSP_CyclicRunSoftwareDelay (uint32_t delay, bsp_cyclicrun_delay_units_t units)
{
    /* Get the system clock frequency in Hz. */
    uint32_t cpu_hz = R_FSP_CyclicRunSystemClockHzGet(FSP_PRIV_CLOCK_CYCLICRUN_CLK_CPU);

    /* Get the # of nanoseconds/cycle. */
    uint32_t ns_per_cycle = BSP_CYCLICRUN_DELAY_NS_PER_SECOND / cpu_hz;
    int32_t  total_ns     = 0;

    /* Check if the result of multiplication will overflow*/
    if (delay > (UINT32_MAX / (units * BSP_CYCLICRUN_DELAY_NS_PER_US)))
    {
        total_ns = (int32_t) UINT32_MAX;
    }
    else
    {
        /* Convert the requested time to nanoseconds. */
        total_ns = (uint32_t) delay * units * BSP_CYCLICRUN_DELAY_NS_PER_US;
    }

    /* Calculate the number of cycles and loops required.*/
    uint32_t cycles_requested = total_ns / ns_per_cycle;
    uint32_t loops_required   = cycles_requested / BSP_CYCLICRUN_DELAY_LOOP_CYCLES;

    /* Only delay if the supplied parameters constitute a delay. */
    if (loops_required > (uint32_t) 0)
    {
        bsp_cyclicrun_software_delay_loop(loops_required);
    }
}

/***********************************************************************************************************************
 * Interrupt
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Finds the ISR context associated with the requested IRQ. Can only used in Cyclic RUN Mode.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @return  ISR context for IRQ.
 **********************************************************************************************************************/
void * R_FSP_CyclicRunIsrContextGet (IRQn_Type const irq)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    return gp_renesas_cyclicrun_isr_context[irq];
}

/*******************************************************************************************************************//**
 * @brief      Sets the ISR context associated with the requested IRQ. Can only used in Cyclic RUN Mode.
 *
 * @param[in]  irq            IRQ number (parameter checking must ensure the IRQ number is valid before calling this
 *                            function.
 * @param[in]  p_context      ISR context for IRQ.
 **********************************************************************************************************************/
void R_FSP_CyclicRunIsrContextSet (IRQn_Type const irq, void * p_context)
{
    /* This provides access to the ISR context array defined in bsp_irq.c. This is an inline function instead of
     * being part of bsp_irq.c for performance considerations because it is used in interrupt service routines. */
    gp_renesas_cyclicrun_isr_context[irq] = p_context;
}

/*******************************************************************************************************************//**
 * Clears pending interrupts in INTC1, INTC2 and FEINT, then enables the interrupt. Can only used in Cyclic RUN Mode.
 *
 * @param[in] irq            Interrupt for which to clear the IR bit and enable in the INTC1, INTC2 and FEINT.
 *                           Note that the enums listed
 *                           for IRQn_Type are only those for the G3KH Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void R_BSP_CyclicRunIrqEnable (IRQn_Type const irq)
{
    R_INTC1_PE0_Type * R_INTC1_PE;

    /* INTC1 */
    if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
    {
        R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE);
        R_INTC1_PE->EIC_b[irq].EIMK &= BSP_ENABLE_EIIC;
    }

    /* INTC2 */
    else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
    {
        R_INTC2->EIC_b[irq].EIMK &= BSP_ENABLE_EIIC;
        R_INTC2->EIBD_b[irq].PEID = 0U;
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
 * Disables interrupts in the INTC1, INTC2 and FEINT. Can only used in Cyclic RUN Mode.
 *
 * @param[in] irq            The IRQ to disable in the INTC1, INTC2 and FEINT. Note that the enums listed for IRQn_Type
 *                           are only those for the G3KH Processor Exceptions Numbers.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void R_BSP_CyclicRunIrqDisable (IRQn_Type const irq)
{
    R_INTC1_PE0_Type * R_INTC1_PE;

    /* INTC1 */
    if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
    {
        R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE);
        R_INTC1_PE->EIC_b[irq].EIMK = BSP_DISABLE_EIIC;
    }

    /* INTC2 */
    else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
    {
        R_INTC2->EIC_b[irq].EIMK = BSP_DISABLE_EIIC;
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
 * Sets the interrupt priority and context. Can only used in Cyclic RUN Mode.
 *
 * @param[in] irq            The IRQ number to configure.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void R_BSP_CyclicRunIrqCfg (IRQn_Type const irq, uint32_t priority, void * p_context)
{
    R_INTC1_PE0_Type * R_INTC1_PE;

    if (irq < BSP_INT_VECTOR_MAX_ENTRIES)
    {
        /* INTC1 */
        if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
        {
            R_INTC1_PE                 = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE);
            R_INTC1_PE->EIC_b[irq].EIP = (uint16_t) (priority);
        }

        /* INTC2 */
        else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
        {
            R_INTC2->EIC_b[irq].EIP = (uint16_t) (priority);
        }

        /* FEINT */
        else
        {
            /* Nothing to do */
        }

        /* Store the context. The context is recovered in the ISR. */
        R_FSP_CyclicRunIsrContextSet(irq, p_context);
    }
}

/*******************************************************************************************************************//**
 * Sets the interrupt priority and context, clears pending interrupts, then enables the interrupt.
 * Can only used in Cyclic RUN Mode.
 *
 * @param[in] irq            Interrupt number.
 * @param[in] priority       GIC priority of the interrupt
 * @param[in] p_context      The interrupt context is a pointer to data required in the ISR.
 *
 * @warning Do not call this function for system exceptions where the IRQn_Type value is < 0.
 **********************************************************************************************************************/
void R_BSP_CyclicRunIrqCfgEnable (IRQn_Type const irq, uint16_t priority, void * p_context)
{
    R_BSP_CyclicRunIrqCfg(irq, priority, p_context);

    if (BSP_IRQ_DISABLED == priority)
    {
        R_BSP_CyclicRunIrqDisable(irq);
    }
    else
    {
        R_BSP_CyclicRunIrqEnable(irq);
    }
}

/*******************************************************************************************************************//**
 * Return active interrupt vector number value. Can only used in Cyclic RUN Mode.
 *
 * @return     Active interrupt vector number value
 **********************************************************************************************************************/
IRQn_Type R_FSP_CyclicRunCurrentIrqGet (void)
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
 * FENMI function. Used in Cyclic RUN Mode
 **********************************************************************************************************************/
void FENMI_CyclicRun (void)
{
    uint32_t   channel;
    uint32_t * p_fenmif;

    /* Get address of FENMIF register */
    p_fenmif = (uint32_t *) (&R_FENC->FENMIF);

    /* Extract channel field via SCH1R */
    channel = SCH1R(*p_fenmif) - 1;

    /* Call the irq callback */
    bsp_cyclicrun_group_isr_call((bsp_grp_irq_t) channel);
}

/*******************************************************************************************************************//**
 * FEINT function. Used in Cyclic RUN Mode
 **********************************************************************************************************************/
void FEINT_CyclicRun (void)
{
    uint32_t   channel;
    uint32_t * pfeintf;

    /* Get address of FEINT register */
    pfeintf = (uint32_t *) (&R_FEINC->FEINTF);

    /* Extract channel field via SCH1R */
    channel  = SCH1R(*pfeintf);
    channel += BSP_FENMI_VECTOR_MAX_ENTRIES;

    /* call the irq callback */
    bsp_cyclicrun_group_isr_call((bsp_grp_irq_t) channel);
}

/*******************************************************************************************************************//**
 * Register a callback function for supported interrupts. If NULL is passed for the callback argument then any
 * previously registered callbacks are unregistered.
 * Used in Cyclic RUN Mode
 *
 * @param[in]  irq          Interrupt for which  to register a callback.
 * @param[in]  p_callback   Pointer to function to call when interrupt occurs.
 *
 * @retval     FSP_SUCCESS                  Callback registered
 * @retval     FSP_ERR_ASSERTION            Callback pointer is NULL
 **********************************************************************************************************************/
fsp_err_t R_BSP_CyclicRunGroupIrqWrite (bsp_grp_irq_t irq, void (* p_callback)(bsp_grp_irq_t irq))
{
 #if BSP_CFG_PARAM_CHECKING_ENABLE

    /* Check pointer for NULL value. */
    FSP_ASSERT(p_callback);
 #endif

    /* Register callback. */
    g_bsp_cyclicrun_group_isr_sources[irq] = p_callback;

    return FSP_SUCCESS;
}

/***********************************************************************************************************************
 * Clock Controller
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initializes system clocks.  Makes no assumptions about current register settings.
 * This function can only be used in Cyclic RUN mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunInitClock (void)
{
    /* Depend on the operation of each system */
    bsp_cyclicrun_clock_driver_init();
}

/*******************************************************************************************************************//**
 * Get a clock frequency. This function can only be used in Cyclic RUN Mode.
 *
 * @param[in]   clock                 Element number of the array that defines the frequency of each clock.
 *
 * @retval      g_clock_cyclicrun_freq[clock]   Clock frequency.
 *
 * @return     Frequency of requested clock in Hertz.
 **********************************************************************************************************************/
uint32_t R_FSP_CyclicRunSystemClockHzGet (fsp_priv_clock_cyclicrun_t clock)
{
    return g_clock_cyclicrun_freq[clock];
}

/***********************************************************************************************************************
 * IO Port
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Enable access to the selected port registers. Unlock PWE register protect by using R_BSP_PinAccessEnable
 * before calling this function.
 **********************************************************************************************************************/
void R_BSP_CyclicRunPortWriteEnable (bsp_io_port_t port)
{
    uint32_t pwe_pos = (port & BSP_IO_PORT_PWE_MASK) >> BSP_IO_PORT_PWE_POS;

    if (BSP_IO_PORT_PWE_UNSED != pwe_pos)
    {
        R_BSP_PORT->PWE = R_BSP_PORT->PWE | (1 << pwe_pos);
    }
}

/*******************************************************************************************************************//**
 * Disable writing to the PWE registers. Unlock PWE register protect by using R_BSP_PinAccessEnable
 * before calling this function.
 **********************************************************************************************************************/
void R_BSP_CyclicRunPortWriteDisable (bsp_io_port_t port)
{
    uint32_t pwe_pos = (port & BSP_IO_PORT_PWE_MASK) >> BSP_IO_PORT_PWE_POS;

    if (BSP_IO_PORT_PWE_UNSED != pwe_pos)
    {
        R_BSP_PORT->PWE = R_BSP_PORT->PWE & (uint32_t) ~(1 << pwe_pos);
    }
}

/*******************************************************************************************************************//**
 * Enable access to the PWE registers.
 **********************************************************************************************************************/
void R_BSP_CyclicRunPinAccessEnable ()
{
    R_BSP_PORT->PKCPROT = BSP_IO_PORT_KEYCODE_ENABLE;
}

/*******************************************************************************************************************//**
 * Disable access to the PWE registers.
 **********************************************************************************************************************/
void R_BSP_CyclicRunPinAccessDisable ()
{
    R_BSP_PORT->PKCPROT = BSP_IO_PORT_KEYCODE_DISABLE;
}

/*******************************************************************************************************************//**
 * Read the current input level of the pin. Pin must be a valid pin before calling this function
 *
 * @param[in]  pin             The pin
 *
 * @retval     Current input level
 **********************************************************************************************************************/
uint32_t R_BSP_CyclicRunPinRead (bsp_io_port_pin_t pin)
{
    uint32_t level      = 0;
    uint32_t port_type  = (pin & BSP_IO_PORT_TYPE_MASK) >> BSP_IO_PORT_TYPE_POS;
    uint32_t port_group = (pin & BSP_IO_PORT_GROUP_MASK) >> BSP_IO_PORT_GROUP_POS;
    uint32_t port_pin   = pin & BSP_IO_PORT_PIN_MASK;

    if (NULL != R_BSP_CyclicRun_PORT_TYPE[port_type])
    {
        level = R_BSP_CyclicRun_PORT_TYPE[port_type]->PCRn_m[port_group].PCR_b[port_pin].PPR;
    }

    return level;
}

/*******************************************************************************************************************//**
 * Set the output level to the level provided. Disable port write
 * protection using R_BSP_PortWriteEnable() before calling this function.
 *
 * @param[in]  pin      The pin to write
 * @param[in]  level    The level
 **********************************************************************************************************************/
void R_BSP_CyclicRunPinWrite (bsp_io_port_pin_t pin, bsp_io_level_t level)
{
    uint32_t port_type  = (pin & BSP_IO_PORT_TYPE_MASK) >> BSP_IO_PORT_TYPE_POS;
    uint32_t port_group = pin & BSP_IO_PORT_GROUP_MASK;
    uint32_t port_pin   = pin & BSP_IO_PORT_PIN_MASK;

    if (NULL != R_BSP_CyclicRun_PORT_TYPE[port_type])
    {
        R_BSP_CyclicRun_PORT_TYPE[port_type]->PORT_CTR[port_group >>
                                                       BSP_IO_PORT_GROUP_POS].PSR =
            (1U << (BSP_IO_SR_MASK_POS + port_pin)) |
            (level << port_pin);
    }
}

/*******************************************************************************************************************//**
 * Toggle the level of the provided pin. Disable port write
 * protection using R_BSP_PortWriteEnable() before calling this function.
 *
 * @param[in]  pin      The pin to write
 **********************************************************************************************************************/
void R_BSP_CyclicRunPinToggle (bsp_io_port_pin_t pin)
{
    uint32_t port_type  = (pin & BSP_IO_PORT_TYPE_MASK) >> BSP_IO_PORT_TYPE_POS;
    uint32_t port_group = pin & BSP_IO_PORT_GROUP_MASK;
    uint32_t port_pin   = pin & BSP_IO_PORT_PIN_MASK;

    if (NULL != R_BSP_CyclicRun_PORT_TYPE[port_type])
    {
        R_BSP_CyclicRun_PORT_TYPE[port_type]->PORT_CTR[port_group >> BSP_IO_PORT_GROUP_POS].PNOT = (1 << port_pin);
    }
}

/*******************************************************************************************************************//**
 * Configure a pin. Disable port write protection by using R_BSP_PortWriteEnable() before calling
 * this function.
 *
 * @param[in]  pin      The pin to be configured
 * @param[in]  cfg      Configuration for the pin (PCRm_n register setting)
 **********************************************************************************************************************/
void R_BSP_CyclicRunPinCfg (bsp_io_port_pin_t pin, uint32_t cfg)
{
    uint32_t port_type  = (pin & BSP_IO_PORT_TYPE_MASK) >> BSP_IO_PORT_TYPE_POS;
    uint32_t port_group = pin & BSP_IO_PORT_GROUP_MASK;
    uint32_t port_pin   = pin & BSP_IO_PORT_PIN_MASK;

    if (NULL != R_BSP_CyclicRun_PORT_TYPE[port_type])
    {
        R_BSP_CyclicRun_PORT_TYPE[port_type]->PCRn_m[port_group >> BSP_IO_PORT_GROUP_POS].PCR[port_pin] = cfg;
    }
}

/***********************************************************************************************************************
 * Standby Controller
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Enable supply clock of configured Module standby available in Cyclic RUN Mode that is enabled on BSP MDF.
 **********************************************************************************************************************/
void R_BSP_CyclicRunModuleStandbyEnableAll (void)
{
    uint8_t msr_reg_count;
    uint8_t total_msr;

    /* Get total of g_msr_info_table elements */
    total_msr = sizeof(g_cyclicrun_msr_info_table) / sizeof(g_cyclicrun_msr_info_table[0]);

    /* Loop all configured Module standby for initialization. Skip the first element BSP_MODULE_STANDBY_START */
    for (msr_reg_count = 0; msr_reg_count < total_msr; msr_reg_count++)
    {
        /* Get Module standby initial configured value of specific module */
        /* If enable Module standby, process to enable clock supply */
        bsp_cyclicrun_stbc_set_channels((bsp_module_standby_t) msr_reg_count, 1U);
    }
}

/*******************************************************************************************************************//**
 * Disable all Clock supply available in Cyclic RUN Mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunModuleStandbyDisableAll (void)
{
    uint8_t msr_reg_count;
    uint8_t total_msr;

    /* Get total of g_msr_info_table elements */
    total_msr = sizeof(g_cyclicrun_msr_info_table) / sizeof(g_cyclicrun_msr_info_table[0]);

    /* Loop all configured Module standby for initialization. Skip the first element BSP_MODULE_STANDBY_START */
    for (msr_reg_count = 0; msr_reg_count < total_msr; msr_reg_count++)
    {
        /* Disable clock supply to target module */
        bsp_cyclicrun_stbc_set_channels((bsp_module_standby_t) msr_reg_count, 0U);
    }
}

/*******************************************************************************************************************//**
 * Enable clock supply for input module. Module Standby Register for
 * target module MSR_<module name> in Cyclic RUN mode.
 *
 * @param[in] module      Module to enable supply clock
 * @param[in] channel     The channel belongs to the target module
 **********************************************************************************************************************/
void R_BSP_CyclicRunModuleStandbyEnable (bsp_module_standby_t module, uint32_t channel)
{
    /* Enable clock supply to target module */
    bsp_cyclicrun_stbc_set_channel(module, channel, 1U);
}

/*******************************************************************************************************************//**
 * Disable clock supply for input module. Module Standby Register for
 * target module MSR_<module name> in Cyclic RUN mode.
 *
 * @param[in] module      Module to disable supply clock.
 * @param[in] channel     The channel belongs to the target module
 **********************************************************************************************************************/
void R_BSP_CyclicRunModuleStandbyDisable (bsp_module_standby_t module, uint32_t channel)
{
    /* Disable clock supply to target module */
    bsp_cyclicrun_stbc_set_channel(module, channel, 0U);
}

/*******************************************************************************************************************//**
 * Perform a MCU standby mode. This function can only be used in CYCLIC mode.
 *
 * @param[in] mode      Standby Mode to be performed
 **********************************************************************************************************************/
void R_BSP_CyclicRunMcuStandbyModeSet (bsp_mcu_standby_mode_t mode)
{
    /* Call function via function pointer */
    r_bsp_cyclicrun_mcu_standby_mode_funcs[mode]();
}

/*******************************************************************************************************************//**
 * Clear and set wake up source for MCU. This function can only be used in Cyclic mode.
 *
 * @param[in] wake_src     Wake Up Factor.
 **********************************************************************************************************************/
void R_BSP_CyclicRunWakeupFactorClearAndSet (bsp_wk_factor_cfg_t wake_src)
{
    uint8_t num_of_factors;
    uint8_t wakeup_reg;
    uint8_t wakeup_reg_count;

    /* Clear Wake up Flag */
    for (wakeup_reg_count = 0U;
         wakeup_reg_count < BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE;
         wakeup_reg_count++)
    {
        *((volatile uint32_t *) r_bsp_run_mode_wakeup_reg[wakeup_reg_count +
                                                          BSP_WK_REG_WUFC_A0]) = BSP_WAKEUP_FACTOR_MASK;
        *((volatile uint32_t *) r_bsp_cyclicrun_mode_wakeup_reg[wakeup_reg_count +
                                                                BSP_WK_REG_WUFC_A0]) = BSP_WAKEUP_FACTOR_MASK;
    }

    /* Mask all wake-up factor in RUN mode */
    for (num_of_factors = 0U; num_of_factors < BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE; num_of_factors++)
    {
        *((volatile uint32_t *) r_bsp_run_mode_wakeup_reg[num_of_factors +
                                                          BSP_WK_REG_WUFMSK_A0]) = BSP_WAKEUP_FACTOR_MASK;
    }

    /* Mask all wake-up factor in CYCLIC mode */
    for (num_of_factors = 0U; num_of_factors < BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE; num_of_factors++)
    {
        *((volatile uint32_t *) r_bsp_cyclicrun_mode_wakeup_reg[num_of_factors +
                                                                BSP_WK_REG_WUFMSK_A0]) = BSP_WAKEUP_FACTOR_MASK;
    }

    /* When a Wake-Up Factor is assigned to both Wake-Up Factor 0 registers and Wake-Up
     * Factor 1 registers, it can be used only in one of them. */

    /* Set wake up source for RUN mode */
    for (num_of_factors = 0U; num_of_factors < wake_src.num_of_wk_run_factor; num_of_factors++)
    {
        wakeup_reg = (uint8_t) wake_src.wk_run[num_of_factors] / BSP_WAKEUP_FACTOR_WORD;
        *((volatile uint32_t *) r_bsp_run_mode_wakeup_reg[wakeup_reg + BSP_WK_REG_WUFMSK_A0]) &=
            ~(BSP_WAKEUP_FACTOR_BIT_MASK << (wake_src.wk_run[num_of_factors] % BSP_WAKEUP_FACTOR_WORD));
    }

    /* Set wake up source for CYCLIC mode */
    for (num_of_factors = 0U; num_of_factors < wake_src.num_of_wk_cyclicrun_factor; num_of_factors++)
    {
        wakeup_reg = (uint8_t) wake_src.wk_cyclicrun[num_of_factors] / BSP_WAKEUP_FACTOR_WORD;
        *((volatile uint32_t *) r_bsp_cyclicrun_mode_wakeup_reg[wakeup_reg + BSP_WK_REG_WUFMSK_A0]) &=
            ~(BSP_WAKEUP_FACTOR_BIT_MASK << (wake_src.wk_cyclicrun[num_of_factors] % BSP_WAKEUP_FACTOR_WORD));
    }
}

/*******************************************************************************************************************//**
 * Get Wake Up Factor in DeepStop Mode and Stop Mode. This function can only be used in Cyclic RUN Mode.
 *
 * @return     Wake Up Factor.
 **********************************************************************************************************************/
bsp_wk_factor_t R_BSP_CyclicRunWakeUpFactorGetAndClear (void)
{
    bsp_wk_factor_t factor = BSP_WK_FACTOR_NONE;
    uint8_t         wakeup_reg_count;

    /* Check whether it was waked up or not */
    if (0U != ((R_WUF->WUFMON) & BSP_WAKEUP_FACTOR_1_MONITOR_MASK))
    {
        uint8_t bit_one_pos    = SCH1R(((R_WUF->WUFMON) & BSP_WAKEUP_FACTOR_1_MONITOR_MASK) >> 16U) - 1U;
        uint8_t wakeup_bit_pos =
            SCH1R(*((volatile uint32_t *) r_bsp_cyclicrun_mode_wakeup_reg[bit_one_pos + BSP_WK_REG_WUF_A0])) - 1U;
        factor =
            (bsp_wk_factor_t) (wakeup_bit_pos + (bit_one_pos * BSP_WAKEUP_FACTOR_WORD) +
                               BSP_STANDBY_WAKE_UP_FACTOR_A0_START);
    }

    /* Clear Wake up Flag */
    for (wakeup_reg_count = 0U;
         wakeup_reg_count < BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_NUM_OF_WAKEUP_SOURCE;
         wakeup_reg_count++)
    {
        *((volatile uint32_t *) r_bsp_run_mode_wakeup_reg[wakeup_reg_count +
                                                          BSP_WK_REG_WUFC_A0]) = BSP_WAKEUP_FACTOR_MASK;
        *((volatile uint32_t *) r_bsp_cyclicrun_mode_wakeup_reg[wakeup_reg_count +
                                                                BSP_WK_REG_WUFC_A0]) = BSP_WAKEUP_FACTOR_MASK;
    }

    return factor;
}

/*******************************************************************************************************************//**
 * Hold I/O Port State before enter DeepStop. This function can only be used in Cyclic RUN Mode.
 *
 * @param[in] io_buffer      I/0 Port.
 **********************************************************************************************************************/
void R_BSP_CyclicRunSetIOBufferHold (bsp_io_hold_t io_buffer)
{
    /* Enable write access to STBC control-protected registers */
    R_SYS0_STBC->STBCKCPROT = BSP_PRV_KEYCODE_ENABLE;

    /* If requesting to clear all IOHOLD bits */
    if (BSP_IO_HOLD_CLEAR_ALL == io_buffer)
    {
        R_SYS0_A_STBC->IOHOLD0 = BSP_IOHOLD_CLEAR_ALL;
 #if (1U == BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME)
        R_SYS0_A_STBC->IOHOLD1 = BSP_IOHOLD_CLEAR_ALL;
 #elif (2U == BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME)
        R_SYS0_A_STBC->IOHOLD2 = BSP_IOHOLD_CLEAR_ALL;
 #endif
    }
    else
    {
        /* If io_buffer index belongs to the upper IOHOLD register */
        if (io_buffer >= BSP_WAKEUP_FACTOR_WORD)
        {
 #if (1U == BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME)
            R_SYS0_A_STBC->IOHOLD1 |= (uint32_t) (1U << (io_buffer % BSP_WAKEUP_FACTOR_WORD));
 #elif (2U == BSP_FEATURE_CHIP_STANDBY_IOHOLD_REG_NAME)
            R_SYS0_A_STBC->IOHOLD2 |= (uint32_t) (1U << (io_buffer % BSP_WAKEUP_FACTOR_WORD));
 #endif
        }
        else
        {
            /* Otherwise set bit in the IOHOLD0 register */
            R_SYS0_A_STBC->IOHOLD0 |= (uint32_t) (1U << (io_buffer % BSP_WAKEUP_FACTOR_WORD));
        }
    }

    /* Disable write access to STBC control-protected registers */
    R_SYS0_STBC->STBCKCPROT = BSP_PRV_KEYCODE_DISABLE;
}

/***********************************************************************************************************************
 * FPU/FXU
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initialize FPU. Can only be used in Cyclic Run Mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunFPUInit (void)
{
    __asm volatile (
        "stsr  6, r10, 1        \n"    /* r10 <- PID */
        "shl  21, r10           \n"
        "shr  30, r10           \n"
        "bz  .exit_fpu_init     \n"    /* detecting FPU */
        "stsr  5, r10, 0        \n"    /* r10 <- PSW */
        "movhi  0x0001, r0, r11 \n"
        "or  r11, r10           \n"
        "ldsr  r10, 5, 0        \n"    /* enable FPU */
        "movhi  0x0002, r0, r11 \n"
        "ldsr  r11, 6, 0        \n"
        "ldsr  r0, 7, 0         \n"    /* initialize FPEPC */
        ".exit_fpu_init:        \n"
        );
}

/*******************************************************************************************************************//**
 * Initialize FXU. Can only be used in Cyclic Run Mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunFXUInit (void)
{
    __asm volatile (
        "stsr  6, r10, 1            \n" /* r10 <- PID */
        "shl  20, r10               \n"
        "shr  31, r10               \n"
        "bz  .exit_fxu_init         \n" /* detecting FXU */
        "stsr  5, r10, 0            \n" /* r10 <- PSW */
        "movhi  0x0002, r0, r11     \n"
        "or  r11, r10               \n"
        "ldsr  r10, 5, 0            \n" /* enable FXU */
        "movhi  0x0002, r0, r11     \n"
        "ldsr  r11, 6, 10           \n" /* initialize FXSR */
        "ldsr  r0, 8, 10            \n" /* initialize FXST */
        "ldsr  r0, 10, 10           \n" /* initialize FXCFG */
        ".exit_fxu_init:            \n"
        );
}

/***********************************************************************************************************************
 * Instruction Cache.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Enable Instruction cache. Can only be used in Cyclic Run Mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunMPUInstructionCacheEnable (void)
{
 #if defined(__CCRH__)
    __asm__ volatile (
        "mov 3, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
    );
 #elif defined(__ghs__) || defined(__GNUC__)
    __asm__ volatile (
        "mov 3, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
        :
        :
        : "r10"
    );
 #endif
}

/*******************************************************************************************************************//**
 * Disable Instruction cache. Can only be used in Cyclic Run Mode.
 **********************************************************************************************************************/
void R_BSP_CyclicRunMPUInstructionCacheDisable (void)
{
 #if defined(__CCRH__)
    __asm__ volatile (
        "mov 0, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
    );
 #elif defined(__ghs__) || defined(__GNUC__)
    __asm__ volatile (
        "mov 0, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
        :
        :
        : "r10"
    );
 #endif
}

/***********************************************************************************************************************
 * Internal Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Using the vector table information section that has been built by the linker and placed into ROM in the.
 * vector_info. section, this function will initialize the INTC1, INTC2 and FEINT, so that configured interrupts
 * will trigger interrupts in the INTC1, INTC2 and FEINT.
 * Used in Cyclic RUN Mode
 **********************************************************************************************************************/
void bsp_cyclicrun_irq_cfg (void)
{
    R_INTC1_PE0_Type * R_INTC1_PE;
    uint16_t           irq;

    R_INTC1_PE = (R_INTC1_PE0_Type *) (R_INTC1_PE0_BASE);
    for (irq = 0; irq < BSP_INT_VECTOR_MAX_ENTRIES; irq++)
    {
        /* INTC1 */
        if (irq < BSP_INTC1_VECTOR_MAX_ENTRIES)
        {
            R_INTC1_PE->EIC_b[irq].EITB |= (uint16_t) (BSP_EIC_EITB_SET);
        }

        /* INTC2 */
        else if (irq < BSP_INTC_VECTOR_MAX_ENTRIES)
        {
            R_INTC2->EIC_b[irq].EITB |= (uint16_t) (BSP_EIC_EITB_SET);
        }

        /* FEINT */
        else
        {
            /* Nothing to do */
        }
    }

    /* Enable interrupt */
    EI();
}

/*******************************************************************************************************************//**
 * Dummy ISR. Used in Cyclic RUN Mode
 **********************************************************************************************************************/
void DummyIsr_CyclicRun (void)
{
    while (1)
    {
        ;
    }
}

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/********************************************************************************************************************
 * Software delay support 64 bits parameters
 * @param[in]   loop_cnt  The number of 'units' to delay.
 * @retval: void
 * *****************************************************************************************************************/
static void bsp_cyclicrun_software_delay_loop (uint64_t loop_cnt)
{
    /* Split the 64-bit loop counter into upper and lower 32-bit parts, get upper part */
    uint32_t upper_part = (uint32_t) (loop_cnt >> 32);

    /* Outer loop: Iterate over the upper 32 bits */
    while (upper_part > 0)
    {
        /* Call the assembly function with the 32-bit loop counter*/
        bsp_cyclicrun_prv_software_delay_loop(0xFFFFFFFF);

        /* Decrement the upper part of the loop counter */
        upper_part--;
    }

    if (0 == upper_part)
    {
        bsp_cyclicrun_prv_software_delay_loop((uint32_t) loop_cnt);
    }
}

/********************************************************************************************************************
 * 1 cycle as CPUCLKD
 * LOOP cycle: 4 When the updated reg1 != 0 \ 2 When the updated reg1 = 0
 * NOP cycle: 1 to cover 2 cycles when reg1 of LOOP cycle == 0
 * Total cycle = (loop_cnt * 4)
 *
 * @param[in]   loop_cnt  The number of 'units' to delay.
 * @retval: void
 * *****************************************************************************************************************/
void bsp_cyclicrun_prv_software_delay_loop (uint32_t loop_cnt)
{
 #if defined(__CCRH__)
    __asm__ volatile (
         "nop            \n"
         "nop            \n"
         "loop r6, 0"
    );
 #elif defined(__ghs__) || defined(__GNUC__)
    __asm__ volatile (
         "nop            \n"
         "nop            \n"
#if defined(__ghs__)
         "loop_label%=: "
         "loop %0, loop_label%="
#elif defined(__GNUC__)
         "loop %0, 0"
#endif
         : "+r" (loop_cnt)
         :
         : "memory"
    );
 #endif
}

/*******************************************************************************************************************//**
 * Calls the callback function for an interrupt if a callback has been registered.
 * Used in Cyclic RUN Mode
 *
 * @param[in]   irq         Which interrupt to check and possibly call.
 *
 * @retval FSP_SUCCESS              Callback was called.
 * @retval FSP_ERR_INVALID_ARGUMENT No valid callback has been registered for this interrupt source.
 *
 * @warning This function is called from within an interrupt
 **********************************************************************************************************************/
static void bsp_cyclicrun_group_isr_call (bsp_grp_irq_t irq)
{
    /** Check for valid callback */
    if (NULL != g_bsp_cyclicrun_group_isr_sources[irq])
    {
        /** Callback has been found. Call it. */
        g_bsp_cyclicrun_group_isr_sources[irq](irq);
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
 * Enter Cyclic DeepStop Mode. Depending on operation parameter, this function will set wakeup factor or perform
 * DeepStop operation
 * This function can only be used in Cyclic RUN Mode.
 **********************************************************************************************************************/
static void bsp_cyclicrun_deep_stop_mode (void)
{
    /* Disable Interrupt */
    DI();

    /* Reset Wake Up Factor Flag */
    R_SYS_RESET->RESFC = BSP_RESET_FLAG_MASK;

    R_SYS0_A_STBC->PWRGD_CNT = BSP_WAIT_TIME_FOR_VDD_STABLE;

    /* Transition to DeepStop */
    R_SYS0_STBC->STBCKCPROT = BSP_PRV_KEYCODE_ENABLE;
    R_SYS0_A_STBC->STBC0PSC = R_SYS0_A_STBC_STBC0PSC_STBC0DISTRG_Msk;
    R_SYS0_STBC->STBCKCPROT = BSP_PRV_KEYCODE_DISABLE;
}

/*******************************************************************************************************************//**
 * Enter Stop Mode. Depending on operation parameter, this function will set wakeup factor or perform Stop operation
 * This function can only be used in Cyclic RUN Mode.
 **********************************************************************************************************************/
static void bsp_cyclicrun_stop_mode (void)
{
    /* Disable Interrupt */
    DI();

    /* Reset Wake Up Factor Flag */
    R_SYS_RESET->RESFC = BSP_RESET_FLAG_MASK;

    R_SYS0_A_STBC->PWRGD_CNT = BSP_WAIT_TIME_FOR_VDD_STABLE;

    /* Transition to Stop */
    R_SYS0_STBC->STBCKCPROT  = BSP_PRV_KEYCODE_ENABLE;
    R_SYS0_A_STBC->STBC0STPT = R_SYS0_A_STBC_STBC0STPT_STBC0STPTRG_Msk;
    R_SYS0_STBC->STBCKCPROT  = BSP_PRV_KEYCODE_DISABLE;
}

/*******************************************************************************************************************//**
 * Enter Halt state. This function can only be used in Cyclic RUN Mode.
 **********************************************************************************************************************/
static void bsp_cyclicrun_halt (void)
{
    HALT();
}

/*******************************************************************************************************************//**
 * @brief Configures the channel setting for a specific STBC module.
 *
 * Enables or disables the specified channel for the given STBC module.
 *
 * @param[in] module   Target STBC module to configure.
 * @param[in] channel  Channel index to be set.
 * @param[in] enable   Enable (true) or disable (false) the specified channel.
 **********************************************************************************************************************/
static void bsp_cyclicrun_stbc_set_channel (bsp_module_standby_t module, uint32_t channel, bool enable)
{
    volatile uint32_t * p_msr_address;
    uint32_t            setting_value;

    /* Get the MSR module register address */
    p_msr_address = (uint32_t *) (g_cyclicrun_msr_info_table[module].msr_reg_address);

    /* Enable/Disable clock supply to target module */
    if (enable)
    {
        setting_value = (*p_msr_address & (uint32_t) ~(1U << channel));
    }
    else
    {
        setting_value = (*p_msr_address | (1 << channel));
    }

    /* Write the setting value */
    bsp_cyclicrun_stbc_write_with_protection(p_msr_address, setting_value);
}

/*******************************************************************************************************************//**
 * Configures the STBC module with the specified module.
 *
 * @param[in] module       Module to enable supply clock.
 * @param[in] enable       Enable/Disable supply clock for the target.
 **********************************************************************************************************************/
static void bsp_cyclicrun_stbc_set_channels (bsp_module_standby_t module, bool enable)
{
    volatile uint32_t * p_msr_address;
    uint32_t            setting_value;
    uint32_t            default_value;

    p_msr_address = (uint32_t *) (g_cyclicrun_msr_info_table[module].msr_reg_address);

    if (enable)
    {
        /* If enabled, retrieve the initial value from the configuration. */
        setting_value = g_cyclicrun_msr_info_table[module].initial_cfg_value;
    }
    else
    {
        /* If disabled, retrieve the default value for each module. */
        default_value = (uint32_t) (g_cyclicrun_msr_info_table[module].default_value | \
                                    (uint32_t) (1U << BSP_MODULE_MSR_STPMSK_Pos));
        setting_value = (uint32_t) (*p_msr_address | default_value);
    }

    /* Write the setting value */
    bsp_cyclicrun_stbc_write_with_protection(p_msr_address, setting_value);
}

/*******************************************************************************************************************//**
 * @brief Writes a value to a protected STBC register. This function can only be used in Cyclic RUN Mode.
 *
 * Enables write access to the specified STBC register, writes the given value,
 * and then disables write access to ensure register protection.
 *
 * @param[in] p_msr_address  Pointer to the target STBC register address.
 * @param[in] value          Value to be written to the register.
 **********************************************************************************************************************/
static void bsp_cyclicrun_stbc_write_with_protection (volatile uint32_t * p_msr_address, uint32_t value)
{
    /* Enable write access to protected registers */
    R_SYS0_STBC->MSRKCPROT = (uint32_t) BSP_PRV_KEYCODE_ENABLE;

    *p_msr_address = value;

    /* Disable write access to protected registers */
    R_SYS0_STBC->MSRKCPROT = (uint32_t) BSP_PRV_KEYCODE_DISABLE;
}

BSP_PRAGMA_PLACE_IN_SECTION(BSP_PRAGMA_SECTION_TEXT, BSP_PRAGMA_SECTION_DEFAULT)

#endif                                 /* #if (BSP_CYCLICRUN_MODE_ENABLE) && (BSP_PE0_USED) */

/** @} (end addtogroup BSP_STANDBY_CRUN) */

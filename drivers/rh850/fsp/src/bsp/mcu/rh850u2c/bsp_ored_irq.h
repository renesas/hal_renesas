/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_ORED_IRQ_H
#define BSP_ORED_IRQ_H

#include "bsp_api.h"

#if (BSP_FEATURE_DEVICE_HAS_ORED_IRQ)
 #include "bsp_ored_irq_info.h"
#endif

FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_INTMASK_U2CX BSP Interrupt Mask
 * @brief Interrupt source mask for RH850 MCUs.
 *
 * @{
 **********************************************************************************************************************/

#define BSP_ORED_MASK                  0xFFU                                                              ///< Mask for extracting 8-bit ORed fields

/** Extract fields from compact ID */
#define BSP_OREDINT_SHIFT_MODULE_ID    (24U)                                                              ///< Bit shift for module ID
#define BSP_OREDINT_SHIFT_EIC          (16U)                                                              ///< Bit shift for EIC index
#define BSP_OREDINT_SHIFT_CHANNEL      (8U)                                                               ///< Bit shift for channel value
#define BSP_OREDINT_SHIFT_ORED         (0U)                                                               ///< Bit shift for ORed ID

#define BSP_OREDINT_GET_MODULE_ID(cfg)    (((cfg) >> BSP_OREDINT_SHIFT_MODULE_ID) & BSP_ORED_MASK)        ///< Get module ID
#define BSP_OREDINT_GET_EIC(cfg)          (((cfg) >> BSP_OREDINT_SHIFT_EIC) & BSP_ORED_MASK)              ///< Get EIC index
#define BSP_OREDINT_GET_CHANNEL(cfg)      (((cfg) >> BSP_OREDINT_SHIFT_CHANNEL) & BSP_ORED_MASK)          ///< Get channel ID
#define BSP_OREDINT_GET_ORED(cfg)         ((cfg) & BSP_ORED_MASK)                                         ///< Get ORed index

/** Maximum number of supported ORed channels */
#define BSP_ORED_MAX_OFFSET_CHANNEL        (16U)                                                          ///< Maximum number of ORed channels
#define BSP_ORED_OFFSET_CHANNEL            (4U)                                                           ///< Offset per ORed channel
#define BSP_ORED_OFFSET_REGISTER           (0x10U)                                                        ///< Offset to ORed mask register
#define BSP_ORED_OFFSET_TYPE_REGISTER      (0x20U)                                                        ///< Offset to ORed type register
#define BSP_ORED_REG_COUNT_MAX             (4U)                                                           ///< Maximum count of ORed registers
#define BSP_ORED_MAX_TYPE_MSPI_COUNT       (4U)                                                           ///< Max MSPI ORed type count
#define BSP_ORED_MAX_TYPE_CANFD_COUNT      (6U)                                                           ///< Max CANFD ORed type count
#define BSP_ORED_MAX_TYPE_COUNT            (BSP_ORED_MAX_TYPE_MSPI_COUNT + BSP_ORED_MAX_TYPE_CANFD_COUNT) ///< Total ORed type count

/** ORed register mask */
#define BSP_ORED_MSK_ALL_CHANNELS          (0xFFFFFFFFUL)                                                 ///< Mask for all ORed channels enabled

/** Mask value to disable all MSPI interrupt channels in OR-ed configuration */
#define BSP_ORED_MSK_MSPI_ALL_CHANNELS     (0x3FFUL)                                                      ///< MSPI: disable all channels

/** Mask value to disable all CANFD interrupt channels in OR-ed configuration */
#define BSP_ORED_MSK_CANFD_ALL_CHANNELS    (0x3FFFUL)                                                     ///< CANFD: disable all channels

/** EIIC status register mask */
#define BSP_ORED_MSPI_EIC_START            (88U)                                                          ///< MSPI EIC start index
#define BSP_ORED_CANFD_EIC_START           (106U)                                                         ///< CANFD EIC start index
#define BSP_ORED_MSPI_GROUP_SIZE           (4U)                                                           ///< MSPI group size
#define BSP_ORED_CANFD_GROUP_SIZE          (6U)                                                           ///< CANFD group size
#define BSP_ORED_GROUP_SIZE_DIFF           (2U)                                                           ///< Difference between group sizes
#define BSP_ORED_EIC_START_DIFF            (18U)                                                          ///< EIC offset between MSPI and CANFD

/** Base address for interrupt mask registers */
#define BSP_MASK_BASE                      (0xFF090800UL)                                                 ///< Base address for ORed mask registers

/** MSPI EIC range */
#define BSP_MSPI_EIC_START                 (88U)                                                          ///< MSPI EIC range start
#define BSP_MSPI_EIC_END                   (103U)                                                         ///< MSPI EIC range end
#define BSP_MSPI_GROUP_SIZE                (4U)                                                           ///< MSPI group size
#define BSP_MSPI_OFFSET_BASE               (0x10U)                                                        ///< MSPI base offset
#define BSP_MSPI_OFFSET_STEP               (0x20U)                                                        ///< MSPI offset step

/** CANFD EIC range */
#define BSP_CANFD_EIC_START                (106U)                                                         ///< CANFD EIC range start
#define BSP_CANFD_EIC_END                  (129U)                                                         ///< CANFD EIC range end
#define BSP_CANFD_GROUP_SIZE               (6U)                                                           ///< CANFD group size
#define BSP_CANFD_OFFSET_BASE              (0x90U)                                                        ///< CANFD base offset
#define BSP_CANFD_OFFSET_STEP              (0x20U)                                                        ///< CANFD offset step

/** Calculate register offset for a given EIC */
#define BSP_EIC_OFFSET(eic)                                                                            \
    (                                                                                                  \
        /* MSPI (EIC 88–103): channels 0..3 */                                                         \
        ((((uint32_t) ((eic) - BSP_MSPI_EIC_START) % BSP_MSPI_GROUP_SIZE) * BSP_MSPI_OFFSET_STEP) +    \
         BSP_MSPI_OFFSET_BASE) *                                                                       \
        (uint32_t) ((eic) >= BSP_MSPI_EIC_START && (eic) <= BSP_MSPI_EIC_END) +                        \
        /* CANFD (EIC 106–129): channels 0..5 */                                                       \
        ((((uint32_t) ((eic) - BSP_CANFD_EIC_START) % BSP_CANFD_GROUP_SIZE) * BSP_CANFD_OFFSET_STEP) + \
         BSP_CANFD_OFFSET_BASE) *                                                                      \
        (uint32_t) ((eic) >= BSP_CANFD_EIC_START && (eic) <= BSP_CANFD_EIC_END)                        \
    )

/** Calculate ORED number from EIIC value */
#define BSP_ORED_GET_CHANNEL(eic)                                                                          \
    ((((eic) - (BSP_ORED_MSPI_EIC_START + (((eic) / BSP_ORED_CANFD_EIC_START) * BSP_ORED_EIC_START_DIFF))) \
      / (BSP_ORED_MSPI_GROUP_SIZE + (((eic) / BSP_ORED_CANFD_EIC_START) * BSP_ORED_GROUP_SIZE_DIFF)))      \
     % (BSP_ORED_MSPI_GROUP_SIZE + (((eic) / BSP_ORED_CANFD_EIC_START) * BSP_ORED_GROUP_SIZE_DIFF)))       \


/** Calculate OREDIRQ number from EIC and channel */
#define BSP_ORED_IRQ_CONVERT(eic, channel) \
    ((IRQn_Type) ((!channel) * eic) +      \
     ((!!channel) *                        \
      (BSP_INTC2_VECTOR_MAX_ENTRIES + channel + (eic - BSP_ORED_MSPI_EIC_START) * BSP_ORED_MAX_OFFSET_CHANNEL)))

/** Mask address = base + offset + per-channel increment */
#define BSP_MASK_ADDR(eic, ored) \
    (BSP_MASK_BASE + BSP_EIC_OFFSET((eic)) + (BSP_ORED_OFFSET_CHANNEL * (uint32_t) (ored)))

/** Read mask address */
#define BSP_READ_MASK_ADDR(eic, ored)                                   \
    (BSP_MASK_BASE + BSP_EIC_OFFSET((eic)) - BSP_ORED_OFFSET_REGISTER + \
     (BSP_ORED_OFFSET_CHANNEL * (uint32_t) (ored)))

/*******************************************************************************************************************//**
 * @brief  It sets all interrupt sources to masked state, effectively disabling them at startup.
 *         This ensures that no interrupt is triggered until explicitly enabled by the application.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_ORed_IrqInit (void)
{
    volatile uint32_t * p_ored_msk_reg_n;
    uint32_t            ored_msk_addr;
    uint8_t             number;
    uint8_t             type_number;

    /* Configure interrupt mask */
    for (type_number = 0U; type_number < BSP_ORED_MAX_TYPE_COUNT; type_number++)
    {
        /* Retrieve the mask register address of MSPI and CANFD */
        ored_msk_addr = BSP_MASK_BASE + BSP_ORED_OFFSET_TYPE_REGISTER * type_number + BSP_ORED_OFFSET_REGISTER;

        for (number = 0U; number < BSP_ORED_REG_COUNT_MAX; number++)
        {
            /* Get the mask register of each ORed's register number */
            p_ored_msk_reg_n = (volatile uint32_t *) (ored_msk_addr + BSP_ORED_OFFSET_CHANNEL * number);

            /* Mask all the channels */
            *p_ored_msk_reg_n = BSP_ORED_MSK_ALL_CHANNELS;
        }
    }
}

/*******************************************************************************************************************//**
 * @brief   Configures the interrupt source mask for a peripheral module.
 *
 * @param[in]  ored_int    ORED Interrupt source mask.
 * @param[in] priority     Priority of the interrupt
 * @param[in]  p_context   The interrupt context is a pointer to data required in the ISR.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_ORed_IrqCfgEnable (bsp_ored_t const ored_int, uint16_t priority, void * p_context)
{
    volatile uint32_t * p_ored_reg;
    uint32_t            ored_channel = BSP_OREDINT_GET_ORED(ored_int);
    uint32_t            channel      = BSP_OREDINT_GET_CHANNEL(ored_int);
    uint32_t            eic          = BSP_OREDINT_GET_EIC(ored_int);
    uint32_t            number;

    /* Configure interrupt mask */
    for (number = 0U; number < BSP_ORED_REG_COUNT_MAX; number++)
    {
        /* Configure other ored */
        p_ored_reg = (volatile uint32_t *) BSP_MASK_ADDR(eic, number);

        /* If the current channel matches the ORed channel and the priority is not disabled,
         *  the corresponding bit in the ORed register is set to not-mask to enable the interrupt trigger */
        if ((number == ored_channel) && (BSP_IRQ_DISABLED != priority))
        {
            *p_ored_reg &= (uint32_t) ~(1 << channel);
        }
        else
        {
            *p_ored_reg |= (uint32_t) (1 << channel);
        }
    }

    R_INTC2->EIC_b[eic].EIP = (uint16_t) (priority & R_INTC2_EIC_EIP_Msk);

    /* Set context for virtual IRQ */
    R_FSP_IsrContextSet((IRQn_Type) BSP_ORED_IRQ_CONVERT(eic, channel), p_context);

    /* Configure interrupt mask */
    if (BSP_IRQ_DISABLED != priority)
    {
        R_INTC2->EIC_b[eic].EIMK = BSP_ENABLE_EIIC;
    }

    /* Dummy read and sync */
    BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EIC[eic]);
}

/*******************************************************************************************************************//**
 * @brief   Disables the interrupt source mask for a peripheral module.
 *
 * @param[in]  ored_int    ORED Interrupt source mask.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_ORed_IrqCfgDisable (bsp_ored_t const ored_int)
{
    volatile uint32_t * p_ored_reg;
    uint32_t            ored_channel = BSP_OREDINT_GET_ORED(ored_int);
    uint32_t            channel      = BSP_OREDINT_GET_CHANNEL(ored_int);
    uint32_t            eic          = BSP_OREDINT_GET_EIC(ored_int);
    uint32_t            module       = BSP_OREDINT_GET_MODULE_ID(ored_int);
    uint32_t            mask_value   = 0UL;

    /* MSPI MASK with LOGICAL(module == 0) || CANFD MASK with LOGICAL(module != 0) */
    mask_value = !(module) * BSP_ORED_MSK_MSPI_ALL_CHANNELS + !!(module) * BSP_ORED_MSK_CANFD_ALL_CHANNELS;

    /* Configure interrupt mask to disable */
    p_ored_reg = (volatile uint32_t *) BSP_MASK_ADDR(eic, ored_channel);
    *(volatile uint32_t *) p_ored_reg |= (1U << channel);

    /* If all channels is masked, disable the interrupt */
    if (mask_value == (*(volatile uint32_t *) p_ored_reg & mask_value))
    {
        /* Disable the interrupt */
        R_INTC2->EIC_b[eic].EIMK = BSP_DISABLE_EIIC;
    }

    /* Dummy read and sync */
    BSP_INT_IOREG_DUMMY_READ_SYNCP(R_INTC2->EIC[eic]);
}

/*******************************************************************************************************************//**
 * @brief   Gets the IRQ for a peripheral module.
 **********************************************************************************************************************/
__STATIC_INLINE IRQn_Type R_FSP_ORed_CurrentIrqGet (void)
{
    volatile uint32_t * p_ored_reg;
    uint32_t            intid;
    uint32_t            channel;
    uint8_t             ored_channel;

    /* Get EIIC value */
    intid = STSR(EIIC);
    intid = intid & BSP_EIIC_MSK;

    /* Return if not in the range of ored EIC */
    if (BSP_ORED_MSPI_EIC_START > intid)
    {
        return (IRQn_Type) intid;
    }

    /* Get ored channel */
    ored_channel = (uint8_t) BSP_ORED_GET_CHANNEL(intid);

    /* Get ored register address */
    p_ored_reg = (volatile uint32_t *) BSP_READ_MASK_ADDR(intid, ored_channel);

    /* Get channel number */
    channel = (SCH1R(*p_ored_reg) - 1);

    /* Convert and return the value as IRQn_Type for the corresponding Interrupt Slot */
    return (IRQn_Type) BSP_ORED_IRQ_CONVERT(intid, channel);
}

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_INTMASK)
 **********************************************************************************************************************/
FSP_FOOTER
#endif                                 /* BSP_ORED_IRQ_H */

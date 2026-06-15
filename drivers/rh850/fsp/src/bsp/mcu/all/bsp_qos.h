/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_QOS_H
#define BSP_QOS_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

#if (BSP_FEATURE_QOS_IS_AVAILABLE)
 #include "bsp_qos_driver.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU
 * @defgroup BSP_QOS BSP QoS
 * @brief This section provides quality of service control support.
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Latency Monitor maximum number of channels */
 #define BSP_QOS_LATENCY_MONITOR_MAX_NUM_CHANNEL                                   (0x04U)

/** Latency Monitor start status */
 #define BSP_QOS_LATENCY_MONITOR_START                                             (0x01U)

/** Latency Monitor stop status */
 #define BSP_QOS_LATENCY_MONITOR_STOP                                              (0x00U)

/** Banwidth Regulator read/write access threshold forbid value */
 #define BSP_QOS_BANDWIDTH_REGULATOR_READ_WRITE_ACCESS_THRESHOLD_FORBID_VALUE_1    (0xFFFFU)

/** Banwidth Regulator read/write access threshold forbid value */
 #define BSP_QOS_BANDWIDTH_REGULATOR_READ_WRITE_ACCESS_THRESHOLD_FORBID_VALUE_2    (0xFFFEU)

/** Total of QoS's interrupt */
 #define BSP_QOS_IRQ_NUM_TOTAL                                                     (8U)

/** Banwidth Regulator max access count default value */
 #define BSP_QOS_MAX_ACCESS_COUNT_DEFAULT_VALUE                                    (0x00U)

/** Banwidth Regulator min access count default value */
 #define BSP_QOS_MIN_ACCESS_COUNT_DEFAULT_VALUE                                    (0xFFFFU)

/** Macros support accessing Latency Monitor's registers */
 #define BSP_QOS_LMn_INTBD(unit,                                                                                   \
                           channel)       *(volatile uint8_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_INTBD) + \
                                                                 (0x80U * channel))
 #define BSP_QOS_LMn_LTTH_R(unit,                                                                                    \
                            channel)      *(volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LTTH_R) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_LTTH_W(unit,                                                                                    \
                            channel)      *(volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LTTH_W) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_LTMAX_R(unit,                                                                                        \
                             channel)     *(const volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LTMAX_R) \
                                                                        + (0x80U * channel))
 #define BSP_QOS_LMn_LTMAX_W(unit,                                                                                        \
                             channel)     *(const volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LTMAX_W) \
                                                                        + (0x80U * channel))
 #define BSP_QOS_LMn_OVFTH_R(unit,                                                                                    \
                             channel)     *(volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_OVFTH_R) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_OVFTH_W(unit,                                                                                    \
                             channel)     *(volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_OVFTH_W) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_OVFCNT_R(unit,                                                                           \
                              channel)    *(const volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]-> \
                                                                                    LM0_OVFCNT_R) + (0x80U * channel))
 #define BSP_QOS_LMn_OVFCNT_W(unit,                                                                           \
                              channel)    *(const volatile uint16_t *) ((uint32_t) (&g_qos_info_table[unit]-> \
                                                                                    LM0_OVFCNT_W) + (0x80U * channel))
 #define BSP_QOS_LMn_LADDR0(unit,                                                                                    \
                            channel)      *(volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LADDR0) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_UADDR0(unit,                                                                                    \
                            channel)      *(volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_UADDR0) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_LADDR1(unit,                                                                                    \
                            channel)      *(volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_LADDR1) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_UADDR1(unit,                                                                                    \
                            channel)      *(volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_UADDR1) + \
                                                                  (0x80U * channel))
 #define BSP_QOS_LMn_SPID(unit,                                                                                    \
                          channel)        *(volatile uint32_t *) ((uint32_t) (&g_qos_info_table[unit]->LM0_SPID) + \
                                                                  (0x80U * channel))

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variable (to be accessed by other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This function returns the Minimum Read Access count for the input unit.
 *
 * @param[in]  unit          QoS unit.
 *
 * @retval     The value of Minimum Read Access count.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSBandwidthRegulatorMinReadAccessGet (bsp_qos_unit_t unit)
{
    return g_qos_info_table[(uint8_t) (unit)]->BR_ACCNTMIN_R;
}

/*******************************************************************************************************************//**
 * This function returns the Maximum Read Access count for the input unit.
 *
 * @param[in]  unit          QoS unit.
 *
 * @retval     The value of Maximum Read Access count.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSBandwidthRegulatorMaxReadAccessGet (bsp_qos_unit_t unit)
{
    return g_qos_info_table[(uint8_t) (unit)]->BR_ACCNTMAX_R;
}

/*******************************************************************************************************************//**
 * This function returns the Minimum Write Access count for the input unit.
 *
 * @param[in]  unit          QoS unit.
 *
 * @retval     The value of Minimum Write Access count.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSBandwidthRegulatorMinWriteAccessGet (bsp_qos_unit_t unit)
{
    return g_qos_info_table[(uint8_t) (unit)]->BR_ACCNTMIN_W;
}

/*******************************************************************************************************************//**
 * This function returns the Maximum Write Access count for the input unit.
 *
 * @param[in]  unit          QoS unit.
 *
 * @retval     The value of Maximum Write Access count.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSBandwidthRegulatorMaxWriteAccessGet (bsp_qos_unit_t unit)
{
    return g_qos_info_table[(uint8_t) (unit)]->BR_ACCNTMAX_W;
}

/*******************************************************************************************************************//**
 * This function clears the interrupt flag of Latency Monitor.
 *
 * @param[in]  unit          QoS unit.
 * @param[in]  bit           Bit in flag which need to get.
 *
 * @retval     Status of the bit.
 **********************************************************************************************************************/
__STATIC_INLINE uint8_t R_BSP_QoSLatencyMonitorInterruptFlagGet (bsp_qos_unit_t unit, uint8_t bit)
{
    volatile R_QOSCNT_FL_PE0CL0_Type * p_qos_reg = g_qos_info_table[(uint8_t) (unit)];

    /* Synchronization of instruction fetching */
    __SYNCP();

    return (p_qos_reg->LM_INTFLG & (1U << (uint8_t) (bit))) >> bit;
}

/*******************************************************************************************************************//**
 * This function clears the interrupt flag of Latency Monitor.
 *
 * @param[in]  unit          QoS unit.
 *
 * @retval     None
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_QoSLatencyMonitorInterruptFlagClear (bsp_qos_unit_t unit)
{
    volatile R_QOSCNT_FL_PE0CL0_Type * p_qos_reg = g_qos_info_table[(uint8_t) (unit)];
    p_qos_reg->LM_INTCLR = R_QOSCNT_FL_PE0CL0_LM_INTCLR_CLR_Msk;

    /* Wait for completion of the instruction, then refetch the subsequent instructions*/
    __SYNCI();
}

/*******************************************************************************************************************//**
 * This function returns the number of read operations that exceed the reading cycle threshold.
 *
 * @param[in]  unit             QoS unit.
 * @param[in]  channel          Latency Monitor channel.
 *
 * @retval     Number of read operations that exceed the reading cycle threshold.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSReadLatencyOverflowCountGet (bsp_qos_unit_t unit, uint8_t channel)
{
    return BSP_QOS_LMn_OVFCNT_R((uint8_t) unit, channel);
}

/*******************************************************************************************************************//**
 * This function returns the number of write operations that exceed the reading cycle threshold.
 *
 * @param[in]  unit             QoS unit.
 * @param[in]  channel          Latency Monitor channel.
 *
 * @retval     Number of write operations that exceed the reading cycle threshold.
 **********************************************************************************************************************/
__STATIC_INLINE uint16_t R_BSP_QoSWriteLatencyOverflowCountGet (bsp_qos_unit_t unit, uint8_t channel)
{
    return BSP_QOS_LMn_OVFCNT_W((uint8_t) unit, channel);
}

/*******************************************************************************************************************//**
 * This function enables Latency Overflow interrupt for specific interrupt port bit.
 *
 * @param[in]  unit                        QoS unit.
 * @param[in]  interrupt_port_bind_bit     Interrupt port bit.
 *
 * @retval     None.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_QoSLatencyOverflowIrqEnable (bsp_qos_unit_t unit, uint8_t interrupt_port_bind_bit)
{
    volatile R_QOSCNT_FL_PE0CL0_Type * p_qos_reg = g_qos_info_table[(uint8_t) (unit)];
    p_qos_reg->LM_INTEN_b.EN |= (1U << interrupt_port_bind_bit);

    /* Wait for loading to be completed */
    __SYNCP();
}

/*******************************************************************************************************************//**
 * This function disables Latency Overflow interrupt for specific interrupt port bit.
 *
 * @param[in]  unit                        QoS unit.
 * @param[in]  interrupt_port_bind_bit     Interrupt port bit.
 *
 * @retval     None.
 **********************************************************************************************************************/
__STATIC_INLINE void R_BSP_QoSLatencyOverflowIrqDisable (bsp_qos_unit_t unit, uint8_t interrupt_port_bind_bit)
{
    volatile R_QOSCNT_FL_PE0CL0_Type * p_qos_reg = g_qos_info_table[(uint8_t) (unit)];
    p_qos_reg->LM_INTEN_b.EN &= ~(1U << interrupt_port_bind_bit);

    /* Wait for loading to be completed */
    __SYNCP();
}

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/
void R_BSP_QoSEnableAll(void);
void R_BSP_QoSDisableAll(void);

fsp_err_t R_BSP_QoSBandwidthRegulatorEnable(const bsp_bandwidth_regulator_cfg_t * p_cfg);
void      R_BSP_QoSBandwidthRegulatorDisable(bsp_qos_unit_t unit);
uint16_t  R_BSP_QoSTotalReadAccessGet(bsp_qos_unit_t unit);
uint16_t  R_BSP_QoSTotalWriteAccessGet(bsp_qos_unit_t unit);
void      R_BSP_QoSNumberofAccessClear(bsp_qos_unit_t unit);

fsp_err_t R_BSP_QoSLatencyMonitorEnable(const bsp_latency_monitor_cfg_t * p_cfg);
void      R_BSP_QoSLatencyMonitorDisable(bsp_qos_unit_t unit, uint8_t channel);
void      R_BSP_QoSLatencyMonitorStart(bsp_qos_unit_t unit);
void      R_BSP_QoSLatencyMonitorStop(bsp_qos_unit_t unit);
void      R_BSP_QoSLatencyMonitorCountClear(bsp_qos_unit_t unit);

#endif                                 /* BSP_FEATURE_QOS_IS_AVAILABLE */

/** @} (end addtogroup BSP_QOS) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif

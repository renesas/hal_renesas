/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef _BSP_QOS_DRIVER_H_
#define _BSP_QOS_DRIVER_H_

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#if (BSP_FEATURE_QOS_IS_AVAILABLE)

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2Bx
 * @defgroup BSP_QOS BSP QoS
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

 #define BSP_QOS_INTQOSnMON0_BASE    (0xFF090850U)
 #define BSP_QOS_INTQOSnMON1_BASE    (0xFF090854U)

 #define BSP_QOS_INTQOSnMON0(n)    *(volatile uint32_t *) (BSP_QOS_INTQOSnMON0_BASE + (n * 8U))
 #define BSP_QOS_INTQOSnMON1(n)    *(volatile uint32_t *) (BSP_QOS_INTQOSnMON1_BASE + (n * 8U))

/** IRQ lowest priority */
 #define BSP_QOS_LOWEST_PRIORITY           (63U)

/** Offset of INTQOSnMON1 bit's name */
 #define BSP_QOS_INTQOSnMON1_BIT_OFFSET    (32U)

/** Macro used to get the interrupt port for QoS isr function */
 #define BSP_QOS_GET_INTERRUPT_PORT(irq, port)        \
    if (irq >= INTQOS0_0_IRQn)                        \
    {                                                 \
        port = (uint8_t) (irq - INTQOS0_0_IRQn);      \
    }                                                 \
    else                                              \
    {                                                 \
        port = (uint8_t) (irq - INTQOS3_0_IRQn) + 3U; \
    }

/** SPID default value for QoS unit */
 #define BSP_QOS_CPU0_SPID_DEFAULT_VALUE        (0)
 #define BSP_QOS_CPU1_SPID_DEFAULT_VALUE        (1)
 #define BSP_QOS_CPU2_SPID_DEFAULT_VALUE        (2)
 #define BSP_QOS_CPU3_SPID_DEFAULT_VALUE        (3)
 #define BSP_QOS_CPU4_SPID_DEFAULT_VALUE        (4)
 #define BSP_QOS_CPU5_SPID_DEFAULT_VALUE        (5)
 #define BSP_QOS_GTM_SPID_DEFAULT_VALUE         (10)
 #define BSP_QOS_R_SWITCH_SPID_DEFAULT_VALUE    (13)
 #define BSP_QOS_ACEU0_SPID_DEFAULT_VALUE       (14)
 #define BSP_QOS_ACEU1_SPID_DEFAULT_VALUE       (15)
 #define BSP_QOS_RHSIF1_SPID_DEFAULT_VALUE      (18)
 #define BSP_QOS_RHSIF0_SPID_DEFAULT_VALUE      (19)
 #define BSP_QOS_FLXA0_SPID_DEFAULT_VALUE       (23)
 #define BSP_QOS_ETND1_SPID_DEFAULT_VALUE       (24)
 #define BSP_QOS_ETND0_SPID_DEFAULT_VALUE       (25)
 #define BSP_QOS_ICUMHB_SPID_DEFAULT_VALUE      (26)
 #define BSP_QOS_DMAC1_SPID_DEFAULT_VALUE       (27)
 #define BSP_QOS_DMAC0_SPID_DEFAULT_VALUE       (28)
 #define BSP_QOS_DTS_SPID_DEFAULT_VALUE         (29)
 #define BSP_QOS_DFP_SPID_DEFAULT_VALUE         (30)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** QoS Unit */
typedef enum e_bsp_qos_unit
{
    BSP_QOS_UNIT_FL_PE0CL0,            ///< CPU0 Code Flash bus
    BSP_QOS_UNIT_VC_PE0CL0,            ///< CPU0 Peripheral bus
    BSP_QOS_UNIT_MB_PE0CL0,            ///< CPU0 Cluster RAM bus
    BSP_QOS_UNIT_FL_PE1CL0,            ///< CPU1 Code Flash bus
    BSP_QOS_UNIT_VC_PE1CL0,            ///< CPU1 Peripheral bus
    BSP_QOS_UNIT_MB_PE1CL0,            ///< CPU1 Cluster RAM bus
    BSP_QOS_UNIT_FL_PE0CL1,            ///< CPU2 Code Flash bus
    BSP_QOS_UNIT_VC_PE0CL1,            ///< CPU2 Peripheral bus
    BSP_QOS_UNIT_MB_PE0CL1,            ///< CPU2 Cluster RAM bus
    BSP_QOS_UNIT_FL_PE1CL1,            ///< CPU3 Code Flash bus
    BSP_QOS_UNIT_VC_PE1CL1,            ///< CPU3 Peripheral bus
    BSP_QOS_UNIT_MB_PE1CL1,            ///< CPU3 Cluster RAM bus
 #if (BSP_FEATURE_QOS_PE0CL2_UNIT_AVAILABLE)
    BSP_QOS_UNIT_FL_PE0CL2,            ///< CPU4 Code Flash bus
    BSP_QOS_UNIT_VC_PE0CL2,            ///< CPU4 Peripheral bus
    BSP_QOS_UNIT_MB_PE0CL2,            ///< CPU4 Cluster RAM bus
 #endif
 #if (BSP_FEATURE_QOS_PE1CL2_UNIT_AVAILABLE)
    BSP_QOS_UNIT_FL_PE1CL2,            ///< CPU5 Code Flash bus
    BSP_QOS_UNIT_VC_PE1CL2,            ///< CPU5 Peripheral bus
    BSP_QOS_UNIT_MB_PE1CL2,            ///< CPU5 Cluster RAM bus
 #endif
    BSP_QOS_UNIT_DTS,                  ///< DTS AXI bus
    BSP_QOS_UNIT_SDMAC0,               ///< sDMAC0 AXI bus
    BSP_QOS_UNIT_SDMAC1,               ///< sDMAC1 AXI bus
    BSP_QOS_UNIT_FL_ICUM,              ///< ICUM Code Flash bus
    BSP_QOS_UNIT_SYS_ICUM,             ///< ICUM System bus
 #if (BSP_FEATURE_QOS_ACEU0_UNIT_AVAILABLE)
    BSP_QOS_UNIT_ACEU0,                ///< ACEU0 AXI bus
 #endif
 #if (BSP_FEATURE_QOS_ACEU1_UNIT_AVAILABLE)
    BSP_QOS_UNIT_ACEU1,                ///< ACEU1 AXI bus
 #endif
    BSP_QOS_UNIT_FL_DFP,               ///< DFP Code Flash bus
    BSP_QOS_UNIT_SYS_DFP,              ///< DFP System bus
    BSP_QOS_UNIT_GTM,                  ///< GTM AXI bus
 #if (BSP_FEATURE_QOS_RHSIF0_UNIT_AVAILABLE)
    BSP_QOS_UNIT_RHSIF0,               ///< RHSIF0 AXI bus
 #endif
 #if (BSP_FEATURE_QOS_RHSIF1_UNIT_AVAILABLE)
    BSP_QOS_UNIT_RHSIF1,               ///< RHSIF1 AXI bus
 #endif
    BSP_QOS_UNIT_FLXA0,                ///< FLXA0 AXI bus
 #if (BSP_FEATURE_QOS_ETHER0_UNIT_AVAILABLE)
    BSP_QOS_UNIT_ETHER0,               ///< Ether 0 AXI bus
 #endif
 #if (BSP_FEATURE_QOS_ETHER1_UNIT_AVAILABLE)
    BSP_QOS_UNIT_ETHER1,               ///< Ether 1 AXI bus
 #endif
 #if (BSP_FEATURE_QOS_R_SWITCH_UNIT_AVAILABLE)
    BSP_QOS_UNIT_R_SWITCH,             ///< R_SWITCH AXI bus
 #endif
    BSP_QOS_UNIT_NUM                   ///< End of QoS Unit
} bsp_qos_unit_t;

/** Bandwidth Regulator Mode */
typedef enum e_bsp_bandwidth_regulator_mode
{
    BSP_BANDWIDTH_REGULATOR_MODE_DISABLE   = 0U, ///< Bandwidth Regulator Disable
    BSP_BANDWIDTH_REGULATOR_MODE_INTERVAL  = 2U, ///< Bandwidth Regulator Interval mode
    BSP_BANDWIDTH_REGULATOR_MODE_THRESHOLD = 3U  ///< Bandwidth Regulator Threshold mode
} bsp_bandwidth_regulator_mode_t;

/** QoS interrupt index in Interrupt monitor Register */
typedef enum e_bsp_qos_irq_index
{
    BSP_QOS_IRQ_INDEX_0  = 0U,         ///< QoS Interrupt index 0
    BSP_QOS_IRQ_INDEX_1  = 1U,         ///< QoS Interrupt index 1
    BSP_QOS_IRQ_INDEX_2  = 2U,         ///< QoS Interrupt index 2
    BSP_QOS_IRQ_INDEX_3  = 3U,         ///< QoS Interrupt index 3
    BSP_QOS_IRQ_INDEX_4  = 4U,         ///< QoS Interrupt index 4
    BSP_QOS_IRQ_INDEX_5  = 5U,         ///< QoS Interrupt index 5
    BSP_QOS_IRQ_INDEX_6  = 6U,         ///< QoS Interrupt index 6
    BSP_QOS_IRQ_INDEX_7  = 7U,         ///< QoS Interrupt index 7
    BSP_QOS_IRQ_INDEX_8  = 8U,         ///< QoS Interrupt index 8
    BSP_QOS_IRQ_INDEX_9  = 9U,         ///< QoS Interrupt index 9
    BSP_QOS_IRQ_INDEX_10 = 10U,        ///< QoS Interrupt index 10
    BSP_QOS_IRQ_INDEX_11 = 11U,        ///< QoS Interrupt index 11
 #if (BSP_FEATURE_QOS_PE0CL2_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_12 = 12U,        ///< QoS Interrupt index 12
    BSP_QOS_IRQ_INDEX_13 = 13U,        ///< QoS Interrupt index 13
    BSP_QOS_IRQ_INDEX_14 = 14U,        ///< QoS Interrupt index 14
 #endif
 #if (BSP_FEATURE_QOS_PE1CL2_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_15 = 15U,        ///< QoS Interrupt index 15
    BSP_QOS_IRQ_INDEX_16 = 16U,        ///< QoS Interrupt index 16
    BSP_QOS_IRQ_INDEX_17 = 17U,        ///< QoS Interrupt index 17
 #endif
    BSP_QOS_IRQ_INDEX_24 = 24U,        ///< QoS Interrupt index 24
    BSP_QOS_IRQ_INDEX_25 = 25U,        ///< QoS Interrupt index 25
    BSP_QOS_IRQ_INDEX_26 = 26U,        ///< QoS Interrupt index 26
    BSP_QOS_IRQ_INDEX_27 = 27U,        ///< QoS Interrupt index 27
    BSP_QOS_IRQ_INDEX_28 = 28U,        ///< QoS Interrupt index 28
 #if (BSP_FEATURE_QOS_ACEU0_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_29 = 29U,        ///< QoS Interrupt index 29
 #endif
 #if (BSP_FEATURE_QOS_ACEU1_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_30 = 30U,        ///< QoS Interrupt index 30
 #endif
    BSP_QOS_IRQ_INDEX_31 = 31U,        ///< QoS Interrupt index 31
    BSP_QOS_IRQ_INDEX_32 = 32U,        ///< QoS Interrupt index 32
    BSP_QOS_IRQ_INDEX_33 = 33U,        ///< QoS Interrupt index 33
 #if (BSP_FEATURE_QOS_RHSIF0_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_34 = 34U,        ///< QoS Interrupt index 34
 #endif
 #if (BSP_FEATURE_QOS_RHSIF1_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_35 = 35U,        ///< QoS Interrupt index 35
 #endif
    BSP_QOS_IRQ_INDEX_36 = 36U,        ///< QoS Interrupt index 36
 #if (BSP_FEATURE_QOS_ETHER0_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_38 = 38U,        ///< QoS Interrupt index 38
 #endif
 #if (BSP_FEATURE_QOS_ETHER1_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_39 = 39U,        ///< QoS Interrupt index 39
 #endif
 #if (BSP_FEATURE_QOS_R_SWITCH_UNIT_AVAILABLE)
    BSP_QOS_IRQ_INDEX_40 = 40U,        ///< QoS Interrupt index 40
 #endif
    BSP_QOS_IRQ_INDEX_MAX              ///< End of QoS Interrupt index
} bsp_qos_irq_index_t;

/** Bandwidth Regulator data structure */
typedef struct st_bsp_bandwidth_regulator_cfg
{
    bsp_bandwidth_regulator_mode_t mode; ///< Bandwidth Regulator mode
    bsp_qos_unit_t                 unit; ///< QoS unit
    uint16_t period;                     ///< Bandwidth Regulator period
    uint16_t read_access;                ///< Bandwidth Regulator reading access threshold
    uint16_t write_access;               ///< Bandwidth Regulator writing access threshold
} bsp_bandwidth_regulator_cfg_t;

/** Latency Monitor data structure */
typedef struct st_bsp_latency_monitor_cfg
{
    uint32_t minimum_address_0;                      ///< Latency Monitor minimum address 0
    uint32_t maximum_address_0;                      ///< Latency Monitor maximum address 0
    uint32_t minimum_address_1;                      ///< Latency Monitor minimum address 1
    uint32_t maximum_address_1;                      ///< Latency Monitor maximum address 1
    uint32_t spid;                                   ///< Latency Monitor SPID
    uint16_t cycle_read_latency_threshold;           ///< Latency Monitor cycle read latency threshold
    uint16_t cycle_write_latency_threshold;          ///< Latency Monitor cycle write latency threshold
    uint16_t read_latency_overflow_count_threshold;  ///< Latency Monitor read latency overflow count threshold
    uint16_t write_latency_overflow_count_threshold; ///< Latency Monitor write latency overflow count threshold
    uint8_t  channel;                                ///< Latency Monitor channel
    uint8_t  interrupt_port_bind_bit;                ///< Latency Monitor Interrupt Bind bit
    uint8_t  ipl;                                    ///< Latency Monitor Interrupt Priority
    void (* p_callback)(void);                       ///< Latency Monitor Interrupt Callback
    bsp_qos_unit_t unit;                             ///< QoS unit
} bsp_latency_monitor_cfg_t;

typedef struct st_bsp_qos_cfg
{
    const bsp_bandwidth_regulator_cfg_t bandwidth_regulator_cfg;
    const bsp_latency_monitor_cfg_t     latency_monitor_cfg;
} bsp_qos_cfg_t;

/***********************************************************************************************************************
 * Exported global variable (to be accessed by other files)
 **********************************************************************************************************************/
extern R_QOSCNT_FL_PE0CL0_Type   * g_qos_info_table[BSP_QOS_UNIT_NUM];
extern const bsp_qos_cfg_t         g_qos_config_table[BSP_QOS_UNIT_NUM];
extern bsp_latency_monitor_cfg_t * g_qos_context_table[BSP_QOS_IRQ_INDEX_MAX];
extern const bsp_qos_irq_index_t   g_qos_irq_index_table[BSP_QOS_IRQ_INDEX_MAX];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif                                 /* BSP_FEATURE_QOS_IS_AVAILABLE */

/** @} (end addtogroup BSP_QOS) */

#endif

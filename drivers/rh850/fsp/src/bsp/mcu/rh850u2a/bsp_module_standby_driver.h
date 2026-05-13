/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_MODULE_STANDBY_DRIVER_H
#define BSP_MODULE_STANDBY_DRIVER_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_MODULE_STANDBY_MSR_RSCFD_DEFAULT_VALUE       (0x00000003UL) ///< Default MSR value for RSCFD
#define BSP_MODULE_STANDBY_MSR_FLXA_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for FLXA
#define BSP_MODULE_STANDBY_MSR_GTM_DEFAULT_VALUE         (0x00000001UL) ///< Default MSR value for GTM
#define BSP_MODULE_STANDBY_MSR_ETNB_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for ETNB
#define BSP_MODULE_STANDBY_MSR_RSENT_DEFAULT_VALUE       (0x000000FFUL) ///< Default MSR value for RSENT
#define BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE        (0x000003FFUL) ///< Default MSR value for MSPI
#define BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE       (0x00FFFFFFUL) ///< Default MSR value for RLIN3
#define BSP_MODULE_STANDBY_MSR_ADCJ_ISO_DEFAULT_VALUE    (0x00000003UL) ///< Default MSR value for ADCJ_ISO
#define BSP_MODULE_STANDBY_MSR_CXPI_DEFAULT_VALUE        (0x0000000FUL) ///< Default MSR value for CXPI
#define BSP_MODULE_STANDBY_MSR_MMCA_DEFAULT_VALUE        (0x00000001UL) ///< Default MSR value for MMCA
#define BSP_MODULE_STANDBY_MSR_ENCA_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for ENCA
#define BSP_MODULE_STANDBY_MSR_PSI5_DEFAULT_VALUE        (0x0000000FUL) ///< Default MSR value for PSI5
#define BSP_MODULE_STANDBY_MSR_PSI5S_DEFAULT_VALUE       (0x00000003UL) ///< Default MSR value for PSI5S
#define BSP_MODULE_STANDBY_MSR_PWMD_DEFAULT_VALUE        (0x00000001UL) ///< Default MSR value for PWMD
#define BSP_MODULE_STANDBY_MSR_RHSIF_DEFAULT_VALUE       (0x00000001UL) ///< Default MSR value for RHSIF
#define BSP_MODULE_STANDBY_MSR_RIIC_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for RIIC
#define BSP_MODULE_STANDBY_MSR_SCI3_DEFAULT_VALUE        (0x00000007UL) ///< Default MSR value for SCI3
#define BSP_MODULE_STANDBY_MSR_TAPA_DEFAULT_VALUE        (0x0000000FUL) ///< Default MSR value for TAPA
#define BSP_MODULE_STANDBY_MSR_TAUD_DEFAULT_VALUE        (0x00000007UL) ///< Default MSR value for TAUD
#define BSP_MODULE_STANDBY_MSR_TAUJ_ISO_DEFAULT_VALUE    (0x00000003UL) ///< Default MSR value for TAUJ_ISO
#define BSP_MODULE_STANDBY_MSR_TPBA_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for TPBA
#define BSP_MODULE_STANDBY_MSR_TSG3_DEFAULT_VALUE        (0x00000003UL) ///< Default MSR value for TSG3
#define BSP_MODULE_STANDBY_MSR_OSTM_DEFAULT_VALUE        (0x000003FFUL) ///< Default MSR value for OSTM
#define BSP_MODULE_STANDBY_MSR_ADCJ_AWO_DEFAULT_VALUE    (0x00000001UL) ///< Default MSR value for ADCJ_AWO
#define BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE        (0x00000001UL) ///< Default MSR value for RTCA
#define BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE    (0x00000003UL) ///< Default MSR value for TAUJ_AWO
#define BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE    (0x00000000UL) ///< Default MSR value for WDTB_AWO

#define BSP_MODULE_MSR_STPMSK_Pos                        (31U)          ///< Bit position of STPMSK in MSR (bit 31)

#define BSP_PRV_MSRKCPROT_RESET                          0x00000000     ///< MSRKCPROT register reset value

/* Wake-up factor index ranges (for enum mapping) */
#define BSP_STANDBY_WAKE_UP_FACTOR_A0_END                32U            ///< End index of WK Factor A0
#define BSP_STANDBY_WAKE_UP_FACTOR_A1_END                64U            ///< End index of WK Factor A1
#define BSP_STANDBY_WAKE_UP_FACTOR_A2_END                96U            ///< End index of WK Factor A2
#define BSP_STANDBY_WAKE_UP_FACTOR_I0_END                128U           ///< End index of WK Factor I0
#define BSP_STANDBY_WAKE_UP_FACTOR_I1_END                160U           ///< End index of WK Factor I1

#define BSP_STANDBY_WAKE_UP_FACTOR_A0_START              0U             ///< Start index of WK Factor A0
#define BSP_STANDBY_WAKE_UP_FACTOR_A1_START              32U            ///< Start index of WK Factor A1
#define BSP_STANDBY_WAKE_UP_FACTOR_A2_START              64U            ///< Start index of WK Factor A2
#define BSP_STANDBY_WAKE_UP_FACTOR_I0_START              96U            ///< Start index of WK Factor I0
#define BSP_STANDBY_WAKE_UP_FACTOR_I1_START              128U           ///< Start index of WK Factor I1

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2AX
 * @defgroup BSP_MODULE_STANDBY_U2AX BSP Module Standby
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Standby Module */
typedef enum e_bsp_module_standby
{
    BSP_MODULE_STANDBY_MSR_RSCFD,      ///< Standby MSR for RSCFD
    BSP_MODULE_STANDBY_MSR_FLXA,       ///< Standby MSR for FLXA
    BSP_MODULE_STANDBY_MSR_GTM,        ///< Standby MSR for GTM
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_ETNB,       ///< Standby MSR for ETNB
#endif
    BSP_MODULE_STANDBY_MSR_RSENT,      ///< Standby MSR for RSENT
    BSP_MODULE_STANDBY_MSR_MSPI,       ///< Standby MSR for MSPI
    BSP_MODULE_STANDBY_MSR_RLIN3,      ///< Standby MSR for RLIN3
    BSP_MODULE_STANDBY_MSR_ADCJ_ISO,   ///< Standby MSR for ADCJ_ISO
#if (BSP_FEATURE_CXPI_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_CXPI,       ///< Standby MSR for CXPI
#endif
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_MMCA,       ///< Standby MSR for MMCA
#endif
#if (BSP_FEATURE_ENCA_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_ENCA,       ///< Standby MSR for ENCA
#endif
    BSP_MODULE_STANDBY_MSR_PSI5,       ///< Standby MSR for PSI5
    BSP_MODULE_STANDBY_MSR_PSI5S,      ///< Standby MSR for PSI5S
    BSP_MODULE_STANDBY_MSR_PWMD,       ///< Standby MSR for PWMD
#if (BSP_FEATURE_RHSIF_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_RHSIF,      ///< Standby MSR for RHSIF
#endif
    BSP_MODULE_STANDBY_MSR_RIIC,       ///< Standby MSR for RIIC
#if (BSP_FEATURE_SCI3_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_SCI3,       ///< Standby MSR for SCI3
#endif
    BSP_MODULE_STANDBY_MSR_TAPA,       ///< Standby MSR for TAPA
    BSP_MODULE_STANDBY_MSR_TAUD,       ///< Standby MSR for TAUD
    BSP_MODULE_STANDBY_MSR_TAUJ_ISO,   ///< Standby MSR for TAUJ_ISO
    BSP_MODULE_STANDBY_MSR_TPBA,       ///< Standby MSR for TPBA
    BSP_MODULE_STANDBY_MSR_TSG3,       ///< Standby MSR for TSG3
    BSP_MODULE_STANDBY_MSR_OSTM,       ///< Standby MSR for OSTM
    BSP_MODULE_STANDBY_MSR_ADCJ_AWO,   ///< Standby MSR for ADCJ_AWO
    BSP_MODULE_STANDBY_MSR_RTCA,       ///< Standby MSR for RTCA
    BSP_MODULE_STANDBY_MSR_TAUJ_AWO,   ///< Standby MSR for TAUJ_AWO
    BSP_MODULE_STANDBY_MSR_WDTB_AWO,   ///< Standby MSR for WDTB_AWO
    BSP_MODULE_STANDBY_TOTAL_INDEX_END ///< End of standby MSR list
} bsp_module_standby_t;

typedef enum e_bsp_module_stopmode
{
    BSP_MODULE_OPERATION_MODE_RSCFD    = BSP_MODULE_STANDBY_MSR_RSCFD,    ///< Operation mode for RSCFD in STOP mode
    BSP_MODULE_OPERATION_MODE_RLIN3    = BSP_MODULE_STANDBY_MSR_RLIN3,    ///< Operation mode for RLIN3 in STOP mode
    BSP_MODULE_OPERATION_MODE_ADCJ_AWO = BSP_MODULE_STANDBY_MSR_ADCJ_AWO, ///< Operation mode for ADCJ_AWO in STOP mode
    BSP_MODULE_OPERATION_MODE_RTCA     = BSP_MODULE_STANDBY_MSR_RTCA,     ///< Operation mode for RTCA in STOP mode
    BSP_MODULE_OPERATION_MODE_TAUJ_AWO = BSP_MODULE_STANDBY_MSR_TAUJ_AWO, ///< Operation mode for TAUJ_AWO in STOP mode
    BSP_MODULE_OPERATION_MODE_WDTB_AWO = BSP_MODULE_STANDBY_MSR_WDTB_AWO, ///< Operation mode for WDTB_AWO in STOP mode
} bsp_module_stopmode_t;

/** Chip Standby Mode */
typedef enum e_bsp_module_stopmode_config
{
    BSP_MODULE_CLOCK_STOPPED  = 0U,    ///< Clock stopped (module disabled)
    BSP_MODULE_CLOCK_SUPPLIED = 1U     ///< Clock supplied (module enabled)
} bsp_module_stopmode_config_t;

/** Wakup Factor for DeepStop */
typedef enum e_bsp_wk_factor
{
    BSP_WK_FACTOR_NONE = 255U,                                               ///< None

    /* Wake-up factor for register A0 */
    BSP_WK_FACTOR_TNMI     = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 0U,       ///< A0: TNMI
    BSP_WK_FACTOR_INTWDTBA = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 5U,       ///< A0: INTWDTBA
    BSP_WK_FACTOR_INTP0    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 6U,       ///< A0: INTP0
    BSP_WK_FACTOR_INTP1    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 7U,       ///< A0: INTP1
    BSP_WK_FACTOR_INTP2    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 8U,       ///< A0: INTP2
    BSP_WK_FACTOR_INTP3    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 9U,       ///< A0: INTP3
    BSP_WK_FACTOR_INTP4    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 10U,      ///< A0: INTP4
    BSP_WK_FACTOR_INTP5    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 11U,      ///< A0: INTP5
    BSP_WK_FACTOR_INTP6    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 12U,      ///< A0: INTP6
    BSP_WK_FACTOR_INTP7    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 13U,      ///< A0: INTP7
    BSP_WK_FACTOR_INTP8    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 14U,      ///< A0: INTP8
    BSP_WK_FACTOR_INTP9    = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 15U,      ///< A0: INTP9
    BSP_WK_FACTOR_INTP10   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 16U,      ///< A0: INTP10
    BSP_WK_FACTOR_INTP11   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 17U,      ///< A0: INTP11
    BSP_WK_FACTOR_INTP12   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 18U,      ///< A0: INTP12
    BSP_WK_FACTOR_INTP13   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 19U,      ///< A0: INTP13
    BSP_WK_FACTOR_INTP14   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 20U,      ///< A0: INTP14
    BSP_WK_FACTOR_INTP15   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 21U,      ///< A0: INTP15

    /* Wake-up factor for register A1 */
    BSP_WK_FACTOR_INTP16 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 0U,         ///< A1: INTP16
    BSP_WK_FACTOR_INTP17 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 1U,         ///< A1: INTP17
    BSP_WK_FACTOR_INTP18 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 2U,         ///< A1: INTP18
    BSP_WK_FACTOR_INTP19 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 3U,         ///< A1: INTP19
    BSP_WK_FACTOR_INTP20 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 4U,         ///< A1: INTP20
    BSP_WK_FACTOR_INTP21 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 5U,         ///< A1: INTP21
    BSP_WK_FACTOR_INTP22 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 6U,         ///< A1: INTP22
    BSP_WK_FACTOR_INTP23 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 7U,         ///< A1: INTP23
    BSP_WK_FACTOR_INTP24 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 8U,         ///< A1: INTP24
    BSP_WK_FACTOR_INTP25 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 9U,         ///< A1: INTP25
    BSP_WK_FACTOR_INTP26 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 10U,        ///< A1: INTP26
    BSP_WK_FACTOR_INTP27 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 11U,        ///< A1: INTP27
    BSP_WK_FACTOR_INTP28 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 12U,        ///< A1: INTP28
    BSP_WK_FACTOR_INTP29 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 13U,        ///< A1: INTP29
    BSP_WK_FACTOR_INTP30 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 14U,        ///< A1: INTP30
    BSP_WK_FACTOR_INTP31 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 15U,        ///< A1: INTP31
    BSP_WK_FACTOR_INTP32 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 16U,        ///< A1: INTP32
    BSP_WK_FACTOR_INTP33 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 17U,        ///< A1: INTP33
    BSP_WK_FACTOR_INTP34 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 18U,        ///< A1: INTP34
    BSP_WK_FACTOR_INTP35 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 19U,        ///< A1: INTP35
    BSP_WK_FACTOR_INTP36 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 20U,        ///< A1: INTP36
    BSP_WK_FACTOR_INTP37 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 21U,        ///< A1: INTP37
    BSP_WK_FACTOR_INTP38 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 22U,        ///< A1: INTP38
    BSP_WK_FACTOR_INTP39 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 23U,        ///< A1: INTP39

    /* Wake-up factor for register A2 */
    BSP_WK_FACTOR_WUTRG0     = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 0U,     ///< A2: WUTRG0
    BSP_WK_FACTOR_WUTRG1     = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 1U,     ///< A2: WUTRG1
    BSP_WK_FACTOR_INTDCUTDI  = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 2U,     ///< A2: INTDCUTDI
    BSP_WK_FACTOR_INTTAUJ2I0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 3U,     ///< A2: INTTAUJ2I0
    BSP_WK_FACTOR_INTTAUJ2I1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 4U,     ///< A2: INTTAUJ2I1
    BSP_WK_FACTOR_INTTAUJ2I2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 5U,     ///< A2: INTTAUJ2I2
    BSP_WK_FACTOR_INTTAUJ2I3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 6U,     ///< A2: INTTAUJ2I3
    BSP_WK_FACTOR_INTTAUJ3I0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 7U,     ///< A2: INTTAUJ3I0
    BSP_WK_FACTOR_INTTAUJ3I1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 8U,     ///< A2: INTTAUJ3I1
    BSP_WK_FACTOR_INTTAUJ3I2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 9U,     ///< A2: INTTAUJ3I2
    BSP_WK_FACTOR_INTTAUJ3I3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 10U,    ///< A2: INTTAUJ3I3
    BSP_WK_FACTOR_INTRTCA01S = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 11U,    ///< A2: INTRTCA01S
    BSP_WK_FACTOR_INTRTCA0AL = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 12U,    ///< A2: INTRTCA0AL
    BSP_WK_FACTOR_INTRTCA0R  = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 13U,    ///< A2: INTRTCA0R
    BSP_WK_FACTOR_INTADCJ2I0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 14U,    ///< A2: INTADCJ2I0
    BSP_WK_FACTOR_INTADCJ2I1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 15U,    ///< A2: INTADCJ2I1
    BSP_WK_FACTOR_INTADCJ2I2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 16U,    ///< A2: INTADCJ2I2
    BSP_WK_FACTOR_INTADCJ2I3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 17U,    ///< A2: INTADCJ2I3
    BSP_WK_FACTOR_INTADCJ2I4 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 18U,    ///< A2: INTADCJ2I4

    /* Wake-up factor for register I0 */
    BSP_WK_FACTOR_INTRCANGRECC0 = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 0U,  ///< I0: INTRCANGRECC0
    BSP_WK_FACTOR_INTRCAN0REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 1U,  ///< I0: INTRCAN0REC
    BSP_WK_FACTOR_INTRCAN1REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 2U,  ///< I0: INTRCAN1REC
    BSP_WK_FACTOR_INTRCAN2REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 3U,  ///< I0: INTRCAN2REC
    BSP_WK_FACTOR_INTRCAN3REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 4U,  ///< I0: INTRCAN3REC
    BSP_WK_FACTOR_INTRCAN4REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 5U,  ///< I0: INTRCAN4REC
    BSP_WK_FACTOR_INTRCAN5REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 6U,  ///< I0: INTRCAN5REC
    BSP_WK_FACTOR_INTRCAN6REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 7U,  ///< I0: INTRCAN6REC
    BSP_WK_FACTOR_INTRCAN7REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 8U,  ///< I0: INTRCAN7REC
    BSP_WK_FACTOR_INTRCANGRECC1 = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 9U,  ///< I0: INTRCANGRECC1
    BSP_WK_FACTOR_INTRCAN8REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 10U, ///< I0: INTRCAN8REC
    BSP_WK_FACTOR_INTRCAN9REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 11U, ///< I0: INTRCAN9REC
    BSP_WK_FACTOR_INTRCAN10REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 12U, ///< I0: INTRCAN10REC
    BSP_WK_FACTOR_INTRCAN11REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 13U, ///< I0: INTRCAN11REC
    BSP_WK_FACTOR_INTRCAN12REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 14U, ///< I0: INTRCAN12REC
    BSP_WK_FACTOR_INTRCAN13REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 15U, ///< I0: INTRCAN13REC
    BSP_WK_FACTOR_INTRCAN14REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 16U, ///< I0: INTRCAN14REC
    BSP_WK_FACTOR_INTRCAN15REC  = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 17U, ///< I0: INTRCAN15REC

    /* Wake-up factor for register I1 */
    BSP_WK_FACTOR_INTRLIN30  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 0U,     ///< I1: INTRLIN30
    BSP_WK_FACTOR_INTRLIN31  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 1U,     ///< I1: INTRLIN31
    BSP_WK_FACTOR_INTRLIN32  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 2U,     ///< I1: INTRLIN32
    BSP_WK_FACTOR_INTRLIN33  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 3U,     ///< I1: INTRLIN33
    BSP_WK_FACTOR_INTRLIN34  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 4U,     ///< I1: INTRLIN34
    BSP_WK_FACTOR_INTRLIN35  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 5U,     ///< I1: INTRLIN35
    BSP_WK_FACTOR_INTRLIN36  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 6U,     ///< I1: INTRLIN36
    BSP_WK_FACTOR_INTRLIN37  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 7U,     ///< I1: INTRLIN37
    BSP_WK_FACTOR_INTRLIN38  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 8U,     ///< I1: INTRLIN38
    BSP_WK_FACTOR_INTRLIN39  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 9U,     ///< I1: INTRLIN39
    BSP_WK_FACTOR_INTRLIN310 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 10U,    ///< I1: INTRLIN310
    BSP_WK_FACTOR_INTRLIN311 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 11U,    ///< I1: INTRLIN311
    BSP_WK_FACTOR_INTRLIN312 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 12U,    ///< I1: INTRLIN312
    BSP_WK_FACTOR_INTRLIN313 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 13U,    ///< I1: INTRLIN313
    BSP_WK_FACTOR_INTRLIN314 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 14U,    ///< I1: INTRLIN314
    BSP_WK_FACTOR_INTRLIN315 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 15U,    ///< I1: INTRLIN315
    BSP_WK_FACTOR_INTRLIN316 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 16U,    ///< I1: INTRLIN316
    BSP_WK_FACTOR_INTRLIN317 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 17U,    ///< I1: INTRLIN317
    BSP_WK_FACTOR_INTRLIN318 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 18U,    ///< I1: INTRLIN318
    BSP_WK_FACTOR_INTRLIN319 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 19U,    ///< I1: INTRLIN319
    BSP_WK_FACTOR_INTRLIN320 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 20U,    ///< I1: INTRLIN320
    BSP_WK_FACTOR_INTRLIN321 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 21U,    ///< I1: INTRLIN321
    BSP_WK_FACTOR_INTRLIN322 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 22U,    ///< I1: INTRLIN322
    BSP_WK_FACTOR_INTRLIN323 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 23U,    ///< I1: INTRLIN323
} bsp_wk_factor_t;

/** IO Hold */
typedef enum e_bsp_io_hold
{
    BSP_IO_HOLD_P02 = (2U),            ///< IO hold for P02
    BSP_IO_HOLD_P03 = (3U),            ///< IO hold for P03
    BSP_IO_HOLD_P04 = (4U),            ///< IO hold for P04
    BSP_IO_HOLD_P05 = (5U),            ///< IO hold for P05
    BSP_IO_HOLD_P09 = (9U),            ///< IO hold for P09
    BSP_IO_HOLD_P10 = (10U),           ///< IO hold for P10
    BSP_IO_HOLD_P11 = (11U),           ///< IO hold for P11
    BSP_IO_HOLD_P12 = (12U),           ///< IO hold for P12
    BSP_IO_HOLD_P17 = (17U),           ///< IO hold for P17
    BSP_IO_HOLD_P18 = (18U),           ///< IO hold for P18
    BSP_IO_HOLD_P19 = (19U),           ///< IO hold for P19
    BSP_IO_HOLD_P20 = (20U),           ///< IO hold for P20
    BSP_IO_HOLD_P21 = (21U),           ///< IO hold for P21
    BSP_IO_HOLD_P22 = (22U),           ///< IO hold for P22
    BSP_IO_HOLD_P23 = (23U),           ///< IO hold for P23
    BSP_IO_HOLD_P24 = (24U),           ///< IO hold for P24

    BSP_IO_HOLD_AP0 = (32U),           ///< IO hold for AP0
    BSP_IO_HOLD_AP1 = (33U),           ///< IO hold for AP1
    BSP_IO_HOLD_AP2 = (34U),           ///< IO hold for AP2
    BSP_IO_HOLD_AP3 = (35U),           ///< IO hold for AP3

    BSP_IO_HOLD_CLEAR_ALL = 0xFFU      ///< Clear all IO hold settings
} bsp_io_hold_t;

/** Wake-up register */
typedef enum e_bsp_wk_reg
{
    BSP_WK_REG_WUFMSK_A0,              ///< Wake-up mask register A0
    BSP_WK_REG_WUFMSK_A1,              ///< Wake-up mask register A1
    BSP_WK_REG_WUFMSK_A2,              ///< Wake-up mask register A2
    BSP_WK_REG_WUFMSK_I0,              ///< Wake-up mask register I0
    BSP_WK_REG_WUFMSK_I1,              ///< Wake-up mask register I1

    BSP_WK_REG_WUFC_A0,                ///< Wake-up factor clear register A0
    BSP_WK_REG_WUFC_A1,                ///< Wake-up factor clear register A1
    BSP_WK_REG_WUFC_A2,                ///< Wake-up factor clear register A2
    BSP_WK_REG_WUFC_I0,                ///< Wake-up factor clear register I0
    BSP_WK_REG_WUFC_I1,                ///< Wake-up factor clear register I1

    BSP_WK_REG_WUF_A0,                 ///< Wake-up factor register A0
    BSP_WK_REG_WUF_A1,                 ///< Wake-up factor register A1
    BSP_WK_REG_WUF_A2,                 ///< Wake-up factor register A2
    BSP_WK_REG_WUF_I0,                 ///< Wake-up factor register I0
    BSP_WK_REG_WUF_I1                  ///< Wake-up factor register I1
} bsp_wk_reg_t;

typedef struct st_bsp_module_standby_info
{
    volatile uint32_t * msr_reg_address;   ///< Guard channel, bit position indicate the channel.
    const uint32_t      default_value;     ///< Default value of module.
    const uint32_t      initial_cfg_value; ///< Configured value in MDF used to enable Guard for all module.
} bsp_module_standby_info_t;

/* Assign Wake-up Factor for Run Mode and Cyclic RUN Mode */
typedef struct st_bsp_wk_factor_cfg
{
    bsp_wk_factor_t * wk_run;                     ///< Wake up Factors for RUN Mode
    uint8_t           num_of_wk_run_factor;       ///< Number of wake up factors for RUN Mode
    bsp_wk_factor_t * wk_cyclicrun;               ///< Wake up Factors for Cyclic RUN Mode
    uint8_t           num_of_wk_cyclicrun_factor; ///< Number of wake up factors for Cyclic RUN Mode
} bsp_wk_factor_cfg_t;

/** @} (end addtogroup BSP_MODULE_STANDBY) */

/***********************************************************************************************************************
 * Exported global variable (to be accessed by other files)
 **********************************************************************************************************************/
extern const bsp_module_standby_info_t g_msr_info_table[BSP_MODULE_STANDBY_TOTAL_INDEX_END];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif                                 /* BSP_MODULE_STANDBY_DRIVER_H */

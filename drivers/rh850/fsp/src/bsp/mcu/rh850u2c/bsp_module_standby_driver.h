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

#define BSP_MODULE_STANDBY_MSR_RSCFD_DEFAULT_VALUE       (0x00000003UL) ///< Standby default value for RSCFD
#define BSP_MODULE_STANDBY_MSR_FLXA_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for FLXA
#define BSP_MODULE_STANDBY_MSR_GTM_DEFAULT_VALUE         (0x00000001UL) ///< Standby default value for GTM
#define BSP_MODULE_STANDBY_MSR_ETND_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for ETND
#define BSP_MODULE_STANDBY_MSR_ETNF_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for ETNF
#define BSP_MODULE_STANDBY_MSR_RSENT_DEFAULT_VALUE       (0x000000FFUL) ///< Standby default value for RSENT
#define BSP_MODULE_STANDBY_MSR_MSPI_DEFAULT_VALUE        (0x000003FFUL) ///< Standby default value for MSPI
#define BSP_MODULE_STANDBY_MSR_RLIN3_DEFAULT_VALUE       (0x00FFFFFFUL) ///< Standby default value for RLIN3
#define BSP_MODULE_STANDBY_MSR_ADCK_ISO_DEFAULT_VALUE    (0x00000003UL) ///< Standby default value for ADCK_ISO
#define BSP_MODULE_STANDBY_MSR_CXPI_DEFAULT_VALUE        (0x0000000FUL) ///< Standby default value for CXPI
#define BSP_MODULE_STANDBY_MSR_MMCA_DEFAULT_VALUE        (0x00000001UL) ///< Standby default value for MMCA
#define BSP_MODULE_STANDBY_MSR_ENCA_DEFAULT_VALUE        (0x00000001UL) ///< Standby default value for ENCA
#define BSP_MODULE_STANDBY_MSR_PSI5_DEFAULT_VALUE        (0x00000023UL) ///< Standby default value for PSI5
#define BSP_MODULE_STANDBY_MSR_PSI5S_DEFAULT_VALUE       (0x00000001UL) ///< Standby default value for PSI5S
#define BSP_MODULE_STANDBY_MSR_PWMD_DEFAULT_VALUE        (0x00000001UL) ///< Standby default value for PWMD
#define BSP_MODULE_STANDBY_MSR_SFMA_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for SFMA
#define BSP_MODULE_STANDBY_MSR_TAPA_DEFAULT_VALUE        (0x0000000FUL) ///< Standby default value for TAPA
#define BSP_MODULE_STANDBY_MSR_TAUD_DEFAULT_VALUE        (0x00000007UL) ///< Standby default value for TAUD
#define BSP_MODULE_STANDBY_MSR_TAUJ_ISO_DEFAULT_VALUE    (0x00000003UL) ///< Standby default value for TAUJ_ISO
#define BSP_MODULE_STANDBY_MSR_TSG3_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for TSG3
#define BSP_MODULE_STANDBY_MSR_OSTM_DEFAULT_VALUE        (0x0000000FUL) ///< Standby default value for OSTM
#define BSP_MODULE_STANDBY_MSR_RI3C_DEFAULT_VALUE        (0x0000000FUL) ///< Standby default value for RI3C
#define BSP_MODULE_STANDBY_MSR_SSIF_DEFAULT_VALUE        (0x00000003UL) ///< Standby default value for SSIF
#define BSP_MODULE_STANDBY_MSR_ADCK_AWO_DEFAULT_VALUE    (0x00000001UL) ///< Standby default value for ADCK_AWO
#define BSP_MODULE_STANDBY_MSR_RTCA_DEFAULT_VALUE        (0x00000001UL) ///< Standby default value for RTCA
#define BSP_MODULE_STANDBY_MSR_TAUJ_AWO_DEFAULT_VALUE    (0x00000003UL) ///< Standby default value for TAUJ_AWO
#define BSP_MODULE_STANDBY_MSR_WDTB_AWO_DEFAULT_VALUE    (0x00000000UL) ///< Standby default value for WDTB_AWO

#define BSP_MODULE_MSR_STPMSK_Pos                        (31U)          ///< STPMSK bit position

#define BSP_PRV_MSRKCPROT_RESET                          0x00000000     ///< MSRKCPROT register reset value

/** Macro used for wake up enum */
#define BSP_STANDBY_WAKE_UP_FACTOR_A0_END                32U            ///< Wake-up factor A0 end index
#define BSP_STANDBY_WAKE_UP_FACTOR_A1_END                64U            ///< Wake-up factor A1 end index
#define BSP_STANDBY_WAKE_UP_FACTOR_A2_END                96U            ///< Wake-up factor A2 end index
#define BSP_STANDBY_WAKE_UP_FACTOR_I0_END                128U           ///< Wake-up factor I0 end index
#define BSP_STANDBY_WAKE_UP_FACTOR_I1_END                160U           ///< Wake-up factor I1 end index
#define BSP_STANDBY_WAKE_UP_FACTOR_I2_END                192U           ///< Wake-up factor I2 end index

#define BSP_STANDBY_WAKE_UP_FACTOR_A0_START              0U             ///< Wake-up factor A0 start index
#define BSP_STANDBY_WAKE_UP_FACTOR_A1_START              32U            ///< Wake-up factor A1 start index
#define BSP_STANDBY_WAKE_UP_FACTOR_A2_START              64U            ///< Wake-up factor A2 start index
#define BSP_STANDBY_WAKE_UP_FACTOR_I0_START              96U            ///< Wake-up factor I0 start index
#define BSP_STANDBY_WAKE_UP_FACTOR_I1_START              128U           ///< Wake-up factor I1 start index
#define BSP_STANDBY_WAKE_UP_FACTOR_I2_START              160U           ///< Wake-up factor I2 start index

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_MODULE_STANDBY_U2CX BSP Module Standby
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Standby control module list */
typedef enum e_bsp_module_standby
{
    BSP_MODULE_STANDBY_MSR_RSCFD,      ///< Standby module RSCFD
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_FLXA,       ///< Standby module FLXA
#endif /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE) */
    BSP_MODULE_STANDBY_MSR_GTM,        ///< Standby module GTM
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_ETND,       ///< Standby module ETND
#endif /* #if (BSP_FEATURE_ETND_IS_AVAILABLE) */
    BSP_MODULE_STANDBY_MSR_ETNF,       ///< Standby module ETNF
    BSP_MODULE_STANDBY_MSR_RSENT,      ///< Standby module RSENT
    BSP_MODULE_STANDBY_MSR_MSPI,       ///< Standby module MSPI
    BSP_MODULE_STANDBY_MSR_RLIN3,      ///< Standby module RLIN3
    BSP_MODULE_STANDBY_MSR_ADCK_ISO,   ///< Standby module ADCK_ISO
    BSP_MODULE_STANDBY_MSR_CXPI,       ///< Standby module CXPI
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_MMCA,       ///< Standby module MMCA
#endif /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */
    BSP_MODULE_STANDBY_MSR_ENCA,       ///< Standby module ENCA
    BSP_MODULE_STANDBY_MSR_PSI5,       ///< Standby module PSI5
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    BSP_MODULE_STANDBY_MSR_PSI5S,      ///< Standby module PSI5S
#endif /* #if (BSP_FEATURE_PSI5S_IS_AVAILABLE) */
    BSP_MODULE_STANDBY_MSR_PWMD,       ///< Standby module PWMD
    BSP_MODULE_STANDBY_MSR_SFMA,       ///< Standby module SFMA
    BSP_MODULE_STANDBY_MSR_TAPA,       ///< Standby module TAPA
    BSP_MODULE_STANDBY_MSR_TAUD,       ///< Standby module TAUD
    BSP_MODULE_STANDBY_MSR_TAUJ_ISO,   ///< Standby module TAUJ_ISO
    BSP_MODULE_STANDBY_MSR_TSG3,       ///< Standby module TSG3
    BSP_MODULE_STANDBY_MSR_OSTM,       ///< Standby module OSTM
    BSP_MODULE_STANDBY_MSR_RI3C,       ///< Standby module RI3C
    BSP_MODULE_STANDBY_MSR_SSIF,       ///< Standby module SSIF
    BSP_MODULE_STANDBY_MSR_ADCK_AWO,   ///< Standby module ADCK_AWO
    BSP_MODULE_STANDBY_MSR_RTCA,       ///< Standby module RTCA
    BSP_MODULE_STANDBY_MSR_TAUJ_AWO,   ///< Standby module TAUJ_AWO
    BSP_MODULE_STANDBY_MSR_WDTB_AWO,   ///< Standby module WDTB_AWO
    BSP_MODULE_STANDBY_TOTAL_INDEX_END ///< Total number of standby modules
} bsp_module_standby_t;

/** Stop-mode operation mapping to standby modules */
typedef enum e_bsp_module_stopmode
{
    BSP_MODULE_OPERATION_MODE_MSR_RSCFD    = BSP_MODULE_STANDBY_MSR_RSCFD,    ///< Stop mode module RSCFD
    BSP_MODULE_OPERATION_MODE_MSR_RLIN3    = BSP_MODULE_STANDBY_MSR_RLIN3,    ///< Stop mode module RLIN3
    BSP_MODULE_OPERATION_MODE_MSR_ADCK_ISO = BSP_MODULE_STANDBY_MSR_ADCK_ISO, ///< Stop mode module ADCK_ISO
    BSP_MODULE_OPERATION_MODE_MSR_RI3C     = BSP_MODULE_STANDBY_MSR_RI3C,     ///< Stop mode module RI3C
    BSP_MODULE_OPERATION_MODE_MSR_ADCK_AWO = BSP_MODULE_STANDBY_MSR_ADCK_AWO, ///< Stop mode module ADCK_AWO
    BSP_MODULE_OPERATION_MODE_MSR_RTCA     = BSP_MODULE_STANDBY_MSR_RTCA,     ///< Stop mode module RTCA
    BSP_MODULE_OPERATION_MODE_MSR_TAUJ_AWO = BSP_MODULE_STANDBY_MSR_TAUJ_AWO, ///< Stop mode module TAUJ_AWO
    BSP_MODULE_OPERATION_MODE_MSR_WDTB_AWO = BSP_MODULE_STANDBY_MSR_WDTB_AWO  ///< Stop mode module WDTB_AWO
} bsp_module_stopmode_t;

/** Stop-mode clock supply configuration */
typedef enum e_bsp_module_stopmode_config
{
    BSP_MODULE_CLOCK_STOPPED  = 0U,    ///< Clock stopped in stop mode
    BSP_MODULE_CLOCK_SUPPLIED = 1U     ///< Clock supplied in stop mode
} bsp_module_stopmode_config_t;

/** Wakup Factor for DeepStop */
typedef enum e_bsp_wk_factor
{
    BSP_WK_FACTOR_NONE = 255U,                                               ///< No wake-up factor selected

    /* Wake-up factor for register A0 */
    BSP_WK_FACTOR_TNMI       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 0U,     ///< Wake-up factor TNMI
    BSP_WK_FACTOR_INTCLMATI0 = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 1U,     ///< Wake-up factor INTCLMATI0
    BSP_WK_FACTOR_INTCLMATI1 = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 2U,     ///< Wake-up factor INTCLMATI1
    BSP_WK_FACTOR_INTCLMATI2 = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 3U,     ///< Wake-up factor INTCLMATI2
    BSP_WK_FACTOR_INTWDTBA   = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 5U,     ///< Wake-up factor INTWDTBA
    BSP_WK_FACTOR_IRQ0       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 6U,     ///< Wake-up factor IRQ0
    BSP_WK_FACTOR_IRQ1       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 7U,     ///< Wake-up factor IRQ1
    BSP_WK_FACTOR_IRQ2       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 8U,     ///< Wake-up factor IRQ2
    BSP_WK_FACTOR_IRQ3       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 9U,     ///< Wake-up factor IRQ3
    BSP_WK_FACTOR_IRQ4       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 10U,    ///< Wake-up factor IRQ4
    BSP_WK_FACTOR_IRQ5       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 11U,    ///< Wake-up factor IRQ5
    BSP_WK_FACTOR_IRQ6       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 12U,    ///< Wake-up factor IRQ6
    BSP_WK_FACTOR_IRQ7       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 13U,    ///< Wake-up factor IRQ7
    BSP_WK_FACTOR_IRQ8       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 14U,    ///< Wake-up factor IRQ8
    BSP_WK_FACTOR_IRQ9       = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 15U,    ///< Wake-up factor IRQ9
    BSP_WK_FACTOR_IRQ10      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 16U,    ///< Wake-up factor IRQ10
    BSP_WK_FACTOR_IRQ11      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 17U,    ///< Wake-up factor IRQ11
    BSP_WK_FACTOR_IRQ12      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 18U,    ///< Wake-up factor IRQ12
    BSP_WK_FACTOR_IRQ13      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 19U,    ///< Wake-up factor IRQ13
    BSP_WK_FACTOR_IRQ14      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 20U,    ///< Wake-up factor IRQ14
    BSP_WK_FACTOR_IRQ15      = BSP_STANDBY_WAKE_UP_FACTOR_A0_START + 21U,    ///< Wake-up factor IRQ15

    /* Wake-up factor for register A1 */
    BSP_WK_FACTOR_IRQ16 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 0U,          ///< Wake-up factor IRQ16
    BSP_WK_FACTOR_IRQ17 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 1U,          ///< Wake-up factor IRQ17
    BSP_WK_FACTOR_IRQ18 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 2U,          ///< Wake-up factor IRQ18
    BSP_WK_FACTOR_IRQ19 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 3U,          ///< Wake-up factor IRQ19
    BSP_WK_FACTOR_IRQ20 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 4U,          ///< Wake-up factor IRQ20
    BSP_WK_FACTOR_IRQ21 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 5U,          ///< Wake-up factor IRQ21
    BSP_WK_FACTOR_IRQ22 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 6U,          ///< Wake-up factor IRQ22
    BSP_WK_FACTOR_IRQ23 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 7U,          ///< Wake-up factor IRQ23
    BSP_WK_FACTOR_IRQ24 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 8U,          ///< Wake-up factor IRQ24
    BSP_WK_FACTOR_IRQ25 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 9U,          ///< Wake-up factor IRQ25
    BSP_WK_FACTOR_IRQ26 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 10U,         ///< Wake-up factor IRQ26
    BSP_WK_FACTOR_IRQ27 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 11U,         ///< Wake-up factor IRQ27
    BSP_WK_FACTOR_IRQ28 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 12U,         ///< Wake-up factor IRQ28
    BSP_WK_FACTOR_IRQ29 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 13U,         ///< Wake-up factor IRQ29
    BSP_WK_FACTOR_IRQ30 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 14U,         ///< Wake-up factor IRQ30
    BSP_WK_FACTOR_IRQ31 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 15U,         ///< Wake-up factor IRQ31
    BSP_WK_FACTOR_IRQ32 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 16U,         ///< Wake-up factor IRQ32
    BSP_WK_FACTOR_IRQ33 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 17U,         ///< Wake-up factor IRQ33
    BSP_WK_FACTOR_IRQ34 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 18U,         ///< Wake-up factor IRQ34
    BSP_WK_FACTOR_IRQ35 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 19U,         ///< Wake-up factor IRQ35
    BSP_WK_FACTOR_IRQ36 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 20U,         ///< Wake-up factor IRQ36
    BSP_WK_FACTOR_IRQ37 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 21U,         ///< Wake-up factor IRQ37
    BSP_WK_FACTOR_IRQ38 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 22U,         ///< Wake-up factor IRQ38
    BSP_WK_FACTOR_IRQ39 = BSP_STANDBY_WAKE_UP_FACTOR_A1_START + 23U,         ///< Wake-up factor IRQ39

    /* Wake-up factor for register A2 */
    BSP_WK_FACTOR_WUTRG0     = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 0U,     ///< Wake-up factor WUTRG0
    BSP_WK_FACTOR_WUTRG1     = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 1U,     ///< Wake-up factor WUTRG1
    BSP_WK_FACTOR_INTDCUTDI  = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 2U,     ///< Wake-up factor INTDCUTDI
    BSP_WK_FACTOR_INTTAUJ2I0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 3U,     ///< Wake-up factor INTTAUJ2I0
    BSP_WK_FACTOR_INTTAUJ2I1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 4U,     ///< Wake-up factor INTTAUJ2I1
    BSP_WK_FACTOR_INTTAUJ2I2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 5U,     ///< Wake-up factor INTTAUJ2I2
    BSP_WK_FACTOR_INTTAUJ2I3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 6U,     ///< Wake-up factor INTTAUJ2I3
    BSP_WK_FACTOR_INTTAUJ3I0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 7U,     ///< Wake-up factor INTTAUJ3I0
    BSP_WK_FACTOR_INTTAUJ3I1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 8U,     ///< Wake-up factor INTTAUJ3I1
    BSP_WK_FACTOR_INTTAUJ3I2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 9U,     ///< Wake-up factor INTTAUJ3I2
    BSP_WK_FACTOR_INTTAUJ3I3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 10U,    ///< Wake-up factor INTTAUJ3I3
    BSP_WK_FACTOR_INTRTCA01S = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 11U,    ///< Wake-up factor INTRTCA01S
    BSP_WK_FACTOR_INTRTCA0AL = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 12U,    ///< Wake-up factor INTRTCA0AL
    BSP_WK_FACTOR_INTRTCA0R  = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 13U,    ///< Wake-up factor INTRTCA0R
    BSP_WK_FACTOR_INTADCKAI0 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 14U,    ///< Wake-up factor INTADCKAI0
    BSP_WK_FACTOR_INTADCKAI1 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 15U,    ///< Wake-up factor INTADCKAI1
    BSP_WK_FACTOR_INTADCKAI2 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 16U,    ///< Wake-up factor INTADCKAI2
    BSP_WK_FACTOR_INTADCKAI3 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 17U,    ///< Wake-up factor INTADCKAI3
    BSP_WK_FACTOR_INTADCKAI4 = BSP_STANDBY_WAKE_UP_FACTOR_A2_START + 18U,    ///< Wake-up factor INTADCKAI4

    /* Wake-up factor for register I0 */
    BSP_WK_FACTOR_INTRCANGRECC0 = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 0U,  ///< Wake-up factor INTRCANGRECC0
    BSP_WK_FACTOR_INTRCAN0REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 1U,  ///< Wake-up factor INTRCAN0REC
    BSP_WK_FACTOR_INTRCAN1REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 2U,  ///< Wake-up factor INTRCAN1REC
    BSP_WK_FACTOR_INTRCAN2REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 3U,  ///< Wake-up factor INTRCAN2REC
    BSP_WK_FACTOR_INTRCAN3REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 4U,  ///< Wake-up factor INTRCAN3REC
    BSP_WK_FACTOR_INTRCAN4REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 5U,  ///< Wake-up factor INTRCAN4REC
    BSP_WK_FACTOR_INTRCAN5REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 6U,  ///< Wake-up factor INTRCAN5REC
    BSP_WK_FACTOR_INTRCAN6REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 7U,  ///< Wake-up factor INTRCAN6REC
    BSP_WK_FACTOR_INTRCAN7REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 8U,  ///< Wake-up factor INTRCAN7REC
    BSP_WK_FACTOR_INTRCANGRECC1 = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 9U,  ///< Wake-up factor INTRCANGRECC1
    BSP_WK_FACTOR_INTRCAN8REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 10U, ///< Wake-up factor INTRCAN8REC
    BSP_WK_FACTOR_INTRCAN9REC   = BSP_STANDBY_WAKE_UP_FACTOR_I0_START + 11U, ///< Wake-up factor INTRCAN9REC

    /* Wake-up factor for register I1 */
    BSP_WK_FACTOR_INTRLIN30  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 0U,     ///< Wake-up factor INTRLIN30
    BSP_WK_FACTOR_INTRLIN31  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 1U,     ///< Wake-up factor INTRLIN31
    BSP_WK_FACTOR_INTRLIN32  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 2U,     ///< Wake-up factor INTRLIN32
    BSP_WK_FACTOR_INTRLIN33  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 3U,     ///< Wake-up factor INTRLIN33
    BSP_WK_FACTOR_INTRLIN34  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 4U,     ///< Wake-up factor INTRLIN34
    BSP_WK_FACTOR_INTRLIN35  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 5U,     ///< Wake-up factor INTRLIN35
    BSP_WK_FACTOR_INTRLIN36  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 6U,     ///< Wake-up factor INTRLIN36
    BSP_WK_FACTOR_INTRLIN37  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 7U,     ///< Wake-up factor INTRLIN37
    BSP_WK_FACTOR_INTRLIN38  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 8U,     ///< Wake-up factor INTRLIN38
    BSP_WK_FACTOR_INTRLIN39  = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 9U,     ///< Wake-up factor INTRLIN39
    BSP_WK_FACTOR_INTRLIN310 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 10U,    ///< Wake-up factor INTRLIN310
    BSP_WK_FACTOR_INTRLIN311 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 11U,    ///< Wake-up factor INTRLIN311
    BSP_WK_FACTOR_INTRLIN312 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 12U,    ///< Wake-up factor INTRLIN312
    BSP_WK_FACTOR_INTRLIN313 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 13U,    ///< Wake-up factor INTRLIN313
    BSP_WK_FACTOR_INTRLIN314 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 14U,    ///< Wake-up factor INTRLIN314
    BSP_WK_FACTOR_INTRLIN315 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 15U,    ///< Wake-up factor INTRLIN315
    BSP_WK_FACTOR_INTRLIN316 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 16U,    ///< Wake-up factor INTRLIN316
    BSP_WK_FACTOR_INTRLIN317 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 17U,    ///< Wake-up factor INTRLIN317
    BSP_WK_FACTOR_INTRLIN318 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 18U,    ///< Wake-up factor INTRLIN318
    BSP_WK_FACTOR_INTRLIN319 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 19U,    ///< Wake-up factor INTRLIN319
    BSP_WK_FACTOR_INTRLIN320 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 20U,    ///< Wake-up factor INTRLIN320
    BSP_WK_FACTOR_INTRLIN321 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 21U,    ///< Wake-up factor INTRLIN321
    BSP_WK_FACTOR_INTRLIN322 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 22U,    ///< Wake-up factor INTRLIN322
    BSP_WK_FACTOR_INTRLIN323 = BSP_STANDBY_WAKE_UP_FACTOR_I1_START + 23U,    ///< Wake-up factor INTRLIN323

    /* Wake-up factor for register I2 */
    BSP_WK_FACTOR_INTRCAN0VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 1U,   ///< Wake-up factor INTRCAN0VMRX
    BSP_WK_FACTOR_INTRCAN1VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 2U,   ///< Wake-up factor INTRCAN1VMRX
    BSP_WK_FACTOR_INTRCAN2VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 3U,   ///< Wake-up factor INTRCAN2VMRX
    BSP_WK_FACTOR_INTRCAN3VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 4U,   ///< Wake-up factor INTRCAN3VMRX
    BSP_WK_FACTOR_INTRCAN4VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 5U,   ///< Wake-up factor INTRCAN4VMRX
    BSP_WK_FACTOR_INTRCAN5VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 6U,   ///< Wake-up factor INTRCAN5VMRX
    BSP_WK_FACTOR_INTRCAN6VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 7U,   ///< Wake-up factor INTRCAN6VMRX
    BSP_WK_FACTOR_INTRCAN7VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 8U,   ///< Wake-up factor INTRCAN7VMRX
    BSP_WK_FACTOR_INTRCAN8VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 10U,  ///< Wake-up factor INTRCAN8VMRX
    BSP_WK_FACTOR_INTRCAN9VMRX = BSP_STANDBY_WAKE_UP_FACTOR_I2_START + 11U   ///< Wake-up factor INTRCAN9VMRX
} bsp_wk_factor_t;

/** IO Hold */
typedef enum e_bsp_io_hold
{
    BSP_IO_HOLD_P00       = (0U),      ///< IO hold P00
    BSP_IO_HOLD_P03       = (3U),      ///< IO hold P03
    BSP_IO_HOLD_P08       = (8U),      ///< IO hold P08
    BSP_IO_HOLD_P10       = (10U),     ///< IO hold P10
    BSP_IO_HOLD_P17       = (17U),     ///< IO hold P17
    BSP_IO_HOLD_P19       = (19U),     ///< IO hold P19
    BSP_IO_HOLD_P20       = (20U),     ///< IO hold P20
    BSP_IO_HOLD_P21       = (21U),     ///< IO hold P21
    BSP_IO_HOLD_P22       = (22U),     ///< IO hold P22
    BSP_IO_HOLD_P24       = (24U),     ///< IO hold P24
    BSP_IO_HOLD_AP2       = (34U),     ///< IO hold AP2
    BSP_IO_HOLD_AP3       = (35U),     ///< IO hold AP3
    BSP_IO_HOLD_AP4       = (36U),     ///< IO hold AP4
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
    BSP_WK_REG_WUFMSK_I2,              ///< Wake-up mask register I2

    BSP_WK_REG_WUFC_A0,                ///< Wake-up clear register A0
    BSP_WK_REG_WUFC_A1,                ///< Wake-up clear register A1
    BSP_WK_REG_WUFC_A2,                ///< Wake-up clear register A2

    BSP_WK_REG_WUFC_I0,                ///< Wake-up clear register I0
    BSP_WK_REG_WUFC_I1,                ///< Wake-up clear register I1
    BSP_WK_REG_WUFC_I2,                ///< Wake-up clear register I2

    BSP_WK_REG_WUF_A0,                 ///< Wake-up flag register A0
    BSP_WK_REG_WUF_A1,                 ///< Wake-up flag register A1
    BSP_WK_REG_WUF_A2,                 ///< Wake-up flag register A2

    BSP_WK_REG_WUF_I0,                 ///< Wake-up flag register I0
    BSP_WK_REG_WUF_I1,                 ///< Wake-up flag register I1
    BSP_WK_REG_WUF_I2                  ///< Wake-up flag register I2
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

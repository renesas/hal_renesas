/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_PIC_H
#define R_PIC_H

/*******************************************************************************************************************//**
 * @addtogroup PIC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_pic_api.h"
#include "r_pic_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Macro for ADC Units */
#define PIC_ADC_UNIT_0                        (0U)
#define PIC_ADC_UNIT_A                        (0U)
#define PIC_ADC_UNIT_1                        (1U)
#define PIC_ADC_UNIT_2                        (2U)
#define PIC_ADC_UNIT_3                        (3U)

/** Macro for ADC Scan group */
#define PIC_ADC_SG_0                          (0U)
#define PIC_ADC_SG_1                          (1U)
#define PIC_ADC_SG_2                          (2U)
#define PIC_ADC_SG_3                          (3U)
#define PIC_ADC_SG_4                          (4U)
#define PIC_ADC_SG_DIAG                       (5U)

/** Macro for TAUD unit in PIC20 and PIC21 for ADC Trigger Selection */
#define PIC_ADC_TRIG_TAUD_UNIT_0              (0U)
#define PIC_ADC_TRIG_TAUD_UNIT_1              (1U)
#define PIC_ADC_TRIG_TAUD_UNIT_2              (0U)
#define PIC_ADC_TRIG_TAUD_UNIT_3              (1U)

/** Macro for PIC Units registers */
#define PIC_DSADC_UNIT_0                      (0U)
#define PIC_DSADC_UNIT_1                      (1U)
#define PIC_DSADC_UNIT_2                      (2U)
#define PIC_DSADC_UNIT_3                      (3U)
#define PIC_DSADC_UNIT_4                      (4U)
#define PIC_DSADC_UNIT_5                      (5U)
#define PIC_DSADC_UNIT_6                      (6U)
#define PIC_DSADC_UNIT_7                      (7U)
#define PIC_DSADC_UNIT_8                      (8U)
#define PIC_DSADC_UNIT_9                      (9U)
#define PIC_DSADC_UNIT_10                     (10U)
#define PIC_DSADC_UNIT_11                     (11U)
#define PIC_DSADC_UNIT_12                     (12U)
#define PIC_DSADC_UNIT_13                     (13U)

/** Macro for PIC20DSADTEN0nx, PIC21DSADTEN0nx (x= 0 to 3) registers */
#define PIC_DSADC_REG_0                       (0U)
#define PIC_DSADC_REG_1                       (1U)
#define PIC_DSADC_REG_2                       (2U)
#define PIC_DSADC_REG_3                       (3U)

/** Macro for PIC2DSADTSENk (k = 0 to 1, 4 to 6) registers */
#define PIC_DSADC_READ_GATE_NUMBER            (5U)
#define PIC_DSADC_READ_GATE_0                 (0U)
#define PIC_DSADC_READ_GATE_1                 (1U)
#define PIC_DSADC_READ_GATE_4                 (2U)
#define PIC_DSADC_READ_GATE_5                 (3U)
#define PIC_DSADC_READ_GATE_6                 (4U)

/** Macro for PIC2DSADTCFG registers */
#define PIC_DSADTCFG_REG_NUMBER               (2U)
#define PIC_DSADTCFG_REG_0                    (0U)
#define PIC_DSADTCFG_REG_1                    (1U)

/** Macro for PIC2CADTCFG registers */
#define PIC_CADTCFG_REG_NUMBER                (2U)
#define PIC_CADTTCFG_REG_0                    (0U)
#define PIC_CADTTCFG_REG_1                    (1U)

/** Macro for TAUD channel in PIC10 features */
#define PIC_TAUD_FEATURE_UNIT_0               (0U)
#define PIC_TAUD_FEATURE_UNIT_1               (1U)
#define PIC_TAUD_FEATURE_UNIT_2               (2U)
#define PIC_TAUD_FEATURE_UNIT_3               (3U)

/** Macro for ENCA features */
#define PIC_ENCA_FEATURE_UNIT_0               (0U)
#define PIC_ENCA_FEATURE_UNIT_1               (1U)

/** Macro for TSG3 features */
#define PIC_TSG3_FEATURE_UNIT_0               (0U)
#define PIC_TSG3_FEATURE_UNIT_1               (1U)
#define PIC_TSG3_FEATURE_UNIT_2               (2U)

/** Macro for EMU features */
#define PIC_EMU3S_UNIT_0                      (0U)
#define PIC_EMU3S_UNIT_1                      (1U)
#define PIC_EMU3S_NUMBER_UNIT                 (2U)

/** Macro for PIC2GTMSEN registers */
#define PIC_GTMSEN_REG_NUMBER                 (8U)
#define PIC_GTMSEN_REG_0                      (0U)
#define PIC_GTMSEN_REG_1                      (1U)
#define PIC_GTMSEN_REG_2                      (2U)
#define PIC_GTMSEN_REG_3                      (3U)
#define PIC_GTMSEN_REG_4                      (4U)
#define PIC_GTMSEN_REG_5                      (5U)
#define PIC_GTMSEN_REG_6                      (6U)
#define PIC_GTMSEN_REG_7                      (7U)

/** Macro for PIC2ADTCFG registers */
#define PIC_ADTCFG_REG_0                      (0U)
#define PIC_ADTCFG_REG_1                      (1U)
#define PIC_ADTCFG_REG_2                      (2U)
#define PIC_ADTCFG_REG_3                      (3U)
#define PIC_ADTCFG_REG_4                      (4U)
#define PIC_ADTCFG_REG_5                      (5U)
#define PIC_ADTCFG_REG_6                      (6U)
#define PIC_ADTCFG_REG_7                      (7U)
#define PIC_ADTCFG_REG_8                      (8U)
#define PIC_ADTCFG_REG_9                      (9U)

/** Macro for PIC2ENCATCFG registers */
#define PIC_ENCATCFG_REG_NUMBER               (2U)
#define PIC_ENCATCFG_REG_0                    (0U)
#define PIC_ENCATCFG_REG_1                    (1U)

/** Macro for RDC features */
#define PIC_RDC3A_FEATURE_UNIT_0              (0U)
#define PIC_RDC3A_FEATURE_UNIT_1              (1U)
#define PIC_RDC3A_FEATURE_NUMBER_UNIT         (2U)
#define PIC_RDC_FEATURE_RDC3AL                (0U)
#define PIC_RDC_FEATURE_RDC3AS                (1U)

/** Macro for MSPI Trigger Selection */
#define PIC_GTM_MSPI_SIGNAL_0                 (0U)
#define PIC_GTM_MSPI_SIGNAL_1                 (1U)
#define PIC_GTM_MSPI_SIGNAL_2                 (2U)
#define PIC_GTM_MSPI_SIGNAL_3                 (3U)
#define PIC_GTM_MSPI_SIGNAL_4                 (4U)
#define PIC_GTM_MSPI_SIGNAL_5                 (5U)
#define PIC_GTM_MSPI_SIGNAL_6                 (6U)
#define PIC_GTM_MSPI_SIGNAL_7                 (7U)
#define PIC_GTM_MSPI_SIGNAL_8                 (8U)
#define PIC_GTM_MSPI_SIGNAL_9                 (9U)

/** Macro for GTM Timer(TIM) Input Select */
#define PIC_GTMINEN_GROUP_0                   (0U)
#define PIC_GTMINEN_GROUP_1                   (1U)
#define PIC_GTMINEN_GROUP_2                   (2U)
#define PIC_GTMINEN_GROUP_3                   (3U)
#define PIC_GTMINEN_GROUP_4                   (4U)
#define PIC_GTMINEN_GROUP_5                   (5U)
#define PIC_GTMINEN_GROUP_6                   (6U)

/** Macro for GTM Timer(TIO) Input Select */
#define PIC_GTMINTIOEN_GROUP_0                (0U)
#define PIC_GTMINTIOEN_GROUP_1                (1U)
#define PIC_GTMINTIOEN_GROUP_2                (2U)
#define PIC_GTMINTIOEN_GROUP_3                (3U)
#define PIC_GTMINTIOEN_GROUP_4                (4U)
#define PIC_GTMINTIOEN_GROUP_5                (5U)
#define PIC_GTMINTIOEN_GROUP_6                (6U)
#define PIC_GTMINTIOEN_GROUP_7                (7U)

/** Macro for DFETEDSEL registers */
#define PIC_DFETEDSEL_REG_NUMBER              (3U)
#define PIC_DFETEDSEL_REG_0                   (0U)
#define PIC_DFETEDSEL_REG_1                   (1U)
#define PIC_DFETEDSEL_REG_2                   (2U)

/** Macro for DFE Trigger Selection */
#define PIC_DFE_TIM_COMP_0                    (0U)
#define PIC_DFE_TIM_COMP_1                    (1U)
#define PIC_DFE_TIM_COMP_2                    (2U)
#define PIC_DFE_TIM_COMP_3                    (3U)
#define PIC_DFE_TIM_COMP_4                    (4U)
#define PIC_DFE_TIM_COMP_5                    (5U)
#define PIC_DFE_TIM_COMP_6                    (6U)
#define PIC_DFE_TIM_COMP_7                    (7U)
#define PIC_DFE_TIM_COMP_8                    (8U)
#define PIC_DFE_TIM_COMP_9                    (9U)
#define PIC_DFE_TIM_COMP_10                   (10U)
#define PIC_DFE_TIM_COMP_11                   (11U)
#define PIC_DFE_TIM_COMP_12                   (12U)
#define PIC_DFE_TIM_COMP_13                   (13U)
#define PIC_DFE_TIM_COMP_14                   (14U)
#define PIC_DFE_TIM_COMP_15                   (15U)
#define PIC_DFE_TIM_COMP_16                   (16U)
#define PIC_DFE_TIM_COMP_17                   (17U)
#define PIC_DFE_TIM_COMP_18                   (18U)
#define PIC_DFE_TIM_COMP_19                   (19U)
#define PIC_DFE_TIM_COMP_20                   (20U)
#define PIC_DFE_TIM_COMP_21                   (21U)

/** Macro for PSI5S features */
#define PIC_PSI5S_FEATURE_UNIT_0              (0U)
#define PIC_PSI5S_FEATURE_UNIT_1              (1U)

/** Macro for PSI5S registers */
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_0         (0U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_1         (1U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_2         (2U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_3         (3U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_4         (4U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_5         (5U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_6         (6U)
#define PIC_PSI5S_SYNC_OUT_CTRL_REG_7         (7U)

/** Macro for PSI5 features */
#define PIC_PSI5_FEATURE_UNIT_0               (0U)
#define PIC_PSI5_FEATURE_UNIT_1               (1U)
#define PIC_PSI5_FEATURE_UNIT_2               (2U)
#define PIC_PSI5_FEATURE_UNIT_3               (3U)

/** Macro for PSI5 registers */
#define PIC_PSI5_SYNC_OUT_CTRL_REG_0          (0U)
#define PIC_PSI5_SYNC_OUT_CTRL_REG_1          (1U)
#define PIC_PSI5_SYNC_OUT_CTRL_REG_2          (2U)
#define PIC_PSI5_SYNC_OUT_CTRL_REG_3          (3U)
#define PIC_PSI5_SYNC_OUT_CTRL_REG_4          (4U)
#define PIC_PSI5_SYNC_OUT_CTRL_REG_5          (5U)

/** Macro for PIC2ABFGODSEN registers */
#define PIC_ABFGODSEN_REG_0                   (0U)
#define PIC_ABFGODSEN_REG_1                   (1U)
#define PIC_ABFGODSEN_REG_2                   (2U)
#define PIC_ABFGODSEN_REG_3                   (3U)
#define PIC_ABFGODSEN_REG_4                   (4U)
#define PIC_ABFGODSEN_REG_5                   (5U)
#define PIC_ABFGODSEN_REG_6                   (6U)
#define PIC_ABFGODSEN_REG_7                   (7U)
#define PIC_ABFGODSEN_REG_8                   (8U)
#define PIC_ABFGODSEN_REG_9                   (9U)
#define PIC_ABFGODSEN_REG_10                  (10U)
#define PIC_ABFGODSEN_REG_11                  (11U)
#define PIC_ABFGODSEN_REG_12                  (12U)
#define PIC_ABFGODSEN_REG_13                  (13U)

/** Macro for PIC2CANIOSEN register */
#define PIC_CANIOSEN_REG_0                    (0U)
#define PIC_CANIOSEN_REG_1                    (1U)
#define PIC_CANIOSEN_REG_2                    (2U)
#define PIC_CANIOSEN_REG_3                    (3U)
#define PIC_CANIOSEN_REG_4                    (4U)

/** Macro for PIC2FCMPISEN registers. */
#define PIC_FCMPISEN_REG_0                    (0U)
#define PIC_FCMPISEN_REG_1                    (1U)
#define PIC_FCMPISEN_REG_2                    (2U)
#define PIC_FCMPISEN_REG_3                    (3U)
#define PIC_FCMPISEN_REG_4                    (4U)
#define PIC_FCMPISEN_REG_5                    (5U)
#define PIC_FCMPISEN_REG_6                    (6U)
#define PIC_FCMPISEN_REG_7                    (7U)
#define PIC_FCMPISEN_REG_8                    (8U)
#define PIC_FCMPISEN_REG_9                    (9U)

/** Macro for TSG3 input from GTM */
#define PIC_TSG3_INPUT_FEATURE_NUMBER_UNIT    (2U)

/** GTM signal is not selected. */
#define PIC_GTM_OUT_SEL_NONE                  ((uint16_t) 0x0000U)

/** Select GTM_TOM[i]_OUT[x] output value as signal source */
#define PIC_GTM_OUT_1_SEL_TOM_OUT(i, x)          ((uint16_t) (1 + ((i) * 16) + (x)))

/** Select GTM_ATOM[i]_OUT[x] output value as signal source */
#define PIC_GTM_OUT_1_SEL_ATOM_OUT(i, x)         ((uint16_t) (97 + ((i) * 8) + (x)))

/** Select GTM_TIO[i]_OUT[x] (i = 0) output value as signal source */
#define PIC_GTM_OUT_1_SEL_TIO_OUT_0(i, x)        ((uint16_t) (177 + ((i) * 8) + (x)))

/** Select GTM_TIO[i]_OUT[x] (i = 1 to 4) output value as signal source */
#define PIC_GTM_OUT_1_SEL_TIO_OUT_1(i, x)        ((uint16_t) (177 + (((i) - 1) * 8) + (x)))

/** Select GTM_TIO[i]_OUT[x] (i = 6 to 9) output value as signal source */
#define PIC_GTM_OUT_1_SEL_TIO_OUT_2(i, x)        ((uint16_t) (177 + (((i) - 2) * 8) + (x)))

/** Select GTM_TOM[i]_OUT[x]_N output value as signal source */
#define PIC_GTM_OUT_1_SEL_TOM_OUT_N(i, x)        ((uint16_t) (241 + ((i) * 8) + (x)))

/** Select GTM_ATOM[i]_OUT[x]_N output value as signal source */
#define PIC_GTM_OUT_1_SEL_ATOM_OUT_N(i, x)       ((uint16_t) (281 + ((i) * 8) + (x)))

/** Select GTM_TIO[i]_OUT[x]_N output value as signal source */
#define PIC_GTM_OUT_1_SEL_TIO_OUT_N(i, x)        ((uint16_t) (337 + (((i) - 1) * 8) + (x)))

/** Select GTM_MCS[i]_IRQ[x] output value as signal source */
#define PIC_GTM_OUT_1_SEL_MCS_IRQ(i, x)          ((uint16_t) (345 + ((i) * 8) + (x)))

/** Select GTM_TIM[i]_IRQ[x] output value as signal source */
#define PIC_GTM_OUT_1_SEL_TIM_IRQ(i, x)          ((uint16_t) (425 + ((i) * 8) + (x)))

/** GTM_TOM[i]_OUT[x] */
#define PIC_GTM_OUT_2_SEL_TOM_OUT(i, x)          ((uint16_t) (1 + ((i) * 16) + (x)))

/** GTM_ATOM[i]_OUT[x] */
#define PIC_GTM_OUT_2_SEL_ATOM_OUT(i, x)         ((uint16_t) (97 + ((i) * 8) + (x)))

/** GTM_TIM[i]_IRQ[x] */
#define PIC_GTM_OUT_2_SEL_TIM_IRQ(i, x)          ((uint16_t) (193 + ((i) * 8) + (x)))

/** GTM_TIM[i]_CH[x] */
#define PIC_GTM_OUT_2_SEL_TIM_CH(i, x)           ((uint16_t) (193 + ((i) * 8) + (x)))

/** GTM_TIO[i]_OUT[x] (U2C TIO0) */
#define PIC_GTM_OUT_2_SEL_TIO_OUT(i, x)          ((uint16_t) (257 + ((i) * 8) + (x)))

/** GTM_TIO[i]_OUT[x] (U2B TIO1 to TIO5) */
#define PIC_GTM_OUT_2_SEL_TIO_OUT_1(i, x)        ((uint16_t) (257 + (((i) - 1) * 8) + (x)))

/** GTM_TIO[i]_OUT[x] (U2B TIO6 to TIO10) */
#define PIC_GTM_OUT_2_SEL_TIO_OUT_2(i, x)        ((uint16_t) (257 + (((i) - 2) * 8) + (x)))

/** GTM_TOM[i]_OUT[x]_N */
#define PIC_GTM_OUT_2_SEL_TOM_OUT_N(i, x)        ((uint16_t) (321 + ((i) * 8) + (x)))

/** GTM_ATOM[i]_OUT[x]_N */
#define PIC_GTM_OUT_2_SEL_ATOM_OUT_N(i, x)       ((uint16_t) (361 + ((i) * 8) + (x)))

/** GTM_TIO[i]_OUT[x]_N */
#define PIC_GTM_OUT_2_SEL_TIO_OUT_N(i, x)        ((uint16_t) (417 + (((i) - 1) * 8) + (x)))

/** INTGTMA0TIM[i][x] (U2A) */
#define PIC_GTM_OUT_2_SEL_INTGTMA0TIM(i, x)      ((uint16_t) (177 + ((i) * 8) + (x)))

/** Select TAUD output as trigger source for RDC Angle / Angular Velocity Latch */
#define PIC_RDC_LATCH_TRIG_SEL_TAUD(n, m)        ((uint8_t) (1 + ((n) * 16) + (m)))

/** Select GTM ATOM output as trigger source for RDC Angle / Angular Velocity Latch */
#define PIC_RDC_LATCH_TRIG_SEL_GTM_ATOM(i, x)    ((uint8_t) (65 + ((i) * 8) + (x)))

/** Selects the CAN I/O signal as not selected. */
#define PIC_CANIOSEN_SEL_NONE    (0U)

/** Select GTM ATOM output as trigger source for IO sharing CAN/GTM-Internal arbitration */
#define PIC_CANIOSEN_SEL_GTM_ATOM_OUT(i, x)    ((uint8_t) (1 + ((i) * 8) + (x)))

/** The ABFG trigger source for GTM DTM is not selected.  */
#define PIC_ABFG_BF_OUT_DTM_SEL_NONE    (0U)

/** Select ABFG_BFn signal as trigger source for GTM */
#define PIC_ABFG_BF_OUT_DTM_SEL_BF(n)    ((uint8_t) (1 + n))

/** GTM output as a signal source for TSG3 is not selected.  */
#define PIC_GTM_INPUT_TSG3_SEL_NONE    (0U)

/** Selects the GTM_ATOM[i]_OUT[x] output as a signal source for TSG3 */
#define PIC_GTM_INPUT_TSG3_SEL_ATOM_OUT(i, x)    ((uint8_t) (1 + (i * 8) + x))

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/*********************************************************************************************************************/

#if (BSP_FEATURE_PIC_FEATURE_U2BX)

/**  Selects a source of timer input of GTM (TIM) */
typedef enum e_pic_gtm_tim_src_sel
{
    PIC_GTM_TIM_SRC_SEL_GPIO                = 0U,   ///< GPIO (via Port Functions)
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG0           = 1U,   ///< ADCK0 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG1           = 2U,   ///< ADCK0 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG2           = 3U,   ///< ADCK0 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG3           = 4U,   ///< ADCK0 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG4           = 5U,   ///< ADCK0 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK0_SG_DIAG       = 6U,   ///< ADCK0 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG0           = 7U,   ///< ADCK1 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG1           = 8U,   ///< ADCK1 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG2           = 9U,   ///< ADCK1 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG3           = 10U,  ///< ADCK1 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG4           = 11U,  ///< ADCK1 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG_DIAG       = 12U,  ///< ADCK1 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG0           = 13U,  ///< ADCK2 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG1           = 14U,  ///< ADCK2 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG2           = 15U,  ///< ADCK2 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG3           = 16U,  ///< ADCK2 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG4           = 17U,  ///< ADCK2 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG_DIAG       = 18U,  ///< ADCK2 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG0           = 19U,  ///< ADCK3 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG1           = 20U,  ///< ADCK3 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG2           = 21U,  ///< ADCK3 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG3           = 22U,  ///< ADCK3 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG4           = 23U,  ///< ADCK3 scan group 4 (SG4) end
    PIC_GTM_TIM_SRC_SEL_ADCK3_SG_DIAG       = 24U,  ///< ADCK3 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC00             = 25U,  ///< DSADC00 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC10             = 26U,  ///< DSADC10 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC20             = 27U,  ///< DSADC20 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC12             = 28U,  ///< DSADC12 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC13             = 33U,  ///< DSADC13 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC11             = 34U,  ///< DSADC11 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC15             = 35U,  ///< DSADC15 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC14             = 36U,  ///< DSADC14 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC22             = 37U,  ///< DSADC22 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_DSADC21             = 38U,  ///< DSADC21 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_CADC00              = 39U,  ///< CADC00 A/D conversion end interrupt
    PIC_GTM_TIM_SRC_SEL_RLIN30              = 40U,  ///< RLIN30 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN31              = 41U,  ///< RLIN31 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN32              = 42U,  ///< RLIN32 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN33              = 43U,  ///< RLIN33 receive data input
    PIC_GTM_TIM_SRC_SEL_DFE0_PH_0           = 44U,  ///< DFE0 P/H updating trigger 0
    PIC_GTM_TIM_SRC_SEL_DFE0_PH_1           = 45U,  ///< DFE0 P/H updating trigger 1
    PIC_GTM_TIM_SRC_SEL_DFE0_PH_2           = 46U,  ///< DFE0 P/H updating trigger 2
    PIC_GTM_TIM_SRC_SEL_DFE0_PH_3           = 47U,  ///< DFE0 P/H updating trigger 3
    PIC_GTM_TIM_SRC_SEL_DFE0_CH0_DATA_INTR  = 48U,  ///< DFE0 CH0 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH1_DATA_INTR  = 49U,  ///< DFE0 CH1 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH2_DATA_INTR  = 50U,  ///< DFE0 CH2 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH3_DATA_INTR  = 51U,  ///< DFE0 CH3 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH4_DATA_INTR  = 52U,  ///< DFE0 CH4 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH5_DATA_INTR  = 53U,  ///< DFE0 CH5 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH6_DATA_INTR  = 54U,  ///< DFE0 CH6 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH7_DATA_INTR  = 55U,  ///< DFE0 CH7 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH8_DATA_INTR  = 56U,  ///< DFE0 CH8 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH9_DATA_INTR  = 57U,  ///< DFE0 CH9 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH10_DATA_INTR = 58U,  ///< DFE0 CH10 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH11_DATA_INTR = 59U,  ///< DFE0 CH11 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH12_DATA_INTR = 60U,  ///< DFE0 CH12 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH13_DATA_INTR = 61U,  ///< DFE0 CH13 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH14_DATA_INTR = 62U,  ///< DFE0 CH14 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE0_CH15_DATA_INTR = 63U,  ///< DFE0 CH15 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE1_PH_0           = 64U,  ///< DFE1 P/H updating trigger 0
    PIC_GTM_TIM_SRC_SEL_DFE1_PH_1           = 65U,  ///< DFE1 P/H updating trigger 1
    PIC_GTM_TIM_SRC_SEL_DFE1_PH_2           = 66U,  ///< DFE1 P/H updating trigger 2
    PIC_GTM_TIM_SRC_SEL_DFE1_PH_3           = 67U,  ///< DFE1 P/H updating trigger 3
    PIC_GTM_TIM_SRC_SEL_DFE1_CH0_DATA_INTR  = 68U,  ///< DFE1 CH0 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE1_CH1_DATA_INTR  = 69U,  ///< DFE1 CH1 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE1_CH2_DATA_INTR  = 70U,  ///< DFE1 CH2 output data interrupt
    PIC_GTM_TIM_SRC_SEL_DFE1_CH3_DATA_INTR  = 71U,  ///< DFE1 CH3 output data interrupt
    PIC_GTM_TIM_SRC_SEL_ENCA0_A             = 72U,  ///< ENCA0 Encoder input (phase A)
    PIC_GTM_TIM_SRC_SEL_ENCA0_B             = 73U,  ///< ENCA0 Encoder input (phase B)
    PIC_GTM_TIM_SRC_SEL_ENCA0_Z             = 74U,  ///< ENCA0 Encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_I0            = 75U,  ///< ENCA0 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_I1            = 76U,  ///< ENCA0 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_IEC           = 77U,  ///< ENCA0 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_CNT_CLK       = 78U,  ///< ENCA0 count clock
    PIC_GTM_TIM_SRC_SEL_ENCA0_DOWN_CNT      = 79U,  ///< ENCA0 down-count enable signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_A             = 80U,  ///< ENCA1 Encoder input (phase A)
    PIC_GTM_TIM_SRC_SEL_ENCA1_B             = 81U,  ///< ENCA1 Encoder input (phase B)
    PIC_GTM_TIM_SRC_SEL_ENCA1_Z             = 82U,  ///< ENCA1 Encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA1_I0            = 83U,  ///< ENCA1 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_I1            = 84U,  ///< ENCA1 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_IEC           = 85U,  ///< ENCA1 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA1_CNT_CLK       = 86U,  ///< ENCA1 count clock
    PIC_GTM_TIM_SRC_SEL_ENCA1_DOWN_CNT      = 87U,  ///< ENCA1 down-count enable signal
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_ENC_A       = 88U,  ///< RDC3AL0 encoder pulse A output
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_ENC_B       = 89U,  ///< RDC3AL0 encoder pulse B output
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_ENC_Z       = 90U,  ///< RDC3AL0 encoder pulse Z output
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_PHI_CMP_0   = 91U,  ///< RDC3AL0 phi comparison interrupt output 0
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_PHI_CMP_1   = 92U,  ///< RDC3AL0 phi comparison interrupt output 1
    PIC_GTM_TIM_SRC_SEL_RDC3AL0_PHI_CMP_2   = 93U,  ///< RDC3AL0 phi comparison interrupt output 2
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_ENC_A       = 95U,  ///< RDC3AL1 encoder pulse A output
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_ENC_B       = 96U,  ///< RDC3AL1 encoder pulse B output
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_ENC_Z       = 97U,  ///< RDC3AL1 encoder pulse Z output
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_PHI_CMP_0   = 98U,  ///< RDC3AL1 phi comparison interrupt output 0
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_PHI_CMP_1   = 99U,  ///< RDC3AL1 phi comparison interrupt output 1
    PIC_GTM_TIM_SRC_SEL_RDC3AL1_PHI_CMP_2   = 100U, ///< RDC3AL1 phi comparison interrupt output 2
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_ENC_A       = 102U, ///< RDC3AS0 encoder pulse A output
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_ENC_B       = 103U, ///< RDC3AS0 encoder pulse B output
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_ENC_Z       = 104U, ///< RDC3AS0 encoder pulse Z output
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_PHI_CMP_0   = 105U, ///< RDC3AS0 phi comparison interrupt output 1
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_PHI_CMP_1   = 106U, ///< RDC3AS0 phi comparison interrupt output 1
    PIC_GTM_TIM_SRC_SEL_RDC3AS0_PHI_CMP_2   = 107U, ///< RDC3AS0 phi comparison interrupt output 2
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_ENC_A       = 109U, ///< RDC3AS1 encoder pulse A output
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_ENC_B       = 110U, ///< RDC3AS1 encoder pulse B output
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_ENC_Z       = 111U, ///< RDC3AS1 encoder pulse Z output
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_PHI_CMP_0   = 112U, ///< RDC3AS1 phi comparison interrupt output 0
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_PHI_CMP_1   = 113U, ///< RDC3AS1 phi comparison interrupt output 1
    PIC_GTM_TIM_SRC_SEL_RDC3AS1_PHI_CMP_2   = 114U, ///< RDC3AS1 phi comparison interrupt output 2
    PIC_GTM_TIM_SRC_SEL_EMU3S0TSTWECMP      = 116U, ///< EMU3S0 write enable for carrier period and comparison values
    PIC_GTM_TIM_SRC_SEL_EMU3S0TSTIUP        = 117U, ///< EMU3S0 rectangle wave output (U phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S0TSTIVP        = 118U, ///< EMU3S0 rectangle wave output (V phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S0TSTIWP        = 119U, ///< EMU3S0 rectangle wave output (W phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT0          = 120U, ///< EMU3S0 interrupt 0
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT1          = 121U, ///< EMU3S0 interrupt 1
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT2          = 122U, ///< EMU3S0 interrupt 2
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT3          = 123U, ///< EMU3S0 interrupt 3
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT4          = 124U, ///< EMU3S0 interrupt 4
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT5          = 125U, ///< EMU3S0 interrupt 5
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT6          = 126U, ///< EMU3S0 interrupt 6
    PIC_GTM_TIM_SRC_SEL_EMU3S0INT7          = 127U, ///< EMU3S0 interrupt 7
    PIC_GTM_TIM_SRC_SEL_EMU3S1TSTWECMP      = 130U, ///< EMU3S1 write enable for carrier period and comparison values
    PIC_GTM_TIM_SRC_SEL_EMU3S1TSTIUP        = 131U, ///< EMU3S1 rectangle wave output (U phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S1TSTIVP        = 132U, ///< EMU3S1 rectangle wave output (V phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S1TSTIWP        = 133U, ///< EMU3S1 rectangle wave output (W phase)
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT0          = 134U, ///< EMU3S1 interrupt 0
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT1          = 135U, ///< EMU3S1 interrupt 1
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT2          = 136U, ///< EMU3S1 interrupt 2
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT3          = 137U, ///< EMU3S1 interrupt 3
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT4          = 138U, ///< EMU3S1 interrupt 4
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT5          = 139U, ///< EMU3S1 interrupt 5
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT6          = 140U, ///< EMU3S1 interrupt 6
    PIC_GTM_TIM_SRC_SEL_EMU3S1INT7          = 141U, ///< EMU3S1 interrupt 7
    PIC_GTM_TIM_SRC_SEL_CAN0RX              = 144U, ///< CAN0 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN1RX              = 145U, ///< CAN1 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN2RX              = 146U, ///< CAN2 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN3RX              = 147U, ///< CAN3 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN4RX              = 148U, ///< CAN4 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN5RX              = 149U, ///< CAN5 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN6RX              = 150U, ///< CAN6 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN7RX              = 151U, ///< CAN7 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN8RX              = 152U, ///< CAN8 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN9RX              = 153U, ///< CAN9 receive data input
    PIC_GTM_TIM_SRC_SEL_FCMP0_CMP           = 164U, ///< FCMP0 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP0_DAC           = 165U, ///< FCMP0 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP0_CMP_INTR      = 166U, ///< FCMP0 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP0_ERR_INTR      = 167U, ///< FCMP0 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP1_CMP           = 168U, ///< FCMP1 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP1_DAC           = 169U, ///< FCMP1 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP1_CMP_INTR      = 170U, ///< FCMP1 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP1_ERR_INTR      = 171U, ///< FCMP1 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP2_CMP           = 172U, ///< FCMP2 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP2_DAC           = 173U, ///< FCMP2 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP2_CMP_INTR      = 174U, ///< FCMP2 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP2_ERR_INTR      = 175U, ///< FCMP2 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP3_CMP           = 176U, ///< FCMP3 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP3_DAC           = 177U, ///< FCMP3 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP3_CMP_INTR      = 178U, ///< FCMP3 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP3_ERR_INTR      = 179U, ///< FCMP3 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP4_CMP           = 180U, ///< FCMP4 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP4_DAC           = 181U, ///< FCMP4 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP4_CMP_INTR      = 182U, ///< FCMP4 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP4_ERR_INTR      = 183U, ///< FCMP4 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP5_CMP           = 184U, ///< FCMP5 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP5_DAC           = 185U, ///< FCMP5 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP5_CMP_INTR      = 186U, ///< FCMP5 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP5_ERR_INTR      = 187U, ///< FCMP5 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP6_CMP           = 188U, ///< FCMP6 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP6_DAC           = 189U, ///< FCMP6 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP6_CMP_INTR      = 190U, ///< FCMP6 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP6_ERR_INTR      = 191U, ///< FCMP6 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP7_CMP           = 192U, ///< FCMP7 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP7_DAC           = 193U, ///< FCMP7 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP7_CMP_INTR      = 194U, ///< FCMP7 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP7_ERR_INTR      = 195U, ///< FCMP7 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP8_CMP           = 196U, ///< FCMP8 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP8_DAC           = 197U, ///< FCMP8 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP8_CMP_INTR      = 198U, ///< FCMP8 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP8_ERR_INTR      = 199U, ///< FCMP8 error interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP9_CMP           = 200U, ///< FCMP9 comparator output data
    PIC_GTM_TIM_SRC_SEL_FCMP9_DAC           = 201U, ///< FCMP9 DAC data update
    PIC_GTM_TIM_SRC_SEL_FCMP9_CMP_INTR      = 202U, ///< FCMP9 comparator result interrupt
    PIC_GTM_TIM_SRC_SEL_FCMP9_ERR_INTR      = 203U, ///< FCMP9 error interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR0        = 204U, ///< sDMAC0 channel 0 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR1        = 205U, ///< sDMAC0 channel 1 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR2        = 206U, ///< sDMAC0 channel 2 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR3        = 207U, ///< sDMAC0 channel 3 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR4        = 208U, ///< sDMAC0 channel 4 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR5        = 209U, ///< sDMAC0 channel 5 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR6        = 210U, ///< sDMAC0 channel 6 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR7        = 211U, ///< sDMAC0 channel 7 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR8        = 212U, ///< sDMAC0 channel 8 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR9        = 213U, ///< sDMAC0 channel 9 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR10       = 214U, ///< sDMAC0 channel 10 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR11       = 215U, ///< sDMAC0 channel 11 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR12       = 216U, ///< sDMAC0 channel 12 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR13       = 217U, ///< sDMAC0 channel 13 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR14       = 218U, ///< sDMAC0 channel 14 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR15       = 219U, ///< sDMAC0 channel 15 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR0        = 220U, ///< sDMAC1 channel 0 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR1        = 221U, ///< sDMAC1 channel 1 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR2        = 222U, ///< sDMAC1 channel 2 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR3        = 223U, ///< sDMAC1 channel 3 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR4        = 224U, ///< sDMAC1 channel 4 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR5        = 225U, ///< sDMAC1 channel 5 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR6        = 226U, ///< sDMAC1 channel 6 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR7        = 227U, ///< sDMAC1 channel 7 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR8        = 228U, ///< sDMAC1 channel 8 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR9        = 229U, ///< sDMAC1 channel 9 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR10       = 230U, ///< sDMAC1 channel 10 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR11       = 231U, ///< sDMAC1 channel 11 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR12       = 232U, ///< sDMAC1 channel 12 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR13       = 233U, ///< sDMAC1 channel 13 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR14       = 234U, ///< sDMAC1 channel 14 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR15       = 235U, ///< sDMAC1 channel 15 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_DTS_CH0             = 236U, ///< DTS channel 0 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH1             = 237U, ///< DTS channel 1 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH2             = 238U, ///< DTS channel 2 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH3             = 239U, ///< DTS channel 3 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH4             = 240U, ///< DTS channel 4 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH5             = 241U, ///< DTS channel 5 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH6             = 242U, ///< DTS channel 6 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH7             = 243U, ///< DTS channel 7 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH8             = 244U, ///< DTS channel 8 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH9             = 245U, ///< DTS channel 9 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH10            = 246U, ///< DTS channel 10 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH11            = 247U, ///< DTS channel 11 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH12            = 248U, ///< DTS channel 12 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH13            = 249U, ///< DTS channel 13 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH14            = 250U, ///< DTS channel 14 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH15            = 251U, ///< DTS channel 15 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH16            = 252U, ///< DTS channel 16 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH17            = 253U, ///< DTS channel 17 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH18            = 254U, ///< DTS channel 18 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH19            = 255U, ///< DTS channel 19 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH20            = 256U, ///< DTS channel 20 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH21            = 257U, ///< DTS channel 21 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH22            = 258U, ///< DTS channel 22 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH23            = 259U, ///< DTS channel 23 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH24            = 260U, ///< DTS channel 24 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH25            = 261U, ///< DTS channel 25 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH26            = 262U, ///< DTS channel 26 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH27            = 263U, ///< DTS channel 27 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH28            = 264U, ///< DTS channel 28 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH29            = 265U, ///< DTS channel 29 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH30            = 266U, ///< DTS channel 30 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH31            = 267U, ///< DTS channel 31 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH32            = 268U, ///< DTS channel 32 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH33            = 269U, ///< DTS channel 33 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH34            = 270U, ///< DTS channel 34 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH35            = 271U, ///< DTS channel 35 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH36            = 272U, ///< DTS channel 36 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH37            = 273U, ///< DTS channel 37 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH38            = 274U, ///< DTS channel 38 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH39            = 275U, ///< DTS channel 39 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH40            = 276U, ///< DTS channel 40 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH41            = 277U, ///< DTS channel 41 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH42            = 278U, ///< DTS channel 42 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH43            = 279U, ///< DTS channel 43 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH44            = 280U, ///< DTS channel 44 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH45            = 281U, ///< DTS channel 45 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH46            = 282U, ///< DTS channel 46 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH47            = 283U, ///< DTS channel 47 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH48            = 284U, ///< DTS channel 48 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH49            = 285U, ///< DTS channel 49 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH50            = 286U, ///< DTS channel 50 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH51            = 287U, ///< DTS channel 51 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH52            = 288U, ///< DTS channel 52 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH53            = 289U, ///< DTS channel 53 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH54            = 290U, ///< DTS channel 54 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH55            = 291U, ///< DTS channel 55 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH56            = 292U, ///< DTS channel 56 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH57            = 293U, ///< DTS channel 57 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH58            = 294U, ///< DTS channel 58 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH59            = 295U, ///< DTS channel 59 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH60            = 296U, ///< DTS channel 60 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH61            = 297U, ///< DTS channel 61 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH62            = 298U, ///< DTS channel 62 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH63            = 299U, ///< DTS channel 63 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH64            = 300U, ///< DTS channel 64 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH65            = 301U, ///< DTS channel 65 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH66            = 302U, ///< DTS channel 66 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH67            = 303U, ///< DTS channel 67 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH68            = 304U, ///< DTS channel 68 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH69            = 305U, ///< DTS channel 69 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH70            = 306U, ///< DTS channel 70 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH71            = 307U, ///< DTS channel 71 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH72            = 308U, ///< DTS channel 72 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH73            = 309U, ///< DTS channel 73 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH74            = 310U, ///< DTS channel 74 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH75            = 311U, ///< DTS channel 75 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH76            = 312U, ///< DTS channel 76 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH77            = 313U, ///< DTS channel 77 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH78            = 314U, ///< DTS channel 78 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH79            = 315U, ///< DTS channel 79 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH80            = 316U, ///< DTS channel 80 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH81            = 317U, ///< DTS channel 81 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH82            = 318U, ///< DTS channel 82 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH83            = 319U, ///< DTS channel 83 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH84            = 320U, ///< DTS channel 84 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH85            = 321U, ///< DTS channel 85 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH86            = 322U, ///< DTS channel 86 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH87            = 323U, ///< DTS channel 87 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH88            = 324U, ///< DTS channel 88 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH89            = 325U, ///< DTS channel 89 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH90            = 326U, ///< DTS channel 90 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH91            = 327U, ///< DTS channel 91 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH92            = 328U, ///< DTS channel 92 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH93            = 329U, ///< DTS channel 93 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH94            = 330U, ///< DTS channel 94 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH95            = 331U, ///< DTS channel 95 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH96            = 332U, ///< DTS channel 96 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH97            = 333U, ///< DTS channel 97 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH98            = 334U, ///< DTS channel 98 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH99            = 335U, ///< DTS channel 99 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH100           = 336U, ///< DTS channel 100 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH101           = 337U, ///< DTS channel 101 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH102           = 338U, ///< DTS channel 102 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH103           = 339U, ///< DTS channel 103 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH104           = 340U, ///< DTS channel 104 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH105           = 341U, ///< DTS channel 105 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH106           = 342U, ///< DTS channel 106 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH107           = 343U, ///< DTS channel 107 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH108           = 344U, ///< DTS channel 108 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH109           = 345U, ///< DTS channel 109 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH110           = 346U, ///< DTS channel 110 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH111           = 347U, ///< DTS channel 111 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH112           = 348U, ///< DTS channel 112 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH113           = 349U, ///< DTS channel 113 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH114           = 350U, ///< DTS channel 114 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH115           = 351U, ///< DTS channel 115 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH116           = 352U, ///< DTS channel 116 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH117           = 353U, ///< DTS channel 117 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH118           = 354U, ///< DTS channel 118 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH119           = 355U, ///< DTS channel 119 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH120           = 356U, ///< DTS channel 120 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH121           = 357U, ///< DTS channel 121 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH122           = 358U, ///< DTS channel 122 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH123           = 359U, ///< DTS channel 123 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH124           = 360U, ///< DTS channel 124 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH125           = 361U, ///< DTS channel 125 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH126           = 362U, ///< DTS channel 126 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH127           = 363U, ///< DTS channel 127 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_OSTM0               = 364U, ///< OSTM0 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM1               = 365U, ///< OSTM1 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM2               = 366U, ///< OSTM2 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM3               = 367U, ///< OSTM3 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM4               = 368U, ///< OSTM4 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM5               = 369U, ///< OSTM5 interrupt
    PIC_GTM_TIM_SRC_SEL_DSMIF0_INTR0        = 374U, ///< DSMIF0 data update interrupt channel 0
    PIC_GTM_TIM_SRC_SEL_DSMIF0_INTR1        = 375U, ///< DSMIF0 data update interrupt channel 1Table 45.191 PIC2GTMINENk List of Function (6/7)
    PIC_GTM_TIM_SRC_SEL_DSMIF1_INTR0        = 376U, ///< DSMIF0 data update interrupt channel 0
    PIC_GTM_TIM_SRC_SEL_DSMIF1_INTR1        = 377U, ///< DSMIF1 data update interrupt channel 1
} pic_gtm_tim_src_sel_t;
#elif (BSP_FEATURE_PIC_FEATURE_U2CX)

/**  Selects a source of timer input of GTM (TIM) */
typedef enum e_pic_gtm_tim_src_sel
{
    PIC_GTM_TIM_SRC_SEL_GPIO           = 0U,   ///< GPIO (via Port Functions)
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG0      = 1U,   ///< ADCKA scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG1      = 2U,   ///< ADCKA scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG2      = 3U,   ///< ADCKA scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG3      = 4U,   ///< ADCKA scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG4      = 5U,   ///< ADCKA scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCKA_SG_DIAG  = 6U,   ///< ADCKA SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG0      = 7U,   ///< ADCK1 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG1      = 8U,   ///< ADCK1 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG2      = 9U,   ///< ADCK1 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG3      = 10U,  ///< ADCK1 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG4      = 11U,  ///< ADCK1 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK1_SG_DIAG  = 12U,  ///< ADCK1 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG0      = 13U,  ///< ADCK2 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG1      = 14U,  ///< ADCK2 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG2      = 15U,  ///< ADCK2 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG3      = 16U,  ///< ADCK2 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG4      = 17U,  ///< ADCK2 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCK2_SG_DIAG  = 18U,  ///< ADCK2 SG-Diag end interrupt
    PIC_GTM_TIM_SRC_SEL_RLIN30         = 40U,  ///< RLIN30 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN31         = 41U,  ///< RLIN31 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN32         = 42U,  ///< RLIN32 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN33         = 43U,  ///< RLIN33 receive data input
    PIC_GTM_TIM_SRC_SEL_ENCA0_A        = 72U,  ///< ENCA0 Encoder input (phase A)
    PIC_GTM_TIM_SRC_SEL_ENCA0_B        = 73U,  ///< ENCA0 Encoder input (phase B)
    PIC_GTM_TIM_SRC_SEL_ENCA0_Z        = 74U,  ///< ENCA0 Encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_I0       = 75U,  ///< ENCA0 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_I1       = 76U,  ///< ENCA0 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_IEC      = 77U,  ///< ENCA0 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_CNT_CLK  = 78U,  ///< ENCA0 count clock
    PIC_GTM_TIM_SRC_SEL_ENCA0_DOWN_CNT = 79U,  ///< ENCA0 down-count enable signal
    PIC_GTM_TIM_SRC_SEL_CAN0RX         = 144U, ///< CAN0 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN1RX         = 145U, ///< CAN1 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN2RX         = 146U, ///< CAN2 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN3RX         = 147U, ///< CAN3 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN4RX         = 148U, ///< CAN4 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN5RX         = 149U, ///< CAN5 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN6RX         = 150U, ///< CAN6 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN7RX         = 151U, ///< CAN7 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN8RX         = 152U, ///< CAN8 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN9RX         = 153U, ///< CAN9 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN10RX        = 154U, ///< CAN10 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN11RX        = 155U, ///< CAN11 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN12RX        = 156U, ///< CAN12 receive data input
    PIC_GTM_TIM_SRC_SEL_CAN13RX        = 157U, ///< CAN13 receive data input
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR0   = 204U, ///< sDMAC0 channel 0 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR1   = 205U, ///< sDMAC0 channel 1 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR2   = 206U, ///< sDMAC0 channel 2 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR3   = 207U, ///< sDMAC0 channel 3 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR4   = 208U, ///< sDMAC0 channel 4 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR5   = 209U, ///< sDMAC0 channel 5 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR6   = 210U, ///< sDMAC0 channel 6 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR7   = 211U, ///< sDMAC0 channel 7 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR8   = 212U, ///< sDMAC0 channel 8 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR9   = 213U, ///< sDMAC0 channel 9 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR10  = 214U, ///< sDMAC0 channel 10 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR11  = 215U, ///< sDMAC0 channel 11 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR12  = 216U, ///< sDMAC0 channel 12 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR13  = 217U, ///< sDMAC0 channel 13 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR14  = 218U, ///< sDMAC0 channel 14 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC0_INTR15  = 219U, ///< sDMAC0 channel 15 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR0   = 220U, ///< sDMAC1 channel 0 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR1   = 221U, ///< sDMAC1 channel 1 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR2   = 222U, ///< sDMAC1 channel 2 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR3   = 223U, ///< sDMAC1 channel 3 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR4   = 224U, ///< sDMAC1 channel 4 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR5   = 225U, ///< sDMAC1 channel 5 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR6   = 226U, ///< sDMAC1 channel 6 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR7   = 227U, ///< sDMAC1 channel 7 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR8   = 228U, ///< sDMAC1 channel 8 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR9   = 229U, ///< sDMAC1 channel 9 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR10  = 230U, ///< sDMAC1 channel 10 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR11  = 231U, ///< sDMAC1 channel 11 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR12  = 232U, ///< sDMAC1 channel 12 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR13  = 233U, ///< sDMAC1 channel 13 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR14  = 234U, ///< sDMAC1 channel 14 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_SDMAC1_INTR15  = 235U, ///< sDMAC1 channel 15 descriptor step end interrupt
    PIC_GTM_TIM_SRC_SEL_DTS_CH0        = 236U, ///< DTS channel 0 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH1        = 237U, ///< DTS channel 1 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH2        = 238U, ///< DTS channel 2 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH3        = 239U, ///< DTS channel 3 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH4        = 240U, ///< DTS channel 4 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH5        = 241U, ///< DTS channel 5 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH6        = 242U, ///< DTS channel 6 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH7        = 243U, ///< DTS channel 7 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH8        = 244U, ///< DTS channel 8 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH9        = 245U, ///< DTS channel 9 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH10       = 246U, ///< DTS channel 10 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH11       = 247U, ///< DTS channel 11 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH12       = 248U, ///< DTS channel 12 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH13       = 249U, ///< DTS channel 13 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH14       = 250U, ///< DTS channel 14 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH15       = 251U, ///< DTS channel 15 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH16       = 252U, ///< DTS channel 16 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH17       = 253U, ///< DTS channel 17 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH18       = 254U, ///< DTS channel 18 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH19       = 255U, ///< DTS channel 19 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH20       = 256U, ///< DTS channel 20 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH21       = 257U, ///< DTS channel 21 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH22       = 258U, ///< DTS channel 22 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH23       = 259U, ///< DTS channel 23 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH24       = 260U, ///< DTS channel 24 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH25       = 261U, ///< DTS channel 25 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH26       = 262U, ///< DTS channel 26 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH27       = 263U, ///< DTS channel 27 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH28       = 264U, ///< DTS channel 28 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH29       = 265U, ///< DTS channel 29 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH30       = 266U, ///< DTS channel 30 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH31       = 267U, ///< DTS channel 31 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH32       = 268U, ///< DTS channel 32 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH33       = 269U, ///< DTS channel 33 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH34       = 270U, ///< DTS channel 34 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH35       = 271U, ///< DTS channel 35 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH36       = 272U, ///< DTS channel 36 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH37       = 273U, ///< DTS channel 37 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH38       = 274U, ///< DTS channel 38 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH39       = 275U, ///< DTS channel 39 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH40       = 276U, ///< DTS channel 40 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH41       = 277U, ///< DTS channel 41 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH42       = 278U, ///< DTS channel 42 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH43       = 279U, ///< DTS channel 43 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH44       = 280U, ///< DTS channel 44 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH45       = 281U, ///< DTS channel 45 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH46       = 282U, ///< DTS channel 46 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH47       = 283U, ///< DTS channel 47 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH48       = 284U, ///< DTS channel 48 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH49       = 285U, ///< DTS channel 49 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH50       = 286U, ///< DTS channel 50 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH51       = 287U, ///< DTS channel 51 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH52       = 288U, ///< DTS channel 52 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH53       = 289U, ///< DTS channel 53 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH54       = 290U, ///< DTS channel 54 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH55       = 291U, ///< DTS channel 55 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH56       = 292U, ///< DTS channel 56 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH57       = 293U, ///< DTS channel 57 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH58       = 294U, ///< DTS channel 58 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH59       = 295U, ///< DTS channel 59 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH60       = 296U, ///< DTS channel 60 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH61       = 297U, ///< DTS channel 61 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH62       = 298U, ///< DTS channel 62 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH63       = 299U, ///< DTS channel 63 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH64       = 300U, ///< DTS channel 64 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH65       = 301U, ///< DTS channel 65 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH66       = 302U, ///< DTS channel 66 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH67       = 303U, ///< DTS channel 67 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH68       = 304U, ///< DTS channel 68 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH69       = 305U, ///< DTS channel 69 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH70       = 306U, ///< DTS channel 70 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH71       = 307U, ///< DTS channel 71 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH72       = 308U, ///< DTS channel 72 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH73       = 309U, ///< DTS channel 73 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH74       = 310U, ///< DTS channel 74 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH75       = 311U, ///< DTS channel 75 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH76       = 312U, ///< DTS channel 76 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH77       = 313U, ///< DTS channel 77 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH78       = 314U, ///< DTS channel 78 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH79       = 315U, ///< DTS channel 79 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH80       = 316U, ///< DTS channel 80 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH81       = 317U, ///< DTS channel 81 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH82       = 318U, ///< DTS channel 82 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH83       = 319U, ///< DTS channel 83 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH84       = 320U, ///< DTS channel 84 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH85       = 321U, ///< DTS channel 85 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH86       = 322U, ///< DTS channel 86 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH87       = 323U, ///< DTS channel 87 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH88       = 324U, ///< DTS channel 88 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH89       = 325U, ///< DTS channel 89 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH90       = 326U, ///< DTS channel 90 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH91       = 327U, ///< DTS channel 91 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH92       = 328U, ///< DTS channel 92 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH93       = 329U, ///< DTS channel 93 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH94       = 330U, ///< DTS channel 94 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH95       = 331U, ///< DTS channel 95 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH96       = 332U, ///< DTS channel 96 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH97       = 333U, ///< DTS channel 97 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH98       = 334U, ///< DTS channel 98 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH99       = 335U, ///< DTS channel 99 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH100      = 336U, ///< DTS channel 100 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH101      = 337U, ///< DTS channel 101 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH102      = 338U, ///< DTS channel 102 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH103      = 339U, ///< DTS channel 103 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH104      = 340U, ///< DTS channel 104 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH105      = 341U, ///< DTS channel 105 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH106      = 342U, ///< DTS channel 106 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH107      = 343U, ///< DTS channel 107 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH108      = 344U, ///< DTS channel 108 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH109      = 345U, ///< DTS channel 109 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH110      = 346U, ///< DTS channel 110 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH111      = 347U, ///< DTS channel 111 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH112      = 348U, ///< DTS channel 112 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH113      = 349U, ///< DTS channel 113 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH114      = 350U, ///< DTS channel 114 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH115      = 351U, ///< DTS channel 115 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH116      = 352U, ///< DTS channel 116 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH117      = 353U, ///< DTS channel 117 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH118      = 354U, ///< DTS channel 118 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH119      = 355U, ///< DTS channel 119 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH120      = 356U, ///< DTS channel 120 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH121      = 357U, ///< DTS channel 121 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH122      = 358U, ///< DTS channel 122 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH123      = 359U, ///< DTS channel 123 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH124      = 360U, ///< DTS channel 124 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH125      = 361U, ///< DTS channel 125 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH126      = 362U, ///< DTS channel 126 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_DTS_CH127      = 363U, ///< DTS channel 127 transfer end or transfer count match
    PIC_GTM_TIM_SRC_SEL_OSTM0          = 364U, ///< OSTM0 interrupt
    PIC_GTM_TIM_SRC_SEL_OSTM1          = 365U, ///< OSTM1 interrupt
    PIC_GTM_TIM_SRC_SEL_PIC1_SST       = 374U, ///< PIC1 synchronous start trigger
    PIC_GTM_TIM_SRC_SEL_TAUD0_CH14     = 375U, ///< TAUD0 channel 14 interrupt signal
    PIC_GTM_TIM_SRC_SEL_TAUD0_CH15     = 376U, ///< TAUD0 channel 15 interrupt signal
    PIC_GTM_TIM_SRC_SEL_TSG30_INT0     = 377U, ///< Timer interrupt 0 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT1     = 378U, ///< Timer interrupt 1 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT2     = 379U, ///< Timer interrupt 2 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT3     = 380U, ///< Timer interrupt 3 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT4     = 381U, ///< Timer interrupt 4 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT5     = 382U, ///< Timer interrupt 5 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT6     = 383U, ///< Timer interrupt 6 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT7     = 384U, ///< Timer interrupt 7 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT8     = 385U, ///< Timer interrupt 8 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT9     = 386U, ///< Timer interrupt 9 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT10    = 387U, ///< Timer interrupt 10 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT11    = 388U, ///< Timer interrupt 11 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_INT12    = 389U, ///< Timer interrupt 12 (TSG30)
    PIC_GTM_TIM_SRC_SEL_TSG30_PEAK     = 390U, ///< TSG30 peak interrupt
    PIC_GTM_TIM_SRC_SEL_TSG30_VALLEY   = 391U, ///< TSG30 valley interrupt
    PIC_GTM_TIM_SRC_SEL_TSG31_INT0     = 392U, ///< Timer interrupt 0 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT1     = 393U, ///< Timer interrupt 1 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT2     = 394U, ///< Timer interrupt 2 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT3     = 395U, ///< Timer interrupt 3 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT4     = 396U, ///< Timer interrupt 4 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT5     = 397U, ///< Timer interrupt 5 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT6     = 398U, ///< Timer interrupt 6 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT7     = 399U, ///< Timer interrupt 7 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT8     = 400U, ///< Timer interrupt 8 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT9     = 401U, ///< Timer interrupt 9 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT10    = 402U, ///< Timer interrupt 10 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT11    = 403U, ///< Timer interrupt 11 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_INT12    = 404U, ///< Timer interrupt 12 (TSG31)
    PIC_GTM_TIM_SRC_SEL_TSG31_PEAK     = 405U, ///< TSG31 peak interrupt
    PIC_GTM_TIM_SRC_SEL_TSG31_VALLEY   = 406U, ///< TSG31 valley interrupt
} pic_gtm_tim_src_sel_t;
#elif (BSP_FEATURE_PIC_FEATURE_U2AX)

/**  Selects a source of timer input of GTM (TIM) */
typedef enum e_pic_gtm_tim_src_sel
{
    PIC_GTM_TIM_SRC_SEL_GPIO           = 0U,  ///< GPIO (via Port Functions)
    PIC_GTM_TIM_SRC_SEL_ADCJ0_SG0      = 1U,  ///< ADCJ0 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ0_SG1      = 2U,  ///< ADCJ0 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ0_SG2      = 3U,  ///< ADCJ0 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ0_SG3      = 4U,  ///< ADCJ0 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ0_SG4      = 5U,  ///< ADCJ0 scan group 4 (SG4) end Interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ1_SG0      = 6U,  ///< ADCJ1 scan group 0 (SG0) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ1_SG1      = 7U,  ///< ADCJ1 scan group 1 (SG1) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ1_SG2      = 8U,  ///< ADCJ1 scan group 2 (SG2) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ1_SG3      = 9U,  ///< ADCJ1 scan group 3 (SG3) end interrupt
    PIC_GTM_TIM_SRC_SEL_ADCJ1_SG4      = 10U, ///< ADCJ1 scan group 4 (SG4) end interrupt
    PIC_GTM_TIM_SRC_SEL_RLIN30         = 33U, ///< RLIN30 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN31         = 34U, ///< RLIN31 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN32         = 35U, ///< RLIN32 receive data input
    PIC_GTM_TIM_SRC_SEL_RLIN33         = 36U, ///< RLIN33 receive data input
    PIC_GTM_TIM_SRC_SEL_ENCA0_A        = 65U, ///< ENCA0 Encoder input (phase A)
    PIC_GTM_TIM_SRC_SEL_ENCA0_B        = 66U, ///< ENCA0 Encoder input (phase B)
    PIC_GTM_TIM_SRC_SEL_ENCA0_Z        = 67U, ///< ENCA0 Encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_I0       = 68U, ///< ENCA0 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_I1       = 69U, ///< ENCA0 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA0_IEC      = 70U, ///< ENCA0 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA0_CNT_CLK  = 71U, ///< ENCA0 count clock
    PIC_GTM_TIM_SRC_SEL_ENCA0_DOWN_CNT = 72U, ///< ENCA0 down-count enable signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_A        = 73U, ///< ENCA1 Encoder input (phase A)
    PIC_GTM_TIM_SRC_SEL_ENCA1_B        = 74U, ///< ENCA1 Encoder input (phase B)
    PIC_GTM_TIM_SRC_SEL_ENCA1_Z        = 75U, ///< ENCA1 Encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA1_I0       = 76U, ///< ENCA1 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_I1       = 77U, ///< ENCA1 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIM_SRC_SEL_ENCA1_IEC      = 78U, ///< ENCA1 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIM_SRC_SEL_ENCA1_CNT_CLK  = 79U, ///< ENCA1 count clock
    PIC_GTM_TIM_SRC_SEL_ENCA1_DOWN_CNT = 80U, ///< ENCA1 down-count enable signal
} pic_gtm_tim_src_sel_t;
#endif

#if (BSP_FEATURE_PIC_FEATURE_U2BX)

/**  Selects a source of timer input of GTM (TIO) */
typedef enum e_pic_gtm_tio_src_sel
{
    PIC_GTM_TIO_SRC_SEL_GPIO           = 0U,  ///< GPIO (via Port Functions)
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG0      = 1U,  ///< ADCK0 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG1      = 2U,  ///< ADCK0 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG2      = 3U,  ///< ADCK0 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG3      = 4U,  ///< ADCK0 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG4      = 5U,  ///< ADCK0 scan group 4 (SG4) end Interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK0_SG       = 6U,  ///< ADCK0 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG0      = 7U,  ///< ADCK1 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG1      = 8U,  ///< ADCK1 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG2      = 9U,  ///< ADCK1 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG3      = 10U, ///< ADCK1 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG4      = 11U, ///< ADCK1 scan group 4 (SG4) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG       = 12U, ///< ADCK1 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG0      = 13U, ///< ADCK2 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG1      = 14U, ///< ADCK2 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG2      = 15U, ///< ADCK2 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG3      = 16U, ///< ADCK2 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG4      = 17U, ///< ADCK2 scan group 4 (SG4) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG       = 18U, ///< ADCK2 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG0      = 19U, ///< ADCK3 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG1      = 20U, ///< ADCK3 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG2      = 21U, ///< ADCK3 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG3      = 22U, ///< ADCK3 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG4      = 23U, ///< ADCK3 scan group 4 (SG4) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK3_SG       = 24U, ///< ADCK3 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC00        = 25U, ///< DSADC00 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC10        = 26U, ///< DSADC10 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC20        = 27U, ///< DSADC20 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC12        = 28U, ///< DSADC12 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC17        = 29U, ///< DSADC17 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC16        = 30U, ///< DSADC16 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC13        = 33U, ///< DSADC13 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC11        = 34U, ///< DSADC11 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC31        = 35U, ///< DSADC31 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC30        = 36U, ///< DSADC30 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC33        = 37U, ///< DSADC33 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DSADC32        = 38U, ///< DSADC32 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_CADC00         = 39U, ///< CADC00 A/D conversion end interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_TR0       = 44U, ///< DFE0 P/H updating trigger 0
    PIC_GTM_TIO_SRC_SEL_DFE0_TR1       = 45U, ///< DFE0 P/H updating trigger 1
    PIC_GTM_TIO_SRC_SEL_DFE0_TR2       = 46U, ///< DFE0 P/H updating trigger 2
    PIC_GTM_TIO_SRC_SEL_DFE0_TR3       = 47U, ///< DFE0 P/H updating trigger 3
    PIC_GTM_TIO_SRC_SEL_DFE0_CH0       = 48U, ///< DFE0 CH0 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH1       = 49U, ///< DFE0 CH1 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH2       = 50U, ///< DFE0 CH2 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH3       = 51U, ///< DFE0 CH3 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH4       = 52U, ///< DFE0 CH4 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH5       = 53U, ///< DFE0 CH5 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH6       = 54U, ///< DFE0 CH6 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH7       = 55U, ///< DFE0 CH7 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH8       = 56U, ///< DFE0 CH8 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH9       = 57U, ///< DFE0 CH9 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH10      = 58U, ///< DFE0 CH10 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH11      = 59U, ///< DFE0 CH11 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH12      = 60U, ///< DFE0 CH12 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH13      = 61U, ///< DFE0 CH13 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH14      = 62U, ///< DFE0 CH14 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE0_CH15      = 63U, ///< DFE0 CH15 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE1_TR0       = 64U, ///< DFE1 P/H updating trigger 0
    PIC_GTM_TIO_SRC_SEL_DFE1_TR1       = 65U, ///< DFE1 P/H updating trigger 1
    PIC_GTM_TIO_SRC_SEL_DFE1_TR2       = 66U, ///< DFE1 P/H updating trigger 2
    PIC_GTM_TIO_SRC_SEL_DFE1_TR3       = 67U, ///< DFE1 P/H updating trigger 3
    PIC_GTM_TIO_SRC_SEL_DFE1_CH0       = 68U, ///< DFE1 CH0 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE1_CH1       = 69U, ///< DFE1 CH1 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE1_CH2       = 70U, ///< DFE1 CH2 output data interrupt
    PIC_GTM_TIO_SRC_SEL_DFE1_CH3       = 71U, ///< DFE1 CH3 output data interrupt
    PIC_GTM_TIO_SRC_SEL_ENCA0_A        = 72U, ///< ENCA0 Encoder input (phase A)
    PIC_GTM_TIO_SRC_SEL_ENCA0_B        = 73U, ///< ENCA0 Encoder input (phase B)
    PIC_GTM_TIO_SRC_SEL_ENCA0_Z        = 74U, ///< ENCA0 Encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA0_INT0     = 75U, ///< ENCA0 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA0_INT1     = 76U, ///< ENCA0 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA0_CL_INT   = 77U, ///< ENCA0 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA0_CNT_CLK  = 78U, ///< ENCA0 count clock
    PIC_GTM_TIO_SRC_SEL_ENCA0_DOWN_CNT = 79U, ///< ENCA0 down-count enable signal
    PIC_GTM_TIO_SRC_SEL_ENCA1_A        = 80U, ///< ENCA1 Encoder input (phase A)
    PIC_GTM_TIO_SRC_SEL_ENCA1_B        = 81U, ///< ENCA1 Encoder input (phase B)
    PIC_GTM_TIO_SRC_SEL_ENCA1_Z        = 82U, ///< ENCA1 Encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA1_INT0     = 83U, ///< ENCA1 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA1_INT1     = 84U, ///< ENCA1 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA1_CL_INT   = 85U, ///< ENCA1 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA1_CNT_CLK  = 86U, ///< ENCA1 count clock
    PIC_GTM_TIO_SRC_SEL_ENCA1_DOWN_CNT = 87U, ///< ENCA1 down-count enable signal
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH0     = 204U, ///< sDMAC0 channel 0 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH1     = 205U, ///< sDMAC0 channel 1 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH2     = 206U, ///< sDMAC0 channel 2 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH3     = 207U, ///< sDMAC0 channel 3 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH4     = 208U, ///< sDMAC0 channel 4 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH5     = 209U, ///< sDMAC0 channel 5 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH6     = 210U, ///< sDMAC0 channel 6 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH7     = 211U, ///< sDMAC0 channel 7 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH8     = 212U, ///< sDMAC0 channel 8 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH9     = 213U, ///< sDMAC0 channel 9 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH10    = 214U, ///< sDMAC0 channel 10 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH11    = 215U, ///< sDMAC0 channel 11 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH12    = 216U, ///< sDMAC0 channel 12 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH13    = 217U, ///< sDMAC0 channel 13 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH14    = 218U, ///< sDMAC0 channel 14 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH15    = 219U, ///< sDMAC0 channel 15 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH0     = 220U, ///< sDMAC1 channel 0 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH1     = 221U, ///< sDMAC1 channel 1 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH2     = 222U, ///< sDMAC1 channel 2 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH3     = 223U, ///< sDMAC1 channel 3 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH4     = 224U, ///< sDMAC1 channel 4 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH5     = 225U, ///< sDMAC1 channel 5 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH6     = 226U, ///< sDMAC1 channel 6 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH7     = 227U, ///< sDMAC1 channel 7 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH8     = 228U, ///< sDMAC1 channel 8 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH9     = 229U, ///< sDMAC1 channel 9 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH10    = 230U, ///< sDMAC1 channel 10 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH11    = 231U, ///< sDMAC1 channel 11 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH12    = 232U, ///< sDMAC1 channel 12 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH13    = 233U, ///< sDMAC1 channel 13 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH14    = 234U, ///< sDMAC1 channel 14 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH15    = 235U, ///< sDMAC1 channel 15 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_DTS_CH0        = 236U, ///< DTS channel 0 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH1        = 237U, ///< DTS channel 1 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH2        = 238U, ///< DTS channel 2 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH3        = 239U, ///< DTS channel 3 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH4        = 240U, ///< DTS channel 4 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH5        = 241U, ///< DTS channel 5 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH6        = 242U, ///< DTS channel 6 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH7        = 243U, ///< DTS channel 7 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH8        = 244U, ///< DTS channel 8 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH9        = 245U, ///< DTS channel 9 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH10       = 246U, ///< DTS channel 10 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH11       = 247U, ///< DTS channel 11 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH12       = 248U, ///< DTS channel 12 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH13       = 249U, ///< DTS channel 13 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH14       = 250U, ///< DTS channel 14 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH15       = 251U, ///< DTS channel 15 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH16       = 252U, ///< DTS channel 16 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH17       = 253U, ///< DTS channel 17 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH18       = 254U, ///< DTS channel 18 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH19       = 255U, ///< DTS channel 19 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH20       = 256U, ///< DTS channel 20 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH21       = 257U, ///< DTS channel 21 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH22       = 258U, ///< DTS channel 22 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH23       = 259U, ///< DTS channel 23 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH24       = 260U, ///< DTS channel 24 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH25       = 261U, ///< DTS channel 25 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH26       = 262U, ///< DTS channel 26 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH27       = 263U, ///< DTS channel 27 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH28       = 264U, ///< DTS channel 28 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH29       = 265U, ///< DTS channel 29 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH30       = 266U, ///< DTS channel 30 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH31       = 267U, ///< DTS channel 31 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH32       = 268U, ///< DTS channel 32 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH33       = 269U, ///< DTS channel 33 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH34       = 270U, ///< DTS channel 34 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH35       = 271U, ///< DTS channel 35 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH36       = 272U, ///< DTS channel 36 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH37       = 273U, ///< DTS channel 37 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH38       = 274U, ///< DTS channel 38 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH39       = 275U, ///< DTS channel 39 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH40       = 276U, ///< DTS channel 40 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH41       = 277U, ///< DTS channel 41 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH42       = 278U, ///< DTS channel 42 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH43       = 279U, ///< DTS channel 43 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH44       = 280U, ///< DTS channel 44 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH45       = 281U, ///< DTS channel 45 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH46       = 282U, ///< DTS channel 46 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH47       = 283U, ///< DTS channel 47 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH48       = 284U, ///< DTS channel 48 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH49       = 285U, ///< DTS channel 49 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH50       = 286U, ///< DTS channel 50 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH51       = 287U, ///< DTS channel 51 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH52       = 288U, ///< DTS channel 52 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH53       = 289U, ///< DTS channel 53 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH54       = 290U, ///< DTS channel 54 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH55       = 291U, ///< DTS channel 55 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH56       = 292U, ///< DTS channel 56 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH57       = 293U, ///< DTS channel 57 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH58       = 294U, ///< DTS channel 58 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH59       = 295U, ///< DTS channel 59 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH60       = 296U, ///< DTS channel 60 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH61       = 297U, ///< DTS channel 61 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH62       = 298U, ///< DTS channel 62 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH63       = 299U, ///< DTS channel 63 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH64       = 300U, ///< DTS channel 64 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH65       = 301U, ///< DTS channel 65 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH66       = 302U, ///< DTS channel 66 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH67       = 303U, ///< DTS channel 67 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH68       = 304U, ///< DTS channel 68 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH69       = 305U, ///< DTS channel 69 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH70       = 306U, ///< DTS channel 70 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH71       = 307U, ///< DTS channel 71 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH72       = 308U, ///< DTS channel 72 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH73       = 309U, ///< DTS channel 73 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH74       = 310U, ///< DTS channel 74 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH75       = 311U, ///< DTS channel 75 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH76       = 312U, ///< DTS channel 76 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH77       = 313U, ///< DTS channel 77 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH78       = 314U, ///< DTS channel 78 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH79       = 315U, ///< DTS channel 79 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH80       = 316U, ///< DTS channel 80 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH81       = 317U, ///< DTS channel 81 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH82       = 318U, ///< DTS channel 82 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH83       = 319U, ///< DTS channel 83 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH84       = 320U, ///< DTS channel 84 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH85       = 321U, ///< DTS channel 85 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH86       = 322U, ///< DTS channel 86 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH87       = 323U, ///< DTS channel 87 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH88       = 324U, ///< DTS channel 88 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH89       = 325U, ///< DTS channel 89 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH90       = 326U, ///< DTS channel 90 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH91       = 327U, ///< DTS channel 91 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH92       = 328U, ///< DTS channel 92 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH93       = 329U, ///< DTS channel 93 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH94       = 330U, ///< DTS channel 94 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH95       = 331U, ///< DTS channel 95 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH96       = 332U, ///< DTS channel 96 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH97       = 333U, ///< DTS channel 97 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH98       = 334U, ///< DTS channel 98 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH99       = 335U, ///< DTS channel 99 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH100      = 336U, ///< DTS channel 100 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH101      = 337U, ///< DTS channel 101 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH102      = 338U, ///< DTS channel 102 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH103      = 339U, ///< DTS channel 103 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH104      = 340U, ///< DTS channel 104 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH105      = 341U, ///< DTS channel 105 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH106      = 342U, ///< DTS channel 106 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH107      = 343U, ///< DTS channel 107 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH108      = 344U, ///< DTS channel 108 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH109      = 345U, ///< DTS channel 109 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH110      = 346U, ///< DTS channel 110 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH111      = 347U, ///< DTS channel 111 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH112      = 348U, ///< DTS channel 112 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH113      = 349U, ///< DTS channel 113 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH114      = 350U, ///< DTS channel 114 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH115      = 351U, ///< DTS channel 115 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH116      = 352U, ///< DTS channel 116 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH117      = 353U, ///< DTS channel 117 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH118      = 354U, ///< DTS channel 118 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH119      = 355U, ///< DTS channel 119 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH120      = 356U, ///< DTS channel 120 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH121      = 357U, ///< DTS channel 121 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH122      = 358U, ///< DTS channel 122 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH123      = 359U, ///< DTS channel 123 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH124      = 360U, ///< DTS channel 124 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH125      = 361U, ///< DTS channel 125 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH126      = 362U, ///< DTS channel 126 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH127      = 363U, ///< DTS channel 127 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_OSTM0          = 364U, ///< OSTM0 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM1          = 365U, ///< OSTM1 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM2          = 366U, ///< OSTM2 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM3          = 367U, ///< OSTM3 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM4          = 368U, ///< OSTM4 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM5          = 369U, ///< OSTM5 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM6          = 370U, ///< OSTM6 interrupt
    PIC_GTM_TIO_SRC_SEL_DSMIF0_INT0    = 374U, ///< DSMIF0 data update interrupt channel 0
    PIC_GTM_TIO_SRC_SEL_DSMIF0_INT1    = 375U, ///< DSMIF0 data update interrupt channel 1
    PIC_GTM_TIO_SRC_SEL_DSMIF1_INT0    = 376U, ///< DSMIF1 data update interrupt channel 0
    PIC_GTM_TIO_SRC_SEL_DSMIF1_INT1    = 377U, ///< DSMIF1 data update interrupt channel 1
} pic_gtm_tio_src_sel_t;
 #elif (BSP_FEATURE_PIC_FEATURE_U2CX)

/**  Selects a source of timer input of GTM (TIO) */
typedef enum e_pic_gtm_tio_src_sel
{
    PIC_GTM_TIO_SRC_SEL_GPIO           = 0U,  ///< GPIO (via Port Functions)
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG0      = 1U,  ///< ADCKA scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG1      = 2U,  ///< ADCKA scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG2      = 3U,  ///< ADCKA scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG3      = 4U,  ///< ADCKA scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG4      = 5U,  ///< ADCKA scan group 4 (SG4) end Interrupt
    PIC_GTM_TIO_SRC_SEL_ADCKA_SG       = 6U,  ///< ADCKA SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG0      = 7U,  ///< ADCK1 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG1      = 8U,  ///< ADCK1 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG2      = 9U,  ///< ADCK1 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG3      = 10U, ///< ADCK1 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG4      = 11U, ///< ADCK1 scan group 4 (SG4) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK1_SG       = 12U, ///< ADCK1 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG0      = 13U, ///< ADCK2 scan group 0 (SG0) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG1      = 14U, ///< ADCK2 scan group 1 (SG1) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG2      = 15U, ///< ADCK2 scan group 2 (SG2) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG3      = 16U, ///< ADCK2 scan group 3 (SG3) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG4      = 17U, ///< ADCK2 scan group 4 (SG4) end interrupt
    PIC_GTM_TIO_SRC_SEL_ADCK2_SG       = 18U, ///< ADCK2 SG-Diag end interrupt
    PIC_GTM_TIO_SRC_SEL_ENCA0_A        = 72U, ///< ENCA0 Encoder input (phase A)
    PIC_GTM_TIO_SRC_SEL_ENCA0_B        = 73U, ///< ENCA0 Encoder input (phase B)
    PIC_GTM_TIO_SRC_SEL_ENCA0_Z        = 74U, ///< ENCA0 Encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA0_INT0     = 75U, ///< ENCA0 Compare 0 match or Capture 0 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA0_INT1     = 76U, ///< ENCA0 Compare 1 match or Capture 1 interrupt signal
    PIC_GTM_TIO_SRC_SEL_ENCA0_CL_INT   = 77U, ///< ENCA0 Clear interrupt signal by encoder input (phase Z)
    PIC_GTM_TIO_SRC_SEL_ENCA0_CNT_CLK  = 78U, ///< ENCA0 count clock
    PIC_GTM_TIO_SRC_SEL_ENCA0_DOWN_CNT = 79U, ///< ENCA0 down-count enable signal
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH0     = 204U, ///< sDMAC0 channel 0 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH1     = 205U, ///< sDMAC0 channel 1 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH2     = 206U, ///< sDMAC0 channel 2 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH3     = 207U, ///< sDMAC0 channel 3 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH4     = 208U, ///< sDMAC0 channel 4 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH5     = 209U, ///< sDMAC0 channel 5 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH6     = 210U, ///< sDMAC0 channel 6 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH7     = 211U, ///< sDMAC0 channel 7 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH8     = 212U, ///< sDMAC0 channel 8 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH9     = 213U, ///< sDMAC0 channel 9 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH10    = 214U, ///< sDMAC0 channel 10 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH11    = 215U, ///< sDMAC0 channel 11 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH12    = 216U, ///< sDMAC0 channel 12 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH13    = 217U, ///< sDMAC0 channel 13 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH14    = 218U, ///< sDMAC0 channel 14 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC0_CH15    = 219U, ///< sDMAC0 channel 15 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH0     = 220U, ///< sDMAC1 channel 0 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH1     = 221U, ///< sDMAC1 channel 1 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH2     = 222U, ///< sDMAC1 channel 2 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH3     = 223U, ///< sDMAC1 channel 3 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH4     = 224U, ///< sDMAC1 channel 4 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH5     = 225U, ///< sDMAC1 channel 5 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH6     = 226U, ///< sDMAC1 channel 6 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH7     = 227U, ///< sDMAC1 channel 7 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH8     = 228U, ///< sDMAC1 channel 8 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH9     = 229U, ///< sDMAC1 channel 9 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH10    = 230U, ///< sDMAC1 channel 10 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH11    = 231U, ///< sDMAC1 channel 11 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH12    = 232U, ///< sDMAC1 channel 12 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH13    = 233U, ///< sDMAC1 channel 13 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH14    = 234U, ///< sDMAC1 channel 14 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_sDMAC1_CH15    = 235U, ///< sDMAC1 channel 15 descriptor step end interrupt
    PIC_GTM_TIO_SRC_SEL_DTS_CH0        = 236U, ///< DTS channel 0 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH1        = 237U, ///< DTS channel 1 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH2        = 238U, ///< DTS channel 2 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH3        = 239U, ///< DTS channel 3 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH4        = 240U, ///< DTS channel 4 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH5        = 241U, ///< DTS channel 5 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH6        = 242U, ///< DTS channel 6 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH7        = 243U, ///< DTS channel 7 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH8        = 244U, ///< DTS channel 8 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH9        = 245U, ///< DTS channel 9 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH10       = 246U, ///< DTS channel 10 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH11       = 247U, ///< DTS channel 11 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH12       = 248U, ///< DTS channel 12 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH13       = 249U, ///< DTS channel 13 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH14       = 250U, ///< DTS channel 14 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH15       = 251U, ///< DTS channel 15 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH16       = 252U, ///< DTS channel 16 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH17       = 253U, ///< DTS channel 17 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH18       = 254U, ///< DTS channel 18 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH19       = 255U, ///< DTS channel 19 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH20       = 256U, ///< DTS channel 20 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH21       = 257U, ///< DTS channel 21 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH22       = 258U, ///< DTS channel 22 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH23       = 259U, ///< DTS channel 23 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH24       = 260U, ///< DTS channel 24 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH25       = 261U, ///< DTS channel 25 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH26       = 262U, ///< DTS channel 26 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH27       = 263U, ///< DTS channel 27 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH28       = 264U, ///< DTS channel 28 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH29       = 265U, ///< DTS channel 29 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH30       = 266U, ///< DTS channel 30 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH31       = 267U, ///< DTS channel 31 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH32       = 268U, ///< DTS channel 32 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH33       = 269U, ///< DTS channel 33 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH34       = 270U, ///< DTS channel 34 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH35       = 271U, ///< DTS channel 35 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH36       = 272U, ///< DTS channel 36 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH37       = 273U, ///< DTS channel 37 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH38       = 274U, ///< DTS channel 38 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH39       = 275U, ///< DTS channel 39 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH40       = 276U, ///< DTS channel 40 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH41       = 277U, ///< DTS channel 41 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH42       = 278U, ///< DTS channel 42 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH43       = 279U, ///< DTS channel 43 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH44       = 280U, ///< DTS channel 44 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH45       = 281U, ///< DTS channel 45 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH46       = 282U, ///< DTS channel 46 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH47       = 283U, ///< DTS channel 47 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH48       = 284U, ///< DTS channel 48 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH49       = 285U, ///< DTS channel 49 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH50       = 286U, ///< DTS channel 50 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH51       = 287U, ///< DTS channel 51 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH52       = 288U, ///< DTS channel 52 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH53       = 289U, ///< DTS channel 53 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH54       = 290U, ///< DTS channel 54 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH55       = 291U, ///< DTS channel 55 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH56       = 292U, ///< DTS channel 56 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH57       = 293U, ///< DTS channel 57 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH58       = 294U, ///< DTS channel 58 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH59       = 295U, ///< DTS channel 59 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH60       = 296U, ///< DTS channel 60 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH61       = 297U, ///< DTS channel 61 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH62       = 298U, ///< DTS channel 62 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH63       = 299U, ///< DTS channel 63 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH64       = 300U, ///< DTS channel 64 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH65       = 301U, ///< DTS channel 65 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH66       = 302U, ///< DTS channel 66 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH67       = 303U, ///< DTS channel 67 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH68       = 304U, ///< DTS channel 68 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH69       = 305U, ///< DTS channel 69 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH70       = 306U, ///< DTS channel 70 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH71       = 307U, ///< DTS channel 71 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH72       = 308U, ///< DTS channel 72 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH73       = 309U, ///< DTS channel 73 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH74       = 310U, ///< DTS channel 74 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH75       = 311U, ///< DTS channel 75 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH76       = 312U, ///< DTS channel 76 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH77       = 313U, ///< DTS channel 77 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH78       = 314U, ///< DTS channel 78 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH79       = 315U, ///< DTS channel 79 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH80       = 316U, ///< DTS channel 80 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH81       = 317U, ///< DTS channel 81 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH82       = 318U, ///< DTS channel 82 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH83       = 319U, ///< DTS channel 83 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH84       = 320U, ///< DTS channel 84 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH85       = 321U, ///< DTS channel 85 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH86       = 322U, ///< DTS channel 86 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH87       = 323U, ///< DTS channel 87 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH88       = 324U, ///< DTS channel 88 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH89       = 325U, ///< DTS channel 89 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH90       = 326U, ///< DTS channel 90 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH91       = 327U, ///< DTS channel 91 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH92       = 328U, ///< DTS channel 92 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH93       = 329U, ///< DTS channel 93 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH94       = 330U, ///< DTS channel 94 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH95       = 331U, ///< DTS channel 95 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH96       = 332U, ///< DTS channel 96 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH97       = 333U, ///< DTS channel 97 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH98       = 334U, ///< DTS channel 98 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH99       = 335U, ///< DTS channel 99 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH100      = 336U, ///< DTS channel 100 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH101      = 337U, ///< DTS channel 101 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH102      = 338U, ///< DTS channel 102 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH103      = 339U, ///< DTS channel 103 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH104      = 340U, ///< DTS channel 104 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH105      = 341U, ///< DTS channel 105 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH106      = 342U, ///< DTS channel 106 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH107      = 343U, ///< DTS channel 107 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH108      = 344U, ///< DTS channel 108 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH109      = 345U, ///< DTS channel 109 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH110      = 346U, ///< DTS channel 110 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH111      = 347U, ///< DTS channel 111 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH112      = 348U, ///< DTS channel 112 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH113      = 349U, ///< DTS channel 113 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH114      = 350U, ///< DTS channel 114 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH115      = 351U, ///< DTS channel 115 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH116      = 352U, ///< DTS channel 116 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH117      = 353U, ///< DTS channel 117 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH118      = 354U, ///< DTS channel 118 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH119      = 355U, ///< DTS channel 119 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH120      = 356U, ///< DTS channel 120 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH121      = 357U, ///< DTS channel 121 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH122      = 358U, ///< DTS channel 122 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH123      = 359U, ///< DTS channel 123 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH124      = 360U, ///< DTS channel 124 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH125      = 361U, ///< DTS channel 125 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH126      = 362U, ///< DTS channel 126 transfer end or transfer count match
    PIC_GTM_TIO_SRC_SEL_DTS_CH127      = 363U, ///< DTS channel 127 transfer end or transfer count match 
    PIC_GTM_TIO_SRC_SEL_OSTM0          = 364U, ///< OSTM0 interrupt
    PIC_GTM_TIO_SRC_SEL_OSTM1          = 365U, ///< OSTM1 interrupt
    PIC_GTM_TIO_SRC_SEL_PIC1_TRG       = 374U, ///< PIC1 synchronous start trigger
    PIC_GTM_TIO_SRC_SEL_TAUD0_CH1014   = 375U, ///< TAUD0 channel 14 interrupt signal
    PIC_GTM_TIO_SRC_SEL_TAUD0_CH1015   = 376U, ///< TAUD0 channel 15 interrupt signal
    PIC_GTM_TIO_SRC_SEL_TSG30_INT0     = 377U, ///< Timer interrupt 0 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT1     = 378U, ///< Timer interrupt 1 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT2     = 379U, ///< Timer interrupt 2 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT3     = 380U, ///< Timer interrupt 3 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT4     = 381U, ///< Timer interrupt 4 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT5     = 382U, ///< Timer interrupt 5 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT6     = 383U, ///< Timer interrupt 6 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT7     = 384U, ///< Timer interrupt 7 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT8     = 385U, ///< Timer interrupt 8 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT9     = 386U, ///< Timer interrupt 9 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT10    = 387U, ///< Timer interrupt 10 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT11    = 388U, ///< Timer interrupt 11 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_INT12    = 389U, ///< Timer interrupt 12 (TSG30)
    PIC_GTM_TIO_SRC_SEL_TSG30_PEAK     = 390U, ///< TSG30 peak interrupt
    PIC_GTM_TIO_SRC_SEL_TSG30_VALLEY   = 391U, ///< TSG30 valley interrupt
    PIC_GTM_TIO_SRC_SEL_TSG31_CH0      = 392U, ///< Timer interrupt 0 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH1      = 393U, ///< Timer interrupt 1 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH2      = 394U, ///< Timer interrupt 2 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH3      = 395U, ///< Timer interrupt 3 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH4      = 396U, ///< Timer interrupt 4 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH5      = 397U, ///< Timer interrupt 5 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH6      = 398U, ///< Timer interrupt 6 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH7      = 399U, ///< Timer interrupt 7 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH8      = 400U, ///< Timer interrupt 8 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_CH9      = 401U, ///< Timer interrupt 9 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_INT10    = 402U, ///< Timer interrupt 10 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_INT11    = 403U, ///< Timer interrupt 11 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_INT12    = 404U, ///< Timer interrupt 12 (TSG31)
    PIC_GTM_TIO_SRC_SEL_TSG31_PEAK     = 405U, ///< TSG31 peak interrupt
    PIC_GTM_TIO_SRC_SEL_TSG31_VALLEY   = 406U, ///< TSG31 valley interrupt
} pic_gtm_tio_src_sel_t;

/** PWM signal active level */
typedef enum e_pic_active_level
{
    PIC_ACTIVE_LEVEL_NONE = 0x0,       ///< PWM signal was disable.
    PIC_ACTIVE_LEVEL_HIGH = 0x1,       ///< PWM signal active high.
    PIC_ACTIVE_LEVEL_LOW  = 0x2        ///< PWM signal active low.
} pic_active_level_t;

/** Set a trigger source from TAUDn channel m. (n = 0 to 2; j = 0 to 5) */
typedef enum e_pic_adc_trigger_sel_taud_channel
{
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_00 = (0x0001U), ///< Trigger source of TAUDn channel 0 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_01 = (0x0002U), ///< Trigger source of TAUDn channel 1 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_02 = (0x0004U), ///< Trigger source of TAUDn channel 2 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_03 = (0x0008U), ///< Trigger source of TAUDn channel 3 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_04 = (0x0010U), ///< Trigger source of TAUDn channel 4 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_05 = (0x0020U), ///< Trigger source of TAUDn channel 5 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_06 = (0x0040U), ///< Trigger source of TAUDn channel 6 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_07 = (0x0080U), ///< Trigger source of TAUDn channel 7 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_08 = (0x0100U), ///< Trigger source of TAUDn channel 8 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_09 = (0x0200U), ///< Trigger source of TAUDn channel 9 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_10 = (0x0400U), ///< Trigger source of TAUDn channel 10 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_11 = (0x0800U), ///< Trigger source of TAUDn channel 11 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_12 = (0x1000U), ///< Trigger source of TAUDn channel 12 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_13 = (0x2000U), ///< Trigger source of TAUDn channel 13 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_14 = (0x4000U), ///< Trigger source of TAUDn channel 14 selected as the ADC trigger.
    PIC_ADC_TRIGGER_SEL_TAUD_CHANNEL_15 = (0x8000U), ///< Trigger source of TAUDn channel 15 selected as the ADC trigger.
} pic_adc_trigger_sel_taud_t;

/** Selects triggers from TAUD0, TAUD1, TAUJ0, external pin ADCiTRGj for ADCn scan group j.*/
typedef enum e_pic20_adc_trigger_sel
{
    PIC20_ADC_TRIGGER_SEL_ENCA         = (0x10000000UL), ///< Output value of ENCAn interrupt signal selected by PIC20ADTEN42j.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I12  = (0x08000000UL), ///< Selects the INTTSG31I12 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I11  = (0x04000000UL), ///< Selects the INTTSG31I11 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I8   = (0x02000000UL), ///< Selects the INTTSG31I8 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I7   = (0x01000000UL), ///< Selects the INTTSG31I7 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I4   = (0x00800000UL), ///< Selects the INTTSG31I4 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG31I3   = (0x00400000UL), ///< Selects the INTTSG31I3 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TAPA0TADOUT1 = (0x00200000UL), ///< Selects the TAPATADOUT1 signal of TAPA0 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TAPA0TADOUT0 = (0x00100000UL), ///< Selects the TAPATADOUT0 signal of TAPA0 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I12  = (0x00080000UL), ///< Selects the INTTSG30I12 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I11  = (0x00040000UL), ///< Selects the INTTSG30I11 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I8   = (0x00020000UL), ///< Selects the INTTSG30I8 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I7   = (0x00010000UL), ///< Selects the INTTSG30I7 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I4   = (0x00008000UL), ///< Selects the INTTSG30I4 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTSG30I3   = (0x00004000UL), ///< Selects the INTTSG30I3 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_ADCTRG4      = (0x00002000UL), ///< Selects the ADCiTRG4 pin as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_ADCTRG3      = (0x00001000UL), ///< Selects the ADCiTRG3 pin as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_ADCTRG2      = (0x00000800UL), ///< Selects the ADCiTRG2 pin as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_ADCTRG1      = (0x00000400UL), ///< Selects the ADCiTRG1 pin as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_ADCTRG0      = (0x00000200UL), ///< Selects the ADCiTRG0 pin as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TSG31TRG1    = (0x00000100UL), ///< Selects the TSG31ADTRG1 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TSG31TRG0    = (0x00000080UL), ///< Selects the TSG31ADTRG0 signal of TSG31 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TSG30TRG1    = (0x00000040UL), ///< Selects the TSG30ADTRG1 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_TSG30TRG0    = (0x00000020UL), ///< Selects the TSG30ADTRG0 signal of TSG30 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTAUJ1I3   = (0x00000010UL), ///< Selects the INTTAUJ1I3 signal of TAUJ1 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_INTTAUJ0I3   = (0x00000008UL), ///< Selects the INTTAUJ0I3 signal of TAUJ0 as the trigger source for ADCi SGj.
    PIC20_ADC_TRIGGER_SEL_GTM_ATU      = (0x00000004UL), ///< Output value of GTM timer output selected by PIC20ADTEN5nj registers, the ATU trigger selected by the PIC20ADTEN7nj, PIC20ADTEN8nj registers.
    PIC20_ADC_TRIGGER_SEL_TAUD1        = (0x00000002UL), ///< Selects the TAUD1 interrupt signal selected.
    PIC20_ADC_TRIGGER_SEL_TAUD0        = (0x00000001UL), ///< Selects the TAUD0 interrupt signal selected.
} pic20_adc_trigger_sel_t;

/** Selects triggers from GTM (by set output in A/D converter trigger output configuration register) for ADCn scan group j.*/
typedef enum e_pic_adc_trigger_sel_gtm
{
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG9_2416 = (0x00080000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG9 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG9_0800 = (0x00040000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG9 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG8_2416 = (0x00020000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG8 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG8_0800 = (0x00010000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG8 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG7_2416 = (0x00008000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG7 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG7_0800 = (0x00004000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG7 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG6_2416 = (0x00002000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG6 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG6_0800 = (0x00001000UL), ///< Output value of GTM timer output selected by PIC2ADTCFG6 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG5_2416 = (0x00000800UL), ///< Output value of GTM timer output selected by PIC2ADTCFG5 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG5_0800 = (0x00000400UL), ///< Output value of GTM timer output selected by PIC2ADTCFG5 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG4_2416 = (0x00000200UL), ///< Output value of GTM timer output selected by PIC2ADTCFG4 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG4_0800 = (0x00000100UL), ///< Output value of GTM timer output selected by PIC2ADTCFG4 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG3_2416 = (0x00000080UL), ///< Output value of GTM timer output selected by PIC2ADTCFG3 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG3_0800 = (0x00000040UL), ///< Output value of GTM timer output selected by PIC2ADTCFG3 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG2_2416 = (0x00000020UL), ///< Output value of GTM timer output selected by PIC2ADTCFG2 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG2_0800 = (0x00000010UL), ///< Output value of GTM timer output selected by PIC2ADTCFG2 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG1_2416 = (0x00000008UL), ///< Output value of GTM timer output selected by PIC2ADTCFG1 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG1_0800 = (0x00000004UL), ///< Output value of GTM timer output selected by PIC2ADTCFG1 bits 8 to 0.

    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG0_2416 = (0x00000002UL), ///< Output value of GTM timer output selected by PIC2ADTCFG0 bits 24 to 16.
    PIC_ADC_TRIGGER_SEL_GTM_PIC2ADTCFG0_0800 = (0x00000001UL), ///< Output value of GTM timer output selected by PIC2ADTCFG0 bits 8 to 0.
} pic_adc_trigger_sel_gtm_t;

/** Select trigger source from GTM TIM shared interrupt (INTGTMA0TIM) for ADCJn scan group j. */
typedef enum e_pic_adc_trigger_sel_gtm_intgtma0tim
{
    /** TIM3 shared interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_37 = (int32_t)(0x80000000UL), ///< INTGTMA0TIM37
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_36 = (int32_t)(0x40000000UL), ///< INTGTMA0TIM36
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_35 = (int32_t)(0x20000000UL), ///< INTGTMA0TIM35
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_34 = (int32_t)(0x10000000UL), ///< INTGTMA0TIM34
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_33 = (int32_t)(0x08000000UL), ///< INTGTMA0TIM33
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_32 = (int32_t)(0x04000000UL), ///< INTGTMA0TIM32
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_31 = (int32_t)(0x02000000UL), ///< INTGTMA0TIM31
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_30 = (int32_t)(0x01000000UL), ///< INTGTMA0TIM30

    /** TIM2 shared interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_27 = (int32_t)(0x00800000UL), ///< INTGTMA0TIM27
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_26 = (int32_t)(0x00400000UL), ///< INTGTMA0TIM26
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_25 = (int32_t)(0x00200000UL), ///< INTGTMA0TIM25
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_24 = (int32_t)(0x00100000UL), ///< INTGTMA0TIM24
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_23 = (int32_t)(0x00080000UL), ///< INTGTMA0TIM23
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_22 = (int32_t)(0x00040000UL), ///< INTGTMA0TIM22
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_21 = (int32_t)(0x00020000UL), ///< INTGTMA0TIM21
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_20 = (int32_t)(0x00010000UL), ///< INTGTMA0TIM20

    /** TIM1 shared interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_17 = (int32_t)(0x00008000UL), ///< INTGTMA0TIM17
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_16 = (int32_t)(0x00004000UL), ///< INTGTMA0TIM16
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_15 = (int32_t)(0x00002000UL), ///< INTGTMA0TIM15
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_14 = (int32_t)(0x00001000UL), ///< INTGTMA0TIM14
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_13 = (int32_t)(0x00000800UL), ///< INTGTMA0TIM13
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_12 = (int32_t)(0x00000400UL), ///< INTGTMA0TIM12
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_11 = (int32_t)(0x00000200UL), ///< INTGTMA0TIM11
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_10 = (int32_t)(0x00000100UL), ///< INTGTMA0TIM10

    /** TIM0 shared interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_07 = (int32_t)(0x00000080UL), ///< INTGTMA0TIM07
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_06 = (int32_t)(0x00000040UL), ///< INTGTMA0TIM06
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_05 = (int32_t)(0x00000020UL), ///< INTGTMA0TIM05
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_04 = (int32_t)(0x00000010UL), ///< INTGTMA0TIM04
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_03 = (int32_t)(0x00000008UL), ///< INTGTMA0TIM03
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_02 = (int32_t)(0x00000004UL), ///< INTGTMA0TIM02
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_01 = (int32_t)(0x00000002UL), ///< INTGTMA0TIM01
    PIC_ADC_TRIGGER_SEL_GTM_INTGTMA0TIM_00 = (int32_t)(0x00000001UL), ///< INTGTMA0TIM00
} pic_adc_trigger_sel_gtm_intgtma0tim_t;

/** Select trigger source from  GTM timer output (GTM_ATOM0/GTM_ATOM2 and GTM_ATM1/GTM_ATOM3) for ADCJn scan group j. */
typedef enum e_pic_adc_trigger_sel_gtm_atom
{
    /** ATOM1/ATOM3 */
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT7_N = (int32_t)(0x80000000UL),  ///< GTM_ATOM1_OUT7_N_N/GTM_ATOM3_OUT7_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT7   = (int32_t)(0x40000000UL),  ///< GTM_ATOM1_OUT7/GTM_ATOM3_OUT7.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT6_N = (int32_t)(0x20000000UL),  ///< GTM_ATOM1_OUT6_N/GTM_ATOM3_OUT6_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT6   = (int32_t)(0x10000000UL),  ///< GTM_ATOM1_OUT6/GTM_ATOM3_OUT6.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT5_N = (int32_t)(0x08000000UL),  ///< GTM_ATOM1_OUT5_N/GTM_ATOM3_OUT5_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT5   = (int32_t)(0x04000000UL),  ///< GTM_ATOM1_OUT5/GTM_ATOM3_OUT5.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT4_N = (int32_t)(0x02000000UL),  ///< GTM_ATOM1_OUT4_N/GTM_ATOM3_OUT4_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT4   = (int32_t)(0x01000000UL),  ///< GTM_ATOM1_OUT4/GTM_ATOM3_OUT4.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT3_N = (int32_t)(0x00800000UL),  ///< GTM_ATOM1_OUT3_N/GTM_ATOM3_OUT3_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT3   = (int32_t)(0x00400000UL),  ///< GTM_ATOM1_OUT3/GTM_ATOM3_OUT3.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT2_N = (int32_t)(0x00200000UL),  ///< GTM_ATOM1_OUT2_N/GTM_ATOM3_OUT2_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT2   = (int32_t)(0x00100000UL),  ///< GTM_ATOM1_OUT2/GTM_ATOM3_OUT2.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT1_N = (int32_t)(0x00080000UL),  ///< GTM_ATOM1_OUT1_N/GTM_ATOM3_OUT1_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT1   = (int32_t)(0x00040000UL),  ///< GTM_ATOM1_OUT1/GTM_ATOM3_OUT1.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT0_N = (int32_t)(0x00020000UL),  ///< GTM_ATOM1_OUT0_N/GTM_ATOM3_OUT0_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_ODD_OUT0   = (int32_t)(0x00010000UL),  ///< GTM_ATOM1_OUT0/GTM_ATOM3_OUT0.

    /** ATOM0/ATOM2 */
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT7_N = (0x00008000UL), ///< GTM_ATOM0_OUT7_N/GTM_ATOM2_OUT7_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT7   = (0x00004000UL), ///< GTM_ATOM0_OUT7/GTM_ATOM2_OUT7.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT6_N = (0x00002000UL), ///< GTM_ATOM0_OUT6_N/GTM_ATOM2_OUT6_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT6   = (0x00001000UL), ///< GTM_ATOM0_OUT6/GTM_ATOM2_OUT6.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT5_N = (0x00000800UL), ///< GTM_ATOM0_OUT5_N/GTM_ATOM2_OUT5_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT5   = (0x00000400UL), ///< GTM_ATOM0_OUT5/GTM_ATOM2_OUT5.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT4_N = (0x00000200UL), ///< GTM_ATOM0_OUT4_N/GTM_ATOM2_OUT4_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT4   = (0x00000100UL), ///< GTM_ATOM0_OUT4/GTM_ATOM2_OUT4.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT3_N = (0x00000080UL), ///< GTM_ATOM0_OUT3_N/GTM_ATOM2_OUT3_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT3   = (0x00000040UL), ///< GTM_ATOM0_OUT3/GTM_ATOM2_OUT3.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT2_N = (0x00000020UL), ///< GTM_ATOM0_OUT2_N/GTM_ATOM2_OUT2_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT2   = (0x00000010UL), ///< GTM_ATOM0_OUT2/GTM_ATOM2_OUT2.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT1_N = (0x00000008UL), ///< GTM_ATOM0_OUT1_N/GTM_ATOM2_OUT1_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT1   = (0x00000004UL), ///< GTM_ATOM0_OUT1/GTM_ATOM2_OUT1.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT0_N = (0x00000002UL), ///< GTM_ATOM0_OUT0_N/GTM_ATOM2_OUT0_N.
    PIC_ADC_TRIGGER_SEL_GTM_ATOM_EVEN_OUT0   = (0x00000001UL)  ///< GTM_ATOM0_OUT0/GTM_ATOM2_OUT0.
} pic_adc_trigger_sel_gtm_atom_t;

/** Select trigger source from GTM timer output (MCS interrupt) for ADCJn scan group j. */
typedef enum e_pic_adc_trigger_sel_gtm_mcs
{
    /** MCS3 interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ7 = (int32_t)(0x80000000UL), ///< GTM_MCS3_IRQ7.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ6 = (int32_t)(0x40000000UL), ///< GTM_MCS3_IRQ6.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ5 = (int32_t)(0x20000000UL), ///< GTM_MCS3_IRQ5.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ4 = (int32_t)(0x10000000UL), ///< GTM_MCS3_IRQ4.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ3 = (int32_t)(0x08000000UL), ///< GTM_MCS3_IRQ3.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ2 = (int32_t)(0x04000000UL), ///< GTM_MCS3_IRQ2.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ1 = (int32_t)(0x02000000UL), ///< GTM_MCS3_IRQ1.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_3_IRQ0 = (int32_t)(0x01000000UL), ///< GTM_MCS3_IRQ0.

    /** MCS2 interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ7 = (int32_t)(0x00800000UL), ///< GTM_MCS2_IRQ7.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ6 = (int32_t)(0x00400000UL), ///< GTM_MCS2_IRQ6.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ5 = (int32_t)(0x00200000UL), ///< GTM_MCS2_IRQ5.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ4 = (int32_t)(0x00100000UL), ///< GTM_MCS2_IRQ4.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ3 = (int32_t)(0x00080000UL), ///< GTM_MCS2_IRQ3.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ2 = (int32_t)(0x00040000UL), ///< GTM_MCS2_IRQ2.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ1 = (int32_t)(0x00020000UL), ///< GTM_MCS2_IRQ1.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_2_IRQ0 = (int32_t)(0x00010000UL), ///< GTM_MCS2_IRQ0.

    /** MCS1 interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ7 = (int32_t)(0x00008000UL), ///< GTM_MCS1_IRQ7.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ6 = (int32_t)(0x00004000UL), ///< GTM_MCS1_IRQ6.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ5 = (int32_t)(0x00002000UL), ///< GTM_MCS1_IRQ5.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ4 = (int32_t)(0x00001000UL), ///< GTM_MCS1_IRQ4.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ3 = (int32_t)(0x00000800UL), ///< GTM_MCS1_IRQ3.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ2 = (int32_t)(0x00000400UL), ///< GTM_MCS1_IRQ2.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ1 = (int32_t)(0x00000200UL), ///< GTM_MCS1_IRQ1.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_1_IRQ0 = (int32_t)(0x00000100UL), ///< GTM_MCS1_IRQ0.

    /** MCS0 interrupts */
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ7 = (int32_t)(0x00000080UL), ///< GTM_MCS0_IRQ7.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ6 = (int32_t)(0x00000040UL), ///< GTM_MCS0_IRQ6.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ5 = (int32_t)(0x00000020UL), ///< GTM_MCS0_IRQ5.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ4 = (int32_t)(0x00000010UL), ///< GTM_MCS0_IRQ4.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ3 = (int32_t)(0x00000008UL), ///< GTM_MCS0_IRQ3.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ2 = (int32_t)(0x00000004UL), ///< GTM_MCS0_IRQ2.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ1 = (int32_t)(0x00000002UL), ///< GTM_MCS0_IRQ1.
    PIC_ADC_TRIGGER_SEL_GTM_MCS_0_IRQ0 = (int32_t)(0x00000001UL)  ///< GTM_MCS0_IRQ0.
} pic_adc_trigger_sel_gtm_mcs_t;

/** Selects triggers from ENCAn interrupt for ADCn scan group j.*/
typedef enum e_pic_adc_trigger_sel_enca
{
    PIC_ADC_TRIGGER_SEL_ENCA_1_CLEAR             = (0x00000020UL), ///< ENCA1 Clear interrupt signal by encoder input (phase Z).
    PIC_ADC_TRIGGER_SEL_ENCA_1_COMPARE1_CAPTURE1 = (0x00000010UL), ///< ENCA1 Compare 1 match or Capture 1 interrupt signal.
    PIC_ADC_TRIGGER_SEL_ENCA_1_COMPARE0_CAPTURE0 = (0x00000008UL), ///< ENCA1 Compare 0 match or Capture 0 interrupt signal.
    PIC_ADC_TRIGGER_SEL_ENCA_0_CLEAR             = (0x00000004UL), ///< ENCA0 Clear interrupt signal by encoder input (phase Z).
    PIC_ADC_TRIGGER_SEL_ENCA_0_COMPARE1_CAPTURE1 = (0x00000002UL), ///< ENCA0 Compare 1 match or Capture 1 interrupt signal.
    PIC_ADC_TRIGGER_SEL_ENCA_0_COMPARE0_CAPTURE0 = (0x00000001UL), ///< ENCA0 Compare 0 match or Capture 0 interrupt signal.
} pic_adc_trigger_sel_enca_t;

/** Select triggers from ATU timer D 1shot pulse ON/OFF occurrence for ADCn scan group j.*/
typedef enum e_pic_adc_trigger_sel_atu_d_1shot_pulse
{
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D93 = (0x00080000UL), ///< 1shot pulse ON/OFF D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D92 = (0x00040000UL), ///< 1shot pulse ON/OFF D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D91 = (0x00020000UL), ///< 1shot pulse ON/OFF D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D90 = (0x00010000UL), ///< 1shot pulse ON/OFF D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D63 = (0x00008000UL), ///< 1shot pulse ON/OFF D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D62 = (0x00004000UL), ///< 1shot pulse ON/OFF D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D61 = (0x00002000UL), ///< 1shot pulse ON/OFF D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D60 = (0x00001000UL), ///< 1shot pulse ON/OFF D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D43 = (0x00000800UL), ///< 1shot pulse ON/OFF D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D42 = (0x00000400UL), ///< 1shot pulse ON/OFF D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D41 = (0x00000200UL), ///< 1shot pulse ON/OFF D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D40 = (0x00000100UL), ///< 1shot pulse ON/OFF D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D23 = (0x00000080UL), ///< 1shot pulse ON/OFF D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D22 = (0x00000040UL), ///< 1shot pulse ON/OFF D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D21 = (0x00000020UL), ///< 1shot pulse ON/OFF D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D20 = (0x00000010UL), ///< 1shot pulse ON/OFF D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D03 = (0x00000008UL), ///< 1shot pulse ON/OFF D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D02 = (0x00000004UL), ///< 1shot pulse ON/OFF D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D01 = (0x00000002UL), ///< 1shot pulse ON/OFF D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON/OFF occurence interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_1SHOT_PULSE_D00 = (0x00000001UL), ///< 1shot pulse ON/OFF D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON/OFF occurence interrupt).
} pic_adc_trigger_sel_atu_d_1shot_pulse_t;

/** Select triggers from ATU timer D compare match A and ATU timer G compare match for ADCn scan group j.*/
typedef enum e_pic_adc_trigger_sel_atu_d_g_compare_match
{
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG13   = (0x20000000UL), ///< OCRG13 compare match interrupt (ATU6 timer G subblock 13).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG12   = (0x10000000UL), ///< OCRG12 compare match interrupt (ATU6 timer G subblock 12).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG11   = (0x08000000UL), ///< OCRG11 compare match interrupt (ATU6 timer G subblock 11).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG10   = (0x04000000UL), ///< OCRG10 compare match interrupt (ATU6 timer G subblock 10).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG9    = (0x02000000UL), ///< OCRG9 compare match interrupt (ATU6 timer G subblock 9).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCRG8    = (0x01000000UL), ///< OCRG8 compare match interrupt (ATU6 timer G subblock 8).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D113 = (0x00800000UL), ///< OCR1D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D112 = (0x00400000UL), ///< OCR1D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D111 = (0x00200000UL), ///< OCR1D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D110 = (0x00100000UL), ///< OCR1D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D93  = (0x00080000UL), ///< OCR1D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D92  = (0x00040000UL), ///< OCR1D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D91  = (0x00020000UL), ///< OCR1D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D90  = (0x00010000UL), ///< OCR1D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D63  = (0x00008000UL), ///< OCR1D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D62  = (0x00004000UL), ///< OCR1D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D61  = (0x00002000UL), ///< OCR1D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D60  = (0x00001000UL), ///< OCR1D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D43  = (0x00000800UL), ///< OCR1D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D42  = (0x00000400UL), ///< OCR1D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D41  = (0x00000200UL), ///< OCR1D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D40  = (0x00000100UL), ///< OCR1D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D23  = (0x00000080UL), ///< OCR1D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D22  = (0x00000040UL), ///< OCR1D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D21  = (0x00000020UL), ///< OCR1D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D20  = (0x00000010UL), ///< OCR1D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D03  = (0x00000008UL), ///< OCR1D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D02  = (0x00000004UL), ///< OCR1D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D01  = (0x00000002UL), ///< OCR1D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match A interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_OCR1D00  = (0x00000001UL), ///< OCR1D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match A interrupt).
} pic_adc_trigger_sel_atu_d_g_compare_match_t;

/** Select triggers from ATU timer D compare match B for ADCn scan group j.*/
typedef enum e_pic_adc_trigger_sel_atu_d_compare_match_b
{
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D113 = (0x00800000UL), ///< OCR2D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D112 = (0x00400000UL), ///< OCR2D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D111 = (0x00200000UL), ///< OCR2D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D110 = (0x00100000UL), ///< OCR2D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D93  = (0x00080000UL), ///< OCR2D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D92  = (0x00040000UL), ///< OCR2D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D91  = (0x00020000UL), ///< OCR2D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D90  = (0x00010000UL), ///< OCR2D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D63  = (0x00008000UL), ///< OCR2D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D62  = (0x00004000UL), ///< OCR2D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D61  = (0x00002000UL), ///< OCR2D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D60  = (0x00001000UL), ///< OCR2D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D43  = (0x00000800UL), ///< OCR2D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D42  = (0x00000400UL), ///< OCR2D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D41  = (0x00000200UL), ///< OCR2D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D40  = (0x00000100UL), ///< OCR2D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D23  = (0x00000080UL), ///< OCR2D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D22  = (0x00000040UL), ///< OCR2D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D21  = (0x00000020UL), ///< OCR2D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D20  = (0x00000010UL), ///< OCR2D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D03  = (0x00000008UL), ///< OCR2D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D02  = (0x00000004UL), ///< OCR2D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D01  = (0x00000002UL), ///< OCR2D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match B interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D00  = (0x00000001UL), ///< OCR2D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match B interrupt).
} pic_adc_trigger_sel_atu_d_compare_match_b_t;

/** Select triggers from ATU timer D down-counter underflow for ADCKn scan group j.*/
typedef enum e_pic_adc_trigger_sel_atu_d_down_counter_underflow
{
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID93 = (0x00080000UL), ///< UDID93 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 3 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID92 = (0x00040000UL), ///< UDID92 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 2 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID91 = (0x00020000UL), ///< UDID91 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 1 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID90 = (0x00010000UL), ///< UDID90 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 0 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID63 = (0x00008000UL), ///< UDID63 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 3 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID62 = (0x00004000UL), ///< UDID62 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 2 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID61 = (0x00002000UL), ///< UDID61 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 1 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID60 = (0x00001000UL), ///< UDID60 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 0 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID43 = (0x00000800UL), ///< UDID43 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 3 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID42 = (0x00000400UL), ///< UDID42 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 2 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID41 = (0x00000200UL), ///< UDID41 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 1 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID40 = (0x00000100UL), ///< UDID40 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 0 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID23 = (0x00000080UL), ///< UDID23 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 3 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID22 = (0x00000040UL), ///< UDID22 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 2 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID21 = (0x00000020UL), ///< UDID21 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 1 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID20 = (0x00000010UL), ///< UDID20 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 0 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID03 = (0x00000008UL), ///< UDID03 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 3 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID02 = (0x00000004UL), ///< UDID02 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 2 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID01 = (0x00000002UL), ///< UDID01 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 1 down-counter underflow interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID00 = (0x00000001UL), ///< UDID00 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 0 down-counter underflow interrupt).
} pic_adc_trigger_sel_atu_d_down_counter_underflow_t;

typedef enum e_pic_adc_trigger_sel_atu_c_g_dma
{
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC73 = (int32_t)(0x80000000UL), ///< GRC73 input capture / compare match interrupt (ATU6 timer C subblock 7 channel 3 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC72 = (int32_t)(0x40000000UL), ///< GRC72 input capture / compare match interrupt (ATU6 timer C subblock 7 channel 2 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC71 = (int32_t)(0x20000000UL), ///< GRC71 input capture / compare match interrupt (ATU6 timer C subblock 7 channel 1 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC70 = (int32_t)(0x10000000UL), ///< GRC70 input capture / compare match interrupt (ATU6 timer C subblock 7 channel 0 input capture/compare match interrupt).

    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC63 = (int32_t)(0x08000000UL), ///< GRC63 input capture / compare match interrupt (ATU6 timer C subblock 6 channel 3 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC62 = (int32_t)(0x04000000UL), ///< GRC62 input capture / compare match interrupt (ATU6 timer C subblock 6 channel 2 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC61 = (int32_t)(0x02000000UL), ///< GRC61 input capture / compare match interrupt (ATU6 timer C subblock 6 channel 1 input capture / compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC60 = (int32_t)(0x01000000UL), ///< GRC60 input capture / compare match interrupt (ATU6 timer C subblock 6 channel 0 input capture/compare match interrupt).

    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC43 = (int32_t)(0x00800000UL), ///< GRC43 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 3 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC42 = (int32_t)(0x00400000UL), ///< GRC42 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 2 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC41 = (int32_t)(0x00200000UL), ///< GRC41 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 1 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC40 = (int32_t)(0x00100000UL), ///< GRC40 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 0 input capture/compare match interrupt).

    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC03 = (int32_t)(0x00080000UL), ///< GRC43 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 3 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC02 = (int32_t)(0x00040000UL), ///< GRC42 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 2 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC01 = (int32_t)(0x00020000UL), ///< GRC41 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 1 input capture/compare match interrupt).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_GRC00 = (int32_t)(0x00010000UL), ///< GRC40 input capture / compare match interrupt (ATU6 timer C subblock 4 channel 0 input capture/compare match interrupt).

    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG7 = (int32_t)(0x00008000UL), ///< OCRG7 compare match interrupt (ATU6 timer G subblock 7).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG6 = (int32_t)(0x00004000UL), ///< OCRG6 compare match interrupt (ATU6 timer G subblock 6).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG5 = (int32_t)(0x00002000UL), ///< OCRG5 compare match interrupt (ATU6 timer G subblock 5).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG4 = (int32_t)(0x00001000UL), ///< OCRG4 compare match interrupt (ATU6 timer G subblock 4).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG3 = (int32_t)(0x00000800UL), ///< OCRG3 compare match interrupt (ATU6 timer G subblock 3).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG2 = (int32_t)(0x00000400UL), ///< OCRG2 compare match interrupt (ATU6 timer G subblock 2).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG1 = (int32_t)(0x00000200UL), ///< OCRG1 compare match interrupt (ATU6 timer G subblock 1).
    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_OCRG0 = (int32_t)(0x00000100UL), ///< OCRG0 compare match interrupt (ATU6 timer G subblock 0).

    PIC_ADC_TRIGGER_SEL_ATU_C_G_DMA_REQ = (int32_t)(0x00000001UL),   ///< Output signal from the ADCK trigger selection circuit for ATU DMA / A/D request auto-switching.
} pic_adc_trigger_sel_atu_c_g_dma_t;

/** Selects triggers from TAUD0, TAUD1, TAUJ0, external pin ADCKiTRGj for ADCKn scan group j.*/
typedef enum e_pic21_adc_trigger_sel
{
    PIC21_ADC_TRIGGER_SEL_RDC3AS1_ET    = (int32_t)(0x80000000UL), ///< Selects the RDC3AS1 excitation timer (ET) A/D conversion start trigger signal output as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AS1_CMP0  = (int32_t)(0x40000000UL), ///< Selects the RDC3AS1 compare 0 match interrupt request signal as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AS0_ET    = (int32_t)(0x20000000UL), ///< Selects the RDC3AS0 excitation timer (ET) A/D conversion start trigger signal output as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AS0_CMP0  = (int32_t)(0x10000000UL), ///< Selects the RDC3AS0 compare 0 match interrupt request signal as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AL1_ET    = (int32_t)(0x08000000UL), ///< Selects the RDC3AL1 excitation timer (ET) A/D conversion start trigger signal output as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AL1_CMP0  = (int32_t)(0x04000000UL), ///< Selects the RDC3AL1 compare 0 match interrupt request signal as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AL0_ET    = (int32_t)(0x02000000UL), ///< Selects the RDC3AL0 excitation timer (ET) A/D conversion start trigger signal output as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_RDC3AL0_CMP0  = (int32_t)(0x01000000UL), ///< Selects the RDC3AL0 compare 0 match interrupt request signal as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_EMU3S1_ADTRG1 = (int32_t)(0x00800000UL), ///< Selects the ADC scan group conversion start signal of EMU3S channel 1, EMU3S1 ADTRG1, as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_EMU3S1_ADTRG0 = (int32_t)(0x00400000UL), ///< Selects the ADC scan group conversion start signal of EMU3S channel 1, EMU3S1 ADTRG0, as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_EMU3S0_ADTRG1 = (int32_t)(0x00200000UL), ///< Selects the ADC scan group conversion start signal of EMU3S channel 0, EMU3S0 ADTRG1, as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_EMU3S0_ADTRG0 = (int32_t)(0x00100000UL), ///< Selects the ADC scan group conversion start signal of EMU3S channel 0, EMU3S0 ADTRG0, as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I12   = (int32_t)(0x00080000UL), ///< Selects the INTTSG32I12 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I11   = (int32_t)(0x00040000UL), ///< Selects the INTTSG32I11 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I8    = (int32_t)(0x00020000UL), ///< Selects the INTTSG32I8 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I7    = (int32_t)(0x00010000UL), ///< Selects the INTTSG32I7 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I4    = (int32_t)(0x00008000UL), ///< Selects the INTTSG32I4 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_INTTSG32I3    = (int32_t)(0x00004000UL), ///< Selects the INTTSG32I3 signal of TSG32 as the trigger source for ADCKn SGj.

    PIC21_ADC_TRIGGER_SEL_TAPA2TADOUT1 = (0x00001000UL),  ///< Selects the TAPA2TADOUT1 signal of TAPA2 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAPA2TADOUT0 = (0x00000800UL),  ///< Selects the TAPA2TADOUT0 signal of TAPA2 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAPA1TADOUT1 = (0x00000400UL),  ///< Selects the TAPA1TADOUT1 signal of TAPA1 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAPA1TADOUT0 = (0x00000200UL),  ///< Selects the TAPA1TADOUT0 signal of TAPA1 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAPA0TADOUT1 = (0x00000100UL),  ///< Selects the TAPA0TADOUT1 signal of TAPA0 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAPA0TADOUT0 = (0x00000080UL),  ///< Selects the TAPA0TADOUT0 signal of TAPA0 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TSG32ADTRG1  = (0x00000040UL),  ///< Selects the TSG32ADTRG1 signal of TSG32 as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TSG32ADTRG0  = (0x00000020UL),  ///< Selects the TSG32ADTRG0 signal of TSG32 as the trigger source for ADCKn SGj.

    PIC21_ADC_TRIGGER_SEL_ATU   = (0x00000004UL),         ///< Selects the ATU trigger selected by the PIC21ADTEN5nj, PIC21ADTEN6nj, PIC21ADTEN7nj, PIC21ADTEN8nj registers as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAUD3 = (0x00000002UL),         ///< Selects the TAUD3 interrupt signal selected by the PIC21ADTEN4nj register as the trigger source for ADCKn SGj.
    PIC21_ADC_TRIGGER_SEL_TAUD2 = (0x00000001UL),         ///< Selects the TAUD2 interrupt signal selected by the PIC21ADTEN4nj register as the trigger source for ADCKn SGj.
} pic21_adc_trigger_sel_t;

/** Start trigger output control register 0n0 enables a trigger source from GTM to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_start_trigger_bit
{
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG13_BIT_24_TO_16  = (0x08000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG13 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG13_BIT_08_TO_00  = (0x04000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG13 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG12_BIT_24_TO_16  = (0x02000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG12 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG12_BIT_08_TO_00  = (0x01000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG12 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG11_BIT_24_TO_16  = (0x00800000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG11 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG11_BIT_08_TO_00  = (0x00400000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG11 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG10_BIT_24_TO_16  = (0x00200000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG10 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG10_BIT_08_TO_00  = (0x00100000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG10 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG9_BIT_24_TO_16   = (0x00080000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG9 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG9_BIT_08_TO_00   = (0x00040000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG9 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG8_BIT_24_TO_16   = (0x00020000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG8 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG8_BIT_08_TO_00   = (0x00010000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG8 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG7_BIT_24_TO_16   = (0x00008000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG7 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG7_BIT_08_TO_00   = (0x00004000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG7 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG6_BIT_24_TO_16   = (0x00002000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG6 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG6_BIT_08_TO_00   = (0x00001000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG6 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG5_BIT_24_TO_16   = (0x00000800UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG5 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG5_BIT_08_TO_00   = (0x00000400UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG5 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG4_BIT_24_TO_16   = (0x00000200UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG4 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG4_BIT_08_TO_00   = (0x00000100UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG4 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG3_BIT_24_TO_16   = (0x00000080UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG3 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG3_BIT_08_TO_00   = (0x00000040UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG3 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG2_BIT_24_TO_16   = (0x00000020UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG2 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG2_BIT_08_TO_00   = (0x00000010UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG2 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG1_BIT_24_TO_16   = (0x00000008UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG1 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG1_BIT_08_TO_00   = (0x00000004UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG1 bits 8 to 0
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG0_BIT_24_TO_16   = (0x00000002UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG0 bits 24 to 16
    PIC_DSADC_START_TRIGGER_PIC2DSADTCFG0_BIT_08_TO_00   = (0x00000001UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG0 bits 8 to 0
} pic_dsadc_start_trigger_bit_t;

/** Start trigger output control register 0n1 enables a trigger source from ENCA to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_start_trigger_compare_or_capture
{
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATIEC  = (0x00000020UL), ///< Selects the INTENCA1IEC as a trigger source of DSADCn.
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATINT1 = (0x00000010UL), ///< Selects the INTENCA1I1 as a trigger source of DSADCn.
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATINT0 = (0x00000008UL), ///< Selects the INTENCA1I0 as a trigger source of DSADCn.
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATIEC  = (0x00000004UL), ///< Selects the INTENCA0IEC as a trigger source of DSADCn.
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATINT1 = (0x00000002UL), ///< Selects the INTENCA0I1 as a trigger source of DSADCn.
    PIC_DSADC_START_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATIEC0 = (0x00000001UL), ///< Selects the INTENCA0I0 as a trigger source of DSADCn.
} pic_dsadc_start_trigger_compare_or_capture_t;

/** Start trigger output control register 0n2 enables a trigger source from ATU timer D 1shot pulse ON occurrence to be selected as a trigger for starting DSADC. */
typedef enum e_pic_dsadc_start_trigger_sel_atu_d_1shot_pulse_on
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D93 = (0x00080000UL), ///< 1shot pulse ON D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D92 = (0x00040000UL), ///< 1shot pulse ON D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D91 = (0x00020000UL), ///< 1shot pulse ON D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D90 = (0x00010000UL), ///< 1shot pulse ON D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D63 = (0x00008000UL), ///< 1shot pulse ON D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D62 = (0x00004000UL), ///< 1shot pulse ON D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D61 = (0x00002000UL), ///< 1shot pulse ON D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D60 = (0x00001000UL), ///< 1shot pulse ON D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D43 = (0x00000800UL), ///< 1shot pulse ON D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D42 = (0x00000400UL), ///< 1shot pulse ON D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D41 = (0x00000200UL), ///< 1shot pulse ON D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D40 = (0x00000100UL), ///< 1shot pulse ON D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D23 = (0x00000080UL), ///< 1shot pulse ON D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D22 = (0x00000040UL), ///< 1shot pulse ON D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D21 = (0x00000020UL), ///< 1shot pulse ON D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D20 = (0x00000010UL), ///< 1shot pulse ON D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D03 = (0x00000008UL), ///< 1shot pulse ON D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D02 = (0x00000004UL), ///< 1shot pulse ON D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D01 = (0x00000002UL), ///< 1shot pulse ON D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D00 = (0x00000001UL), ///< 1shot pulse ON D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_dsadc_start_trigger_sel_atu_d_1shot_pulse_on_t;

/** Start trigger output control register 0n3 enables a trigger source from ATU timer D 1shot pulse OFF occurrence to be selected as a trigger for starting DSADC .*/
typedef enum e_pic_dsadc_start_trigger_sel_atu_d_1shot_pulse_off
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D93 = (0x00080000UL), ///< 1shot pulse OFF D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D92 = (0x00040000UL), ///< 1shot pulse OFF D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D91 = (0x00020000UL), ///< 1shot pulse OFF D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D90 = (0x00010000UL), ///< 1shot pulse OFF D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D63 = (0x00008000UL), ///< 1shot pulse OFF D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D62 = (0x00004000UL), ///< 1shot pulse OFF D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D61 = (0x00002000UL), ///< 1shot pulse OFF D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D60 = (0x00001000UL), ///< 1shot pulse OFF D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D43 = (0x00000800UL), ///< 1shot pulse OFF D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D42 = (0x00000400UL), ///< 1shot pulse OFF D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D41 = (0x00000200UL), ///< 1shot pulse OFF D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D40 = (0x00000100UL), ///< 1shot pulse OFF D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D23 = (0x00000080UL), ///< 1shot pulse OFF D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D22 = (0x00000040UL), ///< 1shot pulse OFF D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D21 = (0x00000020UL), ///< 1shot pulse OFF D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D20 = (0x00000010UL), ///< 1shot pulse OFF D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D03 = (0x00000008UL), ///< 1shot pulse OFF D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D02 = (0x00000004UL), ///< 1shot pulse OFF D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D01 = (0x00000002UL), ///< 1shot pulse OFF D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D00 = (0x00000001UL), ///< 1shot pulse OFF D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_dsadc_start_trigger_sel_atu_d_1shot_pulse_off_t;

/** Select triggers from ATU timer C compare match and ATU timer G compare match to be selected as a trigger for starting DSADC. */
typedef enum e_pic_dsadc_start_trigger_sel_atu_c_g_compare_match
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC133   = (int32_t)(0x80000000UL), ///< OCRC133 capture / compare match interrupt (ATU6 timer C subblock 13 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC132   = (int32_t)(0x40000000UL), ///< OCRC132 capture / compare match interrupt (ATU6 timer C subblock 13 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC131   = (int32_t)(0x20000000UL), ///< OCRC131 capture / compare match interrupt (ATU6 timer C subblock 13 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC130   = (int32_t)(0x10000000UL), ///< OCRC130 capture / compare match interrupt (ATU6 timer C subblock 13 channel 0 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC123   = (int32_t)(0x08000000UL), ///< OCRC123 capture / compare match interrupt (ATU6 timer C subblock 12 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC122   = (int32_t)(0x04000000UL), ///< OCRC122 capture / compare match interrupt (ATU6 timer C subblock 12 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC121   = (int32_t)(0x02000000UL), ///< OCRC121 capture / compare match interrupt (ATU6 timer C subblock 12 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC120   = (int32_t)(0x01000000UL), ///< OCRC120 capture / compare match interrupt (ATU6 timer C subblock 12 channel 0 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC113   = (int32_t)(0x00800000UL), ///< OCRC113 capture / compare match interrupt (ATU6 timer C subblock 11 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC112   = (int32_t)(0x00400000UL), ///< OCRC112 capture / compare match interrupt (ATU6 timer C subblock 11 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC111   = (int32_t)(0x00200000UL), ///< OCRC111 capture / compare match interrupt (ATU6 timer C subblock 11 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC110   = (int32_t)(0x00100000UL), ///< OCRC110 capture / compare match interrupt (ATU6 timer C subblock 11 channel 0 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC103   = (int32_t)(0x00080000UL), ///< OCRC103 capture / compare match interrupt (ATU6 timer C subblock 10 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC102   = (int32_t)(0x00040000UL), ///< OCRC102 capture / compare match interrupt (ATU6 timer C subblock 10 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC101   = (int32_t)(0x00020000UL), ///< OCRC101 capture / compare match interrupt (ATU6 timer C subblock 10 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC100   = (int32_t)(0x00010000UL), ///< OCRC100 capture / compare match interrupt (ATU6 timer C subblock 10 channel 0 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC7     = (int32_t)(0x00008000UL), ///< OCRG7 compare match interrupt (ATU6 timer G subblock 7).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC6     = (int32_t)(0x00004000UL), ///< OCRG6 compare match interrupt (ATU6 timer G subblock 6).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC5     = (int32_t)(0x00002000UL), ///< OCRG5 compare match interrupt (ATU6 timer G subblock 5).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC4     = (int32_t)(0x00001000UL), ///< OCRG4 compare match interrupt (ATU6 timer G subblock 4).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC3     = (int32_t)(0x00000800UL), ///< OCRG3 compare match interrupt (ATU6 timer G subblock 3).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC2     = (int32_t)(0x00000400UL), ///< OCRG2 compare match interrupt (ATU6 timer G subblock 2).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC1     = (int32_t)(0x00000200UL), ///< OCRG1 compare match interrupt (ATU6 timer G subblock 1).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC0     = (int32_t)(0x00000100UL), ///< OCRG0 compare match interrupt (ATU6 timer G subblock 0).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC93    = (int32_t)(0x00000080UL), ///< OCRC93 capture / compare match interrupt (ATU6 timer C subblock 9 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC92    = (int32_t)(0x00000040UL), ///< OCRC92 capture / compare match interrupt (ATU6 timer C subblock 9 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC91    = (int32_t)(0x00000020UL), ///< OCRC91 capture / compare match interrupt (ATU6 timer C subblock 9 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC90    = (int32_t)(0x00000010UL), ///< OCRC90 capture / compare match interrupt (ATU6 timer C subblock 9 channel 0 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC83    = (int32_t)(0x00000008UL), ///< OCRC83 capture / compare match interrupt (ATU6 timer C subblock 8 channel 3 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC82    = (int32_t)(0x00000004UL), ///< OCRC82 capture / compare match interrupt (ATU6 timer C subblock 8 channel 2 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC81    = (int32_t)(0x00000002UL), ///< OCRC81 capture / compare match interrupt (ATU6 timer C subblock 8 channel 1 capture / compare match interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC80    = (int32_t)(0x00000001UL), ///< OCRC80 capture / compare match interrupt (ATU6 timer C subblock 8 channel 0 capture / compare match interrupt).
} pic_dsadc_start_trigger_sel_atu_c_g_compare_match_t;

/** Select triggers from ATU timer D compare match A and ATU timer G compare match to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_start_trigger_sel_atu_d_g_compare_match_a
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG13   = (0x20000000UL), ///< OCRG13 compare match interrupt (ATU6 timer G subblock 13).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG12   = (0x10000000UL), ///< OCRG12 compare match interrupt (ATU6 timer G subblock 12).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG11   = (0x08000000UL), ///< OCRG11 compare match interrupt (ATU6 timer G subblock 11).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG10   = (0x04000000UL), ///< OCRG10 compare match interrupt (ATU6 timer G subblock 10).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG9    = (0x02000000UL), ///< OCRG9 compare match interrupt (ATU6 timer G subblock 9).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG8    = (0x01000000UL), ///< OCRG8 compare match interrupt (ATU6 timer G subblock 8).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D113 = (0x00800000UL), ///< OCR1D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D112 = (0x00400000UL), ///< OCR1D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D111 = (0x00200000UL), ///< OCR1D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D110 = (0x00100000UL), ///< OCR1D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D93  = (0x00080000UL), ///< OCR1D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D92  = (0x00040000UL), ///< OCR1D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D91  = (0x00020000UL), ///< OCR1D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D90  = (0x00010000UL), ///< OCR1D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D63  = (0x00008000UL), ///< OCR1D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D62  = (0x00004000UL), ///< OCR1D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D61  = (0x00002000UL), ///< OCR1D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D60  = (0x00001000UL), ///< OCR1D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D43  = (0x00000800UL), ///< OCR1D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D42  = (0x00000400UL), ///< OCR1D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D41  = (0x00000200UL), ///< OCR1D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D40  = (0x00000100UL), ///< OCR1D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D23  = (0x00000080UL), ///< OCR1D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D22  = (0x00000040UL), ///< OCR1D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D21  = (0x00000020UL), ///< OCR1D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D20  = (0x00000010UL), ///< OCR1D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D03  = (0x00000008UL), ///< OCR1D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D02  = (0x00000004UL), ///< OCR1D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D01  = (0x00000002UL), ///< OCR1D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match A interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D00  = (0x00000001UL), ///< OCR1D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match A interrupt).
} pic_dsadc_start_trigger_sel_atu_d_g_compare_match_a_t;

/** Select triggers from ATU timer D compare match B to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_start_trigger_sel_atu_d_compare_match_b
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D113 = (0x00080000UL), ///< OCR2D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D112 = (0x00040000UL), ///< OCR2D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D111 = (0x00020000UL), ///< OCR2D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D110 = (0x00010000UL), ///< OCR2D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D93  = (0x00008000UL), ///< OCR2D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D92  = (0x00004000UL), ///< OCR2D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D91  = (0x00002000UL), ///< OCR2D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D90  = (0x00001000UL), ///< OCR2D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D63  = (0x00000800UL), ///< OCR2D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D62  = (0x00000400UL), ///< OCR2D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D61  = (0x00000200UL), ///< OCR2D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D60  = (0x00000100UL), ///< OCR2D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D43  = (0x00000080UL), ///< OCR2D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D42  = (0x00000040UL), ///< OCR2D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D41  = (0x00000020UL), ///< OCR2D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D40  = (0x00000010UL), ///< OCR2D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D23  = (0x00000008UL), ///< OCR2D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D22  = (0x00000004UL), ///< OCR2D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D21  = (0x00000002UL), ///< OCR2D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D20  = (0x00000001UL), ///< OCR2D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
} pic_dsadc_start_trigger_sel_atu_d_compare_match_b_t;

/** Select triggers from ATU timer D down-counter underflow for ADCKn scan group j.*/
typedef enum e_pic_dsadc_start_trigger_sel_atu_d_down_counter_underflow
{
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID93 = (0x00080000UL), ///< UDID93 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 3 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID92 = (0x00040000UL), ///< UDID92 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 2 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID91 = (0x00020000UL), ///< UDID91 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 1 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID90 = (0x00010000UL), ///< UDID90 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 0 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID63 = (0x00008000UL), ///< UDID63 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 3 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID62 = (0x00004000UL), ///< UDID62 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 2 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID61 = (0x00002000UL), ///< UDID61 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 1 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID60 = (0x00001000UL), ///< UDID60 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 0 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID43 = (0x00000800UL), ///< UDID43 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 3 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID42 = (0x00000400UL), ///< UDID42 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 2 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID41 = (0x00000200UL), ///< UDID41 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 1 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID40 = (0x00000100UL), ///< UDID40 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 0 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID23 = (0x00000080UL), ///< UDID23 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 3 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID22 = (0x00000040UL), ///< UDID22 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 2 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID21 = (0x00000020UL), ///< UDID21 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 1 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID20 = (0x00000010UL), ///< UDID20 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 0 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID03 = (0x00000008UL), ///< UDID03 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 3 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID02 = (0x00000004UL), ///< UDID02 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 2 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID01 = (0x00000002UL), ///< UDID01 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 1 down-counter underflow interrupt).
    PIC_DSADC_START_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID00 = (0x00000001UL), ///< UDID00 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 0 down-counter underflow interrupt).
} pic_dsadc_start_trigger_sel_atu_d_down_counter_underflow_t;

/** Stop trigger output control register 0n0 enables a trigger source from GTM to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_stop_trigger_bit
{
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG13_BIT_24_TO_16  = (0x08000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG13 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG13_BIT_08_TO_00  = (0x04000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG13 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG12_BIT_24_TO_16  = (0x02000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG12 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG12_BIT_08_TO_00  = (0x01000000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG12 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG11_BIT_24_TO_16  = (0x00800000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG11 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG11_BIT_08_TO_00  = (0x00400000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG11 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG10_BIT_24_TO_16  = (0x00200000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG10 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG10_BIT_08_TO_00  = (0x00100000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG10 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG9_BIT_24_TO_16   = (0x00080000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG9 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG9_BIT_08_TO_00   = (0x00040000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG9 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG8_BIT_24_TO_16   = (0x00020000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG8 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG8_BIT_08_TO_00   = (0x00010000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG8 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG7_BIT_24_TO_16   = (0x00008000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG7 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG7_BIT_08_TO_00   = (0x00004000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG7 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG6_BIT_24_TO_16   = (0x00002000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG6 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG6_BIT_08_TO_00   = (0x00001000UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG6 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG5_BIT_24_TO_16   = (0x00000800UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG5 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG5_BIT_08_TO_00   = (0x00000400UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG5 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG4_BIT_24_TO_16   = (0x00000200UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG4 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG4_BIT_08_TO_00   = (0x00000100UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG4 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG3_BIT_24_TO_16   = (0x00000080UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG3 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG3_BIT_08_TO_00   = (0x00000040UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG3 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG2_BIT_24_TO_16   = (0x00000020UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG2 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG2_BIT_08_TO_00   = (0x00000010UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG2 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG1_BIT_24_TO_16   = (0x00000008UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG1 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG1_BIT_08_TO_00   = (0x00000004UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG1 bits 8 to 0
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG0_BIT_24_TO_16   = (0x00000002UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG0 bits 24 to 16
    PIC_DSADC_STOP_TRIGGER_PIC2DSADTCFG0_BIT_08_TO_00   = (0x00000001UL), ///< Output value of GTM timer output selected by PIC2DSADTCFG0 bits 8 to 0
} pic_dsadc_stop_trigger_bit_t;

/** Stop trigger output control register 0n1 enables a trigger source from ENCA to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_stop_trigger_compare_or_capture
{
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATIEC  = (0x00000020UL), ///< Selects the INTENCA1IEC as a trigger source of DSADCn.
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATINT1 = (0x00000010UL), ///< Selects the INTENCA1I1 as a trigger source of DSADCn.
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA1_ENCATINT0 = (0x00000008UL), ///< Selects the INTENCA1I0 as a trigger source of DSADCn.
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATIEC  = (0x00000004UL), ///< Selects the INTENCA0IEC as a trigger source of DSADCn.
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATINT1 = (0x00000002UL), ///< Selects the INTENCA0I1 as a trigger source of DSADCn.
    PIC_DSADC_STOP_TRIGGER_COMPARE_OR_CAPTURE_ENCA0_ENCATIEC0 = (0x00000001UL), ///< Selects the INTENCA0I0 as a trigger source of DSADCn.
} pic_dsadc_stop_trigger_compare_or_capture_t;

/** Stop trigger output control register 0n2 enables a trigger source from ATU timer D 1shot pulse ON occurrence to be selected as a trigger for stopping DSADC. */
typedef enum e_pic_dsadc_stop_trigger_sel_atu_d_1shot_pulse_on
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D93 = (0x00080000UL), ///< 1shot pulse ON D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D92 = (0x00040000UL), ///< 1shot pulse ON D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D91 = (0x00020000UL), ///< 1shot pulse ON D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D90 = (0x00010000UL), ///< 1shot pulse ON D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D63 = (0x00008000UL), ///< 1shot pulse ON D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D62 = (0x00004000UL), ///< 1shot pulse ON D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D61 = (0x00002000UL), ///< 1shot pulse ON D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D60 = (0x00001000UL), ///< 1shot pulse ON D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D43 = (0x00000800UL), ///< 1shot pulse ON D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D42 = (0x00000400UL), ///< 1shot pulse ON D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D41 = (0x00000200UL), ///< 1shot pulse ON D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D40 = (0x00000100UL), ///< 1shot pulse ON D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D23 = (0x00000080UL), ///< 1shot pulse ON D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D22 = (0x00000040UL), ///< 1shot pulse ON D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D21 = (0x00000020UL), ///< 1shot pulse ON D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D20 = (0x00000010UL), ///< 1shot pulse ON D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D03 = (0x00000008UL), ///< 1shot pulse ON D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D02 = (0x00000004UL), ///< 1shot pulse ON D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D01 = (0x00000002UL), ///< 1shot pulse ON D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_ON_D00 = (0x00000001UL), ///< 1shot pulse ON D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_dsadc_stop_trigger_sel_atu_d_1shot_pulse_on_t;

/** Stop trigger output control register 0n3 enables a trigger source from ATU timer D 1shot pulse OFF occurrence to be selected as a trigger for stopping DSADC. */
typedef enum e_pic_dsadc_stop_trigger_sel_atu_d_1shot_pulse_off
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D93 = (0x00080000UL), ///< 1shot pulse OFF D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D92 = (0x00040000UL), ///< 1shot pulse OFF D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D91 = (0x00020000UL), ///< 1shot pulse OFF D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D90 = (0x00010000UL), ///< 1shot pulse OFF D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D63 = (0x00008000UL), ///< 1shot pulse OFF D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D62 = (0x00004000UL), ///< 1shot pulse OFF D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D61 = (0x00002000UL), ///< 1shot pulse OFF D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D60 = (0x00001000UL), ///< 1shot pulse OFF D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D43 = (0x00000800UL), ///< 1shot pulse OFF D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D42 = (0x00000400UL), ///< 1shot pulse OFF D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D41 = (0x00000200UL), ///< 1shot pulse OFF D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D40 = (0x00000100UL), ///< 1shot pulse OFF D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D23 = (0x00000080UL), ///< 1shot pulse OFF D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D22 = (0x00000040UL), ///< 1shot pulse OFF D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D21 = (0x00000020UL), ///< 1shot pulse OFF D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D20 = (0x00000010UL), ///< 1shot pulse OFF D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D03 = (0x00000008UL), ///< 1shot pulse OFF D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D02 = (0x00000004UL), ///< 1shot pulse OFF D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D01 = (0x00000002UL), ///< 1shot pulse OFF D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_1SHOT_PULSE_OFF_D00 = (0x00000001UL), ///< 1shot pulse OFF D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_dsadc_stop_trigger_sel_atu_d_1shot_pulse_off_t;

/** Select triggers from ATU timer C compare match and ATU timer G compare match to be selected as a trigger for starting DSADC..*/
typedef enum e_pic_dsadc_stop_trigger_sel_atu_c_g_compare_match
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC133   = (int32_t)(0x80000000UL), ///< OCRC133 capture / compare match interrupt (ATU6 timer C subblock 13 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC132   = (int32_t)(0x40000000UL), ///< OCRC132 capture / compare match interrupt (ATU6 timer C subblock 13 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC131   = (int32_t)(0x20000000UL), ///< OCRC131 capture / compare match interrupt (ATU6 timer C subblock 13 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC130   = (int32_t)(0x10000000UL), ///< OCRC130 capture / compare match interrupt (ATU6 timer C subblock 13 channel 0 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC123   = (int32_t)(0x08000000UL), ///< OCRC123 capture / compare match interrupt (ATU6 timer C subblock 12 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC122   = (int32_t)(0x04000000UL), ///< OCRC122 capture / compare match interrupt (ATU6 timer C subblock 12 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC121   = (int32_t)(0x02000000UL), ///< OCRC121 capture / compare match interrupt (ATU6 timer C subblock 12 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC120   = (int32_t)(0x01000000UL), ///< OCRC120 capture / compare match interrupt (ATU6 timer C subblock 12 channel 0 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC113   = (int32_t)(0x00800000UL), ///< OCRC113 capture / compare match interrupt (ATU6 timer C subblock 11 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC112   = (int32_t)(0x00400000UL), ///< OCRC112 capture / compare match interrupt (ATU6 timer C subblock 11 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC111   = (int32_t)(0x00200000UL), ///< OCRC111 capture / compare match interrupt (ATU6 timer C subblock 11 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC110   = (int32_t)(0x00100000UL), ///< OCRC110 capture / compare match interrupt (ATU6 timer C subblock 11 channel 0 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC103   = (int32_t)(0x00080000UL), ///< OCRC103 capture / compare match interrupt (ATU6 timer C subblock 10 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC102   = (int32_t)(0x00040000UL), ///< OCRC102 capture / compare match interrupt (ATU6 timer C subblock 10 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC101   = (int32_t)(0x00020000UL), ///< OCRC101 capture / compare match interrupt (ATU6 timer C subblock 10 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC100   = (int32_t)(0x00010000UL), ///< OCRC100 capture / compare match interrupt (ATU6 timer C subblock 10 channel 0 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG7     = (int32_t)(0x00008000UL), ///< OCRG7 compare match interrupt (ATU6 timer G subblock 7).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG6     = (int32_t)(0x00004000UL), ///< OCRG6 compare match interrupt (ATU6 timer G subblock 6).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG5     = (int32_t)(0x00002000UL), ///< OCRG5 compare match interrupt (ATU6 timer G subblock 5).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG4     = (int32_t)(0x00001000UL), ///< OCRG4 compare match interrupt (ATU6 timer G subblock 4).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG3     = (int32_t)(0x00000800UL), ///< OCRG3 compare match interrupt (ATU6 timer G subblock 3).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG2     = (int32_t)(0x00000400UL), ///< OCRG2 compare match interrupt (ATU6 timer G subblock 2).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG1     = (int32_t)(0x00000200UL), ///< OCRG1 compare match interrupt (ATU6 timer G subblock 1).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRG0     = (int32_t)(0x00000100UL), ///< OCRG0 compare match interrupt (ATU6 timer G subblock 0).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC93    = (int32_t)(0x00000080UL), ///< OCRC93 capture / compare match interrupt (ATU6 timer C subblock 9 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC92    = (int32_t)(0x00000040UL), ///< OCRC92 capture / compare match interrupt (ATU6 timer C subblock 9 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC91    = (int32_t)(0x00000020UL), ///< OCRC91 capture / compare match interrupt (ATU6 timer C subblock 9 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC90    = (int32_t)(0x00000010UL), ///< OCRC90 capture / compare match interrupt (ATU6 timer C subblock 9 channel 0 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC83    = (int32_t)(0x00000008UL), ///< OCRC83 capture / compare match interrupt (ATU6 timer C subblock 8 channel 3 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC82    = (int32_t)(0x00000004UL), ///< OCRC82 capture / compare match interrupt (ATU6 timer C subblock 8 channel 2 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC81    = (int32_t)(0x00000002UL), ///< OCRC81 capture / compare match interrupt (ATU6 timer C subblock 8 channel 1 capture / compare match interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_C_G_COMPARE_MATCH_OCRC80    = (int32_t)(0x00000001UL), ///< OCRC80 capture / compare match interrupt (ATU6 timer C subblock 8 channel 0 capture / compare match interrupt).
} pic_dsadc_stop_trigger_sel_atu_c_g_compare_match_t;

/** Select triggers from ATU timer D compare match A and ATU timer G compare match to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_stop_trigger_sel_atu_d_g_compare_match_a
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG13   = (0x20000000UL), ///< OCRG13 compare match interrupt (ATU6 timer G subblock 13).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG12   = (0x10000000UL), ///< OCRG12 compare match interrupt (ATU6 timer G subblock 12).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG11   = (0x08000000UL), ///< OCRG11 compare match interrupt (ATU6 timer G subblock 11).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG10   = (0x04000000UL), ///< OCRG10 compare match interrupt (ATU6 timer G subblock 10).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG9    = (0x02000000UL), ///< OCRG9 compare match interrupt (ATU6 timer G subblock 9).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCRG8    = (0x01000000UL), ///< OCRG8 compare match interrupt (ATU6 timer G subblock 8).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D113 = (0x00800000UL), ///< OCR1D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D112 = (0x00400000UL), ///< OCR1D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D111 = (0x00200000UL), ///< OCR1D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D110 = (0x00100000UL), ///< OCR1D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D93  = (0x00080000UL), ///< OCR1D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D92  = (0x00040000UL), ///< OCR1D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D91  = (0x00020000UL), ///< OCR1D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D90  = (0x00010000UL), ///< OCR1D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D63  = (0x00008000UL), ///< OCR1D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D62  = (0x00004000UL), ///< OCR1D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D61  = (0x00002000UL), ///< OCR1D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D60  = (0x00001000UL), ///< OCR1D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D43  = (0x00000800UL), ///< OCR1D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D42  = (0x00000400UL), ///< OCR1D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D41  = (0x00000200UL), ///< OCR1D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D40  = (0x00000100UL), ///< OCR1D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D23  = (0x00000080UL), ///< OCR1D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D22  = (0x00000040UL), ///< OCR1D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D21  = (0x00000020UL), ///< OCR1D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D20  = (0x00000010UL), ///< OCR1D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D03  = (0x00000008UL), ///< OCR1D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D02  = (0x00000004UL), ///< OCR1D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D01  = (0x00000002UL), ///< OCR1D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match A interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_G_COMPARE_MATCH_A_OCR1D00  = (0x00000001UL), ///< OCR1D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match A interrupt).
} pic_dsadc_stop_trigger_sel_atu_d_g_compare_match_a_t;

/** Select triggers from ATU timer D compare match B to be selected as a trigger for starting DSADC.*/
typedef enum e_pic_dsadc_stop_trigger_sel_atu_d_compare_match_b
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D113 = (0x00080000UL), ///< OCR2D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D112 = (0x00040000UL), ///< OCR2D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D111 = (0x00020000UL), ///< OCR2D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D110 = (0x00010000UL), ///< OCR2D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D93  = (0x00008000UL), ///< OCR2D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D92  = (0x00004000UL), ///< OCR2D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D91  = (0x00002000UL), ///< OCR2D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D90  = (0x00001000UL), ///< OCR2D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D63  = (0x00000800UL), ///< OCR2D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D62  = (0x00000400UL), ///< OCR2D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D61  = (0x00000200UL), ///< OCR2D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D60  = (0x00000100UL), ///< OCR2D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D43  = (0x00000080UL), ///< OCR2D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D42  = (0x00000040UL), ///< OCR2D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D41  = (0x00000020UL), ///< OCR2D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D40  = (0x00000010UL), ///< OCR2D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D23  = (0x00000008UL), ///< OCR2D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D22  = (0x00000004UL), ///< OCR2D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D21  = (0x00000002UL), ///< OCR2D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_COMPARE_MATCH_B_OCR2D20  = (0x00000001UL), ///< OCR2D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
} pic_dsadc_stop_trigger_sel_atu_d_compare_match_b_t;

/** Select triggers from ATU timer D down-counter underflow for ADCKn scan group j.*/
typedef enum e_pic_dsadc_stop_trigger_sel_atu_d_down_counter_underflow
{
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID93 = (0x00080000UL), ///< UDID93 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 3 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID92 = (0x00040000UL), ///< UDID92 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 2 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID91 = (0x00020000UL), ///< UDID91 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 1 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID90 = (0x00010000UL), ///< UDID90 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 0 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID63 = (0x00008000UL), ///< UDID63 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 3 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID62 = (0x00004000UL), ///< UDID62 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 2 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID61 = (0x00002000UL), ///< UDID61 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 1 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID60 = (0x00001000UL), ///< UDID60 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 0 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID43 = (0x00000800UL), ///< UDID43 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 3 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID42 = (0x00000400UL), ///< UDID42 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 2 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID41 = (0x00000200UL), ///< UDID41 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 1 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID40 = (0x00000100UL), ///< UDID40 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 0 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID23 = (0x00000080UL), ///< UDID23 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 3 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID22 = (0x00000040UL), ///< UDID22 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 2 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID21 = (0x00000020UL), ///< UDID21 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 1 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID20 = (0x00000010UL), ///< UDID20 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 0 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID03 = (0x00000008UL), ///< UDID03 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 3 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID02 = (0x00000004UL), ///< UDID02 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 2 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID01 = (0x00000002UL), ///< UDID01 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 1 down-counter underflow interrupt).
    PIC_DSADC_STOP_TRIGGER_SEL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID00 = (0x00000001UL), ///< UDID00 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 0 down-counter underflow interrupt).
} pic_dsadc_stop_trigger_sel_atu_d_down_counter_underflow_t;

/** DSADC trigger selection control register00 selects a trigger for starting DSADCj.*/
typedef enum e_pic_dsadc_trigger_sel_ctrl_reg00
{
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC21       = (0x08000000UL), ///< Selects the DSADTRG21 pin as a trigger source for starting DSADC21.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC21 = (0x04000000UL), ///< Selects the trigger selected by DSADC start trigger output control register 013j (j = 0 to 3) as a trigger source for starting DSADC21.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC22       = (0x02000000UL), ///< Selects the DSADTRG22 pin as a trigger source for starting DSADC22.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC22 = (0x01000000UL), ///< Selects the trigger selected by DSADC start trigger output control register 012j (j = 0 to 3) as a trigger source for starting DSADC22.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC14       = (0x00800000UL), ///< Selects the DSADTRG14 pin as a trigger source for starting DSADC14.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC14 = (0x00400000UL), ///< Selects the trigger selected by DSADC start trigger output control register 011j (j = 0 to 3) as a trigger source for starting DSADC14.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC15       = (0x00200000UL), ///< Selects the DSADTRG15 pin as a trigger source for starting DSADC15.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC15 = (0x00100000UL), ///< Selects the trigger selected by DSADC start trigger output control register 010j (j = 0 to 3) as a trigger source for starting DSADC15.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC11       = (0x00080000UL), ///< Selects the DSADTRG11 pin as a trigger source for starting DSADC11.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC11 = (0x00040000UL), ///< Selects the trigger selected by DSADC start trigger output control register 09j (j = 0 to 3) as a trigger source for starting DSADC11.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC13       = (0x00020000UL), ///< Selects the DSADTRG13 pin as a trigger source for starting DSADC13.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC13 = (0x00010000UL), ///< Selects the trigger selected by DSADC start trigger output control register 08j (j = 0 to 3) as a trigger source for starting DSADC13.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC12       = (0x00000080UL), ///< Selects the DSADTRG12 pin as a trigger source for starting DSADC12.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC12 = (0x00000040UL), ///< Selects the trigger selected by DSADC start trigger output control register 03j (j = 0 to 3) as a trigger source for starting DSADC12.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC20       = (0x00000020UL), ///< Selects the DSADTRG20 pin as a trigger source for starting DSADC20.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC20 = (0x00000010UL), ///< Selects the trigger selected by DSADC start trigger output control register 02j (j = 0 to 3) as a trigger source for starting DSADC20.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC10       = (0x00000008UL), ///< Selects the DSADTRG10 pin as a trigger source for starting DSADC10.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC10 = (0x00000004UL), ///< Selects the trigger selected by DSADC start trigger output control register 01j (j = 0 to 3) as a trigger source for starting DSADC10.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG00_DSADC00       = (0x00000002UL), ///< Selects the DSADTRG00 pin as a trigger source for starting DSADC00.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG00_DSADC00 = (0x00000001UL), ///< Selects the trigger selected by DSADC start trigger output control register 00j (j = 0 to 3) as a trigger source for starting DSADC00.
} pic_dsadc_trigger_sel_ctrl_reg00_t;

/** DSADC trigger selection control register01 selects a trigger for stopping DSADCj.*/
typedef enum e_pic_dsadc_trigger_sel_ctrl_reg01
{
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC21       = (0x08000000UL), ///< Selects the DSADTRG21 pin as a trigger source for stopping DSADC21.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC21  = (0x04000000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 013j (j = 0 to 3) as a trigger source for starting DSADC21.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC22       = (0x02000000UL), ///< Selects the DSADTRG22 pin as a trigger source for stopping DSADC22.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC22  = (0x01000000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 012j (j = 0 to 3) as a trigger source for starting DSADC22.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC14       = (0x00800000UL), ///< Selects the DSADTRG14 pin as a trigger source for stopping DSADC14.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC14  = (0x00400000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 011j (j = 0 to 3) as a trigger source for starting DSADC14.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC15       = (0x00200000UL), ///< Selects the DSADTRG15 pin as a trigger source for stopping DSADC15.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC15  = (0x00100000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 010j (j = 0 to 3) as a trigger source for starting DSADC15.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC11       = (0x00080000UL), ///< Selects the DSADTRG11 pin as a trigger source for stopping DSADC11.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC11  = (0x00040000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 09j (j = 0 to 3) as a trigger source for starting DSADC11.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC13       = (0x00020000UL), ///< Selects the DSADTRG13 pin as a trigger source for stopping DSADC13.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC13  = (0x00010000UL), ///< Selects the trigger selected by DSADC stop trigger output control register 08j (j = 0 to 3) as a trigger source for starting DSADC13.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC12       = (0x00000080UL), ///< Selects the DSADTRG12 pin as a trigger source for stopping DSADC12.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC12  = (0x00000040UL), ///< Selects the trigger selected by DSADC stop trigger output control register 03j (j = 0 to 3) as a trigger source for starting DSADC12.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC20       = (0x00000020UL), ///< Selects the DSADTRG20 pin as a trigger source for stopping DSADC20.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC20  = (0x00000010UL), ///< Selects the trigger selected by DSADC stop trigger output control register 02j (j = 0 to 3) as a trigger source for starting DSADC20.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC10       = (0x00000008UL), ///< Selects the DSADTRG10 pin as a trigger source for stopping DSADC10.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC10  = (0x00000004UL), ///< Selects the trigger selected by DSADC stop trigger output control register 01j (j = 0 to 3) as a trigger source for starting DSADC10.
    PIC_DSADC_TRIGGER_SEL_CTRL_REG01_DSADC00       = (0x00000002UL), ///< Selects the DSADTRG00 pin as a trigger source for stopping DSADC00.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG01_DSADC00  = (0x00000001UL), ///< Selects the trigger selected by DSADC stop trigger output control register 00j (j = 0 to 3) as a trigger source for starting DSADC00.
} pic_dsadc_trigger_sel_ctrl_reg01_t;

/** DSADC trigger selection control register10 selects a trigger for starting DSADCj.*/
typedef enum e_pic_dsadc_start_trigger_sel_ctrl_reg10
{
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC21  = (0x04000000UL), ///< Selects the trigger selected by DSADC start trigger output control register 113j (j = 0 to 3) as a trigger source for starting DSADC21.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC22  = (0x01000000UL), ///< Selects the trigger selected by DSADC start trigger output control register 112j (j = 0 to 3) as a trigger source for starting DSADC22.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC14  = (0x00400000UL), ///< Selects the trigger selected by DSADC start trigger output control register 111j (j = 0 to 3) as a trigger source for starting DSADC14.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC15  = (0x00100000UL), ///< Selects the trigger selected by DSADC start trigger output control register 110j (j = 0 to 3) as a trigger source for starting DSADC15.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC11  = (0x00040000UL), ///< Selects the trigger selected by DSADC start trigger output control register 19j (j = 0 to 3) as a trigger source for starting DSADC11.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC13  = (0x00010000UL), ///< Selects the trigger selected by DSADC start trigger output control register 18j (j = 0 to 3) as a trigger source for starting DSADC13.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC12  = (0x00000040UL), ///< Selects the trigger selected by DSADC start trigger output control register 13j (j = 0 to 3) as a trigger source for starting DSADC12.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC20  = (0x00000010UL), ///< Selects the trigger selected by DSADC start trigger output control register 12j (j = 0 to 3) as a trigger source for starting DSADC20.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC10  = (0x00000004UL), ///< Selects the trigger selected by DSADC start trigger output control register 11j (j = 0 to 3) as a trigger source for starting DSADC10.
    PIC_DSADC_START_TRIGGER_SEL_CTRL_REG10_DSADC00  = (0x00000001UL), ///< Selects the trigger selected by DSADC start trigger output control register 10j (j = 0 to 3) as a trigger source for starting DSADC00.
} pic_dsadc_start_trigger_sel_ctrl_reg10_t;

/** DSADC trigger selection control register11 selects a trigger for stopping DSADCj.*/
typedef enum e_pic_dsadc_stop_trigger_sel_ctrl_reg11
{
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC21  = (0x04000000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 113j (j = 0 to 3) as a trigger source for starting DSADC21.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC22  = (0x01000000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 112j (j = 0 to 3) as a trigger source for starting DSADC22.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC14  = (0x00400000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 111j (j = 0 to 3) as a trigger source for starting DSADC14.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC15  = (0x00100000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 110j (j = 0 to 3) as a trigger source for starting DSADC15.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC11  = (0x00040000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 19j (j = 0 to 3) as a trigger source for starting DSADC11.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC13  = (0x00010000UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 18j (j = 0 to 3) as a trigger source for starting DSADC13.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC12  = (0x00000040UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 13j (j = 0 to 3) as a trigger source for starting DSADC12.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC20  = (0x00000010UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 12j (j = 0 to 3) as a trigger source for starting DSADC20.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC10  = (0x00000004UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 11j (j = 0 to 3) as a trigger source for starting DSADC10.
    PIC_DSADC_STOP_TRIGGER_SEL_CTRL_REG11_DSADC00  = (0x00000001UL), ///< Selects the trigger selected by DSADC stopping trigger output control register 10j (j = 0 to 3) as a trigger source for starting DSADC00.
} pic_dsadc_stop_trigger_sel_ctrl_reg11_t;

/** CADC Trigger Selection Control Register 00 . */
typedef enum e_pic_cadc_trigger_sel_ctrl_reg00
{
    PIC_CADC_TRIGGER_SEL_CTRL_REG00_CADCATSEL1 = (0x00000002UL), ///< Selects the CADTRG00 pin as a trigger source for starting CADC00.
    PIC_CADC_TRIGGER_SEL_CTRL_REG00_CADCATSEL0 = (0x00000001UL), ///< Selects the trigger selected by CADC start trigger output control register 00j (j = 0 to 3) as a trigger source for starting CADC00.
} pic_cadc_trigger_sel_ctrl_reg00_t;

/** CADC Trigger Selection Control Register 01 . */
typedef enum e_pic_cadc_trigger_sel_ctrl_reg01
{
    PIC_CADC_TRIGGER_SEL_CTRL_REG01_CADCATSEL1  = (0x00000002UL), ///< Selects the CADTRG00 pin as a trigger source for stopping CADC00.
    PIC_CADC_TRIGGER_SEL_CTRL_REG01_CADCATSEL0  = (0x00000001UL), ///< Selects the trigger selected by CADC stop trigger output control register 00j (j = 0 to 3) as a trigger source for starting CADC00.
} pic_cadc_trigger_sel_ctrl_reg01_t;

/** CADC Trigger Selection Control Register 10 . */
typedef enum e_pic_cadc_trigger_sel_ctrl_reg10
{
    PIC_CADC_TRIGGER_SEL_CTRL_REG10  = (0x00000001UL), ///< Selects the trigger selected by CADC start trigger output control register 10j (j = 0 to 3) as a trigger source for starting CADC00.
} pic_cadc_trigger_sel_ctrl_reg10_t;

/** CADC Trigger Selection Control Register 10 . */
typedef enum e_pic_cadc_trigger_sel_ctrl_reg11
{
    PIC_CADC_TRIGGER_SEL_CTRL_REG11  = (0x00000001UL), ///< Selects the trigger selected by CADC stop trigger output control register 11j (j = 0 to 3) as a trigger source for starting CADC00.
} pic_cadc_trigger_sel_ctrl_reg11_t;

/** CADC start trigger output control register 000 enables a trigger source from GTM to be selected as a trigger for starting CADC00. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_reg00
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_REG00_CADTEN001 = (0x00000002UL), ///< Output value of GTM timer output selected by PIC2CADTCFG0 bits 24 to 16.
    PIC_CADC_START_TRIGGER_OUT_CTRL_REG00_CADTEN000 = (0x00000001UL), ///< Output value of GTM timer output selected by PIC2CADTCFG0 bit 8 to 0.
} pic_cadc_start_trigger_out_ctrl_reg00_t;

/** CADC start trigger output control register 001 enables a trigger source from ENCA to be selected as a trigger for starting CADC00. */
typedef enum e_pic_cadc_start_trigger_out_ctrl
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA1_ENCATIEC  = (0x00000020UL), ///< Selects the INTENCA1IEC as a trigger source of CADC00.
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA1_ENCATINT1 = (0x00000010UL), ///< Selects the INTENCA1I1 as a trigger source of CADC00.
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA1_ENCATINT0 = (0x00000008UL), ///< Selects the INTENCA1I0 as a trigger source of CADC00.
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA0_ENCATIEC  = (0x00000004UL), ///< Selects the INTENCA0IEC as a trigger source of CADC00.
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA0_ENCATINT1 = (0x00000002UL), ///< Selects the INTENCA0I1 as a trigger source of CADC00.
    PIC_CADC_START_TRIGGER_OUT_CTRL_ENCA0_ENCATIEC0 = (0x00000001UL), ///< Selects the INTENCA0I0 as a trigger source of CADC00.
} pic_cadc_start_trigger_out_ctrl_t;

/** CADC start trigger output control register 002 enables a trigger source from ATU timer D 1shot pulse ON occurrence to be selected as a trigger for starting CADC00. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_on
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D93 = (0x00080000UL), ///< 1shot pulse ON D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D92 = (0x00040000UL), ///< 1shot pulse ON D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D91 = (0x00020000UL), ///< 1shot pulse ON D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D90 = (0x00010000UL), ///< 1shot pulse ON D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D63 = (0x00008000UL), ///< 1shot pulse ON D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D62 = (0x00004000UL), ///< 1shot pulse ON D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D61 = (0x00002000UL), ///< 1shot pulse ON D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D60 = (0x00001000UL), ///< 1shot pulse ON D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D43 = (0x00000800UL), ///< 1shot pulse ON D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D42 = (0x00000400UL), ///< 1shot pulse ON D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D41 = (0x00000200UL), ///< 1shot pulse ON D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D40 = (0x00000100UL), ///< 1shot pulse ON D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D23 = (0x00000080UL), ///< 1shot pulse ON D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D22 = (0x00000040UL), ///< 1shot pulse ON D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D21 = (0x00000020UL), ///< 1shot pulse ON D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D20 = (0x00000010UL), ///< 1shot pulse ON D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D03 = (0x00000008UL), ///< 1shot pulse ON D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D02 = (0x00000004UL), ///< 1shot pulse ON D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D01 = (0x00000002UL), ///< 1shot pulse ON D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D00 = (0x00000001UL), ///< 1shot pulse ON D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_on_t;

/** CADC start trigger output control register 003 enables a trigger source from ATU timer D 1shot pulse OFF occurrence to be selected as a trigger for starting CADC00. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_off
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D93 = (0x00080000UL), ///< 1shot pulse OFF D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D92 = (0x00040000UL), ///< 1shot pulse OFF D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D91 = (0x00020000UL), ///< 1shot pulse OFF D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D90 = (0x00010000UL), ///< 1shot pulse OFF D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D63 = (0x00008000UL), ///< 1shot pulse OFF D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D62 = (0x00004000UL), ///< 1shot pulse OFF D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D61 = (0x00002000UL), ///< 1shot pulse OFF D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D60 = (0x00001000UL), ///< 1shot pulse OFF D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D43 = (0x00000800UL), ///< 1shot pulse OFF D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D42 = (0x00000400UL), ///< 1shot pulse OFF D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D41 = (0x00000200UL), ///< 1shot pulse OFF D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D40 = (0x00000100UL), ///< 1shot pulse OFF D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D23 = (0x00000080UL), ///< 1shot pulse OFF D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D22 = (0x00000040UL), ///< 1shot pulse OFF D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D21 = (0x00000020UL), ///< 1shot pulse OFF D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D20 = (0x00000010UL), ///< 1shot pulse OFF D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D03 = (0x00000008UL), ///< 1shot pulse OFF D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D02 = (0x00000004UL), ///< 1shot pulse OFF D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D01 = (0x00000002UL), ///< 1shot pulse OFF D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D00 = (0x00000001UL), ///< 1shot pulse OFF D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_off_t;

/** Select triggers from ATU timer C compare match and ATU timer G compare match to be selected as a trigger for starting CADC. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_c_g_compare_match
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC133   = (int32_t)(0x80000000UL), ///< OCRC133 capture / compare match interrupt (ATU6 timer C subblock 13 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC132   = (int32_t)(0x40000000UL), ///< OCRC132 capture / compare match interrupt (ATU6 timer C subblock 13 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC131   = (int32_t)(0x20000000UL), ///< OCRC131 capture / compare match interrupt (ATU6 timer C subblock 13 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC130   = (int32_t)(0x10000000UL), ///< OCRC130 capture / compare match interrupt (ATU6 timer C subblock 13 channel 0 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC123   = (int32_t)(0x08000000UL), ///< OCRC123 capture / compare match interrupt (ATU6 timer C subblock 12 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC122   = (int32_t)(0x04000000UL), ///< OCRC122 capture / compare match interrupt (ATU6 timer C subblock 12 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC121   = (int32_t)(0x02000000UL), ///< OCRC121 capture / compare match interrupt (ATU6 timer C subblock 12 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC120   = (int32_t)(0x01000000UL), ///< OCRC120 capture / compare match interrupt (ATU6 timer C subblock 12 channel 0 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC113   = (int32_t)(0x00800000UL), ///< OCRC113 capture / compare match interrupt (ATU6 timer C subblock 11 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC112   = (int32_t)(0x00400000UL), ///< OCRC112 capture / compare match interrupt (ATU6 timer C subblock 11 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC111   = (int32_t)(0x00200000UL), ///< OCRC111 capture / compare match interrupt (ATU6 timer C subblock 11 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC110   = (int32_t)(0x00100000UL), ///< OCRC110 capture / compare match interrupt (ATU6 timer C subblock 11 channel 0 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC103   = (int32_t)(0x00080000UL), ///< OCRC103 capture / compare match interrupt (ATU6 timer C subblock 10 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC102   = (int32_t)(0x00040000UL), ///< OCRC102 capture / compare match interrupt (ATU6 timer C subblock 10 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC101   = (int32_t)(0x00020000UL), ///< OCRC101 capture / compare match interrupt (ATU6 timer C subblock 10 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC100   = (int32_t)(0x00010000UL), ///< OCRC100 capture / compare match interrupt (ATU6 timer C subblock 10 channel 0 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG7     = (int32_t)(0x00008000UL), ///< OCRG7 compare match interrupt (ATU6 timer G subblock 7).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG6     = (int32_t)(0x00004000UL), ///< OCRG6 compare match interrupt (ATU6 timer G subblock 6).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG5     = (int32_t)(0x00002000UL), ///< OCRG5 compare match interrupt (ATU6 timer G subblock 5).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG4     = (int32_t)(0x00001000UL), ///< OCRG4 compare match interrupt (ATU6 timer G subblock 4).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG3     = (int32_t)(0x00000800UL), ///< OCRG3 compare match interrupt (ATU6 timer G subblock 3).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG2     = (int32_t)(0x00000400UL), ///< OCRG2 compare match interrupt (ATU6 timer G subblock 2).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG1     = (int32_t)(0x00000200UL), ///< OCRG1 compare match interrupt (ATU6 timer G subblock 1).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG0     = (int32_t)(0x00000100UL), ///< OCRG0 compare match interrupt (ATU6 timer G subblock 0).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC93    = (int32_t)(0x00000080UL), ///< OCRC93 capture / compare match interrupt (ATU6 timer C subblock 9 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC92    = (int32_t)(0x00000040UL), ///< OCRC92 capture / compare match interrupt (ATU6 timer C subblock 9 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC91    = (int32_t)(0x00000020UL), ///< OCRC91 capture / compare match interrupt (ATU6 timer C subblock 9 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC90    = (int32_t)(0x00000010UL), ///< OCRC90 capture / compare match interrupt (ATU6 timer C subblock 9 channel 0 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC83    = (int32_t)(0x00000008UL), ///< OCRC83 capture / compare match interrupt (ATU6 timer C subblock 8 channel 3 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC82    = (int32_t)(0x00000004UL), ///< OCRC82 capture / compare match interrupt (ATU6 timer C subblock 8 channel 2 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC81    = (int32_t)(0x00000002UL), ///< OCRC81 capture / compare match interrupt (ATU6 timer C subblock 8 channel 1 capture / compare match interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC80    = (int32_t)(0x00000001UL), ///< OCRC80 capture / compare match interrupt (ATU6 timer C subblock 8 channel 0 capture / compare match interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_c_g_compare_match_t;

/** Select triggers from ATU timer D compare match B to be selected as a trigger for starting CADC . */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_d_compare_match_b
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D113 = (0x00800000UL), ///< OCR2D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D112 = (0x00400000UL), ///< OCR2D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D111 = (0x00200000UL), ///< OCR2D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D110 = (0x00100000UL), ///< OCR2D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D93  = (0x00080000UL), ///< OCR2D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D92  = (0x00040000UL), ///< OCR2D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D91  = (0x00020000UL), ///< OCR2D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D90  = (0x00010000UL), ///< OCR2D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D63  = (0x00008000UL), ///< OCR2D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D62  = (0x00004000UL), ///< OCR2D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D61  = (0x00002000UL), ///< OCR2D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D60  = (0x00001000UL), ///< OCR2D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D43  = (0x00000800UL), ///< OCR2D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D42  = (0x00000400UL), ///< OCR2D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D41  = (0x00000200UL), ///< OCR2D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D40  = (0x00000100UL), ///< OCR2D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D23  = (0x00000080UL), ///< OCR2D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D22  = (0x00000040UL), ///< OCR2D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D21  = (0x00000020UL), ///< OCR2D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D20  = (0x00000010UL), ///< OCR2D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D03  = (0x00000008UL), ///< OCR2D03 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D02  = (0x00000004UL), ///< OCR2D02 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D01  = (0x00000002UL), ///< OCR2D01 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D00  = (0x00000001UL), ///< OCR2D00 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_d_compare_match_b_t;

/** Select triggers from ATU timer D compare match A and ATU timer G compare match to be selected as a trigger for starting CADC. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_d_g_compare_match_a
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG13   = (0x20000000UL), ///< OCRG13 compare match interrupt (ATU6 timer G subblock 13).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG12   = (0x10000000UL), ///< OCRG12 compare match interrupt (ATU6 timer G subblock 12).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG11   = (0x08000000UL), ///< OCRG11 compare match interrupt (ATU6 timer G subblock 11).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG10   = (0x04000000UL), ///< OCRG10 compare match interrupt (ATU6 timer G subblock 10).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG9    = (0x02000000UL), ///< OCRG9 compare match interrupt (ATU6 timer G subblock 9).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG8    = (0x01000000UL), ///< OCRG8 compare match interrupt (ATU6 timer G subblock 8).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D113 = (0x00800000UL), ///< OCR1D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D112 = (0x00400000UL), ///< OCR1D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D111 = (0x00200000UL), ///< OCR1D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D110 = (0x00100000UL), ///< OCR1D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D93  = (0x00080000UL), ///< OCR1D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D92  = (0x00040000UL), ///< OCR1D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D91  = (0x00020000UL), ///< OCR1D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D90  = (0x00010000UL), ///< OCR1D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D63  = (0x00008000UL), ///< OCR1D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D62  = (0x00004000UL), ///< OCR1D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D61  = (0x00002000UL), ///< OCR1D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D60  = (0x00001000UL), ///< OCR1D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D43  = (0x00000800UL), ///< OCR1D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D42  = (0x00000400UL), ///< OCR1D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D41  = (0x00000200UL), ///< OCR1D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D40  = (0x00000100UL), ///< OCR1D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D23  = (0x00000080UL), ///< OCR1D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D22  = (0x00000040UL), ///< OCR1D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D21  = (0x00000020UL), ///< OCR1D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D20  = (0x00000010UL), ///< OCR1D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match A interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D03  = (0x00000008UL), ///< OCR1D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match B interrup).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D02  = (0x00000004UL), ///< OCR1D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D01  = (0x00000002UL), ///< OCR1D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match B interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D00  = (0x00000001UL), ///< OCR1D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match B interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_d_g_compare_match_a_t;

/** Select triggers from ATU timer D down-counter underflow for starting CADC00. */
typedef enum e_pic_cadc_start_trigger_out_ctrl_atu_d_down_counter_underflow
{
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID93 = (0x00080000UL), ///< UDID93 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 3 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID92 = (0x00040000UL), ///< UDID92 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 2 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID91 = (0x00020000UL), ///< UDID91 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 1 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID90 = (0x00010000UL), ///< UDID90 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 0 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID63 = (0x00008000UL), ///< UDID63 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 3 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID62 = (0x00004000UL), ///< UDID62 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 2 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID61 = (0x00002000UL), ///< UDID61 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 1 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID60 = (0x00001000UL), ///< UDID60 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 0 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID43 = (0x00000800UL), ///< UDID43 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 3 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID42 = (0x00000400UL), ///< UDID42 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 2 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID41 = (0x00000200UL), ///< UDID41 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 1 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID40 = (0x00000100UL), ///< UDID40 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 0 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID23 = (0x00000080UL), ///< UDID23 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 3 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID22 = (0x00000040UL), ///< UDID22 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 2 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID21 = (0x00000020UL), ///< UDID21 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 1 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID20 = (0x00000010UL), ///< UDID20 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 0 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID03 = (0x00000008UL), ///< UDID03 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 3 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID02 = (0x00000004UL), ///< UDID02 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 2 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID01 = (0x00000002UL), ///< UDID01 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 1 down-counter underflow interrupt).
    PIC_CADC_START_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID00 = (0x00000001UL), ///< UDID00 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 0 down-counter underflow interrupt).
} pic_cadc_start_trigger_out_ctrl_atu_d_down_counter_underflow_t;

/** CADC stop trigger output control register 000 enables a trigger source from GTM to be selected as a trigger for stopping CADC00. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_reg00
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_REG00_CADTEN001  = (0x00000004UL), ///< Output value of GTM timer output selected by PIC2CADTCFG0 bits 24 to 16.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_REG00_CADTEN000  = (0x00000001UL), ///< Output value of GTM timer output selected by PIC2CADTCFG0 bit 8 to 0.
} pic_cadc_stop_trigger_out_ctrl_reg00_t;

/** CADC start trigger output control register 001 enables a trigger source from ENCA to be selected as a trigger for stopping CADC00. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA1_ENCATIEC  = (0x00000020UL), ///< Selects the INTENCA1IEC as a trigger source of CADC00.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA1_ENCATINT1 = (0x00000010UL), ///< Selects the INTENCA1I1 as a trigger source of CADC00.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA1_ENCATINT0 = (0x00000008UL), ///< Selects the INTENCA1I0 as a trigger source of CADC00.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA0_ENCATIEC  = (0x00000004UL), ///< Selects the INTENCA0IEC as a trigger source of CADC00.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA0_ENCATINT1 = (0x00000002UL), ///< Selects the INTENCA0I1 as a trigger source of CADC00.
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ENCA0_ENCATIEC0 = (0x00000001UL), ///< Selects the INTENCA0I0 as a trigger source of CADC00.
} pic_cadc_stop_trigger_out_ctrl_t;

/** CADC start trigger output control register 002 enables a trigger source from ATU timer D 1shot pulse ON occurrence to be selected as a trigger for stopping CADC00. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_on
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D93 = (0x00080000UL), ///< 1shot pulse ON D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D92 = (0x00040000UL), ///< 1shot pulse ON D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D91 = (0x00020000UL), ///< 1shot pulse ON D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D90 = (0x00010000UL), ///< 1shot pulse ON D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D63 = (0x00008000UL), ///< 1shot pulse ON D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D62 = (0x00004000UL), ///< 1shot pulse ON D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D61 = (0x00002000UL), ///< 1shot pulse ON D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D60 = (0x00001000UL), ///< 1shot pulse ON D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D43 = (0x00000800UL), ///< 1shot pulse ON D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D42 = (0x00000400UL), ///< 1shot pulse ON D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D41 = (0x00000200UL), ///< 1shot pulse ON D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D40 = (0x00000100UL), ///< 1shot pulse ON D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D23 = (0x00000080UL), ///< 1shot pulse ON D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D22 = (0x00000040UL), ///< 1shot pulse ON D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D21 = (0x00000020UL), ///< 1shot pulse ON D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D20 = (0x00000010UL), ///< 1shot pulse ON D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D03 = (0x00000008UL), ///< 1shot pulse ON D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D02 = (0x00000004UL), ///< 1shot pulse ON D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D01 = (0x00000002UL), ///< 1shot pulse ON D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_ON_D00 = (0x00000001UL), ///< 1shot pulse ON D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_on_t;

/** CADC stop trigger output control register 003 enables a trigger source from ATU timer D 1shot pulse OFF occurrence to be selected as a trigger for stopping CADC00. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_off
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D93 = (0x00080000UL), ///< 1shot pulse OFF D93 occurrence interrupt (ATU6 timer D subblock 9 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D92 = (0x00040000UL), ///< 1shot pulse OFF D92 occurrence interrupt (ATU6 timer D subblock 9 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D91 = (0x00020000UL), ///< 1shot pulse OFF D91 occurrence interrupt (ATU6 timer D subblock 9 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D90 = (0x00010000UL), ///< 1shot pulse OFF D90 occurrence interrupt (ATU6 timer D subblock 9 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D63 = (0x00008000UL), ///< 1shot pulse OFF D63 occurrence interrupt (ATU6 timer D subblock 6 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D62 = (0x00004000UL), ///< 1shot pulse OFF D62 occurrence interrupt (ATU6 timer D subblock 6 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D61 = (0x00002000UL), ///< 1shot pulse OFF D61 occurrence interrupt (ATU6 timer D subblock 6 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D60 = (0x00001000UL), ///< 1shot pulse OFF D60 occurrence interrupt (ATU6 timer D subblock 6 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D43 = (0x00000800UL), ///< 1shot pulse OFF D43 occurrence interrupt (ATU6 timer D subblock 4 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D42 = (0x00000400UL), ///< 1shot pulse OFF D42 occurrence interrupt (ATU6 timer D subblock 4 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D41 = (0x00000200UL), ///< 1shot pulse OFF D41 occurrence interrupt (ATU6 timer D subblock 4 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D40 = (0x00000100UL), ///< 1shot pulse OFF D40 occurrence interrupt (ATU6 timer D subblock 4 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D23 = (0x00000080UL), ///< 1shot pulse OFF D23 occurrence interrupt (ATU6 timer D subblock 2 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D22 = (0x00000040UL), ///< 1shot pulse OFF D22 occurrence interrupt (ATU6 timer D subblock 2 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D21 = (0x00000020UL), ///< 1shot pulse OFF D21 occurrence interrupt (ATU6 timer D subblock 2 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D20 = (0x00000010UL), ///< 1shot pulse OFF D20 occurrence interrupt (ATU6 timer D subblock 2 channel 0 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D03 = (0x00000008UL), ///< 1shot pulse OFF D03 occurrence interrupt (ATU6 timer D subblock 0 channel 3 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D02 = (0x00000004UL), ///< 1shot pulse OFF D02 occurrence interrupt (ATU6 timer D subblock 0 channel 2 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D01 = (0x00000002UL), ///< 1shot pulse OFF D01 occurrence interrupt (ATU6 timer D subblock 0 channel 1 1shot pulse ON occurence interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_1SHOT_PULSE_OFF_D00 = (0x00000001UL), ///< 1shot pulse OFF D00 occurrence interrupt (ATU6 timer D subblock 0 channel 0 1shot pulse ON occurence interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_off_t;

/** Select triggers from ATU timer C compare match and ATU timer G compare match to be selected as a trigger for stopping CADC. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_c_g_compare_match
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC133   = (int32_t)(0x80000000UL), ///< OCRC133 capture / compare match interrupt (ATU6 timer C subblock 13 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC132   = (int32_t)(0x40000000UL), ///< OCRC132 capture / compare match interrupt (ATU6 timer C subblock 13 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC131   = (int32_t)(0x20000000UL), ///< OCRC131 capture / compare match interrupt (ATU6 timer C subblock 13 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC130   = (int32_t)(0x10000000UL), ///< OCRC130 capture / compare match interrupt (ATU6 timer C subblock 13 channel 0 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC123   = (int32_t)(0x08000000UL), ///< OCRC123 capture / compare match interrupt (ATU6 timer C subblock 12 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC122   = (int32_t)(0x04000000UL), ///< OCRC122 capture / compare match interrupt (ATU6 timer C subblock 12 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC121   = (int32_t)(0x02000000UL), ///< OCRC121 capture / compare match interrupt (ATU6 timer C subblock 12 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC120   = (int32_t)(0x01000000UL), ///< OCRC120 capture / compare match interrupt (ATU6 timer C subblock 12 channel 0 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC113   = (int32_t)(0x00800000UL), ///< OCRC113 capture / compare match interrupt (ATU6 timer C subblock 11 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC112   = (int32_t)(0x00400000UL), ///< OCRC112 capture / compare match interrupt (ATU6 timer C subblock 11 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC111   = (int32_t)(0x00200000UL), ///< OCRC111 capture / compare match interrupt (ATU6 timer C subblock 11 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC110   = (int32_t)(0x00100000UL), ///< OCRC110 capture / compare match interrupt (ATU6 timer C subblock 11 channel 0 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC103   = (int32_t)(0x00080000UL), ///< OCRC103 capture / compare match interrupt (ATU6 timer C subblock 10 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC102   = (int32_t)(0x00040000UL), ///< OCRC102 capture / compare match interrupt (ATU6 timer C subblock 10 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC101   = (int32_t)(0x00020000UL), ///< OCRC101 capture / compare match interrupt (ATU6 timer C subblock 10 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC100   = (int32_t)(0x00010000UL), ///< OCRC100 capture / compare match interrupt (ATU6 timer C subblock 10 channel 0 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG7     = (int32_t)(0x00008000UL), ///< OCRG7 compare match interrupt (ATU6 timer G subblock 7).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG6     = (int32_t)(0x00004000UL), ///< OCRG6 compare match interrupt (ATU6 timer G subblock 6).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG5     = (int32_t)(0x00002000UL), ///< OCRG5 compare match interrupt (ATU6 timer G subblock 5).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG4     = (int32_t)(0x00001000UL), ///< OCRG4 compare match interrupt (ATU6 timer G subblock 4).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG3     = (int32_t)(0x00000800UL), ///< OCRG3 compare match interrupt (ATU6 timer G subblock 3).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG2     = (int32_t)(0x00000400UL), ///< OCRG2 compare match interrupt (ATU6 timer G subblock 2).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG1     = (int32_t)(0x00000200UL), ///< OCRG1 compare match interrupt (ATU6 timer G subblock 1).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRG0     = (int32_t)(0x00000100UL), ///< OCRG0 compare match interrupt (ATU6 timer G subblock 0).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC93    = (int32_t)(0x00000080UL), ///< OCRC93 capture / compare match interrupt (ATU6 timer C subblock 9 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC92    = (int32_t)(0x00000040UL), ///< OCRC92 capture / compare match interrupt (ATU6 timer C subblock 9 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC91    = (int32_t)(0x00000020UL), ///< OCRC91 capture / compare match interrupt (ATU6 timer C subblock 9 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC90    = (int32_t)(0x00000010UL), ///< OCRC90 capture / compare match interrupt (ATU6 timer C subblock 9 channel 0 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC83    = (int32_t)(0x00000008UL), ///< OCRC83 capture / compare match interrupt (ATU6 timer C subblock 8 channel 3 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC82    = (int32_t)(0x00000004UL), ///< OCRC82 capture / compare match interrupt (ATU6 timer C subblock 8 channel 2 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC81    = (int32_t)(0x00000002UL), ///< OCRC81 capture / compare match interrupt (ATU6 timer C subblock 8 channel 1 capture / compare match interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_C_G_COMPARE_MATCH_OCRC80    = (int32_t)(0x00000001UL), ///< OCRC80 capture / compare match interrupt (ATU6 timer C subblock 8 channel 0 capture / compare match interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_c_g_compare_match_t;

/** Select triggers from ATU timer D compare match B to be selected as a trigger for stopping CADC. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_d_compare_match_b
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D113 = (0x00800000UL), ///< OCR2D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D112 = (0x00400000UL), ///< OCR2D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D111 = (0x00200000UL), ///< OCR2D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D110 = (0x00100000UL), ///< OCR2D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D93  = (0x00080000UL), ///< OCR2D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D92  = (0x00040000UL), ///< OCR2D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D91  = (0x00020000UL), ///< OCR2D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D90  = (0x00010000UL), ///< OCR2D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D63  = (0x00008000UL), ///< OCR2D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D62  = (0x00004000UL), ///< OCR2D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D61  = (0x00002000UL), ///< OCR2D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D60  = (0x00001000UL), ///< OCR2D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D43  = (0x00000800UL), ///< OCR2D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D42  = (0x00000400UL), ///< OCR2D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D41  = (0x00000200UL), ///< OCR2D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D40  = (0x00000100UL), ///< OCR2D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D23  = (0x00000080UL), ///< OCR2D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D22  = (0x00000040UL), ///< OCR2D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D21  = (0x00000020UL), ///< OCR2D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D20  = (0x00000010UL), ///< OCR2D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D03  = (0x00000008UL), ///< OCR2D03 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D02  = (0x00000004UL), ///< OCR2D02 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D01  = (0x00000002UL), ///< OCR2D01 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_COMPARE_MATCH_B_OCR2D00  = (0x00000001UL), ///< OCR2D00 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match B interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_d_compare_match_b_t;

/** Select triggers from ATU timer D compare match A and ATU timer G compare match to be selected as a trigger for stopping CADC. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_d_g_compare_match_a
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG13   = (0x20000000UL), ///< OCRG13 compare match interrupt (ATU6 timer G subblock 13).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG12   = (0x10000000UL), ///< OCRG12 compare match interrupt (ATU6 timer G subblock 12).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG11   = (0x08000000UL), ///< OCRG11 compare match interrupt (ATU6 timer G subblock 11).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG10   = (0x04000000UL), ///< OCRG10 compare match interrupt (ATU6 timer G subblock 10).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG9    = (0x02000000UL), ///< OCRG9 compare match interrupt (ATU6 timer G subblock 9).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCRG8    = (0x01000000UL), ///< OCRG8 compare match interrupt (ATU6 timer G subblock 8).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D113 = (0x00800000UL), ///< OCR1D113 compare match interrupt (ATU6 timer D subblock 11 channel 3 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D112 = (0x00400000UL), ///< OCR1D112 compare match interrupt (ATU6 timer D subblock 11 channel 2 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D111 = (0x00200000UL), ///< OCR1D111 compare match interrupt (ATU6 timer D subblock 11 channel 1 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D110 = (0x00100000UL), ///< OCR1D110 compare match interrupt (ATU6 timer D subblock 11 channel 0 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D93  = (0x00080000UL), ///< OCR1D93 compare match interrupt (ATU6 timer D subblock 9 channel 3 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D92  = (0x00040000UL), ///< OCR1D92 compare match interrupt (ATU6 timer D subblock 9 channel 2 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D91  = (0x00020000UL), ///< OCR1D91 compare match interrupt (ATU6 timer D subblock 9 channel 1 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D90  = (0x00010000UL), ///< OCR1D90 compare match interrupt (ATU6 timer D subblock 9 channel 0 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D63  = (0x00008000UL), ///< OCR1D63 compare match interrupt (ATU6 timer D subblock 6 channel 3 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D62  = (0x00004000UL), ///< OCR1D62 compare match interrupt (ATU6 timer D subblock 6 channel 2 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D61  = (0x00002000UL), ///< OCR1D61 compare match interrupt (ATU6 timer D subblock 6 channel 1 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D60  = (0x00001000UL), ///< OCR1D60 compare match interrupt (ATU6 timer D subblock 6 channel 0 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D43  = (0x00000800UL), ///< OCR1D43 compare match interrupt (ATU6 timer D subblock 4 channel 3 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D42  = (0x00000400UL), ///< OCR1D42 compare match interrupt (ATU6 timer D subblock 4 channel 2 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D41  = (0x00000200UL), ///< OCR1D41 compare match interrupt (ATU6 timer D subblock 4 channel 1 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D40  = (0x00000100UL), ///< OCR1D40 compare match interrupt (ATU6 timer D subblock 4 channel 0 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D23  = (0x00000080UL), ///< OCR1D23 compare match interrupt (ATU6 timer D subblock 2 channel 3 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D22  = (0x00000040UL), ///< OCR1D22 compare match interrupt (ATU6 timer D subblock 2 channel 2 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D21  = (0x00000020UL), ///< OCR1D21 compare match interrupt (ATU6 timer D subblock 2 channel 1 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D20  = (0x00000010UL), ///< OCR1D20 compare match interrupt (ATU6 timer D subblock 2 channel 0 compare match A interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D03  = (0x00000008UL), ///< OCR1D03 compare match interrupt (ATU6 timer D subblock 0 channel 3 compare match B interrup).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D02  = (0x00000004UL), ///< OCR1D02 compare match interrupt (ATU6 timer D subblock 0 channel 2 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D01  = (0x00000002UL), ///< OCR1D01 compare match interrupt (ATU6 timer D subblock 0 channel 1 compare match B interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_G_COMPARE_MATCH_A_OCR1D00  = (0x00000001UL), ///< OCR1D00 compare match interrupt (ATU6 timer D subblock 0 channel 0 compare match B interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_d_g_compare_match_a_t;

/** Select triggers from ATU timer D down-counter underflow for stopping CADC00. */
typedef enum e_pic_cadc_stop_trigger_out_ctrl_atu_d_down_counter_underflow
{
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID93 = (0x00080000UL), ///< UDID93 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 3 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID92 = (0x00040000UL), ///< UDID92 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 2 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID91 = (0x00020000UL), ///< UDID91 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 1 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID90 = (0x00010000UL), ///< UDID90 down-counter underflow interrupt (ATU6 timer D subblock 9 channel 0 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID63 = (0x00008000UL), ///< UDID63 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 3 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID62 = (0x00004000UL), ///< UDID62 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 2 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID61 = (0x00002000UL), ///< UDID61 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 1 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID60 = (0x00001000UL), ///< UDID60 down-counter underflow interrupt (ATU6 timer D subblock 6 channel 0 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID43 = (0x00000800UL), ///< UDID43 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 3 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID42 = (0x00000400UL), ///< UDID42 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 2 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID41 = (0x00000200UL), ///< UDID41 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 1 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID40 = (0x00000100UL), ///< UDID40 down-counter underflow interrupt (ATU6 timer D subblock 4 channel 0 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID23 = (0x00000080UL), ///< UDID23 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 3 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID22 = (0x00000040UL), ///< UDID22 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 2 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID21 = (0x00000020UL), ///< UDID21 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 1 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID20 = (0x00000010UL), ///< UDID20 down-counter underflow interrupt (ATU6 timer D subblock 2 channel 0 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID03 = (0x00000008UL), ///< UDID03 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 3 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID02 = (0x00000004UL), ///< UDID02 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 2 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID01 = (0x00000002UL), ///< UDID01 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 1 down-counter underflow interrupt).
    PIC_CADC_STOP_TRIGGER_OUT_CTRL_ATU_D_DOWN_COUNTER_UNDERFLOW_UDID00 = (0x00000001UL), ///< UDID00 down-counter underflow interrupt (ATU6 timer D subblock 0 channel 0 down-counter underflow interrupt).
} pic_cadc_stop_trigger_out_ctrl_atu_d_down_counter_underflow_t;

/** Selects an effective edge of ADCKn SG-Diag. */
typedef enum e_pic_adc_trigger_edge_sel_sg_diag
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG_DIAG_RISING  = (uint16_t)(0x0U << 10), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG_DIAG_FALLING = (uint16_t)(0x1U << 10), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG_DIAG_BOTH    = (uint16_t)(0x2U << 10), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg_diag_t;

/** Selects an effective edge of ADCKn scan group 4. */
typedef enum e_pic_adc_trigger_edge_sel_sg4
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG4_RISING  = (uint16_t)(0x0U << 8), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG4_FALLING = (uint16_t)(0x1U << 8), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG4_BOTH    = (uint16_t)(0x2U << 8), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg4_t;

/** Selects an effective edge of ADCKn scan group 3. */
typedef enum e_pic_adc_trigger_edge_sel_sg3
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG3_RISING  = (uint16_t)(0x0U << 6), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG3_FALLING = (uint16_t)(0x1U << 6), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG3_BOTH    = (uint16_t)(0x2U << 6), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg3_t;

/** Selects an effective edge of ADCKn scan group 2. */
typedef enum e_pic_adc_trigger_edge_sel_sg2
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG2_RISING  = (uint16_t)(0x0U << 4), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG2_FALLING = (uint16_t)(0x1U << 4), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG2_BOTH    = (uint16_t)(0x2U << 4), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg2_t;

/** Selects an effective edge of ADCKn scan group 1. */
typedef enum e_pic_adc_trigger_edge_sel_sg1
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG1_RISING  = (uint16_t)(0x0U << 2), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG1_FALLING = (uint16_t)(0x1U << 2), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG1_BOTH    = (uint16_t)(0x2U << 2), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg1_t;

/** Selects an effective edge of ADCKn scan group 0. */
typedef enum e_pic_adc_trigger_edge_sel_sg0
{
    PIC_ADC_TRIGGER_EDGE_SEL_SG0_RISING  = (uint16_t)(0x0U << 0), ///< Rising edge is selected.
    PIC_ADC_TRIGGER_EDGE_SEL_SG0_FALLING = (uint16_t)(0x1U << 0), ///< Falling edge is selected
    PIC_ADC_TRIGGER_EDGE_SEL_SG0_BOTH    = (uint16_t)(0x2U << 0), ///< Both edges are selected.
} pic_adc_trigger_edge_sel_sg0_t;

/** Selects switches on/off of the function to output low/high level. */
typedef enum e_pic_switch_tsg_output_mode
{
    PIC_SWITCH_TSG_OUTPUT_MODE_OFF = (uint8_t)(0x00U), ///< Only TSG3 output is available.
    PIC_SWITCH_TSG_OUTPUT_MODE_ON  = (uint8_t)(0x01U), ///< TSG3 output and low/high level output can be switched
} pic_switch_mode_t;

/** Select the output signal from either TSG30 output or low/high level output. */
typedef enum e_switch_tsg_output_signal
{
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_TSG3             = (uint8_t)(0x00U), ///< Output signal of TSG3.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_1_LOW_HIGH_LEVEL = (uint8_t)(0x02U), ///< Output 1 low/high level signal.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_2_LOW_HIGH_LEVEL = (uint8_t)(0x04U), ///< Output 2 low/high level signal.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_3_LOW_HIGH_LEVEL = (uint8_t)(0x08U), ///< Output 3 low/high level signal.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_4_LOW_HIGH_LEVEL = (uint8_t)(0x10U), ///< Output 4 low/high level signal.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_5_LOW_HIGH_LEVEL = (uint8_t)(0x20U), ///< Output 5 low/high level signal.
    PIC_SWITCH_TSG_OUTPUT_SIGNAL_6_LOW_HIGH_LEVEL = (uint8_t)(0x40U), ///< Output 6 low/high level signal.
} pic_switch_tsg_output_signal_t;

/** Select low/high level output of the TSG30 output. */
typedef enum e_pic_tsg_output_level
{
    PIC_TSG_OUTPUT_LEVEL_LOW           = 0U,                ///< Output low level signal.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_1 = (uint16_t)(0x02U), ///< Output signal 1 high level.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_2 = (uint16_t)(0x04U), ///< Output signal 2 high level.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_3 = (uint16_t)(0x08U), ///< Output signal 3 high level.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_4 = (uint16_t)(0x10U), ///< Output signal 4 high level.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_5 = (uint16_t)(0x20U), ///< Output signal 5 high level.
    PIC_TSG_OUTPUT_LEVEL_HIGH_SIGNAL_6 = (uint16_t)(0x40U), ///< Output signal 6 high level.
} pic_tsg_output_level_t;

/** Selects the INTTSG30I4 signal of TSG30 as the input for EMU3S0 compare match interrupt 4.*/
typedef enum e_pic_emu_input_compare_match_interrupt_4
{
    PIC_EMU_INPUT_COMPARE_MATCH_INTERRUPT_4_INTTSG3I4 = 0U,                  ///< Select INTTSG3I4 signal of TSG3 as input.
    PIC_EMU_INPUT_COMPARE_MATCH_INTERRUPT_4_NONE      = ((uint16_t)(0x20U)), ///< No input.
} pic_emu_input_compare_match_interrupt_4_t;

/** Selects the INTTSG30I3 signal of TSG30 as the input for EMU3S0 compare match interrupt 3.*/
typedef enum e_pic_emu_input_compare_match_interrupt_3
{
    PIC_EMU_INPUT_COMPARE_MATCH_INTERRUPT_3_INTTSG3I3 = 0U,                  ///< Select INTTSG3I3 signal of TSG3 as input.
    PIC_EMU_INPUT_COMPARE_MATCH_INTERRUPT_3_NONE      = ((uint16_t)(0x10U)), ///< No input.
} pic_emu_input_compare_match_interrupt_3_t;

/** Selects the TSG30ADTRG1 signal of TSG30 as the input for EMU3S0 A/D conversion trigger signals 1. */
typedef enum e_pic_emu_input_adc_trigger_signal_1
{
    PIC_EMU_INPUT_ADC_TRIGGER_SIGNAL_1_TSG3ADTRG1 = 0U,                  ///< Select TSG30ADTRG1 signal of TSG3 as input.
    PIC_EMU_INPUT_ADC_TRIGGER_SIGNAL_1_NONE       = ((uint16_t)(0x08U)), ///< No input.
} pic_emu_input_adc_trigger_signal_1_t;

/** Selects the TSG30ADTRG0 signal of TSG30 as the input for EMU3S0 A/D conversion trigger signals 0.*/
typedef enum e_pic_emu_input_adc_trigger_signal_0
{
    PIC_EMU_INPUT_ADC_TRIGGER_SIGNAL_0_TSG3ADTRG0 = 0U,                  ///< Select TSG30ADTRG0 signal of TSG3 as input.
    PIC_EMU_INPUT_ADC_TRIGGER_SIGNAL_0_NONE       = ((uint16_t)(0x04U)), ///< No input.
} pic_emu_input_adc_trigger_signal_0_t;

/** Selects output value of EMU3S0 Trough Interrupt selected by PIC2EMUISEN0 or TSTVLYTIM signal of TSG30 as the input for EMU3S0 Trough Interrupt.*/
typedef enum e_pic_emu_input_trough_interrupt
{
    PIC_EMU_INPUT_TROUGH_INTERRUPT_TSTVLYTIM = 0U,                  ///< Select TSTVLYTIM signal of TSG3 as input.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM  = ((uint16_t)(0x02U)), ///< Select GTM ATOM as input.
} pic_emu_input_trough_interrupt_t;

/** Selects output value of EMU3S0 Peak Interrupt selected by PIC2EMUISEN0 or TSTPEKTIM signal of TSG30 as the input for EMU3S0 Peak Interrupt.*/
typedef enum e_pic_emu_input_peak_interrupt
{
    PIC_EMU_INPUT_PEAK_INTERRUPT_TSTPEKTIM = 0U,                  ///< Select TSTPEKTIM signal of TSG3 as input.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM  = ((uint16_t)(0x01U)), ///< Select GTM ATOM as input.
} pic_emu_input_peak_interrupt_t;

/** Selects the GTM output as a signal source for EMU3S Trough Interrupt. */
typedef enum e_pic_emu_input_peak_interrupt_gtm_atom
{
    // Outputs: GTM_ATOM[i]_OUT[x]^1 0..31
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT0 = 0U,  ///< Select GTM ATOM0 output 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT1 = 1U,  ///< Select GTM ATOM0 output 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT2 = 2U,  ///< Select GTM ATOM0 output 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT3 = 3U,  ///< Select GTM ATOM0 output 3 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT4 = 4U,  ///< Select GTM ATOM0 output 4 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT5 = 5U,  ///< Select GTM ATOM0 output 5 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT6 = 6U,  ///< Select GTM ATOM0 output 6 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_OUT7 = 7U,  ///< Select GTM ATOM0 output 7 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT0 = 8U,  ///< Select GTM ATOM1 output 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT1 = 9U,  ///< Select GTM ATOM1 output 1 for peak interrupt.`
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT2 = 10U, ///< Select GTM ATOM1 output 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT3 = 11U, ///< Select GTM ATOM1 output 3 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT4 = 12U, ///< Select GTM ATOM1 output 4 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT5 = 13U, ///< Select GTM ATOM1 output 5 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT6 = 14U, ///< Select GTM ATOM1 output 6 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_OUT7 = 15U, ///< Select GTM ATOM1 output 7 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT0 = 16U, ///< Select GTM ATOM2 output 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT1 = 17U, ///< Select GTM ATOM2 output 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT2 = 18U, ///< Select GTM ATOM2 output 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT3 = 19U, ///< Select GTM ATOM2 output 3 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT4 = 20U, ///< Select GTM ATOM2 output 4 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT5 = 21U, ///< Select GTM ATOM2 output 5 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT6 = 22U, ///< Select GTM ATOM2 output 6 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_OUT7 = 23U, ///< Select GTM ATOM2 output 7 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT0 = 24U, ///< Select GTM ATOM3 output 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT1 = 25U, ///< Select GTM ATOM3 output 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT2 = 26U, ///< Select GTM ATOM3 output 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT3 = 27U, ///< Select GTM ATOM3 output 3 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT4 = 28U, ///< Select GTM ATOM3 output 4 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT5 = 29U, ///< Select GTM ATOM3 output 5 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT6 = 30U, ///< Select GTM ATOM3 output 6 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_OUT7 = 31U, ///< Select GTM ATOM3 output 7 for peak interrupt.

    // IRQ outputs: GTM_ATOM[i]_IRQ[x]^2 80..95
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_IRQ0 = 80U, ///< Select GTM ATOM0 IRQ 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_IRQ1 = 81U, ///< Select GTM ATOM0 IRQ 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_IRQ2 = 82U, ///< Select GTM ATOM0 IRQ 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_0_IRQ3 = 83U, ///< Select GTM ATOM0 IRQ 3 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_IRQ0 = 84U, ///< Select GTM ATOM1 IRQ 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_IRQ1 = 85U, ///< Select GTM ATOM1 IRQ 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_IRQ2 = 86U, ///< Select GTM ATOM1 IRQ 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_1_IRQ3 = 87U, ///< Select GTM ATOM1 IRQ 3 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_IRQ0 = 88U, ///< Select GTM ATOM2 IRQ 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_IRQ1 = 89U, ///< Select GTM ATOM2 IRQ 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_IRQ2 = 90U, ///< Select GTM ATOM2 IRQ 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_2_IRQ3 = 91U, ///< Select GTM ATOM2 IRQ 3 for peak interrupt.

    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_IRQ0 = 92U, ///< Select GTM ATOM3 IRQ 0 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_IRQ1 = 93U, ///< Select GTM ATOM3 IRQ 1 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_IRQ2 = 94U, ///< Select GTM ATOM3 IRQ 2 for peak interrupt.
    PIC_EMU_INPUT_PEAK_INTERRUPT_GTM_ATOM_3_IRQ3 = 95U, ///< Select GTM ATOM3 IRQ 3 for peak interrupt.
} pic_emu_input_peak_interrupt_gtm_atom_t;

/** Selects the GTM output as a signal source for EMU3S Peak Interrupt. */
typedef enum e_pic_emu_input_trough_interrupt_gtm_atom
{
    // Inverted outputs: GTM_ATOM[i]_OUT[x]^1 0..31
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT0_INV = 0U,  ///< Select GTM ATOM0 output 0 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT1_INV = 1U,  ///< Select GTM ATOM0 output 1 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT2_INV = 2U,  ///< Select GTM ATOM0 output 2 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT3_INV = 3U,  ///< Select GTM ATOM0 output 3 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT4_INV = 4U,  ///< Select GTM ATOM0 output 4 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT5_INV = 5U,  ///< Select GTM ATOM0 output 5 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT6_INV = 6U,  ///< Select GTM ATOM0 output 6 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_OUT7_INV = 7U,  ///< Select GTM ATOM0 output 7 (inverted) for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT0_INV = 8U,  ///< Select GTM ATOM1 output 0 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT1_INV = 9U,  ///< Select GTM ATOM1 output 1 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT2_INV = 10U, ///< Select GTM ATOM1 output 2 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT3_INV = 11U, ///< Select GTM ATOM1 output 3 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT4_INV = 12U, ///< Select GTM ATOM1 output 4 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT5_INV = 13U, ///< Select GTM ATOM1 output 5 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT6_INV = 14U, ///< Select GTM ATOM1 output 6 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_OUT7_INV = 15U, ///< Select GTM ATOM1 output 7 (inverted) for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT0_INV = 16U, ///< Select GTM ATOM2 output 0 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT1_INV = 17U, ///< Select GTM ATOM2 output 1 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT2_INV = 18U, ///< Select GTM ATOM2 output 2 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT3_INV = 19U, ///< Select GTM ATOM2 output 3 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT4_INV = 20U, ///< Select GTM ATOM2 output 4 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT5_INV = 21U, ///< Select GTM ATOM2 output 5 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT6_INV = 22U, ///< Select GTM ATOM2 output 6 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_OUT7_INV = 23U, ///< Select GTM ATOM2 output 7 (inverted) for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT0_INV = 24U, ///< Select GTM ATOM3 output 0 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT1_INV = 25U, ///< Select GTM ATOM3 output 1 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT2_INV = 26U, ///< Select GTM ATOM3 output 2 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT3_INV = 27U, ///< Select GTM ATOM3 output 3 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT4_INV = 28U, ///< Select GTM ATOM3 output 4 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT5_INV = 29U, ///< Select GTM ATOM3 output 5 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT6_INV = 30U, ///< Select GTM ATOM3 output 6 (inverted) for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_OUT7_INV = 31U, ///< Select GTM ATOM3 output 7 (inverted) for trough interrupt.

    // IRQ outputs: GTM_ATOM[i]_IRQ[x]^2 80..95
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_IRQ0 = 80U,     ///< Select GTM ATOM0 IRQ 0 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_IRQ1 = 81U,     ///< Select GTM ATOM0 IRQ 1 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_IRQ2 = 82U,     ///< Select GTM ATOM0 IRQ 2 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_0_IRQ3 = 83U,     ///< Select GTM ATOM0 IRQ 3 for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_IRQ0 = 84U,     ///< Select GTM ATOM1 IRQ 0 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_IRQ1 = 85U,     ///< Select GTM ATOM1 IRQ 1 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_IRQ2 = 86U,     ///< Select GTM ATOM1 IRQ 2 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_1_IRQ3 = 87U,     ///< Select GTM ATOM1 IRQ 3 for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_IRQ0 = 88U,     ///< Select GTM ATOM2 IRQ 0 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_IRQ1 = 89U,     ///< Select GTM ATOM2 IRQ 1 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_IRQ2 = 90U,     ///< Select GTM ATOM2 IRQ 2 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_2_IRQ3 = 91U,     ///< Select GTM ATOM2 IRQ 3 for trough interrupt.

    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_IRQ0 = 92U,     ///< Select GTM ATOM3 IRQ 0 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_IRQ1 = 93U,     ///< Select GTM ATOM3 IRQ 1 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_IRQ2 = 94U,     ///< Select GTM ATOM3 IRQ 2 for trough interrupt.
    PIC_EMU_INPUT_TROUGH_INTERRUPT_GTM_ATOM_3_IRQ3 = 95U,     ///< Select GTM ATOM3 IRQ 3 for trough interrupt.e
} pic_emu_input_trough_interrupt_gtm_atom_t;

/** Enable or disable the output of simultaneouse start triggers from PIC1 */
typedef enum e_pic_simultaneous_start_trigger
{
    PIC_SIMULTANEOUS_START_TRIGGER_DISABLE = (uint8_t)(0x00U), ///< Disables the output of simultaneous start triggers from PIC1.
    PIC_SIMULTANEOUS_START_TRIGGER_ENABLE  = (uint8_t)(0x01U), ///< Enables the output of simultaneous start triggers from PIC1.
} pic_simultaneous_start_trigger_t;

/** Enable a start trigger for each channel of TAUD. */
typedef enum e_pic_start_trigger_taud
{
    PIC_START_TRIGGER_TAUD_CHANNEL00 = (uint16_t)(0x0001U), ///< Enables trigger for CH0 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL01 = (uint16_t)(0x0002U), ///< Enables trigger for CH1 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL02 = (uint16_t)(0x0004U), ///< Enables trigger for CH2 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL03 = (uint16_t)(0x0008U), ///< Enables trigger for CH3 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL04 = (uint16_t)(0x0010U), ///< Enables trigger for CH4 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL05 = (uint16_t)(0x0020U), ///< Enables trigger for CH5 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL06 = (uint16_t)(0x0040U), ///< Enables trigger for CH6 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL07 = (uint16_t)(0x0080U), ///< Enables trigger for CH7 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL08 = (uint16_t)(0x0100U), ///< Enables trigger for CH8 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL09 = (uint16_t)(0x0200U), ///< Enables trigger for CH9 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL10 = (uint16_t)(0x0400U), ///< Enables trigger for CH10 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL11 = (uint16_t)(0x0800U), ///< Enables trigger for CH11 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL12 = (uint16_t)(0x1000U), ///< Enables trigger for CH12 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL13 = (uint16_t)(0x2000U), ///< Enables trigger for CH13 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL14 = (uint16_t)(0x4000U), ///< Enables trigger for CH14 of TAUD.
    PIC_START_TRIGGER_TAUD_CHANNEL15 = (uint16_t)(0x8000U), ///< Enables trigger for CH15 of TAUD.
} pic_start_trigger_taud_t;

/** Enable a start trigger for each channel of TAUJn, TSG3n, TPBAn, and ENCAn. */
typedef enum e_pic_start_trigger_tauj_tsg_tpba_enca
{
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_GTM     = (uint16_t)(0x4000U), ///< Enables simultaneous start trigger for the GTM.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_ENCA1   = (uint16_t)(0x2000U), ///< Enables simultaneous start trigger for the ENCA1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_ENCA0   = (uint16_t)(0x1000U), ///< Enables simultaneous start trigger for the ENCA0.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TPBA0   = (uint16_t)(0x0800U), ///< Enables simultaneous start trigger for the TPBA0.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TPBA1   = (uint16_t)(0x0400U), ///< Enables simultaneous start trigger for the TPBA1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TSG31   = (uint16_t)(0x0200U), ///< Enables simultaneous start trigger for the TSG31.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TSG30   = (uint16_t)(0x0100U), ///< Enables simultaneous start trigger for the TSG30.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ1_3 = (uint16_t)(0x0080U), ///< Enables simultaneous start trigger for CH03 of TAUJ1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ1_2 = (uint16_t)(0x0040U), ///< Enables simultaneous start trigger for CH02 of TAUJ1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ1_1 = (uint16_t)(0x0020U), ///< Enables simultaneous start trigger for CH01 of TAUJ1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ1_0 = (uint16_t)(0x0010U), ///< Enables simultaneous start trigger for CH00 of TAUJ1.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ0_3 = (uint16_t)(0x0008U), ///< Enables simultaneous start trigger for CH03 of TAUJ0.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ0_2 = (uint16_t)(0x0004U), ///< Enables simultaneous start trigger for CH02 of TAUJ0.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ0_1 = (uint16_t)(0x0002U), ///< Enables simultaneous start trigger for CH01 of TAUJ0.
    PIC_START_TRIGGER_TAUJ_TSG_TPBA_ENCA_TAUJ0_0 = (uint16_t)(0x0001U), ///< Enables simultaneous start trigger for CH00 of TAUJ0.
} pic_start_trigger_tsg_tpba_enca_t;

/** Enable a start trigger for each channel of TSG32. */
typedef enum e_pic_start_trigger_tsg32
{
    PIC_START_TRIGGER_TSG32 = (uint16_t)(0x0100U), ///< Enables simultaneous start trigger for the TSG32.
} pic_start_trigger_tsg32_t;

/** Enables a start trigger for each channel of OSTMn. */
typedef enum e_pic12_start_trigger_ostm
{
    PIC12_START_TRIGGER_OSTM_0 = (uint16_t)(0x001U),  ///< Enables simultaneous start trigger for the OSTM0.
    PIC12_START_TRIGGER_OSTM_1 = (uint16_t)(0x002U),  ///< Enables simultaneous start trigger for the OSTM1.
    PIC12_START_TRIGGER_OSTM_2 = (uint16_t)(0x004U),  ///< Enables simultaneous start trigger for the OSTM2.
    PIC12_START_TRIGGER_OSTM_3 = (uint16_t)(0x008U),  ///< Enables simultaneous start trigger for the OSTM3.
    PIC12_START_TRIGGER_OSTM_4 = (uint16_t)(0x0010U), ///< Enables simultaneous start trigger for the OSTM4.
    PIC12_START_TRIGGER_OSTM_5 = (uint16_t)(0x0020U), ///< Enables simultaneous start trigger for the OSTM5.
    PIC12_START_TRIGGER_OSTM_6 = (uint16_t)(0x0040U), ///< Enables simultaneous start trigger for the OSTM6.
} pic12_start_trigger_ostm_t;

#if (BSP_FEATURE_PIC_FEATURE_U2CX)

/** Enables a start trigger for each channel of OSTMn (n = 0, 1). */
typedef enum e_pic10_start_trigger_ostm
{
    PIC10_START_TRIGGER_OSTM_0 = (uint16_t)(0x01U), ///< Enables simultaneous start trigger for the OSTM8.
    PIC10_START_TRIGGER_OSTM_1 = (uint16_t)(0x02U), ///< Enables simultaneous start trigger for the OSTM9.
} pic10_start_trigger_ostm_t;
#elif (BSP_FEATURE_PIC_FEATURE_U2AX)

/** Enables a start trigger for each channel of OSTMn (n = 8, 9). */
typedef enum e_pic10_start_trigger_ostm
{
    PIC10_START_TRIGGER_OSTM_8 = (uint16_t)(0x01U), ///< Enables simultaneous start trigger for the OSTM8.
    PIC10_START_TRIGGER_OSTM_9 = (uint16_t)(0x02U), ///< Enables simultaneous start trigger for the OSTM9.
} pic10_start_trigger_ostm_t;
#endif

/** Enable output of simultaneous start triggers from one PIC1 unit to the other. */
typedef enum e_pic_start_trigger_output
{
    PIC_START_TRIGGER_OUTPUT_NONE  = (0x00U), ///< No start trigger.
    PIC_START_TRIGGER_OUTPUT_PIC10 = (0x01U), ///< PIC10 start trigger.
    PIC_START_TRIGGER_OUTPUT_PIC11 = (0x02U), ///< PIC11 start trigger.
} pic_start_trigger_output_t;

/** Selects a start trigger for TSG3 */
typedef enum e_pic_tsg_start_trigger_sel
{
    PIC_TSG_START_TRIGGER_SEL_PIC1SYNCTRG = (0x0U), ///< Select PIC1SYNCTRG.
    PIC_TSG_START_TRIGGER_SEL_TSGTSST     = (0x1U), ///< Select an external trigger (TSGTSST).
} pic_tsg_start_trigger_sel_t;

/** Selects TAUD0 channels 14 and 15 for synchronous starting or clearing of TSG30 and TSG31.  */
typedef enum e_pic_tsg_sync_start_clear_sel
{
    PIC_TSG_SYNC_START_CLEAR_SEL_DISABLE                       = (0x00U), ///< TSG30: Disabled TSG31: Disabled
    PIC_TSG_SYNC_START_CLEAR_SEL_TSG30_TAUD0I14                = (0x01U), ///< TSG30: INTTAUD0I14 enabled TSG31: Disabled
    PIC_TSG_SYNC_START_CLEAR_SEL_TSG31_TAUD0I15                = (0x02U), ///< TSG30: Disabled  TSG31: INTTAUD0I15 enabled
    PIC_TSG_SYNC_START_CLEAR_SEL_TSG30_TAUD0I14_TSG31_TAUD0I15 = (0x03U), ///< TSG30: INTTAUD0I14 enabled TSG31: INTTAUD0I15 enabled
    PIC_TSG_SYNC_START_CLEAR_SEL_BOTH_TAUD0I15                 = (0x04U), ///< TSG30: INTTAUD0I15 enabled TSG31: INTTAUD0I15 enabled
} pic_tsg_sync_start_clear_sel_t;

/** Selects Hi-Z control input signals for TAUD, TSG3. */
typedef enum e_pic_hiz_control_input
{
    PIC_HIZ_CONTROL_INPUT_INTADC1ERR              = (0x80U), ///< Enables Hi-Z control by ADCK1/ADCJ1 error interrupt.
    PIC_HIZ_CONTROL_INPUT_INTADC0ERR              = (0x40U), ///< Enables Hi-Z control by ADCK0/ADCJ0 error interrupt.
    PIC_HIZ_CONTROL_INPUT_ERROROUTZ               = (0x20U), ///< Enables Hi-Z control by ERROROUTZ signal.
    PIC_HIZ_CONTROL_INPUT_INTTSG31IER             = (0x10U), ///< Enables Hi-Z control by INTTSG31IER interrupt signal.
    PIC_HIZ_CONTROL_INPUT_INTTSG30IER_INTTSG32IER = (0x08U), ///< Enables Hi-Z control by INTTSG30IER/INTTSG32IER interrupt signal.
    PIC_HIZ_CONTROL_INPUT_WDTBTNMI                = (0x04U), // < Enables Hi-Z control by WDTBn (WDTBTNMI).
    PIC_HIZ_CONTROL_INPUT_FCMPCMPO                = (0x02U), ///< Enables Hi-Z control by FCMPn comparator output data(FCMPnCMPO).
    PIC_HIZ_CONTROL_INPUT_TAPAESO                 = (0x01U), ///< Enables Hi-Z control by TAPA5nSO pin input.
} pic_hiz_control_input_t;

/** Selects the signal to be input as the TSG3nOPCI0 signal of the TSG3n timer */
typedef enum e_pic_tsg3opci0_input_signal
{
    PIC_TSG3OPCI0_INPUT_SIGNAL_NONE       = (0x00),  ///< No signal
    PIC_TSG3OPCI0_INPUT_SIGNAL_INTENCA0I1 = (0x01U), ///< Select INTENCA0I1
    PIC_TSG3OPCI0_INPUT_SIGNAL_INTENCA1I1 = (0x02U), ///< Select INTENCA1I1
} pic_tsg3opci0_input_signal_t;

/** Selects INTTAUDnIm, ENCA0I1, ENCA1I1 and ADCKTTOUTn[j]/ADCJTTOUTn[j] to be used as a capture trigger signal for ENCA */
typedef enum e_pic_enc_cap_trig
{
    PIC_ENC_CAP_TRIG_ENCA0I1_INTTAUD0I = (0x00U), ///< Select trigger from ENCA0I1 or INTAUD0Im.
    PIC_ENC_CAP_TRIG_ENCA1I1_INTTAUD1I = (0x01U), ///< Select trigger from ENCA1I1 or INTAUD1Im.
    PIC_ENC_CAP_TRIG_ADCTTOUT04        = (0x02U), ///< Select trigger from ADCKTTOUT0[4]/ADCKTTOUT0[4]
    PIC_ENC_CAP_TRIG_ADCTTOUT03        = (0x03U), ///< Select trigger from ADCKTTOUT0[3]/ADCKTTOUT0[3]
    PIC_ENC_CAP_TRIG_ADCTTOUT02        = (0x04U), ///< Select trigger from ADCKTTOUT0[2]/ADCKTTOUT0[2]
    PIC_ENC_CAP_TRIG_ADCTTOUT01        = (0x05U), ///< Select trigger from ADCKTTOUT0[1]/ADCKTTOUT0[1]
    PIC_ENC_CAP_TRIG_ADCTTOUT00        = (0x06U), ///< Select trigger from ADCKTTOUT0[0]/ADCKTTOUT0[0]
    PIC_ENC_CAP_TRIG_ADCTTOUT14        = (0x07U), ///< Select trigger from ADCKTTOUT1[4]/ADCKTTOUT1[4]
    PIC_ENC_CAP_TRIG_ADCTTOUT13        = (0x08U), ///< Select trigger from ADCKTTOUT1[3]/ADCKTTOUT1[3]
    PIC_ENC_CAP_TRIG_ADCTTOUT12        = (0x09U), ///< Select trigger from ADCKTTOUT1[2]/ADCKTTOUT1[2]
    PIC_ENC_CAP_TRIG_ADCTTOUT11        = (0x0AU), ///< Select trigger from ADCKTTOUT1[1]/ADCKTTOUT1[1]
    PIC_ENC_CAP_TRIG_ADCTTOUT10        = (0x0BU), ///< Select trigger from ADCKTTOUT1[0]/ADCKTTOUT1[0]
} pic_enc_cap_trig_t;

/** Selects ENCAnI1, INTTAUDnIm to be used as a capture trigger signal for ENCA */
typedef enum e_pic_enc_cap_trig_encai_inttaudi
{
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_ENCAI1     = (0x00U), ///< Select trigger from ENCAnI1.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI0  = (0x10U), ///< Select trigger from INTTAUDnI0.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI1  = (0x11U), ///< Select trigger from INTTAUDnI1.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI2  = (0x12U), ///< Select trigger from INTTAUDnI2.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI3  = (0x13U), ///< Select trigger from INTTAUDnI3.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI4  = (0x14U), ///< Select trigger from INTTAUDnI4.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI5  = (0x15U), ///< Select trigger from INTTAUDnI5.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI6  = (0x16U), ///< Select trigger from INTTAUDnI6.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI7  = (0x17U), ///< Select trigger from INTTAUDnI7.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI8  = (0x18U), ///< Select trigger from INTTAUDnI8.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI9  = (0x19U), ///< Select trigger from INTTAUDnI9.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI10 = (0x1AU), ///< Select trigger from INTTAUDnI10.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI11 = (0x1BU), ///< Select trigger from INTTAUDnI11.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI12 = (0x1CU), ///< Select trigger from INTTAUDnI12.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI13 = (0x1DU), ///< Select trigger from INTTAUDnI13.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI14 = (0x1EU), ///< Select trigger from INTTAUDnI14.
    PIC_ENC_CAP_TRIG_ENCAI_INTTAUDI_INTTAUDI15 = (0x1FU), ///< Select trigger from INTTAUDnI15.
} pic_enc_cap_trig_encai_inttaudi_t;

/** Select the TAUDn channel used by TAPAnTSIM0 and TAPAnTUDCM0. */
typedef enum e_pic_taud_int_signal_output
{
    PIC_TAUD_INT_SIGNAL_OUTPUT_NOT_SELECTED = (0x00U), ///< Not selected.
    PIC_TAUD_INT_SIGNAL_OUTPUT_CHANNEL_0    = (0x01U), ///< TAUD channel 0 selected.
    PIC_TAUD_INT_SIGNAL_OUTPUT_CHANNEL_2    = (0x02U), ///< TAUD channel 2 selected.
    PIC_TAUD_INT_SIGNAL_OUTPUT_CHANNEL_8    = (0x03U), ///< TAUD channel 8 selected.
} pic_taud_int_signal_output_t;

/** Encoder control function*/
typedef enum e_pic_encoder_ctrl_func
{
    PIC_ENCODER_CTRL_FUNC_NONE = 0,             ///< Not selected function
    PIC_ENCODER_CTRL_FUNC_TWO_PHASE_METHOD_1_2, ///< Two-phase encoder control function (control method 1/2)
    PIC_ENCODER_CTRL_FUNC_TW0_PHASE_METHOD_3,   ///< Two-phase encoder control function (control method 3)
    PIC_ENCODER_CTRL_FUNC_THREE_PHASE_ENCODER,  ///< Three-phase encoder control function
    PIC_ENCODER_CTRL_FUNC_THREE_PHASE_PULSE,    // < Three-phase pulse input control function
} pic_encoder_ctrl_func_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD0 channel 0. */
typedef enum e_pic_trigger_pulse_width_measure_taud0_ch0
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH0_NONE    = ((uint32_t) 0x00U),  ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH0_EQ0_U00 = ((uint32_t) 0x400U), ///< Output signal of ENCAT0EQ0 or INTTPTMU00
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH0_EQ1_U01 = ((uint32_t) 0x40U),  ///< Output signal of ENCAT0EQ1 or INTTPTMU01
} pic_trigger_pulse_width_measure_taud0_ch0_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD0 channel 1. */
typedef enum e_pic_trigger_pulse_width_measure_taud0_ch1
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH1_NONE    = ((uint32_t) 0x00U),  ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH1_EQ1_U01 = ((uint32_t) 0x840U), ///< Output signal of ENCAT0EQ1 or INTTPTMU01
} pic_trigger_pulse_width_measure_taud0_ch1_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD0 channel 2. */
typedef enum e_pic_trigger_pulse_width_measure_taud0_ch2
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH2_NONE    = ((uint32_t) 0x00),    ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD0_CH2_EQ0_U00 = ((uint32_t) 0x1000U), ///< Output signal of ENCAT0EQ0 or INTTPTMU00
} pic_trigger_pulse_width_measure_taud0_ch2_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD1 channel 0. */
typedef enum e_pic_trigger_pulse_width_measure_taud1_ch0
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH0_NONE    = ((uint32_t) 0x00),     ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH0_EQ0_U11 = ((uint32_t) 0x80000U), ///< Output signal of ENCAT1EQ0 or INTTPTMU11
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH0_EQ1_U20 = ((uint32_t) 0x8000U),  ///< Output signal of ENCAT1EQ1 or INTTPTMU20
} pic_trigger_pulse_width_measure_taud1_ch0_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD1 channel 1. */
typedef enum e_pic_trigger_pulse_width_measure_taud1_ch1
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH1_NONE    = ((uint32_t) 0x00),      ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH1_EQ1_U20 = ((uint32_t) 0x108000U), ///< Output signal of ENCAT1EQ1 or INTTPTMU20
} pic_trigger_pulse_width_measure_taud1_ch1_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUD1 channel 2. */
typedef enum e_pic_trigger_pulse_width_measure_taud1_ch2
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH2_NONE    = ((uint32_t) 0x00),      ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUD1_CH2_EQ0_U11 = ((uint32_t) 0x200000U), ///< Output signal of ENCAT1EQ0 or INTTPTMU11
} pic_trigger_pulse_width_measure_taud1_ch2_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUJ0 channel 0. */
typedef enum e_pic_trigger_pulse_width_measure_tauj0_ch0
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH0_NONE    = ((uint32_t) 0x00),  ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH0_IEC_U10 = ((uint32_t) 0x01U), ///< Output signal of INTENCA0IEC or INTTPTMU10
} pic_trigger_pulse_width_measure_tauj0_ch0_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUJ0 channel 1. */
typedef enum e_pic_trigger_pulse_width_measure_tauj0_ch1
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH1_NONE    = ((uint32_t) 0x00),  ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH1_IEC_U10 = ((uint32_t) 0x02U), ///< Output signal of INTENCA0IEC or INTTPTMU10
} pic_trigger_pulse_width_measure_tauj0_ch1_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUJ0 channel 2. */
typedef enum e_pic_trigger_pulse_width_measure_tauj0_ch2
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH2_NONE    = ((uint32_t) 0x00),   ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH2_IEC_U30 = ((uint32_t)(0x08U)), ///< Output signal of INTENCA1IEC or INTTPTMU30
} pic_trigger_pulse_width_measure_tauj0_ch2_t;

/** Input the trigger signal output from ENCAn and TPTM PEn to TAUJ0 channel 3. */
typedef enum e_pic_trigger_pulse_width_measure_tauj0_ch3
{
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH3_NONE    = ((uint32_t) 0x00),   ///< Not selected
    PIC_TRIGGER_PULSE_WIDTH_MEASURE_TAUJ0_CH3_IEC_U30 = ((uint32_t)(0x10U)), ///< Output signal of INTENCA10IEC or INTTPTMU30
} pic_trigger_pulse_width_measure_tauj0_ch3_t;

/** ENCA input select unit. */
typedef enum e_pic_enca_input_sel_unit
{
    PIC_ENCA_INPUT_SEL_UNIT_1 = 0U,    ///< Select input from unit 1 selector
    PIC_ENCA_INPUT_SEL_UNIT_2,         ///< Select input from unit 2 selector
} pic_enca_input_sel_unit_t;

/** ENCA unit 1 input select signal. */
typedef enum e_pic_enca_unit1_input_signal_sel
{
    PIC_ENCA_UNIT1_INPUT_SIGNAL_SEL_RDC3A0_ENC = (0x01U), ///< Select RDC3AL0_ENC/RDC3AS0_ENC
    PIC_ENCA_UNIT1_INPUT_SIGNAL_SEL_RDC3A1_ENC = (0x02U), ///< Select RDC3AL0_ENC/RDC3AS0_ENC
    PIC_ENCA_UNIT1_INPUT_SIGNAL_SEL_ENCA       = (0x00U), ///< Select ENCA pin input
} pic_enca_unit1_input_signal_sel_t;

/** ENCA unit 2 input select signal. */
typedef enum e_pic_enca_unit2_input_signal_sel
{
    PIC_ENCA_UNIT2_INPUT_SIGNAL_SEL_RDC3A0_ENC = (0x02U), ///< Select RDC3AL0_ENC/RDC3AS0_ENC
    PIC_ENCA_UNIT2_INPUT_SIGNAL_SEL_RDC3A1_ENC = (0x01U), ///< Select RDC3AL0_ENC/RDC3AS0_ENC
    PIC_ENCA_UNIT2_INPUT_SIGNAL_SEL_ENCA       = (0x00U), ///< Select ENCA pin input
} pic_enca_unit2_input_signal_sel_t;

/** Selects the FCMP comparator output data signal. */
typedef enum e_pic_fcmp_output_signal_sel
{
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH0 = (0x0001U), ///< Selects the FCMP comparator output data signal of channel 0.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH1 = (0x0002U), ///< Selects the FCMP comparator output data signal of channel 1.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH2 = (0x0004U), ///< Selects the FCMP comparator output data signal of channel 2.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH3 = (0x0008U), ///< Selects the FCMP comparator output data signal of channel 3.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH4 = (0x0010U), ///< Selects the FCMP comparator output data signal of channel 4.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH5 = (0x0020U), ///< Selects the FCMP comparator output data signal of channel 5.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH6 = (0x0040U), ///< Selects the FCMP comparator output data signal of channel 6.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH7 = (0x0080U), ///< Selects the FCMP comparator output data signal of channel 7.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH8 = (0x0100U), ///< Selects the FCMP comparator output data signal of channel 8.
    PIC_FCMP_OUTPUT_SIGNAL_SEL_CH9 = (0x0200U), ///< Selects the FCMP comparator output data signal of channel 9.
} pic_fcmp_output_signal_sel_t;

/** Selects high/low level FCMP comparator output data as a signal source. */
typedef enum e_pic_fcmp_output_level_sel
{
    PIC_FCMP_OUTPUT_LEVEL_SEL_NONE = (0x00U), ///< FCMP signals is not selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH0  = (0x01U), ///< FCMP0 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH1  = (0x02U), ///< FCMP1 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH2  = (0x03U), ///< FCMP2 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH3  = (0x04U), ///< FCMP3 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH4  = (0x05U), ///< FCMP4 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH5  = (0x06U), ///< FCMP5 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH6  = (0x07U), ///< FCMP6 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH7  = (0x08U), ///< FCMP7 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH8  = (0x09U), ///< FCMP8 comparator output data is selected.
    PIC_FCMP_OUTPUT_LEVEL_SEL_CH9  = (0x0AU), ///< FCMP9 comparator output data is selected.
} pic_fcmp_output_level_sel_t;

typedef enum e_pic_hrpwm_fcmp_hiz_ctrl
{
    PIC_HRPWM_FCMP_HIZ_CTRL_NONE = (0x0000U),         ///< FCMP signals is not selected.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN1_3 = (1U << 15), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN0_3 = (1U << 14), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN1_2 = (1U << 13), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN0_2 = (1U << 12), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN1_1 = (1U << 11), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN0_1 = (1U << 10), ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN1_0 = (1U << 9),  ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM2_IN0_0 = (1U << 8),  ///< Enables Hi-Z control by FCMP output to GTM ATOM2_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN1_3 = (1U << 7),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN0_3 = (1U << 6),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN1_2 = (1U << 5),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN0_2 = (1U << 4),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN1_1 = (1U << 3),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN0_1 = (1U << 2),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN0_HRPWM_HIZ.

    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN1_0 = (1U << 1),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN1_HRPWM_HIZ.
    PIC_HRPWM_FCMP_HIZ_CTRL_ATOM1_IN0_0 = (1U << 0),  ///< Enables Hi-Z control by FCMP output to GTM ATOM1_IN0_HRPWM_HIZ.
} pic_hrpwm_fcmp_hiz_ctrl_t;

/** Selects the signal to output to TAUD0TTIN0. */
typedef enum e_pic_taud0ttin0_sel
{
    PIC_TAUD0TTIN0_SEL_TAUD0I_0 = 0x0U, ///< TAUD0I 0 is selected.
    PIC_TAUD0TTIN0_SEL_TAUD0I_1 = 0x1U, ///< TAUD0I 1 is selected.
    PIC_TAUD0TTIN0_SEL_TAUD1I_0 = 0x2U, ///< TAUD1I 0 is selected.
    PIC_TAUD0TTIN0_SEL_TAUD1I_1 = 0x3U, ///< TAUD1I 1 is selected.
} pic_taud0ttin0_sel_t;

/** Selects the signal to output to TAUD0TTIN1. */
typedef enum e_pic_taud0ttin1_sel
{
    PIC_TAUD0TTIN1_SEL_TAUD0I_1 = 0x0U, ///< TAUD0I 1 is selected.
    PIC_TAUD0TTIN1_SEL_TAUD0I_0 = 0x1U, ///< TAUD0I 0 is selected.
    PIC_TAUD0TTIN1_SEL_TAUD1I_1 = 0x2U, ///< TAUD1I 1 is selected.
    PIC_TAUD0TTIN1_SEL_TAUD1I_0 = 0x3U, ///< TAUD1I 0 is selected.
} pic_taud0ttin1_sel_t;

/** Selects the signal to output to TAUD0TTIN2. */
typedef enum e_pic_taud0ttin2_sel
{
    PIC_TAUD0TTIN2_SEL_TAUD0I_2 = 0X0U, ///< TAUD0I 2 is selected.
    PIC_TAUD0TTIN2_SEL_TAUD0I_3 = 0x1U, ///< TAUD0I 3 is selected.
    PIC_TAUD0TTIN2_SEL_TAUD1I_2 = 0x2U, ///< TAUD1I 2 is selected.
    PIC_TAUD0TTIN2_SEL_TAUD1I_3 = 0x3U, ///< TAUD1I 3 is selected.
} pic_taud0ttin2_sel_t;

/** Selects the signal to output to TAUD0TTIN3. */
typedef enum e_pic_taud0ttin3_sel
{
    PIC_TAUD0TTIN3_SEL_TAUD0I_3 = 0x0U, ///< TAUD0I 3 is selected.
    PIC_TAUD0TTIN3_SEL_TAUD0I_2 = 0x1U, ///< TAUD0I 2 is selected.
    PIC_TAUD0TTIN3_SEL_TAUD1I_3 = 0x2U, ///< TAUD1I 3 is selected.
    PIC_TAUD0TTIN3_SEL_TAUD1I_2 = 0x3U, ///< TAUD1I 2 is selected.
} pic_taud0ttin3_sel_t;

/** Selects the signal to output to TAUD0TTIN4. */
typedef enum e_pic_taud0ttin4_sel
{
    PIC_TAUD0TTIN4_SEL_TAUD0I_4 = 0x0U, ///< TAUD0I 4 is selected.
    PIC_TAUD0TTIN4_SEL_TAUD0I_5 = 0x1U, ///< TAUD0I 5 is selected.
    PIC_TAUD0TTIN4_SEL_TAUD1I_4 = 0x2U, ///< TAUD1I 4 is selected.
    PIC_TAUD0TTIN4_SEL_TAUD1I_5 = 0x3U, ///< TAUD1I 5 is selected.
} pic_taud0ttin4_sel_t;

/** Selects the signal to output to TAUD0TTIN5. */
typedef enum e_pic_taud0ttin5_sel
{
    PIC_TAUD0TTIN5_SEL_TAUD0I_5 = 0x0U, ///< TAUD0I 5 is selected.
    PIC_TAUD0TTIN5_SEL_TAUD0I_4 = 0x1U, ///< TAUD0I 4 is selected.
    PIC_TAUD0TTIN5_SEL_TAUD1I_5 = 0x2U, ///< TAUD1I 5 is selected.
    PIC_TAUD0TTIN5_SEL_TAUD1I_4 = 0x3U, ///< TAUD1I 4 is selected.
} pic_taud0ttin5_sel_t;

/** Selects the signal to output to TAUD0TTIN6. */
typedef enum e_pic_taud0ttin6_sel
{
    PIC_TAUD0TTIN6_SEL_TAUD0I_6 = 0x0U, ///< TAUD0I 6 is selected.
    PIC_TAUD0TTIN6_SEL_TAUD0I_7 = 0x1U, ///< TAUD0I 7 is selected.
    PIC_TAUD0TTIN6_SEL_TAUD1I_6 = 0x2U, ///< TAUD1I 6 is selected.
    PIC_TAUD0TTIN6_SEL_TAUD1I_7 = 0x3U, ///< TAUD1I 7 is selected.
} pic_taud0ttin6_sel_t;

/** Selects the signal to output to TAUD0TTIN7. */
typedef enum e_pic_taud0ttin7_sel
{
    PIC_TAUD0TTIN7_SEL_TAUD0I_7 = 0x0U, ///< TAUD0I 7 is selected.
    PIC_TAUD0TTIN7_SEL_TAUD0I_6 = 0x1U, ///< TAUD0I 6 is selected.
    PIC_TAUD0TTIN7_SEL_TAUD1I_7 = 0x2U, ///< TAUD1I 7 is selected.
    PIC_TAUD0TTIN7_SEL_TAUD1I_6 = 0x3U, ///< TAUD1I 6 is selected.
} pic_taud0ttin7_sel_t;

/** Selects the signal to output to TAUD0TTIN8. */
typedef enum e_pic_taud0ttin8_sel
{
    PIC_TAUD0TTIN8_SEL_TAUD0I_8 = 0x0U, ///< TAUD0I 8 is selected.
    PIC_TAUD0TTIN8_SEL_TAUD0I_9 = 0x1U, ///< TAUD0I 9 is selected.
    PIC_TAUD0TTIN8_SEL_TAUD1I_8 = 0x2U, ///< TAUD1I 8 is selected.
    PIC_TAUD0TTIN8_SEL_TAUD1I_9 = 0x3U, ///< TAUD1I 9 is selected.
} pic_taud0ttin8_sel_t;

/** Selects the signal to output to TAUD0TTIN9. */
typedef enum e_pic_taud0ttin9_sel
{
    PIC_TAUD0TTIN9_SEL_TAUD0I_9 = 0x0U, ///< TAUD0I 9 is selected.
    PIC_TAUD0TTIN9_SEL_TAUD0I_8 = 0x1U, ///< TAUD0I 8 is selected.
    PIC_TAUD0TTIN9_SEL_TAUD1I_9 = 0x2U, ///< TAUD1I 9 is selected.
    PIC_TAUD0TTIN9_SEL_TAUD1I_8 = 0x3U, ///< TAUD1I 8 is selected.
} pic_taud0ttin9_sel_t;

/** Selects the signal to output to TAUD0TTIN10. */
typedef enum e_pic_taud0ttin10_sel
{
    PIC_TAUD0TTIN10_SEL_TAUD0I_10 = 0x0U, ///< TAUD0I 10 is selected.
    PIC_TAUD0TTIN10_SEL_TAUD0I_11 = 0x1U, ///< TAUD0I 11 is selected.
    PIC_TAUD0TTIN10_SEL_TAUD1I_10 = 0x2U, ///< TAUD1I 10 is selected.
    PIC_TAUD0TTIN10_SEL_TAUD1I_11 = 0x3U, ///< TAUD1I 11 is selected.
} pic_taud0ttin10_sel_t;

/** Selects the signal to output to TAUD0TTIN11. */
typedef enum e_pic_taud0ttin11_sel
{
    PIC_TAUD0TTIN11_SEL_TAUD0I_11 = 0x0U, ///< TAUD0I 11 is selected.
    PIC_TAUD0TTIN11_SEL_TAUD0I_10 = 0x1U, ///< TAUD0I 10 is selected.
    PIC_TAUD0TTIN11_SEL_TAUD1I_11 = 0x2U, ///< TAUD1I 11 is selected.
    PIC_TAUD0TTIN11_SEL_TAUD1I_10 = 0x3U, ///< TAUD1I 10 is selected.
} pic_taud0ttin11_sel_t;

/** Selects the signal to output to TAUD0TTIN12. */
typedef enum e_pic_taud0ttin12_sel
{
    PIC_TAUD0TTIN12_SEL_TAUD0I_12 = 0x0U, ///< TAUD0I 12 is selected.
    PIC_TAUD0TTIN12_SEL_TAUD0I_13 = 0x1U, ///< TAUD0I 13 is selected.
    PIC_TAUD0TTIN12_SEL_TAUD1I_12 = 0x2U, ///< TAUD1I 12 is selected.
    PIC_TAUD0TTIN12_SEL_TAUD1I_13 = 0x3U, ///< TAUD1I 13 is selected.
} pic_taud0ttin12_sel_t;

/** Selects the signal to output to TAUD0TTIN13. */
typedef enum e_pic_taud0ttin13_sel
{
    PIC_TAUD0TTIN13_SEL_TAUD0I_13 = 0x0U, ///< TAUD0I 13 is selected.
    PIC_TAUD0TTIN13_SEL_TAUD0I_12 = 0x1U, ///< TAUD0I 12 is selected.
    PIC_TAUD0TTIN13_SEL_TAUD1I_13 = 0x2U, ///< TAUD1I 13 is selected.
    PIC_TAUD0TTIN13_SEL_TAUD1I_12 = 0x3U, ///< TAUD1I 12 is selected.
} pic_taud0ttin13_sel_t;

/** Selects the signal to output to TAUD0TTIN14. */
typedef enum e_pic_taud0ttin14_sel
{
    PIC_TAUD0TTIN14_SEL_TAUD0I_14 = 0x0U, ///< TAUD0I 14 is selected.
    PIC_TAUD0TTIN14_SEL_TAUD0I_15 = 0x1U, ///< TAUD0I 15 is selected.
    PIC_TAUD0TTIN14_SEL_TAUD1I_14 = 0x2U, ///< TAUD1I 14 is selected.
    PIC_TAUD0TTIN14_SEL_TAUD1I_15 = 0x3U, ///< TAUD1I 15 is selected.
} pic_taud0ttin14_sel_t;

/** Selects the signal to output to TAUD0TTIN15. */
typedef enum e_pic_taud0ttin15_sel
{
    PIC_TAUD0TTIN15_SEL_TAUD0I_15 = 0x0U, ///< TAUD0I 15 is selected.
    PIC_TAUD0TTIN15_SEL_TAUD0I_14 = 0x1U, ///< TAUD0I 14 is selected.
    PIC_TAUD0TTIN15_SEL_TAUD1I_15 = 0x2U, ///< TAUD1I 15 is selected.
    PIC_TAUD0TTIN15_SEL_TAUD1I_14 = 0x3U, ///< TAUD1I 14 is selected.
} pic_taud0ttin15_sel_t;

/** Selects the signal to output to TAUD1TTIN0. */
typedef enum e_pic_taud1ttin0_sel
{
    PIC_TAUD1TTIN0_SEL_TAUD1I_0 = 0x0U, ///< TAUD1I 0 is selected.
    PIC_TAUD1TTIN0_SEL_TAUD1I_1 = 0x1U, ///< TAUD1I 1 is selected.
    PIC_TAUD1TTIN0_SEL_TAUD0I_0 = 0x2U, ///< TAUD0I 0 is selected.
    PIC_TAUD1TTIN0_SEL_TAUD0I_1 = 0x3U, ///< TAUD0I 1 is selected.
} pic_taud1ttin0_sel_t;

/** Selects the signal to output to TAUD1TTIN1. */
typedef enum e_pic_taud1ttin1_sel
{
    PIC_TAUD1TTIN1_SEL_TAUD1I_1 = 0x0U, ///< TAUD1I 1 is selected.
    PIC_TAUD1TTIN1_SEL_TAUD1I_0 = 0x1U, ///< TAUD1I 0 is selected.
    PIC_TAUD1TTIN1_SEL_TAUD0I_1 = 0x2U, ///< TAUD0I 1 is selected.
    PIC_TAUD1TTIN1_SEL_TAUD0I_0 = 0x3U, ///< TAUD0I 0 is selected.
} pic_taud1ttin1_sel_t;

/** Selects the signal to output to TAUD1TTIN2. */
typedef enum e_pic_taud1ttin2_sel
{
    PIC_TAUD1TTIN2_SEL_TAUD1I_2 = 0x0U, ///< TAUD1I 2 is selected.
    PIC_TAUD1TTIN2_SEL_TAUD1I_3 = 0x1U, ///< TAUD1I 3 is selected.
    PIC_TAUD1TTIN2_SEL_TAUD0I_2 = 0x2U, ///< TAUD0I 2 is selected.
    PIC_TAUD1TTIN2_SEL_TAUD0I_3 = 0x3U, ///< TAUD0I 3 is selected.
} pic_taud1ttin2_sel_t;

/** Selects the signal to output to TAUD1TTIN3. */
typedef enum e_pic_taud1ttin3_sel
{
    PIC_TAUD1TTIN3_SEL_TAUD1I_3 = 0x0U, ///< TAUD1I 3 is selected.
    PIC_TAUD1TTIN3_SEL_TAUD1I_2 = 0x1U, ///< TAUD1I 2 is selected.
    PIC_TAUD1TTIN3_SEL_TAUD0I_3 = 0x2U, ///< TAUD0I 3 is selected.
    PIC_TAUD1TTIN3_SEL_TAUD0I_2 = 0x3U, ///< TAUD0I 2 is selected.
} pic_taud1ttin3_sel_t;

/** Selects the signal to output to TAUD1TTIN4. */
typedef enum e_pic_taud1ttin4_sel
{
    PIC_TAUD1TTIN4_SEL_TAUD1I_4 = 0x0U, ///< TAUD1I 4 is selected.
    PIC_TAUD1TTIN4_SEL_TAUD1I_5 = 0x1U, ///< TAUD1I 5 is selected.
    PIC_TAUD1TTIN4_SEL_TAUD0I_4 = 0x2U, ///< TAUD0I 4 is selected.
    PIC_TAUD1TTIN4_SEL_TAUD0I_5 = 0x3U, ///< TAUD0I 5 is selected.
} pic_taud1ttin4_sel_t;

/** Selects the signal to output to TAUD1TTIN5. */
typedef enum e_pic_taud1ttin5_sel
{
    PIC_TAUD1TTIN5_SEL_TAUD1I_5 = 0x0U, ///< TAUD1I 5 is selected.
    PIC_TAUD1TTIN5_SEL_TAUD1I_4 = 0x1U, ///< TAUD1I 4 is selected.
    PIC_TAUD1TTIN5_SEL_TAUD0I_5 = 0x2U, ///< TAUD0I 5 is selected.
    PIC_TAUD1TTIN5_SEL_TAUD0I_4 = 0x3U, ///< TAUD0I 4 is selected.
} pic_taud1ttin5_sel_t;

/** Selects the signal to output to TAUD1TTIN6. */
typedef enum e_pic_taud1ttin6_sel
{
    PIC_TAUD1TTIN6_SEL_TAUD1I_6 = 0x0U, ///< TAUD1I 6 is selected.
    PIC_TAUD1TTIN6_SEL_TAUD1I_7 = 0x1U, ///< TAUD1I 7 is selected.
    PIC_TAUD1TTIN6_SEL_TAUD0I_6 = 0x2U, ///< TAUD0I 6 is selected.
    PIC_TAUD1TTIN6_SEL_TAUD0I_7 = 0x3U, ///< TAUD0I 7 is selected.
} pic_taud1ttin6_sel_t;

/** Selects the signal to output to TAUD1TTIN7. */
typedef enum e_pic_taud1ttin7_sel
{
    PIC_TAUD1TTIN7_SEL_TAUD1I_7 = 0x0U, ///< TAUD1I 7 is selected.
    PIC_TAUD1TTIN7_SEL_TAUD1I_6 = 0x1U, ///< TAUD1I 6 is selected.
    PIC_TAUD1TTIN7_SEL_TAUD0I_7 = 0x2U, ///< TAUD0I 7 is selected.
    PIC_TAUD1TTIN7_SEL_TAUD0I_6 = 0x3U, ///< TAUD0I 6 is selected.
} pic_taud1ttin7_sel_t;

/** Selects the signal to output to TAUD1TTIN8. */
typedef enum e_pic_taud1ttin8_sel
{
    PIC_TAUD1TTIN8_SEL_TAUD1I_8 = 0x0U, ///< TAUD1I 8 is selected.
    PIC_TAUD1TTIN8_SEL_TAUD1I_9 = 0x1U, ///< TAUD1I 9 is selected.
    PIC_TAUD1TTIN8_SEL_TAUD0I_8 = 0x2U, ///< TAUD0I 8 is selected.
    PIC_TAUD1TTIN8_SEL_TAUD0I_9 = 0x3U, ///< TAUD0I 9 is selected.
} pic_taud1ttin8_sel_t;

/** Selects the signal to output to TAUD1TTIN9. */
typedef enum e_pic_taud1ttin9_sel
{
    PIC_TAUD1TTIN9_SEL_TAUD1I_9 = 0x0U, ///< TAUD1I 9 is selected.
    PIC_TAUD1TTIN9_SEL_TAUD1I_8 = 0x1U, ///< TAUD1I 8 is selected.
    PIC_TAUD1TTIN9_SEL_TAUD0I_9 = 0x2U, ///< TAUD0I 9 is selected.
    PIC_TAUD1TTIN9_SEL_TAUD0I_8 = 0x3U, ///< TAUD0I 8 is selected.
} pic_taud1ttin9_sel_t;

/** Selects the signal to output to TAUD1TTIN10. */
typedef enum e_pic_taud1ttin10_sel
{
    PIC_TAUD1TTIN10_SEL_TAUD1I_10 = 0x0U, ///< TAUD1I 10 is selected.
    PIC_TAUD1TTIN10_SEL_TAUD1I_11 = 0x1U, ///< TAUD1I 11 is selected.
    PIC_TAUD1TTIN10_SEL_TAUD0I_10 = 0x2U, ///< TAUD0I 10 is selected.
    PIC_TAUD1TTIN10_SEL_TAUD0I_11 = 0x3U, ///< TAUD0I 11 is selected.
} pic_taud1ttin10_sel_t;

/** Selects the signal to output to TAUD1TTIN11. */
typedef enum e_pic_taud1ttin11_sel
{
    PIC_TAUD1TTIN11_SEL_TAUD1I_11 = 0x0U, ///< TAUD1I 11 is selected.
    PIC_TAUD1TTIN11_SEL_TAUD1I_10 = 0x1U, ///< TAUD1I 10 is selected.
    PIC_TAUD1TTIN11_SEL_TAUD0I_11 = 0x2U, ///< TAUD0I 11 is selected.
    PIC_TAUD1TTIN11_SEL_TAUD0I_10 = 0x3U, ///< TAUD0I 10 is selected.
} pic_taud1ttin11_sel_t;

/** Selects the signal to output to TAUD1TTIN12. */
typedef enum e_pic_taud1ttin12_sel
{
    PIC_TAUD1TTIN12_SEL_TAUD1I_12 = 0x0U, ///< TAUD1I 12 is selected.
    PIC_TAUD1TTIN12_SEL_TAUD1I_13 = 0x1U, ///< TAUD1I 13 is selected.
    PIC_TAUD1TTIN12_SEL_TAUD0I_12 = 0x2U, ///< TAUD0I 12 is selected.
    PIC_TAUD1TTIN12_SEL_TAUD0I_13 = 0x3U, ///< TAUD0I 13 is selected.
} pic_taud1ttin12_sel_t;

/** Selects the signal to output to TAUD1TTIN13. */
typedef enum e_pic_taud1ttin13_sel
{
    PIC_TAUD1TTIN13_SEL_TAUD1I_13 = 0x0U, ///< TAUD1I 13 is selected.
    PIC_TAUD1TTIN13_SEL_TAUD1I_12 = 0x1U, ///< TAUD1I 12 is selected.
    PIC_TAUD1TTIN13_SEL_TAUD0I_13 = 0x2U, ///< TAUD0I 13 is selected.
    PIC_TAUD1TTIN13_SEL_TAUD0I_12 = 0x3U, ///< TAUD0I 12 is selected.
} pic_taud1ttin13_sel_t;

/** Selects the signal to output to TAUD1TTIN14. */
typedef enum e_pic_taud1ttin14_sel
{
    PIC_TAUD1TTIN14_SEL_TAUD1I_14 = 0x0U, ///< TAUD1I 14 is selected.
    PIC_TAUD1TTIN14_SEL_TAUD1I_15 = 0x1U, ///< TAUD1I 15 is selected.
    PIC_TAUD1TTIN14_SEL_TAUD0I_14 = 0x2U, ///< TAUD0I 14 is selected.
    PIC_TAUD1TTIN14_SEL_TAUD0I_15 = 0x3U, ///< TAUD0I 15 is selected.
} pic_taud1ttin14_sel_t;

/** Selects the signal to output to TAUD1TTIN15. */
typedef enum e_pic_taud1ttin15_sel
{
    PIC_TAUD1TTIN15_SEL_TAUD1I_15 = 0x0U, ///< TAUD1I 15 is selected.
    PIC_TAUD1TTIN15_SEL_TAUD1I_14 = 0x1U, ///< TAUD1I 14 is selected.
    PIC_TAUD1TTIN15_SEL_TAUD0I_15 = 0x2U, ///< TAUD0I 15 is selected.
    PIC_TAUD1TTIN15_SEL_TAUD0I_14 = 0x3U, ///< TAUD0I 14 is selected.
} pic_taud1ttin15_sel_t;

/** Selects the signal to output to TAUD2TTIN0. */
typedef enum e_pic_taud2ttin0_sel
{
    PIC_TAUD2TTIN0_SEL_TAUD2I_0 = 0x0U, ///< TAUD2I 0 is selected.
    PIC_TAUD2TTIN0_SEL_TAUD2I_1 = 0x1U, ///< TAUD2I 1 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN0_SEL_TAUD3I_0 = 0x2U, ///< TAUD3I 0 is selected.
    PIC_TAUD2TTIN0_SEL_TAUD3I_1 = 0x3U, ///< TAUD3I 1 is selected.
#endif
} pic_taud2ttin0_sel_t;

/** Selects the signal to output to TAUD2TTIN1. */
typedef enum e_pic_taud2ttin1_sel
{
    PIC_TAUD2TTIN1_SEL_TAUD2I_1 = 0x0U, ///< TAUD2I 1 is selected.
    PIC_TAUD2TTIN1_SEL_TAUD2I_0 = 0x1U, ///< TAUD2I 0 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN1_SEL_TAUD3I_1 = 0x2U, ///< TAUD3I 1 is selected.
    PIC_TAUD2TTIN1_SEL_TAUD3I_0 = 0x3U, ///< TAUD3I 0 is selected.
#endif
} pic_taud2ttin1_sel_t;

/** Selects the signal to output to TAUD2TTIN2. */
typedef enum e_pic_taud2ttin2_sel
{
    PIC_TAUD2TTIN2_SEL_TAUD2I_2 = 0X0U, ///< TAUD2I 2 is selected.
    PIC_TAUD2TTIN2_SEL_TAUD2I_3 = 0x1U, ///< TAUD2I 3 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN2_SEL_TAUD3I_2 = 0x2U, ///< TAUD3I 2 is selected.
    PIC_TAUD2TTIN2_SEL_TAUD3I_3 = 0x3U, ///< TAUD3I 3 is selected.
#endif
} pic_taud2ttin2_sel_t;

/** Selects the signal to output to TAUD2TTIN3. */
typedef enum e_pic_taud2ttin3_sel
{
    PIC_TAUD2TTIN3_SEL_TAUD2I_3 = 0x0U, ///< TAUD2I 3 is selected.
    PIC_TAUD2TTIN3_SEL_TAUD2I_2 = 0x1U, ///< TAUD2I 2 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN3_SEL_TAUD3I_3 = 0x2U, ///< TAUD3I 3 is selected.
    PIC_TAUD2TTIN3_SEL_TAUD3I_2 = 0x3U, ///< TAUD3I 2 is selected.
#endif
} pic_taud2ttin3_sel_t;

/** Selects the signal to output to TAUD2TTIN4. */
typedef enum e_pic_taud2ttin4_sel
{
    PIC_TAUD2TTIN4_SEL_TAUD2I_4 = 0x0U, ///< TAUD2I 4 is selected.
    PIC_TAUD2TTIN4_SEL_TAUD2I_5 = 0x1U, ///< TAUD2I 5 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN4_SEL_TAUD3I_4 = 0x2U, ///< TAUD3I 4 is selected.
    PIC_TAUD2TTIN4_SEL_TAUD3I_5 = 0x3U, ///< TAUD3I 5 is selected.
#endif
} pic_taud2ttin4_sel_t;

/** Selects the signal to output to TAUD2TTIN5. */
typedef enum e_pic_taud2ttin5_sel
{
    PIC_TAUD2TTIN5_SEL_TAUD2I_5 = 0x0U, ///< TAUD2I 5 is selected.
    PIC_TAUD2TTIN5_SEL_TAUD2I_4 = 0x1U, ///< TAUD2I 4 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN5_SEL_TAUD3I_5 = 0x2U, ///< TAUD3I 5 is selected.
    PIC_TAUD2TTIN5_SEL_TAUD3I_4 = 0x3U, ///< TAUD3I 4 is selected.
#endif
} pic_taud2ttin5_sel_t;

/** Selects the signal to output to TAUD2TTIN6. */
typedef enum e_pic_taud2ttin6_sel
{
    PIC_TAUD2TTIN6_SEL_TAUD2I_6 = 0x0U, ///< TAUD2I 6 is selected.
    PIC_TAUD2TTIN6_SEL_TAUD2I_7 = 0x1U, ///< TAUD2I 7 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN6_SEL_TAUD3I_6 = 0x2U, ///< TAUD3I 6 is selected.
    PIC_TAUD2TTIN6_SEL_TAUD3I_7 = 0x3U, ///< TAUD3I 7 is selected.
#endif
} pic_taud2ttin6_sel_t;

/** Selects the signal to output to TAUD2TTIN7. */
typedef enum e_pic_taud2ttin7_sel
{
    PIC_TAUD2TTIN7_SEL_TAUD2I_7 = 0x0U, ///< TAUD2I 7 is selected.
    PIC_TAUD2TTIN7_SEL_TAUD2I_6 = 0x1U, ///< TAUD2I 6 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN7_SEL_TAUD3I_7 = 0x2U, ///< TAUD3I 7 is selected.
    PIC_TAUD2TTIN7_SEL_TAUD3I_6 = 0x3U, ///< TAUD3I 6 is selected.
#endif
} pic_taud2ttin7_sel_t;

/** Selects the signal to output to TAUD2TTIN8. */
typedef enum e_pic_taud2ttin8_sel
{
    PIC_TAUD2TTIN8_SEL_TAUD2I_8 = 0x0U, ///< TAUD2I 8 is selected.
    PIC_TAUD2TTIN8_SEL_TAUD2I_9 = 0x1U, ///< TAUD2I 9 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN8_SEL_TAUD3I_8 = 0x2U, ///< TAUD3I 8 is selected.
    PIC_TAUD2TTIN8_SEL_TAUD3I_9 = 0x3U, ///< TAUD3I 9 is selected.
#endif
} pic_taud2ttin8_sel_t;

/** Selects the signal to output to TAUD2TTIN9. */
typedef enum e_pic_taud2ttin9_sel
{
    PIC_TAUD2TTIN9_SEL_TAUD2I_9 = 0x0U, ///< TAUD2I 9 is selected.
    PIC_TAUD2TTIN9_SEL_TAUD2I_8 = 0x1U, ///< TAUD2I 8 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN9_SEL_TAUD3I_9 = 0x2U, ///< TAUD3I 9 is selected.
    PIC_TAUD2TTIN9_SEL_TAUD3I_8 = 0x3U, ///< TAUD3I 8 is selected.
#endif
} pic_taud2ttin9_sel_t;

/** Selects the signal to output to TAUD2TTIN10. */
typedef enum e_pic_taud2ttin10_sel
{
    PIC_TAUD2TTIN10_SEL_TAUD2I_10 = 0x0U, ///< TAUD2I 10 is selected.
    PIC_TAUD2TTIN10_SEL_TAUD2I_11 = 0x1U, ///< TAUD2I 11 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN10_SEL_TAUD3I_10 = 0x2U, ///< TAUD3I 10 is selected.
    PIC_TAUD2TTIN10_SEL_TAUD3I_11 = 0x3U, ///< TAUD3I 11 is selected.
#endif
} pic_taud2ttin10_sel_t;

/** Selects the signal to output to TAUD2TTIN11. */
typedef enum e_pic_taud2ttin11_sel
{
    PIC_TAUD2TTIN11_SEL_TAUD2I_11 = 0x0U, ///< TAUD2I 11 is selected.
    PIC_TAUD2TTIN11_SEL_TAUD2I_10 = 0x1U, ///< TAUD2I 10 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN11_SEL_TAUD3I_11 = 0x2U, ///< TAUD3I 11 is selected.
    PIC_TAUD2TTIN11_SEL_TAUD3I_10 = 0x3U, ///< TAUD3I 10 is selected.
#endif
} pic_taud2ttin11_sel_t;

/** Selects the signal to output to TAUD2TTIN12. */
typedef enum e_pic_taud2ttin12_sel
{
    PIC_TAUD2TTIN12_SEL_TAUD2I_12 = 0x0U, ///< TAUD2I 12 is selected.
    PIC_TAUD2TTIN12_SEL_TAUD2I_13 = 0x1U, ///< TAUD2I 13 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN12_SEL_TAUD3I_12 = 0x2U, ///< TAUD3I 12 is selected.
    PIC_TAUD2TTIN12_SEL_TAUD3I_13 = 0x3U, ///< TAUD3I 13 is selected.
#endif
} pic_taud2ttin12_sel_t;

/** Selects the signal to output to TAUD2TTIN13. */
typedef enum e_pic_taud2ttin13_sel
{
    PIC_TAUD2TTIN13_SEL_TAUD2I_13 = 0x0U, ///< TAUD2I 13 is selected.
    PIC_TAUD2TTIN13_SEL_TAUD2I_12 = 0x1U, ///< TAUD2I 12 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN13_SEL_TAUD3I_13 = 0x2U, ///< TAUD3I 13 is selected.
    PIC_TAUD2TTIN13_SEL_TAUD3I_12 = 0x3U, ///< TAUD3I 12 is selected.
#endif
} pic_taud2ttin13_sel_t;

/** Selects the signal to output to TAUD2TTIN14. */
typedef enum e_pic_taud2ttin14_sel
{
    PIC_TAUD2TTIN14_SEL_TAUD2I_14 = 0x0U, ///< TAUD2I 14 is selected.
    PIC_TAUD2TTIN14_SEL_TAUD2I_15 = 0x1U, ///< TAUD2I 15 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN14_SEL_TAUD3I_14 = 0x2U, ///< TAUD3I 14 is selected.
    PIC_TAUD2TTIN14_SEL_TAUD3I_15 = 0x3U, ///< TAUD3I 15 is selected.
#endif
} pic_taud2ttin14_sel_t;

/** Selects the signal to output to TAUD2TTIN15. */
typedef enum e_pic_taud2ttin15_sel
{
    PIC_TAUD2TTIN15_SEL_TAUD2I_15 = 0x0U, ///< TAUD2I 15 is selected.
    PIC_TAUD2TTIN15_SEL_TAUD2I_14 = 0x1U, ///< TAUD2I 14 is selected.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    PIC_TAUD2TTIN15_SEL_TAUD3I_15 = 0x2U, ///< TAUD3I 15 is selected.
    PIC_TAUD2TTIN15_SEL_TAUD3I_14 = 0x3U, ///< TAUD3I 14 is selected.
#endif
} pic_taud2ttin15_sel_t;

/** Selects the signal to output to TAUD3TTIN0. */
typedef enum e_pic_taud3ttin0_sel
{
    PIC_TAUD3TTIN0_SEL_TAUD3I_0 = 0x0U, ///< TAUD3I 0 is selected.
    PIC_TAUD3TTIN0_SEL_TAUD3I_1 = 0x1U, ///< TAUD3I 1 is selected.
    PIC_TAUD3TTIN0_SEL_TAUD2I_0 = 0x2U, ///< TAUD2I 0 is selected.
    PIC_TAUD3TTIN0_SEL_TAUD2I_1 = 0x3U, ///< TAUD2I 1 is selected.
} pic_taud3ttin0_sel_t;

/** Selects the signal to output to TAUD3TTIN1. */
typedef enum e_pic_taud3ttin1_sel
{
    PIC_TAUD3TTIN1_SEL_TAUD3I_1 = 0x0U, ///< TAUD3I 1 is selected.
    PIC_TAUD3TTIN1_SEL_TAUD3I_0 = 0x1U, ///< TAUD3I 0 is selected.
    PIC_TAUD3TTIN1_SEL_TAUD2I_1 = 0x2U, ///< TAUD2I 1 is selected.
    PIC_TAUD3TTIN1_SEL_TAUD2I_0 = 0x3U, ///< TAUD2I 0 is selected.
} pic_taud3ttin1_sel_t;

/** Selects the signal to output to TAUD3TTIN2. */
typedef enum e_pic_taud3ttin2_sel
{
    PIC_TAUD3TTIN2_SEL_TAUD3I_2 = 0x0U, ///< TAUD3I 2 is selected.
    PIC_TAUD3TTIN2_SEL_TAUD3I_3 = 0x1U, ///< TAUD3I 3 is selected.
    PIC_TAUD3TTIN2_SEL_TAUD2I_2 = 0x2U, ///< TAUD2I 2 is selected.
    PIC_TAUD3TTIN2_SEL_TAUD2I_3 = 0x3U, ///< TAUD2I 3 is selected.
} pic_taud3ttin2_sel_t;

/** Selects the signal to output to TAUD3TTIN3. */
typedef enum e_pic_taud3ttin3_sel
{
    PIC_TAUD3TTIN3_SEL_TAUD3I_3 = 0x0U, ///< TAUD3I 3 is selected.
    PIC_TAUD3TTIN3_SEL_TAUD3I_2 = 0x1U, ///< TAUD3I 2 is selected.
    PIC_TAUD3TTIN3_SEL_TAUD2I_3 = 0x2U, ///< TAUD2I 3 is selected.
    PIC_TAUD3TTIN3_SEL_TAUD2I_2 = 0x3U, ///< TAUD2I 2 is selected.
} pic_taud3ttin3_sel_t;

/** Selects the signal to output to TAUD3TTIN4. */
typedef enum e_pic_taud3ttin4_sel
{
    PIC_TAUD3TTIN4_SEL_TAUD3I_4 = 0x0U, ///< TAUD3I 4 is selected.
    PIC_TAUD3TTIN4_SEL_TAUD3I_5 = 0x1U, ///< TAUD3I 5 is selected.
    PIC_TAUD3TTIN4_SEL_TAUD2I_4 = 0x2U, ///< TAUD2I 4 is selected.
    PIC_TAUD3TTIN4_SEL_TAUD2I_5 = 0x3U, ///< TAUD2I 5 is selected.
} pic_taud3ttin4_sel_t;

/** Selects the signal to output to TAUD3TTIN5. */
typedef enum e_pic_taud3ttin5_sel
{
    PIC_TAUD3TTIN5_SEL_TAUD3I_5 = 0x0U, ///< TAUD3I 5 is selected.
    PIC_TAUD3TTIN5_SEL_TAUD3I_4 = 0x1U, ///< TAUD3I 4 is selected.
    PIC_TAUD3TTIN5_SEL_TAUD2I_5 = 0x2U, ///< TAUD2I 5 is selected.
    PIC_TAUD3TTIN5_SEL_TAUD2I_4 = 0x3U, ///< TAUD2I 4 is selected.
} pic_taud3ttin5_sel_t;

/** Selects the signal to output to TAUD3TTIN6. */
typedef enum e_pic_taud3ttin6_sel
{
    PIC_TAUD3TTIN6_SEL_TAUD3I_6 = 0x0U, ///< TAUD3I 6 is selected.
    PIC_TAUD3TTIN6_SEL_TAUD3I_7 = 0x1U, ///< TAUD3I 7 is selected.
    PIC_TAUD3TTIN6_SEL_TAUD2I_6 = 0x2U, ///< TAUD2I 6 is selected.
    PIC_TAUD3TTIN6_SEL_TAUD2I_7 = 0x3U, ///< TAUD2I 7 is selected.
} pic_taud3ttin6_sel_t;

/** Selects the signal to output to TAUD3TTIN7. */
typedef enum e_pic_taud3ttin7_sel
{
    PIC_TAUD3TTIN7_SEL_TAUD3I_7 = 0x0U, ///< TAUD3I 7 is selected.
    PIC_TAUD3TTIN7_SEL_TAUD3I_6 = 0x1U, ///< TAUD3I 6 is selected.
    PIC_TAUD3TTIN7_SEL_TAUD2I_7 = 0x2U, ///< TAUD2I 7 is selected.
    PIC_TAUD3TTIN7_SEL_TAUD2I_6 = 0x3U, ///< TAUD2I 6 is selected.
} pic_taud3ttin7_sel_t;

/** Selects the signal to output to TAUD3TTIN8. */
typedef enum e_pic_taud3ttin8_sel
{
    PIC_TAUD3TTIN8_SEL_TAUD3I_8 = 0x0U, ///< TAUD3I 8 is selected.
    PIC_TAUD3TTIN8_SEL_TAUD3I_9 = 0x1U, ///< TAUD3I 9 is selected.
    PIC_TAUD3TTIN8_SEL_TAUD2I_8 = 0x2U, ///< TAUD2I 8 is selected.
    PIC_TAUD3TTIN8_SEL_TAUD2I_9 = 0x3U, ///< TAUD2I 9 is selected.
} pic_taud3ttin8_sel_t;

/** Selects the signal to output to TAUD3TTIN9. */
typedef enum e_pic_taud3ttin9_sel
{
    PIC_TAUD3TTIN9_SEL_TAUD3I_9 = 0x0U, ///< TAUD3I 9 is selected.
    PIC_TAUD3TTIN9_SEL_TAUD3I_8 = 0x1U, ///< TAUD3I 8 is selected.
    PIC_TAUD3TTIN9_SEL_TAUD2I_9 = 0x2U, ///< TAUD2I 9 is selected.
    PIC_TAUD3TTIN9_SEL_TAUD2I_8 = 0x3U, ///< TAUD2I 8 is selected.
} pic_taud3ttin9_sel_t;

/** Selects the signal to output to TAUD3TTIN10. */
typedef enum e_pic_taud3ttin10_sel
{
    PIC_TAUD3TTIN10_SEL_TAUD3I_10 = 0x0U, ///< TAUD3I 10 is selected.
    PIC_TAUD3TTIN10_SEL_TAUD3I_11 = 0x1U, ///< TAUD3I 11 is selected.
    PIC_TAUD3TTIN10_SEL_TAUD2I_10 = 0x2U, ///< TAUD2I 10 is selected.
    PIC_TAUD3TTIN10_SEL_TAUD2I_11 = 0x3U, ///< TAUD2I 11 is selected.
} pic_taud3ttin10_sel_t;

/** Selects the signal to output to TAUD3TTIN11. */
typedef enum e_pic_taud3ttin11_sel
{
    PIC_TAUD3TTIN11_SEL_TAUD3I_11 = 0x0U, ///< TAUD3I 11 is selected.
    PIC_TAUD3TTIN11_SEL_TAUD3I_10 = 0x1U, ///< TAUD3I 10 is selected.
    PIC_TAUD3TTIN11_SEL_TAUD2I_11 = 0x2U, ///< TAUD2I 11 is selected.
    PIC_TAUD3TTIN11_SEL_TAUD2I_10 = 0x3U, ///< TAUD2I 10 is selected.
} pic_taud3ttin11_sel_t;

/** Selects the signal to output to TAUD3TTIN12. */
typedef enum e_pic_taud3ttin12_sel
{
    PIC_TAUD3TTIN12_SEL_TAUD3I_12 = 0x0U, ///< TAUD3I 12 is selected.
    PIC_TAUD3TTIN12_SEL_TAUD3I_13 = 0x1U, ///< TAUD3I 13 is selected.
    PIC_TAUD3TTIN12_SEL_TAUD2I_12 = 0x2U, ///< TAUD2I 12 is selected.
    PIC_TAUD3TTIN12_SEL_TAUD2I_13 = 0x3U, ///< TAUD2I 13 is selected.
} pic_taud3ttin12_sel_t;

/** Selects the signal to output to TAUD3TTIN13. */
typedef enum e_pic_taud3ttin13_sel
{
    PIC_TAUD3TTIN13_SEL_TAUD3I_13 = 0x0U, ///< TAUD3I 13 is selected.
    PIC_TAUD3TTIN13_SEL_TAUD3I_12 = 0x1U, ///< TAUD3I 12 is selected.
    PIC_TAUD3TTIN13_SEL_TAUD2I_13 = 0x2U, ///< TAUD2I 13 is selected.
    PIC_TAUD3TTIN13_SEL_TAUD2I_12 = 0x3U, ///< TAUD2I 12 is selected.
} pic_taud3ttin13_sel_t;

/** Selects the signal to output to TAUD3TTIN14. */
typedef enum e_pic_taud3ttin14_sel
{
    PIC_TAUD3TTIN14_SEL_TAUD3I_14 = 0x0U, ///< TAUD3I 14 is selected.
    PIC_TAUD3TTIN14_SEL_TAUD3I_15 = 0x1U, ///< TAUD3I 15 is selected.
    PIC_TAUD3TTIN14_SEL_TAUD2I_14 = 0x2U, ///< TAUD2I 14 is selected.
    PIC_TAUD3TTIN14_SEL_TAUD2I_15 = 0x3U, ///< TAUD2I 15 is selected.
} pic_taud3ttin14_sel_t;

/** Selects the signal to output to TAUD3TTIN15. */
typedef enum e_pic_taud3ttin15_sel
{
    PIC_TAUD3TTIN15_SEL_TAUD3I_15 = 0x0U, ///< TAUD3I 15 is selected.
    PIC_TAUD3TTIN15_SEL_TAUD3I_14 = 0x1U, ///< TAUD3I 14 is selected.
    PIC_TAUD3TTIN15_SEL_TAUD2I_15 = 0x2U, ///< TAUD2I 15 is selected.
    PIC_TAUD3TTIN15_SEL_TAUD2I_14 = 0x3U, ///< TAUD2I 14 is selected.
} pic_taud3ttin15_sel_t;

/** Selects the signal to output to TAUJ2TTIN2. */
typedef enum e_pic_tauj2ttin2_sel
{
    PIC_TAUJ2TTIN2_SEL_TAUDJ2I_2   = 0x0U, ///< TAUJ2I2 is selected.
    PIC_TAUJ2TTIN2_SEL_TAUJ1TTOUT0 = 0x1U, ///< TAUJ1TTOUT0 is selected.
} pic_tauj2ttin2_sel_t;

/** Selects the signal to output to TAUJ2TTIN3. */
typedef enum e_pic_tauj2ttin3_sel
{
    PIC_TAUJ2TTIN3_SEL_TAUDJ2I_3   = 0x0U, ///< TAUJ2I3 is selected.
    PIC_TAUJ2TTIN3_SEL_TAUJ1TTOUT0 = 0x1U, ///< TAUJ1TTOUT0 is selected.
} pic_tauj2ttin3_sel_t;

/** Selects the signal to output to TAUJ3TTIN3. */
typedef enum e_pic_tauj3ttin3_sel
{
    PIC_TAUJ3TTIN3_SEL_TAUJ3I_3    = 0x00U, ///< TAUJ3I3 is selected.
    PIC_TAUJ3TTIN3_SEL_RTCA0OUT    = 0x02U, ///< RTCA0OUT is selected.
    PIC_TAUJ3TTIN3_SEL_TAUJ0TTOUT0 = 0x0AU, ///< TAUJ0TTOUT0 is selected.
} pic_tauj3ttin3_sel_t;

/** Selects the signal to output to TAUJ3TTIN2. */
typedef enum e_pic_tauj3ttin2_sel
{
    PIC_TAUJ3TTIN2_SEL_TAUJ3I_2    = 0x00U, ///< TAUJ3I2 is selected.
    PIC_TAUJ3TTIN2_SEL_RTCA0OUT    = 0x01U, ///< RTCA0OUT is selected.
    PIC_TAUJ3TTIN2_SEL_TAUJ0TTOUT0 = 0x05U, ///< TAUJ0TTOUT0 is selected.
} pic_tauj3ttin2_sel_t;

/** Selects sources to TAUDn input signals channel m */
typedef enum e_pic_taud_input_sel
{
    PIC_TAUD_INPUT_SEL_PIC1TAUDSEL     = 0x00U, ///< Select by PIC1TAUDnSEL.
    PIC_TAUD_INPUT_SEL_PIC1POMONSEL    = 0x01U, ///< Select by PIC1POMONSEL for TAUD2 only.
    PIC_TAUD_INPUT_SEL_RSENT           = 0x02U, ///< Select RSENT input.
    PIC_TAUD_INPUT_SEL_ATU_C_SUB_0     = 0x04U, ///< Select ATU Timer C, sub-block 0.
    PIC_TAUD_INPUT_SEL_ATU_C_SUB_4     = 0x05U, ///< Select ATU Timer C, sub-block 4.
    PIC_TAUD_INPUT_SEL_ATU_C_SUB_8     = 0x06U, ///< Select ATU Timer C, sub-block 8.
    PIC_TAUD_INPUT_SEL_ATU_C_SUB_12    = 0x07U, ///< Select ATU Timer C, sub-block 12.
    PIC_TAUD_INPUT_SEL_ATU_D_SUB_0     = 0x08U, ///< Select ATU Timer D, sub-block 0.
    PIC_TAUD_INPUT_SEL_ATU_D_SUB_1     = 0x09U, ///< Select ATU Timer D, sub-block 1.
    PIC_TAUD_INPUT_SEL_BOOST_MOSFET    = 0x0CU, ///< Select Boost MOSFET control (TAUD0 only).
    PIC_TAUD_INPUT_SEL_HIGHSIDE_MOSFET = 0x0DU, ///< Select HighSide MOSFET control (TAUD0 only).
    PIC_TAUD_INPUT_SEL_LOWSIDE_MOSFET  = 0x0EU, ///< Select LowSide MOSFET control (TAUD0 only).
} pic_taud_input_sel_t;

/** Select output to be monitored using TAUD2.  */
typedef enum e_pic_taud2_out_monitor_sel
{
    PIC_TAUD2_OUT_MONITOR_SEL_0 = 0x00U, ///< Option 0 is selected (detail in User Manual).
    PIC_TAUD2_OUT_MONITOR_SEL_1 = 0x01U, ///< Option 1 is selected (detail in User Manual).
    PIC_TAUD2_OUT_MONITOR_SEL_2 = 0x02U, ///< Option 2 is selected (detail in User Manual).
    PIC_TAUD2_OUT_MONITOR_SEL_3 = 0x03U, ///< Option 3 is selected (detail in User Manual).
    PIC_TAUD2_OUT_MONITOR_SEL_4 = 0x04U, ///< Option 4 is selected (detail in User Manual).
} pic_taud2_out_monitor_sel_t;

/** Select input to be monitored using TAUJ2.  */
typedef enum e_pic_tauj2_in_monitor_sel
{
    PIC_TAUJ2_IN_MONITOR_SEL_0 = 0x00U, ///< Option 0 is selected (detail in User Manual).
    PIC_TAUJ2_IN_MONITOR_SEL_1 = 0x01U, ///< Option 1 is selected (detail in User Manual).
    PIC_TAUJ2_IN_MONITOR_SEL_2 = 0x02U, ///< Option 2 is selected (detail in User Manual).
} pic_tauj2_in_monitor_sel_t;

/** Selects trigger sources for ENCAnTTIN0. */
typedef enum e_pic_enca_trig_sel
{
    PIC_ENCA_TRIG_SEL_INTENCA1IEC        = (int32_t)(0x80000000U), ///< Select INTENCA1IEC as trigger source for ENCAnTTIN0.
    PIC_ENCA_TRIG_SEL_INTENCA1I1         = (int32_t)(0x40000000U), ///< Select INTENCA1I1 as trigger source for ENCAnTTIN0 (INTENCA1I1 compare 1 match or capture 1 interrupt).
    PIC_ENCA_TRIG_SEL_INTENCA1I0         = (int32_t)(0x20000000U), ///< Select INTENCA1I0 as trigger source for ENCAnTTIN0 (INTENCA1I0 compare 0 match or capture 0 interrupt).
    PIC_ENCA_TRIG_SEL_INTENCA0IEC        = (int32_t)(0x10000000U), ///< Select INTENCA0IEC as trigger source for ENCAnTTIN0 (INTENCA0IEC clear interrupt signal by encoder).
    PIC_ENCA_TRIG_SEL_INTENCA0I0         = (int32_t)(0x04000000U), ///< Select INTENCA0I0 as trigger source for ENCAnTTIN0 (INTENCA0I0 compare 0 match or capture 0 interrupt).
    PIC_ENCA_TRIG_SEL_ENCA1I0            = (int32_t)(0x00000020U), ///< Select ENCA1I0 pin as trigger source for ENCAnTTIN0.
    PIC_ENCA_TRIG_SEL_INTENCA0I1         = (int32_t)(0x08000000U), ///< Select INTENCA0I1 as trigger source for ENCAnTTIN0 (INTENCA0I1 compare 1 match or capture 1 interrupt).
    PIC_ENCA_TRIG_SEL_ENCA0I0            = (int32_t)(0x00000010U), ///< Select ENCA0I0 pin as trigger source for ENCAnTTIN0.
    PIC_ENCA_TRIG_SEL_GTM_ENCATCFG1_2416 = (int32_t)(0x00000008U), ///< Select GTM timer output selected by PIC2ENCATCFG1 bits 24 to 16 as ENCA trigger.
    PIC_ENCA_TRIG_SEL_GTM_ENCATCFG1_0800 = (int32_t)(0x00000004U), ///< Select GTM timer output selected by PIC2ENCATCFG1 bits 8 to 0 as ENCA trigger.
    PIC_ENCA_TRIG_SEL_GTM_ENCATCFG0_2416 = (int32_t)(0x00000002U), ///< Select GTM timer output selected by PIC2ENCATCFG0 bits 24 to 16 as ENCA trigger.
    PIC_ENCA_TRIG_SEL_GTM_ENCATCFG0_0800 = (int32_t)(0x00000001U), ///< Select GTM timer output selected by PIC2ENCATCFG0 bits 8 to 0 as ENCA trigger.
} pic_enca_trig_sel_t;

/** Selects ENCAn input connection signals  */
typedef enum e_pic_enca_input_connect_sel
{
    PIC_ENCA_INPUT_CONNECT_SEL_SERIES   = 0x00U, ///< Select series connection.
    PIC_ENCA_INPUT_CONNECT_SEL_PARALLEL = 0x01U, ///< Select parallel connection.
    PIC_ENCA_INPUT_CONNECT_SEL_CASCADE  = 0x02U, ///< Select cascade
} pic_enca_input_connect_sel_t;

/** Select Hi-Z function for GTM outputs */
typedef enum e_pic_hiz_gtm_out
{
    PIC_HIZ_GTM_OUT_HIZ3ECM = 0x80U,   ///< Select Hi-Z control by ECM for GTM ATOM3 outputs.
    PIC_HIZ_GTM_OUT_HIZ2ECM = 0x40U,   ///< Select Hi-Z control by ECM for GTM ATOM2 outputs.
    PIC_HIZ_GTM_OUT_HIZ1ECM = 0x20U,   ///< Select Hi-Z control by ECM for GTM ATOM1 outputs.
    PIC_HIZ_GTM_OUT_HIZ0ECM = 0x10U,   ///< Select Hi-Z control by ECM for GTM ATOM0 outputs.
    PIC_HIZ_GTM_OUT_HIZ3ESO = 0x08U,   ///< Select Hi-Z control by ESO3Z for GTM ATOM3 outputs.
    PIC_HIZ_GTM_OUT_HIZ2ESO = 0x04U,   ///< Select Hi-Z control by ESO2Z for GTM ATOM2 outputs.
    PIC_HIZ_GTM_OUT_HIZ1ESO = 0x02U,   ///< Select Hi-Z control by ESO1Z for GTM ATOM1 outputs.
    PIC_HIZ_GTM_OUT_HIZ0ESO = 0x01U,   ///< Select Hi-Z control by ESO0Z for GTM ATOM0 outputs.
} pic_hiz_gtm_out_t;

/** Selects the TPBA and GTM output as a signal source for RDC3AS0. */
typedef enum e_pic_rdc_excit_input_sel
{
    PIC_RDC_EXCIT_INPUT_SEL_NONE         = 0x00U,  ///< Signal is not selected.
    PIC_RDC_EXCIT_INPUT_SEL_TPBA0TINTPAT = 0x01U,  ///< TPBA0TINTPAT output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_TPBA1TINTPAT = 0x02U,  ///< TPBA1TINTPAT output signal is selected.

    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ0 = 0x03U, ///< GTM_PSM0_IRQ0 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ1 = 0x04U, ///< GTM_PSM0_IRQ1 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ2 = 0x05U, ///< GTM_PSM0_IRQ2 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ3 = 0x06U, ///< GTM_PSM0_IRQ3 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ4 = 0x07U, ///< GTM_PSM0_IRQ4 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ5 = 0x08U, ///< GTM_PSM0_IRQ5 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ6 = 0x09U, ///< GTM_PSM0_IRQ6 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM0_IRQ7 = 0x0AU, ///< GTM_PSM0_IRQ7 output signal is selected.

    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ0 = 0x0BU, ///< GTM_PSM1_IRQ0 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ1 = 0x0CU, ///< GTM_PSM1_IRQ1 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ2 = 0x0DU, ///< GTM_PSM1_IRQ2 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ3 = 0x0EU, ///< GTM_PSM1_IRQ3 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ4 = 0x0FU, ///< GTM_PSM1_IRQ4 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ5 = 0x10U, ///< GTM_PSM1_IRQ5 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ6 = 0x11U, ///< GTM_PSM1_IRQ6 output signal is selected.
    PIC_RDC_EXCIT_INPUT_SEL_GTM_PSM1_IRQ7 = 0x12U, ///< GTM_PSM1_IRQ7 output signal is selected.
} pic_rdc_excit_input_sel_t;

/** Selects hardware triggers of MSPI GTMMSPICFG signal. */
typedef enum e_pic_mspi_trigger_sel_gtm
{
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT0   = (int32_t)(0x00010000U), ///< Selects GTM_TOM[k]_OUT[0]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT1   = (int32_t)(0x00020000U), ///< Selects GTM_TOM[k]_OUT[1]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT2   = (int32_t)(0x00040000U), ///< Selects GTM_TOM[k]_OUT[2]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT3   = (int32_t)(0x00080000U), ///< Selects GTM_TOM[k]_OUT[3]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT4   = (int32_t)(0x00100000U), ///< Selects GTM_TOM[k]_OUT[4]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT5   = (int32_t)(0x00200000U), ///< Selects GTM_TOM[k]_OUT[5]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT6   = (int32_t)(0x00400000U), ///< Selects GTM_TOM[k]_OUT[6]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT7   = (int32_t)(0x00800000U), ///< Selects GTM_TOM[k]_OUT[7]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT8   = (int32_t)(0x01000000U), ///< Selects GTM_TOM[k]_OUT[8]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT9   = (int32_t)(0x02000000U), ///< Selects GTM_TOM[k]_OUT[9]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT10  = (int32_t)(0x04000000U), ///< Selects GTM_TOM[k]_OUT[10]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT11  = (int32_t)(0x08000000U), ///< Selects GTM_TOM[k]_OUT[11]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT12  = (int32_t)(0x10000000U), ///< Selects GTM_TOM[k]_OUT[12]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT13  = (int32_t)(0x20000000U), ///< Selects GTM_TOM[k]_OUT[13]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT14  = (int32_t)(0x40000000U), ///< Selects GTM_TOM[k]_OUT[14]
    PIC_MSPI_TRIGGER_SEL_GTM_TOM_OUT15  = (int32_t)(0x80000000U), ///< Selects GTM_TOM[k]_OUT[15]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT0  = (int32_t)(0x00000100U), ///< Selects GTM_ATOM[k]_OUT[0]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT1  = (int32_t)(0x00000200U), ///< Selects GTM_ATOM[k]_OUT[1]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT2  = (int32_t)(0x00000400U), ///< Selects GTM_ATOM[k]_OUT[2]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT3  = (int32_t)(0x00000800U), ///< Selects GTM_ATOM[k]_OUT[3]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT4  = (int32_t)(0x00001000U), ///< Selects GTM_ATOM[k]_OUT[4]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT5  = (int32_t)(0x00002000U), ///< Selects GTM_ATOM[k]_OUT[5]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT6  = (int32_t)(0x00004000U), ///< Selects GTM_ATOM[k]_OUT[6]
    PIC_MSPI_TRIGGER_SEL_GTM_ATOM_OUT7  = (int32_t)(0x00008000U), ///< Selects GTM_ATOM[k]_OUT[7]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ0 = (int32_t)(0x00000001U), ///< Selects GTM_MCS[k]_S_IRQ[0]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ1 = (int32_t)(0x00000002U), ///< Selects GTM_MCS[k]_S_IRQ[1]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ2 = (int32_t)(0x00000004U), ///< Selects GTM_MCS[k]_S_IRQ[2]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ3 = (int32_t)(0x00000008U), ///< Selects GTM_MCS[k]_S_IRQ[3]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ4 = (int32_t)(0x00000010U), ///< Selects GTM_MCS[k]_S_IRQ[4]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ5 = (int32_t)(0x00000020U), ///< Selects GTM_MCS[k]_S_IRQ[5]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ6 = (int32_t)(0x00000040U), ///< Selects GTM_MCS[k]_S_IRQ[6]
    PIC_MSPI_TRIGGER_SEL_GTM_MCS_S_IRQ7 = (int32_t)(0x00000080U), ///< Selects GTM_MCS[k]_S_IRQ[7]
} pic_mspi_trigger_sel_gtm_t;

/** Selects an effective edge of MSPI HW trigger signal */
typedef enum e_pic_mspi_trigger_edge_sel
{
    PIC_MSPI_TRIGGER_EDGE_SEL_RISING  = 0x00U, ///< Select rising edge.
    PIC_MSPI_TRIGGER_EDGE_SEL_FALLING = 0x01U, ///< Select falling edge.
    PIC_MSPI_TRIGGER_EDGE_SEL_BOTH    = 0x02U, ///< Select both edge.
} pic_mspi_trigger_edge_sel_t;

/** Selects an effective edge of DFE HW trigger signal */
typedef enum e_pic_dfe_trigger_edge_sel
{
    PIC_DFE_TRIGGER_EDGE_SEL_RISING  = 0x00U, ///< Select rising edge.
    PIC_DFE_TRIGGER_EDGE_SEL_FALLING = 0x01U, ///< Select falling edge.
} pic_dfe_trigger_edge_sel_t;

/** Selects the a signal source for TSG3nCLKI */
typedef enum e_pic_tsg3clki_signal_src_sel
{
    PIC_TSG3CLKI_SIGNAL_SRC_SEL_PIC2TSG3TRG2016 = 0x00U, ///< Select by PIC2TSG3nTRGk[20:16].
    PIC_TSG3CLKI_SIGNAL_SRC_SEL_EXT_INPUT       = 0x01U, ///< Select from external input TSG3nCLKI.
} pic_tsg3clki_signal_src_sel_t;

/** Select GTM output type as TSG3nCLKI */
typedef enum e_pic_tsg3clki_gtm_out_type_sel
{
    PIC_TSG3CLKI_GTM_OUT_TYPE_SEL_LEVEL = 0x00U, ///< GTM outputs as level type for TSG3nCLKI.
    PIC_TSG3CLKI_GTM_OUT_TYPE_SEL_PULSE = 0x01U, ///< GTM outputs as pulse type for TSG3nCLKI.
} pic_tsg3clki_gtm_out_type_sel_t;

/** Selects sources to TAUDnTTINm (m = 0 to 7) input signals */
typedef union u_pic_taudtinsel0_ctrl
{
    uint32_t taudtinsel;

    /** Struct contain bit field of TAUDnTTINm Input Selection Control Register 0. */
    struct st_pic_taudtinsel0_ctrl_b
    {
        pic_taud_input_sel_t taudtin_sel0 : 4; ///< Selects the signal to output to TAUDnTTIN0.
        pic_taud_input_sel_t taudtin_sel1 : 4; ///< Selects the signal to output to TAUDnTTIN1.
        pic_taud_input_sel_t taudtin_sel2 : 4; ///< Selects the signal to output to TAUDnTTIN2.
        pic_taud_input_sel_t taudtin_sel3 : 4; ///< Selects the signal to output to TAUDnTTIN3.
        pic_taud_input_sel_t taudtin_sel4 : 4; ///< Selects the signal to output to TAUDnTTIN4.
        pic_taud_input_sel_t taudtin_sel5 : 4; ///< Selects the signal to output to TAUDnTTIN5.
        pic_taud_input_sel_t taudtin_sel6 : 4; ///< Selects the signal to output to TAUDnTTIN6.
        pic_taud_input_sel_t taudtin_sel7 : 4; ///< Selects the signal to output to TAUDnTTIN7.
    } taudtinsel_b;
} pic_taudtinsel0_ctrl_t;

/** Selects sources to TAUDnTTINm (m = 8 to 15) input signals */
typedef union u_pic_taudtinsel1_ctrl
{
    uint32_t taudtinsel;

    /** Struct contain bit field of TAUDnTTINm Input Selection Control Register 0. */
    struct st_pic_taudtinsel1_ctrl_b
    {
        pic_taud_input_sel_t taudtin_sel8  : 4; ///< Selects the signal to output to TAUDnTTIN8.
        pic_taud_input_sel_t taudtin_sel9  : 4; ///< Selects the signal to output to TAUDnTTIN9.
        pic_taud_input_sel_t taudtin_sel10 : 4; ///< Selects the signal to output to TAUDnTTIN10.
        pic_taud_input_sel_t taudtin_sel11 : 4; ///< Selects the signal to output to TAUDnTTIN11.
        pic_taud_input_sel_t taudtin_sel12 : 4; ///< Selects the signal to output to TAUDnTTIN12.
        pic_taud_input_sel_t taudtin_sel13 : 4; ///< Selects the signal to output to TAUDnTTIN13.
        pic_taud_input_sel_t taudtin_sel14 : 4; ///< Selects the signal to output to TAUDnTTIN14.
        pic_taud_input_sel_t taudtin_sel15 : 4; ///< Selects the signal to output to TAUDnTTIN15.
    } taudtinsel_b;
} pic_taudtinsel1_ctrl_t;

/** Selects sources to TAUDn input signals */
typedef struct st_pic_taudtinsel_ctrl
{
    pic_taudtinsel0_ctrl_t taudtinsel0_ctrl; ///< TAUDnTTINm (m = 0 to 7) Input Selection Control Register
    pic_taudtinsel1_ctrl_t taudtinsel1_ctrl; ///< TAUDnTTINm (m = 8 to 15) Input Selection Control Register
} pic_taudtinsel_ctrl_t;

typedef union u_pic_fcmp_output_gtm_ctrl
{
    uint32_t gtmsen;

    /** Struct contain bit field of GTM Timer Input Selection Control Register k. */
    struct st_pic_fcmp_output_gtm_ctrl_b
    {
        pic_fcmp_output_level_sel_t gtm_src_sel_0300 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk0300).
        pic_fcmp_output_level_sel_t gtm_src_sel_0704 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk0704).
        pic_fcmp_output_level_sel_t gtm_src_sel_1108 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk1108).
        pic_fcmp_output_level_sel_t gtm_src_sel_1512 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk1512).
        pic_fcmp_output_level_sel_t gtm_src_sel_1916 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk1916).
        pic_fcmp_output_level_sel_t gtm_src_sel_2320 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk2320).
        pic_fcmp_output_level_sel_t gtm_src_sel_2724 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk2724).
        pic_fcmp_output_level_sel_t gtm_src_sel_3128 : 4; ///< Selects the FCMP comparator output data as a signal source for GTM (PIC2GTMSENk3128)
    } gtmsen_b;
} pic_fcmp_output_gtm_ctrl_t;

/** Selects low level of FCMP comparator output data as the sources input of HRPWM Hi-Z Control Function. */
typedef union u_pic_fcmp_output_low_level_ctrl
{
    uint32_t fcmpolsen;

    /** Struct contain bit field of FCMP Output Low Level Selection Control Register. */
    struct st_pic_fcmp_output_low_level_ctrl_b
    {
        pic_fcmp_output_level_sel_t fcmp_output_group0 : 4;  ///< Selects the inversion of FCMP comparator output data as a signal source for GTM CDTM1_DTM4_AUX_IN0 and ATOM1_IN0_HRPWM_HIZ
        pic_fcmp_output_level_sel_t fcmp_output_group1 : 4;  ///< Selects the inversion of FCMP comparator output data as a signal source for GTM CDTM1_DTM4_AUX_IN1 and ATOM1_IN1_HRPWM_HIZ
        pic_fcmp_output_level_sel_t fcmp_output_group2 : 4;  ///< Selects the inversion of FCMP comparator output data as a signal source for GTM CDTM2_DTM4_AUX_IN0 and ATOM2_IN0_HRPWM_HIZ
        pic_fcmp_output_level_sel_t fcmp_output_group3 : 4;  ///< Selects the inversion of FCMP comparator output data as a signal source for GTM CDTM2_DTM4_AUX_IN1 and ATOM2_IN1_HRPWM_HIZ
        uint32_t                                       : 16; ///< Reserved.
    } fcmpolsen_b;
} pic_fcmp_output_low_level_ctrl_t;

/** Selects high/low level of FCMP comparator output data as the sources input of HRPWM High Low Fix Control Function and Hi-Z Control Function of PIC1. */
typedef union u_pic_fcmp_output_level_tsg_ctrl
{
    uint32_t tsgsen;

    /** Struct contain bit field of FCMP Output Low Level Selection Control Register. */
    struct st_pic_fcmp_output_level_tsg_ctrl_b
    {
        pic_fcmp_output_level_sel_t fcmp_output_tsg31 : 4;  ///< Selects the FCMP comparator output data as a signal source for TSG31.
        pic_fcmp_output_level_sel_t fcmp_output_tsg32 : 4;  ///< Selects the FCMP comparator output data as a signal source for TSG32.
        uint32_t                                      : 24; ///< Reserved.
    } tsgsen_b;
} pic_fcmp_output_level_tsg_ctrl_t;

/** Selects a source of input of EMU3Sn. */
typedef union u_pic_emu_input_source_select
{
    uint32_t emuisen0;

    /** Struct contain bit field of EMU Input Source Select Register 0 */
    struct st_pic_emu_input_source_select_b
    {
        pic_emu_input_peak_interrupt_gtm_atom_t emu3s0_input_peak_interrupt_gtm_atom     : 7; ///< Selects the GTM output as a signal source for EMU3S0 Peak Interrupt.
        uint32_t                                                                         : 1; ///< Reserved.
        pic_emu_input_trough_interrupt_gtm_atom_t emu3s0_input_trough_interrupt_gtm_atom : 7; ///< Selects the GTM output as a signal source for EMU3S0 Trough Interrupt.
        uint32_t                                                                         : 1; ///< Reserved.
        pic_emu_input_peak_interrupt_gtm_atom_t emu3s1_input_peak_interrupt_gtm_atom     : 7; ///< Selects the GTM output as a signal source for EMU3S1 Peak Interrupt.
        uint32_t                                                                         : 1; ///< Reserved.
        pic_emu_input_trough_interrupt_gtm_atom_t emu3s1_input_trough_interrupt_gtm_atom : 7; ///< Selects the GTM output as a signal source for EMU3S1 Trough Interrupt.
        uint32_t                                                                         : 1; ///< Reserved.
    } emuisen0_b;
} pic_emu_input_source_select_t;

typedef union  u_pic_adc_convert_trigger_gtm_output_sel
{
    uint32_t adtcfg;

    /** Struct contain bit field of A/D Converter Trigger Output Configuration Register */
    struct st_pic_adc_convert_trigger_gtm_output_sel_b
    {
        uint16_t adtcfg0800 : 9;       ///< Selects the GTM output as a PIC20ADTEN5nj (n=0-3, j=0 to 5) Bit[0 + (k × 2)] register configuration.
        uint8_t             : 7;       ///< Reserved.
        uint16_t adtcfg2416 : 9;       ///< Selects the GTM output as a PIC20ADTEN5nj (n=0-3, j=0 to 5) Bit[1 + (k × 2)] register configuration.
        uint8_t             : 7;       ///< Reserved.
    } adtcfg_b;
} pic_adc_convert_trigger_gtm_output_sel_t;

/** Configure The PIC2DSADTCFGk (k = 0 to 13) register assigns timer output of GTM (TOM, ATOM, TIO) to PIC20DSADTENjn0 (j = 0 to 1, n = 0 to 13).  */
typedef union u_pic_dsadc_sta_sto_trigger_out_cfg
{
    uint32_t dsadcstasto;

    /** Struct contain bit field of DSADC Start / Stop Trigger Output Configuration Register k (k = 0 to 13) */
    struct st_pic_dsadc_sta_sto_trigger_out_cfg
    {
    	uint16_t dsadcstasto0800 : 9;  ///< Selects the GTM output as a PIC20DSADTENjn0 (j = 0 to 1, n = 0 to 13) Bit[1 +(k × 2)] register configuration.
        uint8_t                  : 7;  ///< Reserved
        uint16_t dsadcstasto2416 : 9;  ///< Selects the GTM output as a PIC20DSADTENjn0 (j = 0 to 1, n = 0 to 13) Bit[0 +(k × 2)] register configuration.
        uint8_t                  : 7;  ///< Reserved
    } dsadcstasto_b;
} pic_dsadc_sta_sto_trigger_out_cfg_t;

/** Configure PIC2DSADTSENk register selects a trigger for DSADC read gate */
typedef union u_pic_dsadc_read_gate_trigger_sel
{
    uint32_t dsadcrega;

    /** Struct contain bit field of DSADC Read Gate Trigger Selection Control Register k (k = 0 to 1, 4 to 6)  */
    struct st_pic_dsadc_read_gate_trigger_sel
    {
    	uint16_t dsadcrega0800 : 9;   ///< Selects the GTM output and GTM TIM[i]_CH[x] pins as a read gate source for DSADC.
        uint8_t                : 7;   ///< Reserved
        uint16_t dsadcrega2416 : 9;   ///< Selects the GTM output and GTM TIM[i]_CH[x] pins as a read gate source for DSADC.
        uint8_t                : 7;   ///< Reserved
    } dsadcrega_b;
} pic_dsadc_read_gate_trigger_sel_t;

/** Configure PIC2CADTCFG0 register assigns timer output of GTM (TOM, ATOM, TIO) to PIC22CADTENj00 (j = 0 to 1).  */
typedef union u_pic_cadc_sta_sto_trigger_out_cfg
{
    uint32_t cadcstasto;

    /**  Struct contain bit field of CADC Start / Stop Trigger Output Configuration Register */
    struct st_pic_cadc_sta_sto_trigger_out_cfg
    {
    	uint16_t cadcstasto0800 : 9;  ///< Selects the GTM output as a PIC22CADTENj00 (j = 0 to 1) Bit 0 register configuration.
        uint8_t                 : 7;  ///< Reserved
        uint16_t cadcstasto2416 : 9;  ///< Selects the GTM output as a PIC22CADTENj00 (j = 0 to 1) Bit 1 register configuration.
        uint8_t                 : 7;  ///< Reserved
    } cadcstasto_b;
} pic_cadc_sta_sto_trigger_out_cfg_t;

/** Configure PIC2CADTSEN0 register selects a trigger for CADC00 read gate.  */
typedef union u_pic_cadc_read_gate_trigger_sel
{
    uint32_t cadcrega;

    /** Struct contain bit field of CADC Read Gate Trigger Selection Control Register 0 */
    struct st_pic_cadc_read_gate_trigger_sel
    {
        uint16_t cadcrega0800 : 9;   ///< Selects the GTM output as a read gate source for CADC00.
        uint32_t              : 22;  ///< Reserved
    } cadcrega_b;
} pic_cadc_read_gate_trigger_sel_t;

/** Selects the signal to output to TAUD0TTINm output pin. */
typedef union u_pic_taud0_ttinm
{
    uint32_t taud0sel;

    /** Struct contain bit field of TAUD0 input select register */
    struct st_pic_taud0_ttinm_b
    {
        pic_taud0ttin0_sel_t  taud0sel00 : 2; ///< Selects the signal to output to TAUD0TTIN 0 output pin.
        pic_taud0ttin1_sel_t  taud0sel01 : 2; ///< Selects the signal to output to TAUD0TTIN 1 output pin.
        pic_taud0ttin2_sel_t  taud0sel02 : 2; ///< Selects the signal to output to TAUD0TTIN 2 output pin.
        pic_taud0ttin3_sel_t  taud0sel03 : 2; ///< Selects the signal to output to TAUD0TTIN 3 output pin.
        pic_taud0ttin4_sel_t  taud0sel04 : 2; ///< Selects the signal to output to TAUD0TTIN 4 output pin.
        pic_taud0ttin5_sel_t  taud0sel05 : 2; ///< Selects the signal to output to TAUD0TTIN 5 output pin.
        pic_taud0ttin6_sel_t  taud0sel06 : 2; ///< Selects the signal to output to TAUD0TTIN 6 output pin.
        pic_taud0ttin7_sel_t  taud0sel07 : 2; ///< Selects the signal to output to TAUD0TTIN 7 output pin.
        pic_taud0ttin8_sel_t  taud0sel08 : 2; ///< Selects the signal to output to TAUD0TTIN 8 output pin.
        pic_taud0ttin9_sel_t  taud0sel09 : 2; ///< Selects the signal to output to TAUD0TTIN 9 output pin.
        pic_taud0ttin10_sel_t taud0sel10 : 2; ///< Selects the signal to output to TAUD0TTIN 10 output pin.
        pic_taud0ttin11_sel_t taud0sel11 : 2; ///< Selects the signal to output to TAUD0TTIN 11 output pin.
        pic_taud0ttin12_sel_t taud0sel12 : 2; ///< Selects the signal to output to TAUD0TTIN 12 output pin.
        pic_taud0ttin13_sel_t taud0sel13 : 2; ///< Selects the signal to output to TAUD0TTIN 13 output pin.
        pic_taud0ttin14_sel_t taud0sel14 : 2; ///< Selects the signal to output to TAUD0TTIN 14 output pin.
        pic_taud0ttin15_sel_t taud0sel15 : 2; ///< Selects the signal to output to TAUD0TTIN 15 output pin.
    } taud0sel_b;
} pic_taud0_ttinm_t;

/** Selects the signal to output to TAUD1TTINm output pin. */
typedef union u_pic_taud1_ttinm
{
    uint32_t taud1sel;

    /** Struct contain bit field of TAUD1 input select register */
    struct st_pic_taud1_ttinm_b
    {
        pic_taud1ttin0_sel_t  taud1sel00 : 2; ///< Selects the signal to output to TAUD1TTIN 0 output pin.
        pic_taud1ttin1_sel_t  taud1sel01 : 2; ///< Selects the signal to output to TAUD1TTIN 1 output pin.
        pic_taud1ttin2_sel_t  taud1sel02 : 2; ///< Selects the signal to output to TAUD1TTIN 2 output pin.
        pic_taud1ttin3_sel_t  taud1sel03 : 2; ///< Selects the signal to output to TAUD1TTIN 3 output pin.
        pic_taud1ttin4_sel_t  taud1sel04 : 2; ///< Selects the signal to output to TAUD1TTIN 4 output pin.
        pic_taud1ttin5_sel_t  taud1sel05 : 2; ///< Selects the signal to output to TAUD1TTIN 5 output pin.
        pic_taud1ttin6_sel_t  taud1sel06 : 2; ///< Selects the signal to output to TAUD1TTIN 6 output pin.
        pic_taud1ttin7_sel_t  taud1sel07 : 2; ///< Selects the signal to output to TAUD1TTIN 7 output pin.
        pic_taud1ttin8_sel_t  taud1sel08 : 2; ///< Selects the signal to output to TAUD1TTIN 8 output pin.
        pic_taud1ttin9_sel_t  taud1sel09 : 2; ///< Selects the signal to output to TAUD1TTIN 9 output pin.
        pic_taud1ttin10_sel_t taud1sel10 : 2; ///< Selects the signal to output to TAUD1TTIN 10 output pin.
        pic_taud1ttin11_sel_t taud1sel11 : 2; ///< Selects the signal to output to TAUD1TTIN 11 output pin.
        pic_taud1ttin12_sel_t taud1sel12 : 2; ///< Selects the signal to output to TAUD1TTIN 12 output pin.
        pic_taud1ttin13_sel_t taud1sel13 : 2; ///< Selects the signal to output to TAUD1TTIN 13 output pin.
        pic_taud1ttin14_sel_t taud1sel14 : 2; ///< Selects the signal to output to TAUD1TTIN 14 output pin.
        pic_taud1ttin15_sel_t taud1sel15 : 2; ///< Selects the signal to output to TAUD1TTIN 15 output pin.
    } taud1sel_b;
} pic_taud1_ttinm_t;

#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)

/** Selects the signal to output to TAUD2TTINm output pin. */
typedef union u_pic_taud2_ttinm
{
    uint32_t taud2sel;

    /** Struct contain bit field of TAUD1 input select register */
    struct st_pic_taud2_ttinm_b
    {
        pic_taud2ttin0_sel_t  taud2sel00 : 2; ///< Selects the signal to output to TAUD2TTIN 0 output pin.
        pic_taud2ttin1_sel_t  taud2sel01 : 2; ///< Selects the signal to output to TAUD2TTIN 1 output pin.
        pic_taud2ttin2_sel_t  taud2sel02 : 2; ///< Selects the signal to output to TAUD2TTIN 2 output pin.
        pic_taud2ttin3_sel_t  taud2sel03 : 2; ///< Selects the signal to output to TAUD2TTIN 3 output pin.
        pic_taud2ttin4_sel_t  taud2sel04 : 2; ///< Selects the signal to output to TAUD2TTIN 4 output pin.
        pic_taud2ttin5_sel_t  taud2sel05 : 2; ///< Selects the signal to output to TAUD2TTIN 5 output pin.
        pic_taud2ttin6_sel_t  taud2sel06 : 2; ///< Selects the signal to output to TAUD2TTIN 6 output pin.
        pic_taud2ttin7_sel_t  taud2sel07 : 2; ///< Selects the signal to output to TAUD2TTIN 7 output pin.
        pic_taud2ttin8_sel_t  taud2sel08 : 2; ///< Selects the signal to output to TAUD2TTIN 8 output pin.
        pic_taud2ttin9_sel_t  taud2sel09 : 2; ///< Selects the signal to output to TAUD2TTIN 9 output pin.
        pic_taud2ttin10_sel_t taud2sel10 : 2; ///< Selects the signal to output to TAUD2TTIN 10 output pin.
        pic_taud2ttin11_sel_t taud2sel11 : 2; ///< Selects the signal to output to TAUD2TTIN 11 output pin.
        pic_taud2ttin12_sel_t taud2sel12 : 2; ///< Selects the signal to output to TAUD2TTIN 12 output pin.
        pic_taud2ttin13_sel_t taud2sel13 : 2; ///< Selects the signal to output to TAUD2TTIN 13 output pin.
        pic_taud2ttin14_sel_t taud2sel14 : 2; ///< Selects the signal to output to TAUD2TTIN 14 output pin.
        pic_taud2ttin15_sel_t taud2sel15 : 2; ///< Selects the signal to output to TAUD2TTIN 15 output pin.
    } taud2sel_b;
} pic_taud2_ttinm_t;
#else

/** Selects the signal to output to TAUD2TTINm output pin. */
typedef union u_pic_taud2_ttinm
{
    uint16_t taud2sel;

    /** Struct contain bit field of TAUD1 input select register */
    struct st_pic_taud2_ttinm_b
    {
        pic_taud2ttin0_sel_t  taud2sel00 : 1; ///< Selects the signal to output to TAUD2TTIN 0 output pin.
        pic_taud2ttin1_sel_t  taud2sel01 : 1; ///< Selects the signal to output to TAUD2TTIN 1 output pin.
        pic_taud2ttin2_sel_t  taud2sel02 : 1; ///< Selects the signal to output to TAUD2TTIN 2 output pin.
        pic_taud2ttin3_sel_t  taud2sel03 : 1; ///< Selects the signal to output to TAUD2TTIN 3 output pin.
        pic_taud2ttin4_sel_t  taud2sel04 : 1; ///< Selects the signal to output to TAUD2TTIN 4 output pin.
        pic_taud2ttin5_sel_t  taud2sel05 : 1; ///< Selects the signal to output to TAUD2TTIN 5 output pin.
        pic_taud2ttin6_sel_t  taud2sel06 : 1; ///< Selects the signal to output to TAUD2TTIN 6 output pin.
        pic_taud2ttin7_sel_t  taud2sel07 : 1; ///< Selects the signal to output to TAUD2TTIN 7 output pin.
        pic_taud2ttin8_sel_t  taud2sel08 : 1; ///< Selects the signal to output to TAUD2TTIN 8 output pin.
        pic_taud2ttin9_sel_t  taud2sel09 : 1; ///< Selects the signal to output to TAUD2TTIN 9 output pin.
        pic_taud2ttin10_sel_t taud2sel10 : 1; ///< Selects the signal to output to TAUD2TTIN 10 output pin.
        pic_taud2ttin11_sel_t taud2sel11 : 1; ///< Selects the signal to output to TAUD2TTIN 11 output pin.
        pic_taud2ttin12_sel_t taud2sel12 : 1; ///< Selects the signal to output to TAUD2TTIN 12 output pin.
        pic_taud2ttin13_sel_t taud2sel13 : 1; ///< Selects the signal to output to TAUD2TTIN 13 output pin.
        pic_taud2ttin14_sel_t taud2sel14 : 1; ///< Selects the signal to output to TAUD2TTIN 14 output pin.
        pic_taud2ttin15_sel_t taud2sel15 : 1; ///< Selects the signal to output to TAUD2TTIN 15 output pin.
    } taud2sel_b;
} pic_taud2_ttinm_t;
#endif

/** Selects the signal to output to TAUD3TTINm output pin. */
typedef union u_pic_taud3_ttinm
{
    uint32_t taud3sel;

    /** Struct contain bit field of TAUD1 input select register */
    struct st_pic_taud3_ttinm_b
    {
        pic_taud3ttin0_sel_t  taud3sel00 : 2; ///< Selects the signal to output to TAUD3TTIN 0 output pin.
        pic_taud3ttin1_sel_t  taud3sel01 : 2; ///< Selects the signal to output to TAUD3TTIN 1 output pin.
        pic_taud3ttin2_sel_t  taud3sel02 : 2; ///< Selects the signal to output to TAUD3TTIN 2 output pin.
        pic_taud3ttin3_sel_t  taud3sel03 : 2; ///< Selects the signal to output to TAUD3TTIN 3 output pin.
        pic_taud3ttin4_sel_t  taud3sel04 : 2; ///< Selects the signal to output to TAUD3TTIN 4 output pin.
        pic_taud3ttin5_sel_t  taud3sel05 : 2; ///< Selects the signal to output to TAUD3TTIN 5 output pin.
        pic_taud3ttin6_sel_t  taud3sel06 : 2; ///< Selects the signal to output to TAUD3TTIN 6 output pin.
        pic_taud3ttin7_sel_t  taud3sel07 : 2; ///< Selects the signal to output to TAUD3TTIN 7 output pin.
        pic_taud3ttin8_sel_t  taud3sel08 : 2; ///< Selects the signal to output to TAUD3TTIN 8 output pin.
        pic_taud3ttin9_sel_t  taud3sel09 : 2; ///< Selects the signal to output to TAUD3TTIN 9 output pin.
        pic_taud3ttin10_sel_t taud3sel10 : 2; ///< Selects the signal to output to TAUD3TTIN 10 output pin.
        pic_taud3ttin11_sel_t taud3sel11 : 2; ///< Selects the signal to output to TAUD3TTIN 11 output pin.
        pic_taud3ttin12_sel_t taud3sel12 : 2; ///< Selects the signal to output to TAUD3TTIN 12 output pin.
        pic_taud3ttin13_sel_t taud3sel13 : 2; ///< Selects the signal to output to TAUD3TTIN 13 output pin.
        pic_taud3ttin14_sel_t taud3sel14 : 2; ///< Selects the signal to output to TAUD3TTIN 14 output pin.
        pic_taud3ttin15_sel_t taud3sel15 : 2; ///< Selects the signal to output to TAUD3TTIN 15 output pin.
    } taud3sel_b;
} pic_taud3_ttinm_t;

/** Selects the signal to output to TAUJ2TTINm output pin. */
typedef union u_pic_tauj2_ttinm
{
    uint8_t tauj2sel;

    /** Struct contain bit field of TAUJ2 input select register */
    struct st_pic_tauj2_ttinm_b
    {
        pic_tauj2ttin2_sel_t tauj2ttin2 : 1; ///< Selects the signal to output to TAUj2TTIN 2 output pin.
        pic_tauj2ttin3_sel_t tauj2ttin3 : 1; ///< Selects the signal to output to TAUj2TTIN 3 output pin.
        uint32_t                        : 6; ///< Reserved.
    } tauj2sel_b;
} pic_tauj2_ttinm_t;

/** Configure PIC2DFETEDGSEL0 register sele cts an effective edge of the GTM timer output as trigger signal for DFE. */
typedef union u_pic_dfe_tim_trigger_edge_sel_com_a_ctrl
{
    uint32_t dfetedga;

    /** Struct contain bit field of DFE Timer Trigger Edge Selection Control Register */
    struct st_pic_dfe_tim_trigger_edge_sel_com_a_ctrl
    {
        pic_dfe_trigger_edge_sel_t dfetedga0  : 1;  ///< Selects the effective edge of Timer trigger CompareA0.
        pic_dfe_trigger_edge_sel_t dfetedga1  : 1;  ///< Selects the effective edge of Timer trigger CompareA1.
        pic_dfe_trigger_edge_sel_t dfetedga2  : 1;  ///< Selects the effective edge of Timer trigger CompareA2.
        pic_dfe_trigger_edge_sel_t dfetedga3  : 1;  ///< Selects the effective edge of Timer trigger CompareA3.
        pic_dfe_trigger_edge_sel_t dfetedga4  : 1;  ///< Selects the effective edge of Timer trigger CompareA4.
        pic_dfe_trigger_edge_sel_t dfetedga5  : 1;  ///< Selects the effective edge of Timer trigger CompareA5.
        pic_dfe_trigger_edge_sel_t dfetedga6  : 1;  ///< Selects the effective edge of Timer trigger CompareA6.
        pic_dfe_trigger_edge_sel_t dfetedga7  : 1;  ///< Selects the effective edge of Timer trigger CompareA7.
        pic_dfe_trigger_edge_sel_t dfetedga8  : 1;  ///< Selects the effective edge of Timer trigger CompareA8.
        pic_dfe_trigger_edge_sel_t dfetedga9  : 1;  ///< Selects the effective edge of Timer trigger CompareA9.
        pic_dfe_trigger_edge_sel_t dfetedga10 : 1;  ///< Selects the effective edge of Timer trigger CompareA10.
        pic_dfe_trigger_edge_sel_t dfetedga11 : 1;  ///< Selects the effective edge of Timer trigger CompareA11.
        pic_dfe_trigger_edge_sel_t dfetedga12 : 1;  ///< Selects the effective edge of Timer trigger CompareA12.
        pic_dfe_trigger_edge_sel_t dfetedga13 : 1;  ///< Selects the effective edge of Timer trigger CompareA13.
        pic_dfe_trigger_edge_sel_t dfetedga14 : 1;  ///< Selects the effective edge of Timer trigger CompareA14.
        pic_dfe_trigger_edge_sel_t dfetedga15 : 1;  ///< Selects the effective edge of Timer trigger CompareA15.
        pic_dfe_trigger_edge_sel_t dfetedga16 : 1;  ///< Selects the effective edge of Timer trigger CompareA16.
        pic_dfe_trigger_edge_sel_t dfetedga17 : 1;  ///< Selects the effective edge of Timer trigger CompareA17.
        pic_dfe_trigger_edge_sel_t dfetedga18 : 1;  ///< Selects the effective edge of Timer trigger CompareA18.
        pic_dfe_trigger_edge_sel_t dfetedga19 : 1;  ///< Selects the effective edge of Timer trigger CompareA19.
        uint32_t                              : 12; ///< Reserved
    } dfetedga_b;
} pic_dfe_tim_trigger_edge_sel_com_a_t;

/** Configure PIC2DFETEDGSEL1 register sele cts an effective edge of the GTM timer output as trigger signal for DFE. */
typedef union u_pic_dfe_tim_trigger_edge_sel_com_b_ctrl
{
    uint32_t dfetedgb;

    /** Struct contain bit field of DFE Timer Trigger Edge Selection Control Register */
    struct st_pic_dfe_tim_trigger_edge_sel_com_b_ctrl
    {
        pic_dfe_trigger_edge_sel_t dfetedgb0  : 1;  ///< Selects the effective edge of Timer trigger CompareB0.
        pic_dfe_trigger_edge_sel_t dfetedgb1  : 1;  ///< Selects the effective edge of Timer trigger CompareB1.
        pic_dfe_trigger_edge_sel_t dfetedgb2  : 1;  ///< Selects the effective edge of Timer trigger CompareB2.
        pic_dfe_trigger_edge_sel_t dfetedgb3  : 1;  ///< Selects the effective edge of Timer trigger CompareB3.
        pic_dfe_trigger_edge_sel_t dfetedgb4  : 1;  ///< Selects the effective edge of Timer trigger CompareB4.
        pic_dfe_trigger_edge_sel_t dfetedgb5  : 1;  ///< Selects the effective edge of Timer trigger CompareB5.
        pic_dfe_trigger_edge_sel_t dfetedgb6  : 1;  ///< Selects the effective edge of Timer trigger CompareB6.
        pic_dfe_trigger_edge_sel_t dfetedgb7  : 1;  ///< Selects the effective edge of Timer trigger CompareB7.
        pic_dfe_trigger_edge_sel_t dfetedgb8  : 1;  ///< Selects the effective edge of Timer trigger CompareB8.
        pic_dfe_trigger_edge_sel_t dfetedgb9  : 1;  ///< Selects the effective edge of Timer trigger CompareB9.
        pic_dfe_trigger_edge_sel_t dfetedgb10 : 1;  ///< Selects the effective edge of Timer trigger CompareB10.
        pic_dfe_trigger_edge_sel_t dfetedgb11 : 1;  ///< Selects the effective edge of Timer trigger CompareB11.
        pic_dfe_trigger_edge_sel_t dfetedgb12 : 1;  ///< Selects the effective edge of Timer trigger CompareB12.
        pic_dfe_trigger_edge_sel_t dfetedgb13 : 1;  ///< Selects the effective edge of Timer trigger CompareB13.
        pic_dfe_trigger_edge_sel_t dfetedgb14 : 1;  ///< Selects the effective edge of Timer trigger CompareB14.
        pic_dfe_trigger_edge_sel_t dfetedgb15 : 1;  ///< Selects the effective edge of Timer trigger CompareB15.
        pic_dfe_trigger_edge_sel_t dfetedgb16 : 1;  ///< Selects the effective edge of Timer trigger CompareB16.
        pic_dfe_trigger_edge_sel_t dfetedgb17 : 1;  ///< Selects the effective edge of Timer trigger CompareB17.
        pic_dfe_trigger_edge_sel_t dfetedgb18 : 1;  ///< Selects the effective edge of Timer trigger CompareB18.
        pic_dfe_trigger_edge_sel_t dfetedgb19 : 1;  ///< Selects the effective edge of Timer trigger CompareB19.
        uint32_t                              : 12; ///< Reserved
    } dfetedgb_b;
} pic_dfe_tim_trigger_edge_sel_com_b_t;

/** Configure PIC2DFETEDGSEL2 register sele cts an effective edge of the GTM timer output as trigger signal for DFE. */
typedef union u_pic_dfe_tim_trigger_edge_sel_com_ctrl
{
    uint32_t dfetedg;

    /** Struct contain bit field of DFE Timer Trigger Edge Selection Control Register */
    struct st_pic_dfe_tim_trigger_edge_sel_com_ctrl
    {
        pic_dfe_trigger_edge_sel_t dfetedg0   : 1;  ///< Selects the effective edge of Timer DFE Compare trigger 0.
        pic_dfe_trigger_edge_sel_t dfetedg1   : 1;  ///< Selects the effective edge of Timer DFE Compare trigger 1.
        pic_dfe_trigger_edge_sel_t dfetedg2   : 1;  ///< Selects the effective edge of Timer DFE Compare trigger 2.
        pic_dfe_trigger_edge_sel_t dfetedgfi  : 1;  ///< Selects the effective edge of Timer FIFO Capture trigger.
        uint32_t                              : 28; ///< Reserved
    } dfetedg_b;
} pic_dfe_tim_trigger_edge_sel_com_t;

/** The PIC2DFETSENk register se lects a source of a timer trig ger of DFE (k = 0 to 21). */
typedef union u_pic_dfe_tim_trigger_edge_sel_ctrl
{
    uint32_t dfetsen;

    /** Struct contain bit field of DFE Timer Trigger Selection Control Register */
    struct st_pic_dfe_tim_trigger_edge_sel_ctrl_b
    {
      	uint16_t dfetsen0800 : 9;  ///< Selects the GTM output as a trigger source for DFE.
        uint8_t              : 7;  ///< Reserved
        uint16_t dfetsen2416 : 9;  ///< Selects the GTM output as a trigger source for DFE.
        uint8_t              : 7;  ///< Reserved
    } dfetsen_b;
} pic_dfe_tim_trigger_edge_sel_ctrl_t;

/** Configure PIC2PSI5ENk register selects a source of a sync trigger of PSI5 */
typedef union u_pic_psi5ts_sync_pul_sig_sel_ctrl_ctrl
{
    uint32_t psi5en;

    /** Struct contain bit field of PSI5 Sync Output Control Register */
    struct st_pic_psi5ts_sync_pul_sig_sel_ctrl_b
    {
        uint16_t psi5en0800 : 9;  ///< Selects the GTM output as a PIC2PSI5 register configuration.
        uint8_t             : 7;  ///< Reserved
        uint16_t psi5en2416 : 9;  ///< Selects the GTM output as a PIC2PSI5 register configuration.
        uint8_t             : 7;  ///< Reserved
    } psi5en_b;
} pic_psi5ts_sync_pul_sig_sel_ctrl_t;

#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24 || BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB)
/** Configure PIC2PSI5SENk register selects a source of a sync trigger of PSI5  */
typedef union u_pic_psi5sts_sync_pul_sig_sel_ctrl_ctrl
{
    uint32_t psi5sen;

    /** Struct contain bit field of PSI5S Sync Output Control Register */
    struct st_pic_psi5sts_sync_pul_sig_sel_ctrl_b
    {
      	uint16_t psi5sen0800 : 9;  ///< Selects the GTM output as a signal source for PSI5.
        uint8_t              : 7;  ///< Reserved
        uint16_t psi5sen2416 : 9;  ///< Selects the GTM output as a signal source for PSI5.
        uint8_t              : 7;  ///< Reserved
    } psi5sen_b;
} pic_psi5sts_sync_pul_sig_sel_ctrl_t;

#elif (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22)
/** Configure PIC2PSI5SENk register selects a source of a sync trigger of PSI5  */
typedef union u_pic_psi5sts_sync_pul_sig_sel_ctrl_ctrl
{
    uint32_t psi5sen;

    /** Struct contain bit field of PSI5S Sync Output Control Register */
    struct st_pic_psi5sts_sync_pul_sig_sel_ctrl_b
    {
      	uint16_t psi5sen0700  : 8;  ///< Selects the GTM ATOM output as a signal source for PSI5-Sn.
        uint16_t psi5sen1508  : 8;  ///< Selects the GTM ATOM output as a signal source for PSI5-Sn.
        uint16_t psi5sen2316  : 8;  ///< Selects the GTM ATOM output as a signal source for PSI5-Sn.
        uint16_t psi5sen3124  : 8;  ///< Selects the GTM ATOM output as a signal source for PSI5-Sn.
    } psi5sen_b;
} pic_psi5sts_sync_pul_sig_sel_ctrl_t;
#endif

#if (!BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22)

/** Assigns timer output of GTM (TOM, ATOM, TIO, TIM) to PIC2ENCAnTSEL0 */
typedef union u_pic_enca_trigger_gtm_output_sel
{
    uint32_t encatcfg;

    /** Struct contain bit field of Encoder Timer Trigger Output Configuration Register */
    struct st_pic_enca_trigger_gtm_output_sel_b
    {
        uint16_t encatcfg0800 : 9;     ///< Selects the GTM output as a PIC2ENCAnTSEL0 Bit 3/1 register configuration.
        uint8_t               : 7;     ///< Reserved.
        uint16_t encatcfg2416 : 9;     ///< Selects the GTM output as a PIC2ENCAnTSEL0 Bit 2/0 register configuration.
        uint8_t               : 7;     ///< Reserved.
    } encatcfg_b;
} pic_enca_trigger_gtm_output_sel_t;

#else

/** Assigns timer output of GTM (TOM, ATOM, TIO, TIM) to PIC2ENCAnTSEL0 */
typedef union u_pic_enca_trigger_gtm_output_sel
{
    uint16_t encatcfg;

    /** Struct contain bit field of Encoder Timer Trigger Output Configuration Register */
    struct st_pic_enca_trigger_gtm_output_sel_b
    {
        uint8_t encatcfg0700;          ///< Selects the GTM output as a PIC2ENCAnTSEL0 Bit 3/1 register configuration.
        uint8_t encatcfg1508;          ///< Selects the GTM output as a PIC2ENCAnTSEL0 Bit 2/0 register configuration.
    } encatcfg_b;
} pic_enca_trigger_gtm_output_sel_t;
#endif

#if (BSP_FEATURE_PIC_GTM_TIM_IN_PIC24 || BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB)

/** Selects source of timer input of GTM (TIM)  */
typedef union u_pic_gtm_tim_src_sel_ctrl
{
    uint32_t gtminen;

    /** Struct contain bit field of GTM Timer Input Module (TIM) Source Select Register */
    struct st_pic_gtm_tim_src_sel_ctrl_b
    {
        pic_gtm_tim_src_sel_t gtminen0800 : 9; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[k]_INn (n = 0, 2, 4 ,6))
        uint8_t                           : 7; ///< Reserved
        pic_gtm_tim_src_sel_t gtminen2416 : 9; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[k]_INn (n = 1, 3, 5, 7))
        uint8_t                           : 7; ///< Reserved
    } gtminen_b;
} pic_gtm_tim_src_sel_ctrl_t;

/** Configure GTM TIM input trigger GTM_TIM_IN  */
typedef struct st_pic_gtm_tim_src_sel_group_ctrl
{
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in01; ///< Select trigger source for GTM_TIM[k]_IN0 and GTM_TIM[k]_IN1.
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in23; ///< Select trigger source for GTM_TIM[k]_IN2 and GTM_TIM[k]_IN3.
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in45; ///< Select trigger source for GTM_TIM[k]_IN4 and GTM_TIM[k]_IN5.
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in67; ///< Select trigger source for GTM_TIM[k]_IN6 and GTM_TIM[k]_IN7.
} pic_gtm_tim_src_sel_group_ctrl_t;

#elif (BSP_FEATURE_PIC_GTM_TIM_IN_PIC22)

/** Selects source of timer input of GTM (TIM)  */
typedef union u_pic_gtm_tim_src_sel_ctrl
{
    uint32_t gtminen;

    /** Struct contain bit field of GTM Timer Input Module (TIM) Source Select Register */
    struct st_pic_gtm_tim_src_sel_ctrl_b
    {
        pic_gtm_tim_src_sel_t gtminen0700 : 8; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[(k)]_INn (n = 0, 4))
        pic_gtm_tim_src_sel_t gtminen1508 : 8; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[(k)]_INn (n = 1, 5))
        pic_gtm_tim_src_sel_t gtminen2316 : 8; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[(k)]_INn (n = 2, 6))
        pic_gtm_tim_src_sel_t gtminen3124 : 8; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIM[(k)]_INn (n = 3, 7))
    } gtminen_b;
} pic_gtm_tim_src_sel_ctrl_t;

/** Configure GTM TIM input trigger GTM_TIM_IN  */
typedef struct st_pic_gtm_tim_src_sel_group_ctrl
{
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in03; ///< Select trigger source for GTM_TIM[k]_IN0, GTM_TIM[k]_IN1, GTM_TIM[k]_IN2 and GTM_TIM[k]_IN4.
    pic_gtm_tim_src_sel_ctrl_t gtm_tim_in47; ///< Select trigger source for GTM_TIM[k]_IN4, GTM_TIM[k]_IN5, GTM_TIM[k]_IN6 and GTM_TIM[k]_IN7.
} pic_gtm_tim_src_sel_group_ctrl_t;
#endif

#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)
/** Selects source of timer input of GTM (TIO)  */
typedef union u_pic_gtm_tio_src_sel_ctrl
{
    uint32_t gtmintioen;

    /** Struct contain bit field of GTM Timer Input Module (TIO) Source Select Register */
    struct st_pic_gtm_tio_src_sel_ctrl_b
    {
        pic_gtm_tio_src_sel_t gtmintioen0800 : 9; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TOM[k]_INn (n = 0, 2, 4 ,6))
        uint8_t                              : 7; ///< Reserved
        pic_gtm_tio_src_sel_t gtmintioen2416 : 9; ///< Selects a GTM timer input signal from peripheral IPs (GTM_TIO[k]_INn (n = 1, 3, 5, 7))
        uint8_t                              : 7; ///< Reserved
    } gtmintioen_b;
} pic_gtm_tio_src_sel_ctrl_t;

/** Configure GTM TIO input trigger GTM_TIO_IN  */
typedef struct st_pic_gtm_tio_src_sel_group_ctrl
{
    pic_gtm_tio_src_sel_ctrl_t gtm_tio_in01; ///< Select trigger source for GTM_TIO[k]_IN0 and GTM_TIO[k]_IN1.
    pic_gtm_tio_src_sel_ctrl_t gtm_tio_in23; ///< Select trigger source for GTM_TIO[k]_IN2 and GTM_TIO[k]_IN3.
    pic_gtm_tio_src_sel_ctrl_t gtm_tio_in45; ///< Select trigger source for GTM_TIO[k]_IN4 and GTM_TIO[k]_IN5.
    pic_gtm_tio_src_sel_ctrl_t gtm_tio_in67; ///< Select trigger source for GTM_TIO[k]_IN6 and GTM_TIO[k]_IN7.
} pic_gtm_tio_src_sel_group_ctrl_t;
#endif

/** Configure PIC2GTMMSPITSk struct to selects an effective edge for MSPI */
typedef union u_pic_mspi_trigger_edge_sel_ctrl
{
    uint32_t gtmmspicfg;

    /** Struct contain bit field of MSPI Trigger Edge Selection Control Register */
    struct st_pic_mspi_trigger_edge_sel_ctrl_b
    {
        pic_mspi_trigger_edge_sel_t gtmmspicfgk0 : 2;  ///< Selects an effective edge of MSPI HW trigger signal GTMMSPICFGk0.
        pic_mspi_trigger_edge_sel_t gtmmspicfgk1 : 2;  ///< Selects an effective edge of MSPI HW trigger signal GTMMSPICFGk1.
        pic_mspi_trigger_edge_sel_t gtmmspicfgk2 : 2;  ///< Selects an effective edge of MSPI HW trigger signal GTMMSPICFGk2.
        uint32_t                                 : 26; ///< Reserved
    } gtmmspicfg_b;
} pic_mspi_trigger_edge_sel_ctrl_t;

/** Selects a source of input of IO sharing CAN/GTM- Internal arbitration */
typedef union u_pic_can_gtm_io_sharing_ctrl
{
    uint32_t caniosen;

    /** Struct contain bit field of IO Sharing CAN / GTM- Internal Arbitration Selection Control Register */
    struct st_pic_can_gtm_io_sharing_ctrl_b
    {
        uint8_t caniosen0600 : 7;      ///< Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSENk[6:0].
        uint8_t              : 1;      ///< Reserved.
        uint8_t caniosen1408 : 7;      ///< Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSENk[14:8].
        uint8_t              : 1;      ///< Reserved.
        uint8_t caniosen2216 : 7;      ///< Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSENk[22:16].
        uint8_t              : 1;      ///< Reserved.
        uint8_t caniosen3024 : 7;      ///< Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSENk[30:24].
        uint8_t              : 1;      ///< Reserved.
    } caniosen_b;
} pic_can_gtm_io_sharing_ctrl_t;

/** Selects a source of input of FCMP */
typedef union u_pic_fcmp_input_sel_ctrl
{
    uint32_t fcmpisen;

    /** Struct contain bit field of FCMP Input Selection Control Register */
    struct st_pic_fcmp_input_sel_ctrl_b
    {
        uint8_t fcmpisen0700;          ///< Selects the GTM output as a signal source for FCMP in PIC2FCMPISENk[7:0]
        uint8_t fcmpisen1508;          ///< Selects the GTM output as a signal source for FCMP in PIC2FCMPISENk[15:08]
        uint8_t fcmpisen2316;          ///< Selects the GTM output as a signal source for FCMP in PIC2FCMPISENk[23:16]
        uint8_t fcmpisen3124;          ///< Selects the GTM output as a signal source for FCMP in PIC2FCMPISENk[31:24]
    } fcmpisen_b;
} pic_fcmp_input_sel_ctrl_t;

/** Selects a source of DTM input of GTM */
typedef union u_pic_gtm_abfg_out_dtm_sel_ctrl
{
    uint32_t abfgodsen;

    /** Struct contain bit field of ABFG Output to DTM Selection Control Register */
    struct st_pic_gtm_abfg_out_dtm_sel_ctrl_b
    {
        uint8_t abfgodsen0500 : 6;     ///< Select the ABFG trigger source for GTM DTM in PIC2ABFGODSENk[5:0].
        uint8_t               : 2;     ///< Reserved.
        uint8_t abfgodsen1308 : 6;     ///< Select the ABFG trigger source for GTM DTM in PIC2ABFGODSENk[13:8].
        uint8_t               : 2;     ///< Reserved.
        uint8_t abfgodsen2116 : 6;     ///< Select the ABFG trigger source for GTM DTM in PIC2ABFGODSENk[21:16].
        uint8_t               : 2;     ///< Reserved.
        uint8_t abfgodsen2924 : 6;     ///< Select the ABFG trigger source for GTM DTM in PIC2ABFGODSENk[29:24].
        uint8_t               : 2;     ///< Reserved.
    } abfgodsen_b;
} pic_gtm_abfg_out_dtm_sel_ctrl_t;

/** Selects TSG3n timer input (TSG3nIVS, TSG3nIVC, TSG3nIUS, TSG3nIUC) */
typedef union u_pic_tsg3_input0_sel_ctrl
{
    uint32_t tsg3trg0;

    /** Struct contain bit field of TSG3 Timer input 0 Register */
    struct st_pic_tsg3_input0_sel_ctrl_b
    {
        uint8_t tsg3iuc : 5;           ///< Selects the GTM output as a signal source for TSG3nIUC.
        uint8_t         : 3;           ///< Reserved.
        uint8_t tsg3ius : 5;           ///< Selects the GTM output as a signal source for TSG3nIUS.
        uint8_t         : 3;           ///< Reserved.
        uint8_t tsg3ivc : 5;           ///< Selects the GTM output as a signal source for TSG3nIVC.
        uint8_t         : 3;           ///< Reserved.
        uint8_t tsg3ivs : 5;           ///< Selects the GTM output as a signal source for TSG3nIVS.
        uint8_t         : 3;           ///< Reserved.
    } tsg3trg0_b;
} pic_tsg3_input0_sel_ctrl_t;

/** Selects TSG3n timer input (TSG3nIUP, TSG3nCLKI, TSG3nIWS, TSG3nIWC) */
typedef union u_pic_tsg3_input1_sel_ctrl
{
    uint32_t tsg3trg1;

    /** Struct contain bit field of TSG3 Timer input 1 Register */
    struct st_pic_tsg3_input1_sel_ctrl_b
    {
        uint8_t tsg3iwc                                           : 5; ///< Selects the GTM output as a signal source for TSG3nIWC.
        uint8_t                                                   : 3; ///< Reserved.
        uint8_t tsg3iws                                           : 5; ///< Selects the GTM output as a signal source for TSG3nIWS.
        uint8_t                                                   : 3; ///< Reserved.
        uint8_t tsg3clki                                          : 5; ///< Selects the GTM output as a signal source for TSG3nCLKI.
        pic_tsg3clki_gtm_out_type_sel_t tsg3clki_gtm_out_type_sel : 1; ///< Select GTM output type for TSG3nCLKI.
        uint8_t                                                   : 1; ///< Reserved.
        pic_tsg3clki_signal_src_sel_t tsg3clki_signal_src_sel     : 1; ///< Selects the a signal source for TSG3nCLKI.
        uint8_t tsg3iup                                           : 5; ///< Selects the GTM output as a signal source for TSG3nIUP.
        uint8_t                                                   : 3; ///< Reserved.
    } tsg3trg1_b;
} pic_tsg3_input1_sel_ctrl_t;

/** Selects TSG3n timer input (TSG3nIWP, TSG3nIVP) */
typedef union u_pic_tsg3_input2_sel_ctrl
{
    uint32_t tsg3trg2;

    /** Struct contain bit field of TSG3 Timer input 2 Register */
    struct st_pic_tsg3_input2_sel_ctrl_b
    {
        uint8_t tsg3ivp : 5;           ///< Selects the GTM output as a signal source for TSG3nIVP.
        uint8_t         : 3;           ///< Reserved.
        uint8_t tsg3iwp : 5;           ///< Selects the GTM output as a signal source for TSG3nIWP.
        uint32_t        : 19;          ///< Reserved.
    } tsg3trg2_b;
} pic_tsg3_input2_sel_ctrl_t;

/** Configure for TSG3 inputs from GTM */
typedef struct st_pic_tsg3_input_gtm_sel_ctrl
{
    pic_tsg3_input0_sel_ctrl_t pic_tsg3_input0_sel_ctrl; ///< Select input for TSG3nIVS, TSG3nIVC, TSG3nIUS, TSG3nIUC in PIC2TSG3nTRG0.
    pic_tsg3_input1_sel_ctrl_t pic_tsg3_input1_sel_ctrl; ///< Select input for TSG3nIUP, TSG3nCLKI, TSG3nIWS, TSG3nIWC in PIC2TSG3nTRG1.
    pic_tsg3_input2_sel_ctrl_t pic_tsg3_input2_sel_ctrl; ///< Select input for TSG3nIWP, TSG3nIVP in PIC2TSG3nTRG2.
} pic_tsg3_input_gtm_sel_ctrl_t;

/** Selects the TAUD and GTM output as a signal source for RDC3A */
typedef struct st_pic_rdc_latch_trigger_sel_ctrl
{
    uint8_t phi_latch;                 ///< Select trigger source for phi_latch_trg.
    uint8_t omega_latch;               ///< Select trigger source for omega_latch_trg.
} pic_rdc_latch_trigger_sel_ctrl_t;

/** Configure pwm phase mode and pwm active level. */
typedef struct st_pic_triangle_wave_pwm_phase
{
    pic_mode_t         mode;           ///< Enable/Disable pwm phase.
    pic_active_level_t active_level;   ///< Select active level.
} pic_triangle_wave_pwm_phase_t;

/** Configure for MSPI Trigger Selection. */
typedef struct st_pic_mspi_trigger_sel_cfg
{
    uint32_t trigger_sel;                      ///< Select MSPI HW trigger.
    pic_mspi_trigger_edge_sel_ctrl_t edge_sel; ///< Select effective edge for the one-shot pulse generation circuit.
} pic_mspi_trigger_sel_cfg_t;

/** Configure for DFE Trigger Selection. */
typedef struct st_pic_dfe_tim_trigger_edge_sel
{
    pic_dfe_tim_trigger_edge_sel_com_a_t dfe_tim_trigger_edge_sel_com_a; ///< Selects the effective edge of Timer trigger Compare A.
    pic_dfe_tim_trigger_edge_sel_com_b_t dfe_tim_trigger_edge_sel_com_b; ///< Selects the effective edge of Timer trigger Compare B.
    pic_dfe_tim_trigger_edge_sel_com_t   dfe_tim_trigger_edge_sel_com;   ///< Selects the effective edge of Timer trigger Compare
} pic_dfe_tim_trigger_edge_sel_t;

/** Configure High Accuracy Triangle Wave PWM Output Function with Dead Time for TAUD. */
typedef struct st_pic_triangle_wave_pwm
{
    pic_triangle_wave_pwm_phase_t u_phase; ///< Configure U phase/UB phase
    pic_triangle_wave_pwm_phase_t v_phase; ///< Configure V phase/VB phase
    pic_triangle_wave_pwm_phase_t w_phase; ///< Configure W phase/WB phase
} pic_triangle_wave_pwm_t;

/** Configure PWM Output Function with Dead Time for TAUD. */
typedef struct st_pic_pwm_output
{
    pic_mode_t u_phase;                ///< Configure U phase/UB phase
    pic_mode_t v_phase;                ///< Configure V phase/VB phase
    pic_mode_t w_phase;                ///< Configure W phase/WB phase
} pic_pwm_output_t;

/** Configure Delay Pulse Output Function with Dead Time for TAUD. */
typedef struct st_pic_delay_pulse_output
{
    pic_mode_t u_phase;                ///< Configure U phase/UB phase
    pic_mode_t v_phase;                ///< Configure V phase/VB phase
    pic_mode_t w_phase;                ///< Configure W phase/WB phase
} pic_delay_pulse_output_t;

/** Extended PIC configuration. */
typedef struct st_pic_extended_cfg
{
    pic_encoder_ctrl_func_t pic_encoder_control_function;
    uint32_t                pic20_adc_sg_trigger_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][BSP_FEATURE_PIC_ADC_NUMBER_SG]; ///< Configure for pic20adckntselj register.
    uint16_t                pic20_adc_sg_trigger_edge[BSP_FEATURE_PIC_ADC_NUMBER_UNIT];                               ///< Configure for pic20adcknedgssel register.
    uint16_t                pic20_adc_convert_trigger_taud_sel[BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic20adten4nj register.
    uint32_t pic_adc_convert_trigger_enca_sel[BSP_FEATURE_PIC_ADC_NUMBER_SG];                                         ///< Configure for pic20adten42j register.

#if (BSP_FEATURE_PIC_ADCK_TRIG_SEL_ATU)
    uint32_t pic_adc_convert_trigger_atu_1shot_pulse_on_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic20adten7nj register.
    uint32_t pic_adc_convert_trigger_atu_1shot_pulse_off_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic20adten8nj register.
    uint32_t pic_adc_convert_trigger_atu_c_g_dma_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic21adten5nj register.
    uint32_t pic_adc_convert_trigger_atu_d_g_compare_match_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic21adten6nj register.
    uint32_t pic_adc_convert_trigger_atu_d_compare_match_b_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic21adten7nj register.
    uint32_t pic_adc_convert_trigger_atu_d_downcounter_underflow_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic21adten8nj register.
#endif
#if (!BSP_FEATURE_PIC_ADCJ_TRIG_SEL)
    uint16_t pic21_adc_convert_trigger_taud_sel[BSP_FEATURE_PIC21_TAUD_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic21adten4nj register.
    uint32_t pic_adc_convert_trigger_gtm_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for pic20adten5nj register.
    pic_adc_convert_trigger_gtm_output_sel_t pic_adc_convert_trigger_gtm_output_sel[BSP_FEATURE_PIC_ADTCFG_NUMBER];   ///< Configure for pic2adtcfgk register.
    uint32_t pic21_adc_sg_trigger_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][BSP_FEATURE_PIC_ADC_NUMBER_SG];                ///< Configure for pic21adckntselj register.
    uint16_t pic21_adc_sg_trigger_edge[BSP_FEATURE_PIC_ADC_NUMBER_UNIT];                                              ///< Configure for pic21adcknedgsel register.
#else
    uint32_t pic_adc_convert_trigger_gtm_intgtma0int_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for PIC2ADTEN5nj register.
    uint32_t pic_adc_convert_trigger_gtm_atom_01_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for PIC2ADTEN6nj register.
    uint32_t pic_adc_convert_trigger_gtm_atom_23_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for PIC2ADTEN7nj register.
    uint32_t pic_adc_convert_trigger_gtm_mcs_sel[BSP_FEATURE_PIC_ADC_NUMBER_UNIT][
        BSP_FEATURE_PIC_ADC_NUMBER_SG];                                                                               ///< Configure for PIC2ADTEN8nj register.
#endif
#if (BSP_FEATURE_PIC_DSADC_TRIG_SEL_ATU)
    uint32_t pic_dsadc_start_trigger_atu_1shot_pulse_on_0n2_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN0n2 register.
    uint32_t pic_dsadc_start_trigger_atu_1shot_pulse_off_0n3_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN0n3 register.
    uint32_t pic_dsadc_stop_trigger_atu_1shot_pulse_on_1n2_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN1n2 register.
    uint32_t pic_dsadc_stop_trigger_atu_1shot_pulse_off_1n3_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN1n3 register.
    uint32_t pic_dsadc_start_trigger_out_ctrl_atu_c_g_compare_match_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN0n0 register.
    uint32_t pic_dsadc_start_trigger_out_ctrl_atu_d_g_compare_match_a_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN0n1 register.
    uint32_t pic_dsadc_start_trigger_out_ctrl_atu_d_compare_match_b_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN0n2 register.
    uint32_t pic_dsadc_start_trigger_atu_d_downcounter_underflow_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN0n3 register.
    uint32_t pic_dsadc_stop_trigger_out_ctrl_atu_c_g_compare_match_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN1n0 register.
    uint32_t pic_dsadc_stop_trigger_out_ctrl_atu_d_g_compare_match_a_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN1n1 register.
    uint32_t pic_dsadc_stop_trigger_out_ctrl_atu_d_compare_match_b_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN1n2 register.
    uint32_t pic_dsadc_stop_trigger_atu_d_downcounter_underflow_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC21DSADTEN1n3 register.
    uint32_t pic_dsadc_start_trigger_atu_1shot_pulse_on_0n0_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN0n0 register.
    uint32_t pic_dsadc_start_trigger_enca_0n1_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN0n1 register.
    uint32_t pic_dsadc_start_trigger_atu_1shot_pulse_on_1n0_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN1n0 register.
    uint32_t pic_dsadc_stop_trigger_enca_1n1_sel[BSP_FEATURE_PIC_DSADTCFG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG];                                                                           ///< Configure for PIC20DSADTEN1n1 register.
    pic_dsadc_read_gate_trigger_sel_t   dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_NUMBER];                        ///< Configure for PIC2DSADTSENk register.
    pic_dsadc_sta_sto_trigger_out_cfg_t dsadc_sta_sto_trigger_out_cfg[PIC_DSADTCFG_REG_NUMBER][
        BSP_FEATURE_PIC_DSADTCFG_NUMBER];                                                                               ///< Configure for PIC2DSADTCFGk register.
    uint32_t pic_dsadc_trigger_00_sel;                                                                                  ///< Selects a trigger for PIC20DSADCATSEL0.
    uint32_t pic_dsadc_trigger_01_sel;                                                                                  ///< Selects a trigger for PIC20DSADCATSEL1.
    uint32_t pic_dsadc_trigger_10_sel;                                                                                  ///< Selects a trigger for PIC21DSADCATSEL0.
    uint32_t pic_dsadc_trigger_11_sel;                                                                                  ///< Selects a trigger for PIC21DSADCATSEL1.
#endif
#if (BSP_FEATURE_PIC_CADC_TRIG_SEL_ATU)
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_on_002_sel;                                            ///< Configure for PIC22CADTEN002 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_off_003_sel;                                           ///< Configure for PIC22CADTEN003 register.
    uint32_t pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_on_002_sel;                                             ///< Configure for PIC22CADTEN102 register.
    uint32_t pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_off_003_sel;                                            ///< Configure for PIC22CADTEN103 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_c_g_compare_match_sel;                                               ///< Configure for PIC23CADTEN000 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_d_g_compare_match_a_sel;                                             ///< Configure for PIC23CADTEN001 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_d_compare_match_b_sel;                                               ///< Configure for PIC23CADTEN002 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_atu_d_downcounter_underflow_sel;                                         ///< Configure for PIC23CADTEN003 register.
    uint32_t pic_cadc_stop_trigger_atu_c_g_compare_match_sel;                                                         ///< Configure for PIC23CADTEN100 register.
    uint32_t pic_cadc_stop_trigger_atu_d_g_compare_match_a_sel;                                                       ///< Configure for PIC23CADTEN101 register.
    uint32_t pic_cadc_stop_trigger_atu_d_compare_match_b_sel;                                                         ///< Configure for PIC23CADTEN102 register.
    uint32_t pic_cadc_stop_trigger_atu_d_downcounter_underflow_sel;                                                   ///< Configure for PIC23CADTEN103 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_gtm_000_sel;                                                             ///< Configure for PIC22CADTEN000 register.
    uint32_t pic_cadc_start_trigger_out_ctrl_enca_100_sel;                                                            ///< Configure for PIC22CADTEN001 register.
    uint32_t pic_cadc_stop_trigger_out_ctrl_gtm_100_sel;                                                              ///< Configure for PIC22CADTEN100 register.
    uint32_t pic_cadc_stop_trigger_out_ctrl_enca_101_sel;                                                             ///< Configure for PIC22CADTEN101 register.
    uint32_t pic_cadc_trigger_00_sel;                                                                                 ///< Selects a trigger for PIC22CADCATSEL0.
    uint32_t pic_cadc_trigger_01_sel;                                                                                 ///< Selects a trigger for PIC22CADCATSEL1.
    uint32_t pic_cadc_trigger_10_sel;                                                                                 ///< Selects a trigger for PIC23CADCATSEL0.
    uint32_t pic_cadc_trigger_11_sel;                                                                                 ///< Selects a trigger for PIC23CADCATSEL1.
    pic_cadc_sta_sto_trigger_out_cfg_t cadc_sta_sto_trigger_out_cfg[PIC_CADTCFG_REG_NUMBER];                          ///< Configure for PIC2CADTCFG0 register.
    pic_cadc_read_gate_trigger_sel_t   cadc_read_gate_trigger_sel;                                                    ///< Configure for PIC2CADTSEN0 register.
#endif
#if (BSP_FEATURE_PIC_EMU_FEATURE)
    pic_emu_input_source_select_t pic_emu_input_source_select;                                                        ///< Configure for pic2emuisen0 register.
    pic_mode_t pic_emu_peak_trough_interrupt_adc_trigger_sel_enable;                                                  ///< Enable/Disable EMU Peak/Trough Interrupt and A/D Conversion Trigger Selection Function.
    uint32_t   pic_emu_input_selection_control[PIC_EMU3S_NUMBER_UNIT];                                                ///< Configure for pic2emuisenn register.
#endif
#if (BSP_FEATURE_PIC_SWITCH_TSG_OUT)
    uint8_t pic_switch_tsg_output_low_high_level_select[BSP_FEATURE_PIC_TSG3_NUMBER_UNIT];                            ///< Configure for pic1hlseln register.
    uint8_t pic_switch_tsg_output_control[BSP_FEATURE_PIC_TSG3_NUMBER_UNIT];                                          ///< Configure for pic1tsgoutctrn register.
#endif
    pic_mode_t pic10_simultaneous_start_trigger;                                                                      ///< Generates a start trigger for the timer of PIC10 for which simultaneous start is enabled.
    uint16_t   pic1_simultaneous_start_control_00;                                                                    ///< Configure for pic1sser00 register.
    uint16_t   pic1_simultaneous_start_control_01;                                                                    ///< Configure for pic1sser01 register.
    uint16_t   pic1_simultaneous_start_control_02;                                                                    ///< Configure for pic1sser02 register.
    pic_mode_t pic12_simultaneous_start_trigger;                                                                      ///< Generates a start trigger for the timer of PIC12 for which simultaneous start is enabled.
    uint16_t   pic1_simultaneous_start_control_20;                                                                    ///< Configure for pic1sser20 register.
#if (BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST0 || BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST)
    uint16_t pic1_simultaneous_start_control_03;                                                                      ///< Configure for pic1sser03 register.
#endif
#if (BSP_FEATURE_PIC_SST_PIC1_MULTI_UNIT)
    pic_mode_t                 pic11_simultaneous_start_trigger;                                                      ///< Generates a start trigger for the timer of PIC11 for which simultaneous start is enabled.
    pic_mode_t                 pic10_simultaneous_start_trigger_to_pic11_pic12;                                       ///< Enable output of simultaneous start triggers from PIC10 unit to PIC11 and PIC12 units.
    pic_mode_t                 pic11_simultaneous_start_trigger_to_pic10_pic12;                                       ///< Enable output of simultaneous start triggers from PIC11 unit to PIC10 and PIC12 units.
    pic_start_trigger_output_t pic1_start_trigger_output;                                                             ///< Enable output of simultaneous start triggers from one PIC1 unit to the other.
    uint16_t pic1_simultaneous_start_control_10;                                                                      ///< Configure for pic1sser10 register.
    uint16_t pic1_simultaneous_start_control_11;                                                                      ///< Configure for pic1sser11 register.
    uint16_t pic1_simultaneous_start_control_12;                                                                      ///< Configure for pic1sser12 register.
#endif
#if (BSP_FEATURE_PIC_TSG_START_TRIG)
    pic_tsg_start_trigger_sel_t pic_tsg30_start_trigger_select;                                                       ///< Selects a start trigger for TSG30.
    pic_tsg_start_trigger_sel_t pic_tsg31_start_trigger_select;                                                       ///< Selects a start trigger for TSG31.
 #if (BSP_FEATURE_PIC_TSG_START_TRIG_TSG32)
    pic_tsg_start_trigger_sel_t pic_tsg32_start_trigger_select;                                                       ///< Selects a start trigger for TSG32.
 #endif
#endif
    pic_tsg_sync_start_clear_sel_t pic_tsg_sync_start_clear_select;                                                   ///< Configure for pic1selbsser register.
    pic_tsg3opci0_input_signal_t   pic_tsg30opci0_input_signal;                                                       ///< Select input signal for TSG30OPCI0
    pic_tsg3opci0_input_signal_t   pic_tsg31opci0_input_signal;                                                       ///< Select input signal for TSG31OPCI0
    pic_mode_t                   pic_tsg30_three_phase_pulse_input_control;                                           ///< Enable/Disable TSG30 Three-Phase Pulse Input Control Function.
    pic_mode_t                   pic_tsg31_three_phase_pulse_input_control;                                           ///< Enable/Disable TSG31 Three-Phase Pulse Input Control Function.
    pic_mode_t                   pic_tsg30_three_phase_encoder_control;                                               ///< Enable/Disable TSG30 Three-Phase Encoder Control Function.
    pic_mode_t                   pic_tsg31_three_phase_encoder_control;                                               ///< Enable/Disable TSG31 Three-Phase Encoder Control Function.
    uint8_t                      pic_hiz_control_00;                                                                  ///< Configure for pic1hizcen00 register.
    uint8_t                      pic_hiz_control_01;                                                                  ///< Configure for pic1hizcen01 register.
    uint8_t                      pic_hiz_control_02;                                                                  ///< Configure for pic1hizcen02 register.
    uint8_t                      pic_hiz_control_03;                                                                  ///< Configure for pic1hizcen03 register.
    pic_triangle_wave_pwm_t      pic_taud_triangle_wave_pwm[BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER];               ///< Configure High Accuracy Triangle Wave PWM Output Function with Dead Time for TAUD.
    pic_pwm_output_t             pic_taud_pwm_output[BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER];                         ///< Configure PWM Output Function with Dead Time for TAUD.
    pic_delay_pulse_output_t     pic_taud_delay_pulse_output[BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER];                ///< Configure Delay Pulse Output Function with Dead Time for TAUD.
    pic_taud_int_signal_output_t pic_taud_int_signal_output[BSP_FEATURE_PIC_TAUD_INT_SIGNAL_OUT_NUMBER];              ///< Select the TAUD channel used by TAPA TSIM and TUDCM.
#if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN1)
    uint8_t pic_hiz_control_10;                                                                                       ///< Configure for pic1hizcen10 register.
    uint8_t pic_hiz_control_12;                                                                                       ///< Configure for pic1hizcen12 register.
#endif
    pic_enc_cap_trig_t                pic_enc_cap_trig_enca0;                                                         ///< Storage for the enca0 input capture trigger selection.
    pic_enc_cap_trig_t                pic_enc_cap_trig_enca1;                                                         ///< Storage for the enca1 input capture trigger selection.
    pic_enc_cap_trig_encai_inttaudi_t pic_enc_cap_trig_enca0_inttaud0i;                                               ///< Storage for the selecty INTTAUD0Im or ENCA0I1 as input for enca0 and enca1 input capture trigger selection.
    pic_enc_cap_trig_encai_inttaudi_t pic_enc_cap_trig_enca1_inttaud1i;                                               ///< Storage for the selecty INTTAUD1Im or ENCA1I1 as input for enca0 and enca1 input capture trigger selection.
    pic_mode_t pic_trigger_pulse_width_measure;                                                                       ///< Enable/Disable Trigger Pulse Width Measurement Function.
    uint32_t   pic_trigger_pulse_width_measure_sel;                                                                   ///< Select trigger signal output from ENCAn and TPTM PEn to TAUDn/TAUJn.
    pic_taud2_out_monitor_sel_t pic_taud2_monitor_sel;                                                                ///< Selects the signal to be monitored by TAUD2.
    uint32_t pic_enca_trigger_sel[BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT];                                          ///< Selects a trigger for ENCAnTTIN0.
    pic_enca_trigger_gtm_output_sel_t pic_enca_trigger_config[PIC_ENCATCFG_REG_NUMBER];                               ///< Configure for PIC2ENCATCFG register.
    pic_gtm_tim_src_sel_group_ctrl_t  pic_gtm_tim_src_sel[BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP];                      ///< Configure for PIC2GTMINEN registers.
#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)
    pic_gtm_tio_src_sel_group_ctrl_t  pic_gtm_tio_src_sel[BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP];                   ///< Configure for PIC2GTMINTIOEN registers.
#endif
#if (BSP_FEATURE_PIC_ENCA_INPUT_SEL)
    pic_mode_t                        pic_enca_input_sel;                                                             ///< Enable/Disable ENCA input select function.
    pic_enca_input_sel_unit_t         pic_enca0_input_sel_unit;                                                       ///< Select unit to input to ENCAT0E0, ENCAT0E1 and ENCAT0EC via unit 3.
    pic_enca_input_sel_unit_t         pic_enca1_input_sel_unit;                                                       ///< Select unit to input to ENCAT1E0, ENCAT1E1 and ENCAT1EC via unit 3.
    pic_enca_unit1_input_signal_sel_t pic_enca_unit1_input_signal_sel;                                                ///< Select the signal to input to ENCAT0E0, ENCAT0E1, and ENCAT0EC in unit 1
    pic_enca_unit2_input_signal_sel_t pic_enca_unit2_input_signal_sel;                                                ///< Select the signal to input to ENCAT0E0, ENCAT0E1, and ENCAT0EC in unit 2
#endif
#if (BSP_FEATURE_PIC_FCMP_FEATURE)
    pic_fcmp_input_sel_ctrl_t pic_fcmp_input_sel_ctrl[BSP_FEATURE_PIC_FCMPISEN_NUMBER_REG];                           ///< Configure PIC2FCMPISEN to selects a source of input of FCMP.
    uint32_t pic_fcmp_output_signal_sel;                                                                              ///< Select FCMP comparator output data
    pic_fcmp_output_low_level_ctrl_t pic_fcmp_output_low_level_ctrl;                                                  ///< Selects low level of FCMP comparator output data as the sources input of HRPWM Hi-Z Control Function
    pic_fcmp_output_level_tsg_ctrl_t pic_fcmp_output_high_level_tsg_ctrl;                                             ///< Selects high level of FCMP comparator output data as the sources input of HRPWM High Low Fix Control Function and Hi-Z Control Function of PIC1.
    pic_fcmp_output_level_tsg_ctrl_t pic_fcmp_output_low_level_tsg_ctrl;                                              ///< Selects low level of FCMP comparator output data as the sources input of HRPWM High Low Fix Control Function and Hi-Z Control Function of PIC1.
    pic_fcmp_output_gtm_ctrl_t       pic_fcmp_output_gtm_ctrl[PIC_GTMSEN_REG_NUMBER];                                 ///< Selects the FCMP comparator output data as a signal source for GTM.
#endif
#if (BSP_FEATURE_PIC_HRPWM_FEATURE)
    pic_mode_t pic_hrpwm_highlow_fix_start_tsg32;                                                                     ///< Enable/Disable HRPWM High Low Fix Control Start Trigger for TSG32.
    pic_mode_t pic_hrpwm_highlow_fix_start_tsg31;                                                                     ///< Enable/Disable HRPWM High Low Fix Control Start Trigger for TSG31.
    pic_mode_t pic_hrpwm_hiz_start_atom21;                                                                            ///< Enable/Disable HRPWM Hi-Z Control Start Trigger for ATOM2_IN1_HRPWM_HIZ.
    pic_mode_t pic_hrpwm_hiz_start_atom20;                                                                            ///< Enable/Disable HRPWM Hi-Z Control Start Trigger for ATOM2_IN0_HRPWM_HIZ.
    pic_mode_t pic_hrpwm_hiz_start_atom11;                                                                            ///< Enable/Disable HRPWM Hi-Z Control Start Trigger for ATOM1_IN1_HRPWM_HIZ.
    pic_mode_t pic_hrpwm_hiz_start_atom10;                                                                            ///< Enable/Disable HRPWM Hi-Z Control Start Trigger for ATOM1_IN0_HRPWM_HIZ.
    uint32_t   pic_hrpwm_hiz_fcmp_ctrl;                                                                               ///< Selects Hi-Z control input signals of FCMP comparator output data for HRPWM.
#endif
    pic_taud0_ttinm_t pic_taud0_ttinm;                                                                                ///< Configure for TAUD0SEL.
    pic_taud1_ttinm_t pic_taud1_ttinm;                                                                                ///< Configure for TAUD1SEL.
    pic_taud2_ttinm_t pic_taud2_ttinm;                                                                                ///< Configure for TAUD2SEL.
#if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)
    pic_taud3_ttinm_t pic_taud3_ttinm;                                                                                ///< Configure for TAUD3SEL.
#elif (BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ)
    pic_tauj2_ttinm_t          pic_tauj2_ttinm;                                                                       ///< Configure for TAUJ2TTINm Input Signal Selection Register.
    uint8_t                    pic_tauj3_ttinm;                                                                       ///< Configure for TAUJ3TTINm Input Signal Selection Register.
    pic_tauj2_in_monitor_sel_t pic_tauj2_in_monitor_sel;                                                              ///< Selects the signal to be monitored by TAUJ2.
#endif
#if (BSP_FEATURE_PIC_DFE_TRIG_SEL)
    pic_dfe_tim_trigger_edge_sel_t pic_dfe_trigger_sel;                                                               ///< Configure for DFE Trigger Selection Control Function
    pic_dfe_tim_trigger_edge_sel_ctrl_t pic_dfe_tim_trigger_sel[BSP_FEATURE_PIC_DFETSEN_NUMBER];                      ///< Configure for DFE Timer Trigger Selection Control Function
#endif
#if (BSP_FEATURE_PIC_PSI5_TRIG_SEL)
    pic_psi5ts_sync_pul_sig_sel_ctrl_t pic_psi5ts_sync_pul_sig_sel[BSP_FEATURE_PIC_PSI5EN_NUMBER_UNIT][
    BSP_FEATURE_PIC_PSI5EN_NUMBER];                                                                                   ///< Configure for PIC2PSI5EN registers.
#endif
#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL)
    pic_psi5sts_sync_pul_sig_sel_ctrl_t pic_psi5sts_sync_pul_sig_sel[BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT][
    BSP_FEATURE_PIC_PSI5SEN_NUMBER];                                                                                  ///< Configure for PIC2PSI5SEN registers.
#endif
#if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL)
    pic_enca_input_connect_sel_t pic_enca_input_connect_sel;                                                          ///< Selects connection for input pins (ENCAnE0, ENCAnE1, ENCAnEC) of ENCAn timer
#endif
    uint8_t pic_hiz_gtm_out;                                                                                          ///< Configure for PIC2ENHIZDTM.
#if (BSP_FEATURE_PIC_RDC_FEATURE)
    pic_rdc_latch_trigger_sel_ctrl_t pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_NUMBER_UNIT];                ///< Configure PIC2RDCAVTSEN to selects trigger source of RDC3As.
    pic_rdc_excit_input_sel_t        pic_rdc3as_excit_input_sel[PIC_RDC3A_FEATURE_NUMBER_UNIT];                       ///< Configure PIC2RDCEISEN0 to select excitation Signal Input of RDC3AS.
 #if (BSP_FEATURE_PIC_RDC_FEATURE_RDC3AL)
    pic_rdc_latch_trigger_sel_ctrl_t pic_rdc3al_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_NUMBER_UNIT];                ///< Configure PIC2RDCAVTSEN to selects trigger source of RDC3AL.
 #endif
#endif

#if (BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL)
    pic_gtm_abfg_out_dtm_sel_ctrl_t pic_gtm_abfg_out_dtm_sel_ctrl[BSP_FEATURE_PIC_ABFGODSEN_NUMBER]; ///< Configure PIC2ABFGODSEN to selects ABFG trigger source for GTM DTM.
#endif
#if (BSP_FEATURE_PIC_MSPI_TRIG_SEL)
    pic_mspi_trigger_sel_cfg_t pic_mspi_trigger_sel[BSP_FEATURE_PIC_GTMMSPI_NUMBER];                 ///< Configure for MSPI Trigger Selection Function
#endif
#if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE)
    pic_can_gtm_io_sharing_ctrl_t pic_can_gtm_io_sharing_ctrl[BSP_FEATURE_PIC_CANIOSEN_NUMBER];      ///< Configure for PIC2CANIOSEN registers.
#endif
#if (BSP_FEATURE_PIC_TAUD_INPUT_SEL)
    pic_taudtinsel_ctrl_t pic_taudtinsel_ctrl[BSP_FEATURE_PIC_TAUDTINSEL_NUMBER];                    ///< Selects the signal to output to TAUDTTINm.
#endif
#if (BSP_FEATURE_PIC_TSG3_INPUT_GTM)
    pic_tsg3_input_gtm_sel_ctrl_t pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_INPUT_FEATURE_NUMBER_UNIT];   ///< Configure for TSG3 inputs from GTM.
#endif
} pic_extended_cfg_t;

/** PIC instance control block. */
typedef struct st_pic_instance_ctrl
{
    uint32_t open;                       ///< Indicates whether the open() API has been successfully called.
#if ((BSP_FEATURE_PIC_FEATURE_U2BX))
    R_PIC10_Type * p_regs_pic10;         ///< Base register for PIC10.
    R_PIC11_Type * p_regs_pic11;         ///< Base register for PIC11.
    R_PIC12_Type * p_regs_pic12;         ///< Base register for PIC12.
    R_PIC13_Type * p_regs_pic13;         ///< Base register for PIC13.
    R_PIC20_Type * p_regs_pic20;         ///< Base register for PIC20.
    R_PIC21_Type * p_regs_pic21;         ///< Base register for PIC21.
    R_PIC22_Type * p_regs_pic22;         ///< Base register for PIC22.
 #if (BSP_FEATURE_PIC_FEATURE_U2BX)
    R_PIC23_Type * p_regs_pic23;         ///< Base register for PIC23.
 #endif
    R_PIC24_Type * p_regs_pic24;         ///< Base register for PIC24.
#elif (BSP_FEATURE_PIC_FEATURE_U2CX)
    R_PIC10_Type     * p_regs_pic10;     ///< Base register for PIC10.
    R_PIC1_SELB_Type * p_regs_pic1_selb; ///< Base register for PIC1_SELB.
    R_PIC20_Type     * p_regs_pic20;     ///< Base register for PIC20.
    R_PIC21_Type     * p_regs_pic21;     ///< Base register for PIC21.
    R_PIC22_Type     * p_regs_pic22;     ///< Base register for PIC22.
    R_PIC2_SELB_Type * p_regs_pic2_selb; ///< Base register for PIC2_SELB.
#elif (BSP_FEATURE_PIC_FEATURE_U2AX)
    R_PIC1_Type      * p_regs_pic10;     ///< Base register for PIC1.
    R_PIC1_SELB_Type * p_regs_pic1_selb; ///< Base register for PIC1_SELB.
    R_PIC20_Type     * p_regs_pic20;     ///< Base register for PIC20.
    R_PIC21_Type     * p_regs_pic21;     ///< Base register for PIC21.
    R_PIC22_Type     * p_regs_pic22;     ///< Base register for PIC22.
#endif
    pic_cfg_t const * p_cfg;             ///< Pointer to instance configuration.
} pic_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const pic_api_t g_pic_on_pic;

/** @endcond */

fsp_err_t R_PIC_Open(pic_ctrl_t * const p_api_ctrl, pic_cfg_t const * const p_cfg);
fsp_err_t R_PIC_Close(pic_ctrl_t * const p_api_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup PIC)
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /*R_PIC_H*/

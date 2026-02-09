/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_MCU_RESET_H
#define BSP_MCU_RESET_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

#include "bsp_api.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU
 * @defgroup BSP_MCU_RESET BSP MCU Reset
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Macro for MCU reset */
#define BSP_RESET_FLAG_MASK                 (0xF5DDU)                                     ///< Mask for all reset flags
#define BSP_RESET_KEYCODE_DISABLE           (0xA5A5A500)                                  ///< Keycode to disable reset register protection
#define BSP_RESET_KEYCODE_ENABLE            (0xA5A5A501)                                  ///< Keycode to enable reset register protection
#define BSP_RESET_RESKCPROT0_RESET          (0x00000000)                                  ///< Default reset value for RESKCPROT0
#define BSP_RESET_ECM_RESET_SRC_SYS         (0x00U)                                       ///< ECM reset source mapped to System Reset
#define BSP_RESET_ECM_RESET_SRC_APP         (0x01U)                                       ///< ECM reset source mapped to Application Reset
#define BSP_RESET_SOFTWARE_RESET_TRIGGEG    (0x01U)                                       ///< Trigger bit for software reset
#define BSP_RESET_HW_BIST_0_MASK            (1U << 12U)                                   ///< Mask for HW BIST 0
#define BSP_RESET_HW_BIST_1_MASK            (1U << 13U)                                   ///< Mask for HW BIST 1
#define BSP_RESET_HW_BIST_2_MASK            (1U << 14U)                                   ///< Mask for HW BIST 2

#define BSP_OPBT3_VALUE                     *((const volatile uint32_t *) (0xFF320BACUL)) ///< Pointer to OPBT3 register value
#define BSP_OPBT3_BIT_15_MASK               (1U << 15U)                                   ///< Mask for OPBT3 bit 15

/* Macro module name for Module reset */
#if (BSP_MCU_GROUP_RH850U2Ax)
 #define BSP_RESET_MODULE_RSCFD             RSCFD                                         ///< Module reset name: RSCFD
 #define BSP_RESET_MODULE_FLXA              FLXA                                          ///< Module reset name: FLXA
 #define BSP_RESET_MODULE_GTM               GTM                                           ///< Module reset name: GTM

 #if (BSP_FEATURE_ETNB_IS_AVAILABLE)
  #define BSP_RESET_MODULE_ETNB             ETNB                                          ///< Module reset name: ETNB
 #endif

 #define BSP_RESET_MODULE_RSENT             RSENT                                         ///< Module reset name: RSENT
 #define BSP_RESET_MODULE_MSPI              MSPI                                          ///< Module reset name: MSPI
 #define BSP_RESET_MODULE_RLIN3             RLIN3                                         ///< Module reset name: RLIN3
 #define BSP_RESET_MODULE_ADCJ_ISO          ADCJ_ISO                                      ///< Module reset name: ADCJ_ISO

 #if (BSP_FEATURE_CXPI_IS_AVAILABLE)
  #define BSP_RESET_MODULE_CXPI             CXPI                                          ///< Module reset name: CXPI
 #endif

 #if (BSP_FEATURE_MMCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_MMCA             MMCA                                          ///< Module reset name: MMCA
 #endif

 #if (BSP_FEATURE_ENCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_ENCA             ENCA                                          ///< Module reset name: ENCA
 #endif

 #define BSP_RESET_MODULE_PSI5              PSI5                                          ///< Module reset name: PSI5
 #define BSP_RESET_MODULE_PSI5S             PSI5S                                         ///< Module reset name: PSI5S
 #define BSP_RESET_MODULE_PWMD              PWMD                                          ///< Module reset name: PWMD

 #if (BSP_FEATURE_RHSIF_IS_AVAILABLE)
  #define BSP_RESET_MODULE_RHSIF            RHSIF                                         ///< Module reset name: RHSIF
 #endif

 #define BSP_RESET_MODULE_RIIC              RIIC                                          ///< Module reset name: RIIC

 #if (BSP_FEATURE_SCI3_IS_AVAILABLE)
  #define BSP_RESET_MODULE_SCI3             SCI3                                          ///< Module reset name: SCI3
 #endif

 #if (BSP_FEATURE_SFMA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_SFMA             SFMA                                          ///< Module reset name: SFMA
 #endif

 #define BSP_RESET_MODULE_TAPA              TAPA                                          ///< Module reset name: TAPA
 #define BSP_RESET_MODULE_TAUD              TAUD                                          ///< Module reset name: TAUD
 #define BSP_RESET_MODULE_TAUJ_ISO          TAUJ_ISO                                      ///< Module reset name: TAUJ_ISO
 #define BSP_RESET_MODULE_TPBA              TPBA                                          ///< Module reset name: TPBA
 #define BSP_RESET_MODULE_TSG3              TSG3                                          ///< Module reset name: TSG3
 #define BSP_RESET_MODULE_OSTM              OSTM                                          ///< Module reset name: OSTM

#elif (BSP_MCU_GROUP_RH850U2Bx)
 #define BSP_RESET_MODULE_RSCFD             RSCFD                                         ///< Module reset name: RSCFD
 #define BSP_RESET_MODULE_FLXA              FLXA                                          ///< Module reset name: FLXA
 #define BSP_RESET_MODULE_GTM               GTM                                           ///< Module reset name: GTM
 #define BSP_RESET_MODULE_ETN               ETN                                           ///< Module reset name: ETN
 #define BSP_RESET_MODULE_RSENT             RSENT                                         ///< Module reset name: RSENT
 #define BSP_RESET_MODULE_MSPI              MSPI                                          ///< Module reset name: MSPI
 #define BSP_RESET_MODULE_RLIN3             RLIN3                                         ///< Module reset name: RLIN3
 #define BSP_RESET_MODULE_ADCK_ISO          ADCK_ISO                                      ///< Module reset name: ADCK_ISO

 #if (BSP_FEATURE_MMCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_MMCA             MMCA                                          ///< Module reset name: MMCA
 #endif

 #define BSP_RESET_MODULE_ENCA              ENCA                                          ///< Module reset name: ENCA

 #if (BSP_FEATURE_PSI5_IS_AVAILABLE)
  #define BSP_RESET_MODULE_PSI5             PSI5                                          ///< Module reset name: PSI5
 #endif

 #if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
  #define BSP_RESET_MODULE_PSI5S            PSI5S                                         ///< Module reset name: PSI5S
 #endif

 #if (BSP_FEATURE_RHSIF_IS_AVAILABLE)
  #define BSP_RESET_MODULE_RHSIF            RHSIF                                         ///< Module reset name: RHSIF
 #endif

 #if (BSP_FEATURE_RIIC_IS_AVAILABLE)
  #define BSP_RESET_MODULE_RIIC             RIIC                                          ///< Module reset name: RIIC
 #endif

 #define BSP_RESET_MODULE_SFMA              SFMA                                          ///< Module reset name: SFMA
 #define BSP_RESET_MODULE_TAPA              TAPA                                          ///< Module reset name: TAPA
 #define BSP_RESET_MODULE_TAUD              TAUD                                          ///< Module reset name: TAUD
 #define BSP_RESET_MODULE_TPBA              TPBA                                          ///< Module reset name: TPBA
 #define BSP_RESET_MODULE_TSG3              TSG3                                          ///< Module reset name: TSG3
 #define BSP_RESET_MODULE_OSTM              OSTM                                          ///< Module reset name: OSTM
 #define BSP_RESET_MODULE_DSADC_CADC        DSADC_CADC                                    ///< Module reset name: DSADC_CADC
 #define BSP_RESET_MODULE_RHSB              RHSB                                          ///< Module reset name: RHSB
 #define BSP_RESET_MODULE_ATU               ATU                                           ///< Module reset name: ATU

 #if (BSP_FEATURE_EMU_IS_AVAILABLE)
  #define BSP_RESET_MODULE_EMU              EMU                                           ///< Module reset name: EMU
 #endif

 #define BSP_RESET_MODULE_PE                PE                                            ///< Module reset name: PE
 #define BSP_RESET_MODULE_FCOMP             FCOMP                                         ///< Module reset name: FCOMP
 #define BSP_RESET_MODULE_RDC               RDC                                           ///< Module reset name: RDC

 #if (BSP_FEATURE_DFP_IS_AVAILABLE)
  #define BSP_RESET_MODULE_DFP              DFP                                           ///< Module reset name: DFP
 #endif

 #define BSP_RESET_MODULE_DFE               DFE                                           ///< Module reset name: DFE
 #define BSP_RESET_MODULE_WDTB_ISO          WDTB_ISO                                      ///< Module reset name: WDTB_ISO
 #define BSP_RESET_MODULE_HRPWM             HRPWM                                         ///< Module reset name: HRPWM

 #if (BSP_FEATURE_DSMIF_IS_AVAILABLE)
  #define BSP_RESET_MODULE_DSMIF            DSMIF                                         ///< Module reset name: DSMIF
 #endif

#elif (BSP_MCU_GROUP_RH850U2Cx)
 #define BSP_RESET_MODULE_RSCFD             RSCFD                                         ///< Module reset name: RSCFD

 #if (BSP_FEATURE_FLXA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_FLXA             FLXA                                          ///< Module reset name: FLXA
 #endif

 #define BSP_RESET_MODULE_GTM               GTM                                           ///< Module reset name: GTM

 #if (BSP_FEATURE_ETND_IS_AVAILABLE)
  #define BSP_RESET_MODULE_ETND             ETND                                          ///< Module reset name: ETND
 #endif

 #define BSP_RESET_MODULE_RSENT             RSENT                                         ///< Module reset name: RSENT
 #define BSP_RESET_MODULE_MSPI              MSPI                                          ///< Module reset name: MSPI
 #define BSP_RESET_MODULE_RLIN3             RLIN3                                         ///< Module reset name: RLIN3
 #define BSP_RESET_MODULE_ADCK_ISO          ADCK_ISO                                      ///< Module reset name: ADCK_ISO
 #define BSP_RESET_MODULE_CXPI              CXPI                                          ///< Module reset name: CXPI

 #if (BSP_FEATURE_MMCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_MMCA             MMCA                                          ///< Module reset name: MMCA
 #endif

 #define BSP_RESET_MODULE_ENCA              ENCA                                          ///< Module reset name: ENCA
 #define BSP_RESET_MODULE_PSI5              PSI5                                          ///< Module reset name: PSI5

 #if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
  #define BSP_RESET_MODULE_PSI5S            PSI5S                                         ///< Module reset name: PSI5S
 #endif

 #define BSP_RESET_MODULE_PWMD              PWMD                                          ///< Module reset name: PWMD
 #define BSP_RESET_MODULE_SFMA              SFMA                                          ///< Module reset name: SFMA
 #define BSP_RESET_MODULE_TAPA              TAPA                                          ///< Module reset name: TAPA
 #define BSP_RESET_MODULE_TAUD              TAUD                                          ///< Module reset name: TAUD
 #define BSP_RESET_MODULE_TAUJ_ISO          TAUJ_ISO                                      ///< Module reset name: TAUJ_ISO
 #define BSP_RESET_MODULE_TSG3              TSG3                                          ///< Module reset name: TSG3
 #define BSP_RESET_MODULE_OSTM              OSTM                                          ///< Module reset name: OSTM
 #define BSP_RESET_MODULE_PE                PE                                            ///< Module reset name: PE
 #define BSP_RESET_MODULE_WDTB_ISO          WDTB_ISO                                      ///< Module reset name: WDTB_ISO
 #define BSP_RESET_MODULE_CANXL             CANXL                                         ///< Module reset name: CANXL
 #define BSP_RESET_MODULE_RI3C              RI3C                                          ///< Module reset name: RI3C
 #define BSP_RESET_MODULE_SSIF              SSIF                                          ///< Module reset name: SSIF
 #define BSP_RESET_MODULE_ETNF              ETNF                                          ///< Module reset name: ETNF

#else /* BSP_MCU_GROUP_RH850U2BxE */
 #define BSP_RESET_MODULE_RSCFD             RSCFD                                         ///< Module reset name: RSCFD
 #define BSP_RESET_MODULE_FLXA              FLXA                                          ///< Module reset name: FLXA
 #define BSP_RESET_MODULE_GTM               GTM                                           ///< Module reset name: GTM
 #define BSP_RESET_MODULE_ETN               ETN                                           ///< Module reset name: ETN
 #define BSP_RESET_MODULE_RSENT             RSENT                                         ///< Module reset name: RSENT
 #define BSP_RESET_MODULE_MSPI              MSPI                                          ///< Module reset name: MSPI
 #define BSP_RESET_MODULE_RLIN3             RLIN3                                         ///< Module reset name: RLIN3
 #define BSP_RESET_MODULE_ADCK_ISO          ADCK_ISO                                      ///< Module reset name: ADCK_ISO

 #if (BSP_FEATURE_MMCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_MMCA             MMCA                                          ///< Module reset name: MMCA
 #endif

 #if (BSP_FEATURE_ENCA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_ENCA             ENCA                                          ///< Module reset name: ENCA
 #endif

 #define BSP_RESET_MODULE_PSI5              PSI5                                          ///< Module reset name: PSI5

 #if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
  #define BSP_RESET_MODULE_PSI5S            PSI5S                                         ///< Module reset name: PSI5S
 #endif

 #define BSP_RESET_MODULE_RHSIF             RHSIF                                         ///< Module reset name: RHSIF
 #define BSP_RESET_MODULE_RIIC              RIIC                                          ///< Module reset name: RIIC

 #if (BSP_FEATURE_SFMA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_SFMA             SFMA                                          ///< Module reset name: SFMA
 #endif

 #if (BSP_FEATURE_TAPA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_TAPA             TAPA                                          ///< Module reset name: TAPA
 #endif

 #if (BSP_FEATURE_TAUD_IS_AVAILABLE)
  #define BSP_RESET_MODULE_TAUD             TAUD                                          ///< Module reset name: TAUD
 #endif

 #if (BSP_FEATURE_TPBA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_TPBA             TPBA                                          ///< Module reset name: TPBA
 #endif

 #if (BSP_FEATURE_TSG3_IS_AVAILABLE)
  #define BSP_RESET_MODULE_TSG3             TSG3                                          ///< Module reset name: TSG3
 #endif

 #define BSP_RESET_MODULE_OSTM              OSTM                                          ///< Module reset name: OSTM
 #define BSP_RESET_MODULE_DSADC_CADC        DSADC_CADC                                    ///< Module reset name: DSADC_CADC
 #define BSP_RESET_MODULE_RHSB              RHSB                                          ///< Module reset name: RHSB
 #define BSP_RESET_MODULE_PE                PE                                            ///< Module reset name: PE
 #define BSP_RESET_MODULE_FCOMP             FCOMP                                         ///< Module reset name: FCOMP
 #define BSP_RESET_MODULE_RDC               RDC                                           ///< Module reset name: RDC
 #define BSP_RESET_MODULE_DFE               DFE                                           ///< Module reset name: DFE
 #define BSP_RESET_MODULE_WDTB_ISO          WDTB_ISO                                      ///< Module reset name: WDTB_ISO
 #define BSP_RESET_MODULE_HRPWM             HRPWM                                         ///< Module reset name: HRPWM

 #if (BSP_FEATURE_DFA_IS_AVAILABLE)
  #define BSP_RESET_MODULE_DFA              DFA                                           ///< Module reset name: DFA
 #endif

 #define BSP_RESET_MODULE_CANXL             CANXL                                         ///< Module reset name: CANXL
 #define BSP_RESET_MODULE_ETNF              ETNF                                          ///< Module reset name: ETNF
 #define BSP_RESET_MODULE_DSMIF             DSMIF                                         ///< Module reset name: DSMIF

 #if (BSP_FEATURE_OSPI_IS_AVAILABLE)
  #define BSP_RESET_MODULE_OSPI             OSPI                                          ///< Module reset name: OSPI
 #endif

#endif /* BSP_MCU_GROUP_RH850U2Ax */

/* Macro helper for Module reset.
 * These macros are used to support Module Reset Macro Function
 * to behave correctly */

/** Helper: concatenate register prefix and module name */
#define BSP_RESET_MODULE_HELPER_MACRO(reg, module)    reg ## module

/** Assert reset register for module */
#define BSP_RESET_MODULE_ASSERT_REG(module)           BSP_RESET_MODULE_HELPER_MACRO(SWMRESA_, module)

/** Status register for module reset */
#define BSP_RESET_MODULE_STATUS_REG(module)           BSP_RESET_MODULE_HELPER_MACRO(SWMRESS_, module)

/** Write reset bit for a module */
#define BSP_RESET_MODULE_RESET_EXPAND(module_expand, bit_expand) \
    R_SYS_RESET->BSP_RESET_MODULE_ASSERT_REG(module_expand) = (bit_expand)

/** Read reset status bit for a module */
#define BSP_RESET_MODULE_RESET_STATUS_EXPAND(module_expand, bit_expand) \
    (R_SYS_RESET->BSP_RESET_MODULE_STATUS_REG(module_expand)) & (bit_expand)

/***********************************************************************************************************************
 * Macro Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief      Macro Function Module Reset Flag. This Macro Function reads module reset status flag
 *             of a specific module channel.
 *
 * @param[in]  module  Specific module that need to be reset. (BSP_RESET_MODULE_ + {module_name})
 * @param[in]  bit     Bits of register SWMRESS that need to be read.
 *
 **********************************************************************************************************************/
#define R_BSP_MODULE_RESET_FLAG(module, bit)    BSP_RESET_MODULE_RESET_STATUS_EXPAND(module, bit)

/*******************************************************************************************************************//**
 * @brief      Macro Function Module Reset. This Macro Function triggers module reset
 *             for a specific module channel.
 *
 * @param[in]  module  Specific module that need to be reset. (BSP_RESET_MODULE_ + {module_name})
 * @param[in]  bit     Bits of register SWMRESA that need to be written.
 *
 **********************************************************************************************************************/
#define R_BSP_MODULE_RESET(module, bit)                                   \
    FSP_HARDWARE_REGISTER_WAIT(R_BSP_MODULE_RESET_FLAG(module, bit), 0U); \
    R_SYS_RESET->RESKCPROT0 = BSP_RESET_KEYCODE_ENABLE;                   \
    BSP_RESET_MODULE_RESET_EXPAND(module, bit);                           \
    R_SYS_RESET->RESKCPROT0 = BSP_RESET_KEYCODE_DISABLE;                  \
    FSP_HARDWARE_REGISTER_WAIT(R_BSP_MODULE_RESET_FLAG(module, bit), 0U);

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Reset flag information */
typedef enum e_bsp_reset
{
    BSP_RESET_POWER_ON,                ///< Power On Reset Flag
#if (BSP_RESET_SOURCE_POWER_OFF_STANDBY_IS_AVAILABLE)
    BSP_RESET_POWER_OFF_STANDBY,       ///< Power Off Standby Reset Flag
#endif
    BSP_RESET_EXTERNAL,                ///< External Reset Flag
    BSP_RESET_VMON,                    ///< VMON Reset Flag
    BSP_RESET_ECM_SYSTEM,              ///< ECM System Reset Flag
    BSP_RESET_ECM_APPLICATION,         ///< ECM Application Reset Flag
    BSP_RESET_WDT,                     ///< WDTBA Reset Flag
    BSP_RESET_SYSTEM,                  ///< Software System Reset Flag
    BSP_RESET_ICUM_SYSTEM,             ///< ICUM Software System Reset Flag
    BSP_RESET_APPLICATION,             ///< Software Application Reset Flag
    BSP_RESET_ICUM_APPLICATION,        ///< ICUM Software Application Reset Flag
#if (BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE)
    BSP_RESET_DEEPSTOP,                ///< DeepSTOP Reset Flag
#endif
    BSP_RESET_NONE                     ///< Do not have reset
} bsp_reset_t;

/** Reset flag information */
typedef enum e_bsp_chip_standby_mode
{
    BSP_CHIP_STANDBY_MODE_HW_BIST_FLAG_CLEARED,     ///< The flag is cleared before reading the RESF register
    BSP_CHIP_STANDBY_MODE_RUN_MODE,                 ///< RUN mode after Power On Reset and System Reset 1/2
#if (BSP_FEATURE_CHIP_STANDBY_DEEPSTOP_IS_AVAILABLE)
    BSP_CHIP_STANDBY_MODE_RUN_MODE_AFTER_DEEP_STOP, ///< RUN mode after DeepStop mode
    BSP_CHIP_STANDBY_MODE_CYCLIC,                   ///< CYCLIC mode after DeepStop mode
#endif
    BSP_CHIP_STANDBY_MODE_HWBIST_SKIPPED            ///< HW BIST skipped by OPBT3 bit 15
} bsp_chip_standby_mode_t;

/* Reset table information*/
typedef struct st_bsp_reset_table
{
    uint8_t     bit;                   ///< Bit position of the reset flag in the RESF register
    bsp_reset_t flag;                  ///< Corresponding reset flag
} bsp_reset_table_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** Function declarations */
bsp_reset_t             R_BSP_McuResetGetFlags(void);
bsp_chip_standby_mode_t R_BSP_GetChipStandbyMode(void);
void                    R_BSP_McuResetClearFlags(void);
void                    R_BSP_McuSoftwareSystemReset(void);
void                    R_BSP_McuSoftwareApplicationReset(void);

#if (BSP_RESET_ECM_SELECT_RESET_SOURCE_IS_AVAILABLE)
fsp_err_t R_BSP_ECMResetSourceConfig(bsp_reset_t reset_src);

#endif

/** @} (end addtogroup BSP_MCU_RESET) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

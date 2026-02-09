/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/** @} (end addtogroup BSP_MCU) */

#ifndef BSP_EXCEPTIONS_H
 #define BSP_EXCEPTIONS_H

 #ifdef __cplusplus
extern "C" {
 #endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
 #if defined(__CCRH__)

/** Pragma to support for interrupt function */
  #define BSP_INTERRUPT_ATTRIBUTE       ///< Interrupt attribute placeholder for CCRH
  #define BSP_FEINT_INTERRUPT_ATTRIBUTE ///< FEINT interrupt attribute placeholder for CCRH
  #define BSP_FENMI_INTERRUPT_ATTRIBUTE ///< FENMI interrupt attribute placeholder for CCRH

 #elif defined(__ghs__)

/** Pragma to support for interrupt function */
  #define BSP_INTERRUPT_ATTRIBUTE          __interrupt ///< Interrupt attribute for GHS compiler
  #define BSP_FEINT_INTERRUPT_ATTRIBUTE                ///< FEINT interrupt attribute placeholder for GHS
  #define BSP_FENMI_INTERRUPT_ATTRIBUTE                ///< FENMI interrupt attribute placeholder for GHS

 #elif defined(__GNUC__)

/** Pragma to support for interrupt function */
  #define BSP_INTERRUPT_ATTRIBUTE          __attribute__((interrupt))          ///< General interrupt attribute for HighTec
  #define BSP_FEINT_INTERRUPT_ATTRIBUTE    __attribute__((interrupt("FEINT"))) ///< FEINT interrupt attribute for HighTec
  #define BSP_FENMI_INTERRUPT_ATTRIBUTE    __attribute__((interrupt("FEINT"))) ///< FENMI interrupt attribute for HighTec

  #elif defined(__ICCRH850__)

/** Pragma to support for interrupt function */
  #define BSP_INTERRUPT_ATTRIBUTE          ///< General interrupt attribute for IAR
  #define BSP_FEINT_INTERRUPT_ATTRIBUTE    ///< FEINT interrupt attribute for IAR
  #define BSP_FENMI_INTERRUPT_ATTRIBUTE    ///< FENMI interrupt attribute for IAR

 #endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** This list includes only Arm standard exceptions. Renesas interrupts are defined in vector_data.h. */
typedef enum IRQn
{
    SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES = BSP_VECTOR_TABLE_MAX_ENTRIES ///< Max IRQ number
} IRQn_Type;

 #ifdef __cplusplus
}
 #endif

#endif

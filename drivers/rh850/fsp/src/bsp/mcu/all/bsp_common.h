/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * File Name    : bsp_common.h
 * Description  : This module has information about the bsp common.
 **********************************************************************************************************************/
#ifndef BSP_COMMON_H_
#define BSP_COMMON_H_

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/* C99 includes. */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

#include "bsp_cfg.h"
#include "bsp_compiler_support.h"

/*******************************************************************************************************************//**
 * @addtogroup BSP_COMMON
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Common version structure */
typedef struct st_fsp_version
{
    uint8_t code_version_minor;        ///< Code minor version
    uint8_t code_version_major;        ///< Code major version
    uint8_t api_version_minor;         ///< API minor version
    uint8_t api_version_major;         ///< API major version
} fsp_version_t;

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Used to signify that it is not able to be used as an interrupt. */
#define BSP_IRQ_DISABLED    (0xFFU)

#if 1 == BSP_CFG_RTOS                  /* ThreadX */
 #include "tx_user.h"
 #if defined(TX_ENABLE_EVENT_TRACE) || defined(TX_ENABLE_EXECUTION_CHANGE_NOTIFY)
  #include "tx_port.h"
  #define FSP_CONTEXT_SAVE       tx_isr_start((uint32_t) R_FSP_CurrentIrqGet());
  #define FSP_CONTEXT_RESTORE    tx_isr_end((uint32_t) R_FSP_CurrentIrqGet());
 #else
  #define FSP_CONTEXT_SAVE
  #define FSP_CONTEXT_RESTORE
 #endif
#else
 #if ((0 == BSP_CFG_RTOS) && (1 == BSP_NESTED_INTERRUPT))
  #define FSP_CONTEXT_SAVE       FSP_NESTED_SAVE
  #define FSP_CONTEXT_RESTORE    FSP_NESTED_RESTORE
 #else
  #define FSP_CONTEXT_SAVE
  #define FSP_CONTEXT_RESTORE
 #endif
#endif

/** Macro that can be defined in order to enable logging in FSP modules. */
#ifndef FSP_LOG_PRINT
 #define FSP_LOG_PRINT(X)
#endif

/** Macro to log and return error without an assertion. */
#ifndef FSP_RETURN
 #define FSP_RETURN(err)    FSP_ERROR_LOG((err)); \
    return err;
#endif

/** This function is called before returning an error code. To stop on a runtime error, define fsp_error_log in
 * user code and do required debugging (breakpoints, stack dump, etc) in this function.*/
#if (1 == BSP_CFG_ERROR_LOG)

 #ifndef FSP_ERROR_LOG
  #define FSP_ERROR_LOG(err) \
    fsp_error_log((err), __FILE__, __LINE__);
 #endif
#else

 #define FSP_ERROR_LOG(err)
#endif

/** Default assertion calls ::FSP_ERROR_RETURN if condition "a" is false. Used to identify incorrect use of API's in FSP
 * functions. */
#if (3 == BSP_CFG_ASSERT)
 #define FSP_ASSERT(a)
#elif (2 == BSP_CFG_ASSERT)
 #define FSP_ASSERT(a)    {assert(a);}
#else
 #define FSP_ASSERT(a)    FSP_ERROR_RETURN((a), FSP_ERR_ASSERTION)
#endif

/** All FSP error codes are returned using this macro. Calls ::FSP_ERROR_LOG function if condition "a" is false. Used
 * to identify runtime errors in FSP functions. */
#define FSP_ERROR_RETURN(a, err)                        \
    {                                                   \
        if ((a))                                        \
        {                                               \
            (void) 0;                  /* Do nothing */ \
        }                                               \
        else                                            \
        {                                               \
            FSP_ERROR_LOG(err);                         \
            return err;                                 \
        }                                               \
    }

/* Function-like macro used to wait for a condition to be met, most often used to wait for hardware register updates.
 * This macro can be redefined to add a timeout if necessary. */
#ifndef FSP_HARDWARE_REGISTER_WAIT
 #define FSP_HARDWARE_REGISTER_WAIT(reg, required_value)    while ((reg) != (required_value)) {NOP(); /* Wait. */}
#endif

/** Used to signify that the requested IRQ vector is not defined in this system. */
#define FSP_INVALID_VECTOR    ((IRQn_Type) SHARED_PERIPHERAL_INTERRUPTS_MAX_ENTRIES)

/***********************************************************************************************************************
 * Critical section definitions
 **********************************************************************************************************************/
#ifdef BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION
 #undef BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION
#endif
#define BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION    (0U)

/* This macro defines a variable for saving previous mask value */
#ifndef FSP_CRITICAL_SECTION_DEFINE
 #define FSP_CRITICAL_SECTION_DEFINE                   uint32_t old_mask_level = 0U
#endif

/* These macros abstract methods to save and restore the interrupt state for different architectures. */
#if (0 == BSP_CFG_IRQ_MASK_LEVEL_FOR_CRITICAL_SECTION)
 #define FSP_CRITICAL_SECTION_GET_CURRENT_STATE        __get_INTMASK
 #define FSP_CRITICAL_SECTION_SET_STATE                __set_INTMASK
 #define FSP_CRITICAL_SECTION_IRQ_MASK_SET             (1U)
#endif

/** This macro temporarily saves the current interrupt state and disables interrupts. */
#ifndef FSP_CRITICAL_SECTION_ENTER
 #define FSP_CRITICAL_SECTION_ENTER                            \
    old_mask_level = FSP_CRITICAL_SECTION_GET_CURRENT_STATE(); \
    FSP_CRITICAL_SECTION_SET_STATE(FSP_CRITICAL_SECTION_IRQ_MASK_SET)
#endif

/** This macro restores the previously saved interrupt state, reenabling interrupts. */
#ifndef FSP_CRITICAL_SECTION_EXIT
 #define FSP_CRITICAL_SECTION_EXIT    FSP_CRITICAL_SECTION_SET_STATE(old_mask_level)
#endif

/** This macro for saving and restoring exception cause code in nested interrupt handling. */
#if ((0 == BSP_CFG_RTOS) && (1 == BSP_NESTED_INTERRUPT))
 #define FSP_NESTED_SAVE                     \
    volatile uint16_t eiic_val = STSR(EIWR); \
    LDSR(EIWR, STSR(EIIC));                  \
    EI();

 #define FSP_NESTED_RESTORE \
    DI();                   \
    LDSR(EIIC, eiic_val);   \
    LDSR(EIWR, eiic_val);
#endif

/***********************************************************************************************************************
 * Type definitions
 **********************************************************************************************************************/

/** Different warm start entry locations in the BSP. */
typedef enum e_bsp_warm_start_event
{
    BSP_WARM_START_RESET = 0,          ///< Called almost immediately after reset. No C runtime environment, clocks, or IRQs.
    BSP_WARM_START_POST_CLOCK,         ///< Called after clock initialization. No C runtime environment or IRQs.
    BSP_WARM_START_POST_C              ///< Called after clocks and C runtime environment have been set up
} bsp_warm_start_event_t;

/***********************************************************************************************************************
 * Global variables (defined in other files)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_COMMON)
 **********************************************************************************************************************/

#endif                                 /* FSP_SRC_BSP_MCU_ALL_BSP_COMMON_H_ */

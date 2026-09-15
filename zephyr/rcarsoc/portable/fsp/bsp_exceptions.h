/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_EXCEPTIONS_H
#define BSP_EXCEPTIONS_H

#include <stdint.h>

/* __NOP() and other compiler intrinsics from CMSIS Core_R. */
#include "cmsis_compiler.h"

#ifdef __cplusplus
extern "C" {
 #endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_INTERRUPT_ATTRIBUTE

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Only the FSP sentinel values; interrupt numbers are provided by the caller. */
typedef enum IRQn
{
    FSP_IRQn_NONE = -1,
    FSP_IRQn_MAX  = INT32_MAX,
} IRQn_Type;

 #ifdef __cplusplus
}
 #endif

#endif                                 /* BSP_EXCEPTIONS_H */

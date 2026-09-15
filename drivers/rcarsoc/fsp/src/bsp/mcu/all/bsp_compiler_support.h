/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_COMPILER_SUPPORT_H
#define BSP_COMPILER_SUPPORT_H

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#if !defined(__GNUC__)
 #error "Unsupported compiler. Only GCC-compatible compilers are supported."
#endif

#endif                                 /* BSP_COMPILER_SUPPORT_H */

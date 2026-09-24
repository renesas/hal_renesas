/*************************************************************************************************************
* Copyright (c) [2025] Renesas Electronics Corporation
* This software is released under the MIT License
* http://opensource.org/licenses/mit-license.php
* SPDX-License-Identifier: MIT
*************************************************************************************************************/
#ifndef R_WWDT_API_H
#define R_WWDT_API_H

/**
 * @defgroup WWDT_Module WWDT Module
 * @{
 * @brief Window watchdog
 *
 * Window watchdog
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#pragma once
/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief List of WWDT channels.
 */
typedef enum {
    R_WWDT0 = 0,    ///< Watchdog Timer Unit 0
    R_WWDT1,        ///< Watchdog Timer Unit 1
    R_WWDT2,        ///< Watchdog Timer Unit 2
    R_WWDT3,        ///< Watchdog Timer Unit 3
    R_WWDT4,        ///< Watchdog Timer Unit 4
    R_WWDT5,        ///< Watchdog Timer Unit 5
    R_WWDT6,        ///< Watchdog Timer Unit 6
    R_WWDT7,        ///< Watchdog Timer Unit 7
    R_WWDT8,        ///< Watchdog Timer Unit 8
    R_WWDT9,        ///< Watchdog Timer Unit 9
    R_WWDT10,       ///< Watchdog Timer Unit 10
    R_WWDT11,       ///< Watchdog Timer Unit 11
    R_WWDT12,       ///< Watchdog Timer Unit 12
    R_WWDT13,       ///< Watchdog Timer Unit 13
    R_WWDT14,       ///< Watchdog Timer Unit 14
    R_WWDT15,       ///< Watchdog Timer Unit 15
    R_WWDT16,       ///< Watchdog Timer Unit 16
    R_WWDT17,       ///< Watchdog Timer Unit 17
    R_WWDT18,       ///< Watchdog Timer Unit 18
    R_WWDT19,       ///< Watchdog Timer Unit 19
    R_WWDT20,       ///< Watchdog Timer Unit 20
    R_WWDT_LAST		///< Delimiter indicating the end of the Watchdog Timer Unit enumeration
} wwdt_unit_t;

/**
 * @brief Enum representing the window size percentage for the Watchdog Timer.
 */
typedef enum {
    WINDOW_25P = 0,     ///< 25% window size for the Watchdog Timer
    WINDOW_50P = 1,     ///< 50% window size for the Watchdog Timer
    WINDOW_75P = 2,     ///< 75% window size for the Watchdog Timer
    WINDOW_100P = 3     ///< 1000% window size for the Watchdog Timer
} wwdt_wsize_t;

/**
 * @brief Enum representing the error recovery mode for the Watchdog Timer.
 */
typedef enum {
    ERM_NMI_MODE = 0,   ///< Non-Maskable Interrupt (NMI) mode for error recovery
    ERM_RESET_MODE = 1  ///< Reset mode for error recovery
} wwdt_erm_t;

#ifdef __cplusplus
}
#endif

/** @} */ // end of GPIO_Module

#endif /* R_WWDT_API_H */

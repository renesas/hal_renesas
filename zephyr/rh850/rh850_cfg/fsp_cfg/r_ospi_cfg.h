/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @addtogroup OSPI
 * @{
 **********************************************************************************************************************/

#ifndef R_OSPI_CFG_H
#define R_OSPI_CFG_H

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**
 * Specify whether to include code for API parameter checking. Valid settings include:
 *    - BSP_CFG_PARAM_CHECKING_ENABLE : Utilizes the system default setting from bsp_cfg.h
 *    - 1 : Includes parameter checking
 *    - 0 : Compiles out parameter checking
 */
#ifndef OSPI_CFG_PARAM_CHECKING_ENABLE
 #define OSPI_CFG_PARAM_CHECKING_ENABLE             (BSP_CFG_PARAM_CHECKING_ENABLE)
#endif

/**
 * Enables support for auto-calibration of the DS latching delay for dual-data-rate (DDR) protocol modes.
 */
#ifndef OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE
 #define OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE    (0U)
#endif

/**
 * Maximum number of bytes to buffer before sending a write command to the xSPI device. Valid settings are members
 * of the @ref ospi_combination_function_t enum.
 */
#ifndef OSPI_CFG_COMBINATION_FUNCTION
 #define OSPI_CFG_COMBINATION_FUNCTION              (OSPI_COMBINATION_FUNCTION_64BYTE)
#endif

/**
 * Enables prefetching of sequential data after a memory-mapped read operation.
 */
#ifndef OSPI_CFG_PREFETCH_FUNCTION
 #define OSPI_CFG_PREFETCH_FUNCTION                 (1U)
#endif

/**
 * Enables support for row addressing (required for NAND-type xSPI devices).
 */
#ifndef OSPI_CFG_ROW_ADDRESSING_SUPPORT_ENABLE
 #define OSPI_CFG_ROW_ADDRESSING_SUPPORT_ENABLE     (0U)
#endif

/**
 * Enables support for xSPI devices which can operate in eXecute-in-Place (XiP) mode.
 */
#ifndef OSPI_CFG_XIP_SUPPORT_ENABLE
 #define OSPI_CFG_XIP_SUPPORT_ENABLE                (0U)
#endif

#endif

/*******************************************************************************************************************//**
 * @} (end addtogroup OSPI)
 **********************************************************************************************************************/

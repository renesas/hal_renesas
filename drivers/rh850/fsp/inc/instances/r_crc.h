/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_CRC_H
#define R_CRC_H

/*******************************************************************************************************************//**
 * @addtogroup CRC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_crc_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Specifies the CRC calculate mode 0 (for output) CMD0*/
typedef enum e_crc_output_mode
{
    CRC_OUTPUT_MODE_NORMAL  = 0,       ///< 0: Mode N (N = Normal)
    CRC_OUTPUT_MODE_REFLECT = 1,       ///< 1: Mode R (R = Reflect)
} crc_output_mode_t;

/** Specifies the CRC calculate mode 1 (for input) CMD1*/
typedef enum e_crc_input_mode
{
    CRC_INPUT_MODE_NORMAL  = 0,        ///< 0: Mode N (N = Normal)
    CRC_INPUT_MODE_REFLECT = 1,        ///< 1: Mode R (R = Reflect)
} crc_input_mode_t;

/** Specified the CRC input data size select */
typedef enum e_crc_input_data_size
{
    CRC_INPUT_DATA_SIZE_32_BITS = 0x0UL, ///< 000: 32-bits mode
    CRC_INPUT_DATA_SIZE_16_BITS = 0x1UL, ///< 001: 16-bits mode
    CRC_INPUT_DATA_SIZE_8_BITS  = 0x3UL, ///< 011: 8-bits mode
} crc_input_data_size_t;

/** Specified the CRC polynomials size select */
typedef enum e_crc_polynomial_size
{
    CRC_POLYNOMIAL_SIZE_8_BITS  = 0x07UL, ///< 0x07: 8-bits
    CRC_POLYNOMIAL_SIZE_15_BITS = 0x0EUL, ///< 0x0E: 15-bits
    CRC_POLYNOMIAL_SIZE_16_BITS = 0x0FUL, ///< 0x0F: 16-bits
    CRC_POLYNOMIAL_SIZE_17_BITS = 0x10UL, ///< 0x10: 17-bits
    CRC_POLYNOMIAL_SIZE_21_BITS = 0x14UL, ///< 0x14: 21-bits
    CRC_POLYNOMIAL_SIZE_32_BITS = 0x1FUL, ///< 0x1F: 32-bits
    CRC_POLYNOMIAL_SIZE_64_BITS = 0x3FUL, ///< 0x3F: 64-bits
} crc_polynomial_size_t;

/** CRC Control register */
typedef struct e_crc_ctl_reg
{
    crc_input_mode_t      input_mode;      ///< CRC Polynomial Switching input  CMD1
    crc_output_mode_t     output_mode;     ///< CRC Polynomial Switching output CMD0
    crc_polynomial_size_t poly_size;       ///< CRC Polynomial Switching size  PSIZE
    crc_input_data_size_t input_data_size; ///< CRC Polynomial Switching size  DW
} crc_ctl_reg_t;

/** CRC extended configuration */
typedef struct st_crc_extended_cfg
{
    const crc_ctl_reg_t * control_reg; ///< CRC Control register
    uint32_t              poly0;       ///< CRC Generating Polynomial Switching Lower
    uint32_t              poly1;       ///< CRC Generating Polynomial Switching Upper
    uint32_t              xor0;        ///< CRC XOR mask register (Lower)
    uint32_t              xor1;        ///< CRC XOR mask register (Upper)
    uint32_t              dout0;       ///< CRC data output register (Lower)
    uint32_t              dout1;       ///< CRC data output register (Upper)
} crc_extended_cfg_t;

/** Driver instance control structure. */
typedef struct st_crc_instance_ctrl
{
    uint32_t          open;
    const crc_cfg_t * p_cfg;           // Pointer to initial configurations
    R_KCRC0_Type    * p_reg;           // Base register for this channel
} crc_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const crc_api_t g_crc_on_crc;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_CRC_Open(crc_ctrl_t * const p_ctrl, crc_cfg_t const * const p_cfg);
fsp_err_t R_CRC_Close(crc_ctrl_t * const p_ctrl);
fsp_err_t R_CRC_Calculate(crc_ctrl_t * const p_ctrl, crc_input_t * const p_crc_input, uint64_t * calculatedValue);
fsp_err_t R_CRC_CalculatedValueGet(crc_ctrl_t * const p_ctrl, uint64_t * calculatedValue);
fsp_err_t R_CRC_SnoopEnable(crc_ctrl_t * const p_ctrl, uint32_t crc_seed);
fsp_err_t R_CRC_SnoopDisable(crc_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end addtogroup CRC)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

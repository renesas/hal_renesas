/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup SFMA
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef R_SFMA_H
#define R_SFMA_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include <string.h>
#include "r_sfma_cfg.h"
#include "r_spi_flash_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define SFMA_DEVICE_START_ADDRESS    (0x38000000U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** SFMA output value configuration. */
typedef enum e_sfma_output             /* PRQA S 1536 # Advisory R-2.4 */
{
    SFMA_OUTPUT_0             = 0,     ///< Output value 0
    SFMA_OUTPUT_1             = 1,     ///< Output value 1
    SFMA_OUTPUT_PRECEDING_VAL = 2,     ///< Output value is kept at a preceding value
    SFMA_OUTPUT_HI_Z          = 3,     ///< Output value Hi-Z
} sfma_output_t;                       /* PRQA S 1535 # Advisory R-2.3*/
/* PRQA S 0750 ++ # Advisory R-19.2 */
typedef union u_sfma_common_cfg        /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t sfma_u32;
    struct
    {
        uint32_t                        : 3;
        uint32_t cpol_output            : 1; ///< Sets the SPBCLK output direction during SPBSSL negation period.
        uint32_t cs_polarity            : 1; ///< SPBSSL Signal Polarity
        uint32_t input_latch_odd_edge   : 1; ///< Sets the SPBCLK edge of the reception data
        uint32_t output_shift_even_edge : 1; ///< Sets the SPBCLK edge of the output data
        uint32_t                        : 1;
        uint32_t io0_fix_val            : 2; ///< SPBIO0 Fixed Value for 1-bit Size Input
        uint32_t                        : 2;
        uint32_t io2_fix_val            : 2; ///< SPBIO2 Fixed Value for 1-bit/2-bit Size
        uint32_t io3_fix_val            : 2; ///< SPBIO3 Fixed Value for 1-bit/2-bit Size
        uint32_t io0_idle_val           : 2; ///< SPBSSL Output Idle Value Fix SPBIO0
        uint32_t io1_idle_val           : 2; ///< SPBSSL Output Idle Value Fix SPBIO1
        uint32_t io2_idle_val           : 2; ///< SPBSSL Output Idle Value Fix SPBIO2
        uint32_t io3_idle_val           : 2; ///< SPBSSL Output Idle Value Fix SPBIO3
        uint32_t                        : 7;
        uint32_t spi_operating_mode     : 1; ///< Operating Mode Switch (External address space read/SPI operating mode)
    } sfma_b;
} sfma_common_cfg;

typedef union u_baudrate_cfg                 /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t baudrate_u32;
    struct
    {
        uint32_t bit_rate_pow_div : 2; ///< Bit Rate Frequency Division
        uint32_t                  : 6;
        uint32_t bit_rate_div     : 8; ///< Bit Rate
        uint32_t                  : 16;
    } baudrate_b;
} baudrate_cfg;

typedef union u_delay_cfg              /* PRQA S 1536 # Advisory R-2.4 */
{
    uint32_t delay_u32;
    struct
    {
        uint32_t clock_delay    : 3;   ///< Clock Delay
        uint32_t                : 5;
        uint32_t negation_delay : 3;   ///< SPBSSL Negation Delay
        uint32_t                : 5;
        uint32_t access_delay   : 3;   ///< Next Access Delay
        uint32_t                : 13;
    } delay_b;
} delay_cfg;

/* PRQA S 0750 -- */
/* Extended configuration. */
typedef struct st_sfma_extended_cfg           /* PRQA S 1536 # Advisory R-2.4 */
{
    sfma_common_cfg sfma_common;              ///< SFMA Common Control Configuration
    delay_cfg       delay;                    ///< SSL Delay Configuration
    baudrate_cfg    baudrate;                 ///< SFMA Bit rate calculation
    uint8_t         option_data_sel;          ///< Option data bytes enable for Read Mode
    uint32_t        option_data;              ///< Option data for Read Mode
    uint32_t        end_sc_signal_negate : 1; ///< SPBSSL Negation for Read Mode
    uint32_t        read_burst           : 1; ///< Read Burst
    uint32_t        burst_length         : 4; ///< Read Data Burst Length
} sfma_extended_cfg_t;

/** Instance control block. DO NOT INITIALIZE.  Initialization occurs when @ref spi_flash_api_t::open is called */
typedef struct st_sfma_instance_ctrl          /* PRQA S 1536 # Advisory R-2.4 */
{
    spi_flash_cfg_t const * p_cfg;            ///< Pointer to initial configuration
    spi_flash_data_lines_t  data_lines;       ///< Data lines
    spi_flash_protocol_t    spi_protocol;     ///< Current SPI protocol selected
    uint32_t                open;             ///< Whether or not driver is open
    R_SFMA0_Type          * p_reg;            ///< Base register for this channel
} sfma_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const spi_flash_api_t g_sfma_on_spi_flash;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_SFMA_Open(spi_flash_ctrl_t * const p_ctrl, spi_flash_cfg_t const * const p_cfg);
fsp_err_t R_SFMA_Close(spi_flash_ctrl_t * const p_ctrl);
fsp_err_t R_SFMA_DirectWrite(spi_flash_ctrl_t * const p_ctrl,
                             uint8_t const * const    p_src,
                             uint32_t const           bytes,
                             bool const               read_after_write);
fsp_err_t R_SFMA_DirectRead(spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t R_SFMA_SpiProtocolSet(spi_flash_ctrl_t * const p_ctrl, spi_flash_protocol_t spi_protocol);
fsp_err_t R_SFMA_XipEnter(spi_flash_ctrl_t * const p_ctrl);
fsp_err_t R_SFMA_XipExit(spi_flash_ctrl_t * const p_ctrl);
fsp_err_t R_SFMA_Write(spi_flash_ctrl_t * const p_ctrl,
                       uint8_t const * const    p_src,
                       uint8_t * const          p_dest,
                       uint32_t                 byte_count);
fsp_err_t R_SFMA_Erase(spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_device_address, uint32_t byte_count);
fsp_err_t R_SFMA_StatusGet(spi_flash_ctrl_t * const p_ctrl, spi_flash_status_t * const p_status);
fsp_err_t R_SFMA_BankSet(spi_flash_ctrl_t * const p_ctrl, uint32_t bank);
fsp_err_t R_SFMA_DirectTransfer(spi_flash_ctrl_t * const            p_ctrl,
                                spi_flash_direct_transfer_t * const p_transfer,
                                spi_flash_direct_transfer_dir_t     direction);
fsp_err_t R_SFMA_AutoCalibrate(spi_flash_ctrl_t * const p_ctrl);
fsp_err_t R_SFMA_NegateChipSelect(spi_flash_ctrl_t * const p_ctrl);
fsp_err_t R_SFMA_CacheClear(spi_flash_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end defgroup SFMA)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /*R_SFMA_H*/

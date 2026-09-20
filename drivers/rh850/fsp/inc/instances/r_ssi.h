/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup SSI
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

#ifndef R_SSI_H
#define R_SSI_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_i2s_api.h"
#include "r_ssi_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Bit clock division ratio.  Bit clock frequency = audio clock frequency / bit clock division ratio. */
typedef enum e_ssi_clock_div                       /* PRQA S 1536 */
{
    SSI_CLOCK_DIV_1   = 0,                         ///< Clock divisor 1
    SSI_CLOCK_DIV_2   = 1,                         ///< Clock divisor 2
    SSI_CLOCK_DIV_4   = 2,                         ///< Clock divisor 4
    SSI_CLOCK_DIV_8   = 3,                         ///< Clock divisor 8
    SSI_CLOCK_DIV_16  = 4,                         ///< Clock divisor 16
    SSI_CLOCK_DIV_32  = 5,                         ///< Clock divisor 32
    SSI_CLOCK_DIV_64  = 6,                         ///< Clock divisor 64
    SSI_CLOCK_DIV_128 = 7,                         ///< Clock divisor 128
    SSI_CLOCK_DIV_6   = 8,                         ///< Clock divisor 6
    SSI_CLOCK_DIV_12  = 9,                         ///< Clock divisor 12
    SSI_CLOCK_DIV_24  = 10,                        ///< Clock divisor 24
    SSI_CLOCK_DIV_48  = 11,                        ///< Clock divisor 48
    SSI_CLOCK_DIV_96  = 12,                        ///< Clock divisor 96
} ssi_clock_div_t;

/** Communication format */
typedef enum e_ssi_communication_format            /* PRQA S 1536 */
{
    SSI_COMMUNICATION_FORMAT_I2S      = 0,         ///< Using I2S Format
    SSI_COMMUNICATION_FORMAT_TDM      = 1,         ///< Using TDM Format
    SSI_COMMUNICATION_FORMAT_MONAURAL = 2,         ///< Using Monaural Format
} ssi_communication_format_t;

/** Frame Type For Monaural Format */
typedef enum e_ssi_frametype_monaural_format       /* PRQA S 1536 */
{
    SSI_FRAMETYPE_MONAURAL_FORMAT_SHORT_FRAME = 0, ///< Using short frame for Monaural Format
    SSI_FRAMETYPE_MONAURAL_FORMAT_LONG_FRAME  = 1, ///< Using long frame for Monaural Format
} ssi_frametype_monaural_format_t;

/** Frame Word Number */
typedef enum e_ssi_frame_word_number               /* PRQA S 1536 */
{
    SSI_FRAME_WORD_NUMBER_4 = 1,                   ///< Using FRM is 4
    SSI_FRAME_WORD_NUMBER_6 = 2,                   ///< Using FRM is 6
    SSI_FRAME_WORD_NUMBER_8 = 3,                   ///< Using FRM is 8
} ssi_frame_word_number_t;

/* SSI communication direction */
typedef enum e_ssi_dir                             /* PRQA S 1536 */
{
    SSI_DIR_RX    = 1U,                            ///< Receive direction only
    SSI_DIR_TX    = 2U,                            ///< Transmit direction only
    SSI_DIR_TX_RX = 3U,                            ///< Transmit and receive direction
} ssi_dir_t;

/** Channel instance control block. DO NOT INITIALIZE.  Initialization occurs when @ref i2s_api_t::open is called. */
typedef struct st_ssi_instance_ctrl                /* PRQA S 1536 */
{
    uint32_t          open;                        // Whether or not this control block is initialized
    i2s_cfg_t const * p_cfg;                       // Initial configurations.
    R_SSIF0_Type    * p_reg;                       // Pointer to SSI register base address

    /* Source buffer pointer used to fill hardware FIFO from transmit ISR. */
    void const * volatile p_tx_src;

    /* Size of source buffer used to fill hardware FIFO from transmit ISR. */
    volatile uint32_t tx_src_samples;

    /* Destination buffer pointer used to fill from hardware FIFO in receive ISR. */
    void * volatile p_rx_dest;

    /* Size of destination buffer used to fill from hardware FIFO in receive ISR. */
    volatile uint32_t rx_dest_samples;
    transfer_size_t   fifo_access_size;                       // Access the FIFO as 1 byte, 2 bytes, or 4 bytes

    /* Pointer to callback and optional working memory */
    void (* p_callback)(i2s_callback_args_t *);               /* PRQA S 1336 # DP_MSR_R-8.2_UC1 */
    i2s_callback_args_t * p_callback_memory;
    void                * p_context;                          // < User defined context passed into callback function
} ssi_instance_ctrl_t;

/** SSI configuration extension. */
typedef struct st_ssi_extended_cfg                            /* PRQA S 1536 */
{
    ssi_clock_div_t                 bit_clock_div;            ///< Select bit clock division ratio
    ssi_communication_format_t      communication_format;     ///< Communication format
    ssi_frame_word_number_t         frame_word_number;        ///< Frame word number
    ssi_frametype_monaural_format_t frametype_monauralformat; ///< Frame type for monaural format
    uint8_t transmit_data_empty;                              ///< Transmit data empty setting
    uint8_t receive_data_full;                                ///< Receive data full setting
} ssi_extended_cfg_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
extern const i2s_api_t g_i2s_on_ssi;

/** @endcond */

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

fsp_err_t R_SSI_Open(i2s_ctrl_t * const p_ctrl, i2s_cfg_t const * const p_cfg);
fsp_err_t R_SSI_Stop(i2s_ctrl_t * const p_ctrl);
fsp_err_t R_SSI_StatusGet(i2s_ctrl_t * const p_ctrl, i2s_status_t * const p_status);
fsp_err_t R_SSI_Write(i2s_ctrl_t * const p_ctrl, void const * const p_src, uint32_t const bytes);
fsp_err_t R_SSI_Read(i2s_ctrl_t * const p_ctrl, void * const p_dest, uint32_t const bytes);
fsp_err_t R_SSI_WriteRead(i2s_ctrl_t * const p_ctrl, void const * const p_src, void * const p_dest,
                          uint32_t const bytes);
fsp_err_t R_SSI_Mute(i2s_ctrl_t * const p_ctrl, i2s_mute_t const mute_enable);
fsp_err_t R_SSI_Close(i2s_ctrl_t * const p_ctrl);
fsp_err_t R_SSI_CallbackSet(i2s_ctrl_t * const          p_api_ctrl,
                            void (                    * p_callback)(i2s_callback_args_t * p_args),
                            void * const                p_context,
                            i2s_callback_args_t * const p_callback_memory);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_SSI_H */

/*******************************************************************************************************************//**
 * @} (end defgroup SSI)
 **********************************************************************************************************************/

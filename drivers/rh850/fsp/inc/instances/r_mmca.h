/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_MMCA_H
#define R_MMCA_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_mmca_api.h"
#include "r_mmca_cfg.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup MMCA
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define MMCA_MAX_BLOCK_SIZE           (512U)
#define MMCA_PRV_ACCESS_ERROR_MASK     /* PRQA S 1534 # Advisory R-2.5 */                      \
    (MMCA_EVENT_COMMAND_ERROR | MMCA_EVENT_BUFFER_ACCESS_ERROR | MMCA_EVENT_WRITE_DATA_ERROR | \
     MMCA_EVENT_READ_ERROR | MMCA_EVENT_RESPONSE_INDEX_ERROR | MMCA_EVENT_RESPONSE_ERROR |     \
     MMCA_EVENT_CRC_STATUS_TIMEOUT | MMCA_EVENT_WRITE_DATA_TIMEOUT | MMCA_EVENT_READ_TIMEOUT | \
     MMCA_EVENT_RESPONSE_BUSY_TIMEOUT | MMCA_EVENT_RESPONSE_TIMEOUT)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Transfer direction. */
typedef enum e_mmca_transfer_dir                          /* PRQA S 1536 # Advisory R-2.4*/
{
    MMCA_TRANSFER_DIR_NONE  = 0,                          ///< None Transfer direction of data lines
    MMCA_TRANSFER_DIR_READ  = 1,                          ///< Transfer direction read of data lines
    MMCA_TRANSFER_DIR_WRITE = 2,                          ///< Transfer direction write of data lines
} mmca_transfer_dir_t;                                    /* PRQA S 1535 # Advisory R-2.3*/

/* Background Operation Status. */
typedef enum e_mmca_bkops_status                          /* PRQA S 1536 # Advisory R-2.4*/
{
    MMCA_BKOPS_STATUS_NO_OPERATION           = 0,         ///< No Background Operation
    MMCA_BKOPS_STATUS_OPERATION_NON_CRITICAL = 1,         ///< Background Operation non critical
    MMCA_BKOPS_STATUS_OPERATION_PERFORMANCE  = 2,         ///< Background Operation performance
    MMCA_BKOPS_STATUS_OPERATION_CRITICAL     = 3,         ///< Background Operation critical
} mmca_bkops_status_t;                                    /* PRQA S 1535 # Advisory R-2.3*/

/** MMCA instance control block.  This is private to the FSP and should not be used or modified by the application. */
typedef struct st_mmca_instance_ctrl                      /* PRQA S 1536 # Advisory R-2.4*/
{
    uint32_t              open;                           ///< Used to determine if the MMCA is opened
    mmca_cfg_t const    * p_cfg;                          ///< Pointer to configuration block
    mmca_device_t         device;                         ///< Store device information
    bool                  sector_addressing;              ///< The card access mode: 0 is byte mode, 1 is sector mode
    bool                  transfer_in_progress;           ///< Transfer in progress
    bool                  initialized_mmc;                ///< Status eMMC is connected
    R_MMCA0_Type        * p_reg;                          ///< Pointer to MMCA0 base address
    volatile mmca_event_t mmca_event;                     ///< MMCA event
    uint32_t              current_operation_start_sector; ///< Current operation start sector
    uint32_t              current_operation_sector_count; ///< Current operation sector count
    uint8_t const       * current_operation_buffer;       ///< Pointer to current operation data buffer

    mmc_response_t  csd_infor;                            ///< CSD information
    mmc_response_t  response;                             ///< MMC response
    uint8_t const * p_mmca_tx_src;                        ///< Transmit source pointer
    uint32_t        mmca_tx_bytes_remaining;              ///< Transmit bytes remaining
    uint32_t        mmca_blocks_remaining;                ///< Blocks remaining
    uint32_t        mmca_block_size;                      ///< Block size
    bool            writing_nonblock_flag;                ///< Writing non-block flag
    bool            erasing_nonblock_flag;                ///< Erasing non-block flag
    bool            dma_write_in_progress;                ///< DMA-based (non-HPI) write in progress
    bool            dma_read_in_progress;                 ///< DMA-based (non-HPI) read in progress
    uint32_t        erase_start_sector;                   ///< Erase start sector
    uint32_t        erase_sector_count;                   ///< Erase sector count

    void (* p_callback)(mmca_callback_args_t * p_args);   ///< Pointer to callback
    mmca_callback_args_t * p_callback_memory;             ///< Pointer to optional callback argument memory
    void const           * p_context;                     ///< Pointer to context to be passed into callback function
} mmca_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const mmca_api_t g_mmca_on_mmca;

/** @endcond */

/**********************************************************************************************************************
 * Function Prototypes
 **********************************************************************************************************************/

fsp_err_t R_MMCA_Open(mmca_ctrl_t * const p_api_ctrl, mmca_cfg_t const * const p_cfg);
fsp_err_t R_MMCA_Close(mmca_ctrl_t * const p_api_ctrl);
fsp_err_t R_MMCA_MediaInit(mmca_ctrl_t * const p_api_ctrl, mmca_device_t * const p_device);
fsp_err_t R_MMCA_Read(mmca_ctrl_t * const p_api_ctrl,
                      uint8_t * const     p_dest,
                      uint32_t const      start_sector,
                      uint32_t const      sector_count);
fsp_err_t R_MMCA_Write(mmca_ctrl_t * const   p_api_ctrl,
                       uint8_t const * const p_source,
                       uint32_t const        start_sector,
                       uint32_t const        sector_count);

fsp_err_t R_MMCA_Erase(mmca_ctrl_t * const p_api_ctrl, uint32_t const start_sector, uint32_t const sector_count);
fsp_err_t R_MMCA_Status(mmca_ctrl_t * const p_api_ctrl, mmca_status_t * const p_status);
fsp_err_t R_MMCA_CallbackSet(mmca_ctrl_t * const          p_api_ctrl,
                             void (                     * p_callback)(mmca_callback_args_t * p_args),
                             void const * const           p_context,
                             mmca_callback_args_t * const p_callback_memory);
fsp_err_t R_MMCA_HPI_Trigger(mmca_ctrl_t * const p_api_ctrl);
fsp_err_t R_MMCA_WriteAsync(mmca_ctrl_t * const   p_api_ctrl,
                            uint8_t const * const p_source,
                            uint32_t const        start_sector,
                            uint32_t const        sector_count);
fsp_err_t R_MMCA_EraseAsync(mmca_ctrl_t * const p_api_ctrl, uint32_t const start_sector, uint32_t const sector_count);
fsp_err_t R_MMCA_BackgroundManual(mmca_ctrl_t * const p_api_ctrl);
fsp_err_t R_MMCA_BackgroundAutoSet(mmca_ctrl_t * const p_api_ctrl, mmca_bkops_auto_enabled_t enable);
fsp_err_t R_MMCA_BackgroundStatusGet(mmca_ctrl_t * const p_api_ctrl, uint8_t * const p_status);
fsp_err_t R_MMCA_Boot_Info(mmca_ctrl_t * const p_api_ctrl, mmca_boot_info_t * const p_boot_info);
fsp_err_t R_MMCA_Boot_Configure(mmca_ctrl_t * const p_api_ctrl, mmca_boot_cfg_t const * const p_boot_cfg);
fsp_err_t R_MMCA_Boot_PartitionAccessBegin(mmca_ctrl_t * const              p_api_ctrl,
                                           uint8_t                          partition_access,
                                           mmca_boot_access_guard_t * const p_guard);
fsp_err_t R_MMCA_Boot_PartitionAccessEnd(mmca_ctrl_t * const p_api_ctrl, mmca_boot_access_guard_t * const p_guard);

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */

#endif                                 // R_MMCA_H

/*******************************************************************************************************************//**
 * @} (end addtogroup MMCA)
 **********************************************************************************************************************/

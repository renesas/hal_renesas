/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "r_sfma.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define SFMA_PRV_OPEN                       (0x53464D41U)

/* Register Mask for specific SPI MODE */
#define SFMA_DRENR_MASK_STANDARD_MODE       (0x00004000U)
#define SFMA_DRENR_MASK_FAST_READ_MODE      (0x00004000U)
#define SFMA_DRENR_MASK_DUAL_OUTPUT_MODE    (0x00014000U)
#define SFMA_DRENR_MASK_DUAL_IO_MODE        (0x01114000U)
#define SFMA_DRENR_MASK_QUAD_OUTPUT_MODE    (0x00024000U)
#define SFMA_DRENR_MASK_QUAD_IO_MODE        (0x02224000U)

/* Default Command for specific SPI read mode */
#define SFMA_READ_COMMAND                   (0x03U)
#define SFMA_FAST_READ_COMMAND              (0x0BU)
#define SFMA_DUAL_OUTPUT_COMMAND            (0x3BU)
#define SFMA_DUAL_IO_COMMAND                (0xBBU)
#define SFMA_QUAD_OUTPUT_COMMAND            (0x6BU)
#define SFMA_QUAD_IO_COMMAND                (0xEBU)

/* Mask for SPI protocol */
#define SFMA_SPI_PROTOCOL_COMMAND_MASK      (0x03U)
#define SFMA_SPI_PROTOCOL_ADDRESS_MASK      (0x18U)
#define SFMA_SPI_PROTOCOL_DATA_MASK         (0xC0U)

/* Mask for SPI Enable Register */
#define SFMA_SPI_CLEAR_SMENR_MASK           ~(R_SFMA0_SMENR_ADE_Msk | R_SFMA0_SMENR_SPIDE_Msk | R_SFMA0_SMENR_DME_Msk | \
                                              R_SFMA0_SMENR_CDE_Msk | R_SFMA0_SMENR_OPDE_Msk | R_SFMA0_SMENR_OCDE_Msk)

/* SPIDE values for different transfer sizes */
#define SFMA_SPIDE_8BIT_TRANSFER            (8U)  ///< 8-bit (1 byte) data transfer
#define SFMA_SPIDE_16BIT_TRANSFER           (12U) ///< 16-bit (2 bytes) data transfer
#define SFMA_SPIDE_32BIT_TRANSFER           (15U) ///< 32-bit (4 bytes) data transfer

/* Mask for a read only bit */
#define SFMA_CMNCR_READ_MASK                (0x01000000U)

/* 1 byte transfer data position */
#define SFMA_1BYTE_TRANSFER_DATA_POS        24U

/* 2 bytes transfer data position */
#define SFMA_2BYTE_TRANSFER_DATA_POS        16U

/* Address Bank Select Position Shift */
#define SFMA_ADDRESS_BANK_POS_SHIFT         26U

/* EAV bit mask */
#define SFMA_EAV_BIT_MASK                   (0x3FU)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static bool        r_sfma_status_sub(sfma_instance_ctrl_t * p_instance_ctrl);
static inline void r_sfma_read_enable(sfma_instance_ctrl_t * p_instance_ctrl);
static inline void r_sfma_write_enable(sfma_instance_ctrl_t * p_instance_ctrl);
static inline void r_sfma_start_transfer(sfma_instance_ctrl_t * p_instance_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* SFMAn Data Read Enable Setting Register Mask for read mode  */
static const uint32_t g_read_mode_mask[6] = /* PRQA S 3218 # Advisory R-8.9 */
{
    SFMA_DRENR_MASK_STANDARD_MODE,
    SFMA_DRENR_MASK_FAST_READ_MODE,
    SFMA_DRENR_MASK_DUAL_OUTPUT_MODE,
    SFMA_DRENR_MASK_DUAL_IO_MODE,
    SFMA_DRENR_MASK_QUAD_OUTPUT_MODE,
    SFMA_DRENR_MASK_QUAD_IO_MODE
};

/* SPI read command for each protocol  */
static const uint32_t g_read_command[6] = /* PRQA S 3218 # Advisory R-8.9 */
{
    SFMA_READ_COMMAND,
    SFMA_FAST_READ_COMMAND,
    SFMA_DUAL_OUTPUT_COMMAND,
    SFMA_DUAL_IO_COMMAND,
    SFMA_QUAD_OUTPUT_COMMAND,
    SFMA_QUAD_IO_COMMAND
};

/* Position of the first bytes for 0/8/16/32 bit transfer in SPI Operation Mode */
static const uint32_t g_data_pos[5] = {0U, 24U, 16U, 0U, 0U};             /* PRQA S 3218 # Advisory R-8.9 */

/* Address Enable Mask for 0/8/16/24/32 bytes address */
static const uint32_t g_address_enable[5] = {0U, 0x4U, 0x6U, 0x7U, 0xFU}; /* PRQA S 3218 # Advisory R-8.9 */

/* Data Enable Mask for 0/8/16/24/32 bytes address */
static const uint32_t g_data_enable[5] = {0U, 0x8U, 0xCU, 0xEU, 0xFU};

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

const spi_flash_api_t g_sfma_on_spi_flash = /* PRQA S 1502 # Advisory R-2.8 */
{
    /* PRQA S 3635 ++ # Advisory R-17.12 */
    .open           = R_SFMA_Open,
    .directWrite    = R_SFMA_DirectWrite,
    .directRead     = R_SFMA_DirectRead,
    .directTransfer = R_SFMA_DirectTransfer,
    .spiProtocolSet = R_SFMA_SpiProtocolSet,
    .write          = R_SFMA_Write,
    .erase          = R_SFMA_Erase,
    .statusGet      = R_SFMA_StatusGet,
    .xipEnter       = R_SFMA_XipEnter,
    .xipExit        = R_SFMA_XipExit,
    .bankSet        = R_SFMA_BankSet,
    .autoCalibrate  = R_SFMA_AutoCalibrate,
    .close          = R_SFMA_Close,

    /* PRQA S 3635 -- */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1 */

/*******************************************************************************************************************//**
 * @addtogroup SFMA
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/* PRQA S 0662 ++ # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

/*******************************************************************************************************************//**
 * Open the SFMA driver module. After the driver is open, the SFMA can be accessed like internal flash memory starting
 * at address 0x38000000.
 *
 * Implements @ref spi_flash_api_t::open.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       The parameter p_instance_ctrl or p_cfg is NULL.
 * @retval FSP_ERR_ALREADY_OPEN    Driver has already been opened with the same p_instance_ctrl.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_Open (spi_flash_ctrl_t * const p_ctrl, spi_flash_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl;                /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif
    sfma_extended_cfg_t const * p_cfg_extend = (sfma_extended_cfg_t const *) p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t drenr  = 0U;
    uint32_t drdmcr = 0U;
    uint32_t smenr  = (uint32_t) R_SFMA0_SMENR_CDE_Msk;
    p_instance_ctrl->p_cfg        = p_cfg;
    p_instance_ctrl->p_reg        = R_SFMA0; /* PRQA S 0306 # Advisory R-11.4 */
    p_instance_ctrl->data_lines   = p_cfg->page_program_address_lines;
    p_instance_ctrl->spi_protocol = p_cfg->spi_protocol;

    /* Set the initial SFMA common control register. */
    p_instance_ctrl->p_reg->CMNCR = p_cfg_extend->sfma_common.sfma_u32 | SFMA_CMNCR_READ_MASK;

    /* Set the SFMA clock rate */
    p_instance_ctrl->p_reg->SPBCR = p_cfg_extend->baudrate.baudrate_u32;

    /* Delay setting for SFMA */
    p_instance_ctrl->p_reg->SSLDR = p_cfg_extend->delay.delay_u32;

    /* Read burst & negate SSL initial setting */
    p_instance_ctrl->p_reg->DRCR = ((uint32_t) p_cfg_extend->read_burst << R_SFMA0_DRCR_RBE_Pos) |
                                   ((uint32_t) p_cfg_extend->burst_length << R_SFMA0_DRCR_RBURST_Pos) |
                                   (uint32_t) p_cfg_extend->end_sc_signal_negate;

    /* SPI External address space read mode */
    drenr = g_read_mode_mask[p_cfg->read_mode];

    /* Enable extended address in  External address space read mode */
    drenr |= ((uint32_t) p_instance_ctrl->p_cfg->address_bytes << R_SFMA0_DRENR_ADE_Pos) & R_SFMA0_DRENR_ADE_Msk;

    /* SPI External address space read command */
    p_instance_ctrl->p_reg->DRCMR_b.CMD = (uint8_t) g_read_command[p_cfg->read_mode];

    /* Dummy cycles Bit Size */
    p_instance_ctrl->p_reg->SMDMCR_b.DMDB = (uint8_t) p_instance_ctrl->data_lines;
    drdmcr = (((uint32_t) p_instance_ctrl->data_lines) << R_SFMA0_DRDMCR_DMDB_Pos) & R_SFMA0_DRDMCR_DMDB_Msk;

    if (p_cfg->dummy_clocks > SPI_FLASH_DUMMY_CLOCKS_0)
    {
        /* Set dummy clock */
        drenr  |= R_SFMA0_DRENR_DME_Msk;
        drdmcr |= (((uint32_t) p_cfg->dummy_clocks) - 1U) & R_SFMA0_DRDMCR_DMCYC_Msk;
    }

    p_instance_ctrl->p_reg->DRDMCR = drdmcr;

    /* Set Option Data */
    drenr |= g_data_enable[p_cfg_extend->option_data_sel] << R_SFMA0_DRENR_OPDE_Pos;
    p_instance_ctrl->p_reg->DROPR = ((uint32_t) p_cfg_extend->option_data) <<
                                    (((((uint32_t) 4U) - (uint32_t) p_cfg_extend->option_data_sel)) << 3U);

    /* Apply SPI protocol to DRENR (External Address Space Read Mode) */
    uint32_t const spi_protocol = (uint32_t) p_cfg->spi_protocol;

    drenr |= (((spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_DRENR_CDB_Pos) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_DRENR_OCDB_Pos) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_DRENR_ADB_Pos - 3U)) | /* PRQA S 3397 # Advisory R-12.1 */
              ((spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_DRENR_OPDB_Pos - 3U)) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_DATA_MASK) << (R_SFMA0_DRENR_DRDB_Pos - 6U)));

    p_instance_ctrl->p_reg->DRENR = drenr;

    /* Initial SPI Operating mode */
    smenr |= (((spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_SMENR_CDB_Pos) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_SMENR_OCDB_Pos) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_SMENR_ADB_Pos - 3U)) | /* PRQA S 3397 # Advisory R-12.1 */
              ((spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_SMENR_OPDB_Pos - 3U)) |
              ((spi_protocol & SFMA_SPI_PROTOCOL_DATA_MASK) << (R_SFMA0_SMENR_SPIDB_Pos - 6U)));
    p_instance_ctrl->p_reg->SMENR = smenr;

    p_instance_ctrl->open = SFMA_PRV_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Writes raw data directly to the SFMA.
 *
 * Implements @ref spi_flash_api_t::directWrite.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_DirectWrite (spi_flash_ctrl_t * const p_ctrl, /* PRQA S 1505 # Advisory R-8.7 */
                              uint8_t const * const    p_src,
                              uint32_t const           bytes,
                              bool const               read_after_write)
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(bytes > 0U);
    FSP_ASSERT(NULL != p_src);
#endif

    uint32_t smenr = p_instance_ctrl->p_reg->SMENR;

    /* Verify device is not busy when write data . */
    FSP_ERROR_RETURN(!r_sfma_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);

    /* Enable SPI Operating mode to DirectWrite to Memory */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 1U;

    smenr &= SFMA_SPI_CLEAR_SMENR_MASK;

    /* Enable 1 bytes data transmission*/
    smenr |= SFMA_SPIDE_8BIT_TRANSFER;

    p_instance_ctrl->p_reg->SMENR = smenr;

    r_sfma_write_enable(p_instance_ctrl);

    /* CS pin level is maintained at the end of transfer */
    p_instance_ctrl->p_reg->SMCR_b.SSLKP = 1U;

    /* Write data and start transmit  */
    for (uint32_t byte_index = 0U; ((byte_index + 1U) < bytes); byte_index++)
    {
        p_instance_ctrl->p_reg->SMWDR = (uint32_t) p_src[byte_index] << SFMA_1BYTE_TRANSFER_DATA_POS;

        r_sfma_start_transfer(p_instance_ctrl);
    }

    /* Read after write will keep setting for CS pin before transmit the last data */
    if (!read_after_write)
    {
        /*  CS pin level is negated at the end of last data transmission */
        p_instance_ctrl->p_reg->SMCR_b.SSLKP = 0U;
        p_instance_ctrl->p_reg->SMWDR        = (uint32_t) p_src[bytes - 1U] << SFMA_1BYTE_TRANSFER_DATA_POS;

        r_sfma_start_transfer(p_instance_ctrl);

        /* Disable Read, Write when completed  */
        p_instance_ctrl->p_reg->SMCR = 0U;

        /* Return to External Address Space Read Mode */
        p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;
    }
    else
    {
        p_instance_ctrl->p_reg->SMWDR = (uint32_t) p_src[bytes - 1U] << SFMA_1BYTE_TRANSFER_DATA_POS;

        r_sfma_start_transfer(p_instance_ctrl);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reads raw data directly from the SFMA.
 *
 * Implements @ref spi_flash_api_t::directRead.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_INVALID_MODE        A required DirectWrite command API first.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_DirectRead (spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes) /* PRQA S 1505 # Advisory R-8.7 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl;                               /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_dest);
    FSP_ASSERT(bytes > 0U);
    FSP_ERROR_RETURN(p_instance_ctrl->p_reg->CMNCR_b.MD == 1U, FSP_ERR_INVALID_MODE);
#endif
    r_sfma_read_enable(p_instance_ctrl);

    /* Enable transfer 16 bits */
    p_instance_ctrl->p_reg->SMENR = p_instance_ctrl->p_reg->SMENR | SFMA_SPIDE_16BIT_TRANSFER;

    /* Start Read 2 bytes data  */
    for (uint32_t byte_index = 0U; byte_index < bytes; byte_index += 2U)
    {
        if ((byte_index + 2U) >= bytes)
        {
            /*  CS pin level is negated at the end of last data transmission */
            p_instance_ctrl->p_reg->SMCR_b.SSLKP = 0U;
        }

        r_sfma_start_transfer(p_instance_ctrl);

        p_dest[byte_index] = (uint8_t) (p_instance_ctrl->p_reg->SMRDR >> SFMA_2BYTE_TRANSFER_DATA_POS);
        if ((byte_index + 1U) < bytes)
        {
            p_dest[byte_index + 1U] = (uint8_t) (p_instance_ctrl->p_reg->SMRDR >> SFMA_1BYTE_TRANSFER_DATA_POS);
        }
    }

    /* Disable Read, Write when completed  */
    p_instance_ctrl->p_reg->SMCR = 0U;

    /* Return to External Address Space Read Mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read/Write raw data directly with the SerialFlash.
 *
 * Implements @ref spi_flash_api_t::directTransfer.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT    Invalid p_transfer.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_DirectTransfer (spi_flash_ctrl_t * const            p_ctrl,  /* PRQA S 1505 # Advisory R-8.7 */
                                 spi_flash_direct_transfer_t * const p_transfer,
                                 spi_flash_direct_transfer_dir_t     direction)
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_transfer);
    FSP_ERROR_RETURN(8U >= p_transfer->dummy_cycles, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN((4U >= p_transfer->data_length) && (p_transfer->data_length != 3u), FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(4U >= p_transfer->address_length, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(2U >= p_transfer->command_length, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(0U != (p_transfer->data_length + p_transfer->address_length + p_transfer->command_length),
                     FSP_ERR_INVALID_ARGUMENT);
#endif

    uint32_t smenr = p_instance_ctrl->p_reg->SMENR;

    /* Verify device is not busy when write data . */
    FSP_ERROR_RETURN(!r_sfma_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);

    /* Switch to SPI Operating mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 1U;

    smenr &= SFMA_SPI_CLEAR_SMENR_MASK;

    /* Enable Command, Option Command, */
    smenr |= R_SFMA0_SMENR_CDE_Msk;
    p_instance_ctrl->p_reg->SMCMR =
        (((uint32_t) p_transfer->command << R_SFMA0_SMCMR_CMD_Pos) & R_SFMA0_SMCMR_CMD_Msk) |
        (((uint32_t) p_transfer->command >> 8U) & R_SFMA0_SMCMR_OCMD_Msk);
    if (p_transfer->command_length > 1U)
    {
        smenr |= R_SFMA0_SMENR_OCDE_Msk;
    }

    /* Enable Address */
    smenr |= g_address_enable[p_transfer->address_length] << R_SFMA0_SMENR_ADE_Pos;
    if (p_transfer->address_length > 3U)
    {
        p_instance_ctrl->p_reg->SMADR = p_transfer->address;
    }
    else
    {
        p_instance_ctrl->p_reg->SMADR = ((uint32_t) p_transfer->address) <<
                                        (24U - (((uint32_t) p_transfer->address_length) << 3U));
    }

    /* Dummy Cycles */
    if (p_transfer->dummy_cycles > 0U)
    {
        smenr |= R_SFMA0_SMENR_DME_Msk;
        p_instance_ctrl->p_reg->SMDMCR_b.DMCYC = (p_transfer->dummy_cycles - 1U) & 0x07U;
    }

    /* Enable Data Transmission or Reception */
    smenr |= g_data_enable[p_transfer->data_length];

    p_instance_ctrl->p_reg->SMENR = smenr;
    if (SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE == direction)
    {
        r_sfma_write_enable(p_instance_ctrl);
        p_instance_ctrl->p_reg->SMWDR = p_transfer->data_transfer.data << g_data_pos[p_transfer->data_length];
    }
    else
    {
        r_sfma_read_enable(p_instance_ctrl);
    }

    r_sfma_start_transfer(p_instance_ctrl);
    p_instance_ctrl->p_reg->SMCR = 0x00;

    /* Read data if direction is Read */
    if (SPI_FLASH_DIRECT_TRANSFER_DIR_READ == direction)
    {
        p_transfer->data_transfer.data = p_instance_ctrl->p_reg->SMRDR >> g_data_pos[p_transfer->data_length];
    }

    /* Return to External Address Space Read Mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;

    p_instance_ctrl->p_reg->DRCR_b.RCF = 1U;

    /* Flush read cache to ensure memory-mapped reads are coherent after SPI write/read operations */
    volatile uint32_t dummy = p_instance_ctrl->p_reg->DRCR_b.RCF;
    FSP_PARAMETER_NOT_USED(dummy);

    /* Disable Dummy Cycles */
    p_instance_ctrl->p_reg->SMENR_b.DME = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enters XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipEnter.
 *
 * @retval FSP_ERR_UNSUPPORTED          API not supported by SFMA
 **********************************************************************************************************************/
fsp_err_t R_SFMA_XipEnter (spi_flash_ctrl_t * const p_ctrl) /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Exits XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipExit.
 *
 * @retval FSP_ERR_UNSUPPORTED          API not supported by SFMA
 **********************************************************************************************************************/
fsp_err_t R_SFMA_XipExit (spi_flash_ctrl_t * const p_ctrl) /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Program a page of data to the flash.
 *
 * Implements @ref spi_flash_api_t::write.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl, p_dest or p_src is NULL, or byte_count crosses a page boundary.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_Write (spi_flash_ctrl_t * const p_ctrl,                      /* PRQA S 1505 # Advisory R-8.7 */
                        uint8_t const * const    p_src,
                        uint8_t * const          p_dest,                      /* PRQA S 3673 # Advisory R-8.13 */
                        uint32_t                 byte_count)
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
    uint32_t page_size_bytes    = p_instance_ctrl->p_cfg->page_size_bytes;
    uint32_t bytes_left_in_page = page_size_bytes - ((uint32_t) p_dest % page_size_bytes); /* PRQA S 0329 # Advisory R-11.4 */
    FSP_ASSERT(byte_count <= bytes_left_in_page);
#endif

    /* Verify device is not busy when write data . */
    FSP_ERROR_RETURN(!r_sfma_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);

    /* Switch to SPI Operating mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 1U;

    /* Reset CDE, ADE, SPIDE bit  */
    p_instance_ctrl->p_reg->SMENR = p_instance_ctrl->p_reg->SMENR & SFMA_SPI_CLEAR_SMENR_MASK;

    /* Enable Command, Disable address, data to transmit enable command  */
    p_instance_ctrl->p_reg->SMENR_b.CDE = 1U;
    p_instance_ctrl->p_reg->SMCMR_b.CMD = p_instance_ctrl->p_cfg->write_enable_command;

    r_sfma_start_transfer(p_instance_ctrl);

    /* Enable Command, Enable address,to transmit write command, address  */
    p_instance_ctrl->p_reg->SMCMR_b.CMD  = p_instance_ctrl->p_cfg->page_program_command;
    p_instance_ctrl->p_reg->SMENR_b.ADE  = (uint8_t) (((uint32_t) p_instance_ctrl->p_cfg->address_bytes) & 0x0FU);
    p_instance_ctrl->p_reg->SMADR        = (uint32_t) (p_dest - SFMA_DEVICE_START_ADDRESS); /* PRQA S 0329, 0488 # Advisory R-11.4, Advisory R-18.4 */
    p_instance_ctrl->p_reg->SMCR_b.SSLKP = 1U;

    r_sfma_start_transfer(p_instance_ctrl);

    /* Disable command, disable address */
    p_instance_ctrl->p_reg->SMENR = p_instance_ctrl->p_reg->SMENR & SFMA_SPI_CLEAR_SMENR_MASK;

    /* Enable data transmit  */
    r_sfma_write_enable(p_instance_ctrl);

    uint32_t byte_offset = 0U;

    /* Transfer 32-bit chunks if possible (aligned and >= 4 bytes) */
    if ((byte_count >= 4U) && (((uint32_t) p_src & 0x3U) == 0U)) /* PRQA S 0329 # Advisory R-11.4 */
    {
        /* Set 32-bit transfer mode */
        p_instance_ctrl->p_reg->SMENR_b.SPIDE = SFMA_SPIDE_32BIT_TRANSFER;

        uint32_t         chunk_count_32 = byte_count / 4U;
        uint32_t const * p_src_32       = (uint32_t const *) p_src; /* PRQA S 3305, 0310 # DP_MSR_R-11.3_UC1 */

        for (uint32_t chunk_index = 0U; chunk_index < chunk_count_32; chunk_index++)
        {
            /* Check if this is the last transfer overall */
            if (((chunk_index + 1U) == chunk_count_32) && ((byte_count % 4U) == 0U))
            {
                /* CS pin will be negated at the end of the last transfer */
                p_instance_ctrl->p_reg->SMCR_b.SSLKP = 0U;
            }

            p_instance_ctrl->p_reg->SMWDR = p_src_32[chunk_index];
            r_sfma_start_transfer(p_instance_ctrl);
        }

        byte_offset = chunk_count_32 * 4U;
    }

    /* Transfer remaining 16-bit chunks if possible */
    if (((byte_count - byte_offset) >= 2U) && (((uint32_t) (p_src + byte_offset) & 0x1U) == 0U)) /* PRQA S 0329, 0488 # Advisory R-11.4, Advisory R-18.4 */
    {
        /* Switch to 16-bit transfer mode */
        p_instance_ctrl->p_reg->SMENR_b.SPIDE = SFMA_SPIDE_16BIT_TRANSFER;

        uint32_t         remaining      = byte_count - byte_offset;
        uint32_t         chunk_count_16 = remaining / 2U;
        uint16_t const * p_src_16       = (uint16_t const *) (p_src + byte_offset); /* PRQA S 3305, 0310, 0488 # DP_MSR_R-11.3_UC1, Advisory R-18.4 */

        for (uint32_t chunk_index = 0U; chunk_index < chunk_count_16; chunk_index++)
        {
            /* Check if this is the last transfer overall */
            if (((chunk_index + 1U) == chunk_count_16) && ((remaining % 2U) == 0U))
            {
                /* CS pin will be negated at the end of the last transfer */
                p_instance_ctrl->p_reg->SMCR_b.SSLKP = 0U;
            }

            p_instance_ctrl->p_reg->SMWDR = (uint32_t) p_src_16[chunk_index] << SFMA_2BYTE_TRANSFER_DATA_POS;
            r_sfma_start_transfer(p_instance_ctrl);
        }

        byte_offset += chunk_count_16 * 2U;
    }

    /* Transfer remaining bytes in 8-bit mode */
    if (byte_offset < byte_count)
    {
        /* Switch to 8-bit transfer mode */
        p_instance_ctrl->p_reg->SMENR_b.SPIDE = SFMA_SPIDE_8BIT_TRANSFER;

        for ( ; byte_offset < byte_count; byte_offset++)
        {
            if ((byte_offset + 1U) == byte_count)
            {
                /* CS pin will be negated at the end of the last byte */
                p_instance_ctrl->p_reg->SMCR_b.SSLKP = 0U;
            }

            p_instance_ctrl->p_reg->SMWDR = (uint32_t) p_src[byte_offset] << SFMA_1BYTE_TRANSFER_DATA_POS;
            r_sfma_start_transfer(p_instance_ctrl);
        }
    }

    /* Disable Read/Write when completed */
    p_instance_ctrl->p_reg->SMCR = 0x00U;

    /* Return to External Address Space Read Mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;

    p_instance_ctrl->p_reg->DRCR_b.RCF = 1U;

    /* Flush read cache to ensure memory-mapped reads are coherent after write */
    volatile uint32_t dummy = p_instance_ctrl->p_reg->DRCR_b.RCF;
    FSP_PARAMETER_NOT_USED(dummy);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Erase a block or sector of flash.  The byte_count must exactly match one of the erase sizes
 * defined in spi_flash_cfg_t. For chip erase, byte_count must be SPI_FLASH_ERASE_SIZE_CHIP_ERASE.
 *
 * Implements @ref spi_flash_api_t::erase.
 *
 * @retval FSP_SUCCESS                 The command to erase the flash was executed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_device_address is NULL, or byte_count doesn't match an
 *                                     erase
 *                                     size defined in spi_flash_cfg_t, or device is in XIP mode.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_ARGUMENT    p_device_address or byte_count is invalid.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_Erase (spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_device_address, uint32_t byte_count) /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl;                                   /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_device_address);
#endif
    uint32_t smenr;

    /* Verify device is not busy when write data . */
    FSP_ERROR_RETURN(!r_sfma_status_sub(p_instance_ctrl), FSP_ERR_DEVICE_BUSY);
    uint16_t erase_command = 0U;
    uint32_t chip_address  = (uint32_t) p_device_address - SFMA_DEVICE_START_ADDRESS + /* PRQA S 0329 # Advisory R-11.4 */
                             (((uint32_t) p_instance_ctrl->p_reg->DREAR_b.EAV) << SFMA_ADDRESS_BANK_POS_SHIFT);
    bool send_address = true;

    for (uint32_t index = 0; index < p_instance_ctrl->p_cfg->erase_command_list_length; index++)
    {
        /* If requested byte_count is supported by underlying flash, store the command. */
        if (p_instance_ctrl->p_cfg->p_erase_command_list[index].size == byte_count)
        {
            if (SPI_FLASH_ERASE_SIZE_CHIP_ERASE == byte_count) /* PRQA S 1281 # DP_MSR_R-7.2_UC1 */
            {
                /* Don't send address for chip erase. */
                send_address = false;
            }

            erase_command = p_instance_ctrl->p_cfg->p_erase_command_list[index].command;
            break;
        }
    }

#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ERROR_RETURN(0U != erase_command, FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN((0U == ((uint32_t) p_device_address % byte_count)) || /* PRQA S 1281, 0329 ++ # DP_MSR_R-7.2_UC1, Advisory R-11.4 */
                     (byte_count == SPI_FLASH_ERASE_SIZE_CHIP_ERASE),
                     FSP_ERR_INVALID_ARGUMENT);
#endif

    /* Switch to SPI Operating mode  */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 1U;

    /* Transmit Write Enable command  */
    smenr  = p_instance_ctrl->p_reg->SMENR;
    smenr &= SFMA_SPI_CLEAR_SMENR_MASK;
    smenr |= R_SFMA0_SMENR_CDE_Msk;
    p_instance_ctrl->p_reg->SMENR       = smenr;
    p_instance_ctrl->p_reg->SMCMR_b.CMD = p_instance_ctrl->p_cfg->write_enable_command;

    r_sfma_start_transfer(p_instance_ctrl);

    /* Transmit Erase command, address  */
    p_instance_ctrl->p_reg->SMCMR_b.CMD = (uint8_t) erase_command;

    if (send_address)
    {
        p_instance_ctrl->p_reg->SMENR_b.ADE = ((uint32_t) p_instance_ctrl->p_cfg->address_bytes) & 0x0FU;
        p_instance_ctrl->p_reg->SMADR       = chip_address;
    }

    r_sfma_start_transfer(p_instance_ctrl);

    p_instance_ctrl->p_reg->SMCR = 0x00U;

    /* Return to External Address Space Read Mode */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;

    p_instance_ctrl->p_reg->DRCR_b.RCF = 1U;

    /* Flush read cache to ensure memory-mapped reads are coherent after erase */
    volatile uint32_t dummy = p_instance_ctrl->p_reg->DRCR_b.RCF;
    FSP_PARAMETER_NOT_USED(dummy);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Gets the write or erase status of the flash.
 *
 * Implements @ref spi_flash_api_t::statusGet.
 *
 * @retval FSP_SUCCESS                 The write status is in p_status.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_status is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_StatusGet (spi_flash_ctrl_t * const p_ctrl, spi_flash_status_t * const p_status) /* PRQA S 1505 # Advisory R-8.7 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl;                     /* PRQA S 0316 # Advisory R-11.5 */

#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read device status. True if busy, false if not. */
    p_status->write_in_progress = r_sfma_status_sub(p_instance_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Selects the bank to access.
 *
 * This function is not required for memory devices less than or equal to 512 Mb (64MB).
 *
 * Implements @ref spi_flash_api_t::bankSet.
 *
 * @retval FSP_SUCCESS                 Bank successfully selected.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_MODE        This function can't be called when using 3 bytes address.
 *
 **********************************************************************************************************************/
fsp_err_t R_SFMA_BankSet (spi_flash_ctrl_t * const p_ctrl, uint32_t bank)     /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(p_instance_ctrl->p_reg->DRENR_b.ADE == 0xFU, FSP_ERR_INVALID_MODE);
#endif

    p_instance_ctrl->p_reg->DREAR_b.EAV = bank & SFMA_EAV_BIT_MASK;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Sets the SPI protocol.
 *
 * Implements @ref spi_flash_api_t::spiProtocolSet.
 *
 * @retval FSP_SUCCESS                SPI protocol updated on MCU peripheral.
 * @retval FSP_ERR_ASSERTION          A required pointer is NULL.
 * @retval FSP_ERR_INVALID_ARGUMENT   Invalid SPI Protocol.
 * @retval FSP_ERR_NOT_OPEN           Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_SpiProtocolSet (spi_flash_ctrl_t * const p_ctrl, spi_flash_protocol_t spi_protocol) /* PRQA S 1505 # Advisory R-8.7 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl;                        /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN((SPI_FLASH_PROTOCOL_DOPI < spi_protocol) ||
                     (SPI_FLASH_PROTOCOL_1S_1S_1S == spi_protocol),
                     FSP_ERR_INVALID_ARGUMENT);
#endif
    uint32_t smenr = p_instance_ctrl->p_reg->SMENR;
    p_instance_ctrl->spi_protocol = spi_protocol;

    smenr &= SFMA_SPI_CLEAR_SMENR_MASK;

    /* Initial SPI Operating mode */
    smenr |= (((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_SMENR_CDB_Pos | /* PRQA S 3397 # Advisory R-12.1 */
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_SMENR_OCDB_Pos |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_SMENR_ADB_Pos - 3U) |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_SMENR_OPDB_Pos - 3U) |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_DATA_MASK) << (R_SFMA0_SMENR_SPIDB_Pos - 6U));
    p_instance_ctrl->p_reg->SMENR = smenr;

    /* Update DRENR (External Address Space Read Mode) to match SPI protocol */
    uint32_t drenr = p_instance_ctrl->p_reg->DRENR;

    /* Clear CDB, OCDB, ADB, OPDB, DRDB bits */
    drenr &= ~(R_SFMA0_DRENR_CDB_Msk | R_SFMA0_DRENR_OCDB_Msk | R_SFMA0_DRENR_ADB_Msk |
               R_SFMA0_DRENR_OPDB_Msk | R_SFMA0_DRENR_DRDB_Msk);

    /* Set protocol bits in DRENR */
    drenr |= (((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_DRENR_CDB_Pos | /* PRQA S 3397 # Advisory R-12.1 */
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_COMMAND_MASK) << R_SFMA0_DRENR_OCDB_Pos |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_DRENR_ADB_Pos - 3U) |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_ADDRESS_MASK) << (R_SFMA0_DRENR_OPDB_Pos - 3U) |
              ((uint32_t) spi_protocol & SFMA_SPI_PROTOCOL_DATA_MASK) << (R_SFMA0_DRENR_DRDB_Pos - 6U));
    p_instance_ctrl->p_reg->DRENR = drenr;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Auto-calibrate the OctaRAM device using the preamble pattern. Unsupported by SFMA.
 * Implements @ref spi_flash_api_t::autoCalibrate.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by SFMA
 **********************************************************************************************************************/
fsp_err_t R_SFMA_AutoCalibrate (spi_flash_ctrl_t * const p_ctrl) /* PRQA S 1505, 3673 # Advisory R-8.7 , Advisory R-8.13 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * Close the SFMA driver module.
 *
 * Implements @ref spi_flash_api_t::close.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_Close (spi_flash_ctrl_t * const p_ctrl)                      /* PRQA S 1505 # Advisory R-8.7 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Clear all the entries in the read cache.
 *
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_CacheClear (spi_flash_ctrl_t * const p_ctrl)                 /* PRQA S 1503, 3673 # DP_MSR_R-2.1_UC1, Advisory R-8.13 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_reg->DRCR_b.RCF = 1U;

    volatile uint32_t dummy = p_instance_ctrl->p_reg->DRCR_b.RCF;
    FSP_PARAMETER_NOT_USED(dummy);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Immediately negate Chip Select pin, only available in Read Burst Mode
 *
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_INVALID_MODE    Not in Read Burst Mode with CS Negate when the accessed address is not continuous.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_SFMA_NegateChipSelect (spi_flash_ctrl_t * const p_ctrl)           /* PRQA S 1503, 3673 # DP_MSR_R-2.1_UC1, Advisory R-8.13 */
{
    sfma_instance_ctrl_t * p_instance_ctrl = (sfma_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Advisory R-11.5, Advisory R-8.13 */
#if SFMA_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN((p_instance_ctrl->p_reg->DRCR_b.SSLE == 1U) && (p_instance_ctrl->p_reg->DRCR_b.RBE == 1U),
                     FSP_ERR_INVALID_MODE);
    FSP_ERROR_RETURN(SFMA_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    p_instance_ctrl->p_reg->DRCR_b.SSLN = 1U;
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CMNSR_b.SSLF, 0U);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup SFMA)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Gets device status.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 *
 * @return True if busy, false if not.
 **********************************************************************************************************************/
static bool r_sfma_status_sub (sfma_instance_ctrl_t * p_instance_ctrl)
{
    /* Store current Control setting */
    uint32_t enr    = p_instance_ctrl->p_reg->SMENR;
    uint32_t smenr  = p_instance_ctrl->p_reg->SMENR & SFMA_SPI_CLEAR_SMENR_MASK;
    uint32_t smcr   = p_instance_ctrl->p_reg->SMCR;
    uint8_t  status = 0U;

    /* Switch to SPI Operating mode  */
    p_instance_ctrl->p_reg->CMNCR_b.MD = 1U;

    r_sfma_read_enable(p_instance_ctrl);

    /* Preserve command/address protocol bits from current setting */
    uint32_t cmd_protocol_bits = smenr & (R_SFMA0_SMENR_CDB_Msk | R_SFMA0_SMENR_OCDB_Msk |
                                          R_SFMA0_SMENR_ADB_Msk | R_SFMA0_SMENR_OPDB_Msk);

    /* Extract command bit width (CDB) to determine data bit width */
    /* For Read Status Register, data protocol must match command protocol */
    /* In QPI mode (CDB=10): use 4-line data (SPIDB=10) */
    /* In Standard/Quad mode (CDB=00): use 1-line data (SPIDB=00) */
    uint32_t cdb_value   = (smenr & R_SFMA0_SMENR_CDB_Msk);
    uint32_t spidb_value = (cdb_value >> (R_SFMA0_SMENR_CDB_Pos - R_SFMA0_SMENR_SPIDB_Pos));

    /* Clear enable bits and protocol bits */
    smenr &= SFMA_SPI_CLEAR_SMENR_MASK;
    smenr &= ~(R_SFMA0_SMENR_CDB_Msk | R_SFMA0_SMENR_OCDB_Msk | R_SFMA0_SMENR_ADB_Msk |
               R_SFMA0_SMENR_OPDB_Msk | R_SFMA0_SMENR_SPIDB_Msk);

    /* Transmit status command and read 8 bit data */
    smenr |= R_SFMA0_SMENR_CDE_Msk;    /* Enable command */
    smenr |= SFMA_SPIDE_8BIT_TRANSFER; /* Set SPIDE to 8-bit data transfer size */

    /* Restore command/address protocol and set data protocol to match command */
    smenr |= cmd_protocol_bits | spidb_value;

    p_instance_ctrl->p_reg->SMENR       = smenr;
    p_instance_ctrl->p_reg->SMCMR_b.CMD = p_instance_ctrl->p_cfg->status_command;

    r_sfma_start_transfer(p_instance_ctrl);

    /* Restore current Control setting */
    p_instance_ctrl->p_reg->SMENR      = enr;
    p_instance_ctrl->p_reg->SMCR       = smcr;
    p_instance_ctrl->p_reg->CMNCR_b.MD = 0U;

    status = (uint8_t) ((p_instance_ctrl->p_reg->SMRDR >> p_instance_ctrl->p_cfg->write_status_bit) & 1U);

    return status != 0U;
}

/*******************************************************************************************************************//**
 * Enable Write data and Disable Read data.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 **********************************************************************************************************************/
static void r_sfma_write_enable (sfma_instance_ctrl_t * p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t smcr = p_instance_ctrl->p_reg->SMCR;
    smcr |= R_SFMA0_SMCR_SPIWE_Msk;
    smcr &= ~R_SFMA0_SMCR_SPIRE_Msk;
    p_instance_ctrl->p_reg->SMCR = smcr;
}

/*******************************************************************************************************************//**
 * Enable Read data and Disable Write data.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 **********************************************************************************************************************/
static void r_sfma_read_enable (sfma_instance_ctrl_t * p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    uint32_t smcr = p_instance_ctrl->p_reg->SMCR;
    smcr &= ~R_SFMA0_SMCR_SPIWE_Msk;
    smcr |= R_SFMA0_SMCR_SPIRE_Msk;
    p_instance_ctrl->p_reg->SMCR = smcr;
}

/*******************************************************************************************************************//**
 * Start the SPI transfer operation and wait for it to complete.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 **********************************************************************************************************************/
static void r_sfma_start_transfer (sfma_instance_ctrl_t * p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    p_instance_ctrl->p_reg->SMCR_b.SPIE = 1U;
    FSP_HARDWARE_REGISTER_WAIT(p_instance_ctrl->p_reg->CMNSR_b.TEND, 1U);
}

/* PRQA S 0662 -- */

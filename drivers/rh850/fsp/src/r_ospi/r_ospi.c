/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_ospi.h"
#include <string.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "xSPI" in ASCII.  Used to determine if the control block is open. */
#define OSPI_PRV_OPEN                   (0x78535049U)

#define OSPI_PRV_CHANNELS_PER_UNIT      (2U)
#define OSPI_PRV_UNIT_CHANNELS_SHIFT    (OSPI_PRV_CHANNELS_PER_UNIT)
#define OSPI_PRV_UNIT_CHANNELS_MASK     ((1U << OSPI_PRV_UNIT_CHANNELS_SHIFT) - 1U)

/**
 * Mask of all channels for a given OSPI unit.
 * @param p_ext_cfg Pointer to a OSPI extended config struct with the unit to mask.
 */
#define OSPI_PRV_UNIT_MASK(p_ext_cfg)                 ((uint32_t) OSPI_PRV_UNIT_CHANNELS_MASK << \
                                                       ((uint32_t) ((p_ext_cfg)->ospi_unit) *    \
                                                        OSPI_PRV_UNIT_CHANNELS_SHIFT))

/**
 * Individual bit mask for a single channel on a given OSPI unit.
 * @param p_ext_cfg Pointer to a OSPI extended config struct with the unit and channel to mask.
 */
#define OSPI_PRV_CH_MASK(p_ext_cfg)                   ((1U << (uint32_t) ((p_ext_cfg)->channel)) << \
                                                       ((uint32_t) ((p_ext_cfg)->ospi_unit) *       \
                                                        OSPI_PRV_UNIT_CHANNELS_SHIFT))

/**
 * Gets the extended configuration struct for this instance.
 * @param p_ctrl Pointer to the instance control block.
 */
#define OSPI_PRV_EXTENDED_CFG(p_ctrl)                 ((const ospi_extended_cfg_t *) \
                                                       ((const ospi_instance_ctrl_t *) (p_ctrl))->p_cfg->p_extend)

/** Indicates the provided protocol mode requires the Data-Strobe signal. */
#define OSPI_PRV_PROTOCOL_USES_DS_SIGNAL(protocol)    ((bool) ((((uint32_t) (protocol)) & 0x200U) != 0U))

/** Number of bytes combined into a single transaction for memory-mapped writes. */
#define OSPI_PRV_COMBINATION_WRITE_LENGTH              (2U * ((uint8_t) OSPI_CFG_COMBINATION_FUNCTION + 1U))

/** Converts @ref spi_flash_address_bytes_t to a register compatible length value. */
#define OSPI_PRV_ADDR_BYTES_TO_LENGTH(spi_flash_bytes)            \
    ((uint8_t) (                                                  \
         ((spi_flash_bytes) == SPI_FLASH_ADDRESS_BYTES_3) ? 3U :  \
         (((spi_flash_bytes) == SPI_FLASH_ADDRESS_BYTES_4) ? 4U : \
          (uint8_t) ((uint32_t) (spi_flash_bytes) + 1U))))

#define OSPI_PRV_BMCTL_DEFAULT_VALUE                   (0x0C)

#define OSPI_PRV_CMCFG_1BYTE_VALUE_MASK                (0xFF00U)
#define OSPI_PRV_CMCFG_2BYTE_VALUE_MASK                (0xFFFFU)

#define OSPI_PRV_AUTOCALIBRATION_DATA_SIZE             (0x1U)
#define OSPI_PRV_AUTOCALIBRATION_LATENCY_CYCLES        (0U)

#define OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_0    (0xFFFF0000U)
#define OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_1    (0x000800FFU)
#define OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_2    (0x00FFF700U)
#define OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_3    (0xF700F708U)

#define OSPI_PRV_ADDRESS_REPLACE_VALUE                 (0xF0U)
#define OSPI_PRV_ADDRESS_REPLACE_ENABLE_BITS           ((uint32_t) OSPI_PRV_ADDRESS_REPLACE_VALUE << \
                                                        R_OSPI_CMCFGCS_CMCFG0_ADDRPEN_Pos)
#define OSPI_PRV_ADDRESS_REPLACE_MASK                  (~((uint32_t) OSPI_PRV_ADDRESS_REPLACE_VALUE << 24U))

#define OSPI_PRV_AUTOCALIBRATION_FRAME_INTERVAL        (0x1FU)
#define OSPI_PRV_AUTOCALIBRATION_NO_WRITE_CMD          (0x1U)
#define OSPI_PRV_AUTOCALIBRATION_SHIFT_DS_END_VALUE    (0x1FU)

#define OSPI_PRV_WORD_ACCESS_SIZE                      (4U)
#define OSPI_PRV_HALF_WORD_ACCESS_SIZE                 (2U)

#define OSPI_PRV_DIRECT_ADDR_AND_DATA_MASK             (7U)
#define OSPI_PRV_PAGE_SIZE_BYTES                       (256U)

#define OSPI_PRV_DIRECT_CMD_SIZE_MASK                  (0x3U)

#define OSPI_PRV_BMCFGCH0_WRMD_VALUE                   (0U)

#define OSPI_PRV_CDTBUF_CMD_OFFSET                     (16U)
#define OSPI_PRV_CDTBUF_CMD_UPPER_OFFSET               (24U)
#define OSPI_PRV_CDTBUF_CMD_1B_VALUE_MASK              (0xFFU)
#define OSPI_PRV_CDTBUF_CMD_1B_VALUE_SHIFT             (8U)
#define OSPI_PRV_CDTBUF_CMD_2B_VALUE_MASK              (0xFFFFU)

#define OSPI_PRV_BMCTL0_DISABLED_VALUE                 (0x00) // 0b0000'0000
#define OSPI_PRV_BMCTL0_READ_ONLY_VALUE                (0x55) // 0b0101'0101
#define OSPI_PRV_BMCTL0_WRITE_ONLY_VALUE               (0xAA) // 0b1010'1010
#define OSPI_PRV_BMCTL0_READ_WRITE_VALUE               (0xFF) // 0b1111'1111

#define OSPI_PRV_BMCTL1_CLEAR_PREFETCH_MASK            ((uint32_t) 0x01U << R_OSPI_BMCTL1_PBUFCLRCH0_Pos)
#define OSPI_PRV_BMCTL1_PUSH_COMBINATION_WRITE_MASK    ((uint32_t) 0x01U << R_OSPI_BMCTL1_MWRPUSHCH0_Pos)

#define OSPI_PRV_COMSTT_MEMACCCH_MASK                  ((uint32_t) 0x01U << R_OSPI_COMSTT_MEMACCCH0_Pos)
#define OSPI_PRV_COMSTT_WRBUFNECH_MASK                 ((uint32_t) 0x01U << R_OSPI_COMSTT_WRBUFNECH0_Pos)

#define OSPI_PRV_COMSTT_PENDING_ACTION_MASK            (OSPI_PRV_COMSTT_MEMACCCH_MASK | \
                                                        OSPI_PRV_COMSTT_WRBUFNECH_MASK)

#define OSPI_SOFTWARE_DELAY                            (50U)

/* These are used as modulus checking, make sure they are powers of 2. */
#define OSPI_PRV_CPU_ACCESS_LENGTH                     (4U)
#define OSPI_PRV_CPU_ACCESS_ALIGNMENT                  (4U)

#define OSPI_PRV_PROTOCOL_USES_DS_MASK                 (0x200U)

#define OSPI_PRV_UINT32_BITS                           (32U)

#ifndef OSPI_MAX_WRITE_ENABLE_LOOPS
 #define OSPI_MAX_WRITE_ENABLE_LOOPS                   (5U)
#endif

/* Number of address bytes in 4 byte address mode. */
#define OSPI_4_BYTE_ADDRESS                            (4U)

/* OSPI channel mask */
#define OSPI_CHANNEL_MASK                              (0x1U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static bool      r_ospi_status_sub(ospi_instance_ctrl_t * p_instance_ctrl, uint8_t bit_pos);
static fsp_err_t r_ospi_protocol_specific_settings(ospi_instance_ctrl_t * p_instance_ctrl);
static fsp_err_t r_ospi_write_enable(ospi_instance_ctrl_t * p_instance_ctrl);
static void      r_ospi_direct_transfer(ospi_instance_ctrl_t const        * p_instance_ctrl,
                                        spi_flash_direct_transfer_t * const p_transfer,
                                        spi_flash_direct_transfer_dir_t     direction);
static ospi_xspi_command_set_t const * r_ospi_command_set_get(ospi_instance_ctrl_t const * p_instance_ctrl);

#if OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE
static fsp_err_t r_ospi_automatic_calibration_seq(ospi_instance_ctrl_t const * p_instance_ctrl);

#endif

#if OSPI_CFG_XIP_SUPPORT_ENABLE
static void r_ospi_dummy_read(uint32_t * p_read_address);
static void r_ospi_xip(ospi_instance_ctrl_t * p_instance_ctrl, bool is_entering);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* Bit-flags specifying which channels are open so the module can be stopped when all are closed. */
static uint32_t g_ospi_channels_open_flags = 0;

/*******************************************************************************************************************//**
 * @addtogroup OSPI
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

const spi_flash_api_t g_ospi_on_spi_flash =
{
    .open           = R_OSPI_Open,
    .directWrite    = R_OSPI_DirectWrite,
    .directRead     = R_OSPI_DirectRead,
    .directTransfer = R_OSPI_DirectTransfer,
    .spiProtocolSet = R_OSPI_SpiProtocolSet,
    .write          = R_OSPI_Write,
    .erase          = R_OSPI_Erase,
    .statusGet      = R_OSPI_StatusGet,
    .xipEnter       = R_OSPI_XipEnter,
    .xipExit        = R_OSPI_XipExit,
    .bankSet        = R_OSPI_BankSet,
    .close          = R_OSPI_Close,
    .autoCalibrate  = R_OSPI_AutoCalibrate,
};

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Open the xSPI device. After the driver is open, the xSPI device can be accessed like internal flash memory.
 *
 * Implements @ref spi_flash_api_t::open.
 *
 * @retval FSP_SUCCESS              Configuration was successful.
 * @retval FSP_ERR_ASSERTION        The parameter p_ctrl or p_cfg is NULL.
 * @retval FSP_ERR_ALREADY_OPEN     Driver has already been opened with the same p_ctrl.
 * @retval FSP_ERR_CALIBRATE_FAILED Failed to perform auto-calibrate.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_Open (spi_flash_ctrl_t * const p_ctrl, spi_flash_cfg_t const * const p_cfg)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;
    fsp_err_t              ret             = FSP_SUCCESS;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    const ospi_extended_cfg_t * const p_cfg_extend = (const ospi_extended_cfg_t *) (p_cfg->p_extend);

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT((OSPI_CHANNEL_MASK & (1U << p_cfg_extend->ospi_unit)) != 0U);
    FSP_ERROR_RETURN(0U == (g_ospi_channels_open_flags & OSPI_PRV_CH_MASK(p_cfg_extend)), FSP_ERR_ALREADY_OPEN);
#endif

    R_OSPI_Type * p_reg = (R_OSPI_Type *) ((uint32_t) R_OSPI);

    /* Initialize control block. */
    p_instance_ctrl->p_cfg        = p_cfg;
    p_instance_ctrl->p_reg        = p_reg;
    p_instance_ctrl->spi_protocol = p_cfg->spi_protocol;
    p_instance_ctrl->channel      = p_cfg_extend->channel;
    p_instance_ctrl->ospi_unit    = p_cfg_extend->ospi_unit;

    /* Disable memory-mapping for this slave. It will be enabled later on after initialization. */
    if (OSPI_DEVICE_NUMBER_0 == p_instance_ctrl->channel)
    {
        p_reg->BMCTL0 &= ~R_OSPI_BMCTL0_CH0CS0ACC_Msk;
    }
    else
    {
        p_reg->BMCTL0 &= ~R_OSPI_BMCTL0_CH0CS1ACC_Msk;
    }

    /* Perform xSPI Initial configuration as described in hardware manual (see
     * "Flows of Operations" in the OSPI section of the relevant hardware manual). */

    /* Set xSPI protocol mode. */
    uint32_t liocfg = ((uint32_t) p_cfg->spi_protocol) << R_OSPI_LIOCFGCS_PRTMD_Pos;
    p_reg->LIOCFGCS[p_cfg_extend->channel] = liocfg;

    /* Set xSPI drive/sampling timing. */
    if (OSPI_DEVICE_NUMBER_0 == p_instance_ctrl->channel)
    {
        p_reg->WRAPCFG = ((uint32_t) p_cfg_extend->data_latch_delay_clocks << R_OSPI_WRAPCFG_DSSFTCS0_Pos) &
                         R_OSPI_WRAPCFG_DSSFTCS0_Msk;
    }
    else
    {
        p_reg->WRAPCFG = ((uint32_t) p_cfg_extend->data_latch_delay_clocks << R_OSPI_WRAPCFG_DSSFTCS1_Pos) &
                         R_OSPI_WRAPCFG_DSSFTCS1_Msk;
    }

    /* Set minimum cycles between xSPI frames. */
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->command_to_command_interval << R_OSPI_LIOCFGCS_CSMIN_Pos) &
              R_OSPI_LIOCFGCS_CSMIN_Msk;

    /* Set CS asserting extension in cycles */
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->cs_pulldown_lead << R_OSPI_LIOCFGCS_CSASTEX_Pos) &
              R_OSPI_LIOCFGCS_CSASTEX_Msk;

    /* Set CS releasing extension in cycles */
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->cs_pullup_lag << R_OSPI_LIOCFGCS_CSNEGEX_Pos) &
              R_OSPI_LIOCFGCS_CSNEGEX_Msk;

    /* Set SDR and DDR timing. */
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->sdr_drive_timing << R_OSPI_LIOCFGCS_SDRDRV_Pos) &
              R_OSPI_LIOCFGCS_SDRDRV_Msk;
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->sdr_sampling_edge << R_OSPI_LIOCFGCS_SDRSMPMD_Pos) &
              R_OSPI_LIOCFGCS_SDRSMPMD_Msk;
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->sdr_sampling_delay << R_OSPI_LIOCFGCS_SDRSMPSFT_Pos) &
              R_OSPI_LIOCFGCS_SDRSMPSFT_Msk;
    liocfg |= ((uint32_t) p_cfg_extend->p_timing_settings->ddr_sampling_extension << R_OSPI_LIOCFGCS_DDRSMPEX_Pos) &
              R_OSPI_LIOCFGCS_DDRSMPEX_Msk;

    /* Set xSPI CSn signal timings. */
    p_reg->LIOCFGCS[p_cfg_extend->channel] = liocfg;

    /* Set xSPI memory-mapping operation. */
    ret = r_ospi_protocol_specific_settings(p_instance_ctrl);

    /* Configure bus bridge for memory-mapped writes per hardware manual specifications */
    uint32_t mwrcomb_enable = (OSPI_CFG_COMBINATION_FUNCTION == OSPI_COMBINATION_FUNCTION_DISABLE) ? 0U : 1U;
    uint32_t mwrsize_value  = ((uint32_t) OSPI_CFG_COMBINATION_FUNCTION & 0xFFU) >> 1;

    /* Return response after issuing write transaction to xSPI bus, Enable prefetch function and combination if desired. */
    const uint32_t bmcfgch = (OSPI_PRV_BMCFGCH0_WRMD_VALUE << R_OSPI_BMCFGCH0_WRMD_Pos) |
                             ((mwrcomb_enable << R_OSPI_BMCFGCH0_MWRCOMB_Pos) & R_OSPI_BMCFGCH0_MWRCOMB_Msk) |
                             ((mwrsize_value << R_OSPI_BMCFGCH0_MWRSIZE_Pos) & R_OSPI_BMCFGCH0_MWRSIZE_Msk) |
                             (((uint32_t) OSPI_CFG_PREFETCH_FUNCTION << R_OSPI_BMCFGCH0_PREEN_Pos) &
                              R_OSPI_BMCFGCH0_PREEN_Msk);

    /* Both of these should have the same configuration and it affects all OSPI slave channels. */
    p_reg->BMCFGCH0 = bmcfgch;

    /* Re-activate memory-mapped mode in Read/Write. */
    if (OSPI_DEVICE_NUMBER_0 == p_instance_ctrl->channel)
    {
        p_reg->BMCTL0 |= R_OSPI_BMCTL0_CH0CS0ACC_Msk;
    }
    else
    {
        p_reg->BMCTL0 |= R_OSPI_BMCTL0_CH0CS1ACC_Msk;
    }

    if (FSP_SUCCESS == ret)
    {
        p_instance_ctrl->open       = OSPI_PRV_OPEN;
        g_ospi_channels_open_flags |= OSPI_PRV_CH_MASK(p_cfg_extend);
    }
    else if (0U == (g_ospi_channels_open_flags & OSPI_PRV_UNIT_MASK(p_cfg_extend)))
    {
        /* If the open fails and no other channels are open, stop the module. */

        p_instance_ctrl->open = 0U;
    }
    else
    {
        /* Do nothing */
    }

    return ret;
}

/*******************************************************************************************************************//**
 * Writes raw data directly to the OctaFlash. API not supported. Use R_OSPI_DirectTransfer
 *
 * Implements @ref spi_flash_api_t::directWrite.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by OSPI.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_DirectWrite (spi_flash_ctrl_t * const p_ctrl,
                              uint8_t const * const    p_src,
                              uint32_t const           bytes,
                              bool const               read_after_write)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_src);
    FSP_PARAMETER_NOT_USED(bytes);
    FSP_PARAMETER_NOT_USED(read_after_write);

    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/*******************************************************************************************************************//**
 * Reads raw data directly from the OctaFlash. API not supported. Use R_OSPI_DirectTransfer.
 *
 * Implements @ref spi_flash_api_t::directRead.
 *
 * @retval FSP_ERR_UNSUPPORTED         API not supported by OSPI.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_DirectRead (spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_dest);
    FSP_PARAMETER_NOT_USED(bytes);

    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/*******************************************************************************************************************//**
 * Read/Write raw data directly with the OctaFlash.
 *
 * Implements @ref spi_flash_api_t::directTransfer.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_DirectTransfer (spi_flash_ctrl_t * const            p_ctrl,
                                 spi_flash_direct_transfer_t * const p_transfer,
                                 spi_flash_direct_transfer_dir_t     direction)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_transfer);
    FSP_ASSERT(0U != p_transfer->command_length);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    r_ospi_direct_transfer(p_instance_ctrl, p_transfer, direction);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Enters XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipEnter.
 *
 * @retval FSP_SUCCESS                 XiP mode was entered successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_UNSUPPORTED         XiP support is not enabled.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_XipEnter (spi_flash_ctrl_t * const p_ctrl)
{
#if OSPI_CFG_XIP_SUPPORT_ENABLE
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

 #if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    r_ospi_xip(p_instance_ctrl, true);

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Exits XIP (execute in place) mode.
 *
 * Implements @ref spi_flash_api_t::xipExit.
 *
 * @retval FSP_SUCCESS                 XiP mode was entered successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_UNSUPPORTED         XiP support is not enabled.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_XipExit (spi_flash_ctrl_t * const p_ctrl)
{
#if OSPI_CFG_XIP_SUPPORT_ENABLE
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

 #if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    r_ospi_xip(p_instance_ctrl, false);

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * Program a page of data to the flash.
 *
 * Implements @ref spi_flash_api_t::write.
 *
 * @retval FSP_SUCCESS                 The flash was programmed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl, p_dest or p_src is NULL, or byte_count crosses a page boundary.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_INVALID_SIZE        Insufficient space remaining in page or write length is not a multiple of CPU access size when not using the DMAC.
 * @retval FSP_ERR_DEVICE_BUSY         Another Write/Erase transaction is in progress.
 * @retval FSP_ERR_WRITE_FAILED        Write operation failed.
 * @retval FSP_ERR_INVALID_ADDRESS     Destination or source is not aligned to CPU access alignment when not using the DMAC.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_Write (spi_flash_ctrl_t * const p_ctrl,
                        uint8_t const * const    p_src,
                        uint8_t * const          p_dest,
                        uint32_t                 byte_count)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_SUCCESS;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_src);
    FSP_ASSERT(NULL != p_dest);
    FSP_ASSERT(0U != byte_count);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    /* Check that space remaining in page is sufficient for requested write size */
    uint32_t page_size   = p_instance_ctrl->p_cfg->page_size_bytes;
    uint32_t page_offset = (uint32_t) p_dest & (page_size - 1U);
    FSP_ERROR_RETURN((page_size - page_offset) >= byte_count, FSP_ERR_INVALID_SIZE);

    FSP_ERROR_RETURN((byte_count & (OSPI_PRV_CPU_ACCESS_LENGTH - 1U)) == 0U, FSP_ERR_INVALID_SIZE);
    FSP_ERROR_RETURN(((uint32_t) p_dest & (OSPI_PRV_CPU_ACCESS_ALIGNMENT - 1U)) == 0U, FSP_ERR_INVALID_ADDRESS);
#endif

    R_OSPI_Type * const p_reg = p_instance_ctrl->p_reg;

    FSP_ERROR_RETURN(false == r_ospi_status_sub(p_instance_ctrl, p_instance_ctrl->p_cfg->write_status_bit),
                     FSP_ERR_DEVICE_BUSY);

    uint32_t       * p_dest32 = (uint32_t *) (void *) ((uint32_t) p_dest & ~(OSPI_PRV_CPU_ACCESS_ALIGNMENT - 1U));
    const uint32_t * p_src32  = (const uint32_t *) (const void *) p_src;

    err = r_ospi_write_enable(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    while (sizeof(uint32_t) <= byte_count)
    {
        /* When combination function is enabled, xSPI master transmits a xSPI
         * frame with the selected size while the sequential address is incremental. Please read
         * "Combination Function" in the OSPI Operation section of the relevant hardware manual.
         * So Basically Enable command should be
         * sent only once for a single burst(incremented addresses up to set combination size.). */
        *p_dest32 = *p_src32;
        p_dest32++;
        p_src32++;
        byte_count -= sizeof(uint32_t);
    }

    p_reg->BMCTL1 = OSPI_PRV_BMCTL1_PUSH_COMBINATION_WRITE_MASK;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Erase a block or sector of flash.  The byte_count must exactly match one of the erase sizes defined in spi_flash_cfg_t.
 * For chip erase, byte_count must be SPI_FLASH_ERASE_SIZE_CHIP_ERASE.
 *
 * Implements @ref spi_flash_api_t::erase.
 *
 * @retval FSP_SUCCESS                 The command to erase the flash was executed successfully.
 * @retval FSP_ERR_ASSERTION           p_instance_ctrl or p_device_address is NULL, byte_count doesn't match an erase
 *                                     size defined in spi_flash_cfg_t, or byte_count is set to 0.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_DEVICE_BUSY         The device is busy.
 * @retval FSP_ERR_WRITE_FAILED        Write operation failed.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_Erase (spi_flash_ctrl_t * const p_ctrl, uint8_t * const p_device_address, uint32_t byte_count)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_device_address);
    FSP_ASSERT(0U != byte_count);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    spi_flash_cfg_t const * p_cfg         = p_instance_ctrl->p_cfg;
    uint16_t                erase_command = 0;
    bool send_address = true;

    ospi_xspi_command_set_t const * p_cmd_set = p_instance_ctrl->p_cmd_set;

    /* Use the address bytes and MSB mask to convert from system address to chip addresses.
     * This is better than using pointer subtraction because some devices (i.e., SiP) have different start addresses.
     * This method better mimics how memory-mapped addresses are converted in the hardware. */
    uint32_t addr_mask = (~(uint32_t) UINT8_MAX | (uint32_t) p_cmd_set->address_msb_mask);            // Start with a full 32-bit mask with the address MSB mask in the lower byte.
    addr_mask <<= (8U * (((uint32_t) OSPI_PRV_ADDR_BYTES_TO_LENGTH(p_cmd_set->address_bytes)) - 1U)); // Shift the MSB to the correct position.
    uint32_t chip_address = (uint32_t) ((uint32_t) p_device_address & ~addr_mask);                    // Mask off the address bits that aren't needed.

    FSP_ERROR_RETURN(false == r_ospi_status_sub(p_instance_ctrl, p_cfg->write_status_bit), FSP_ERR_DEVICE_BUSY);

    /* Select the appropriate erase command from the command set. */
    spi_flash_erase_command_t const * p_erase_list = p_cmd_set->p_erase_commands->p_table;
    const uint8_t erase_list_length                = p_cmd_set->p_erase_commands->length;

    for (uint32_t index = 0; index < erase_list_length; index++)
    {
        /* If requested byte_count is supported by underlying flash, store the command. */
        if (byte_count == p_erase_list[index].size)
        {
            if ((uint32_t) SPI_FLASH_ERASE_SIZE_CHIP_ERASE == byte_count)
            {
                /* Don't send address for chip erase. */
                send_address = false;
            }

            erase_command = p_erase_list[index].command;
            break;
        }
    }

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(0U != erase_command);
#endif

    fsp_err_t err = r_ospi_write_enable(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    spi_flash_direct_transfer_t direct_command =
    {
        .command        = erase_command,
        .command_length = (uint8_t) p_cmd_set->command_bytes,
        .address        = chip_address,
        .address_length = ((send_address) ? OSPI_PRV_ADDR_BYTES_TO_LENGTH(p_cmd_set->address_bytes) : 0U),
        .data_length    = 0,
    };

    r_ospi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);

    /* If prefetch is enabled, make sure the banks aren't being used and flush the prefetch caches after an erase. */
#if OSPI_CFG_PREFETCH_FUNCTION
    R_OSPI_Type * const p_reg = p_instance_ctrl->p_reg;
    FSP_HARDWARE_REGISTER_WAIT((p_reg->COMSTT & OSPI_PRV_COMSTT_MEMACCCH_MASK), 0U);
    p_reg->BMCTL1 = OSPI_PRV_BMCTL1_CLEAR_PREFETCH_MASK;
#endif

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
fsp_err_t R_OSPI_StatusGet (spi_flash_ctrl_t * const p_ctrl, spi_flash_status_t * const p_status)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_status);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Read device status. */
    p_status->write_in_progress = r_ospi_status_sub(p_instance_ctrl, p_instance_ctrl->p_cfg->write_status_bit);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Selects the bank to access. Use ospi_b_bank_select_t as the bank value.
 *
 * Implements @ref spi_flash_api_t::bankSet.
 *
 * @retval FSP_ERR_UNSUPPORTED           This function is unsupported.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_BankSet (spi_flash_ctrl_t * const p_ctrl, uint32_t bank)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(bank);

    FSP_RETURN(FSP_ERR_UNSUPPORTED);
}

/*******************************************************************************************************************//**
 * Sets the SPI protocol.
 *
 * Implements @ref spi_flash_api_t::spiProtocolSet.
 *
 * @retval FSP_SUCCESS                SPI protocol updated on MPU peripheral.
 * @retval FSP_ERR_ASSERTION          A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN           Driver is not opened.
 * @retval FSP_ERR_CALIBRATE_FAILED   Failed to perform auto-calibrate.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_SpiProtocolSet (spi_flash_ctrl_t * const p_ctrl, spi_flash_protocol_t spi_protocol)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Save the old protocol in case of an undefined command set. */
    spi_flash_protocol_t old_protocol = p_instance_ctrl->spi_protocol;
    p_instance_ctrl->spi_protocol = spi_protocol;

    /* Update the SPI protocol and its associated registers. */
    fsp_err_t err = r_ospi_protocol_specific_settings(p_instance_ctrl);

    if (FSP_ERR_INVALID_MODE == err)
    {
        /* Restore the original spi protocol. Nothing else has been changed in this case. */
        p_instance_ctrl->spi_protocol = old_protocol;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Close the OSPI driver module.
 *
 * Implements @ref spi_flash_api_t::close.
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_instance_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        Driver is not opened.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_Close (spi_flash_ctrl_t * const p_ctrl)
{
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;
    fsp_err_t              err             = FSP_SUCCESS;

#if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg);
    FSP_ASSERT(NULL != p_instance_ctrl->p_cfg->p_extend);
#endif

    const ospi_extended_cfg_t * const p_cfg_extend = OSPI_PRV_EXTENDED_CFG(p_instance_ctrl);

    p_instance_ctrl->open       = 0U;
    g_ospi_channels_open_flags &= ~OSPI_PRV_CH_MASK(p_cfg_extend);

    return err;
}

/**************************************************s*****************************************************************//**
 * AutoCalibrate the OSPI DS signal.
 *
 * Implements @ref spi_flash_api_t::autoCalibrate.
 *
 * @retval FSP_SUCCESS                 Autocalibration completed successfully.
 * @retval FSP_ERR_ASSERTION           A required pointer is NULL.
 * @retval FSP_ERR_NOT_OPEN            Driver is not opened.
 * @retval FSP_ERR_UNSUPPORTED         Autocalibration support is not enabled.
 * @retval FSP_ERR_CALIBRATE_FAILED    Failed to perform auto-calibrate.
 **********************************************************************************************************************/
fsp_err_t R_OSPI_AutoCalibrate (spi_flash_ctrl_t * const p_ctrl)
{
#if OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE
    ospi_instance_ctrl_t * p_instance_ctrl = (ospi_instance_ctrl_t *) p_ctrl;

 #if OSPI_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(OSPI_PRV_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
 #endif

    /* Perform Automatic Calibration to appropriately update WRAPCFG DSSFTCSn field. */
    if (OSPI_PRV_PROTOCOL_USES_DS_SIGNAL(p_instance_ctrl->spi_protocol))
    {
        return r_ospi_automatic_calibration_seq(p_instance_ctrl);
    }

    return FSP_SUCCESS;
#else
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
#endif
}

/*******************************************************************************************************************//**
 * @} (end addtogroup OSPI)
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Perform initialization based on SPI/OPI protocol
 *
 * @param[in]   p_instance_ctrl    Pointer to OSPI specific control structure
 *
 * @retval      FSP_SUCCESS                Protocol based settings completed successfully.
 * @retval      FSP_ERR_CALIBRATE_FAILED   Auto-Calibration failed.
 **********************************************************************************************************************/
static fsp_err_t r_ospi_protocol_specific_settings (ospi_instance_ctrl_t * p_instance_ctrl)
{
    R_OSPI_Type * const p_reg = p_instance_ctrl->p_reg;
    fsp_err_t           ret   = FSP_SUCCESS;

    /* Get the command set for the configured protocol and save it to the control struct. */
    ospi_xspi_command_set_t const * p_cmd_set = r_ospi_command_set_get(p_instance_ctrl);
    FSP_ERROR_RETURN(NULL != p_cmd_set, FSP_ERR_INVALID_MODE);

    p_instance_ctrl->p_cmd_set = p_cmd_set;

    /* Update the SPI protocol and latency mode. */
    uint32_t liocfg = p_reg->LIOCFGCS[p_instance_ctrl->channel] &
                      ~(R_OSPI_LIOCFGCS_LATEMD_Msk | R_OSPI_LIOCFGCS_PRTMD_Msk);
    liocfg |= (((uint32_t) p_instance_ctrl->spi_protocol << R_OSPI_LIOCFGCS_PRTMD_Pos) & R_OSPI_LIOCFGCS_PRTMD_Msk);
    liocfg |= (((uint32_t) p_cmd_set->latency_mode << R_OSPI_LIOCFGCS_LATEMD_Pos) & R_OSPI_LIOCFGCS_LATEMD_Msk);
    p_reg->LIOCFGCS[p_instance_ctrl->channel] = liocfg;

    /* Specifies the read/write commands and Read dummy clocks for Device
     * (see "Flow of Memory-mapping" in the OSPI section of the relevant hardware manual). */
    uint32_t cmcfg0 = (((uint32_t) p_cmd_set->address_msb_mask << R_OSPI_CMCFGCS_CMCFG0_ADDRPEN_Pos)) |
                      (((uint32_t) p_cmd_set->frame_format << R_OSPI_CMCFGCS_CMCFG0_FFMT_Pos)) |
                      (((uint32_t) p_cmd_set->address_bytes << R_OSPI_CMCFGCS_CMCFG0_ADDSIZE_Pos) &
                       R_OSPI_CMCFGCS_CMCFG0_ADDSIZE_Msk);

    /* When using 4-byte addressing, always mask off the most-significant nybble to remove the system bus offset from
     * the transmitted addresses. Ex. CS0 starts at 0x2000_0000 so it needs to mask off bits [31:28]. */
    if (p_cmd_set->address_bytes == SPI_FLASH_ADDRESS_BYTES_4)
    {
        cmcfg0 |= OSPI_PRV_ADDRESS_REPLACE_ENABLE_BITS;
    }

    /* Apply the frame format setting and update the register. */
    cmcfg0 |= ((uint32_t) p_cmd_set->frame_format << R_OSPI_CMCFGCS_CMCFG0_FFMT_Pos);
    p_reg->CMCFGCS[p_instance_ctrl->channel].CMCFG0 = cmcfg0;

    /* Cache the appropriate command values for later use. */
    uint16_t read_command  = p_cmd_set->read_command;
    uint16_t write_command = p_cmd_set->program_command;

    /* If no length is specified or if the command byte length is 1, move the command to the upper byte. */
    if (OSPI_COMMAND_BYTES_1 == p_cmd_set->command_bytes)
    {
        read_command =
            (uint16_t) ((read_command & OSPI_PRV_CDTBUF_CMD_1B_VALUE_MASK) << OSPI_PRV_CDTBUF_CMD_1B_VALUE_SHIFT);
        write_command =
            (uint16_t) ((write_command & OSPI_PRV_CDTBUF_CMD_1B_VALUE_MASK) << OSPI_PRV_CDTBUF_CMD_1B_VALUE_SHIFT);
    }

    const uint8_t read_dummy_cycles  = p_cmd_set->read_dummy_cycles;
    const uint8_t write_dummy_cycles = p_cmd_set->program_dummy_cycles;

    p_reg->CMCFGCS[p_instance_ctrl->channel].CMCFG1 =
        (uint32_t) (((uint32_t) (read_command) << R_OSPI_CMCFGCS_CMCFG1_RDCMD_Pos) |
                    (((uint32_t) read_dummy_cycles << R_OSPI_CMCFGCS_CMCFG1_RDLATE_Pos) &
                     R_OSPI_CMCFGCS_CMCFG1_RDLATE_Msk));

    p_reg->CMCFGCS[p_instance_ctrl->channel].CMCFG2 =
        (uint32_t) (((uint32_t) (write_command) << R_OSPI_CMCFGCS_CMCFG2_WRCMD_Pos) |
                    (((uint32_t) write_dummy_cycles << R_OSPI_CMCFGCS_CMCFG2_WRLATE_Pos) &
                     R_OSPI_CMCFGCS_CMCFG2_WRLATE_Msk));

#if OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE
    const ospi_extended_cfg_t * const p_cfg_extend = OSPI_PRV_EXTENDED_CFG(p_instance_ctrl);

    /* Perform Automatic Calibration to appropriately update WRAPCFG DSSFTCSn field. */
    if (((uint8_t) 0U == p_cfg_extend->data_latch_delay_clocks) &&
        OSPI_PRV_PROTOCOL_USES_DS_SIGNAL(p_instance_ctrl->spi_protocol))
    {
        ret = r_ospi_automatic_calibration_seq(p_instance_ctrl);
    }
#endif

    return ret;
}

/*******************************************************************************************************************//**
 * Gets device status.
 *
 * @param[in]  p_instance_ctrl         Pointer to a driver handle
 * @param[in]  bit_pos                 Write-in-progress bit position
 *
 * @return True if busy, false if not.
 **********************************************************************************************************************/
static bool r_ospi_status_sub (ospi_instance_ctrl_t * p_instance_ctrl, uint8_t bit_pos)
{
    ospi_xspi_command_set_t const * p_cmd_set = p_instance_ctrl->p_cmd_set;

    /* Skip status check if no command was specified. */
    if (0U == p_cmd_set->status_command)
    {
        return false;
    }

    spi_flash_direct_transfer_t direct_command =
    {
        .command        = p_cmd_set->status_command,
        .command_length = (uint8_t) p_cmd_set->command_bytes,
        .address_length = (uint8_t) (p_cmd_set->status_needs_address ?
                                     OSPI_PRV_ADDR_BYTES_TO_LENGTH(p_cmd_set->status_address_bytes) : 0U),
        .address      = ((p_cmd_set->status_needs_address) ? p_cmd_set->status_address : 0U),
        .data_length  = 1U,
        .dummy_cycles = p_cmd_set->status_dummy_cycles,
    };

    /* 8D-8D-8D mode requires an address for any kind of read. If the address wasn't set by the configuration
     * set it to the general address length. */
    if ((direct_command.address_length != 0U) && (SPI_FLASH_PROTOCOL_8D_8D_8D == p_instance_ctrl->spi_protocol))
    {
        direct_command.address_length = (OSPI_PRV_ADDR_BYTES_TO_LENGTH(p_cmd_set->address_bytes));
    }

    r_ospi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_READ);

    return (bool) ((direct_command.data_transfer.data >> bit_pos) & 1U);
}

/*******************************************************************************************************************//**
 * Send Write enable command to the OctaFlash
 *
 * @param[in]   p_instance_ctrl    Pointer to OSPI specific control structure
 *
 * @retval      FSP_SUCCESS         Write enable operation completed.
 * @retval      FSP_ERR_NOT_ENABLED Write enable failed.
 **********************************************************************************************************************/
static fsp_err_t r_ospi_write_enable (ospi_instance_ctrl_t * p_instance_ctrl)
{
    ospi_xspi_command_set_t const * const p_cmd_set = p_instance_ctrl->p_cmd_set;

    /* If the command is 0x00, then skip sending the write enable. */
    if (0U == p_cmd_set->write_enable_command)
    {
        return FSP_SUCCESS;
    }

    spi_flash_direct_transfer_t direct_command =
    {
        .command        = p_cmd_set->write_enable_command,
        .command_length = (uint8_t) p_cmd_set->command_bytes,
        .address_length = 0,
        .address        = 0,
        .data_length    = 0,
        .dummy_cycles   = 0,
    };

    r_ospi_direct_transfer(p_instance_ctrl, &direct_command, SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE);

    /* In case write enable is not checked, assume write is enabled. */
    bool write_enabled = true;

#if OSPI_MAX_WRITE_ENABLE_LOOPS > 0U

    /* Verify write is enabled. */
    for (uint32_t i = 0U; i < OSPI_MAX_WRITE_ENABLE_LOOPS; i++)
    {
        write_enabled = r_ospi_status_sub(p_instance_ctrl, p_instance_ctrl->p_cfg->write_enable_bit);
        if (write_enabled)
        {
            break;
        }
    }
#endif

    FSP_ERROR_RETURN(write_enabled, FSP_ERR_NOT_ENABLED);

    return FSP_SUCCESS;
}

#if OSPI_CFG_AUTOCALIBRATION_SUPPORT_ENABLE

/*******************************************************************************************************************//**
 * Perform Automatic Calibration
 *
 * @param[in]   p_instance_ctrl    Pointer to OSPI specific control structure
 *
 * @retval      FSP_SUCCESS                Auto-Calibration completed successfully.
 * @retval      FSP_ERR_CALIBRATE_FAILED   Auto-Calibration failed.
 * @retval      FSP_ERR_DEVICE_BUSY        Auto-Calibration is already running.
 **********************************************************************************************************************/
static fsp_err_t r_ospi_automatic_calibration_seq (ospi_instance_ctrl_t const * p_instance_ctrl)
{
    R_OSPI_Type * const               p_reg        = p_instance_ctrl->p_reg;
    fsp_err_t                         ret          = FSP_SUCCESS;
    const ospi_extended_cfg_t * const p_cfg_extend = OSPI_PRV_EXTENDED_CFG(p_instance_ctrl);

    ospi_xspi_command_set_t const * p_cmd_set = p_instance_ctrl->p_cmd_set;

    ospi_device_number_t channel = p_instance_ctrl->channel;

    /* Check that calibration is not in progress. */
    if (0U != p_reg->CCCTLCS[channel].CCCTL0_b.CAEN)
    {
        return FSP_ERR_DEVICE_BUSY;
    }

    const uint8_t command_bytes     = (uint8_t) p_cmd_set->command_bytes;
    uint16_t      read_command      = p_cmd_set->read_command;
    const uint8_t read_dummy_cycles = p_cmd_set->read_dummy_cycles;
    const uint8_t address_bytes     = (OSPI_PRV_ADDR_BYTES_TO_LENGTH(p_cmd_set->address_bytes));

    /* If using 1 command byte, shift the read command over as the peripheral expects. */
    if (1U == command_bytes)
    {
        read_command =
            (uint16_t) ((read_command & OSPI_PRV_CDTBUF_CMD_1B_VALUE_MASK) <<
                        OSPI_PRV_CDTBUF_CMD_1B_VALUE_SHIFT);
    }

    p_reg->CCCTLCS[channel].CCCTL1 =
        (((uint32_t) command_bytes << R_OSPI_CCCTLCS_CCCTL1_CACMDSIZE_Pos) &
         R_OSPI_CCCTLCS_CCCTL1_CACMDSIZE_Msk) |
        (((uint32_t) address_bytes << R_OSPI_CCCTLCS_CCCTL1_CAADDSIZE_Pos) &
         R_OSPI_CCCTLCS_CCCTL1_CAADDSIZE_Msk) |
        ((uint32_t) OSPI_PRV_AUTOCALIBRATION_DATA_SIZE << R_OSPI_CCCTLCS_CCCTL1_CADATASIZE_Pos) |
        ((uint32_t) OSPI_PRV_AUTOCALIBRATION_LATENCY_CYCLES << R_OSPI_CCCTLCS_CCCTL1_CAWRLATE_Pos) |
        (((uint32_t) read_dummy_cycles << R_OSPI_CCCTLCS_CCCTL1_CARDLATE_Pos) &
         R_OSPI_CCCTLCS_CCCTL1_CARDLATE_Msk);

    p_reg->CCCTLCS[channel].CCCTL2 =
        (((uint32_t) read_command << R_OSPI_CCCTLCS_CCCTL2_CARDCMD_Pos) &
         R_OSPI_CCCTLCS_CCCTL2_CARDCMD_Msk);

    p_reg->CCCTLCS[channel].CCCTL3 = (uint32_t) p_cfg_extend->p_autocalibration_preamble_pattern_addr &
                                     OSPI_PRV_ADDRESS_REPLACE_MASK;
    p_reg->CCCTLCS[channel].CCCTL4 = OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_0;
    p_reg->CCCTLCS[channel].CCCTL5 = OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_1;
    p_reg->CCCTLCS[channel].CCCTL6 = OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_2;
    p_reg->CCCTLCS[channel].CCCTL7 = OSPI_PRV_AUTOCALIBRATION_PREAMBLE_PATTERN_3;

    p_reg->CCCTLCS[channel].CCCTL0 =
        ((uint32_t) OSPI_PRV_AUTOCALIBRATION_NO_WRITE_CMD << R_OSPI_CCCTLCS_CCCTL0_CANOWR_Pos) |
        ((uint32_t) OSPI_PRV_AUTOCALIBRATION_FRAME_INTERVAL <<
            R_OSPI_CCCTLCS_CCCTL0_CAITV_Pos) |
        ((uint32_t) OSPI_PRV_AUTOCALIBRATION_SHIFT_DS_END_VALUE <<
            R_OSPI_CCCTLCS_CCCTL0_CASFTEND_Pos);

    /* Automatic Calibration Enable */
    p_reg->CCCTLCS[channel].CCCTL0_b.CAEN = 1U;

    /* Check calibration success or failure. */
    while ((0U == ((p_reg->INTS >> ((uint32_t) R_OSPI_INTS_CASUCCS0_Pos + (uint32_t) channel)) & 0x01U)) &&
           (0U == ((p_reg->INTS >> ((uint32_t) R_OSPI_INTS_CAFAILCS0_Pos + (uint32_t) channel)) & 0x01U)))
    {
        /* Do nothing. */
    }

    /* Disable automatic calibration */
    p_reg->CCCTLCS[channel].CCCTL0_b.CAEN = 0U;

    if (1U == ((p_reg->INTS >> ((uint32_t) R_OSPI_INTS_CASUCCS0_Pos + (uint32_t) channel)) & 0x01U))
    {
        /* Clear automatic calibration success status */
        p_reg->INTC = (uint32_t) 1U << ((uint32_t) R_OSPI_INTS_CASUCCS0_Pos + (uint32_t) channel);
    }
    else if (1U == ((p_reg->INTS >> ((uint32_t) R_OSPI_INTS_CAFAILCS0_Pos + (uint32_t) channel)) & 0x01U))
    {
        ret = FSP_ERR_CALIBRATE_FAILED;

        /* Clear automatic calibration failure status */
        p_reg->INTC = (uint32_t) 1U << ((uint32_t) R_OSPI_INTS_CAFAILCS0_Pos + (uint32_t) channel);
    }
    else
    {
        /* Do nothing. */
    }

    return ret;
}

#endif

/*******************************************************************************************************************//**
 * Performs direct data transfer with the OctaFlash
 *
 * @param[in]   p_instance_ctrl    Pointer to the instance ctrl struct.
 * @param[in]   p_transfer         Pointer to transfer parameters
 * @param[in]   direction          Read/Write
 **********************************************************************************************************************/
static void r_ospi_direct_transfer (ospi_instance_ctrl_t const        * p_instance_ctrl,
                                    spi_flash_direct_transfer_t * const p_transfer,
                                    spi_flash_direct_transfer_dir_t     direction)
{
    R_OSPI_Type * const        p_reg   = p_instance_ctrl->p_reg;
    const ospi_device_number_t channel = p_instance_ctrl->channel;

    uint32_t cdtbuf0 =
        (((uint32_t) p_transfer->command_length << R_OSPI_CDBUF_CDT_CMDSIZE_Pos) & R_OSPI_CDBUF_CDT_CMDSIZE_Msk) |
        (((uint32_t) p_transfer->address_length << R_OSPI_CDBUF_CDT_ADDSIZE_Pos) & R_OSPI_CDBUF_CDT_ADDSIZE_Msk) |
        (((uint32_t) p_transfer->data_length << R_OSPI_CDBUF_CDT_DATASIZE_Pos) & R_OSPI_CDBUF_CDT_DATASIZE_Msk) |
        (((uint32_t) p_transfer->dummy_cycles << R_OSPI_CDBUF_CDT_LATE_Pos) & R_OSPI_CDBUF_CDT_LATE_Msk) |
        (((uint32_t) direction << R_OSPI_CDBUF_CDT_TRTYPE_Pos) & R_OSPI_CDBUF_CDT_TRTYPE_Msk);

    cdtbuf0 |= (1U == p_transfer->command_length) ?
               (((uint32_t) p_transfer->command & (uint32_t) OSPI_PRV_CDTBUF_CMD_1B_VALUE_MASK) <<
                OSPI_PRV_CDTBUF_CMD_UPPER_OFFSET) :
               (((uint32_t) p_transfer->command & (uint32_t) OSPI_PRV_CDTBUF_CMD_2B_VALUE_MASK) <<
                OSPI_PRV_CDTBUF_CMD_OFFSET);

    /* Setup the manual command control. Cancel any ongoing transactions, direct mode, set channel, 1 transaction. */
    p_reg->CDCTL0 = ((((uint32_t) channel) << R_OSPI_CDCTL0_CSSEL_Pos) & R_OSPI_CDCTL0_CSSEL_Msk);

    /* Direct Read/Write settings
     * (see "Flow of Manual-command Procedure" in the OSPI section of the relevant hardware manual). */
    FSP_HARDWARE_REGISTER_WAIT(p_reg->CDCTL0_b.TRREQ, 0U);

    p_reg->CDBUF[0].CDT = cdtbuf0;
    p_reg->CDBUF[0].CDA = p_transfer->address;

    if (SPI_FLASH_DIRECT_TRANSFER_DIR_WRITE == direction)
    {
        p_reg->CDBUF[0].CDD0 = (uint32_t) (p_transfer->data_transfer.data_u64 & (uint64_t) UINT32_MAX);
        if ((uint32_t) p_transfer->data_length > (uint32_t) sizeof(uint32_t))
        {
            p_reg->CDBUF[0].CDD1 = (uint32_t) (p_transfer->data_transfer.data_u64 >> OSPI_PRV_UINT32_BITS);
        }
    }

    /* Start the transaction and wait for completion. */
    p_reg->CDCTL0_b.TRREQ = 1U;
    FSP_HARDWARE_REGISTER_WAIT(p_reg->CDCTL0_b.TRREQ, 0U);

    if (SPI_FLASH_DIRECT_TRANSFER_DIR_READ == direction)
    {
        p_transfer->data_transfer.data_u64 = p_reg->CDBUF[0].CDD0;
        if ((uint32_t) p_transfer->data_length > (uint32_t) sizeof(uint32_t))
        {
            p_transfer->data_transfer.data_u64 |= (((uint64_t) p_reg->CDBUF[0].CDD1) << OSPI_PRV_UINT32_BITS);
        }
    }

    /* Clear interrupt flags. */
    p_reg->INTC = p_reg->INTS;
}

#if OSPI_CFG_XIP_SUPPORT_ENABLE

/*******************************************************************************************************************//**
 * Performs a dummy read from the provided address.
 *
 * Some scenarios, mainly XiP state changes, require a memory-mapped read operation to emit the proper bus symbol. This
 * function will perform a read at the provided address, throw out the result, then toggle b3 of the address. The
 * toggling of b3 ensures the next dummy read will occur at an address that is not currently available in the OSPI
 * peripheral's 1-line (8-byte) data read buffer (this is not the same as the prefetch buffer). If the same address is
 * read twice in a row, the peripheral may return the buffered value instead of issuing a new bus transaction.
 *
 * @param[in,out] p_read_address Address as a uint32_t to read dummy data from
 **********************************************************************************************************************/
static void r_ospi_dummy_read (uint32_t * p_read_address)
{
    volatile uint64_t dummy_value = 0;

    if (0U != *p_read_address)
    {
        dummy_value     = *((volatile uint64_t *) *p_read_address);
        *p_read_address = ((*p_read_address) ^ 0x08U);
    }

    FSP_PARAMETER_NOT_USED(dummy_value);
}

/*******************************************************************************************************************//**
 * Configures the device to enter or exit XiP mode.
 *
 * @param[in]   p_instance_ctrl    Pointer to the instance ctrl struct.
 * @param[in]   is_entering        true if entering XiP mode, false if exiting.
 **********************************************************************************************************************/
static void r_ospi_xip (ospi_instance_ctrl_t * p_instance_ctrl, bool is_entering)
{
    R_OSPI_Type * const     p_reg              = p_instance_ctrl->p_reg;
    const spi_flash_cfg_t * p_cfg              = p_instance_ctrl->p_cfg;
    uint32_t                dummy_read_address = 0;

    /* Get the initial address to perform dummy reads.
     * This is just the first word of the appropriate memory-mapped CS region.*/
    if (0U == p_instance_ctrl->ospi_unit)
    {
        dummy_read_address = ((OSPI_DEVICE_NUMBER_0 == p_instance_ctrl->channel) ?
                              OSPI_DEVICE_0_START_ADDRESS :
                              OSPI_DEVICE_1_START_ADDRESS);
    }
    else if (1U == p_instance_ctrl->ospi_unit)
    {
        dummy_read_address = ((OSPI_DEVICE_NUMBER_0 == p_instance_ctrl->channel) ?
                              OSPI_DEVICE_0_START_ADDRESS :
                              OSPI_DEVICE_1_START_ADDRESS);
    }
    else
    {
        /* Not a valid unit. */
    }

    const uint32_t reg_cmctlch = ((uint32_t) (p_cfg->xip_enter_command << R_OSPI_CMCTLCH0_XIPENCODE_Pos)) |
                                 ((uint32_t) (p_cfg->xip_exit_command << R_OSPI_CMCTLCH0_XIPEXCODE_Pos));

    /* Save the bus bridge config to restore it after the XiP mode change. */
    const uint32_t reg_bmcfgch0 = p_reg->BMCFGCH0;

 #if OSPI_CFG_PREFETCH_FUNCTION

    /* Clear the pre-fetch buffer so following reads always send a read transaction to the xSPI memory. */
    p_reg->BMCTL1 = OSPI_PRV_BMCTL1_CLEAR_PREFETCH_MASK;

    /* Disable the prefetch buffer. */
    p_reg->BMCFGCH0 = reg_bmcfgch0 & ~R_OSPI_BMCFGCH0_PREEN_Msk;
 #endif

    if (OSPI_COMBINATION_FUNCTION_DISABLE != OSPI_CFG_COMBINATION_FUNCTION)
    {
        /* Flush any combination writes before disabling. */
        p_reg->BMCTL1 = OSPI_PRV_BMCTL1_PUSH_COMBINATION_WRITE_MASK;
    }

    /* Perform a dummy read before disabling memory mapping. */
    r_ospi_dummy_read(&dummy_read_address);

    /* Disable bus bridge buffering. */
    p_reg->BMCFGCH0 = 0U;

    /* Wait for any on-going access to complete. */
    FSP_HARDWARE_REGISTER_WAIT((p_reg->COMSTT & OSPI_PRV_COMSTT_PENDING_ACTION_MASK), 0);

    if (is_entering)
    {
        /* Change memory-mapping to read-only mode. */
        p_reg->BMCTL0 = OSPI_PRV_BMCTL0_READ_ONLY_VALUE;

        /* XiP enter/exit codes are configured only for memory mapped operations and affects both OSPI slave channels. */
        p_reg->CMCTLCH0 = reg_cmctlch | R_OSPI_CMCTLCH0_XIPEN_Msk;

        /* Perform a read to send the enter code. All further reads will use the enter code and will not send a read command code. */
        r_ospi_dummy_read(&dummy_read_address);
    }
    else
    {
        /* Disable XiP. The exit code needs to available at this stage. */
        p_reg->CMCTLCH0 = reg_cmctlch;

        /* Perform a read to send the exit code. */
        r_ospi_dummy_read(&dummy_read_address);

        /* Clear all XiP settings. */
        p_reg->CMCTLCH0 = 0U;

        /* This read should be without XiP. */
        r_ospi_dummy_read(&dummy_read_address);

        /* Change memory-mapping back to R/W mode. */
        p_reg->BMCTL0 = OSPI_PRV_BMCTL0_READ_WRITE_VALUE;
    }

    /* Restore bus bridge buffering to the previous state. */
    p_reg->BMCFGCH0 = reg_bmcfgch0;
}

#endif

/*******************************************************************************************************************//**
 * Gets the xspi command set for a protocol mode if it exists.
 *
 * @param[in]   p_instance_ctrl    Pointer to the instance ctrl struct.
 * @return Pointer to the command set, or NULL if it is normal SPI or has not been configured.
 **********************************************************************************************************************/
static ospi_xspi_command_set_t const * r_ospi_command_set_get (ospi_instance_ctrl_t const * p_instance_ctrl)
{
    const ospi_extended_cfg_t * const p_cfg_extend = OSPI_PRV_EXTENDED_CFG(p_instance_ctrl);

    ospi_xspi_command_set_t const * p_cmd_set;
    for (uint32_t i = 0; i < p_cfg_extend->p_xspi_command_set->length; i++)
    {
        p_cmd_set = &((ospi_xspi_command_set_t const *) p_cfg_extend->p_xspi_command_set->p_table)[i];
        if (p_cmd_set->protocol == p_instance_ctrl->spi_protocol)
        {
            return p_cmd_set;
        }
    }

    /* If the protocol isn't found, return NULL. */
    return NULL;
}

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_crc.h"                     /* PRQA S 0380 # DP_MSR_D-1.1-C99_UC1 & DP_MSR_D-1.1-C11_UC1 */
#include "r_crc_cfg.h"
#include <string.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "CRC" in ASCII, used to determine if channel is open. */
#define CRC_OPEN                 (0x00435243U)
#ifndef BSP_FEATURE_CRC_COMMON_GROUP
 #define CRC_PAIR_STRIDE_EVEN    (R_KCRC2_BASE - R_KCRC0_BASE)
 #define CRC_PAIR_STRIDE_ODD     (R_KCRC3_BASE - R_KCRC1_BASE)
 #define CRC_GROUP_GAP           (R_KCRC0_BASE - R_KCRC1_BASE)
#else
 #define CRC_GROUP_GAP           (R_KCRC1_BASE - R_KCRC0_BASE)
 #define CRC_PAIR_STRIDE         (R_KCRC2_BASE - R_KCRC0_BASE)
#endif

#define CRC_INPUT_ALIGN_16BIT    (0x1U)
#define CRC_INPUT_ALIGN_32BIT    (0x3U)
#define CRC_DOUT1_SHIFT_BITS     (32U)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void crc_calculate_polynomial(crc_instance_ctrl_t * const p_instance_ctrl,
                                     crc_input_t * const         p_crc_input,
                                     crc_result_t              * p_calculatedValue);
static crc_result_t crc_calculated_value_get(crc_instance_ctrl_t * const p_instance_ctrl);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/* Filled in Interface API structure for this Instance. */
const crc_api_t g_crc_on_crc =                /* PRQA S 1502 # Advisory R-2.8 */
{
    .open         = R_CRC_Open,               /* PRQA S 3635 # Advisory R-17.12 */
    .close        = R_CRC_Close,              /* PRQA S 3635 # Advisory R-17.12 */
    .calculate    = R_CRC_Calculate,          /* PRQA S 3635 # Advisory R-17.12 */
    .crcResultGet = R_CRC_CalculatedValueGet, /* PRQA S 3635 # Advisory R-17.12 */
    .snoopEnable  = R_CRC_SnoopEnable,        /* PRQA S 3635 # Advisory R-17.12 */
    .snoopDisable = R_CRC_SnoopDisable,       /* PRQA S 3635 # Advisory R-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup CRC
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Open the CRC driver module.
 *
 * Implements @ref crc_api_t::open
 *
 * Open the CRC driver module and initialize the driver control block according to the passed-in
 * configuration structure.
 *
 * @retval FSP_SUCCESS                       Configuration was successful.
 * @retval FSP_ERR_ASSERTION                 p_ctrl, p_cfg, or p_cfg->p_extend is NULL.
 * @retval FSP_ERR_ALREADY_OPEN              Module already open.
 **********************************************************************************************************************/
fsp_err_t R_CRC_Open (crc_ctrl_t * const p_ctrl, crc_cfg_t const * const p_cfg) /* PRQA S 1505 # Advisory R-8.7 */
{
    crc_instance_ctrl_t * p_instance_ctrl = (crc_instance_ctrl_t *) p_ctrl;     /* PRQA S 0316 # Advisory R-11.5 */
    uint32_t              crc_ctl         = 0;
#if CRC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);

    /* Verify the control block has not already been initialized. */
    FSP_ERROR_RETURN(CRC_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#endif

    /* Pointer to CRC extended configuration. */
    crc_extended_cfg_t const * p_extend = (crc_extended_cfg_t const *) p_cfg->p_extend;                              /* PRQA S 0316 # Advisory R-11.5 */

    /* Map register base for the selected unit. */
#ifdef BSP_FEATURE_CRC_COMMON_GROUP
    p_instance_ctrl->p_reg = (R_KCRC0_Type *) (R_KCRC0_BASE + (CRC_GROUP_GAP * (((uint32_t) p_cfg->channel) % 2U)) + /* PRQA S 0306 # Advisory R-11.4 */
                                               (CRC_PAIR_STRIDE * (((uint32_t) p_cfg->channel) / 2U)));
#else
    if (0U == (p_cfg->channel % 2U))
    {
        p_instance_ctrl->p_reg = (R_KCRC0_Type *) (R_KCRC0_BASE + /* PRQA S 0306 # Advisory R-11.4 */
                                                   (CRC_PAIR_STRIDE_EVEN * ((uint32_t) p_cfg->channel / 2U)));
    }
    else
    {
        p_instance_ctrl->p_reg = (R_KCRC0_Type *) (R_KCRC0_BASE - CRC_GROUP_GAP + /* PRQA S 0306 # Advisory R-11.4 */
                                                   (CRC_PAIR_STRIDE_ODD * ((uint32_t) p_cfg->channel / 2U)));
    }
#endif
    p_instance_ctrl->p_cfg = p_cfg;

    /* Mark driver as initialized by setting the open value to the ASCII equivalent of "CRC" */
    crc_ctl  = (uint32_t) p_extend->control_reg->input_data_size << R_KCRC0_CTL_DW_Pos;
    crc_ctl |= (uint32_t) p_cfg->bit_order << R_KCRC0_CTL_CMD2_Pos;
    crc_ctl |= (uint32_t) p_extend->control_reg->input_mode << R_KCRC0_CTL_CMD1_Pos;
    crc_ctl |= (uint32_t) p_extend->control_reg->output_mode << R_KCRC0_CTL_CMD0_Pos;
    crc_ctl |= (uint32_t) p_extend->control_reg->poly_size << R_KCRC0_CTL_PSIZE_Pos;
    p_instance_ctrl->p_reg->CTL   = (uint32_t) crc_ctl;           /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->POLY0 = (uint32_t) (p_extend->poly0); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->POLY1 = (uint32_t) (p_extend->poly1); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->XOR0  = (uint32_t) (p_extend->xor0);  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    p_instance_ctrl->p_reg->XOR1  = (uint32_t) (p_extend->xor1);  /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    /* Load the initial value for the first continuous stream */
    if (CRC_CALCULATION_MODE_CONTINUOUS == p_extend->calculation_mode)
    {
        p_instance_ctrl->p_reg->DOUT0 = (uint32_t) (p_extend->dout0); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->DOUT1 = (uint32_t) (p_extend->dout1); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    p_instance_ctrl->open = CRC_OPEN;                                 /* PRQA S 1257 # DP_MSR_R-10.3_UC1 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Close the CRC module driver.
 *
 * Implements @ref crc_api_t::close
 *
 * @retval FSP_SUCCESS             Configuration was successful.
 * @retval FSP_ERR_ASSERTION       p_ctrl is NULL.
 * @retval FSP_ERR_NOT_OPEN        The driver is not opened.
 *
 **********************************************************************************************************************/
fsp_err_t R_CRC_Close (crc_ctrl_t * const p_ctrl)                           /* PRQA S 1505 # Advisory R-8.7 */
{
    crc_instance_ctrl_t * p_instance_ctrl = (crc_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Advisory R-11.5 */

#if CRC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(CRC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Mark driver as closed */
    p_instance_ctrl->open = 0U;

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Perform a CRC calculation on a block of 8-bit, 16-bit, 32-bit, 64-bits (for 64-bit polynomial) data.
 *
 * Implements @ref crc_api_t::calculate
 *
 * This function performs a CRC calculation on an array of 8-bit, 16-bit, 32-bit, 64-bits (for 64-bit polynomial)
 * values and returns an 8-bit, 16-bit, 32-bit, 64-bits (for 64-bit polynomial) calculated value.
 *
 * @details
 * In One-shot mode, the CRC calculation is initialized for each call to this function, and the returned value
 * represents the CRC result for the supplied input data buffer.
 *
 * In Continuous mode, the CRC calculation continues from the current CRC state established by previous calls to
 * R_CRC_Calculate(). The value returned through p_calculatedValue represents the current running (intermediate)
 * CRC value after processing the supplied input data buffer.
 *
 * @retval  FSP_SUCCESS               Calculation successful.
 * @retval  FSP_ERR_ASSERTION         Either p_ctrl, p_crc_input, p_input_buffer, or p_calculatedValue is NULL.
 * @retval  FSP_ERR_INVALID_ARGUMENT  p_crc_input->num_bytes is zero.
 * @retval  FSP_ERR_NOT_OPEN          The driver is not opened.
 * @retval  FSP_ERR_INVALID_SIZE      Provided input data size is invalid for the configured CRC input width.
 * @retval  FSP_ERR_INVALID_ADDRESS   Provided input buffer address is not aligned for the configured CRC input data width.
 *********************************************************************************************************************/
fsp_err_t R_CRC_Calculate (crc_ctrl_t * const p_ctrl, crc_input_t * const p_crc_input, crc_result_t * p_calculatedValue) /* PRQA S 1505 # Advisory R-8.7 */
{
    crc_instance_ctrl_t * p_instance_ctrl = (crc_instance_ctrl_t *) p_ctrl;                                              /* PRQA S 0316 # Advisory R-11.5 */

#if CRC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_crc_input);
    FSP_ASSERT(p_crc_input->p_input_buffer);
    FSP_ASSERT(p_calculatedValue);
    FSP_ERROR_RETURN((0UL != p_crc_input->num_bytes), FSP_ERR_INVALID_ARGUMENT);
    FSP_ERROR_RETURN(CRC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Pointer to CRC extended configuration */
    crc_extended_cfg_t const * p_extend = (crc_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

#if CRC_CFG_PARAM_CHECKING_ENABLE

    /* Check input data size */
    if (CRC_INPUT_DATA_SIZE_16_BITS == p_extend->control_reg->input_data_size)
    {
        FSP_ERROR_RETURN((p_crc_input->num_bytes & CRC_INPUT_ALIGN_16BIT) == 0U, FSP_ERR_INVALID_SIZE);
        FSP_ERROR_RETURN((((uintptr_t) p_crc_input->p_input_buffer & CRC_INPUT_ALIGN_16BIT) == 0U),
                         FSP_ERR_INVALID_ADDRESS);
    }
    else if (CRC_INPUT_DATA_SIZE_32_BITS == p_extend->control_reg->input_data_size)
    {
        FSP_ERROR_RETURN((p_crc_input->num_bytes & CRC_INPUT_ALIGN_32BIT) == 0U, FSP_ERR_INVALID_SIZE);
        FSP_ERROR_RETURN((((uintptr_t) p_crc_input->p_input_buffer & CRC_INPUT_ALIGN_32BIT) == 0U),
                         FSP_ERR_INVALID_ADDRESS);
    }
    else
    {
        /* Do nothing */
    }
#endif

    /* Restart the one-shot CRC calculation from the initial value. */
    if (CRC_CALCULATION_MODE_ONE_SHOT == p_extend->calculation_mode)
    {
        p_instance_ctrl->p_reg->DOUT0 = (uint32_t) (p_extend->dout0); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->DOUT1 = (uint32_t) (p_extend->dout1); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    /* Calculate CRC value for the input buffer */
    crc_calculate_polynomial(p_instance_ctrl, p_crc_input, p_calculatedValue);

    return FSP_SUCCESS;
}

/******************************************************************************************************************//**
 * Return the current calculated value.
 *
 * Implements @ref crc_api_t::crcResultGet
 *
 * CRC calculation operates on a running value. This function returns the current calculated value.
 *
 * @details
 * In One-shot mode, this function returns the current CRC value without reinitializing the CRC engine.
 *
 * In Continuous mode, this function returns the current CRC value and reinitializes the CRC engine to the configured
 * initial value. This allows a new continuous CRC calculation sequence to begin.
 *
 * @retval FSP_SUCCESS             Return of calculated value successful.
 * @retval FSP_ERR_ASSERTION       Either p_ctrl or p_calculatedValue is NULL.
 * @retval FSP_ERR_NOT_OPEN        The driver is not opened.
 *
 *********************************************************************************************************************/
fsp_err_t R_CRC_CalculatedValueGet (crc_ctrl_t * const p_ctrl, crc_result_t * p_calculatedValue) /* PRQA S 1505 # Advisory R-8.7 */
{
    crc_instance_ctrl_t * p_instance_ctrl = (crc_instance_ctrl_t *) p_ctrl;                      /* PRQA S 0316 # Advisory R-11.5 */

#if CRC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_calculatedValue);
    FSP_ERROR_RETURN(CRC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Pointer to CRC extended configuration */
    crc_extended_cfg_t const * p_extend = (crc_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Based on the selected polynomial, return the calculated CRC value */
    *p_calculatedValue = crc_calculated_value_get(p_instance_ctrl);

    /* Reload the initial value for the next continuous stream. */
    if (CRC_CALCULATION_MODE_CONTINUOUS == p_extend->calculation_mode)
    {
        p_instance_ctrl->p_reg->DOUT0 = (uint32_t) (p_extend->dout0); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        p_instance_ctrl->p_reg->DOUT1 = (uint32_t) (p_extend->dout1); /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref crc_api_t::snoopEnable not supported on the U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CRC_SnoopEnable (crc_ctrl_t * const p_ctrl, uint32_t crc_seed) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    FSP_PARAMETER_NOT_USED(crc_seed);
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref crc_api_t::snoopDisable not supported on the U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CRC_SnoopDisable (crc_ctrl_t * const p_ctrl) /* PRQA S 1505, 3673 # Advisory R-8.7, Advisory R-8.13 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/** @} (end addtogroup CRC) */

/**********************************************************************************************************************
 * Private Functions
 *********************************************************************************************************************/

/******************************************************************************************************************//**
 * Get CRC value
 *
 * @param[in]  p_instance_ctrl         Pointer to instance control block
 *********************************************************************************************************************/
static crc_result_t crc_calculated_value_get (crc_instance_ctrl_t * const p_instance_ctrl) /* PRQA S 3673 # Advisory R-8.13 */
{
    crc_result_t calculatedValue = 0;

    /* Combine the DOUT1 and DOUT0 values to get the final 64-bit CRC value */
    calculatedValue = ((uint64_t) p_instance_ctrl->p_reg->DOUT1_b.DOUT << CRC_DOUT1_SHIFT_BITS) | /* PRQA S 0404, 0662 # DP_MSR_R-1.3_UC1 & DP_MSR_R-13.2_UC1, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
                      p_instance_ctrl->p_reg->DOUT0_b.DOUT;                                       /* PRQA S 0662 # DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */

    return calculatedValue;
}

/******************************************************************************************************************//**
 * Perform a CRC calculation on a block of data.
 *
 * @param[in]  p_instance_ctrl         Pointer to instance control block
 * @param[in]  p_crc_input             Pointer to input data structure
 * @param[out] p_calculatedValue       Pointer to the CRC result
 *
 *********************************************************************************************************************/
static void crc_calculate_polynomial (crc_instance_ctrl_t * const p_instance_ctrl,
                                      crc_input_t * const         p_crc_input, /* PRQA S 3673 # Advisory R-8.13 */
                                      crc_result_t              * p_calculatedValue)
{
    uint32_t     num_bytes   = p_crc_input->num_bytes;
    const void * inputBuffer = p_crc_input->p_input_buffer;

    /* Pointer to CRC extended configuration */
    crc_extended_cfg_t const * p_extend = (crc_extended_cfg_t const *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0316 # Advisory R-11.5 */

    /* Write each element of the inputBuffer to the CRC Data Input Register. */
    /* Each write to the Data Input Register generates a new calculated value in the Data Output Register*/
    /* Based on the input data size, cast the inputBuffer to the appropriate pointer type */
    if (CRC_INPUT_DATA_SIZE_8_BITS == p_extend->control_reg->input_data_size)
    {
        for (uint32_t i = 0; i < num_bytes; i++)
        {
            p_instance_ctrl->p_reg->DIN = ((const uint8_t *) inputBuffer)[i]; /* PRQA S 0316, 0662 # Advisory R-11.5, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }
    else if (CRC_INPUT_DATA_SIZE_16_BITS == p_extend->control_reg->input_data_size)
    {
        uint32_t count = num_bytes >> 1;
        for (uint32_t i = 0; i < count; i++)
        {
            p_instance_ctrl->p_reg->DIN = ((const uint16_t *) inputBuffer)[i]; /* PRQA S 0316, 0662 # Advisory R-11.5, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }
    else
    {
        uint32_t count = num_bytes >> 2;
        for (uint32_t i = 0; i < count; i++)
        {
            p_instance_ctrl->p_reg->DIN = ((const uint32_t *) inputBuffer)[i]; /* PRQA S 0316, 0662 # Advisory R-11.5, DP_MSR_D-1.1_UC1 & DP_MSR_R-1.2_UC1 */
        }
    }

    /* After processing, read the calculated value from the Data Output Register */
    *p_calculatedValue = crc_calculated_value_get(p_instance_ctrl);
}

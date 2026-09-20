/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_IIC_SLAVE_H
#define R_IIC_SLAVE_H

/*********************************************************************************************************************
 * Include
 ********************************************************************************************************************/
#include "r_iic_slave_cfg.h"
#include "bsp_api.h"
#include "r_i2c_slave_api.h"

/**********************************************************************************************************************
 * Macro definitions
 *********************************************************************************************************************/

/* IIC Slave transaction enumeration */
typedef enum e_iic_slave_transfer_dir_option /* PRQA S 1536 # Advisory R-2.4 */
{
    IIC_SLAVE_TRANSFER_DIR_MASTER_READ_SLAVE_WRITE = 0x0U,
    IIC_SLAVE_TRANSFER_DIR_MASTER_WRITE_SLAVE_READ = 0x1U,
    IIC_SLAVE_TRANSFER_DIR_NOT_ESTABLISHED         = 0x2U
} iic_slave_transfer_dir_t;

/** I2C clock settings */
typedef struct iic_slave_clock_settings               /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t cks_value;                                ///< Internal Reference Clock Select
    uint8_t brl_value;                                ///< Low-level period of SCL clock
    uint8_t digital_filter_stages;                    ///< Number of digital filter stages based on brl_value
} iic_slave_clock_settings_t;

/* I2C control structure. DO NOT INITIALIZE. */
typedef struct st_iic_slave_instance_ctrl             /* PRQA S 1536 # Advisory R-2.4 */
{
    i2c_slave_cfg_t const * p_cfg;                    ///< I2C device configuration
    uint32_t                open;                     ///< Device open flag
    R_RIIC0_Type          * p_reg;                    ///< Base register address

    /* Current transfer information. */
    uint8_t     * p_buff;                             ///< Data transfer buffer
    uint32_t      total;                              ///< Total data bytes to transfer
    uint32_t      remain;                             ///< Remaining data bytes
    uint32_t      loaded;                             ///< Bytes written to register
    uint32_t      transaction_count;                  ///< Transaction counter
    volatile bool notify_request;                     ///< Master request notification flag
    volatile iic_slave_transfer_dir_t direction;      ///< Transfer direction
#if (IIC_SLAVE_CFG_TRANSFER_ENABLE)                   /* PRQA S 3332 # Required R-20.9 */
    volatile bool activation_on_rxi;                  ///< RXI transfer activation flag
    volatile bool activation_on_txi;                  ///< TXI transfer activation flag
#endif
    volatile bool do_dummy_read;                      ///< Dummy read flag
    volatile bool start_interrupt_enabled;            ///< Start interrupt enable flag
    volatile bool transaction_completed;              ///< Transaction completion flag

    /* Pointer to callback and optional working memory */
    void (* p_callback)(i2c_slave_callback_args_t *); /* PRQA S 1336 # Required R-8.2 */
    i2c_slave_callback_args_t * p_callback_memory;

    /* Pointer to context to be passed into callback function */
    void * p_context;
} iic_slave_instance_ctrl_t;

/** R_IIC_SLAVE extended configuration */
typedef struct st_iic_slave_extended_cfg       /* PRQA S 1536 # Advisory R-2.4 */
{
    iic_slave_clock_settings_t clock_settings; ///< I2C Clock settings
} iic_slave_extended_cfg_t;

extern i2c_slave_api_t const g_i2c_slave_on_iic;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_IIC_SLAVE_Open(i2c_slave_ctrl_t * const p_api_ctrl, i2c_slave_cfg_t const * const p_cfg);
fsp_err_t R_IIC_SLAVE_Write(i2c_slave_ctrl_t * const p_api_ctrl, uint8_t * const p_src, uint32_t const bytes);
fsp_err_t R_IIC_SLAVE_Read(i2c_slave_ctrl_t * const p_api_ctrl, uint8_t * const p_dest, uint32_t const bytes);
fsp_err_t R_IIC_SLAVE_Close(i2c_slave_ctrl_t * const p_api_ctrl);
fsp_err_t R_IIC_SLAVE_CallbackSet(i2c_slave_ctrl_t * const          p_api_ctrl,
                                  void (                          * p_callback)(i2c_slave_callback_args_t *), /* PRQA S 1336 # Required R-8.2 */
                                  void * const                      p_context,
                                  i2c_slave_callback_args_t * const p_callback_memory);

#endif                                                                                                        /* R_IIC_SLAVE_H */

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_IIC_MASTER_H
#define R_IIC_MASTER_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_iic_master_cfg.h"
#include "bsp_api.h"
#include "r_i2c_master_api.h"

/**********************************************************************************************************************
 * Macro definitions (Version)
 *********************************************************************************************************************/

/**********************************************************************************************************************
 * Macro definitions (Register bit)
 **********************************************************************************************************************/

/*
 *  I2C Bus Control Register 1 (IICnCR1)
 */

/* Initiates the IIC reset or internal reset (BUSCONTROL1) */
#define IIC_MASTER_PRV_SCL_SDA_NOT_DRIVEN           (0x0000001FUL) /* IIC disable */
#define IIC_MASTER_ICCR1_IICRST_BIT_MASK            (0x00000040UL) /* IIC reset */
#define IIC_MASTER_ICCR1_ICE_BIT_MASK               (0x00000080UL) /* IIC internal reset */

#define IIC_MASTER_BUS_RATE_REG_RESERVED_BITS       (0xE0U)
#define IIC_MASTER_FUNCTION_ENABLE_INIT_SETTINGS    (0x77U)

typedef enum e_iic_master_timeout_scl_low              /* PRQA S 1536 # Advisory R-2.4 */
{
    IIC_MASTER_TIMEOUT_SCL_LOW_DISABLED = 0x0U,        ///< Timeout detection during SCL low disabled
    IIC_MASTER_TIMEOUT_SCL_LOW_ENABLED  = 0x1U         ///< Timeout detection during SCL low enabled
} iic_master_timeout_scl_low_t;

typedef enum e_iic_master_timeout_scl_high             /* PRQA S 1536 # Advisory R-2.4 */
{
    IIC_MASTER_TIMEOUT_SCL_HIGH_DISABLED = 0x0U,       ///< Timeout detection during SCL high disabled
    IIC_MASTER_TIMEOUT_SCL_HIGH_ENABLED  = 0x1U        ///< Timeout detection during SCL high enabled
} iic_master_timeout_scl_high_t;

/** I2C Timeout mode parameter definition */
typedef enum e_iic_master_timeout_mode                 /* PRQA S 1536 # Advisory R-2.4 */
{
    IIC_MASTER_TIMEOUT_MODE_LONG  = 0x0U,              ///< Timeout Detection Time Select: Long Mode -> TMOS = 0
    IIC_MASTER_TIMEOUT_MODE_SHORT = 0x1U               ///< Timeout Detection Time Select: Short Mode -> TMOS = 1
} iic_master_timeout_mode_t;

typedef struct iic_master_clock_settings               /* PRQA S 1536 # Advisory R-2.4 */
{
    uint8_t cks_value;                                 ///< Internal Reference Clock Select
    uint8_t brh_value;                                 ///< High-level period of SCL clock
    uint8_t brl_value;                                 ///< Low-level period of SCL clock
} iic_master_clock_settings_t;

typedef struct st_iic_master_instance_ctrl             /* PRQA S 1536 # Advisory R-2.4 */
{
    i2c_master_cfg_t const * p_cfg;                    ///< Pointer to configuration structure
    uint32_t                 slave;                    ///< Address of the slave device
    i2c_master_addr_mode_t   addr_mode;                ///< Slave address mode interpretation

    uint32_t       open;                               ///< Device open flag
    R_RIIC0_Type * p_reg;                              ///< Base register address for channel

    /* Current transfer information. */
    uint8_t * p_buff;                                  ///< Data buffer for transfer
    uint32_t  total;                                   ///< Total data bytes to transfer
    uint32_t  remain;                                  ///< Remaining data bytes to transfer
    uint32_t  loaded;                                  ///< Bytes written to register

    uint8_t addr_low;                                  ///< Last address byte to issue
    uint8_t addr_high;                                 ///< First address byte (10-bit mode)
    uint8_t addr_total;                                ///< Total address bytes to transfer
    uint8_t addr_remain;                               ///< Remaining address bytes
    uint8_t addr_loaded;                               ///< Address bytes written to register

    volatile bool             read;                    ///< Transfer direction flag
    volatile bool             restart;                 ///< Restart condition flag
    volatile bool             err;                     ///< Error occurred flag
    volatile bool             restarted;               ///< Restart was issued flag
    volatile bool             dummy_read_completed;    ///< Dummy read completion flag
    volatile bool             activation_on_rxi;       ///< Transfer activated on RXI
    volatile bool             activation_on_txi;       ///< Transfer activated on TXI
    volatile bool             address_restarted;       ///< Restart on 10-bit read flag
    volatile bool             nack_before_stop;        ///< NACK before stop detection
    iic_master_timeout_mode_t timeout_mode;            ///< Timeout mode (long or short)

    /* Pointer to callback and optional working memory */
    void (* p_callback)(i2c_master_callback_args_t *); /* PRQA S 1336 # Required R-8.2 */
    i2c_master_callback_args_t * p_callback_memory;

    /* Pointer to context to be passed into callback function */
    void * p_context;
} iic_master_instance_ctrl_t;

/** IIC extended configuration */
typedef struct st_iic_master_extended_cfg           /* PRQA S 1536 # Advisory R-2.4 */
{
    iic_master_timeout_mode_t     timeout_mode;     ///< Timeout mode (long or short)
    iic_master_timeout_scl_low_t  timeout_scl_low;  ///< Allows timeouts to occur when SCL is held low.
    iic_master_timeout_scl_high_t timeout_scl_high; ///< Timeout when SCL held high
    iic_master_clock_settings_t   clock_settings;   ///< I2C Clock settings
    uint8_t noise_filter_stage;                     ///< Noise Filter Stage Selection
} iic_master_extended_cfg_t;

extern i2c_master_api_t const g_i2c_master_on_iic;

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_IIC_MASTER_Open(i2c_master_ctrl_t * const p_api_ctrl, i2c_master_cfg_t const * const p_cfg);
fsp_err_t R_IIC_MASTER_Close(i2c_master_ctrl_t * const p_api_ctrl);
fsp_err_t R_IIC_MASTER_Write(i2c_master_ctrl_t * const p_api_ctrl,
                             uint8_t * const           p_src,
                             uint32_t const            bytes,
                             bool const                restart);
fsp_err_t R_IIC_MASTER_Read(i2c_master_ctrl_t * const p_api_ctrl,
                            uint8_t * const           p_dest,
                            uint32_t const            bytes,
                            bool const                restart);
fsp_err_t R_IIC_MASTER_Abort(i2c_master_ctrl_t * const p_api_ctrl);
fsp_err_t R_IIC_MASTER_SlaveAddressSet(i2c_master_ctrl_t * const    p_api_ctrl,
                                       uint32_t const               slave,
                                       i2c_master_addr_mode_t const addr_mode);
fsp_err_t R_IIC_MASTER_StatusGet(i2c_master_ctrl_t * const p_api_ctrl, i2c_master_status_t * p_status);
fsp_err_t R_IIC_MASTER_CallbackSet(i2c_master_ctrl_t * const          p_api_ctrl,
                                   void (                           * p_callback)(i2c_master_callback_args_t *), /* PRQA S 1336 # Required R-8.2 */
                                   void * const                       p_context,
                                   i2c_master_callback_args_t * const p_callback_memory);

#endif                                                                                                           /* R_IIC_MASTER_H */

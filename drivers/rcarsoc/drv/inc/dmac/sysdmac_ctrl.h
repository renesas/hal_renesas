/*************************************************************************************************************
* Copyright (c) 2025 Renesas Electronics Corporation
*
* SPDX-License-Identifier: MIT
 *************************************************************************************************************/

#ifndef SYSDMAC_CTRL_H
#define SYSDMAC_CTRL_H

/**
 * @defgroup SYSDMAC_Module SYS-DMAC Module
 * @{
 * @brief This module provides functions to configure and control the operation of RT-DMAC/SYS-DMAC.
 *
 * The SYS-DMAC module allows for the configuration and control the operation of RT-DMAC.
 * It provides functions to open, close, configure, read, and write of RT-DMAC.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "dmac/dmac_common.h"

/**
 *  @brief          SYS-DMA intialize
 *  @details
 *  @param[in]      mode
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_SYSDMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode);

/**
 *  @brief          SYS-DMA Execute
 *  @details
 *  @param[in]      cfg
 *  @param[in]      descCfg
 *  @param[in]      cb
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @return         drv_RTDMAC_CH_BUSY
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_SYSDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);

/**
 *  @brief          SYS-DMA Stop
 *  @details
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_SYSDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch);

/**
 * @brief Set a callback function for SYS-DMAC events.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_callback Pointer to the callback function.
 * @param[in] p_context Pointer to the user context.
 *
 * @retval 0 on success.
 * @retval error code on failure.
 */
uint16_t R_SYSDMAC_RcarCallBackSet(dmac_ctrl_t * const p_ctrl, void ( *p_callback)(void *), void * const p_context);

#ifdef __cplusplus
}
#endif

/** @} */ // end of SYSDMAC_Module

#endif  /* SYSDMAC_CTRL_H */

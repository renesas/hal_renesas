/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef RTDMAC_CTRL_H
#define RTDMAC_CTRL_H

/**
 * @defgroup RTDMAC_Module RT-DMAC Module
 * @{
 * @brief This module provides functions to configure and control the operation of RT-DMAC/SYS-DMAC.
 *
 * The RT-DMAC module allows for the configuration and control the operation of RT-DMAC.
 * It provides functions to open, close, configure, read, and write of RT-DMAC.
 */

#ifdef __cplusplus
extern "C" {
#endif

#include "dmac/dmac_common.h"

/**
 *  @brief          RT-DMA intialize
 *  @details
 *  @param[in]      mode
 *  @return         drv_OK
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacCtrlInit(DMAC_t dev, rDmacPriorityMode_t mode);

/**
 *  @brief          RT-DMA Execute
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
uint16_t R_RTDMAC_RcarDmacExec(DMAC_t dev, uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);

/**
 *  @brief          RT-DMA Stop
 *  @details
 *  @return         drv_OK
 *  @return         drv_ERR_NOT_INITIALIZED
 *  @par    Refer   (none)
 *  @par    Modify  (none)
*/
uint16_t R_RTDMAC_RcarDmacStop(DMAC_t dev, uint8_t ch);

/**
 * @brief Set a callback function for RT-DMAC events.
 *
 * @param[in] p_ctrl Pointer to the control structure.
 * @param[in] p_callback Pointer to the callback function.
 * @param[in] p_context Pointer to the user context.
 *
 * @retval 0 on success.
 * @retval error code on failure.
*/
uint16_t R_RTDMAC_RcarCallBackSet(dmac_ctrl_t * const p_ctrl, void ( *p_callback)(void *), void * const p_context);

#ifdef __cplusplus
}
#endif

/** @} */ // end of RTDMAC_Module

#endif  /* RTDMAC_CTRL_H */

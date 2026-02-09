/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_barr.h"

/***********************************************************************************************************************
 * Variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_BARR
 *
 * @{
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Init a channel for synchronization process.
 *
 * @param channel    Sync channel ID.
 **********************************************************************************************************************/
void R_BSP_BarrChannelInit (bsp_barrier_sync_channel_t channel)
{
    BSP_REG_BRnINIT(channel) = BSP_BARRIER_SYNCHRONIZATION_INITIALIZE;
}

/*******************************************************************************************************************//**
 * @brief Enable cores (PEs) to join the synchronization process in specific channel.
 *
 * @param channel    Sync channel ID.
 * @param pe_bitmask Bitmask of PEs.
 **********************************************************************************************************************/
void R_BSP_BarrChannelEnableSet (bsp_barrier_sync_channel_t channel, uint32_t pe_bitmask)
{
    BSP_REG_BRnEN(channel) = (uint8_t) pe_bitmask;
}

/*******************************************************************************************************************//**
 * @brief Return all the CPUs that participating in Barrier-Synchronization in specific channel.
 *
 * @param channel Channel ID.
 *
 * @return Channels participating in Barrier-Synchronization
 **********************************************************************************************************************/
uint8_t R_BSP_BarrChannelEnableGet (bsp_barrier_sync_channel_t channel)
{
    /* Return value of BRnEN register */
    return BSP_REG_BRnEN(channel);
}

/*******************************************************************************************************************//**
 * @brief Set the status of a specific core (PE).
 *
 * @param channel    Sync channel ID.
 * @param peid       Processing Element ID (0–n).
 * @param status     Status to be set.
 **********************************************************************************************************************/
void R_BSP_BarrChannelStatusSet (bsp_barrier_sync_channel_t channel, uint8_t peid, uint8_t status)
{
    BSP_REG_BRnCHKm(channel, peid) = status;
}

/*******************************************************************************************************************//**
 * @brief Get the current status of a specific core (PE).
 *
 * @param channel    Sync channel ID.
 * @param peid       Processing Element ID (0–n).
 *
 * @return Status of specific channel
 **********************************************************************************************************************/
uint8_t R_BSP_BarrChannelStatusGet (bsp_barrier_sync_channel_t channel, uint8_t peid)
{
    return BSP_REG_BRnSYNCm(channel, peid);
}

/*******************************************************************************************************************//**
 * @brief Clear the status of a specific core (PE).
 *
 * @param channel    Sync channel ID.
 * @param peid       Processing Element ID (0–n).
 **********************************************************************************************************************/
void R_BSP_BarrChannelStatusClear (bsp_barrier_sync_channel_t channel, uint8_t peid)
{
    BSP_REG_BRnSYNCm(channel, peid) = BSP_BARRIER_SYNCHRONIZATION_CLEAR;
}

/*******************************************************************************************************************//**
 * @brief Get the current core (PE) ID.
 *
 * Returns the ID of the core executing this function.
 *
 * @return Core ID (0–n).
 **********************************************************************************************************************/
uint8_t R_BSP_GetCoreID (void)
{
    /* Return the unique core identifier: regID: SR0; selID: 2 */
    return (uint8_t) STSR_REGSEL(0, 2);
}

/*******************************************************************************************************************//**
 * @brief Check synchronization status of all PEs on the given channel.
 *
 * Used by a PE to verify if other cores have completed sync.
 *
 * @param channel Sync channel ID.
 **********************************************************************************************************************/
void R_BSP_PESyncCheck (bsp_barrier_sync_channel_t channel)
{
    /* Waiting until the barrier has been enabled */
    while (BSP_BARRIER_NOT_ENABLE_PE == R_BSP_BarrChannelEnableGet(channel))
    {
        NOP();                         /* Wait. */
    }
}

/*******************************************************************************************************************//**
 * @brief Enable a PE to join the synchronization process.
 *
 * Sets the bit corresponding to the PE ID to signal readiness.
 *
 * @param channel    Sync channel ID.
 * @param pe_bitmask Bitmask of PEs.
 **********************************************************************************************************************/
void R_BSP_PESyncEnable (bsp_barrier_sync_channel_t channel, uint32_t pe_bitmask)
{
    R_BSP_BarrChannelEnableSet(channel, pe_bitmask);
    R_BSP_BarrChannelInit(channel);
}

/*******************************************************************************************************************//**
 * @brief Check if all specified PEs have completed synchronization.
 *
 * Compares current sync status with the expected PE bitmask.
 *
 * @param channel Sync channel ID.
 * @param peid    Processing Element ID (0–n).
 **********************************************************************************************************************/
void R_BSP_PESyncComplete (bsp_barrier_sync_channel_t channel, uint8_t peid)
{
    /* Barrier-Synchronization n completion */
    R_BSP_BarrChannelStatusSet(channel, peid, BSP_BARRIER_SYNCHRONIZATION_COMPLETION);

    /* Waiting for barrier synchronization completion */
    while (BSP_BARRIER_SYNCHRONIZATION_COMPLETION != R_BSP_BarrChannelStatusGet(channel, peid))
    {
        NOP();
    }

    /* Clear barrier synchronization completion flag */
    R_BSP_BarrChannelStatusClear(channel, peid);
}

/** @} (end addtogroup BSP_BARR) */

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

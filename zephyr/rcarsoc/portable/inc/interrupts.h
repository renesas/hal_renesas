/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

/**
 * @defgroup Interrupt_Controller Interrupt Controller
 * @{
 * @brief This module provides functions and structures for managing interrupts.
 *
 * The Interrupt Module includes functionality for setting up, enabling, disabling,
 * and handling interrupts. It also provides mechanisms for setting interrupt priorities
 * and managing interrupt contexts.
 */

/**
 * @defgroup Interrupt_ID Interrupt ID
 * @brief Platform-specific interrupt IDs.
 * @ingroup Interrupt_Controller
 */

#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include <stdint.h>
#include "interrupt_id.h"

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
/**
 * @brief Typedef for the IRQ type.
 */
typedef enum e_irq_type
{
    TNT_TYPE_LEVEL_SENSITIVE = 0x0,
    TNT_TYPE_EDGE_TRIGGERED  = 0x2,
} r_irq_type;

/**
 * @brief Typedef for the IRQ handler function pointer.
 *
 * This typedef defines a function pointer type for IRQ handlers. The handler
 * function takes a single argument, which is a pointer to data specific to the
 * interrupt.
 *
 * @param data Pointer to the data specific to the interrupt.
 */
typedef void (*IrqHandlerFn)(void *data);

/**
 * @brief Structure to hold interrupt context information.
 *
 * This structure is used to store context-specific data and channel information
 * for handling interrupts.
 */
typedef struct
{
    void *ctx;          ///< Pointer to the context-specific data.
    int channel_info;   ///< Information about the channel of the merged interrupt.
} Context_t;

/**
 * @brief Initializes the IRQ system.
 */
void Irq_Setup(void);

/**
 * @brief Sets up an IRQ entry with the specified handler and context.
 *
 * @param id The ID of the IRQ.
 * @param Handler The function to handle the IRQ.
 * @param Context The context to be passed to the handler.
 */
void Irq_SetupEntry(unsigned int id, IrqHandlerFn Handler, Context_t *Context);

/**
 * @brief Removes an IRQ entry.
 *
 * @param id The ID of the IRQ to be removed.
 */
void Irq_RemoveEntry(unsigned int id);

/**
 * @brief Enables the specified IRQ.
 *
 * @param id The ID of the IRQ to be enabled.
 */
void Irq_Enable(unsigned int id);

/**
 * @brief Disables the specified IRQ.
 *
 * @param id The ID of the IRQ to be disabled.
 */
void Irq_Disable(unsigned int id);

/**
 * @brief Sets the priority of the specified IRQ.
 *
 * @param id The ID of the IRQ.
 * @param priority The priority level to be set.
 */
void Irq_SetPriority(unsigned int id, uint8_t priority);

/**
 * @brief Gets the ID of the currently active IRQ.
 *
 * @return The ID of the active IRQ.
 */
unsigned int FreeRTOS_GetActiveIRQ(void);

/**
 * @brief Marks the end of the interrupt for the specified IRQ.
 *
 * @param id The ID of the IRQ.
 */
void FreeRTOS_EndOfInterrupt(unsigned int id);

/**
 * @brief Gets the table ID for the specified merged IRQ.
 *
 * @param id The ID of the IRQ.
 *
 * @retval The table ID.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_GetTableId(unsigned int id);

/**
 * @brief Sets up a merged IRQ entry.
 *
 * @param id The ID of the IRQ.
 *
 * @retval 0 on success.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_MergeSetup(unsigned int id);

/**
 * @brief Gets the status register for the merged IRQ.
 *
 * @param id The ID of the IRQ.
 * @return The status register value.
 *
 * @retval Value of status register if it is a merged interrupt.
 * @retval -1 The IRQ ID is not a merged interrupt.
 */
int Irq_GetMergeStatReg(unsigned int id);

/**
 * @brief Sets the type of the specified IRQ.
 *
 * @param id The ID of the IRQ.
 * @param priority The type to be set.Bit 0: Reserved (0 - N-N model, 1 - 1-N model for some GIC before v1)
 *                                    Bit 1: 0 - level sensitive, 1 - edge triggered
 * @retval 0 on success.
 */
int Irq_SetIntType(unsigned int id, r_irq_type type);

#ifdef __cplusplus
}
#endif

/** @} */ // end of Interrupt_Controller

#endif /* INTERRUPTS_H*/

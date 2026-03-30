/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_ORED_IRQ_INFO_H
#define BSP_ORED_IRQ_INFO_H

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Create compact ORed interrupt value */
#define BSP_OREDINT_CREATE(module_id, eic, channel, ored) \
    ((((uint32_t) (module_id) & 0xFFU) << 24U) |          \
     (((uint32_t) (eic) & 0xFFU) << 16U) |                \
     (((uint32_t) (channel) & 0xFFU) << 8U) |             \
     ((uint32_t) (ored) & 0xFFU))

/** Register base addresses */
#define BSP_MSPI_MODULE_ID                  (0U)                                         ///< Module ID for MSPI
#define BSP_CANFD_MODULE_ID                 (1U)                                         ///< Module ID for CANFD

/** Macro disable value */
#define BSP_ORED_MSPI_DISABLE               ((bsp_ored_t) (0x4D524544U))                 ///< Disable MSPI ORed interrupts
#define BSP_ORED_CANFD_DISABLE              ((bsp_ored_t) (0x43524544U))                 ///< Disable CANFD ORed interrupts

/** Register stride addresses */
#define BSP_MSPI_OREDINT_EIC_BASE(ored)    ((uint32_t) (88U + ((uint32_t) (ored) * 4U))) ///< Base EIC index for MSPI ORed
#define BSP_MSPI_OREDINT_TYPE_TRANSMIT      (0U)                                         ///< MSPI ORed type transmit
#define BSP_MSPI_OREDINT_TYPE_RECEIVE       (1U)                                         ///< MSPI ORed type receive
#define BSP_MSPI_OREDINT_TYPE_FRAMECOUNT    (2U)                                         ///< MSPI ORed type framecount
#define BSP_MSPI_OREDINT_TYPE_ERROR         (3U)                                         ///< MSPI ORed type error

/** CANFD */
#define BSP_CANFD_OREDINT_ERROR(ored)    (106U + (6U * (ored)))                          ///< CANFD ORed type error
#define BSP_CANFD_OREDINT_FIFO(ored)     (107U + (6U * (ored)))                          ///< CANFD ORed type fifo
#define BSP_CANFD_OREDINT_TX(ored)       (108U + (6U * (ored)))                          ///< CANFD ORed type transmit
#define BSP_CANFD_OREDINT_VMTX(ored)     (109U + (6U * (ored)))                          ///< CANFD ORed type vmtx
#define BSP_CANFD_OREDINT_VMRX(ored)     (110U + (6U * (ored)))                          ///< CANFD ORed type vmrx
#define BSP_CANFD_OREDINT_VMERR(ored)    (111U + (6U * (ored)))                          ///< CANFD ORed type vmerr

/** CANFD Interrupt Mask */
#define BSP_CANFD_ITEM(prefix, CH, ORED, EIC_MACRO) \
    prefix ## _CANFD_CH ## CH ## _ORED ## ORED = BSP_OREDINT_CREATE(BSP_CANFD_MODULE_ID, EIC_MACRO(ORED), (CH), (ORED)),

/** Repeat 14 times to cover channels 0 to 13 */
#define BSP_CANFD_REPEAT14(prefix, ORED, EIC_MACRO) \
    BSP_CANFD_ITEM(prefix, 0, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 1, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 2, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 3, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 4, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 5, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 6, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 7, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 8, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 9, ORED, EIC_MACRO)      \
    BSP_CANFD_ITEM(prefix, 10, ORED, EIC_MACRO)     \
    BSP_CANFD_ITEM(prefix, 11, ORED, EIC_MACRO)     \
    BSP_CANFD_ITEM(prefix, 12, ORED, EIC_MACRO)     \
    BSP_CANFD_ITEM(prefix, 13, ORED, EIC_MACRO)

/** Repeat 4 times to cover ored 0 to 3 */
#define BSP_CANFD_BLOCK(prefix, EIC_MACRO)   \
    BSP_CANFD_REPEAT14(prefix, 0, EIC_MACRO) \
    BSP_CANFD_REPEAT14(prefix, 1, EIC_MACRO) \
    BSP_CANFD_REPEAT14(prefix, 2, EIC_MACRO) \
    BSP_CANFD_REPEAT14(prefix, 3, EIC_MACRO)

/** MSPI */
#define BSP_MSPI_EIC(ored, type_off)    ((uint32_t) (BSP_MSPI_OREDINT_EIC_BASE(ored) + (uint32_t) (type_off)))

/** Repeat 10 times to cover channels 0 to 9 */
#define BSP_MSPI_ITEM(prefix, TYPE_OFF, m, o) \
    prefix ## _MSPI ## m ## _ORED ## o = BSP_OREDINT_CREATE(BSP_MSPI_MODULE_ID, BSP_MSPI_EIC(o, TYPE_OFF), (m), (o)),

/** Repeat 10 times to cover channels 0 to 9 */
#define BSP_MSPI_REPEAT10(prefix, TYPE_OFF, ored)                                     \
    BSP_MSPI_ITEM(prefix, TYPE_OFF, 0, ored) BSP_MSPI_ITEM(prefix, TYPE_OFF, 1, ored) \
    BSP_MSPI_ITEM(prefix, TYPE_OFF, 2, ored) BSP_MSPI_ITEM(prefix, TYPE_OFF, 3, ored) \
    BSP_MSPI_ITEM(prefix, TYPE_OFF, 4, ored) BSP_MSPI_ITEM(prefix, TYPE_OFF, 5, ored) \
    BSP_MSPI_ITEM(prefix, TYPE_OFF, 6, ored) BSP_MSPI_ITEM(prefix, TYPE_OFF, 7, ored) \
    BSP_MSPI_ITEM(prefix, TYPE_OFF, 8, ored) BSP_MSPI_ITEM(prefix, TYPE_OFF, 9, ored)

/** Repeat 4 times to cover ored 0 to 3 */
#define BSP_MSPI_BLOCK(prefix, TYPE_OFF)   \
    BSP_MSPI_REPEAT10(prefix, TYPE_OFF, 0) \
    BSP_MSPI_REPEAT10(prefix, TYPE_OFF, 1) \
    BSP_MSPI_REPEAT10(prefix, TYPE_OFF, 2) \
    BSP_MSPI_REPEAT10(prefix, TYPE_OFF, 3)

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2C8
 * @defgroup BSP_INTERRUPT_U2C8 BSP Interrupt Events
 * @brief This module provides the macros, data types, and definition constants required for interrupt events.
 *
 * @{
 **********************************************************************************************************************/

/** Interrupt mask enumeration */
typedef enum e_bsp_ored_t
{
    BSP_MSPI_BLOCK(BSP_ORED_TRANSMIT, BSP_MSPI_OREDINT_TYPE_TRANSMIT)     ///< Interrupt mask for MSPI transmit
    BSP_MSPI_BLOCK(BSP_ORED_RECEIVE, BSP_MSPI_OREDINT_TYPE_RECEIVE)       ///< Interrupt mask for MSPI receive
    BSP_MSPI_BLOCK(BSP_ORED_FRAMECOUNT, BSP_MSPI_OREDINT_TYPE_FRAMECOUNT) ///< Interrupt mask for MSPI frame count
    BSP_MSPI_BLOCK(BSP_ORED_ERROR, BSP_MSPI_OREDINT_TYPE_ERROR)           ///< Interrupt mask for MSPI error
    BSP_CANFD_BLOCK(BSP_ORED_ERROR, BSP_CANFD_OREDINT_ERROR)              ///< Interrupt mask for CANFD error
    BSP_CANFD_BLOCK(BSP_ORED_FIFO, BSP_CANFD_OREDINT_FIFO)                ///< Interrupt mask for CANFD FIFO receive completion
    BSP_CANFD_BLOCK(BSP_ORED_TRANSMIT, BSP_CANFD_OREDINT_TX)              ///< Interrupt mask for CANFD transmit
    BSP_CANFD_BLOCK(BSP_ORED_VMTX, BSP_CANFD_OREDINT_VMTX)                ///< Interrupt mask for CANFD virtual transmit
    BSP_CANFD_BLOCK(BSP_ORED_VMRX, BSP_CANFD_OREDINT_VMRX)                ///< Interrupt mask for CANFD virtual receive
    BSP_CANFD_BLOCK(BSP_ORED_VMERR, BSP_CANFD_OREDINT_VMERR)              ///< Interrupt mask for CANFD virtual error
} bsp_ored_t;

/** @} (end addtogroup BSP_MCU_RH850U2C8) */

#endif                                 /* BSP_ORED_IRQ_INFO_H */

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_INTSEL_IRQ_INFO_H
#define BSP_INTSEL_IRQ_INFO_H

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2B10
 * @defgroup BSP_INTERRUPT_U2B10 BSP Interrupt Events
 * @brief This module provides the macros, data types, and definition constants required for interrupt events.
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Module IDs */
#define BSP_INTSEL_MOD_RLIN3               (1U)                            ///< INTSEL module ID for RLIN3
#define BSP_INTSEL_MOD_CANFD               (2U)                            ///< INTSEL module ID for CANFD
#define BSP_INTSEL_MOD_TAUD3               (3U)                            ///< INTSEL module ID for TAUD3
#define BSP_INTSEL_MOD_ENCA1               (4U)                            ///< INTSEL module ID for ENCA1
#define BSP_INTSEL_MOD_TPBA                (5U)                            ///< INTSEL module ID for TPBA

/** Disable MUX/SEL */
#define BSP_INTSEL_RLIN3N_MUX_DISABLED     ((bsp_intsel_t) (0x494E5452UL)) ///< Disable INTSEL MUX for RLIN3 ('INTx')
#define BSP_INTSEL_RCANFDN_MUX_DISABLED    ((bsp_intsel_t) (0x494E5454UL)) ///< Disable INTSEL MUX for CANFD ('INTx')
#define BSP_INTSEL_TAUD3_SEL_DISABLED      ((bsp_intsel_t) (0x494E5455UL)) ///< Disable INTSEL SEL for TAUD3 ('INTx')
#define BSP_INTSEL_ENCA1_SEL_DISABLED      ((bsp_intsel_t) (0x494E5456UL)) ///< Disable INTSEL SEL for ENCA1 ('INTx')
#define BSP_INTSEL_TPBA_SEL_DISABLED       ((bsp_intsel_t) (0x494E5457UL)) ///< Disable INTSEL SEL for TPBA ('INTx')
#define BSP_INTSEL_DISABLED                ((bsp_intsel_t) (0x494E5453UL)) ///< General disable code for INTSEL ('INTx')

/** Compact ID: [module_id | register | source_id | source_pos] */
#define BSP_INTSEL_CREATE(module_id, reg, source_id, source_pos) \
    ((((uint32_t) (module_id) & 0xFFU) << 24) |                  \
     (((uint32_t) (reg) & 0xFFU) << 16) |                        \
     (((uint32_t) (source_id) & 0xFFU) << 8) |                   \
     ((uint32_t) (source_pos) & 0xFFU))

/** RLIN3: instance = 6..22, pos=0..32, reg=pos/6, id = instance-5 */
#define BSP_INTSEL_RLIN_MUX_DEFINE(instance)                                                                   \
    BSP_INTSEL_RLIN3 ## instance ## _MUX0  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 0),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX1  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 1),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX2  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 2),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX3  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 3),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX4  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 4),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX5  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 0, ((instance) - 5), 5),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX6  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 6),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX7  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 7),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX8  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 8),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX9  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 9),  \
    BSP_INTSEL_RLIN3 ## instance ## _MUX10 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 10), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX11 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 1, ((instance) - 5), 11), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX12 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 12), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX13 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 13), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX14 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 14), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX15 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 15), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX16 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 16), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX17 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 2, ((instance) - 5), 17), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX18 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 18), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX19 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 19), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX20 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 20), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX21 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 21), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX22 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 22), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX23 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 3, ((instance) - 5), 23), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX24 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 24), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX25 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 25), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX26 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 26), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX27 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 27), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX28 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 28), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX29 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 4, ((instance) - 5), 29), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX30 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 5, ((instance) - 5), 30), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX31 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 5, ((instance) - 5), 31), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX32 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 5, ((instance) - 5), 32), \
    BSP_INTSEL_RLIN3 ## instance ## _MUX33 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_RLIN3, 5, ((instance) - 5), 33)

/** RSCANFD: instance = 0..14, pos=34..48, reg=(pos-34)/4, id = instance+1 */
#define BSP_INTSEL_RSCANFD_MUX_DEFINE(instance)                                                                  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX0  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 0),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX1  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 1),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX2  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 2),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX3  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 3),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX4  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 4),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX5  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 5),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX6  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 6),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX7  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 7),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX8  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 8),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX9  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 9),  \
    BSP_INTSEL_RSCANFD ## instance ## _MUX10 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 10), \
    BSP_INTSEL_RSCANFD ## instance ## _MUX11 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 11), \
    BSP_INTSEL_RSCANFD ## instance ## _MUX12 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 12), \
    BSP_INTSEL_RSCANFD ## instance ## _MUX13 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 13), \
    BSP_INTSEL_RSCANFD ## instance ## _MUX14 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_CANFD, 0, ((instance) + 1), 14)

/** BSP Interrupt Select Enum */
typedef enum e_bsp_intsel
{
    BSP_INTSEL_RLIN_MUX_DEFINE(6),                                                    ///< RLIN3 source 6
    BSP_INTSEL_RLIN_MUX_DEFINE(7),                                                    ///< RLIN3 source 7
    BSP_INTSEL_RLIN_MUX_DEFINE(8),                                                    ///< RLIN3 source 8
    BSP_INTSEL_RLIN_MUX_DEFINE(9),                                                    ///< RLIN3 source 9
    BSP_INTSEL_RLIN_MUX_DEFINE(10),                                                   ///< RLIN3 source 10
    BSP_INTSEL_RLIN_MUX_DEFINE(11),                                                   ///< RLIN3 source 11
    BSP_INTSEL_RLIN_MUX_DEFINE(12),                                                   ///< RLIN3 source 12
    BSP_INTSEL_RLIN_MUX_DEFINE(13),                                                   ///< RLIN3 source 13
    BSP_INTSEL_RLIN_MUX_DEFINE(14),                                                   ///< RLIN3 source 14
    BSP_INTSEL_RLIN_MUX_DEFINE(15),                                                   ///< RLIN3 source 15
    BSP_INTSEL_RLIN_MUX_DEFINE(16),                                                   ///< RLIN3 source 16
    BSP_INTSEL_RLIN_MUX_DEFINE(17),                                                   ///< RLIN3 source 17
    BSP_INTSEL_RLIN_MUX_DEFINE(18),                                                   ///< RLIN3 source 18
    BSP_INTSEL_RLIN_MUX_DEFINE(19),                                                   ///< RLIN3 source 19
    BSP_INTSEL_RLIN_MUX_DEFINE(20),                                                   ///< RLIN3 source 20
    BSP_INTSEL_RLIN_MUX_DEFINE(21),                                                   ///< RLIN3 source 21
    BSP_INTSEL_RLIN_MUX_DEFINE(22),                                                   ///< RLIN3 source 22
    BSP_INTSEL_RSCANFD_MUX_DEFINE(0),                                                 ///< RSCANFD source 0
    BSP_INTSEL_RSCANFD_MUX_DEFINE(1),                                                 ///< RSCANFD source 1
    BSP_INTSEL_RSCANFD_MUX_DEFINE(2),                                                 ///< RSCANFD source 2
    BSP_INTSEL_RSCANFD_MUX_DEFINE(3),                                                 ///< RSCANFD source 3
    BSP_INTSEL_RSCANFD_MUX_DEFINE(4),                                                 ///< RSCANFD source 4
    BSP_INTSEL_RSCANFD_MUX_DEFINE(5),                                                 ///< RSCANFD source 5
    BSP_INTSEL_RSCANFD_MUX_DEFINE(6),                                                 ///< RSCANFD source 6
    BSP_INTSEL_RSCANFD_MUX_DEFINE(7),                                                 ///< RSCANFD source 7
    BSP_INTSEL_RSCANFD_MUX_DEFINE(8),                                                 ///< RSCANFD source 8
    BSP_INTSEL_RSCANFD_MUX_DEFINE(9),                                                 ///< RSCANFD source 9
    BSP_INTSEL_RSCANFD_MUX_DEFINE(10),                                                ///< RSCANFD source 10
    BSP_INTSEL_RSCANFD_MUX_DEFINE(11),                                                ///< RSCANFD source 11
    BSP_INTSEL_RSCANFD_MUX_DEFINE(12),                                                ///< RSCANFD source 12
    BSP_INTSEL_RSCANFD_MUX_DEFINE(13),                                                ///< RSCANFD source 13
    BSP_INTSEL_RSCANFD_MUX_DEFINE(14),                                                ///< RSCANFD source 14
    BSP_INTSEL_RSCANFD_MUX_DEFINE(15),                                                ///< RSCANFD source 15
    BSP_INTSEL_RSCANFD_MUX_DEFINE(16),                                                ///< RSCANFD source 16
    BSP_INTSEL_RSCANFD_MUX_DEFINE(17),                                                ///< RSCANFD source 17
    BSP_INTSEL_RSCANFD_MUX_DEFINE(18),                                                ///< RSCANFD source 18
    BSP_INTSEL_RSCANFD_MUX_DEFINE(19),                                                ///< RSCANFD source 19
    BSP_INTSEL_TAUD3I0_SEL_379   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 0),  ///< TAUD3I0 source 379
    BSP_INTSEL_TAUD3I0_SEL_505   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 0),  ///< TAUD3I0 source 505
    BSP_INTSEL_TAUD3I0_SEL_793   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 0),  ///< TAUD3I0 source 793
    BSP_INTSEL_TAUD3I1_SEL_380   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 1),  ///< TAUD3I1 source 380
    BSP_INTSEL_TAUD3I1_SEL_506   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 1),  ///< TAUD3I1 source 506
    BSP_INTSEL_TAUD3I1_SEL_794   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 1),  ///< TAUD3I1 source 794
    BSP_INTSEL_TAUD3I2_SEL_381   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 2),  ///< TAUD3I2 source 381
    BSP_INTSEL_TAUD3I2_SEL_507   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 2),  ///< TAUD3I2 source 507
    BSP_INTSEL_TAUD3I2_SEL_795   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 2),  ///< TAUD3I2 source 795
    BSP_INTSEL_TAUD3I3_SEL_382   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 3),  ///< TAUD3I3 source 382
    BSP_INTSEL_TAUD3I3_SEL_508   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 3),  ///< TAUD3I3 source 508
    BSP_INTSEL_TAUD3I3_SEL_796   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 3),  ///< TAUD3I3 source 796
    BSP_INTSEL_TAUD3I4_SEL_383   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 4),  ///< TAUD3I4 source 383
    BSP_INTSEL_TAUD3I4_SEL_509   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 4),  ///< TAUD3I4 source 509
    BSP_INTSEL_TAUD3I4_SEL_797   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 4),  ///< TAUD3I4 source 797
    BSP_INTSEL_TAUD3I5_SEL_384   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 5),  ///< TAUD3I5 source 384
    BSP_INTSEL_TAUD3I5_SEL_510   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 5),  ///< TAUD3I5 source 510
    BSP_INTSEL_TAUD3I5_SEL_798   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 5),  ///< TAUD3I5 source 798
    BSP_INTSEL_TAUD3I6_SEL_385   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 6),  ///< TAUD3I6 source 385
    BSP_INTSEL_TAUD3I6_SEL_511   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 6),  ///< TAUD3I6 source 511
    BSP_INTSEL_TAUD3I6_SEL_799   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 6),  ///< TAUD3I6 source 799
    BSP_INTSEL_TAUD3I7_SEL_386   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 7),  ///< TAUD3I7 source 386
    BSP_INTSEL_TAUD3I7_SEL_512   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 7),  ///< TAUD3I7 source 512
    BSP_INTSEL_TAUD3I7_SEL_800   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 7),  ///< TAUD3I7 source 800
    BSP_INTSEL_TAUD3I8_SEL_387   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 8),  ///< TAUD3I8 source 387
    BSP_INTSEL_TAUD3I8_SEL_513   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 8),  ///< TAUD3I8 source 513
    BSP_INTSEL_TAUD3I8_SEL_801   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 8),  ///< TAUD3I8 source 801
    BSP_INTSEL_TAUD3I9_SEL_388   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 1, 9),  ///< TAUD3I9 source 388
    BSP_INTSEL_TAUD3I9_SEL_514   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 2, 9),  ///< TAUD3I9 source 514
    BSP_INTSEL_TAUD3I9_SEL_802   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 0, 3, 9),  ///< TAUD3I9 source 802
    BSP_INTSEL_TAUD3I10_SEL_389  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 10), ///< TAUD3I10 source 389
    BSP_INTSEL_TAUD3I10_SEL_515  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 10), ///< TAUD3I10 source 515
    BSP_INTSEL_TAUD3I10_SEL_803  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 10), ///< TAUD3I10 source 803
    BSP_INTSEL_TAUD3I11_SEL_390  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 11), ///< TAUD3I11 source 390
    BSP_INTSEL_TAUD3I11_SEL_516  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 11), ///< TAUD3I11 source 516
    BSP_INTSEL_TAUD3I11_SEL_804  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 11), ///< TAUD3I11 source 804
    BSP_INTSEL_TAUD3I12_SEL_391  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 12), ///< TAUD3I12 source 391
    BSP_INTSEL_TAUD3I12_SEL_517  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 12), ///< TAUD3I12 source 517
    BSP_INTSEL_TAUD3I12_SEL_817  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 12), ///< TAUD3I12 source 817
    BSP_INTSEL_TAUD3I13_SEL_392  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 13), ///< TAUD3I13 source 392
    BSP_INTSEL_TAUD3I13_SEL_518  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 13), ///< TAUD3I13 source 518
    BSP_INTSEL_TAUD3I13_SEL_818  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 13), ///< TAUD3I13 source 818
    BSP_INTSEL_TAUD3I14_SEL_393  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 14), ///< TAUD3I14 source 393
    BSP_INTSEL_TAUD3I14_SEL_519  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 14), ///< TAUD3I14 source 519
    BSP_INTSEL_TAUD3I14_SEL_819  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 14), ///< TAUD3I14 source 819
    BSP_INTSEL_TAUD3I15_SEL_394  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 1, 15), ///< TAUD3I15 source 394
    BSP_INTSEL_TAUD3I15_SEL_520  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 2, 15), ///< TAUD3I15 source 520
    BSP_INTSEL_TAUD3I15_SEL_820  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TAUD3, 1, 3, 15), ///< TAUD3I15 source 820
    BSP_INTSEL_ENCA1IOV_SEL_304  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 1, 0),  ///< ENCA1IOV source 304
    BSP_INTSEL_ENCA1IOV_SEL_421  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 2, 0),  ///< ENCA1IOV source 421
    BSP_INTSEL_ENCA1IOV_SEL_505  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 3, 0),  ///< ENCA1IOV source 505
    BSP_INTSEL_ENCA1I0_SEL_305   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 1, 1),  ///< ENCA1I0 source 305
    BSP_INTSEL_ENCA1I0_SEL_422   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 2, 1),  ///< ENCA1I0 source 422
    BSP_INTSEL_ENCA1I0_SEL_506   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 3, 1),  ///< ENCA1I0 source 506
    BSP_INTSEL_ENCA1I1_SEL_306   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 1, 2),  ///< ENCA1I1 source 306
    BSP_INTSEL_ENCA1I1_SEL_423   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 2, 2),  ///< ENCA1I1 source 423
    BSP_INTSEL_ENCA1I1_SEL_507   = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 3, 2),  ///< ENCA1I1 source 507
    BSP_INTSEL_ENCA1IUD_SEL_307  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 1, 3),  ///< ENCA1IUD source 307
    BSP_INTSEL_ENCA1IUD_SEL_424  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 2, 3),  ///< ENCA1IUD source 424
    BSP_INTSEL_ENCA1IUD_SEL_508  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 3, 3),  ///< ENCA1IUD source 508
    BSP_INTSEL_ENCA1IEC_SEL_308  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 1, 4),  ///< ENCA1IEC source 308
    BSP_INTSEL_ENCA1IEC_SEL_425  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 2, 4),  ///< ENCA1IEC source 425
    BSP_INTSEL_ENCA1IEC_SEL_509  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_ENCA1, 0, 3, 4),  ///< ENCA1IEC source 509
    BSP_INTSEL_TPBA0IPRD_SEL_47  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 0),   ///< TPBA0IPRD source 47
    BSP_INTSEL_TPBA0IPRD_SEL_510 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 0),   ///< TPBA0IPRD source 510
    BSP_INTSEL_TPBA0IDTY_SEL_48  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 1),   ///< TPBA0IDTY source 48
    BSP_INTSEL_TPBA0IDTY_SEL_511 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 1),   ///< TPBA0IDTY source 511
    BSP_INTSEL_TPBA0IPAT_SEL_49  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 2),   ///< TPBA0IPAT source 49
    BSP_INTSEL_TPBA0IPAT_SEL_512 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 2),   ///< TPBA0IPAT source 512
    BSP_INTSEL_TPBA1IPRD_SEL_50  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 3),   ///< TPBA1IPRD source 50
    BSP_INTSEL_TPBA1IPRD_SEL_513 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 3),   ///< TPBA1IPRD source 513
    BSP_INTSEL_TPBA1IDTY_SEL_51  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 4),   ///< TPBA1IDTY source 51
    BSP_INTSEL_TPBA1IDTY_SEL_514 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 4),   ///< TPBA1IDTY source 514
    BSP_INTSEL_TPBA1IPAT_SEL_52  = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 1, 5),   ///< TPBA1IPAT source 52
    BSP_INTSEL_TPBA1IPAT_SEL_515 = BSP_INTSEL_CREATE(BSP_INTSEL_MOD_TPBA, 0, 2, 5),   ///< TPBA1IPAT source 515
} bsp_intsel_t;

/** @} (end addtogroup BSP_MCU_RH850U2B10) */

#endif                                 /* BSP_INTSEL_IRQ_INFO_H */

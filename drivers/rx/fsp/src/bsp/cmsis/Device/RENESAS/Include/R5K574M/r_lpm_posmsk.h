/***********************************************************************************************************************
 * Copyright [2020-2026] Renesas Electronics Corporation and/or its affiliates. All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates. and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas"). No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale. Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability. No license, express or implied, to any
 * intellectual property right is granted by Renesas. This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION. RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

#ifndef R_LPM_POSMSK_H
#define R_LPM_POSMSK_H

/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                          R_LPM                                           ================ */
/* =========================================================================================================================== */

/* ========================================================  LPMSAR  ========================================================= */
#define R_LPM_LPMSAR_NONSEC21_Pos       (21UL)         /*!< NONSEC21 (Bit 21)                                     */
#define R_LPM_LPMSAR_NONSEC21_Msk       (0x200000UL)   /*!< NONSEC21 (Bitfield-Mask: 0x01)                        */
#define R_LPM_LPMSAR_NONSEC19_Pos       (19UL)         /*!< NONSEC19 (Bit 19)                                     */
#define R_LPM_LPMSAR_NONSEC19_Msk       (0x80000UL)    /*!< NONSEC19 (Bitfield-Mask: 0x01)                        */
#define R_LPM_LPMSAR_NONSEC18_Pos       (18UL)         /*!< NONSEC18 (Bit 18)                                     */
#define R_LPM_LPMSAR_NONSEC18_Msk       (0x40000UL)    /*!< NONSEC18 (Bitfield-Mask: 0x01)                        */
#define R_LPM_LPMSAR_NONSEC17_Pos       (17UL)         /*!< NONSEC17 (Bit 17)                                     */
#define R_LPM_LPMSAR_NONSEC17_Msk       (0x20000UL)    /*!< NONSEC17 (Bitfield-Mask: 0x01)                        */
#define R_LPM_LPMSAR_NONSEC8_Pos        (8UL)          /*!< NONSEC8 (Bit 8)                                       */
#define R_LPM_LPMSAR_NONSEC8_Msk        (0x100UL)      /*!< NONSEC8 (Bitfield-Mask: 0x01)                         */
#define R_LPM_LPMSAR_NONSEC4_Pos        (4UL)          /*!< NONSEC4 (Bit 4)                                       */
#define R_LPM_LPMSAR_NONSEC4_Msk        (0x10UL)       /*!< NONSEC4 (Bitfield-Mask: 0x01)                         */
#define R_LPM_LPMSAR_NONSEC2_Pos        (2UL)          /*!< NONSEC2 (Bit 2)                                       */
#define R_LPM_LPMSAR_NONSEC2_Msk        (0x4UL)        /*!< NONSEC2 (Bitfield-Mask: 0x01)                         */
#define R_LPM_LPMSAR_NONSEC1_Pos        (1UL)          /*!< NONSEC1 (Bit 1)                                       */
#define R_LPM_LPMSAR_NONSEC1_Msk        (0x2UL)        /*!< NONSEC1 (Bitfield-Mask: 0x01)                         */
#define R_LPM_LPMSAR_NONSEC0_Pos        (0UL)          /*!< NONSEC0 (Bit 0)                                       */
#define R_LPM_LPMSAR_NONSEC0_Msk        (0x1UL)        /*!< NONSEC0 (Bitfield-Mask: 0x01)                         */
/* ========================================================  PGCSAR  ========================================================= */
#define R_LPM_PGCSAR_NONSEC7_Pos        (7UL)          /*!< NONSEC7 (Bit 7)                                       */
#define R_LPM_PGCSAR_NONSEC7_Msk        (0x80UL)       /*!< NONSEC7 (Bitfield-Mask: 0x01)                         */
#define R_LPM_PGCSAR_NONSEC6_Pos        (6UL)          /*!< NONSEC6 (Bit 6)                                       */
#define R_LPM_PGCSAR_NONSEC6_Msk        (0x40UL)       /*!< NONSEC6 (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPFSAR  ========================================================= */
#define R_LPM_DPFSAR_DFPSA31_Pos        (31UL)         /*!< DFPSA31 (Bit 31)                                      */
#define R_LPM_DPFSAR_DFPSA31_Msk        (0x80000000UL) /*!< DFPSA31 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA30_Pos        (30UL)         /*!< DPFSA30 (Bit 30)                                      */
#define R_LPM_DPFSAR_DPFSA30_Msk        (0x40000000UL) /*!< DPFSA30 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA29_Pos        (29UL)         /*!< DPFSA29 (Bit 29)                                      */
#define R_LPM_DPFSAR_DPFSA29_Msk        (0x20000000UL) /*!< DPFSA29 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA25_Pos        (25UL)         /*!< DPFSA25 (Bit 25)                                      */
#define R_LPM_DPFSAR_DPFSA25_Msk        (0x2000000UL)  /*!< DPFSA25 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA24_Pos        (24UL)         /*!< DPFSA24 (Bit 24)                                      */
#define R_LPM_DPFSAR_DPFSA24_Msk        (0x1000000UL)  /*!< DPFSA24 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA20_Pos        (20UL)         /*!< DPFSA20 (Bit 20)                                      */
#define R_LPM_DPFSAR_DPFSA20_Msk        (0x100000UL)   /*!< DPFSA20 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA19_Pos        (19UL)         /*!< DPFSA19 (Bit 19)                                      */
#define R_LPM_DPFSAR_DPFSA19_Msk        (0x80000UL)    /*!< DPFSA19 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA18_Pos        (18UL)         /*!< DPFSA18 (Bit 18)                                      */
#define R_LPM_DPFSAR_DPFSA18_Msk        (0x40000UL)    /*!< DPFSA18 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA17_Pos        (17UL)         /*!< DPFSA17 (Bit 17)                                      */
#define R_LPM_DPFSAR_DPFSA17_Msk        (0x20000UL)    /*!< DPFSA17 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA16_Pos        (16UL)         /*!< DPFSA16 (Bit 16)                                      */
#define R_LPM_DPFSAR_DPFSA16_Msk        (0x10000UL)    /*!< DPFSA16 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA15_Pos        (15UL)         /*!< DPFSA15 (Bit 15)                                      */
#define R_LPM_DPFSAR_DPFSA15_Msk        (0x8000UL)     /*!< DPFSA15 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA14_Pos        (14UL)         /*!< DPFSA14 (Bit 14)                                      */
#define R_LPM_DPFSAR_DPFSA14_Msk        (0x4000UL)     /*!< DPFSA14 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA13_Pos        (13UL)         /*!< DPFSA13 (Bit 13)                                      */
#define R_LPM_DPFSAR_DPFSA13_Msk        (0x2000UL)     /*!< DPFSA13 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA12_Pos        (12UL)         /*!< DPFSA12 (Bit 12)                                      */
#define R_LPM_DPFSAR_DPFSA12_Msk        (0x1000UL)     /*!< DPFSA12 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA11_Pos        (11UL)         /*!< DPFSA11 (Bit 11)                                      */
#define R_LPM_DPFSAR_DPFSA11_Msk        (0x800UL)      /*!< DPFSA11 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA10_Pos        (10UL)         /*!< DPFSA10 (Bit 10)                                      */
#define R_LPM_DPFSAR_DPFSA10_Msk        (0x400UL)      /*!< DPFSA10 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR_DPFSA9_Pos         (9UL)          /*!< DPFSA9 (Bit 9)                                        */
#define R_LPM_DPFSAR_DPFSA9_Msk         (0x200UL)      /*!< DPFSA9 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA8_Pos         (8UL)          /*!< DPFSA8 (Bit 8)                                        */
#define R_LPM_DPFSAR_DPFSA8_Msk         (0x100UL)      /*!< DPFSA8 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA7_Pos         (7UL)          /*!< DPFSA7 (Bit 7)                                        */
#define R_LPM_DPFSAR_DPFSA7_Msk         (0x80UL)       /*!< DPFSA7 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA6_Pos         (6UL)          /*!< DPFSA6 (Bit 6)                                        */
#define R_LPM_DPFSAR_DPFSA6_Msk         (0x40UL)       /*!< DPFSA6 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA5_Pos         (5UL)          /*!< DPFSA5 (Bit 5)                                        */
#define R_LPM_DPFSAR_DPFSA5_Msk         (0x20UL)       /*!< DPFSA5 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA4_Pos         (4UL)          /*!< DPFSA4 (Bit 4)                                        */
#define R_LPM_DPFSAR_DPFSA4_Msk         (0x10UL)       /*!< DPFSA4 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA3_Pos         (3UL)          /*!< DPFSA3 (Bit 3)                                        */
#define R_LPM_DPFSAR_DPFSA3_Msk         (0x8UL)        /*!< DPFSA3 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA2_Pos         (2UL)          /*!< DPFSA2 (Bit 2)                                        */
#define R_LPM_DPFSAR_DPFSA2_Msk         (0x4UL)        /*!< DPFSA2 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA1_Pos         (1UL)          /*!< DPFSA1 (Bit 1)                                        */
#define R_LPM_DPFSAR_DPFSA1_Msk         (0x2UL)        /*!< DPFSA1 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR_DPFSA0_Pos         (0UL)          /*!< DPFSA0 (Bit 0)                                        */
#define R_LPM_DPFSAR_DPFSA0_Msk         (0x1UL)        /*!< DPFSA0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPFSAR1  ======================================================== */
#define R_LPM_DPFSAR1_DPFSA15_Pos       (15UL)         /*!< DPFSA15 (Bit 15)                                      */
#define R_LPM_DPFSAR1_DPFSA15_Msk       (0x8000UL)     /*!< DPFSA15 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA14_Pos       (14UL)         /*!< DPFSA14 (Bit 14)                                      */
#define R_LPM_DPFSAR1_DPFSA14_Msk       (0x4000UL)     /*!< DPFSA14 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA13_Pos       (13UL)         /*!< DPFSA13 (Bit 13)                                      */
#define R_LPM_DPFSAR1_DPFSA13_Msk       (0x2000UL)     /*!< DPFSA13 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA12_Pos       (12UL)         /*!< DPFSA12 (Bit 12)                                      */
#define R_LPM_DPFSAR1_DPFSA12_Msk       (0x1000UL)     /*!< DPFSA12 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA11_Pos       (11UL)         /*!< DPFSA11 (Bit 11)                                      */
#define R_LPM_DPFSAR1_DPFSA11_Msk       (0x800UL)      /*!< DPFSA11 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA10_Pos       (10UL)         /*!< DPFSA10 (Bit 10)                                      */
#define R_LPM_DPFSAR1_DPFSA10_Msk       (0x400UL)      /*!< DPFSA10 (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPFSAR1_DPFSA9_Pos        (9UL)          /*!< DPFSA9 (Bit 9)                                        */
#define R_LPM_DPFSAR1_DPFSA9_Msk        (0x200UL)      /*!< DPFSA9 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA8_Pos        (8UL)          /*!< DPFSA8 (Bit 8)                                        */
#define R_LPM_DPFSAR1_DPFSA8_Msk        (0x100UL)      /*!< DPFSA8 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA7_Pos        (7UL)          /*!< DPFSA7 (Bit 7)                                        */
#define R_LPM_DPFSAR1_DPFSA7_Msk        (0x80UL)       /*!< DPFSA7 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA6_Pos        (6UL)          /*!< DPFSA6 (Bit 6)                                        */
#define R_LPM_DPFSAR1_DPFSA6_Msk        (0x40UL)       /*!< DPFSA6 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA5_Pos        (5UL)          /*!< DPFSA5 (Bit 5)                                        */
#define R_LPM_DPFSAR1_DPFSA5_Msk        (0x20UL)       /*!< DPFSA5 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA4_Pos        (4UL)          /*!< DPFSA4 (Bit 4)                                        */
#define R_LPM_DPFSAR1_DPFSA4_Msk        (0x10UL)       /*!< DPFSA4 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA3_Pos        (3UL)          /*!< DPFSA3 (Bit 3)                                        */
#define R_LPM_DPFSAR1_DPFSA3_Msk        (0x8UL)        /*!< DPFSA3 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA2_Pos        (2UL)          /*!< DPFSA2 (Bit 2)                                        */
#define R_LPM_DPFSAR1_DPFSA2_Msk        (0x4UL)        /*!< DPFSA2 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA1_Pos        (1UL)          /*!< DPFSA1 (Bit 1)                                        */
#define R_LPM_DPFSAR1_DPFSA1_Msk        (0x2UL)        /*!< DPFSA1 (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPFSAR1_DPFSA0_Pos        (0UL)          /*!< DPFSA0 (Bit 0)                                        */
#define R_LPM_DPFSAR1_DPFSA0_Msk        (0x1UL)        /*!< DPFSA0 (Bitfield-Mask: 0x01)                          */
/* ========================================================  RSCSAR  ========================================================= */
#define R_LPM_RSCSAR_RSCSA25_Pos        (25UL)         /*!< RSCSA25 (Bit 25)                                      */
#define R_LPM_RSCSAR_RSCSA25_Msk        (0x2000000UL)  /*!< RSCSA25 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA24_Pos        (24UL)         /*!< RSCSA24 (Bit 24)                                      */
#define R_LPM_RSCSAR_RSCSA24_Msk        (0x1000000UL)  /*!< RSCSA24 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA14_Pos        (14UL)         /*!< RSCSA14 (Bit 14)                                      */
#define R_LPM_RSCSAR_RSCSA14_Msk        (0x4000UL)     /*!< RSCSA14 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA13_Pos        (13UL)         /*!< RSCSA13 (Bit 13)                                      */
#define R_LPM_RSCSAR_RSCSA13_Msk        (0x2000UL)     /*!< RSCSA13 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA12_Pos        (12UL)         /*!< RSCSA12 (Bit 12)                                      */
#define R_LPM_RSCSAR_RSCSA12_Msk        (0x1000UL)     /*!< RSCSA12 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA11_Pos        (11UL)         /*!< RSCSA11 (Bit 11)                                      */
#define R_LPM_RSCSAR_RSCSA11_Msk        (0x800UL)      /*!< RSCSA11 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA10_Pos        (10UL)         /*!< RSCSA10 (Bit 10)                                      */
#define R_LPM_RSCSAR_RSCSA10_Msk        (0x400UL)      /*!< RSCSA10 (Bitfield-Mask: 0x01)                         */
#define R_LPM_RSCSAR_RSCSA9_Pos         (9UL)          /*!< RSCSA9 (Bit 9)                                        */
#define R_LPM_RSCSAR_RSCSA9_Msk         (0x200UL)      /*!< RSCSA9 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA8_Pos         (8UL)          /*!< RSCSA8 (Bit 8)                                        */
#define R_LPM_RSCSAR_RSCSA8_Msk         (0x100UL)      /*!< RSCSA8 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA7_Pos         (7UL)          /*!< RSCSA7 (Bit 7)                                        */
#define R_LPM_RSCSAR_RSCSA7_Msk         (0x80UL)       /*!< RSCSA7 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA6_Pos         (6UL)          /*!< RSCSA6 (Bit 6)                                        */
#define R_LPM_RSCSAR_RSCSA6_Msk         (0x40UL)       /*!< RSCSA6 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA5_Pos         (5UL)          /*!< RSCSA5 (Bit 5)                                        */
#define R_LPM_RSCSAR_RSCSA5_Msk         (0x20UL)       /*!< RSCSA5 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA4_Pos         (4UL)          /*!< RSCSA4 (Bit 4)                                        */
#define R_LPM_RSCSAR_RSCSA4_Msk         (0x10UL)       /*!< RSCSA4 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA3_Pos         (3UL)          /*!< RSCSA3 (Bit 3)                                        */
#define R_LPM_RSCSAR_RSCSA3_Msk         (0x8UL)        /*!< RSCSA3 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA2_Pos         (2UL)          /*!< RSCSA2 (Bit 2)                                        */
#define R_LPM_RSCSAR_RSCSA2_Msk         (0x4UL)        /*!< RSCSA2 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA1_Pos         (1UL)          /*!< RSCSA1 (Bit 1)                                        */
#define R_LPM_RSCSAR_RSCSA1_Msk         (0x2UL)        /*!< RSCSA1 (Bitfield-Mask: 0x01)                          */
#define R_LPM_RSCSAR_RSCSA0_Pos         (0UL)          /*!< RSCSA0 (Bit 0)                                        */
#define R_LPM_RSCSAR_RSCSA0_Msk         (0x1UL)        /*!< RSCSA0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  OPCCR  ========================================================= */
#define R_LPM_OPCCR_OPCMTSF_Pos         (4UL)          /*!< OPCMTSF (Bit 4)                                       */
#define R_LPM_OPCCR_OPCMTSF_Msk         (0x10UL)       /*!< OPCMTSF (Bitfield-Mask: 0x01)                         */
#define R_LPM_OPCCR_OPCM_Pos            (0UL)          /*!< OPCM (Bit 0)                                          */
#define R_LPM_OPCCR_OPCM_Msk            (0x3UL)        /*!< OPCM (Bitfield-Mask: 0x03)                            */
/* =======================================================  PDCTRESWM  ======================================================= */
#define R_LPM_PDCTRESWM_PDPGSF_Pos      (7UL)          /*!< PDPGSF (Bit 7)                                        */
#define R_LPM_PDCTRESWM_PDPGSF_Msk      (0x80UL)       /*!< PDPGSF (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDCTRESWM_PDCSF_Pos       (6UL)          /*!< PDCSF (Bit 6)                                         */
#define R_LPM_PDCTRESWM_PDCSF_Msk       (0x40UL)       /*!< PDCSF (Bitfield-Mask: 0x01)                           */
#define R_LPM_PDCTRESWM_PDDE_Pos        (0UL)          /*!< PDDE (Bit 0)                                          */
#define R_LPM_PDCTRESWM_PDDE_Msk        (0x1UL)        /*!< PDDE (Bitfield-Mask: 0x01)                            */
/* =======================================================  PDRAMSCR0  ======================================================= */
#define R_LPM_PDRAMSCR0_RKEEP11_Pos     (11UL)         /*!< RKEEP11 (Bit 11)                                      */
#define R_LPM_PDRAMSCR0_RKEEP11_Msk     (0x800UL)      /*!< RKEEP11 (Bitfield-Mask: 0x01)                         */
#define R_LPM_PDRAMSCR0_RKEEP10_Pos     (10UL)         /*!< RKEEP10 (Bit 10)                                      */
#define R_LPM_PDRAMSCR0_RKEEP10_Msk     (0x400UL)      /*!< RKEEP10 (Bitfield-Mask: 0x01)                         */
#define R_LPM_PDRAMSCR0_RKEEP9_Pos      (9UL)          /*!< RKEEP9 (Bit 9)                                        */
#define R_LPM_PDRAMSCR0_RKEEP9_Msk      (0x200UL)      /*!< RKEEP9 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP8_Pos      (8UL)          /*!< RKEEP8 (Bit 8)                                        */
#define R_LPM_PDRAMSCR0_RKEEP8_Msk      (0x100UL)      /*!< RKEEP8 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP7_Pos      (7UL)          /*!< RKEEP7 (Bit 7)                                        */
#define R_LPM_PDRAMSCR0_RKEEP7_Msk      (0x80UL)       /*!< RKEEP7 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP6_Pos      (6UL)          /*!< RKEEP6 (Bit 6)                                        */
#define R_LPM_PDRAMSCR0_RKEEP6_Msk      (0x40UL)       /*!< RKEEP6 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP5_Pos      (5UL)          /*!< RKEEP5 (Bit 5)                                        */
#define R_LPM_PDRAMSCR0_RKEEP5_Msk      (0x20UL)       /*!< RKEEP5 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP4_Pos      (4UL)          /*!< RKEEP4 (Bit 4)                                        */
#define R_LPM_PDRAMSCR0_RKEEP4_Msk      (0x10UL)       /*!< RKEEP4 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP3_Pos      (3UL)          /*!< RKEEP3 (Bit 3)                                        */
#define R_LPM_PDRAMSCR0_RKEEP3_Msk      (0x8UL)        /*!< RKEEP3 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP2_Pos      (2UL)          /*!< RKEEP2 (Bit 2)                                        */
#define R_LPM_PDRAMSCR0_RKEEP2_Msk      (0x4UL)        /*!< RKEEP2 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP1_Pos      (1UL)          /*!< RKEEP1 (Bit 1)                                        */
#define R_LPM_PDRAMSCR0_RKEEP1_Msk      (0x2UL)        /*!< RKEEP1 (Bitfield-Mask: 0x01)                          */
#define R_LPM_PDRAMSCR0_RKEEP0_Pos      (0UL)          /*!< RKEEP0 (Bit 0)                                        */
#define R_LPM_PDRAMSCR0_RKEEP0_Msk      (0x1UL)        /*!< RKEEP0 (Bitfield-Mask: 0x01)                          */
/* =======================================================  PDRAMSCR2  ======================================================= */
#define R_LPM_PDRAMSCR2_RKEEP0_Pos      (0UL)          /*!< RKEEP0 (Bit 0)                                        */
#define R_LPM_PDRAMSCR2_RKEEP0_Msk      (0x1UL)        /*!< RKEEP0 (Bitfield-Mask: 0x01)                          */
/* =========================================================  SBYCR  ========================================================= */
#define R_LPM_SBYCR_OPE_Pos             (6UL)          /*!< OPE (Bit 6)                                           */
#define R_LPM_SBYCR_OPE_Msk             (0x40UL)       /*!< OPE (Bitfield-Mask: 0x01)                             */
/* =========================================================  SSCR1  ========================================================= */
#define R_LPM_SSCR1_SS2LP_Pos           (2UL)          /*!< SS2LP (Bit 2)                                         */
#define R_LPM_SSCR1_SS2LP_Msk           (0xcUL)        /*!< SS2LP (Bitfield-Mask: 0x03)                           */
#define R_LPM_SSCR1_SS2FR_Pos           (0UL)          /*!< SS2FR (Bit 0)                                         */
#define R_LPM_SSCR1_SS2FR_Msk           (0x1UL)        /*!< SS2FR (Bitfield-Mask: 0x01)                           */
/* =========================================================  SSCR2  ========================================================= */
#define R_LPM_SSCR2_SS2RSF_Pos          (0UL)          /*!< SS2RSF (Bit 0)                                        */
#define R_LPM_SSCR2_SS2RSF_Msk          (0x1UL)        /*!< SS2RSF (Bitfield-Mask: 0x01)                          */
/* =========================================================  LPSCR  ========================================================= */
#define R_LPM_LPSCR_LPMD_Pos            (0UL)          /*!< LPMD (Bit 0)                                          */
#define R_LPM_LPSCR_LPMD_Msk            (0xfUL)        /*!< LPMD (Bitfield-Mask: 0x0f)                            */
/* ========================================================  DPSBYCR  ======================================================== */
#define R_LPM_DPSBYCR_IOKEEP_Pos        (6UL)          /*!< IOKEEP (Bit 6)                                        */
#define R_LPM_DPSBYCR_IOKEEP_Msk        (0x40UL)       /*!< IOKEEP (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSBYCR_DCSSMODE_Pos      (2UL)          /*!< DCSSMODE (Bit 2)                                      */
#define R_LPM_DPSBYCR_DCSSMODE_Msk      (0xcUL)        /*!< DCSSMODE (Bitfield-Mask: 0x03)                        */
#define R_LPM_DPSBYCR_LDOSSMODE_Pos     (0UL)          /*!< LDOSSMODE (Bit 0)                                     */
#define R_LPM_DPSBYCR_LDOSSMODE_Msk     (0x3UL)        /*!< LDOSSMODE (Bitfield-Mask: 0x03)                       */
/* ========================================================  DPSIER0  ======================================================== */
#define R_LPM_DPSIER0_DIRQ7E_Pos        (7UL)          /*!< DIRQ7E (Bit 7)                                        */
#define R_LPM_DPSIER0_DIRQ7E_Msk        (0x80UL)       /*!< DIRQ7E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ6E_Pos        (6UL)          /*!< DIRQ6E (Bit 6)                                        */
#define R_LPM_DPSIER0_DIRQ6E_Msk        (0x40UL)       /*!< DIRQ6E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ5E_Pos        (5UL)          /*!< DIRQ5E (Bit 5)                                        */
#define R_LPM_DPSIER0_DIRQ5E_Msk        (0x20UL)       /*!< DIRQ5E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ4E_Pos        (4UL)          /*!< DIRQ4E (Bit 4)                                        */
#define R_LPM_DPSIER0_DIRQ4E_Msk        (0x10UL)       /*!< DIRQ4E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ3E_Pos        (3UL)          /*!< DIRQ3E (Bit 3)                                        */
#define R_LPM_DPSIER0_DIRQ3E_Msk        (0x8UL)        /*!< DIRQ3E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ2E_Pos        (2UL)          /*!< DIRQ2E (Bit 2)                                        */
#define R_LPM_DPSIER0_DIRQ2E_Msk        (0x4UL)        /*!< DIRQ2E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ1E_Pos        (1UL)          /*!< DIRQ1E (Bit 1)                                        */
#define R_LPM_DPSIER0_DIRQ1E_Msk        (0x2UL)        /*!< DIRQ1E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER0_DIRQ0E_Pos        (0UL)          /*!< DIRQ0E (Bit 0)                                        */
#define R_LPM_DPSIER0_DIRQ0E_Msk        (0x1UL)        /*!< DIRQ0E (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER1  ======================================================== */
#define R_LPM_DPSIER1_DIRQ15E_Pos       (7UL)          /*!< DIRQ15E (Bit 7)                                       */
#define R_LPM_DPSIER1_DIRQ15E_Msk       (0x80UL)       /*!< DIRQ15E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ14E_Pos       (6UL)          /*!< DIRQ14E (Bit 6)                                       */
#define R_LPM_DPSIER1_DIRQ14E_Msk       (0x40UL)       /*!< DIRQ14E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ13E_Pos       (5UL)          /*!< DIRQ13E (Bit 5)                                       */
#define R_LPM_DPSIER1_DIRQ13E_Msk       (0x20UL)       /*!< DIRQ13E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ12E_Pos       (4UL)          /*!< DIRQ12E (Bit 4)                                       */
#define R_LPM_DPSIER1_DIRQ12E_Msk       (0x10UL)       /*!< DIRQ12E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ11E_Pos       (3UL)          /*!< DIRQ11E (Bit 3)                                       */
#define R_LPM_DPSIER1_DIRQ11E_Msk       (0x8UL)        /*!< DIRQ11E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ10E_Pos       (2UL)          /*!< DIRQ10E (Bit 2)                                       */
#define R_LPM_DPSIER1_DIRQ10E_Msk       (0x4UL)        /*!< DIRQ10E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER1_DIRQ9E_Pos        (1UL)          /*!< DIRQ9E (Bit 1)                                        */
#define R_LPM_DPSIER1_DIRQ9E_Msk        (0x2UL)        /*!< DIRQ9E (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIER1_DIRQ8E_Pos        (0UL)          /*!< DIRQ8E (Bit 0)                                        */
#define R_LPM_DPSIER1_DIRQ8E_Msk        (0x1UL)        /*!< DIRQ8E (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIER2  ======================================================== */
#define R_LPM_DPSIER2_DNMIE_Pos         (4UL)          /*!< DNMIE (Bit 4)                                         */
#define R_LPM_DPSIER2_DNMIE_Msk         (0x10UL)       /*!< DNMIE (Bitfield-Mask: 0x01)                           */
#define R_LPM_DPSIER2_DRTCAIE_Pos       (3UL)          /*!< DRTCAIE (Bit 3)                                       */
#define R_LPM_DPSIER2_DRTCAIE_Msk       (0x8UL)        /*!< DRTCAIE (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER2_DRTCIIE_Pos       (2UL)          /*!< DRTCIIE (Bit 2)                                       */
#define R_LPM_DPSIER2_DRTCIIE_Msk       (0x4UL)        /*!< DRTCIIE (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER2_DPVD2IE_Pos       (1UL)          /*!< DPVD2IE (Bit 1)                                       */
#define R_LPM_DPSIER2_DPVD2IE_Msk       (0x2UL)        /*!< DPVD2IE (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER2_DPVD1IE_Pos       (0UL)          /*!< DPVD1IE (Bit 0)                                       */
#define R_LPM_DPSIER2_DPVD1IE_Msk       (0x1UL)        /*!< DPVD1IE (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIER3  ======================================================== */
#define R_LPM_DPSIER3_DVBATTADIE_Pos    (7UL)          /*!< DVBATTADIE (Bit 7)                                    */
#define R_LPM_DPSIER3_DVBATTADIE_Msk    (0x80UL)       /*!< DVBATTADIE (Bitfield-Mask: 0x01)                      */
#define R_LPM_DPSIER3_DSOSTDIE_Pos      (6UL)          /*!< DSOSTDIE (Bit 6)                                      */
#define R_LPM_DPSIER3_DSOSTDIE_Msk      (0x40UL)       /*!< DSOSTDIE (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIER3_DIWDTIE_Pos       (5UL)          /*!< DIWDTIE (Bit 5)                                       */
#define R_LPM_DPSIER3_DIWDTIE_Msk       (0x20UL)       /*!< DIWDTIE (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER3_DUSBHSIE_Pos      (1UL)          /*!< DUSBHSIE (Bit 1)                                      */
#define R_LPM_DPSIER3_DUSBHSIE_Msk      (0x2UL)        /*!< DUSBHSIE (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIER3_DUSBFSIE_Pos      (0UL)          /*!< DUSBFSIE (Bit 0)                                      */
#define R_LPM_DPSIER3_DUSBFSIE_Msk      (0x1UL)        /*!< DUSBFSIE (Bitfield-Mask: 0x01)                        */
/* ========================================================  DPSIER4  ======================================================== */
#define R_LPM_DPSIER4_DIRQ23E_Pos       (7UL)          /*!< DIRQ23E (Bit 7)                                       */
#define R_LPM_DPSIER4_DIRQ23E_Msk       (0x80UL)       /*!< DIRQ23E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ22E_Pos       (6UL)          /*!< DIRQ22E (Bit 6)                                       */
#define R_LPM_DPSIER4_DIRQ22E_Msk       (0x40UL)       /*!< DIRQ22E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ21E_Pos       (5UL)          /*!< DIRQ21E (Bit 5)                                       */
#define R_LPM_DPSIER4_DIRQ21E_Msk       (0x20UL)       /*!< DIRQ21E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ20E_Pos       (4UL)          /*!< DIRQ20E (Bit 4)                                       */
#define R_LPM_DPSIER4_DIRQ20E_Msk       (0x10UL)       /*!< DIRQ20E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ19E_Pos       (3UL)          /*!< DIRQ19E (Bit 3)                                       */
#define R_LPM_DPSIER4_DIRQ19E_Msk       (0x8UL)        /*!< DIRQ19E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ18E_Pos       (2UL)          /*!< DIRQ18E (Bit 2)                                       */
#define R_LPM_DPSIER4_DIRQ18E_Msk       (0x4UL)        /*!< DIRQ18E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ17E_Pos       (1UL)          /*!< DIRQ17E (Bit 1)                                       */
#define R_LPM_DPSIER4_DIRQ17E_Msk       (0x2UL)        /*!< DIRQ17E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER4_DIRQ16E_Pos       (0UL)          /*!< DIRQ16E (Bit 0)                                       */
#define R_LPM_DPSIER4_DIRQ16E_Msk       (0x1UL)        /*!< DIRQ16E (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIER5  ======================================================== */
#define R_LPM_DPSIER5_DIRQ31E_Pos       (7UL)          /*!< DIRQ31E (Bit 7)                                       */
#define R_LPM_DPSIER5_DIRQ31E_Msk       (0x80UL)       /*!< DIRQ31E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ30E_Pos       (6UL)          /*!< DIRQ30E (Bit 6)                                       */
#define R_LPM_DPSIER5_DIRQ30E_Msk       (0x40UL)       /*!< DIRQ30E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ29E_Pos       (5UL)          /*!< DIRQ29E (Bit 5)                                       */
#define R_LPM_DPSIER5_DIRQ29E_Msk       (0x20UL)       /*!< DIRQ29E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ28E_Pos       (4UL)          /*!< DIRQ28E (Bit 4)                                       */
#define R_LPM_DPSIER5_DIRQ28E_Msk       (0x10UL)       /*!< DIRQ28E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ27E_Pos       (3UL)          /*!< DIRQ27E (Bit 3)                                       */
#define R_LPM_DPSIER5_DIRQ27E_Msk       (0x8UL)        /*!< DIRQ27E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ26E_Pos       (2UL)          /*!< DIRQ26E (Bit 2)                                       */
#define R_LPM_DPSIER5_DIRQ26E_Msk       (0x4UL)        /*!< DIRQ26E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ25E_Pos       (1UL)          /*!< DIRQ25E (Bit 1)                                       */
#define R_LPM_DPSIER5_DIRQ25E_Msk       (0x2UL)        /*!< DIRQ25E (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIER5_DIRQ24E_Pos       (0UL)          /*!< DIRQ24E (Bit 0)                                       */
#define R_LPM_DPSIER5_DIRQ24E_Msk       (0x1UL)        /*!< DIRQ24E (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR0  ======================================================== */
#define R_LPM_DPSIFR0_DIRQ7F_Pos        (7UL)          /*!< DIRQ7F (Bit 7)                                        */
#define R_LPM_DPSIFR0_DIRQ7F_Msk        (0x80UL)       /*!< DIRQ7F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ6F_Pos        (6UL)          /*!< DIRQ6F (Bit 6)                                        */
#define R_LPM_DPSIFR0_DIRQ6F_Msk        (0x40UL)       /*!< DIRQ6F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ5F_Pos        (5UL)          /*!< DIRQ5F (Bit 5)                                        */
#define R_LPM_DPSIFR0_DIRQ5F_Msk        (0x20UL)       /*!< DIRQ5F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ4F_Pos        (4UL)          /*!< DIRQ4F (Bit 4)                                        */
#define R_LPM_DPSIFR0_DIRQ4F_Msk        (0x10UL)       /*!< DIRQ4F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ3F_Pos        (3UL)          /*!< DIRQ3F (Bit 3)                                        */
#define R_LPM_DPSIFR0_DIRQ3F_Msk        (0x8UL)        /*!< DIRQ3F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ2F_Pos        (2UL)          /*!< DIRQ2F (Bit 2)                                        */
#define R_LPM_DPSIFR0_DIRQ2F_Msk        (0x4UL)        /*!< DIRQ2F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ1F_Pos        (1UL)          /*!< DIRQ1F (Bit 1)                                        */
#define R_LPM_DPSIFR0_DIRQ1F_Msk        (0x2UL)        /*!< DIRQ1F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR0_DIRQ0F_Pos        (0UL)          /*!< DIRQ0F (Bit 0)                                        */
#define R_LPM_DPSIFR0_DIRQ0F_Msk        (0x1UL)        /*!< DIRQ0F (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR1  ======================================================== */
#define R_LPM_DPSIFR1_DIRQ15F_Pos       (7UL)          /*!< DIRQ15F (Bit 7)                                       */
#define R_LPM_DPSIFR1_DIRQ15F_Msk       (0x80UL)       /*!< DIRQ15F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ14F_Pos       (6UL)          /*!< DIRQ14F (Bit 6)                                       */
#define R_LPM_DPSIFR1_DIRQ14F_Msk       (0x40UL)       /*!< DIRQ14F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ13F_Pos       (5UL)          /*!< DIRQ13F (Bit 5)                                       */
#define R_LPM_DPSIFR1_DIRQ13F_Msk       (0x20UL)       /*!< DIRQ13F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ12F_Pos       (4UL)          /*!< DIRQ12F (Bit 4)                                       */
#define R_LPM_DPSIFR1_DIRQ12F_Msk       (0x10UL)       /*!< DIRQ12F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ11F_Pos       (3UL)          /*!< DIRQ11F (Bit 3)                                       */
#define R_LPM_DPSIFR1_DIRQ11F_Msk       (0x8UL)        /*!< DIRQ11F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ10F_Pos       (2UL)          /*!< DIRQ10F (Bit 2)                                       */
#define R_LPM_DPSIFR1_DIRQ10F_Msk       (0x4UL)        /*!< DIRQ10F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR1_DIRQ9F_Pos        (1UL)          /*!< DIRQ9F (Bit 1)                                        */
#define R_LPM_DPSIFR1_DIRQ9F_Msk        (0x2UL)        /*!< DIRQ9F (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIFR1_DIRQ8F_Pos        (0UL)          /*!< DIRQ8F (Bit 0)                                        */
#define R_LPM_DPSIFR1_DIRQ8F_Msk        (0x1UL)        /*!< DIRQ8F (Bitfield-Mask: 0x01)                          */
/* ========================================================  DPSIFR2  ======================================================== */
#define R_LPM_DPSIFR2_DNMIF_Pos         (4UL)          /*!< DNMIF (Bit 4)                                         */
#define R_LPM_DPSIFR2_DNMIF_Msk         (0x10UL)       /*!< DNMIF (Bitfield-Mask: 0x01)                           */
#define R_LPM_DPSIFR2_DRTCAIF_Pos       (3UL)          /*!< DRTCAIF (Bit 3)                                       */
#define R_LPM_DPSIFR2_DRTCAIF_Msk       (0x8UL)        /*!< DRTCAIF (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR2_DRTCIIF_Pos       (2UL)          /*!< DRTCIIF (Bit 2)                                       */
#define R_LPM_DPSIFR2_DRTCIIF_Msk       (0x4UL)        /*!< DRTCIIF (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR2_DPVD2IF_Pos       (1UL)          /*!< DPVD2IF (Bit 1)                                       */
#define R_LPM_DPSIFR2_DPVD2IF_Msk       (0x2UL)        /*!< DPVD2IF (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR2_DPVD1IF_Pos       (0UL)          /*!< DPVD1IF (Bit 0)                                       */
#define R_LPM_DPSIFR2_DPVD1IF_Msk       (0x1UL)        /*!< DPVD1IF (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR3  ======================================================== */
#define R_LPM_DPSIFR3_DVBATTADIF_Pos    (7UL)          /*!< DVBATTADIF (Bit 7)                                    */
#define R_LPM_DPSIFR3_DVBATTADIF_Msk    (0x80UL)       /*!< DVBATTADIF (Bitfield-Mask: 0x01)                      */
#define R_LPM_DPSIFR3_DSOSTDIF_Pos      (6UL)          /*!< DSOSTDIF (Bit 6)                                      */
#define R_LPM_DPSIFR3_DSOSTDIF_Msk      (0x40UL)       /*!< DSOSTDIF (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIFR3_DIWDTIF_Pos       (5UL)          /*!< DIWDTIF (Bit 5)                                       */
#define R_LPM_DPSIFR3_DIWDTIF_Msk       (0x20UL)       /*!< DIWDTIF (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR3_DUSBHSIF_Pos      (1UL)          /*!< DUSBHSIF (Bit 1)                                      */
#define R_LPM_DPSIFR3_DUSBHSIF_Msk      (0x2UL)        /*!< DUSBHSIF (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIFR3_DUSBFSIF_Pos      (0UL)          /*!< DUSBFSIF (Bit 0)                                      */
#define R_LPM_DPSIFR3_DUSBFSIF_Msk      (0x1UL)        /*!< DUSBFSIF (Bitfield-Mask: 0x01)                        */
/* ========================================================  DPSIFR4  ======================================================== */
#define R_LPM_DPSIFR4_DIRQ23F_Pos       (7UL)          /*!< DIRQ23F (Bit 7)                                       */
#define R_LPM_DPSIFR4_DIRQ23F_Msk       (0x80UL)       /*!< DIRQ23F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ22F_Pos       (6UL)          /*!< DIRQ22F (Bit 6)                                       */
#define R_LPM_DPSIFR4_DIRQ22F_Msk       (0x40UL)       /*!< DIRQ22F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ21F_Pos       (5UL)          /*!< DIRQ21F (Bit 5)                                       */
#define R_LPM_DPSIFR4_DIRQ21F_Msk       (0x20UL)       /*!< DIRQ21F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ20F_Pos       (4UL)          /*!< DIRQ20F (Bit 4)                                       */
#define R_LPM_DPSIFR4_DIRQ20F_Msk       (0x10UL)       /*!< DIRQ20F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ19F_Pos       (3UL)          /*!< DIRQ19F (Bit 3)                                       */
#define R_LPM_DPSIFR4_DIRQ19F_Msk       (0x8UL)        /*!< DIRQ19F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ18F_Pos       (2UL)          /*!< DIRQ18F (Bit 2)                                       */
#define R_LPM_DPSIFR4_DIRQ18F_Msk       (0x4UL)        /*!< DIRQ18F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ17F_Pos       (1UL)          /*!< DIRQ17F (Bit 1)                                       */
#define R_LPM_DPSIFR4_DIRQ17F_Msk       (0x2UL)        /*!< DIRQ17F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR4_DIRQ16F_Pos       (0UL)          /*!< DIRQ16F (Bit 0)                                       */
#define R_LPM_DPSIFR4_DIRQ16F_Msk       (0x1UL)        /*!< DIRQ16F (Bitfield-Mask: 0x01)                         */
/* ========================================================  DPSIFR5  ======================================================== */
#define R_LPM_DPSIFR5_DIRQ31F_Pos       (7UL)          /*!< DIRQ31F (Bit 7)                                       */
#define R_LPM_DPSIFR5_DIRQ31F_Msk       (0x80UL)       /*!< DIRQ31F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ30F_Pos       (6UL)          /*!< DIRQ30F (Bit 6)                                       */
#define R_LPM_DPSIFR5_DIRQ30F_Msk       (0x40UL)       /*!< DIRQ30F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ29F_Pos       (5UL)          /*!< DIRQ29F (Bit 5)                                       */
#define R_LPM_DPSIFR5_DIRQ29F_Msk       (0x20UL)       /*!< DIRQ29F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ28F_Pos       (4UL)          /*!< DIRQ28F (Bit 4)                                       */
#define R_LPM_DPSIFR5_DIRQ28F_Msk       (0x10UL)       /*!< DIRQ28F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ27F_Pos       (3UL)          /*!< DIRQ27F (Bit 3)                                       */
#define R_LPM_DPSIFR5_DIRQ27F_Msk       (0x8UL)        /*!< DIRQ27F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ26F_Pos       (2UL)          /*!< DIRQ26F (Bit 2)                                       */
#define R_LPM_DPSIFR5_DIRQ26F_Msk       (0x4UL)        /*!< DIRQ26F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ25F_Pos       (1UL)          /*!< DIRQ25F (Bit 1)                                       */
#define R_LPM_DPSIFR5_DIRQ25F_Msk       (0x2UL)        /*!< DIRQ25F (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIFR5_DIRQ24F_Pos       (0UL)          /*!< DIRQ24F (Bit 0)                                       */
#define R_LPM_DPSIFR5_DIRQ24F_Msk       (0x1UL)        /*!< DIRQ24F (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR0  ======================================================== */
#define R_LPM_DPSIEGR0_DIRQ7EG_Pos      (7UL)          /*!< DIRQ7EG (Bit 7)                                       */
#define R_LPM_DPSIEGR0_DIRQ7EG_Msk      (0x80UL)       /*!< DIRQ7EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ6EG_Pos      (6UL)          /*!< DIRQ6EG (Bit 6)                                       */
#define R_LPM_DPSIEGR0_DIRQ6EG_Msk      (0x40UL)       /*!< DIRQ6EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ5EG_Pos      (5UL)          /*!< DIRQ5EG (Bit 5)                                       */
#define R_LPM_DPSIEGR0_DIRQ5EG_Msk      (0x20UL)       /*!< DIRQ5EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ4EG_Pos      (4UL)          /*!< DIRQ4EG (Bit 4)                                       */
#define R_LPM_DPSIEGR0_DIRQ4EG_Msk      (0x10UL)       /*!< DIRQ4EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ3EG_Pos      (3UL)          /*!< DIRQ3EG (Bit 3)                                       */
#define R_LPM_DPSIEGR0_DIRQ3EG_Msk      (0x8UL)        /*!< DIRQ3EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ2EG_Pos      (2UL)          /*!< DIRQ2EG (Bit 2)                                       */
#define R_LPM_DPSIEGR0_DIRQ2EG_Msk      (0x4UL)        /*!< DIRQ2EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ1EG_Pos      (1UL)          /*!< DIRQ1EG (Bit 1)                                       */
#define R_LPM_DPSIEGR0_DIRQ1EG_Msk      (0x2UL)        /*!< DIRQ1EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR0_DIRQ0EG_Pos      (0UL)          /*!< DIRQ0EG (Bit 0)                                       */
#define R_LPM_DPSIEGR0_DIRQ0EG_Msk      (0x1UL)        /*!< DIRQ0EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR1  ======================================================== */
#define R_LPM_DPSIEGR1_DIRQ15EG_Pos     (7UL)          /*!< DIRQ15EG (Bit 7)                                      */
#define R_LPM_DPSIEGR1_DIRQ15EG_Msk     (0x80UL)       /*!< DIRQ15EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ14EG_Pos     (6UL)          /*!< DIRQ14EG (Bit 6)                                      */
#define R_LPM_DPSIEGR1_DIRQ14EG_Msk     (0x40UL)       /*!< DIRQ14EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ13EG_Pos     (5UL)          /*!< DIRQ13EG (Bit 5)                                      */
#define R_LPM_DPSIEGR1_DIRQ13EG_Msk     (0x20UL)       /*!< DIRQ13EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ12EG_Pos     (4UL)          /*!< DIRQ12EG (Bit 4)                                      */
#define R_LPM_DPSIEGR1_DIRQ12EG_Msk     (0x10UL)       /*!< DIRQ12EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ11EG_Pos     (3UL)          /*!< DIRQ11EG (Bit 3)                                      */
#define R_LPM_DPSIEGR1_DIRQ11EG_Msk     (0x8UL)        /*!< DIRQ11EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ10EG_Pos     (2UL)          /*!< DIRQ10EG (Bit 2)                                      */
#define R_LPM_DPSIEGR1_DIRQ10EG_Msk     (0x4UL)        /*!< DIRQ10EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR1_DIRQ9EG_Pos      (1UL)          /*!< DIRQ9EG (Bit 1)                                       */
#define R_LPM_DPSIEGR1_DIRQ9EG_Msk      (0x2UL)        /*!< DIRQ9EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR1_DIRQ8EG_Pos      (0UL)          /*!< DIRQ8EG (Bit 0)                                       */
#define R_LPM_DPSIEGR1_DIRQ8EG_Msk      (0x1UL)        /*!< DIRQ8EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR2  ======================================================== */
#define R_LPM_DPSIEGR2_DNMIEG_Pos       (4UL)          /*!< DNMIEG (Bit 4)                                        */
#define R_LPM_DPSIEGR2_DNMIEG_Msk       (0x10UL)       /*!< DNMIEG (Bitfield-Mask: 0x01)                          */
#define R_LPM_DPSIEGR2_DPVD2EG_Pos      (1UL)          /*!< DPVD2EG (Bit 1)                                       */
#define R_LPM_DPSIEGR2_DPVD2EG_Msk      (0x2UL)        /*!< DPVD2EG (Bitfield-Mask: 0x01)                         */
#define R_LPM_DPSIEGR2_DPVD1EG_Pos      (0UL)          /*!< DPVD1EG (Bit 0)                                       */
#define R_LPM_DPSIEGR2_DPVD1EG_Msk      (0x1UL)        /*!< DPVD1EG (Bitfield-Mask: 0x01)                         */
/* =======================================================  DPSIEGR3  ======================================================== */
#define R_LPM_DPSIEGR3_DIRQ23EG_Pos     (7UL)          /*!< DIRQ23EG (Bit 7)                                      */
#define R_LPM_DPSIEGR3_DIRQ23EG_Msk     (0x80UL)       /*!< DIRQ23EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ22EG_Pos     (6UL)          /*!< DIRQ22EG (Bit 6)                                      */
#define R_LPM_DPSIEGR3_DIRQ22EG_Msk     (0x40UL)       /*!< DIRQ22EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ21EG_Pos     (5UL)          /*!< DIRQ21EG (Bit 5)                                      */
#define R_LPM_DPSIEGR3_DIRQ21EG_Msk     (0x20UL)       /*!< DIRQ21EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ20EG_Pos     (4UL)          /*!< DIRQ20EG (Bit 4)                                      */
#define R_LPM_DPSIEGR3_DIRQ20EG_Msk     (0x10UL)       /*!< DIRQ20EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ19EG_Pos     (3UL)          /*!< DIRQ19EG (Bit 3)                                      */
#define R_LPM_DPSIEGR3_DIRQ19EG_Msk     (0x8UL)        /*!< DIRQ19EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ18EG_Pos     (2UL)          /*!< DIRQ18EG (Bit 2)                                      */
#define R_LPM_DPSIEGR3_DIRQ18EG_Msk     (0x4UL)        /*!< DIRQ18EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ17EG_Pos     (1UL)          /*!< DIRQ17EG (Bit 1)                                      */
#define R_LPM_DPSIEGR3_DIRQ17EG_Msk     (0x2UL)        /*!< DIRQ17EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR3_DIRQ16EG_Pos     (0UL)          /*!< DIRQ16EG (Bit 0)                                      */
#define R_LPM_DPSIEGR3_DIRQ16EG_Msk     (0x1UL)        /*!< DIRQ16EG (Bitfield-Mask: 0x01)                        */
/* =======================================================  DPSIEGR4  ======================================================== */
#define R_LPM_DPSIEGR4_DIRQ31EG_Pos     (7UL)          /*!< DIRQ31EG (Bit 7)                                      */
#define R_LPM_DPSIEGR4_DIRQ31EG_Msk     (0x80UL)       /*!< DIRQ31EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ30EG_Pos     (6UL)          /*!< DIRQ30EG (Bit 6)                                      */
#define R_LPM_DPSIEGR4_DIRQ30EG_Msk     (0x40UL)       /*!< DIRQ30EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ29EG_Pos     (5UL)          /*!< DIRQ29EG (Bit 5)                                      */
#define R_LPM_DPSIEGR4_DIRQ29EG_Msk     (0x20UL)       /*!< DIRQ29EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ28EG_Pos     (4UL)          /*!< DIRQ28EG (Bit 4)                                      */
#define R_LPM_DPSIEGR4_DIRQ28EG_Msk     (0x10UL)       /*!< DIRQ28EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ27EG_Pos     (3UL)          /*!< DIRQ27EG (Bit 3)                                      */
#define R_LPM_DPSIEGR4_DIRQ27EG_Msk     (0x8UL)        /*!< DIRQ27EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ26EG_Pos     (2UL)          /*!< DIRQ26EG (Bit 2)                                      */
#define R_LPM_DPSIEGR4_DIRQ26EG_Msk     (0x4UL)        /*!< DIRQ26EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ25EG_Pos     (1UL)          /*!< DIRQ25EG (Bit 1)                                      */
#define R_LPM_DPSIEGR4_DIRQ25EG_Msk     (0x2UL)        /*!< DIRQ25EG (Bitfield-Mask: 0x01)                        */
#define R_LPM_DPSIEGR4_DIRQ24EG_Pos     (0UL)          /*!< DIRQ24EG (Bit 0)                                      */
#define R_LPM_DPSIEGR4_DIRQ24EG_Msk     (0x1UL)        /*!< DIRQ24EG (Bitfield-Mask: 0x01)                        */
/* =======================================================  PLL1LDOCR  ======================================================= */
#define R_LPM_PLL1LDOCR_SKEEP_Pos       (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_LPM_PLL1LDOCR_SKEEP_Msk       (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_LPM_PLL1LDOCR_LDOSTP_Pos      (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_LPM_PLL1LDOCR_LDOSTP_Msk      (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =======================================================  PLL2LDOCR  ======================================================= */
#define R_LPM_PLL2LDOCR_SKEEP_Pos       (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_LPM_PLL2LDOCR_SKEEP_Msk       (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_LPM_PLL2LDOCR_LDOSTP_Pos      (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_LPM_PLL2LDOCR_LDOSTP_Msk      (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =======================================================  HOCOLDOCR  ======================================================= */
#define R_LPM_HOCOLDOCR_SKEEP_Pos       (1UL)          /*!< SKEEP (Bit 1)                                         */
#define R_LPM_HOCOLDOCR_SKEEP_Msk       (0x2UL)        /*!< SKEEP (Bitfield-Mask: 0x01)                           */
#define R_LPM_HOCOLDOCR_LDOSTP_Pos      (0UL)          /*!< LDOSTP (Bit 0)                                        */
#define R_LPM_HOCOLDOCR_LDOSTP_Msk      (0x1UL)        /*!< LDOSTP (Bitfield-Mask: 0x01)                          */
/* =========================================================  LVOCR  ========================================================= */
#define R_LPM_LVOCR_LVO0E_Pos           (0UL)          /*!< LVO0E (Bit 0)                                         */
#define R_LPM_LVOCR_LVO0E_Msk           (0x1UL)        /*!< LVO0E (Bitfield-Mask: 0x01)                           */
/* =========================================================  VSCR  ========================================================== */
#define R_LPM_VSCR_VSCMTSF_Pos          (4UL)          /*!< VSCMTSF (Bit 4)                                       */
#define R_LPM_VSCR_VSCMTSF_Msk          (0x10UL)       /*!< VSCMTSF (Bitfield-Mask: 0x01)                         */
#define R_LPM_VSCR_VSCM_Pos             (0UL)          /*!< VSCM (Bit 0)                                          */
#define R_LPM_VSCR_VSCM_Msk             (0x7UL)        /*!< VSCM (Bitfield-Mask: 0x07)                            */
/* =========================================================  SVSCR  ========================================================= */
#define R_LPM_SVSCR_SVSCM_Pos           (0UL)          /*!< SVSCM (Bit 0)                                         */
#define R_LPM_SVSCR_SVSCM_Msk           (0x7UL)        /*!< SVSCM (Bitfield-Mask: 0x07)                           */
/* ========================================================  CPUDSCR  ======================================================== */
#define R_LPM_CPUDSCR_PGD0_Pos          (0UL)          /*!< PGD0 (Bit 0)                                          */
#define R_LPM_CPUDSCR_PGD0_Msk          (0x1UL)        /*!< PGD0 (Bitfield-Mask: 0x01)                            */
/* =========================================================  MWMCR  ========================================================= */
#define R_LPM_MWMCR_MWM_Pos             (0UL)          /*!< MWM (Bit 0)                                           */
#define R_LPM_MWMCR_MWM_Msk             (0x3UL)        /*!< MWM (Bitfield-Mask: 0x03)                             */
/* ========================================================  LDOECR  ========================================================= */
#define R_LPM_LDOECR_EFUPTSF_Pos        (7UL)          /*!< EFUPTSF (Bit 7)                                       */
#define R_LPM_LDOECR_EFUPTSF_Msk        (0x80UL)       /*!< EFUPTSF (Bitfield-Mask: 0x01)                         */
#define R_LPM_LDOECR_EFUPE_Pos          (0UL)          /*!< EFUPE (Bit 0)                                         */
#define R_LPM_LDOECR_EFUPE_Msk          (0x1UL)        /*!< EFUPE (Bitfield-Mask: 0x01)                           */
/* =========================================================  DCCR  ========================================================== */
#define R_LPM_DCCR_WCODE_Pos            (1UL)          /*!< WCODE (Bit 1)                                         */
#define R_LPM_DCCR_WCODE_Msk            (0xfeUL)       /*!< WCODE (Bitfield-Mask: 0x7f)                           */
#define R_LPM_DCCR_DSTBYCON_Pos         (0UL)          /*!< DSTBYCON (Bit 0)                                      */
#define R_LPM_DCCR_DSTBYCON_Msk         (0x1UL)        /*!< DSTBYCON (Bitfield-Mask: 0x01)                        */

/** @} */ /* End of group PosMask_peripherals */

#endif /* R_LPM_POSMSK_H */

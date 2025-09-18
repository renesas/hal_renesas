/***********************************************************************************************************************
 * Copyright [2020-2025] Renesas Electronics Corporation and/or its affiliates. All Rights Reserved.
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

#ifndef R_DMA_POSMSK_H
#define R_DMA_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_DMA                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DMAST  ========================================================= */
#define R_DMA_DMAST_DMST_Pos         (0UL)       /*!< DMST (Bit 0)                                          */
#define R_DMA_DMAST_DMST_Msk         (0x1UL)     /*!< DMST (Bitfield-Mask: 0x01)                            */
/* =========================================================  DMIST  ========================================================= */
#define R_DMA_DMIST_DMIS7_Pos        (7UL)       /*!< DMIS7 (Bit 7)                                         */
#define R_DMA_DMIST_DMIS7_Msk        (0x80UL)    /*!< DMIS7 (Bitfield-Mask: 0x01)                           */
#define R_DMA_DMIST_DMIS6_Pos        (6UL)       /*!< DMIS6 (Bit 6)                                         */
#define R_DMA_DMIST_DMIS6_Msk        (0x40UL)    /*!< DMIS6 (Bitfield-Mask: 0x01)                           */
#define R_DMA_DMIST_DMIS5_Pos        (5UL)       /*!< DMIS5 (Bit 5)                                         */
#define R_DMA_DMIST_DMIS5_Msk        (0x20UL)    /*!< DMIS5 (Bitfield-Mask: 0x01)                           */
#define R_DMA_DMIST_DMIS4_Pos        (4UL)       /*!< DMIS4 (Bit 4)                                         */
#define R_DMA_DMIST_DMIS4_Msk        (0x10UL)    /*!< DMIS4 (Bitfield-Mask: 0x01)                           */
/* =========================================================  DMCTL  ========================================================= */
#define R_DMA_DMCTL_ERCH_Pos         (4UL)       /*!< ERCH (Bit 4)                                          */
#define R_DMA_DMCTL_ERCH_Msk         (0x10UL)    /*!< ERCH (Bitfield-Mask: 0x01)                            */
#define R_DMA_DMCTL_PR_Pos           (0UL)       /*!< PR (Bit 0)                                            */
#define R_DMA_DMCTL_PR_Msk           (0x1UL)     /*!< PR (Bitfield-Mask: 0x01)                              */
/* ========================================================  DMECHR  ========================================================= */
#define R_DMA_DMECHR_DMESTA_Pos      (16UL)      /*!< DMESTA (Bit 16)                                       */
#define R_DMA_DMECHR_DMESTA_Msk      (0x10000UL) /*!< DMESTA (Bitfield-Mask: 0x01)                          */
#define R_DMA_DMECHR_DMERRW_Pos      (12UL)      /*!< DMERRW (Bit 12)                                       */
#define R_DMA_DMECHR_DMERRW_Msk      (0x1000UL)  /*!< DMERRW (Bitfield-Mask: 0x01)                          */
#define R_DMA_DMECHR_DMECHSAM_Pos    (8UL)       /*!< DMECHSAM (Bit 8)                                      */
#define R_DMA_DMECHR_DMECHSAM_Msk    (0x100UL)   /*!< DMECHSAM (Bitfield-Mask: 0x01)                        */
#define R_DMA_DMECHR_DMECH_Pos       (0UL)       /*!< DMECH (Bit 0)                                         */
#define R_DMA_DMECHR_DMECH_Msk       (0xfUL)     /*!< DMECH (Bitfield-Mask: 0x0f)                           */

#endif /* R_DMA_POSMSK_H */

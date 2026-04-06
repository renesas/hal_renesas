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

#ifndef R_SRAM_POSMSK_H
#define R_SRAM_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_SRAM                                           ================ */
/* =========================================================================================================================== */

/* ======================================================  SRAMPRCR_S  ======================================================= */
#define R_SRAM_SRAMPRCR_S_KW_Pos         (8UL)      /*!< KW (Bit 8)                                            */
#define R_SRAM_SRAMPRCR_S_KW_Msk         (0xff00UL) /*!< KW (Bitfield-Mask: 0xff)                              */
#define R_SRAM_SRAMPRCR_S_PR_Pos         (0UL)      /*!< PR (Bit 0)                                            */
#define R_SRAM_SRAMPRCR_S_PR_Msk         (0x1UL)    /*!< PR (Bitfield-Mask: 0x01)                              */
/* =======================================================  SRAMWTSC  ======================================================== */
#define R_SRAM_SRAMWTSC_WTEN_Pos         (0UL)      /*!< WTEN (Bit 0)                                          */
#define R_SRAM_SRAMWTSC_WTEN_Msk         (0x1UL)    /*!< WTEN (Bitfield-Mask: 0x01)                            */
/* ========================================================  SRAMCR0  ======================================================== */
#define R_SRAM_SRAMCR0_TSTBYP_Pos        (7UL)      /*!< TSTBYP (Bit 7)                                        */
#define R_SRAM_SRAMCR0_TSTBYP_Msk        (0x80UL)   /*!< TSTBYP (Bitfield-Mask: 0x01)                          */
#define R_SRAM_SRAMCR0_E1STSEN_Pos       (4UL)      /*!< E1STSEN (Bit 4)                                       */
#define R_SRAM_SRAMCR0_E1STSEN_Msk       (0x10UL)   /*!< E1STSEN (Bitfield-Mask: 0x01)                         */
#define R_SRAM_SRAMCR0_ECCMOD_Pos        (2UL)      /*!< ECCMOD (Bit 2)                                        */
#define R_SRAM_SRAMCR0_ECCMOD_Msk        (0xcUL)    /*!< ECCMOD (Bitfield-Mask: 0x03)                          */
#define R_SRAM_SRAMCR0_OAD_Pos           (0UL)      /*!< OAD (Bit 0)                                           */
#define R_SRAM_SRAMCR0_OAD_Msk           (0x1UL)    /*!< OAD (Bitfield-Mask: 0x01)                             */
/* ========================================================  SRAMCR1  ======================================================== */
#define R_SRAM_SRAMCR1_TSTBYP_Pos        (7UL)      /*!< TSTBYP (Bit 7)                                        */
#define R_SRAM_SRAMCR1_TSTBYP_Msk        (0x80UL)   /*!< TSTBYP (Bitfield-Mask: 0x01)                          */
#define R_SRAM_SRAMCR1_E1STSEN_Pos       (4UL)      /*!< E1STSEN (Bit 4)                                       */
#define R_SRAM_SRAMCR1_E1STSEN_Msk       (0x10UL)   /*!< E1STSEN (Bitfield-Mask: 0x01)                         */
#define R_SRAM_SRAMCR1_ECCMOD_Pos        (2UL)      /*!< ECCMOD (Bit 2)                                        */
#define R_SRAM_SRAMCR1_ECCMOD_Msk        (0xcUL)    /*!< ECCMOD (Bitfield-Mask: 0x03)                          */
#define R_SRAM_SRAMCR1_OAD_Pos           (0UL)      /*!< OAD (Bit 0)                                           */
#define R_SRAM_SRAMCR1_OAD_Msk           (0x1UL)    /*!< OAD (Bitfield-Mask: 0x01)                             */
/* ======================================================  SRAMECCRGN0  ====================================================== */
#define R_SRAM_SRAMECCRGN0_ECCRGN_Pos    (0UL)      /*!< ECCRGN (Bit 0)                                        */
#define R_SRAM_SRAMECCRGN0_ECCRGN_Msk    (0xfUL)    /*!< ECCRGN (Bitfield-Mask: 0x0f)                          */
/* ======================================================  SRAMECCRGN1  ====================================================== */
#define R_SRAM_SRAMECCRGN1_ECCRGN_Pos    (0UL)      /*!< ECCRGN (Bit 0)                                        */
#define R_SRAM_SRAMECCRGN1_ECCRGN_Msk    (0xfUL)    /*!< ECCRGN (Bitfield-Mask: 0x0f)                          */
/* ========================================================  SRAMESR  ======================================================== */
#define R_SRAM_SRAMESR_ERR11_Pos         (3UL)      /*!< ERR11 (Bit 3)                                         */
#define R_SRAM_SRAMESR_ERR11_Msk         (0x8UL)    /*!< ERR11 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESR_ERR10_Pos         (2UL)      /*!< ERR10 (Bit 2)                                         */
#define R_SRAM_SRAMESR_ERR10_Msk         (0x4UL)    /*!< ERR10 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESR_ERR01_Pos         (1UL)      /*!< ERR01 (Bit 1)                                         */
#define R_SRAM_SRAMESR_ERR01_Msk         (0x2UL)    /*!< ERR01 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESR_ERR00_Pos         (0UL)      /*!< ERR00 (Bit 0)                                         */
#define R_SRAM_SRAMESR_ERR00_Msk         (0x1UL)    /*!< ERR00 (Bitfield-Mask: 0x01)                           */
/* =======================================================  SRAMESCLR  ======================================================= */
#define R_SRAM_SRAMESCLR_CLR11_Pos       (3UL)      /*!< CLR11 (Bit 3)                                         */
#define R_SRAM_SRAMESCLR_CLR11_Msk       (0x8UL)    /*!< CLR11 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESCLR_CLR10_Pos       (2UL)      /*!< CLR10 (Bit 2)                                         */
#define R_SRAM_SRAMESCLR_CLR10_Msk       (0x4UL)    /*!< CLR10 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESCLR_CLR01_Pos       (1UL)      /*!< CLR01 (Bit 1)                                         */
#define R_SRAM_SRAMESCLR_CLR01_Msk       (0x2UL)    /*!< CLR01 (Bitfield-Mask: 0x01)                           */
#define R_SRAM_SRAMESCLR_CLR00_Pos       (0UL)      /*!< CLR00 (Bit 0)                                         */
#define R_SRAM_SRAMESCLR_CLR00_Msk       (0x1UL)    /*!< CLR00 (Bitfield-Mask: 0x01)                           */
/* =======================================================  SRAMEAR00  ======================================================= */
/* =======================================================  SRAMEAR10  ======================================================= */
/* =======================================================  SRAMEAR01  ======================================================= */
/* =======================================================  SRAMEAR11  ======================================================= */

/* =========================================================================================================================== */
/* ================                                         R_SRAM_NS                                         ================ */
/* =========================================================================================================================== */

/* ======================================================  SRAMPRCR_NS  ====================================================== */
#define R_SRAM_NS_SRAMPRCR_NS_KW_Pos    (8UL)      /*!< KW (Bit 8)                                            */
#define R_SRAM_NS_SRAMPRCR_NS_KW_Msk    (0xff00UL) /*!< KW (Bitfield-Mask: 0xff)                              */
#define R_SRAM_NS_SRAMPRCR_NS_PR_Pos    (0UL)      /*!< PR (Bit 0)                                            */
#define R_SRAM_NS_SRAMPRCR_NS_PR_Msk    (0x1UL)    /*!< PR (Bitfield-Mask: 0x01)                              */

#endif /* R_SRAM_POSMSK_H */

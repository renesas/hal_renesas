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

#ifndef R_WDT_POSMSK_H
#define R_WDT_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_WDT                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  WDTRR  ========================================================= */
#define R_WDT_WDTRR_WDTRR_Pos        (0UL)      /*!< WDTRR (Bit 0)                                         */
#define R_WDT_WDTRR_WDTRR_Msk        (0xffUL)   /*!< WDTRR (Bitfield-Mask: 0xff)                           */
/* =========================================================  WDTCR  ========================================================= */
#define R_WDT_WDTCR_RPSS_Pos         (12UL)     /*!< RPSS (Bit 12)                                         */
#define R_WDT_WDTCR_RPSS_Msk         (0x3000UL) /*!< RPSS (Bitfield-Mask: 0x03)                            */
#define R_WDT_WDTCR_RPES_Pos         (8UL)      /*!< RPES (Bit 8)                                          */
#define R_WDT_WDTCR_RPES_Msk         (0x300UL)  /*!< RPES (Bitfield-Mask: 0x03)                            */
#define R_WDT_WDTCR_CKS_Pos          (4UL)      /*!< CKS (Bit 4)                                           */
#define R_WDT_WDTCR_CKS_Msk          (0xf0UL)   /*!< CKS (Bitfield-Mask: 0x0f)                             */
#define R_WDT_WDTCR_TOPS_Pos         (0UL)      /*!< TOPS (Bit 0)                                          */
#define R_WDT_WDTCR_TOPS_Msk         (0x3UL)    /*!< TOPS (Bitfield-Mask: 0x03)                            */
/* =========================================================  WDTSR  ========================================================= */
#define R_WDT_WDTSR_REFEF_Pos        (15UL)     /*!< REFEF (Bit 15)                                        */
#define R_WDT_WDTSR_REFEF_Msk        (0x8000UL) /*!< REFEF (Bitfield-Mask: 0x01)                           */
#define R_WDT_WDTSR_UNDFF_Pos        (14UL)     /*!< UNDFF (Bit 14)                                        */
#define R_WDT_WDTSR_UNDFF_Msk        (0x4000UL) /*!< UNDFF (Bitfield-Mask: 0x01)                           */
#define R_WDT_WDTSR_CNTVAL_Pos       (0UL)      /*!< CNTVAL (Bit 0)                                        */
#define R_WDT_WDTSR_CNTVAL_Msk       (0x3fffUL) /*!< CNTVAL (Bitfield-Mask: 0x3fff)                        */
/* ========================================================  WDTRCR  ========================================================= */
#define R_WDT_WDTRCR_RSTIRQS_Pos     (7UL)      /*!< RSTIRQS (Bit 7)                                       */
#define R_WDT_WDTRCR_RSTIRQS_Msk     (0x80UL)   /*!< RSTIRQS (Bitfield-Mask: 0x01)                         */
/* =======================================================  WDTCSTPR  ======================================================== */
#define R_WDT_WDTCSTPR_SLCSTP_Pos    (7UL)      /*!< SLCSTP (Bit 7)                                        */
#define R_WDT_WDTCSTPR_SLCSTP_Msk    (0x80UL)   /*!< SLCSTP (Bitfield-Mask: 0x01)                          */

#endif /* R_WDT_POSMSK_H */

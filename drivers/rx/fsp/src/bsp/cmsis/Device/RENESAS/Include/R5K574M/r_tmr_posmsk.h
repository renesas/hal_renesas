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

#ifndef R_TMR_POSMSK_H
#define R_TMR_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_TMR                                           ================ */
/* =========================================================================================================================== */

/* ==========================================================  TCR  ========================================================== */
#define R_TMR_TCR_CMIEB_Pos      (7UL)    /*!< CMIEB (Bit 7)                                         */
#define R_TMR_TCR_CMIEB_Msk      (0x80UL) /*!< CMIEB (Bitfield-Mask: 0x01)                           */
#define R_TMR_TCR_CMIEA_Pos      (6UL)    /*!< CMIEA (Bit 6)                                         */
#define R_TMR_TCR_CMIEA_Msk      (0x40UL) /*!< CMIEA (Bitfield-Mask: 0x01)                           */
#define R_TMR_TCR_OVIE_Pos       (5UL)    /*!< OVIE (Bit 5)                                          */
#define R_TMR_TCR_OVIE_Msk       (0x20UL) /*!< OVIE (Bitfield-Mask: 0x01)                            */
#define R_TMR_TCR_CCLR_Pos       (3UL)    /*!< CCLR (Bit 3)                                          */
#define R_TMR_TCR_CCLR_Msk       (0x18UL) /*!< CCLR (Bitfield-Mask: 0x03)                            */
/* =========================================================  TCSR  ========================================================== */
#define R_TMR_TCSR_OSB_Pos       (2UL)    /*!< OSB (Bit 2)                                           */
#define R_TMR_TCSR_OSB_Msk       (0xcUL)  /*!< OSB (Bitfield-Mask: 0x03)                             */
#define R_TMR_TCSR_OSA_Pos       (0UL)    /*!< OSA (Bit 0)                                           */
#define R_TMR_TCSR_OSA_Msk       (0x3UL)  /*!< OSA (Bitfield-Mask: 0x03)                             */
/* =========================================================  TCORA  ========================================================= */
#define R_TMR_TCORA_TCORA_Pos    (0UL)    /*!< TCORA (Bit 0)                                         */
#define R_TMR_TCORA_TCORA_Msk    (0xffUL) /*!< TCORA (Bitfield-Mask: 0xff)                           */
/* =========================================================  TCORB  ========================================================= */
/* =========================================================  TCNT  ========================================================== */
#define R_TMR_TCNT_TCNT_Pos      (0UL)    /*!< TCNT (Bit 0)                                          */
#define R_TMR_TCNT_TCNT_Msk      (0xffUL) /*!< TCNT (Bitfield-Mask: 0xff)                            */
/* =========================================================  TCCR  ========================================================== */
#define R_TMR_TCCR_TMRIS_Pos     (7UL)    /*!< TMRIS (Bit 7)                                         */
#define R_TMR_TCCR_TMRIS_Msk     (0x80UL) /*!< TMRIS (Bitfield-Mask: 0x01)                           */
#define R_TMR_TCCR_CSS_Pos       (3UL)    /*!< CSS (Bit 3)                                           */
#define R_TMR_TCCR_CSS_Msk       (0x18UL) /*!< CSS (Bitfield-Mask: 0x03)                             */
#define R_TMR_TCCR_CKS_Pos       (0UL)    /*!< CKS (Bit 0)                                           */
#define R_TMR_TCCR_CKS_Msk       (0x7UL)  /*!< CKS (Bitfield-Mask: 0x07)                             */
/* =========================================================  TCSTR  ========================================================= */
#define R_TMR_TCSTR_TCS_Pos      (0UL)    /*!< TCS (Bit 0)                                           */
#define R_TMR_TCSTR_TCS_Msk      (0x1UL)  /*!< TCS (Bitfield-Mask: 0x01)                             */

#endif /* R_TMR_P OSMSK_H*/

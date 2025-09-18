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

#ifndef R_CMT_POSMSK_H
#define R_CMT_POSMSK_H

/* =========================================================================================================================== */
/* ================                                            CM                                             ================ */
/* =========================================================================================================================== */

/* ==========================================================  CR  =========================================================== */
#define R_CMT_UNT_CM_CR_CKS_Pos     (0UL)    /*!< CKS (Bit 0)                                           */
#define R_CMT_UNT_CM_CR_CKS_Msk     (0x3UL)  /*!< CKS (Bitfield-Mask: 0x03)                             */
#define R_CMT_UNT_CM_CR_CMIE_Pos    (6UL)    /*!< CMIE (Bit 6)                                          */
#define R_CMT_UNT_CM_CR_CMIE_Msk    (0x40UL) /*!< CMIE (Bitfield-Mask: 0x01)                            */
/* ==========================================================  CNT  ========================================================== */
/* ==========================================================  COR  ========================================================== */

/* =========================================================================================================================== */
/* ================                                            UNT                                            ================ */
/* =========================================================================================================================== */

/* ========================================================  CMSTR0  ========================================================= */
#define R_CMT_UNT_CMSTR0_STR0_Pos    (0UL)   /*!< STR0 (Bit 0)                                          */
#define R_CMT_UNT_CMSTR0_STR0_Msk    (0x1UL) /*!< STR0 (Bitfield-Mask: 0x01)                            */
#define R_CMT_UNT_CMSTR0_STR1_Pos    (1UL)   /*!< STR1 (Bit 1)                                          */
#define R_CMT_UNT_CMSTR0_STR1_Msk    (0x2UL) /*!< STR1 (Bitfield-Mask: 0x01)                            */

#endif /* R_CMT_POSMSK_H */

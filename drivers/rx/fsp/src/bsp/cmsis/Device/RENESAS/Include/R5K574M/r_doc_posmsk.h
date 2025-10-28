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

#ifndef R_DOC_B_POSMSK_H
#define R_DOC_B_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_DOC_B                                         ================ */
/* =========================================================================================================================== */

/* =========================================================  DOCR  ========================================================== */
#define R_DOC_B_DOCR_DOPCIE_Pos      (7UL)    /*!< DOPCIE (Bit 7)                                      */
#define R_DOC_B_DOCR_DOPCIE_Msk      (0x80UL) /*!< DOPCIE (Bitfield-Mask: 0x01)                        */
#define R_DOC_B_DOCR_DCSEL_Pos       (4UL)    /*!< DCSEL (Bit 4)                                       */
#define R_DOC_B_DOCR_DCSEL_Msk       (0x70UL) /*!< DCSEL (Bitfield-Mask: 0x07)                         */
#define R_DOC_B_DOCR_DOPSZ_Pos       (3UL)    /*!< DOPSZ (Bit 3)                                       */
#define R_DOC_B_DOCR_DOPSZ_Msk       (0x8UL)  /*!< DOPSZ (Bitfield-Mask: 0x01)                         */
#define R_DOC_B_DOCR_OMS_Pos         (0UL)    /*!< OMS (Bit 0)                                         */
#define R_DOC_B_DOCR_OMS_Msk         (0x3UL)  /*!< OMS (Bitfield-Mask: 0x03)                           */
/* =========================================================  DOSR  ========================================================== */
#define R_DOC_B_DOSR_DOPCF_Pos       (0UL)    /*!< DOPCF (Bit 0)                                       */
#define R_DOC_B_DOSR_DOPCF_Msk       (0x1UL)  /*!< DOPCF (Bitfield-Mask: 0x01)                         */
/* =========================================================  DOSCR  ========================================================= */
#define R_DOC_B_DOSCR_DOPCFCL_Pos    (0UL)    /*!< DOPCFCL (Bit 0)                                     */
#define R_DOC_B_DOSCR_DOPCFCL_Msk    (0x1UL)  /*!< DOPCFCL (Bitfield-Mask: 0x01)                       */
/* =========================================================  DODIR  ========================================================= */
/* ========================================================  DODSR0  ========================================================= */
/* ========================================================  DODSR1  ========================================================= */

#endif                                 /* R_DOC_B_POSMSK_H */

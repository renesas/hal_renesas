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

#ifndef R_DTC_POSMSK_H
#define R_DTC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                Pos/Mask Peripheral Section                                ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_peripherals
 * @{
 */

/* =========================================================================================================================== */
/* ================                                           R_DTC                                           ================ */
/* =========================================================================================================================== */

/* =======================================================  DTCCR_SEC  ======================================================= */
#define R_DTC_DTCCR_SEC_RRSS_Pos           (4UL)          /*!< RRSS (Bit 4)                                          */
#define R_DTC_DTCCR_SEC_RRSS_Msk           (0x10UL)       /*!< RRSS (Bitfield-Mask: 0x01)                            */
/* ======================================================  DTCVBR_SEC  ======================================================= */
#define R_DTC_DTCVBR_SEC_DTCVBR_SEC_Pos    (10UL)         /*!< DTCVBR_SEC (Bit 10)                                   */
#define R_DTC_DTCVBR_SEC_DTCVBR_SEC_Msk    (0xfffffc00UL) /*!< DTCVBR_SEC (Bitfield-Mask: 0x3fffff)                  */
/* =========================================================  DTCST  ========================================================= */
#define R_DTC_DTCST_DTCST_Pos              (0UL)          /*!< DTCST (Bit 0)                                         */
#define R_DTC_DTCST_DTCST_Msk              (0x1UL)        /*!< DTCST (Bitfield-Mask: 0x01)                           */
/* ========================================================  DTCSTS  ========================================================= */
#define R_DTC_DTCSTS_ACT_Pos               (15UL)         /*!< ACT (Bit 15)                                          */
#define R_DTC_DTCSTS_ACT_Msk               (0x8000UL)     /*!< ACT (Bitfield-Mask: 0x01)                             */
#define R_DTC_DTCSTS_VECN_Pos              (0UL)          /*!< VECN (Bit 0)                                          */
#define R_DTC_DTCSTS_VECN_Msk              (0xffUL)       /*!< VECN (Bitfield-Mask: 0xff)                            */
/* ========================================================  DTCDISP  ======================================================== */
#define R_DTC_DTCDISP_DTCDISP_Pos          (0UL)          /*!< DTCDISP (Bit 0)                                       */
#define R_DTC_DTCDISP_DTCDISP_Msk          (0xffffffffUL) /*!< DTCDISP (Bitfield-Mask: 0xffffffff)                   */
/* =========================================================  DTEVR  ========================================================= */
#define R_DTC_DTEVR_DTESTA_Pos             (16UL)         /*!< DTESTA (Bit 16)                                       */
#define R_DTC_DTEVR_DTESTA_Msk             (0x10000UL)    /*!< DTESTA (Bitfield-Mask: 0x01)                          */
#define R_DTC_DTEVR_DTEWB_Pos              (15UL)         /*!< DTEWB (Bit 15)                                        */
#define R_DTC_DTEVR_DTEWB_Msk              (0x8000UL)     /*!< DTEWB (Bitfield-Mask: 0x01)                           */
#define R_DTC_DTEVR_DTERRD_Pos             (12UL)         /*!< DTERRD (Bit 12)                                       */
#define R_DTC_DTEVR_DTERRD_Msk             (0x7000UL)     /*!< DTERRD (Bitfield-Mask: 0x07)                          */
#define R_DTC_DTEVR_DTEVSAM_Pos            (8UL)          /*!< DTEVSAM (Bit 8)                                       */
#define R_DTC_DTEVR_DTEVSAM_Msk            (0x100UL)      /*!< DTEVSAM (Bitfield-Mask: 0x01)                         */
#define R_DTC_DTEVR_DTEV_Pos               (0UL)          /*!< DTEV (Bit 0)                                          */
#define R_DTC_DTEVR_DTEV_Msk               (0xffUL)       /*!< DTEV (Bitfield-Mask: 0xff)                            */
/* ========================================================  DTCIBR  ========================================================= */
#define R_DTC_DTCIBR_DTCIBR_Pos            (10UL)         /*!< DTCIBR (Bit 10)                                       */
#define R_DTC_DTCIBR_DTCIBR_Msk            (0xfffffc00UL) /*!< DTCIBR (Bitfield-Mask: 0x3fffff)                      */
/* =========================================================  DTCOR  ========================================================= */
#define R_DTC_DTCOR_SQTFRL_Pos             (0UL)          /*!< SQTFRL (Bit 0)                                        */
#define R_DTC_DTCOR_SQTFRL_Msk             (0x1UL)        /*!< SQTFRL (Bitfield-Mask: 0x01)                          */
/* ========================================================  DTCSQE  ========================================================= */
#define R_DTC_DTCSQE_ESPSEL_Pos            (15UL)         /*!< ESPSEL (Bit 15)                                       */
#define R_DTC_DTCSQE_ESPSEL_Msk            (0x8000UL)     /*!< ESPSEL (Bitfield-Mask: 0x01)                          */
#define R_DTC_DTCSQE_VECN_Pos              (0UL)          /*!< VECN (Bit 0)                                          */
#define R_DTC_DTCSQE_VECN_Msk              (0xffUL)       /*!< VECN (Bitfield-Mask: 0xff)                            */

/** @} */ /* End of group PosMask_peripherals */

#endif /* R_DTC_POSMARK_H */

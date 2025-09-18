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

#ifndef R_DMAC_POSMSK_H
#define R_DMAC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_DMAC0                                          ================ */
/* =========================================================================================================================== */

/* =========================================================  DMSAR  ========================================================= */
#define R_DMAC0_DMSAR_DMSAR_Pos     (0UL)          /*!< DMSAR (Bit 0)                                         */
#define R_DMAC0_DMSAR_DMSAR_Msk     (0xffffffffUL) /*!< DMSAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  DMDAR  ========================================================= */
#define R_DMAC0_DMDAR_DMDAR_Pos     (0UL)          /*!< DMDAR (Bit 0)                                         */
#define R_DMAC0_DMDAR_DMDAR_Msk     (0xffffffffUL) /*!< DMDAR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  DMCRA  ========================================================= */
#define R_DMAC0_DMCRA_DMCRAH_Pos    (16UL)         /*!< DMCRAH (Bit 16)                                       */
#define R_DMAC0_DMCRA_DMCRAH_Msk    (0x3ff0000UL)  /*!< DMCRAH (Bitfield-Mask: 0x3ff)                         */
#define R_DMAC0_DMCRA_DMCRAL_Pos    (0UL)          /*!< DMCRAL (Bit 0)                                        */
#define R_DMAC0_DMCRA_DMCRAL_Msk    (0xffffUL)     /*!< DMCRAL (Bitfield-Mask: 0xffff)                        */
/* =========================================================  DMCRB  ========================================================= */
#define R_DMAC0_DMCRB_DMCRBH_Pos    (16UL)         /*!< DMCRBH (Bit 16)                                       */
#define R_DMAC0_DMCRB_DMCRBH_Msk    (0xffff0000UL) /*!< DMCRBH (Bitfield-Mask: 0xffff)                        */
#define R_DMAC0_DMCRB_DMCRBL_Pos    (0UL)          /*!< DMCRBL (Bit 0)                                        */
#define R_DMAC0_DMCRB_DMCRBL_Msk    (0xffffUL)     /*!< DMCRBL (Bitfield-Mask: 0xffff)                        */
/* =========================================================  DMTMD  ========================================================= */
#define R_DMAC0_DMTMD_MD_Pos        (14UL)         /*!< MD (Bit 14)                                           */
#define R_DMAC0_DMTMD_MD_Msk        (0xc000UL)     /*!< MD (Bitfield-Mask: 0x03)                              */
#define R_DMAC0_DMTMD_DTS_Pos       (12UL)         /*!< DTS (Bit 12)                                          */
#define R_DMAC0_DMTMD_DTS_Msk       (0x3000UL)     /*!< DTS (Bitfield-Mask: 0x03)                             */
#define R_DMAC0_DMTMD_TKP_Pos       (10UL)         /*!< TKP (Bit 10)                                          */
#define R_DMAC0_DMTMD_TKP_Msk       (0x400UL)      /*!< TKP (Bitfield-Mask: 0x01)                             */
#define R_DMAC0_DMTMD_SZ_Pos        (8UL)          /*!< SZ (Bit 8)                                            */
#define R_DMAC0_DMTMD_SZ_Msk        (0x300UL)      /*!< SZ (Bitfield-Mask: 0x03)                              */
#define R_DMAC0_DMTMD_DCTG_Pos      (0UL)          /*!< DCTG (Bit 0)                                          */
#define R_DMAC0_DMTMD_DCTG_Msk      (0x3UL)        /*!< DCTG (Bitfield-Mask: 0x03)                            */
/* =========================================================  DMINT  ========================================================= */
#define R_DMAC0_DMINT_DTIE_Pos      (4UL)          /*!< DTIE (Bit 4)                                          */
#define R_DMAC0_DMINT_DTIE_Msk      (0x10UL)       /*!< DTIE (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMINT_ESIE_Pos      (3UL)          /*!< ESIE (Bit 3)                                          */
#define R_DMAC0_DMINT_ESIE_Msk      (0x8UL)        /*!< ESIE (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMINT_RPTIE_Pos     (2UL)          /*!< RPTIE (Bit 2)                                         */
#define R_DMAC0_DMINT_RPTIE_Msk     (0x4UL)        /*!< RPTIE (Bitfield-Mask: 0x01)                           */
#define R_DMAC0_DMINT_SARIE_Pos     (1UL)          /*!< SARIE (Bit 1)                                         */
#define R_DMAC0_DMINT_SARIE_Msk     (0x2UL)        /*!< SARIE (Bitfield-Mask: 0x01)                           */
#define R_DMAC0_DMINT_DARIE_Pos     (0UL)          /*!< DARIE (Bit 0)                                         */
#define R_DMAC0_DMINT_DARIE_Msk     (0x1UL)        /*!< DARIE (Bitfield-Mask: 0x01)                           */
/* =========================================================  DMAMD  ========================================================= */
#define R_DMAC0_DMAMD_SM_Pos        (14UL)         /*!< SM (Bit 14)                                           */
#define R_DMAC0_DMAMD_SM_Msk        (0xc000UL)     /*!< SM (Bitfield-Mask: 0x03)                              */
#define R_DMAC0_DMAMD_SADR_Pos      (13UL)         /*!< SADR (Bit 13)                                         */
#define R_DMAC0_DMAMD_SADR_Msk      (0x2000UL)     /*!< SADR (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMAMD_SARA_Pos      (8UL)          /*!< SARA (Bit 8)                                          */
#define R_DMAC0_DMAMD_SARA_Msk      (0x1f00UL)     /*!< SARA (Bitfield-Mask: 0x1f)                            */
#define R_DMAC0_DMAMD_DM_Pos        (6UL)          /*!< DM (Bit 6)                                            */
#define R_DMAC0_DMAMD_DM_Msk        (0xc0UL)       /*!< DM (Bitfield-Mask: 0x03)                              */
#define R_DMAC0_DMAMD_DADR_Pos      (5UL)          /*!< DADR (Bit 5)                                          */
#define R_DMAC0_DMAMD_DADR_Msk      (0x20UL)       /*!< DADR (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMAMD_DARA_Pos      (0UL)          /*!< DARA (Bit 0)                                          */
#define R_DMAC0_DMAMD_DARA_Msk      (0x1fUL)       /*!< DARA (Bitfield-Mask: 0x1f)                            */
/* =========================================================  DMOFR  ========================================================= */
#define R_DMAC0_DMOFR_DMOFR_Pos     (0UL)          /*!< DMOFR (Bit 0)                                         */
#define R_DMAC0_DMOFR_DMOFR_Msk     (0xffffffffUL) /*!< DMOFR (Bitfield-Mask: 0xffffffff)                     */
/* =========================================================  DMCNT  ========================================================= */
#define R_DMAC0_DMCNT_DTE_Pos       (0UL)          /*!< DTE (Bit 0)                                           */
#define R_DMAC0_DMCNT_DTE_Msk       (0x1UL)        /*!< DTE (Bitfield-Mask: 0x01)                             */
/* =========================================================  DMREQ  ========================================================= */
#define R_DMAC0_DMREQ_CLRS_Pos      (4UL)          /*!< CLRS (Bit 4)                                          */
#define R_DMAC0_DMREQ_CLRS_Msk      (0x10UL)       /*!< CLRS (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMREQ_SWREQ_Pos     (0UL)          /*!< SWREQ (Bit 0)                                         */
#define R_DMAC0_DMREQ_SWREQ_Msk     (0x1UL)        /*!< SWREQ (Bitfield-Mask: 0x01)                           */
/* =========================================================  DMSTS  ========================================================= */
#define R_DMAC0_DMSTS_ACT_Pos       (7UL)          /*!< ACT (Bit 7)                                           */
#define R_DMAC0_DMSTS_ACT_Msk       (0x80UL)       /*!< ACT (Bitfield-Mask: 0x01)                             */
#define R_DMAC0_DMSTS_DTIF_Pos      (4UL)          /*!< DTIF (Bit 4)                                          */
#define R_DMAC0_DMSTS_DTIF_Msk      (0x10UL)       /*!< DTIF (Bitfield-Mask: 0x01)                            */
#define R_DMAC0_DMSTS_ESIF_Pos      (0UL)          /*!< ESIF (Bit 0)                                          */
#define R_DMAC0_DMSTS_ESIF_Msk      (0x1UL)        /*!< ESIF (Bitfield-Mask: 0x01)                            */
/* =========================================================  DMCSL  ========================================================= */
#define R_DMAC0_DMCSL_DISEL_Pos     (0UL)          /*!< DISEL (Bit 0)                                         */
#define R_DMAC0_DMCSL_DISEL_Msk     (0x1UL)        /*!< DISEL (Bitfield-Mask: 0x01)                           */
/* =========================================================  DMSRR  ========================================================= */
/* =========================================================  DMDRR  ========================================================= */
/* =========================================================  DMSBS  ========================================================= */
#define R_DMAC0_DMSBS_DMSBSH_Pos    (16UL)         /*!< DMSBSH (Bit 16)                                       */
#define R_DMAC0_DMSBS_DMSBSH_Msk    (0xffff0000UL) /*!< DMSBSH (Bitfield-Mask: 0xffff)                        */
#define R_DMAC0_DMSBS_DMSBSL_Pos    (0UL)          /*!< DMSBSL (Bit 0)                                        */
#define R_DMAC0_DMSBS_DMSBSL_Msk    (0xffffUL)     /*!< DMSBSL (Bitfield-Mask: 0xffff)                        */
/* =========================================================  DMDBS  ========================================================= */
#define R_DMAC0_DMDBS_DMDBSH_Pos    (16UL)         /*!< DMDBSH (Bit 16)                                       */
#define R_DMAC0_DMDBS_DMDBSH_Msk    (0xffff0000UL) /*!< DMDBSH (Bitfield-Mask: 0xffff)                        */
#define R_DMAC0_DMDBS_DMDBSL_Pos    (0UL)          /*!< DMDBSL (Bit 0)                                        */
#define R_DMAC0_DMDBS_DMDBSL_Msk    (0xffffUL)     /*!< DMDBSL (Bitfield-Mask: 0xffff)                        */
/* =========================================================  DMBWR  ========================================================= */
#define R_DMAC0_DMBWR_BWE_Pos       (0UL)          /*!< BWE (Bit 0)                                           */
#define R_DMAC0_DMBWR_BWE_Msk       (0x1UL)        /*!< BWE (Bitfield-Mask: 0x01)                             */

#endif /* R_DMAC_POSMSK_H */

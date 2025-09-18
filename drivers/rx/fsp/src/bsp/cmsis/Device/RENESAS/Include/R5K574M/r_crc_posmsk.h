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

#ifndef R_CRC_POSMSK_H
#define R_CRC_POSMSK_H

/* =========================================================================================================================== */
/* ================                                           R_CRC                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  CRCCR  ========================================================= */
#define R_CRC_CRCCR0_DORCLR_Pos          (7UL)          /*!< DORCLR (Bit 7)                                        */
#define R_CRC_CRCCR0_DORCLR_Msk          (0x80UL)       /*!< DORCLR (Bitfield-Mask: 0x01)                          */
#define R_CRC_CRCCR0_LMS_Pos             (6UL)          /*!< LMS (Bit 6)                                           */
#define R_CRC_CRCCR0_LMS_Msk             (0x40UL)       /*!< LMS (Bitfield-Mask: 0x01)                             */
#define R_CRC_CRCCR0_GPS_Pos             (0UL)          /*!< GPS (Bit 0)                                           */
#define R_CRC_CRCCR0_GPS_Msk             (0x7UL)        /*!< GPS (Bitfield-Mask: 0x07)                             */
/* ========================================================  CRCDIR  ========================================================= */
#define R_CRC_CRCDIR_CRCDIR_Pos          (0UL)          /*!< CRCDIR (Bit 0)                                        */
#define R_CRC_CRCDIR_CRCDIR_Msk          (0xffffffffUL) /*!< CRCDIR (Bitfield-Mask: 0xffffffff)                    */
/* =======================================================  CRCDIR_BY  ======================================================= */
#define R_CRC_CRCDIR_BY_CRCDIR_BY_Pos    (0UL)          /*!< CRCDIR_BY (Bit 0)                                     */
#define R_CRC_CRCDIR_BY_CRCDIR_BY_Msk    (0xffUL)       /*!< CRCDIR_BY (Bitfield-Mask: 0xff)                       */
/* ========================================================  CRCDOR  ========================================================= */
#define R_CRC_CRCDOR_CRCDOR_Pos          (0UL)          /*!< CRCDOR (Bit 0)                                        */
#define R_CRC_CRCDOR_CRCDOR_Msk          (0xffffffffUL) /*!< CRCDOR (Bitfield-Mask: 0xffffffff)                    */
/* =======================================================  CRCDOR_HA  ======================================================= */
#define R_CRC_CRCDOR_HA_CRCDOR_HA_Pos    (0UL)          /*!< CRCDOR_HA (Bit 0)                                     */
#define R_CRC_CRCDOR_HA_CRCDOR_HA_Msk    (0xffffUL)     /*!< CRCDOR_HA (Bitfield-Mask: 0xffff)                     */
/* =======================================================  CRCDOR_BY  ======================================================= */
#define R_CRC_CRCDOR_BY_CRCDOR_BY_Pos    (0UL)          /*!< CRCDOR_BY (Bit 0)                                     */
#define R_CRC_CRCDOR_BY_CRCDOR_BY_Msk    (0xffUL)       /*!< CRCDOR_BY (Bitfield-Mask: 0xff)                       */

#endif /* R_CRC_POSMSK_H */

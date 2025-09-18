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

#ifndef R_DAC_B_POSMSK_H
#define R_DAC_B_POSMSK_H

/* =========================================================================================================================== */
/* ================                                          R_DAC_B0                                           ================ */
/* =========================================================================================================================== */

/* =========================================================  DADR  ========================================================== */
/* =========================================================  DACR0  ========================================================= */
#define R_DAC_B0_DACR0_DAOUTDIS_Pos    (30UL)         /*!< DAOUTDIS (Bit 30)                                     */
#define R_DAC_B0_DACR0_DAOUTDIS_Msk    (0xc0000000UL) /*!< DAOUTDIS (Bitfield-Mask: 0x03)                        */
#define R_DAC_B0_DACR0_DAOE_Pos        (28UL)         /*!< DAOE (Bit 28)                                         */
#define R_DAC_B0_DACR0_DAOE_Msk        (0x10000000UL) /*!< DAOE (Bitfield-Mask: 0x01)                            */
#define R_DAC_B0_DACR0_DAE_Pos         (15UL)         /*!< DAE (Bit 15)                                          */
#define R_DAC_B0_DACR0_DAE_Msk         (0x8000UL)     /*!< DAE (Bitfield-Mask: 0x01)                             */
#define R_DAC_B0_DACR0_DACEN_Pos       (0UL)          /*!< DACEN (Bit 0)                                         */
#define R_DAC_B0_DACR0_DACEN_Msk       (0x1UL)        /*!< DACEN (Bitfield-Mask: 0x01)                           */
/* =========================================================  DACR1  ========================================================= */
#define R_DAC_B0_DACR1_DPSEL_Pos       (16UL)         /*!< DPSEL (Bit 16)                                        */
#define R_DAC_B0_DACR1_DPSEL_Msk       (0x10000UL)    /*!< DPSEL (Bitfield-Mask: 0x01)                           */
/* =========================================================  DACR2  ========================================================= */
#define R_DAC_B0_DACR2_DAAMP_Pos       (31UL)         /*!< DAAMP (Bit 31)                                        */
#define R_DAC_B0_DACR2_DAAMP_Msk       (0x80000000UL) /*!< DAAMP (Bitfield-Mask: 0x01)                           */
#define R_DAC_B0_DACR2_OFSSEL_Pos      (8UL)          /*!< OFSSEL (Bit 8)                                        */
#define R_DAC_B0_DACR2_OFSSEL_Msk      (0x100UL)      /*!< OFSSEL (Bitfield-Mask: 0x01)                          */

#endif /* R_DAC_B_POSMSK_H */

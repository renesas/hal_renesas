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

#ifndef R_POEG_REG_H
#define R_POEG_REG_H

/* =========================================================================================================================== */
/* ================                                        R_GPT_POEG0                                        ================ */
/* =========================================================================================================================== */

/**
 * @brief Port Output Enable Module for GPT (R_GPT_POEG0)
 */

typedef struct                          /*!< (@ 0x87412000) R_GPT_POEG0 Structure                                      */
{
    union
    {
        __IOM uint32_t POEGG;           /*!< (@ 0x00000000) POEG Group Setting Register                                */

        struct
        {
            __IOM uint32_t PIDF    : 1; /*!< [0..0] Port Input Detection Flag                                          */
            __IOM uint32_t IOCF    : 1; /*!< [1..1] GPTW Output Stop Request Detection Flag                            */
            __IOM uint32_t OSTPF   : 1; /*!< [2..2] Oscillation Stop Detection Flag                                    */
            __IOM uint32_t SSF     : 1; /*!< [3..3] Software Stop Flag                                                 */
            __IOM uint32_t PIDE    : 1; /*!< [4..4] Port Input Detection Enable                                        */
            __IOM uint32_t IOCE    : 1; /*!< [5..5] GPTW Output Stop Request Enable                                    */
            __IOM uint32_t OSTPE   : 1; /*!< [6..6] Enable Stopping Output on Stopping of Oscillation                  */
            uint32_t               : 9;
            __IM uint32_t ST       : 1; /*!< [16..16] GTETRGn Input Status Flag                                        */
            uint32_t               : 7;
            __IOM uint32_t DERR0ST : 1; /*!< [24..24] DSMIF0 Error Detection Flag                                      */
            __IOM uint32_t DERR1ST : 1; /*!< [25..25] DSMIF1 Error Detection Flag                                      */
            __IOM uint32_t DERR0E  : 1; /*!< [26..26] DSMIF0 Error Detection Enable                                    */
            __IOM uint32_t DERR1E  : 1; /*!< [27..27] DSMIF1 Error Detection Enable                                    */
            __IOM uint32_t INV     : 1; /*!< [28..28] GTETRGn Input Inverting                                          */
            __IOM uint32_t NFEN    : 1; /*!< [29..29] Noise Filter Enable                                              */
            __IOM uint32_t NFCS    : 2; /*!< [31..30] Noise Filter Clock Select                                        */
        } POEGG_b;
    };
    __IM uint32_t RESERVED[11];

    union
    {
        __IOM uint32_t POEGPSR;        /*!< (@ 0x00000030) POEG Group Pin Setting Register                            */

        struct
        {
            __IOM uint32_t PGIF  : 1;  /*!< [0..0] Port Input (POEGn) Detection Flag                                  */
            uint32_t             : 3;
            __IOM uint32_t PGIE  : 1;  /*!< [4..4] Port Input (POEGn) Detection Enable                                */
            uint32_t             : 3;
            __IOM uint32_t INV   : 1;  /*!< [8..8] POEGn Input Inverting                                              */
            uint32_t             : 3;
            __IM uint32_t ST     : 1;  /*!< [12..12] POEGn Input Status                                               */
            uint32_t             : 3;
            __IOM uint32_t NFSCS : 4;  /*!< [19..16] Noise Filter Sampling Count Select                               */
            uint32_t             : 2;
            __IOM uint32_t PIDM  : 2;  /*!< [23..22] Port Input (POEGn) Detection Mode                                 */
            __IOM uint32_t NFCS  : 4;  /*!< [27..24] Digital Filter Clock Select                                      */
            __IOM uint32_t NFEN  : 1;  /*!< [28..28] Noise Filter Enable                                              */
            uint32_t             : 3;
        } POEGPSR_b;
    };
} R_GPT_POEG0_Type;                    /*!< Size = 52 (0x34)                                                          */

#endif /* R_POEG_REG_H */

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

#ifndef R_PDC_REG_H
#define R_PDC_REG_H

/* =========================================================================================================================== */
/* ================                                           R_PDC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Parallel Data Capture Unit (R_PDC)
 */

typedef struct                         /*!< (@ 0x87841000) R_PDC Structure                                            */
{
    union
    {
        __IOM uint32_t PCCR0;          /*!< (@ 0x00000000) PDC Control Register 0                                     */

        struct
        {
            __IOM uint32_t PCKE   : 1; /*!< [0..0] PIXCLK Input Enable                                                */
            __IOM uint32_t VPS    : 1; /*!< [1..1] VSYNC Signal Polarity Select                                       */
            __IOM uint32_t HPS    : 1; /*!< [2..2] HSYNC Signal Polarity Select                                       */
            __OM uint32_t  PRST   : 1; /*!< [3..3] PDC Reset                                                          */
            __IOM uint32_t DFIE   : 1; /*!< [4..4] Receive Data Ready Interrupt Enable                                */
            __IOM uint32_t FEIE   : 1; /*!< [5..5] Frame End Interrupt Enable                                         */
            __IOM uint32_t OVIE   : 1; /*!< [6..6] Overrun Interrupt Enable                                           */
            __IOM uint32_t UDRIE  : 1; /*!< [7..7] Underrun Interrupt Enable                                          */
            __IOM uint32_t VERIE  : 1; /*!< [8..8] Vertical Line Number Setting Error Interrupt Enable                */
            __IOM uint32_t HERIE  : 1; /*!< [9..9] Horizontal Byte Number Setting Error Interrupt Enable              */
            __IOM uint32_t PCKOE  : 1; /*!< [10..10] PCKO Output Enable                                               */
            __IOM uint32_t PCKDIV : 3; /*!< [13..11] PCKO Frequency Division Ratio Select                             */
            __IOM uint32_t EDS    : 1; /*!< [14..14] Endian Select                                                    */
            uint32_t              : 17;
        } PCCR0_b;
    };

    union
    {
        __IOM uint32_t PCCR1;          /*!< (@ 0x00000004) PDC Control Register 1                                     */

        struct
        {
            __IOM uint32_t PCE : 1;    /*!< [0..0] PDC Operation Enable                                               */
            uint32_t           : 31;
        } PCCR1_b;
    };

    union
    {
        __IOM uint32_t PCSR;           /*!< (@ 0x00000008) PDC Status Register                                        */

        struct
        {
            __IM uint32_t  FBSY  : 1;  /*!< [0..0] Frame Busy Flag                                                    */
            __IM uint32_t  FEMPF : 1;  /*!< [1..1] FIFO Empty Flag                                                    */
            __IOM uint32_t FEF   : 1;  /*!< [2..2] Frame End Flag                                                     */
            __IOM uint32_t OVRF  : 1;  /*!< [3..3] Overrun Flag                                                       */
            __IOM uint32_t UDRF  : 1;  /*!< [4..4] Underrun Flag                                                      */
            __IOM uint32_t VERF  : 1;  /*!< [5..5] Vertical Line Number Setting Error Flag                            */
            __IOM uint32_t HERF  : 1;  /*!< [6..6] Horizontal Byte Number Setting Error Flag                          */
            uint32_t             : 25;
        } PCSR_b;
    };

    union
    {
        __IM uint32_t PCMONR;          /*!< (@ 0x0000000C) PDC Pin Monitor Register                                   */

        struct
        {
            __IM uint32_t VSYNC : 1;   /*!< [0..0] VSYNC Signal Status Flag                                           */
            __IM uint32_t HSYNC : 1;   /*!< [1..1] HSYNC Signal Status Flag                                           */
            uint32_t            : 30;
        } PCMONR_b;
    };

    union
    {
        __IM uint32_t PCDR;            /*!< (@ 0x00000010) PDC Receive Data Register                                  */

        struct
        {
            __IM uint32_t PCDR : 32;   /*!< [31..0] The PDC includes a 32-bit-wide, 22-stage FIFO for the
                                        *   storage of captured data. The PCDR register is a 4-byte
                                        *   space to which the FIFO is mapped, and four bytes of data
                                        *   are read from the PCDR register at a time.                                */
        } PCDR_b;
    };

    union
    {
        __IOM uint32_t VCR;            /*!< (@ 0x00000014) Vertical Capture Register                                  */

        struct
        {
            __IOM uint32_t VST : 12;   /*!< [11..0] Vertical Capture Start Line Position                              */
            uint32_t           : 4;
            __IOM uint32_t VSZ : 12;   /*!< [27..16] Vertical Capture Size                                            */
            uint32_t           : 4;
        } VCR_b;
    };

    union
    {
        __IOM uint32_t HCR;            /*!< (@ 0x00000018) Horizontal Capture Register                                */

        struct
        {
            __IOM uint32_t HST : 12;   /*!< [11..0] Horizontal Capture Start Byte Position                            */
            uint32_t           : 4;
            __IOM uint32_t HSZ : 12;   /*!< [27..16] Horizontal Capture Size                                          */
            uint32_t           : 4;
        } HCR_b;
    };
} R_PDC_Type;                          /*!< Size = 28 (0x1c)                                                          */

#endif /* R_PDC_REG_H */

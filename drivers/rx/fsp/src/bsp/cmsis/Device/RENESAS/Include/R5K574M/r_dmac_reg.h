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

#ifndef R_DMAC_REG_H
#define R_DMAC_REG_H

/* =========================================================================================================================== */
/* ================                                          R_DMAC0                                          ================ */
/* =========================================================================================================================== */

/**
 * @brief DMA Controller (R_DMAC0)
 */

typedef struct                          /*!< (@ 0x87007000) R_DMAC0 Structure                                          */
{
    __IOM uint32_t DMSAR;               /*!< (@ 0x00000000) DMA Source Address Register                                */
    __IOM uint32_t DMDAR;               /*!< (@ 0x00000004) DMA Destination Address Register                           */

    union
    {
        __IOM uint32_t DMCRA;           /*!< (@ 0x00000008) DMA Transfer Count Register                                */

        struct
        {
            __IOM uint32_t DMCRAL : 16; /*!< [15..0] Lower bits of transfer count                                      */
            __IOM uint32_t DMCRAH : 10; /*!< [25..16] Upper bits of transfer count                                     */
            uint32_t              : 6;
        } DMCRA_b;
    };

    union
    {
        __IOM uint32_t DMCRB;           /*!< (@ 0x0000000C) DMA Block Transfer Count Register                          */

        struct
        {
            __IOM uint32_t DMCRBL : 16; /*!< [15..0] Functions as a number of block, repeat or repeat-block
                                         *   transfer operations.                                                      */
            __IOM uint32_t DMCRBH : 16; /*!< [31..16] Specifies the number of block, repeat or repeat-block
                                         *   transfer operations.                                                      */
        } DMCRB_b;
    };

    union
    {
        __IOM uint16_t DMTMD;          /*!< (@ 0x00000010) DMA Transfer Mode Register                                 */

        struct
        {
            __IOM uint16_t DCTG : 2;   /*!< [1..0] Transfer Request Source Select                                     */
            uint16_t            : 6;
            __IOM uint16_t SZ   : 2;   /*!< [9..8] Transfer Data Size Select                                          */
            __IOM uint16_t TKP  : 1;   /*!< [10..10] Transfer Keeping                                                 */
            uint16_t            : 1;
            __IOM uint16_t DTS  : 2;   /*!< [13..12] Repeat Area Select                                               */
            __IOM uint16_t MD   : 2;   /*!< [15..14] Transfer Mode Select                                             */
        } DMTMD_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        __IOM uint8_t DMINT;           /*!< (@ 0x00000013) DMA Interrupt Setting Register                             */

        struct
        {
            __IOM uint8_t DARIE : 1;   /*!< [0..0] Destination Address Extended Repeat Area Overflow Interrupt
                                        *   Enable                                                                    */
            __IOM uint8_t SARIE : 1;   /*!< [1..1] Source Address Extended Repeat Area Overflow Interrupt
                                        *   Enable                                                                    */
            __IOM uint8_t RPTIE : 1;   /*!< [2..2] Repeat Size End Interrupt Enable                                   */
            __IOM uint8_t ESIE  : 1;   /*!< [3..3] Transfer Escape End Interrupt Enable                               */
            __IOM uint8_t DTIE  : 1;   /*!< [4..4] Transfer End Interrupt Enable                                      */
            uint8_t             : 3;
        } DMINT_b;
    };

    union
    {
        __IOM uint16_t DMAMD;          /*!< (@ 0x00000014) DMA Address Mode Register                                  */

        struct
        {
            __IOM uint16_t DARA : 5;   /*!< [4..0] Destination Address Extended Repeat Area. Specifies the
                                        *   extended repeat area on the destination address. For details
                                        *   on the settings.                                                          */
            __IOM uint16_t DADR : 1;   /*!< [5..5] Destination Address Update Select After Reload                     */
            __IOM uint16_t DM   : 2;   /*!< [7..6] Destination Address Update Mode                                    */
            __IOM uint16_t SARA : 5;   /*!< [12..8] Source Address Extended Repeat Area. Specifies the extended
                                        *   repeat area on the source address. For details on the settings.           */
            __IOM uint16_t SADR : 1;   /*!< [13..13] Source Address Update Select After Reload                        */
            __IOM uint16_t SM   : 2;   /*!< [15..14] Source Address Update Mode                                       */
        } DMAMD_b;
    };
    __IM uint16_t RESERVED1;

    union
    {
        __IOM uint32_t DMOFR;          /*!< (@ 0x00000018) DMA Offset Register                                        */

        struct
        {
            __IOM uint32_t DMOFR : 32; /*!< [31..0] Specifies the offset when offset addition is selected
                                        *   as the address update mode for transfer source or destination.            */
        } DMOFR_b;
    };

    union
    {
        __IOM uint8_t DMCNT;           /*!< (@ 0x0000001C) DMA Transfer Enable Register                               */

        struct
        {
            __IOM uint8_t DTE : 1;     /*!< [0..0] DMA Transfer Enable                                                */
            uint8_t           : 7;
        } DMCNT_b;
    };

    union
    {
        __IOM uint8_t DMREQ;           /*!< (@ 0x0000001D) DMA Software Start Register                                */

        struct
        {
            __IOM uint8_t SWREQ : 1;   /*!< [0..0] DMA Software Start                                                 */
            uint8_t             : 3;
            __IOM uint8_t CLRS  : 1;   /*!< [4..4] DMA Software Start Bit Auto Clear Select                           */
            uint8_t             : 3;
        } DMREQ_b;
    };

    union
    {
        __IOM uint8_t DMSTS;           /*!< (@ 0x0000001E) DMA Status Register                                        */

        struct
        {
            __IOM uint8_t ESIF : 1;    /*!< [0..0] Transfer Escape End Interrupt Flag                                 */
            uint8_t            : 3;
            __IOM uint8_t DTIF : 1;    /*!< [4..4] Transfer End Interrupt Flag                                        */
            uint8_t            : 2;
            __IM uint8_t ACT   : 1;    /*!< [7..7] DMA Active Flag                                                    */
        } DMSTS_b;
    };

    union
    {
        __IOM uint8_t DMCSL;            /*!< (@ 0x0000001F) DMAC Activation Source Flag Control Register               */

        struct
        {
            __IOM uint8_t DISEL : 1;    /*!< [0..0] Interrupt Select                                                   */
            uint8_t             : 7;
        } DMCSL_b;
    };
    __IOM uint32_t DMSRR;               /*!< (@ 0x00000020) DMA Source Reload Address Register                         */
    __IOM uint32_t DMDRR;               /*!< (@ 0x00000024) DMA Destination Reload Address Register                    */

    union
    {
        __IOM uint32_t DMSBS;           /*!< (@ 0x00000028) DMA Source Buffer Size Register                            */

        struct
        {
            __IOM uint32_t DMSBSL : 16; /*!< [15..0] Functions as data transfer counter in repeat block transfer
                                         *   mode.                                                                     */
            __IOM uint32_t DMSBSH : 16; /*!< [31..16] Specifies the repeat-area size in repeat block transfer
                                         *   mode.                                                                     */
        } DMSBS_b;
    };

    union
    {
        __IOM uint32_t DMDBS;           /*!< (@ 0x0000002C) DMA Destination Buffer Size Register                       */

        struct
        {
            __IOM uint32_t DMDBSL : 16; /*!< [15..0] Functions as data transfer counter in repeat block transfer
                                         *   mode.                                                                     */
            __IOM uint32_t DMDBSH : 16; /*!< [31..16] Specifies the repeat-area size in repeat block transfer
                                         *   mode.                                                                     */
        } DMDBS_b;
    };

    union
    {
        __IOM uint8_t DMBWR;           /*!< (@ 0x00000030) DMA Bufferable Write Enable Register                       */

        struct
        {
            __IOM uint8_t BWE : 1;     /*!< [0..0] Bufferable Write Enable                                            */
            uint8_t           : 7;
        } DMBWR_b;
    };
    __IM uint8_t  RESERVED2;
    __IM uint16_t RESERVED3;
} R_DMAC0_Type;                        /*!< Size = 52 (0x34)                                                          */

#endif /* R_DMAC_REG_H */

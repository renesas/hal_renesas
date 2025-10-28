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

#ifndef R_SPI_B_REG_H
#define R_SPI_B_REG_H

/* =========================================================================================================================== */
/* ================                                         R_SPI_B0                                          ================ */
/* =========================================================================================================================== */

/**
 * @brief Serial Peripheral Interface (SPI_B) (R_SPI_B0)
 */

typedef struct                         /*!< (@ 0x000E2800) R_SPI_B0 Structure                                         */
{
    union
    {
        __IOM uint32_t SPDR;           /*!< (@ 0x00000000) RSPI Data Register                                         */

        struct
        {
            union
            {
                __IOM uint16_t SPDR_HA_L;     /*!< (@ 0x00000000) RSPI Data Register                                  */

                struct
                {
                    __IOM uint8_t SPDR_BY_LL; /*!< (@ 0x00000000) RSPI Data Register                                  */
                    __IOM uint8_t SPDR_BY_LH; /*!< (@ 0x00000001) RSPI Data Register                                  */
                };
            };

            union
            {
                __IOM uint16_t SPDR_HA_H;     /*!< (@ 0x00000002) RSPI Data Register                                  */

                struct
                {
                    __IOM uint8_t SPDR_BY_HL; /*!< (@ 0x00000002) RSPI Data Register                                  */
                    __IOM uint8_t SPDR_BY_HH; /*!< (@ 0x00000003) RSPI Data Register                                  */
                };
            };
        };
    };

    union
    {
        __IOM uint8_t SPCKD;           /*!< (@ 0x00000004) RSPI Clock Delay Register                                  */

        struct
        {
            __IOM uint8_t SCKDL : 3;   /*!< [2..0] RSPCK Delay Setting                                                */
            uint8_t             : 5;
        } SPCKD_b;
    };

    union
    {
        __IOM uint8_t SSLND;           /*!< (@ 0x00000005) RSPI Slave Select Negation Delay Register                  */

        struct
        {
            __IOM uint8_t SLNDL : 3;   /*!< [2..0] SSL Negation Delay Setting                                         */
            uint8_t             : 5;
        } SSLND_b;
    };

    union
    {
        __IOM uint8_t SPND;            /*!< (@ 0x00000006) RSPI Next-Access Delay Register                            */

        struct
        {
            __IOM uint8_t SPNDL : 3;   /*!< [2..0] RSPI Next-Access Delay Setting                                     */
            uint8_t             : 5;
        } SPND_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        __IOM uint32_t SPCR;            /*!< (@ 0x00000008) RSPI Control Register                                      */

        struct
        {
            __IOM uint32_t SPE     : 1; /*!< [0..0] RSPI Function Enable                                               */
            uint32_t               : 7;
            __IOM uint32_t SPPE    : 1; /*!< [8..8] Parity Enable                                                      */
            __IOM uint32_t SPOE    : 1; /*!< [9..9] Parity Mode                                                        */
            uint32_t               : 1;
            __IOM uint32_t PTE     : 1; /*!< [11..11] Parity Self-Diagnosis                                            */
            __IOM uint32_t SCKASE  : 1; /*!< [12..12] RSPCK Auto-Stop Function Enable                                  */
            __IOM uint32_t SCKDDIS : 1; /*!< [13..13] RSPCK Delay Between Data Bytes Disable                           */
            __IOM uint32_t MODFEN  : 1; /*!< [14..14] Mode Fault Error Detection Enable                                */
            uint32_t               : 1;
            __IOM uint32_t SPEIE   : 1; /*!< [16..16] Error Interrupt Enable                                           */
            __IOM uint32_t SPRIE   : 1; /*!< [17..17] Receive Buffer Full Interrupt Enable                             */
            __IOM uint32_t SPIIE   : 1; /*!< [18..18] Idle Interrupt Enable                                            */
            __IOM uint32_t RDRIS   : 1; /*!< [19..19] Receive Data Ready Interrupt Select                              */
            __IOM uint32_t SPTIE   : 1; /*!< [20..20] Transmit Buffer Empty Interrupt Enable                           */
            __IOM uint32_t SPCIE   : 1; /*!< [21..21] Communication End Interrupt Enable                               */
            uint32_t               : 2;
            __IOM uint32_t SPMS    : 1; /*!< [24..24] RSPI Mode Select                                                 */
            __IOM uint32_t FRFS    : 1; /*!< [25..25] Frame Format Select                                              */
            uint32_t               : 2;
            __IOM uint32_t CMMD    : 2; /*!< [29..28] Communication Mode Select                                        */
            __IOM uint32_t MSTR    : 1; /*!< [30..30] RSPI Master/Slave Mode Select                                    */
            __IOM uint32_t SYNDIS  : 1; /*!< [31..31] Synchronizer Disable                                             */
        } SPCR_b;
    };

    union
    {
        __IOM uint8_t SPRMCR;          /*!< (@ 0x0000000C) RSPI Receive-Only Mode Control Register                    */

        struct
        {
            __IOM uint8_t RFC  : 5;    /*!< [4..0] Receive Frame Count Select                                         */
            uint8_t            : 1;
            __OM uint8_t TERM  : 1;    /*!< [6..6] Reception Terminate                                                */
            __OM uint8_t START : 1;    /*!< [7..7] Reception Start                                                    */
        } SPRMCR_b;
    };
    __IOM uint8_t SPDRCSR;             /*!< (@ 0x0000000D) RSPI Receive Data Ready Detect Condition Setting
                                        *                Register                                                     */

    union
    {
        __IOM uint8_t SPPCR;           /*!< (@ 0x0000000E) RSPI Pin Control Register                                  */

        struct
        {
            __IOM uint8_t SPLP  : 1;   /*!< [0..0] RSPI Loopback                                                      */
            __IOM uint8_t SPLP2 : 1;   /*!< [1..1] RSPI Loopback 2                                                    */
            uint8_t             : 2;
            __IOM uint8_t MOIFV : 1;   /*!< [4..4] MOSI Idle Fixed Value                                              */
            __IOM uint8_t MOIFE : 1;   /*!< [5..5] MOSI Idle Value Fixing Enable                                      */
            uint8_t             : 2;
        } SPPCR_b;
    };
    __IM uint8_t RESERVED1;

    union
    {
        __IOM uint8_t SSLP;            /*!< (@ 0x00000010) RSPI Slave Select Polarity Register                        */

        struct
        {
            __IOM uint8_t SSL0P : 1;   /*!< [0..0] SSL00 Signal Polarity Setting                                      */
            __IOM uint8_t SSL1P : 1;   /*!< [1..1] SSL01 Signal Polarity Setting                                      */
            __IOM uint8_t SSL2P : 1;   /*!< [2..2] SSL02 Signal Polarity Setting                                      */
            __IOM uint8_t SSL3P : 1;   /*!< [3..3] SSL03 Signal Polarity Setting                                      */
            uint8_t             : 4;
        } SSLP_b;
    };
    __IOM uint8_t SPBR;                /*!< (@ 0x00000011) RSPI Bit Rate Register                                     */
    __IM uint8_t  RESERVED2;

    union
    {
        __IOM uint8_t SPSCR;           /*!< (@ 0x00000013) RSPI Sequence Control Register                             */

        struct
        {
            __IOM uint8_t SPSLN : 3;   /*!< [2..0] RSPI Sequence Length Specification                                 */
            uint8_t             : 5;
        } SPSCR_b;
    };

    union
    {
        __IOM uint32_t SPCMD0;         /*!< (@ 0x00000014) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD0_b;
    };

    union
    {
        __IOM uint32_t SPCMD1;         /*!< (@ 0x00000018) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD1_b;
    };

    union
    {
        __IOM uint32_t SPCMD2;         /*!< (@ 0x0000001C) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD2_b;
    };

    union
    {
        __IOM uint32_t SPCMD3;         /*!< (@ 0x00000020) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD3_b;
    };

    union
    {
        __IOM uint32_t SPCMD4;         /*!< (@ 0x00000024) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD4_b;
    };

    union
    {
        __IOM uint32_t SPCMD5;         /*!< (@ 0x00000028) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD5_b;
    };

    union
    {
        __IOM uint32_t SPCMD6;         /*!< (@ 0x0000002C) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD6_b;
    };

    union
    {
        __IOM uint32_t SPCMD7;         /*!< (@ 0x00000030) RSPI Command Register [0..7]                               */

        struct
        {
            __IOM uint32_t CPHA   : 1; /*!< [0..0] RSPCK Phase Setting                                                */
            __IOM uint32_t CPOL   : 1; /*!< [1..1] RSPCK Polarity Setting                                             */
            __IOM uint32_t BRDV   : 2; /*!< [3..2] Bit Rate Division Setting                                          */
            uint32_t              : 3;
            __IOM uint32_t SSLKP  : 1; /*!< [7..7] SSL Signal Level Keeping                                           */
            uint32_t              : 4;
            __IOM uint32_t LSBF   : 1; /*!< [12..12] RSPI LSB First                                                   */
            __IOM uint32_t SPNDEN : 1; /*!< [13..13] RSPI Next-Access Delay Enable                                    */
            __IOM uint32_t SLNDEN : 1; /*!< [14..14] SSL Negation Delay Setting Enable                                */
            __IOM uint32_t SCKDEN : 1; /*!< [15..15] RSPCK Delay Setting Enable                                       */
            __IOM uint32_t SPB    : 5; /*!< [20..16] RSPI Data Length Setting                                         */
            uint32_t              : 3;
            __IOM uint32_t SSLA   : 3; /*!< [26..24] SSL Signal Assertion Setting                                     */
            uint32_t              : 5;
        } SPCMD7_b;
    };
    __IM uint32_t RESERVED3[3];

    union
    {
        __IOM uint16_t SPDCR;          /*!< (@ 0x00000040) RSPI Data Control Register                                 */

        struct
        {
            __IOM uint16_t BYSW   : 1; /*!< [0..0] Byte Swap                                                          */
            uint16_t              : 2;
            __IOM uint16_t SPRDTD : 1; /*!< [3..3] RSPI Receive/Transmit Data Select                                  */
            __IOM uint16_t DINV   : 1; /*!< [4..4] Transfer Data Invert                                               */
            uint16_t              : 3;
            __IOM uint16_t SPFC   : 2; /*!< [9..8] Number of Frames Setting                                           */
            uint16_t              : 6;
        } SPDCR_b;
    };
    __IM uint16_t RESERVED4;

    union
    {
        __IOM uint16_t SPFCR;          /*!< (@ 0x00000044) RSPI FIFO Control Register                                 */

        struct
        {
            __IOM uint16_t RTRG : 2;   /*!< [1..0] Receive FIFO Threshold Setting                                     */
            uint16_t            : 6;
            __IOM uint16_t TTRG : 2;   /*!< [9..8] Transmit FIFO Threshold Setting                                    */
            uint16_t            : 6;
        } SPFCR_b;
    };
    __IM uint16_t RESERVED5;
    __IM uint32_t RESERVED6[2];
    __IM uint8_t  RESERVED7;

    union
    {
        __IM uint8_t SPSSR;            /*!< (@ 0x00000051) RSPI Sequence Status Register                              */

        struct
        {
            __IM uint8_t SPCP  : 3;    /*!< [2..0] RSPI Command Pointer                                               */
            uint8_t            : 1;
            __IM uint8_t SPECM : 3;    /*!< [6..4] RSPI Error Command                                                 */
            uint8_t            : 1;
        } SPSSR_b;
    };

    union
    {
        __IM uint16_t SPSR;            /*!< (@ 0x00000052) RSPI Status Register                                       */

        struct
        {
            uint16_t            : 7;
            __IM uint16_t RRDYF : 1;   /*!< [7..7] Receive Data Ready Flag                                            */
            __IM uint16_t OVRF  : 1;   /*!< [8..8] Overrun Error Flag                                                 */
            __IM uint16_t IDLNF : 1;   /*!< [9..9] Idle Flag                                                          */
            __IM uint16_t MODF  : 1;   /*!< [10..10] Mode Fault Error Flag                                            */
            __IM uint16_t PERF  : 1;   /*!< [11..11] Parity Error Flag                                                */
            __IM uint16_t UDRF  : 1;   /*!< [12..12] Underrun Error Flag                                              */
            __IM uint16_t SPTEF : 1;   /*!< [13..13] Transmit Buffer Empty Flag                                       */
            __IM uint16_t SPCF  : 1;   /*!< [14..14] Communication End Flag                                           */
            __IM uint16_t SPRF  : 1;   /*!< [15..15] Receive Buffer Full Flag                                         */
        } SPSR_b;
    };
    __IM uint32_t RESERVED8;

    union
    {
        __IM uint8_t SPTFSR;           /*!< (@ 0x00000058) RSPI Transmit FIFO Status Register                         */

        struct
        {
            __IM uint8_t FREE : 3;     /*!< [2..0] Transmit FIFO Free Level                                           */
            uint8_t           : 5;
        } SPTFSR_b;
    };
    __IM uint8_t  RESERVED9;
    __IM uint16_t RESERVED10;

    union
    {
        __IM uint8_t SPRFSR;           /*!< (@ 0x0000005C) RSPI Receive FIFO Status Register                          */

        struct
        {
            __IM uint8_t FILL : 3;     /*!< [2..0] Receive FIFO Fill Level                                            */
            uint8_t           : 5;
        } SPRFSR_b;
    };
    __IM uint8_t  RESERVED11;
    __IM uint16_t RESERVED12;

    union
    {
        __IM uint32_t SPPSR;           /*!< (@ 0x00000060) RSPI Poling Status Regster                                 */

        struct
        {
            __IM uint32_t SPEPS : 1;   /*!< [0..0] SPE Poling Status                                                 */
            uint32_t            : 31;
        } SPPSR_b;
    };
    __IM uint32_t RESERVED13;
    __IM uint16_t RESERVED14;

    union
    {
        __OM uint16_t SPSCLR;          /*!< (@ 0x0000006A) RSPI Status Clear Register                                 */

        struct
        {
            uint16_t             : 7;
            __OM uint16_t RRDYFC : 1;  /*!< [7..7] RSPI Receive Data Ready Flag Clear                                 */
            __OM uint16_t OVRFC  : 1;  /*!< [8..8] Overrun Error Flag Clear                                           */
            uint16_t             : 1;
            __OM uint16_t MODFC  : 1;  /*!< [10..10] Mode Fault Error Flag Clear                                      */
            __OM uint16_t PERFC  : 1;  /*!< [11..11] Parity Error Flag Clear                                          */
            __OM uint16_t UDRFC  : 1;  /*!< [12..12] Underrun Error Flag Clear                                        */
            __OM uint16_t SPTEFC : 1;  /*!< [13..13] Transmit Buffer Empty Flag Clear                                 */
            __OM uint16_t SPCFC  : 1;  /*!< [14..14] Communication End Flag Clear                                     */
            __OM uint16_t SPRFC  : 1;  /*!< [15..15] Receive Buffer Full Flag Clear                                   */
        } SPSCLR_b;
    };

    union
    {
        __OM uint8_t SPFCLR;           /*!< (@ 0x0000006C) RSPI FIFO Clear Register                                   */

        struct
        {
            __OM uint8_t FCLR : 1;     /*!< [0..0] FIFO Clear                                                         */
            uint8_t           : 7;
        } SPFCLR_b;
    };
    __IM uint8_t  RESERVED15;
    __IM uint16_t RESERVED16;
} R_SPI_B0_Type;                       /*!< Size = 112 (0x70) */

#endif /* R_SPI_B_REG_H */

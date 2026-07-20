/*
* Copyright (c) 2020 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_CGC_REG_H
#define R_CGC_REG_H

/* =========================================================================================================================== */
/* ================                                           R_CGC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Clock Generation Circuit (R_CGC)
 */

typedef struct                         /*!< (@ 0x87019000) R_CGC Structure                                            */
{
    __IM uint32_t RESERVED[8];

    union
    {
        __IOM uint32_t SCKDIVCR;       /*!< (@ 0x00000020) System Clock Division Control Register                     */

        struct
        {
            __IOM uint32_t PCKD  : 4;  /*!< [3..0] Peripheral Module Clock D (PCLKD) Select                           */
            __IOM uint32_t PCKC  : 4;  /*!< [7..4] Peripheral Module Clock C (PCLKC) Select                           */
            __IOM uint32_t PCKB  : 4;  /*!< [11..8] Peripheral Module Clock B (PCLKB) Select                          */
            __IOM uint32_t PCKA  : 4;  /*!< [15..12] Peripheral Module Clock A (PCLKA) Select                         */
            __IOM uint32_t BCK   : 4;  /*!< [19..16] External Bus Clock (BCLK) Select                                 */
            __IOM uint32_t PCKE  : 4;  /*!< [23..20] Peripheral Module Clock E (PCLKE) Select                         */
            __IOM uint32_t ICK   : 4;  /*!< [27..24] System Clock (ICLK) Select                                       */
            __IOM uint32_t MRPCK : 4;  /*!< [31..28] MRAM Clock (MRPCLK) Select                                       */
        } SCKDIVCR_b;
    };

    union
    {
        __IOM uint16_t SCKDIVCR2;      /*!< (@ 0x00000024) System Clock Division Control Register 2                   */

        struct
        {
            __IOM uint16_t CPUCK0 : 4; /*!< [3..0] CPU0 Clock (CPUCLK0) Select                                        */
            uint16_t              : 8;
            __IOM uint16_t MRICK  : 4; /*!< [15..12] MRAM bus Clock (MRICLK) Select                                   */
        } SCKDIVCR2_b;
    };

    union
    {
        __IOM uint8_t SCKSCR;          /*!< (@ 0x00000026) System Clock Source Control Register                       */

        struct
        {
            __IOM uint8_t CKSEL : 3;   /*!< [2..0] Clock Source Select                                                */
            uint8_t             : 5;
        } SCKSCR_b;
    };
    __IM uint8_t  RESERVED1;
    __IM uint16_t RESERVED2;

    union
    {
        __IOM uint8_t PLLCR;           /*!< (@ 0x0000002A) PLL Control Register                                       */

        struct
        {
            __IOM uint8_t PLLSTP : 1;  /*!< [0..0] PLL1 Stop Control                                                  */
            uint8_t              : 7;
        } PLLCR_b;
    };
    __IM uint8_t  RESERVED3;
    __IM uint32_t RESERVED4;

    union
    {
        __IOM uint8_t BCKCR;            /*!< (@ 0x00000030) External Bus Clock Control Register                        */

        struct
        {
            __IOM uint8_t BCLKDIV  : 1; /*!< [0..0] BCLK Pin Output Select                                             */
            uint8_t                : 6;
            __IOM uint8_t EBCKASEL : 1; /*!< [7..7] External Bus Asynchronous Select                                   */
        } BCKCR_b;
    };
    __IM uint8_t RESERVED5;

    union
    {
        __IOM uint8_t MOSCCR;          /*!< (@ 0x00000032) Main Clock Oscillator Control Register                     */

        struct
        {
            __IOM uint8_t MOSTP : 1;   /*!< [0..0] Main Clock Oscillator Stop                                         */
            uint8_t             : 7;
        } MOSCCR_b;
    };
    __IM uint8_t  RESERVED6;
    __IM uint16_t RESERVED7;

    union
    {
        __IOM uint8_t HOCOCR;          /*!< (@ 0x00000036) High-Speed On-Chip Oscillator Control Register             */

        struct
        {
            __IOM uint8_t HCSTP : 1;   /*!< [0..0] HOCO Stop                                                          */
            uint8_t             : 7;
        } HOCOCR_b;
    };
    __IM uint8_t RESERVED8;

    union
    {
        __IOM uint8_t MOCOCR;          /*!< (@ 0x00000038) Middle-Speed On-Chip Oscillator Control Register           */

        struct
        {
            __IOM uint8_t MCSTP : 1;   /*!< [0..0] MOCO Stop                                                          */
            uint8_t             : 7;
        } MOCOCR_b;
    };

    union
    {
        __IOM uint8_t FLLCR1;          /*!< (@ 0x00000039) FLL Control Register 1                                     */

        struct
        {
            __IOM uint8_t FLLEN : 1;   /*!< [0..0] FLL Enable                                                         */
            uint8_t             : 7;
        } FLLCR1_b;
    };

    union
    {
        __IOM uint16_t FLLCR2;           /*!< (@ 0x0000003A) FLL Control Register 2                                     */

        struct
        {
            __IOM uint16_t FLLCNTL : 11; /*!< [10..0] FLL Multiplication Control                                        */
            uint16_t               : 5;
        } FLLCR2_b;
    };

    union
    {
        __IM uint8_t OSCSF;            /*!< (@ 0x0000003C) Oscillation Stabilization Flag Register                    */

        struct
        {
            __IM uint8_t HOCOSF : 1;   /*!< [0..0] HOCO Clock Oscillation Stabilization Flag                          */
            uint8_t             : 2;
            __IM uint8_t MOSCSF : 1;   /*!< [3..3] Main Clock Oscillation Stabilization Flag                          */
            uint8_t             : 1;
            __IM uint8_t PLLSF  : 1;   /*!< [5..5] PLL1 Clock Oscillation Stabilization Flag                          */
            __IM uint8_t PLL2SF : 1;   /*!< [6..6] PLL2 Clock Oscillation Stabilization Flag                          */
            uint8_t             : 1;
        } OSCSF_b;
    };
    __IM uint8_t RESERVED9;

    union
    {
        __IOM uint8_t CKOCR;           /*!< (@ 0x0000003E) Clock Out Control Register                                 */

        struct
        {
            __IOM uint8_t CKOSEL : 4;  /*!< [3..0] Clock Out source select                                            */
            __IOM uint8_t CKODIV : 3;  /*!< [6..4] Clock Output Frequency Division Ratio                              */
            __IOM uint8_t CKOEN  : 1;  /*!< [7..7] Clock Out Enable                                                   */
        } CKOCR_b;
    };
    __IM uint8_t RESERVED10;

    union
    {
        __IOM uint8_t OSTDCR;          /*!< (@ 0x00000040) Oscillation Stop Detection Control Register                */

        struct
        {
            __IOM uint8_t OSTDIE : 1;  /*!< [0..0] Oscillation Stop Detection Interrupt Enable                        */
            uint8_t              : 6;
            __IOM uint8_t OSTDE  : 1;  /*!< [7..7] Oscillation Stop Detection Function Enable                         */
        } OSTDCR_b;
    };

    union
    {
        __IOM uint8_t OSTDSR;          /*!< (@ 0x00000041) Oscillation Stop Detection Status Register                 */

        struct
        {
            __IOM uint8_t OSTDF : 1;   /*!< [0..0] Oscillation Stop Detection Flag                                    */
            uint8_t             : 7;
        } OSTDSR_b;
    };
    __IM uint8_t RESERVED11;

    union
    {
        __IOM uint8_t OSCMONR;         /*!< (@ 0x00000043) Oscillator Monitor Register                                */

        struct
        {
            uint8_t              : 1;
            __IM uint8_t MOCOMON : 1;  /*!< [1..1] MOCO operation monitor                                             */
            __IM uint8_t LOCOMON : 1;  /*!< [2..2] LOCO operation monitor                                             */
            uint8_t              : 5;
        } OSCMONR_b;
    };
    __IM uint32_t RESERVED12;
    __IM uint16_t RESERVED13;

    union
    {
        __IOM uint8_t PLL2CR;          /*!< (@ 0x0000004A) PLL2 Control Register                                      */

        struct
        {
            __IOM uint8_t PLL2STP : 1; /*!< [0..0] PLL2 Stop Control                                                  */
            uint8_t               : 7;
        } PLL2CR_b;
    };
    __IM uint8_t RESERVED14;

    union
    {
        __IOM uint16_t PLLCCR2;         /*!< (@ 0x0000004C) PLL Clock Control Register 2                               */

        struct
        {
            __IOM uint16_t PLODIVP : 4; /*!< [3..0] PLL1 Output Frequency Division Ratio Select for output
                                         *   clock P                                                                   */
            __IOM uint16_t PLODIVQ : 4; /*!< [7..4] PLL1 Output Frequency Division Ratio Select for output
                                         *   clock Q                                                                   */
            __IOM uint16_t PLODIVR : 4; /*!< [11..8] PLL1 Output Frequency Division Ratio Select for output
                                         *   clock R                                                                   */
            uint16_t : 4;
        } PLLCCR2_b;
    };

    union
    {
        __IOM uint16_t PLL2CCR2;       /*!< (@ 0x0000004E) PLL2 Clock Control Register 2                              */

        struct
        {
            uint16_t                : 4;
            __IOM uint16_t PL2ODIVQ : 4; /*!< [7..4] PLL2 Output Frequency Division Ratio Select for output
                                          *   clock Q                                                                   */
            __IOM uint16_t PL2ODIVR : 4; /*!< [11..8] PLL2 Output Frequency Division Ratio Select for output
                                          *   clock R                                                                   */
            uint16_t : 4;
        } PLL2CCR2_b;
    };
    __IM uint16_t RESERVED15;

    union
    {
        __IOM uint8_t EBCKOCR;         /*!< (@ 0x00000052) External Bus Clock Output Control Register                 */

        struct
        {
            __IOM uint8_t EBCKOEN : 1; /*!< [0..0] EBCLK Pin Output Control                                           */
            uint8_t               : 7;
        } EBCKOCR_b;
    };

    union
    {
        __IOM uint8_t SDCKOCR;         /*!< (@ 0x00000053) SDRAM Clock Output Control Register                        */

        struct
        {
            __IOM uint8_t SDCKOEN : 1; /*!< [0..0] SDCLK Pin Output Control                                           */
            uint8_t               : 7;
        } SDCKOCR_b;
    };

    union
    {
        __IOM uint8_t SCICKDIVCR;       /*!< (@ 0x00000054) SCI clock Division control register                        */

        struct
        {
            __IOM uint8_t SCICKDIV : 4; /*!< [3..0] SCI clock (SCICLK) Division Select                                 */
            uint8_t                : 4;
        } SCICKDIVCR_b;
    };

    union
    {
        __IOM uint8_t SCICKCR;           /*!< (@ 0x00000055) SCI clock control register                                 */

        struct
        {
            __IOM uint8_t SCICKSEL  : 4; /*!< [3..0] SCI clock (SCICLK) Source Select                                   */
            uint8_t                 : 2;
            __IOM uint8_t SCICKSREQ : 1; /*!< [6..6] SCI clock (SCICLK) Switching Request                               */
            __IM uint8_t  SCICKSRDY : 1; /*!< [7..7] SCI clock (SCICLK) Switching Ready state flag                      */
        } SCICKCR_b;
    };

    union
    {
        __IOM uint8_t SPICKDIVCR;       /*!< (@ 0x00000056) SPI clock Division control register                        */

        struct
        {
            __IOM uint8_t SPICKDIV : 4; /*!< [3..0] SPI clock (SPICLK) Division Select                                 */
            uint8_t                : 4;
        } SPICKDIVCR_b;
    };

    union
    {
        __IOM uint8_t SPICKCR;           /*!< (@ 0x00000057) SPI clock control register                                 */

        struct
        {
            __IOM uint8_t SPICKSEL  : 4; /*!< [3..0] Clock Source Select                                                */
            uint8_t                 : 2;
            __IOM uint8_t SPICKSREQ : 1; /*!< [6..6] Clock Switching Request                                            */
            __IM uint8_t  SPICKSRDY : 1; /*!< [7..7] Clock Switching Ready state flag                                   */
        } SPICKCR_b;
    };
    __IM uint16_t RESERVED16;

    union
    {
        __IOM uint8_t ADCCKDIVCR;       /*!< (@ 0x0000005A) ADC Clock Division Control Register                        */

        struct
        {
            __IOM uint8_t ADCCKDIV : 4; /*!< [3..0] ADC clock (ADCCLK) Division Select                                 */
            uint8_t                : 4;
        } ADCCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t ADCCKCR;           /*!< (@ 0x0000005B) ADC clock control register                                 */

        struct
        {
            __IOM uint8_t ADCCKSEL  : 4; /*!< [3..0] ADC clock (ADCCLK) Source Select                                   */
            uint8_t                 : 2;
            __IOM uint8_t ADCCKSREQ : 1; /*!< [6..6] ADC clock (ADCCLK) Switching Request                               */
            __IM uint8_t  ADCCKSRDY : 1; /*!< [7..7] ADC clock (ADCCLK) Switching Ready State Flag                      */
        } ADCCKCR_b;
    };

    union
    {
        __IOM uint8_t GPTCKDIVCR;       /*!< (@ 0x0000005C) GPT clock Division control register                        */

        struct
        {
            __IOM uint8_t GPTCKDIV : 4; /*!< [3..0] GPT clock (GPTCLK) Division Select                                 */
            uint8_t                : 4;
        } GPTCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t GPTCKCR;           /*!< (@ 0x0000005D) GPT clock control register                                 */

        struct
        {
            __IOM uint8_t GPTCKSEL  : 4; /*!< [3..0] GPT clock (GPTCLK) Source Select                                   */
            uint8_t                 : 2;
            __IOM uint8_t GPTCKSREQ : 1; /*!< [6..6] GPT clock (GPTCLK) Switching Request                               */
            __IM uint8_t  GPTCKSRDY : 1; /*!< [7..7] GPT clock (GPTCLK) Switching Ready state flag                      */
        } GPTCKCR_b;
    };
    __IM uint16_t RESERVED17;
    __IM uint8_t  RESERVED18;

    union
    {
        __IOM uint8_t MOCOUTCR;         /*!< (@ 0x00000061) MOCO User Trimming Control Register                        */

        struct
        {
            __IOM uint8_t MOCOUTRM : 8; /*!< [7..0] MOCO User Trimming                                                 */
        } MOCOUTCR_b;
    };

    union
    {
        __IOM uint8_t HOCOUTCR;         /*!< (@ 0x00000062) HOCO User Trimming Control Register                        */

        struct
        {
            __IOM uint8_t HOCOUTRM : 8; /*!< [7..0] HOCO User Trimming                                                 */
        } HOCOUTCR_b;
    };
    __IM uint8_t  RESERVED19;
    __IM uint32_t RESERVED20[2];

    union
    {
        __IOM uint8_t USBCKDIVCR;       /*!< (@ 0x0000006C) USB clock Division control register                        */

        struct
        {
            __IOM uint8_t USBCKDIV : 4; /*!< [3..0] USB clock (USBCLK) Division Select                                 */
            uint8_t                : 4;
        } USBCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t XSPICKDIVCR;       /*!< (@ 0x0000006D) XSPI Clock Division Control Register                       */

        struct
        {
            __IOM uint8_t XSPICKDIV : 4; /*!< [3..0] XSPI clock (XSPICLK) Division Select                               */
            uint8_t                 : 4;
        } XSPICKDIVCR_b;
    };

    union
    {
        __IOM uint8_t CANFDCKDIVCR;       /*!< (@ 0x0000006E) CANFD Core clock Division control register                 */

        struct
        {
            __IOM uint8_t CANFDCKDIV : 4; /*!< [3..0] CANFD Core clock (CANFDCLK) Division Select                        */
            uint8_t                  : 4;
        } CANFDCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t USB60CKDIVCR;       /*!< (@ 0x0000006F) USB60 clock Division control register                      */

        struct
        {
            __IOM uint8_t USB60CKDIV : 4; /*!< [3..0] USB clock (USB60CLK) Division Select                               */
            uint8_t                  : 4;
        } USB60CKDIVCR_b;
    };

    union
    {
        __IOM uint8_t I3CCKDIVCR;       /*!< (@ 0x00000070) I3C clock Division control register                        */

        struct
        {
            __IOM uint8_t I3CCKDIV : 4; /*!< [3..0] I3C clock (I3CCLK) Division Select                                 */
            uint8_t                : 4;
        } I3CCKDIVCR_b;
    };
    __IM uint8_t  RESERVED21;
    __IM uint16_t RESERVED22;

    union
    {
        __IOM uint8_t USBCKCR;           /*!< (@ 0x00000074) USB clock control register                                 */

        struct
        {
            __IOM uint8_t USBCKSEL  : 4; /*!< [3..0] USB clock (USBCLK) Source Select                                   */
            uint8_t                 : 2;
            __IOM uint8_t USBCKSREQ : 1; /*!< [6..6] USB clock (USBCLK) Switching Request                               */
            __IM uint8_t  USBCKSRDY : 1; /*!< [7..7] USB clock (USBCLK) Switching Ready state flag                      */
        } USBCKCR_b;
    };

    union
    {
        __IOM uint8_t XSPICKCR;           /*!< (@ 0x00000075) XSPI Clock Control Register                                */

        struct
        {
            __IOM uint8_t XSPICKSEL  : 4; /*!< [3..0] XSPI clock (XSPICLK) Source Select                                 */
            uint8_t                  : 2;
            __IOM uint8_t XSPICKSREQ : 1; /*!< [6..6] XSPI clock (XSPICLK) Switching Request                             */
            __IM uint8_t  XSPICKSRDY : 1; /*!< [7..7] XSPI clock (XSPICLK) Switching Ready state flag                    */
        } XSPICKCR_b;
    };

    union
    {
        __IOM uint8_t CANFDCKCR;           /*!< (@ 0x00000076) CANFD Core clock control register                          */

        struct
        {
            __IOM uint8_t CANFDCKSEL  : 4; /*!< [3..0] CANFD Core clock (CANFDCLK) Source Select                          */
            uint8_t                   : 2;
            __IOM uint8_t CANFDCKSREQ : 1; /*!< [6..6] CANFD Core clock (CANFDCLK) Switching Request                      */
            __IM uint8_t  CANFDCKSRDY : 1; /*!< [7..7] CANFD Core clock (CANFDCLK) Switching Ready state flag             */
        } CANFDCKCR_b;
    };

    union
    {
        __IOM uint8_t USB60CKCR;           /*!< (@ 0x00000077) USB60 clock control register                               */

        struct
        {
            __IOM uint8_t USB60CKSEL  : 4; /*!< [3..0] USB clock (USB60CLK) Source Select                                 */
            uint8_t                   : 2;
            __IOM uint8_t USB60CKSREQ : 1; /*!< [6..6] USB clock (USB60CLK) Switching Request                             */
            __IM uint8_t  USB60CKSRDY : 1; /*!< [7..7] USB clock (USB60CLK) Switching Ready state flag                    */
        } USB60CKCR_b;
    };

    union
    {
        __IOM uint8_t I3CCKCR;           /*!< (@ 0x00000078) I3C clock control register                                 */

        struct
        {
            __IOM uint8_t I3CCKSEL  : 4; /*!< [3..0] I3C clock (I3CCLK) Source Select                                   */
            uint8_t                 : 2;
            __IOM uint8_t I3CCKSREQ : 1; /*!< [6..6] I3C clock (I3CCLK) Switching Request                               */
            __IM uint8_t  I3CCKSRDY : 1; /*!< [7..7] I3C clock (I3CCLK) Switching Ready state flag                      */
        } I3CCKCR_b;
    };
    __IM uint8_t  RESERVED23;
    __IM uint16_t RESERVED24;

    union
    {
        __IOM uint8_t MOSCSCR;          /*!< (@ 0x0000007C) Main Clock Oscillator Standby Control Register             */

        struct
        {
            __IOM uint8_t MOSCSOKP : 1; /*!< [0..0] Main Clock Oscillator Standby Oscillation Keep select              */
            uint8_t                : 7;
        } MOSCSCR_b;
    };

    union
    {
        __IOM uint8_t HOCOSCR;          /*!< (@ 0x0000007D) High-Speed On-Chip Oscillator Standby Control
                                         *                  Register                                                   */

        struct
        {
            __IOM uint8_t HOCOSOKP : 1; /*!< [0..0] HOCO Standby Oscillation Keep select                               */
            uint8_t                : 7;
        } HOCOSCR_b;
    };
    __IM uint16_t RESERVED25;
    __IM uint32_t RESERVED26;

    union
    {
        __IOM uint8_t MOCOSCR;          /*!< (@ 0x00000084) Middle-Speed On-Chip Oscillator Standby Control
                                         *                  Register                                                   */

        struct
        {
            __IOM uint8_t MOCOSOKP : 1; /*!< [0..0] MOCO Standby Oscillation Keep select                               */
            uint8_t                : 7;
        } MOCOSCR_b;
    };
    __IM uint8_t  RESERVED27;
    __IM uint16_t RESERVED28;
    __IM uint32_t RESERVED29[6];
    __IM uint16_t RESERVED30;

    union
    {
        __IOM uint8_t MOSCWTCR;        /*!< (@ 0x000000A2) Main Clock Oscillator Wait Control Register                */

        struct
        {
            __IOM uint8_t MSTS : 4;    /*!< [3..0] Main clock oscillator wait time setting                            */
            uint8_t            : 4;
        } MOSCWTCR_b;
    };
    __IM uint8_t  RESERVED31;
    __IM uint32_t RESERVED32[2];

    union
    {
        __IOM uint32_t PLLCCR;           /*!< (@ 0x000000AC) PLL Clock Control Register                                 */

        struct
        {
            __IOM uint32_t PLIDIV   : 2; /*!< [1..0] PLL1 Input Frequency Division Ratio Select                         */
            uint32_t                : 2;
            __IOM uint32_t PLSRCSEL : 1; /*!< [4..4] PLL1 Clock Source Select                                           */
            uint32_t                : 1;
            __IOM uint32_t PLLMULNF : 2; /*!< [7..6] PLL1 Frequency Multiplication Fractional Factor Select             */
            __IOM uint32_t PLLMUL   : 9; /*!< [16..8] PLL1 Frequency Multiplication Factor Select                       */
            uint32_t                : 15;
        } PLLCCR_b;
    };
    __IM uint32_t RESERVED33[6];

    union
    {
        __IOM uint32_t PLL2CCR;           /*!< (@ 0x000000C8) PLL2 Clock Control Register                                */

        struct
        {
            __IOM uint32_t PL2IDIV   : 2; /*!< [1..0] PLL2 Input Frequency Division Ratio Select                         */
            uint32_t                 : 2;
            __IOM uint32_t PL2SRCSEL : 1; /*!< [4..4] PLL2 Clock Source Select                                           */
            uint32_t                 : 1;
            __IOM uint32_t PLL2MULNF : 2; /*!< [7..6] PLL2 Frequency Multiplication Fractional Factor Select             */
            __IOM uint32_t PLL2MUL   : 9; /*!< [16..8] PLL2 Frequency Multiplication Factor Select                       */
            uint32_t                 : 15;
        } PLL2CCR_b;
    };

    union
    {
        __IM uint8_t SYRACCR;          /*!< (@ 0x000000CC) System Register Access Control Register                    */

        struct
        {
            __IM uint8_t BUSY : 1;     /*!< [0..0] Access Ready monitor                                               */
            uint8_t           : 7;
        } SYRACCR_b;
    };
    __IM uint8_t  RESERVED34;
    __IM uint16_t RESERVED35;
    __IM uint32_t RESERVED36;

    union
    {
        __IOM uint8_t BCKADIVCR;         /*!< (@ 0x000000D4) Asynchronous external Bus clock Division control
                                          *                  register                                                   */

        struct
        {
            __IOM uint8_t BCKACKDIV : 4; /*!< [3..0] Asynchronous external bus clock (BCLKA) Division Select            */
            uint8_t                 : 4;
        } BCKADIVCR_b;
    };

    union
    {
        __IOM uint8_t ESWCKDIVCR;       /*!< (@ 0x000000D5) EtherSW clock Division control register                    */

        struct
        {
            __IOM uint8_t ESWCKDIV : 4; /*!< [3..0] EtherSW clock (ESWCLK) Division Select                             */
            uint8_t                : 4;
        } ESWCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t ESWPCKDIVCR;       /*!< (@ 0x000000D6) EtherSW-PHY clock Division control register                */

        struct
        {
            __IOM uint8_t ESWPCKDIV : 4; /*!< [3..0] EtherSW-PHY clock (ESWPHYCLK) Division Select                      */
            uint8_t                 : 4;
        } ESWPCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t ESCCKDIVCR;       /*!< (@ 0x000000D7) EtherCAT clock Division control register                   */

        struct
        {
            __IOM uint8_t ESCCKDIV : 4; /*!< [3..0] EtherCAT clock (ESCCLK) Division Select                            */
            uint8_t                : 4;
        } ESCCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t ETHPCKDIVCR;       /*!< (@ 0x000000D8) Ether-PHY clock Division control register                  */

        struct
        {
            __IOM uint8_t ETHPCKDIV : 4; /*!< [3..0] Ether-PHY clock (ETHPHYCLK) Division Select                        */
            uint8_t                 : 4;
        } ETHPCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t DSMIFCKDIVCR;       /*!< (@ 0x000000D9) DSMIF clock Division control register                      */

        struct
        {
            __IOM uint8_t DSMIFCKDIV : 4; /*!< [3..0] DSMIF clock (DSMIFCLK) Division Select                             */
            uint8_t                  : 4;
        } DSMIFCKDIVCR_b;
    };

    union
    {
        __IOM uint8_t BCKACR;             /*!< (@ 0x000000DA) Asynchronous external bus clock control register           */

        struct
        {
            __IOM uint8_t BCKACKSEL  : 4; /*!< [3..0] Asynchronous externa bus clock (BCLKA) Source Select               */
            uint8_t                  : 2;
            __IOM uint8_t BCKACKSREQ : 1; /*!< [6..6] Asynchronous external bus clock (BCLKA) Switching Request          */
            __IM uint8_t  BCKACKSRDY : 1; /*!< [7..7] Asynchronous external bus clock (BCLKA) Switching Ready
                                           *   state flag                                                                */
        } BCKACR_b;
    };

    union
    {
        __IOM uint8_t ESWCKCR;           /*!< (@ 0x000000DB) EtherSW clock control register                             */

        struct
        {
            __IOM uint8_t ESWCKSEL  : 4; /*!< [3..0] EtherSW clock (ESWCLK) Source Select                               */
            uint8_t                 : 2;
            __IOM uint8_t ESWCKSREQ : 1; /*!< [6..6] EtherSW clock (ESWCLK) Switching Request                           */
            __IM uint8_t  ESWCKSRDY : 1; /*!< [7..7] EtherSW clock (ESWCLK) Switching Ready state flag                  */
        } ESWCKCR_b;
    };

    union
    {
        __IOM uint8_t ESWPCKCR;           /*!< (@ 0x000000DC) EtherSW-PHY clock control register                         */

        struct
        {
            __IOM uint8_t ESWPCKSEL  : 4; /*!< [3..0] EtherSW-PHY clock (ESWPHYCLK) Source Select                        */
            uint8_t                  : 2;
            __IOM uint8_t ESWPCKSREQ : 1; /*!< [6..6] EtherSW-PHY clock (ESWPHYCLK) Switching Request                    */
            __IM uint8_t  ESWPCKSRDY : 1; /*!< [7..7] EtherSW-PHY clock (ESWPHYCLK) Switching Ready state flag           */
        } ESWPCKCR_b;
    };

    union
    {
        __IOM uint8_t ESCCKCR;           /*!< (@ 0x000000DD) EtherCAT clock control register                            */

        struct
        {
            __IOM uint8_t ESCCKSEL  : 4; /*!< [3..0] EtherCAT clock (ESCCLK) Source Select                              */
            uint8_t                 : 2;
            __IOM uint8_t ESCCKSREQ : 1; /*!< [6..6] EtherCAT clock (ESCCLK) Switching Request                          */
            __IM uint8_t  ESCCKSRDY : 1; /*!< [7..7] EtherCAT clock (ESCCLK) Switching Ready state flag                 */
        } ESCCKCR_b;
    };

    union
    {
        __IOM uint8_t ETHPCKCR;           /*!< (@ 0x000000DE) Ether-PHY clock control register                           */

        struct
        {
            __IOM uint8_t ETHPCKSEL  : 4; /*!< [3..0] Ether-PHY clock (ETHPHYCLK) Source Select                          */
            uint8_t                  : 2;
            __IOM uint8_t ETHPCKSREQ : 1; /*!< [6..6] Ether-PHY clock (ETHPHYCLK) Switching Request                      */
            __IM uint8_t  ETHPCKSRDY : 1; /*!< [7..7] Ether-PHY clock (ETHPHYCLK) Switching Ready state flag             */
        } ETHPCKCR_b;
    };

    union
    {
        __IOM uint8_t DSMIFCKCR;           /*!< (@ 0x000000DF) DSMIF clock control register                               */

        struct
        {
            __IOM uint8_t DSMIFCKSEL  : 4; /*!< [3..0] DSMIF clock (DSMIFCLK) Source Select                               */
            uint8_t                   : 2;
            __IOM uint8_t DSMIFCKSREQ : 1; /*!< [6..6] DSMIF clock (DSMIFCLK) Switching Request                           */
            __IM uint8_t  DSMIFCKSRDY : 1; /*!< [7..7] DSMIF clock (DSMIFCLK) Switching Ready state flag                  */
        } DSMIFCKCR_b;
    };
    __IM uint32_t RESERVED37[184];

    union
    {
        __IOM uint32_t CGFSAR;           /*!< (@ 0x000003C0) Clock Generation Function Security Attribute
                                          *                  Register                                                   */

        struct
        {
            __IOM uint32_t NONSEC00 : 1; /*!< [0..0] Non Secure Attribute bit 00                                        */
            uint32_t                : 1;
            __IOM uint32_t NONSEC02 : 1; /*!< [2..2] Non Secure Attribute bit 02                                        */
            __IOM uint32_t NONSEC03 : 1; /*!< [3..3] Non Secure Attribute bit 03                                        */
            __IOM uint32_t NONSEC04 : 1; /*!< [4..4] Non Secure Attribute bit 04                                        */
            __IOM uint32_t NONSEC05 : 1; /*!< [5..5] Non Secure Attribute bit 05                                        */
            __IOM uint32_t NONSEC06 : 1; /*!< [6..6] Non Secure Attribute bit 06                                        */
            __IOM uint32_t NONSEC07 : 1; /*!< [7..7] Non Secure Attribute bit 07                                        */
            __IOM uint32_t NONSEC08 : 1; /*!< [8..8] Non Secure Attribute bit 08                                        */
            __IOM uint32_t NONSEC09 : 1; /*!< [9..9] Non Secure Attribute bit 09                                        */
            __IOM uint32_t NONSEC10 : 1; /*!< [10..10] Non Secure Attribute bit 10                                      */
            __IOM uint32_t NONSEC11 : 1; /*!< [11..11] Non Secure Attribute bit 11                                      */
            __IOM uint32_t NONSEC12 : 1; /*!< [12..12] Non Secure Attribute bit 12                                      */
            __IOM uint32_t NONSEC13 : 1; /*!< [13..13] Non Secure Attribute bit 13                                      */
            __IOM uint32_t NONSEC14 : 1; /*!< [13..13] Non Secure Attribute bit 14                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC16 : 1; /*!< [16..16] Non Secure Attribute bit 16                                      */
            __IOM uint32_t NONSEC17 : 1; /*!< [17..17] Non Secure Attribute bit 17                                      */
            __IOM uint32_t NONSEC18 : 1; /*!< [18..18] Non Secure Attribute bit 18                                      */
            __IOM uint32_t NONSEC19 : 1; /*!< [19..19] Non Secure Attribute bit 19                                      */
            __IOM uint32_t NONSEC20 : 1; /*!< [20..20] Non Secure Attribute bit 20                                      */
            __IOM uint32_t NONSEC21 : 1; /*!< [20..20] Non Secure Attribute bit 20                                      */
            __IOM uint32_t NONSEC22 : 1; /*!< [22..22] Non Secure Attribute bit 22                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC24 : 1; /*!< [24..24] Non Secure Attribute bit 24                                      */
            __IOM uint32_t NONSEC25 : 1; /*!< [25..25] Non Secure Attribute bit 25                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC27 : 1; /*!< [27..27] Non Secure Attribute bit 27                                      */
            __IOM uint32_t NONSEC28 : 1; /*!< [28..28] Non Secure Attribute bit 28                                      */
            __IOM uint32_t NONSEC29 : 1; /*!< [29..29] Non Secure Attribute bit 29                                      */
            uint32_t                : 2;
        } CGFSAR_b;
    };
    __IM uint32_t RESERVED38[15];

    union
    {
        __IOM uint8_t LOCOCR;          /*!< (@ 0x00000400) Low-Speed On-Chip Oscillator Control Register              */

        struct
        {
            __IOM uint8_t LCSTP : 1;   /*!< [0..0] LOCO Stop                                                          */
            uint8_t             : 7;
        } LOCOCR_b;
    };
    __IM uint8_t RESERVED39;

    union
    {
        __IOM uint8_t LOCOUTCR;         /*!< (@ 0x00000402) LOCO User Trimming Control Register                        */

        struct
        {
            __IOM uint8_t LOCOUTRM : 8; /*!< [7..0] LOCO User Trimming                                                 */
        } LOCOUTCR_b;
    };
    __IM uint8_t  RESERVED40;
    __IM uint32_t RESERVED41[403];

    union
    {
        __IOM uint8_t MOMCR;           /*!< (@ 0x00000A50) Main Clock Oscillator Mode Oscillation Control
                                        *                  Register                                                   */

        struct
        {
            uint8_t              : 1;
            __IOM uint8_t MODRV0 : 3;  /*!< [3..1] Main Clock Oscillator Drive Capability 0 Switching                 */
            uint8_t              : 2;
            __IOM uint8_t MOSEL  : 1;  /*!< [6..6] Main Clock Oscillator Switching                                    */
            uint8_t              : 1;
        } MOMCR_b;
    };
    __IM uint8_t  RESERVED42;
    __IM uint16_t RESERVED43;
    __IM uint32_t RESERVED44[107];

    union
    {
        __IOM uint8_t SOSCCR;          /*!< (@ 0x00000C00) Sub-Clock Oscillator Control Register                      */

        struct
        {
            __IOM uint8_t SOSTP : 1;   /*!< [0..0] Sub-Clock Oscillator Stop                                          */
            uint8_t             : 7;
        } SOSCCR_b;
    };

    union
    {
        __IOM uint8_t SOMCR;           /*!< (@ 0x00000C01) Sub Clock Oscillator Mode Control Register                 */

        struct
        {
            __IOM uint8_t SODRV : 2;   /*!< [1..0] Sub Clock Oscillator Drive Capability Switching                    */
            uint8_t             : 4;
            __IOM uint8_t SOSEL : 1;   /*!< [6..6] Sub-Clock Oscillator Switching                                     */
            uint8_t             : 1;
        } SOMCR_b;
    };
    __IM uint16_t RESERVED45;

    union
    {
        __IOM uint8_t SOSTDCR;         /*!< (@ 0x00000C04) Sub-clock Oscillation Stop Detection Control
                                        *                  Register                                                   */

        struct
        {
            __IOM uint8_t SOSTDIE : 1; /*!< [0..0] Sub-clock Oscillation Stop Detection Interrupt Enable              */
            uint8_t               : 6;
            __IOM uint8_t SOSTDE  : 1; /*!< [7..7] Sub-clock Oscillation Stop Detection Function Enable               */
        } SOSTDCR_b;
    };

    union
    {
        __IOM uint8_t SOSTDSR;         /*!< (@ 0x00000C05) Sub-clock Oscillation Stop Detection Status Register       */

        struct
        {
            __IOM uint8_t SOSTDF : 1;  /*!< [0..0] Sub-clock Oscillation Stop Detection Flag                          */
            uint8_t              : 7;
        } SOSTDSR_b;
    };
    __IM uint16_t RESERVED46;
} R_CGC_Type;                          /*!< Size = 3080 (0xc08)                                                       */

#endif /* R_CGC_REG_H */

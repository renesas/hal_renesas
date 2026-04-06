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

#ifndef R_SYSTEM_REG_H
#define R_SYSTEM_REG_H

/* ================                              Device Specific Cluster Section                              ================ */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief removed R_SYSTEM_DPSBKR [DPSBKR] (Deep Standby Backup Register) as 74m has not
 */

/* =========================================================================================================================== */
/* ================                                          R_SYSTEM                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief System Control (R_SYSTEM)
 */

typedef struct                         /*!< (@ 0x87019000) R_SYSTEM Structure                                           */
{
    __IM uint32_t RESERVED;
    __IM uint16_t RESERVED1;

    union
    {
        __IOM uint16_t SYSCR0;         /*!< (@ 0x00000006) System Control Register 0                                  */

        struct
        {
            __IOM uint16_t ROME : 1;   /*!< [0..0] On-chip MRAM Enable                                                */
            uint16_t            : 7;
            __IOM uint16_t KEY  : 8;   /*!< [15..8] SYSCR0 Key Code                                                   */
        } SYSCR0_b;
    };
    __IM uint32_t RESERVED2;

    union
    {
        __IOM uint8_t SBYCR;           /*!< (@ 0x0000000C) Standby Control Register                                   */

        struct
        {
            uint8_t           : 6;
            __IOM uint8_t OPE : 1;     /*!< [6..6] Output Port Enable                                                 */
            uint8_t           : 1;
        } SBYCR_b;
    };
    __IM uint8_t RESERVED3;

    union
    {
        __IOM uint8_t SSCR2;           /*!< (@ 0x0000000E) Software Standby Control Register 2                        */

        struct
        {
            __IM uint8_t SS2RSF : 1;   /*!< [0..0] Software Standby mode 2 regulator status flag                      */
            uint8_t             : 7;
        } SSCR2_b;
    };
    __IM uint8_t  RESERVED4;
    __IM uint32_t RESERVED5;

    union
    {
        __IOM uint8_t VSCR;            /*!< (@ 0x00000014) Voltage Scaling Control Register                           */

        struct
        {
            __IOM uint8_t VSCM   : 3;  /*!< [2..0] Voltage Scaling Control Mode Bit                                   */
            uint8_t              : 1;
            __IM uint8_t VSCMTSF : 1;  /*!< [4..4] Voltage Scaling Control Mode Transition Status Flag                */
            uint8_t              : 3;
        } VSCR_b;
    };
    __IM uint8_t  RESERVED6;
    __IM uint16_t RESERVED7;
    __IM uint32_t RESERVED8[2];

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
    __IM uint8_t  RESERVED9;
    __IM uint16_t RESERVED10;

    union
    {
        __IOM uint8_t PLLCR;           /*!< (@ 0x0000002A) PLL Control Register                                       */

        struct
        {
            __IOM uint8_t PLLSTP : 1;  /*!< [0..0] PLL1 Stop Control                                                  */
            uint8_t              : 7;
        } PLLCR_b;
    };
    __IM uint8_t  RESERVED11;
    __IM uint32_t RESERVED12;

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
    __IM uint8_t RESERVED13;

    union
    {
        __IOM uint8_t MOSCCR;          /*!< (@ 0x00000032) Main Clock Oscillator Control Register                     */

        struct
        {
            __IOM uint8_t MOSTP : 1;   /*!< [0..0] Main Clock Oscillator Stop                                         */
            uint8_t             : 7;
        } MOSCCR_b;
    };
    __IM uint8_t  RESERVED14;
    __IM uint16_t RESERVED15;

    union
    {
        __IOM uint8_t HOCOCR;          /*!< (@ 0x00000036) High-Speed On-Chip Oscillator Control Register             */

        struct
        {
            __IOM uint8_t HCSTP : 1;   /*!< [0..0] HOCO Stop                                                          */
            uint8_t             : 7;
        } HOCOCR_b;
    };
    __IM uint8_t RESERVED16;

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
            __IM uint8_t HOCOSF : 1;   /*!< [0..0] HOCO Clock Oscillation Stabilization FlagNOTE: The HOCOSF
                                        *   bit value after a reset is 1 when the OFS1.HOCOEN bit is
                                        *   0. It is 0 when the OFS1.HOCOEN bit is 1.                                 */
            uint8_t             : 2;
            __IM uint8_t MOSCSF : 1;   /*!< [3..3] Main Clock Oscillation Stabilization Flag                          */
            uint8_t             : 1;
            __IM uint8_t PLLSF  : 1;   /*!< [5..5] PLL1 Clock Oscillation Stabilization Flag                          */
            __IM uint8_t PLL2SF : 1;   /*!< [6..6] PLL2 Clock Oscillation Stabilization Flag                          */
            uint8_t             : 1;
        } OSCSF_b;
    };
    __IM uint8_t RESERVED17;

    union
    {
        __IOM uint8_t CKOCR;           /*!< (@ 0x0000003E) Clock Out Control Register                                 */

        struct
        {
            __IOM uint8_t CKOSEL : 4;  /*!< [3..0] Clock Out source select                                            */
            __IOM uint8_t CKODIV : 3;  /*!< [6..4] Clock out input frequency Division Select                          */
            __IOM uint8_t CKOEN  : 1;  /*!< [7..7] Clock out enable                                                   */
        } CKOCR_b;
    };
    __IM uint8_t RESERVED18;

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
    __IM uint8_t RESERVED19;

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
    __IM uint32_t RESERVED20;
    __IM uint16_t RESERVED21;

    union
    {
        __IOM uint8_t PLL2CR;          /*!< (@ 0x0000004A) PLL2 Control Register                                      */

        struct
        {
            __IOM uint8_t PLL2STP : 1; /*!< [0..0] PLL2 Stop Control                                                  */
            uint8_t               : 7;
        } PLL2CR_b;
    };
    __IM uint8_t RESERVED22;

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
        __IOM uint16_t PLL2CCR2;         /*!< (@ 0x0000004E) PLL2 Clock Control Register 2                              */

        struct
        {
            __IOM uint16_t PL2ODIVP : 4; /*!< [3..0] PLL2 Output Frequency Division Ratio Select for output
                                          *   clock P                                                                   */
            __IOM uint16_t PL2ODIVQ : 4; /*!< [7..4] PLL2 Output Frequency Division Ratio Select for output
                                          *   clock Q                                                                   */
            __IOM uint16_t PL2ODIVR : 4; /*!< [11..8] PLL2 Output Frequency Division Ratio Select for output
                                          *   clock R                                                                   */
            uint16_t : 4;
        } PLL2CCR2_b;
    };
    __IM uint16_t RESERVED23;

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
    __IM uint16_t RESERVED24;

    union
    {
        __IOM uint8_t ADCCKDIVCR;       /*!< (@ 0x0000005A) ADC clock Division control register                        */

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
            __IM uint8_t  ADCCKSRDY : 1; /*!< [7..7] ADC clock (ADCCLK) Switching Ready state flag                      */
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
    __IM uint16_t RESERVED25;
    __IM uint8_t  RESERVED26;

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
    __IM uint8_t  RESERVED27;
    __IM uint32_t RESERVED28[2];

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
        __IOM uint8_t XSPICKDIVCR;       /*!< (@ 0x0000006D) XSPI clock Division control register                  */

        struct
        {
            __IOM uint8_t XSPICKDIV : 4; /*!< [3..0] XSPI clock (XSPICLK) Division Select                          */
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
    __IM uint8_t  RESERVED29;
    __IM uint16_t RESERVED30;

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
        __IOM uint8_t XSPICKCR;           /*!< (@ 0x00000075) XSPI clock control register                           */

        struct
        {
            __IOM uint8_t XSPICKSEL  : 4; /*!< [3..0] XSPI clock (XSPICLK) Source Select                            */
            uint8_t                  : 2;
            __IOM uint8_t XSPICKSREQ : 1; /*!< [6..6] XSPI clock (XSPICLK) Switching Request                        */
            __IM uint8_t  XSPICKSRDY : 1; /*!< [7..7] XSPI clock (XSPICLK) Switching Ready state flag               */
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
            __IOM uint8_t I3CCKREQ  : 1; /*!< [6..6] I3C clock (I3CCLK) Switching Request                               */
            __IM uint8_t  I3CCKSRDY : 1; /*!< [7..7] I3C clock (I3CCLK) Switching Ready state flag                      */
        } I3CCKCR_b;
    };
    __IM uint8_t  RESERVED31;
    __IM uint16_t RESERVED32;

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
    __IM uint16_t RESERVED33;
    __IM uint32_t RESERVED34;

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
    __IM uint8_t  RESERVED35;
    __IM uint16_t RESERVED36;
    __IM uint32_t RESERVED37[6];

    union
    {
        __IOM uint8_t OPCCR;           /*!< (@ 0x000000A0) Operating Power Control Register                           */

        struct
        {
            __IOM uint8_t OPCM   : 2;  /*!< [1..0] Operating Power Control Mode Select                                */
            uint8_t              : 2;
            __IM uint8_t OPCMTSF : 1;  /*!< [4..4] Operating Power Control Mode Transition Status Flag                */
            uint8_t              : 3;
        } OPCCR_b;
    };
    __IM uint8_t RESERVED38;

    union
    {
        __IOM uint8_t MOSCWTCR;        /*!< (@ 0x000000A2) Main Clock Oscillator Wait Control Register                */

        struct
        {
            __IOM uint8_t MSTS : 4;    /*!< [3..0] Main clock oscillator wait time setting                            */
            uint8_t            : 4;
        } MOSCWTCR_b;
    };
    __IM uint8_t  RESERVED39;
    __IM uint32_t RESERVED40[2];

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

    union
    {
        __IOM uint16_t SWRR;           /*!< (@ 0x000000B0) Software Reset Register                                    */

        struct
        {
            __IOM uint16_t SWRR : 16;  /*!< [15..0] Software Reset bit                                                */
        } SWRR_b;
    };
    __IM uint16_t RESERVED41;
    __IM uint32_t RESERVED42[3];

    union
    {
        __IOM uint32_t RSTSR1;         /*!< (@ 0x000000C0) Reset Status Register 1                                    */

        struct
        {
            __IOM uint32_t IWDTRF : 1; /*!< [0..0] Independent Watchdog Timer Reset Detect FlagNOTE: Writable
                                        *   only to clear the flag. Confirm the value is 1 and then
                                        *   write 0.                                                                  */
            __IOM uint32_t WDT0RF : 1; /*!< [1..1] Watchdog Timer0 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            __IOM uint32_t SWRF : 1;   /*!< [2..2] Software Reset Detect FlagNOTE: Writable only to clear
                                        *   the flag. Confirm the value is 1 and then write 0.                        */
            uint32_t             : 2;
            __IOM uint32_t LM0RF : 1;  /*!< [5..5] Local memory 0 error Reset Detect FlagNOTE: Writable
                                        *   only to clear the flag. Confirm the value is 1 and then
                                        *   write 0.                                                                  */
            uint32_t             : 4;
            __IOM uint32_t BUSRF : 1;  /*!< [10..10] Bus error Reset Detect FlagNOTE: Writable only to clear
                                        *   the flag. Confirm the value is 1 and then write 0.                        */
            uint32_t            : 3;
            __IOM uint32_t CMRF : 1;   /*!< [14..14] Common memory error Reset Detect FlagNOTE: Writable
                                        *   only to clear the flag. Confirm the value is 1 and then
                                        *   write 0.                                                                  */
            uint32_t : 17;
        } RSTSR1_b;
    };
    __IM uint32_t RESERVED43;

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
    __IM uint8_t  RESERVED44;
    __IM uint16_t RESERVED45;
    __IM uint32_t RESERVED46;

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
        __IOM uint8_t ETHPCKDIVCR;       /*!< (@ 0x000000D8) EtherCAT-PHY clock Division control register               */

        struct
        {
            __IOM uint8_t ETHPCKDIV : 4; /*!< [3..0] Clock Division Select                                              */
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
        __IOM uint8_t ETHPCKCR;           /*!< (@ 0x000000DE) EtherCAT-PHY clock control register                        */

        struct
        {
            __IOM uint8_t ETHPCKSEL  : 4; /*!< [3..0] Clock Source Select                                                */
            uint8_t                  : 2;
            __IOM uint8_t ETHPCKSREQ : 1; /*!< [6..6] Clock Switching Request                                            */
            __IM uint8_t  ETHPCKSRDY : 1; /*!< [7..7] Clock Switching Ready state flag                                   */
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

    union
    {
        __IOM uint8_t PVD1CR1;         /*!< (@ 0x000000E0) Voltage Monitor 1 Circuit Control Register 1               */

        struct
        {
            __IOM uint8_t IDTSEL : 2;  /*!< [1..0] Voltage Monitor m Interrupt Generation Condition Select            */
            __IOM uint8_t IRQSEL : 1;  /*!< [2..2] Voltage Monitor m Interrupt Type Select                            */
            uint8_t              : 5;
        } PVD1CR1_b;
    };

    union
    {
        __IOM uint8_t PVD1SR;          /*!< (@ 0x000000E1) Voltage Monitor 1 Circuit Status Register                  */

        struct
        {
            __IOM uint8_t DET : 1;     /*!< [0..0] Voltage Monitor Voltage Change Detection Flag NOTE: Only
                                        *   0 can be written to this bit. After writing 0 to this bit,
                                        *   it takes 2 system clock cycles for the bit to be read as
                                        *   0.                                                                        */
            __IOM uint8_t MON : 1;     /*!< [1..1] Voltage Monitor m Interrupt Type Select                            */
            uint8_t           : 6;
        } PVD1SR_b;
    };

    union
    {
        __IOM uint8_t PVD2CR1;         /*!< (@ 0x000000E2) Voltage Monitor 2 Circuit Control Register 1               */

        struct
        {
            __IOM uint8_t IDTSEL : 2;  /*!< [1..0] Voltage Monitor m Interrupt Generation Condition Select            */
            __IOM uint8_t IRQSEL : 1;  /*!< [2..2] Voltage Monitor m Interrupt Type Select                            */
            uint8_t              : 5;
        } PVD2CR1_b;
    };

    union
    {
        __IOM uint8_t PVD2SR;          /*!< (@ 0x000000E3) Voltage Monitor 2 Circuit Status Register                  */

        struct
        {
            __IOM uint8_t DET : 1;     /*!< [0..0] Voltage Monitor Voltage Change Detection Flag NOTE: Only
                                        *   0 can be written to this bit. After writing 0 to this bit,
                                        *   it takes 2 system clock cycles for the bit to be read as
                                        *   0.                                                                        */
            __IOM uint8_t MON : 1;     /*!< [1..1] Voltage Monitor m Interrupt Type Select                            */
            uint8_t           : 6;
        } PVD2SR_b;
    };

    union
    {
        __IOM uint8_t PVD3CR1;         /*!< (@ 0x000000E4) Voltage Monitor 3 Circuit Control Register 1               */

        struct
        {
            __IOM uint8_t IDTSEL : 2;  /*!< [1..0] Voltage Monitor m Interrupt Generation Condition Select            */
            __IOM uint8_t IRQSEL : 1;  /*!< [2..2] Voltage Monitor m Interrupt Type Select                            */
            uint8_t              : 5;
        } PVD3CR1_b;
    };

    union
    {
        __IOM uint8_t PVD3SR;          /*!< (@ 0x000000E5) Voltage Monitor 3 Circuit Status Register                  */

        struct
        {
            __IOM uint8_t DET : 1;     /*!< [0..0] Voltage Monitor Voltage Change Detection Flag NOTE: Only
                                        *   0 can be written to this bit. After writing 0 to this bit,
                                        *   it takes 2 system clock cycles for the bit to be read as
                                        *   0.                                                                        */
            __IOM uint8_t MON : 1;     /*!< [1..1] Voltage Monitor m Interrupt Type Select                            */
            uint8_t           : 6;
        } PVD3SR_b;
    };

    union
    {
        __IOM uint8_t PVD4CR1;         /*!< (@ 0x000000E6) Voltage Monitor 4 Circuit Control Register 1               */

        struct
        {
            __IOM uint8_t IDTSEL : 2;  /*!< [1..0] Voltage Monitor m Interrupt Generation Condition Select            */
            __IOM uint8_t IRQSEL : 1;  /*!< [2..2] Voltage Monitor m Interrupt Type Select                            */
            uint8_t              : 5;
        } PVD4CR1_b;
    };
    __IM uint8_t RESERVED47;

    union
    {
        __IOM uint8_t PVD5CR1;         /*!< (@ 0x000000E8) Voltage Monitor 5 Circuit Control Register 1               */

        struct
        {
            __IOM uint8_t IDTSEL : 2;  /*!< [1..0] Voltage Monitor m Interrupt Generation Condition Select            */
            __IOM uint8_t IRQSEL : 1;  /*!< [2..2] Voltage Monitor m Interrupt Type Select                            */
            uint8_t              : 5;
        } PVD5CR1_b;
    };
    __IM uint8_t  RESERVED48;
    __IM uint16_t RESERVED49;
    __IM uint32_t RESERVED50[5];

    union
    {
        __IOM uint8_t CPUDSCR;         /*!< (@ 0x00000100) Power Gating Shift Control Register                        */

        struct
        {
            __IOM uint8_t PGD0 : 1;    /*!< [0..0] Power Gating Disable for CPU0                                      */
            uint8_t            : 7;
        } CPUDSCR_b;
    };
    __IM uint8_t  RESERVED51;
    __IM uint16_t RESERVED52;
    __IM uint32_t RESERVED53[5];

    union
    {
        __IOM uint8_t PDCTRESWM;       /*!< (@ 0x00000118) General Power Domain 2 Control Register                    */

        struct
        {
            __IOM uint8_t PDDE  : 1;   /*!< [0..0] Power control enable                                               */
            uint8_t             : 5;
            __IM uint8_t PDCSF  : 1;   /*!< [6..6] Power control status flag                                          */
            __IM uint8_t PDPGSF : 1;   /*!< [7..7] Power gating status flag                                           */
        } PDCTRESWM_b;
    };
    __IM uint8_t  RESERVED54;
    __IM uint16_t RESERVED55;
    __IM uint32_t RESERVED56[9];

    union
    {
        __IOM uint16_t PDRAMSCR0;       /*!< (@ 0x00000140) SRAM power domain Standby Control Register 0               */

        struct
        {
            __IOM uint16_t RKEEP0  : 1; /*!< [0..0] RAM Retention                                                      */
            __IOM uint16_t RKEEP1  : 1; /*!< [1..1] RAM Retention                                                      */
            __IOM uint16_t RKEEP2  : 1; /*!< [2..2] RAM Retention                                                      */
            __IOM uint16_t RKEEP3  : 1; /*!< [3..3] RAM Retention                                                      */
            __IOM uint16_t RKEEP4  : 1; /*!< [4..4] RAM Retention                                                      */
            __IOM uint16_t RKEEP5  : 1; /*!< [5..5] RAM Retention                                                      */
            __IOM uint16_t RKEEP6  : 1; /*!< [6..6] RAM Retention                                                      */
            __IOM uint16_t RKEEP7  : 1; /*!< [7..7] RAM Retention                                                      */
            __IOM uint16_t RKEEP8  : 1; /*!< [8..8] RAM Retention                                                      */
            __IOM uint16_t RKEEP9  : 1; /*!< [9..9] RAM Retention                                                      */
            __IOM uint16_t RKEEP10 : 1; /*!< [10..10] RAM Retention                                                    */
            __IOM uint16_t RKEEP11 : 1; /*!< [11..11] RAM Retention                                                    */
            uint16_t               : 4;
        } PDRAMSCR0_b;
    };
    __IM uint8_t RESERVED57;

    union
    {
        __IOM uint8_t PDRAMSCR2;       /*!< (@ 0x00000143) SRAM power domain Standby Control Register 2               */

        struct
        {
            __IOM uint8_t RKEEP0 : 1;  /*!< [0..0] RAM Retention                                                      */
            uint8_t              : 7;
        } PDRAMSCR2_b;
    };
    __IM uint32_t RESERVED58[155];

    union
    {
        __IOM uint16_t VBRSABAR;       /*!< (@ 0x000003B0) VBATT Backup Register Security Attribute Boundary
                                        *                  Address Register                                           */

        struct
        {
            __IOM uint16_t SABA : 16;  /*!< [15..0] Security Attribute Boundary Address Boundary address
                                        *   between Secure region and non-secure                                      */
        } VBRSABAR_b;
    };
    __IM uint16_t RESERVED59;
    __IM uint32_t RESERVED60[3];

    union
    {
        __IOM uint32_t CGFSAR;           /*!< (@ 0x000003C0) Clock Generation Function Security Attribute
                                          *                  Register                                                   */

        struct
        {
            __IOM uint32_t NONSEC00 : 1; /*!< [0..0] Non-secure Attribute bit 0                                         */
            uint32_t                : 1;
            __IOM uint32_t NONSEC02 : 1; /*!< [2..2] Non-secure Attribute bit 2                                         */
            __IOM uint32_t NONSEC03 : 1; /*!< [3..3] Non-secure Attribute bit 3                                         */
            __IOM uint32_t NONSEC04 : 1; /*!< [4..4] Non-secure Attribute bit 4                                         */
            __IOM uint32_t NONSEC05 : 1; /*!< [5..5] Non-secure Attribute bit 5                                         */
            __IOM uint32_t NONSEC06 : 1; /*!< [6..6] Non-secure Attribute bit 6                                         */
            __IOM uint32_t NONSEC07 : 1; /*!< [7..7] Non-secure Attribute bit 7                                         */
            __IOM uint32_t NONSEC08 : 1; /*!< [8..8] Non-secure Attribute bit 8                                         */
            __IOM uint32_t NONSEC09 : 1; /*!< [9..9] Non-secure Attribute bit 9                                         */
            __IOM uint32_t NONSEC10 : 1; /*!< [10..10] Non-secure Attribute bit 10                                      */
            __IOM uint32_t NONSEC11 : 1; /*!< [11..11] Non-secure Attribute bit 11                                      */
            __IOM uint32_t NONSEC12 : 1; /*!< [12..12] Non-secure Attribute bit 12                                      */
            __IOM uint32_t NONSEC13 : 1; /*!< [13..13] Non-secure Attribute bit 13                                      */
            __IOM uint32_t NONSEC14 : 1; /*!< [14..14] Non-secure Attribute bit 14                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC16 : 1; /*!< [16..16] Non-secure Attribute bit 16                                      */
            __IOM uint32_t NONSEC17 : 1; /*!< [17..17] Non-secure Attribute bit 17                                      */
            __IOM uint32_t NONSEC18 : 1; /*!< [18..18] Non-secure Attribute bit 18                                      */
            __IOM uint32_t NONSEC19 : 1; /*!< [19..19] Non-secure Attribute bit 19                                      */
            __IOM uint32_t NONSEC20 : 1; /*!< [20..20] Non-secure Attribute bit 20                                      */
            __IOM uint32_t NONSEC21 : 1; /*!< [21..21] Non-secure Attribute bit 21                                      */
            __IOM uint32_t NONSEC22 : 1; /*!< [22..22] Non-secure Attribute bit 22                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC24 : 1; /*!< [24..24] Non-secure Attribute bit 24                                      */
            __IOM uint32_t NONSEC25 : 1; /*!< [25..25] Non-secure Attribute bit 25                                      */
            uint32_t                : 1;
            __IOM uint32_t NONSEC27 : 1; /*!< [27..27] Non-secure Attribute bit 27                                      */
            __IOM uint32_t NONSEC28 : 1; /*!< [28..28] Non-secure Attribute bit 28                                      */
            __IOM uint32_t NONSEC29 : 1; /*!< [29..29] Non-secure Attribute bit 29                                      */
            uint32_t                : 2;
        } CGFSAR_b;
    };

    union
    {
        __IOM uint32_t RSTSAR;          /*!< (@ 0x000003C4) Reset Security Attribution Register                        */

        struct
        {
            __IOM uint32_t NONSEC0 : 1; /*!< [0..0] Non-secure Attribute bit 0 Target register: Reset Status
                                         *   Register0                                                                 */
            __IOM uint32_t NONSEC1 : 1; /*!< [1..1] Non-secure Attribute bit 1 Target register: Reset Status
                                         *   Register1                                                                 */
            __IOM uint32_t NONSEC2 : 1; /*!< [2..2] Non-secure Attribute bit 2 Target register: Reset Status
                                         *   Register2                                                                 */
            __IOM uint32_t NONSEC3 : 1; /*!< [3..3] Non-secure Attribute bit 3 Target register: Reset Status
                                         *   Register3                                                                 */
            uint32_t               : 1;
            __IOM uint32_t NONSEC5 : 1; /*!< [5..5] Non-secure Attribute bit 5 Target register: Reset Status
                                         *   Register5                                                                 */
            uint32_t : 26;
        } RSTSAR_b;
    };

    union
    {
        __IOM uint32_t LPMSAR;           /*!< (@ 0x000003C8) Low Power Mode Security Attribution Register               */

        struct
        {
            __IOM uint32_t NONSEC0 : 1;  /*!< [0..0] Non-secure Attribute bit 0 Target register: OPCCR, MLSMCR          */
            __IOM uint32_t NONSEC1 : 1;  /*!< [1..1] Non-secure Attribute bit 1 Target register: LPSCR, DPSWCR          */
            __IOM uint32_t NONSEC2 : 1;  /*!< [2..2] Non-secure Attribute bit 2 Target register: SBYCR, SSCR1,
                                          *   SVSCR                                                                     */
            uint32_t               : 1;
            __IOM uint32_t NONSEC4 : 1;  /*!< [4..4] Non-secure Attribute bit 4 Target register: VSCR                   */
            uint32_t               : 3;
            __IOM uint32_t NONSEC8 : 1;  /*!< [8..8] Non-secure Attribute bit 8 Target register: DPSBYCR,
                                          *   DCCR                                                                      */
            uint32_t                : 8;
            __IOM uint32_t NONSEC17 : 1; /*!< [17..17] Non-secure Attribute bit 17 Target register: HOCOLDOCR           */
            __IOM uint32_t NONSEC18 : 1; /*!< [18..18] Non-secure Attribute bit 18 Target register: PLL1LDOCR           */
            __IOM uint32_t NONSEC19 : 1; /*!< [19..19] Non-secure Attribute bit 19 Target register: PLL2LDOCR           */
            uint32_t                : 1;
            __IOM uint32_t NONSEC21 : 1; /*!< [21..21] Non-secure Attribute bit 21 Target register: LVOCR,
                                          *   MWMCR                                                                     */
            uint32_t : 10;
        } LPMSAR_b;
    };

    union
    {
        __IOM uint32_t PVDSAR;          /*!< (@ 0x000003CC) Programable Voltage Detection Security Attribution
                                         *                  Register                                                   */

        struct
        {
            __IOM uint32_t NONSEC0 : 1; /*!< [0..0] Non-secure Attribute bit 0 Target register: registers
                                         *   for PVD1                                                                  */
            __IOM uint32_t NONSEC1 : 1; /*!< [1..1] Non-secure Attribute bit 1 Target register: registers
                                         *   for PVD2                                                                  */
            uint32_t : 30;
        } PVDSAR_b;
    };

    union
    {
        __IOM uint32_t BBFSAR;          /*!< (@ 0x000003D0) Battery Backup Function Security Attribute Register        */

        struct
        {
            __IOM uint32_t NONSEC0 : 1; /*!< [0..0] Non-secure Attribute bit 0 Target register: VBATTMNSELR            */
            __IOM uint32_t NONSEC1 : 1; /*!< [1..1] Non-secure Attribute bit 1 Target register: VBTBER                 */
            __IOM uint32_t NONSEC2 : 1; /*!< [2..2] Non-secure Attribute bit 2 Target register: VBTICTLR,
                                         *   VBTICTLR2, VBTIMONR                                                       */
            __IOM uint32_t NONSEC3 : 1; /*!< [3..3] Non-secure Attribute bit 3 Target register: VBTBPCR1,
                                         *   VBTBPCR2, VBTBPSR                                                         */
            __IOM uint32_t NONSEC4 : 1; /*!< [4..4] Non-secure Attribute bit 4 Target register: VBTADSR,
                                         *   VBTADCR1, VBTADCR2,                                                       */
            __IOM uint32_t NONSEC5 : 1; /*!< [5..5] Non-secure Attribute bit 5 Target register: VBTADCR3               */
            __IOM uint32_t NONSEC6 : 1; /*!< [6..6] Non-secure Attribute bit 6 Target register: VBTNCWCR               */
            uint32_t               : 25;
        } BBFSAR_b;
    };

    union
    {
        __IOM uint16_t MDSAR;          /*!< (@ 0x000003D4) Operating Modes Security Attribution Register              */

        struct
        {
            uint16_t               : 8;
            __IOM uint16_t NONSEC8 : 1; /*!< [8..8] Non-secure Attribute Target register: System Control
                                         *   Register0                                                                 */
            uint16_t : 7;
        } MDSAR_b;
    };
    __IM uint16_t RESERVED61;

    union
    {
        __IOM uint32_t PGCSAR;         /*!< (@ 0x000003D8) Power Gating Control Security Attribution Register         */

        struct
        {
            uint32_t               : 6;
            __IOM uint32_t NONSEC6 : 1; /*!< [6..6] Non-secure Attribute bit 6 Target register: PDCTRGEN2              */
            __IOM uint32_t NONSEC7 : 1; /*!< [7..7] Non-secure Attribute bit 7 Target register: CPUDSCR                */
            uint32_t               : 24;
        } PGCSAR_b;
    };
    __IM uint32_t RESERVED62;

    union
    {
        __IOM uint32_t DPFSAR;          /*!< (@ 0x000003E0) Deep Standby Interrupt Factor Security Attribution
                                         *                  Register                                                   */

        struct
        {
            __IOM uint32_t DPFSA0 : 1;  /*!< [0..0] Deep Standby Interrupt Factor Security Attribute bit
                                         *   0 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:0)Target factor : IRQn-DS Pin (n:0)                                    */
            __IOM uint32_t DPFSA1 : 1;  /*!< [1..1] Deep Standby Interrupt Factor Security Attribute bit
                                         *   1 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:1)Target factor : IRQn-DS Pin (n:1)                                    */
            __IOM uint32_t DPFSA2 : 1;  /*!< [2..2] Deep Standby Interrupt Factor Security Attribute bit
                                         *   2 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:2)Target factor : IRQn-DS Pin (n:2)                                    */
            __IOM uint32_t DPFSA3 : 1;  /*!< [3..3] Deep Standby Interrupt Factor Security Attribute bit
                                         *   3 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:3)Target factor : IRQn-DS Pin (n:3)                                    */
            __IOM uint32_t DPFSA4 : 1;  /*!< [4..4] Deep Standby Interrupt Factor Security Attribute bit
                                         *   4 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:4)Target factor : IRQn-DS Pin (n:4)                                    */
            __IOM uint32_t DPFSA5 : 1;  /*!< [5..5] Deep Standby Interrupt Factor Security Attribute bit
                                         *   5 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:5)Target factor : IRQn-DS Pin (n:5)                                    */
            __IOM uint32_t DPFSA6 : 1;  /*!< [6..6] Deep Standby Interrupt Factor Security Attribute bit
                                         *   6 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:6)Target factor : IRQn-DS Pin (n:6)                                    */
            __IOM uint32_t DPFSA7 : 1;  /*!< [7..7] Deep Standby Interrupt Factor Security Attribute bit
                                         *   7 Target register: DPSIER0.bn, DPSIFR0.bn, DPSIEGR0.bn
                                         *   (n:7)Target factor : IRQn-DS Pin (n:7)                                    */
            __IOM uint32_t DPFSA8 : 1;  /*!< [8..8] Deep Standby Interrupt Factor Security Attribute bit
                                         *   8 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:0)Target factor : IRQn-DS Pin (n:8)                                    */
            __IOM uint32_t DPFSA9 : 1;  /*!< [9..9] Deep Standby Interrupt Factor Security Attribute bit
                                         *   9 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:1)Target factor : IRQn-DS Pin (n:9)                                    */
            __IOM uint32_t DPFSA10 : 1; /*!< [10..10] Deep Standby Interrupt Factor Security Attribute bit
                                         *   10 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:2)Target factor : IRQn-DS Pin (n:10)                                   */
            __IOM uint32_t DPFSA11 : 1; /*!< [11..11] Deep Standby Interrupt Factor Security Attribute bit
                                         *   11 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:3)Target factor : IRQn-DS Pin (n:11)                                   */
            __IOM uint32_t DPFSA12 : 1; /*!< [12..12] Deep Standby Interrupt Factor Security Attribute bit
                                         *   12 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:4)Target factor : IRQn-DS Pin (n:12)                                   */
            __IOM uint32_t DPFSA13 : 1; /*!< [13..13] Deep Standby Interrupt Factor Security Attribute bit
                                         *   13 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:5)Target factor : IRQn-DS Pin (n:13)                                   */
            __IOM uint32_t DPFSA14 : 1; /*!< [14..14] Deep Standby Interrupt Factor Security Attribute bit
                                         *   14 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:6)Target factor : IRQn-DS Pin (n:14)                                   */
            __IOM uint32_t DPFSA15 : 1; /*!< [15..15] Deep Standby Interrupt Factor Security Attribute bit
                                         *   15 Target register: DPSIER1.bn, DPSIFR1.bn, DPSIEGR1.bn
                                         *   (n:7)Target factor : IRQn-DS Pin (n:15)                                   */
            __IOM uint32_t DPFSA16 : 1; /*!< [16..16] Deep Standby Interrupt Factor Security Attribute bit
                                         *   16 Target register: DPSIER2.b0, DPSIFR2.b0, DPSIEGR2.b0Target
                                         *   factor : PVD1                                                             */
            __IOM uint32_t DPFSA17 : 1; /*!< [17..17] Deep Standby Interrupt Factor Security Attribute bit
                                         *   17 Target register: DPSIER2.b1, DPSIFR2.b1, DPSIEGR2.b1Target
                                         *   factor : PVD2                                                             */
            __IOM uint32_t DPFSA18 : 1; /*!< [18..18] Deep Standby Interrupt Factor Security Attribute bit
                                         *   18 Target register: DPSIER2.b2, DPSIFR2.b2Target factor
                                         *   : RTC Interval                                                            */
            __IOM uint32_t DPFSA19 : 1; /*!< [19..19] Deep Standby Interrupt Factor Security Attribute bit
                                         *   19 Target register: DPSIER2.b3, DPSIFR2.b3Target factor
                                         *   : RTC Alarm                                                               */
            __IOM uint32_t DPFSA20 : 1; /*!< [20..20] Deep Standby Interrupt Factor Security Attribute bit
                                         *   20 Target register: DPSIER2.b4, DPSIFR2.b4, DPSIEGR2.b4Target
                                         *   factor : NMI Pin                                                          */
            uint32_t               : 3;
            __IOM uint32_t DPFSA24 : 1; /*!< [24..24] Deep Standby Interrupt Factor Security Attribute bit
                                         *   24 Target register: DPSIER3.b0, DPSIFR3.b0Target factor
                                         *   : USBFS Suspend/Resume                                                    */
            __IOM uint32_t DPFSA25 : 1; /*!< [25..25] Deep Standby Interrupt Factor Security Attribute bit
                                         *   25 Target register: DPSIER3.b1, DPSIFR3.b1Target factor
                                         *   : USBHS Suspend/Resume                                                    */
            uint32_t               : 3;
            __IOM uint32_t DPFSA29 : 1; /*!< [29..29] Deep Standby Interrupt Factor Security Attribute bit
                                         *   29 Target register: DPSIER3.b5, DPSIFR3.b5Target factor
                                         *   : IWDT Overflow                                                           */
            __IOM uint32_t DPFSA30 : 1; /*!< [30..30] Deep Standby Interrupt Factor Security Attribute bit
                                         *   30 Target register: DPSIER3.b6, DPSIFR3.b6Target factor
                                         *   : Sub-clock Oscillation stop detection                                    */
            __IOM uint32_t DFPSA31 : 1; /*!< [31..31] Deep Standby Interrupt Factor Security Attribute bit
                                         *   31 Target register: DPSIER3.b7, DPSIFR3.b7Target factor
                                         *   : Anti-Tamper Detection                                                   */
        } DPFSAR_b;
    };

    union
    {
        __IOM uint32_t RSCSAR;          /*!< (@ 0x000003E4) RAM Standby Control Security Attribution Register          */

        struct
        {
            __IOM uint32_t RSCSA0 : 1;  /*!< [0..0] RAM Standby Control Security Attribute bit 0 Target register:
                                         *   PDRAMSCR0.bn (n:0)                                                        */
            __IOM uint32_t RSCSA1 : 1;  /*!< [1..1] RAM Standby Control Security Attribute bit 1 Target register:
                                         *   PDRAMSCR0.bn (n:1)                                                        */
            __IOM uint32_t RSCSA2 : 1;  /*!< [2..2] RAM Standby Control Security Attribute bit 2 Target register:
                                         *   PDRAMSCR0.bn (n:2)                                                        */
            __IOM uint32_t RSCSA3 : 1;  /*!< [3..3] RAM Standby Control Security Attribute bit 3 Target register:
                                         *   PDRAMSCR0.bn (n:3)                                                        */
            __IOM uint32_t RSCSA4 : 1;  /*!< [4..4] RAM Standby Control Security Attribute bit 4 Target register:
                                         *   PDRAMSCR0.bn (n:4)                                                        */
            __IOM uint32_t RSCSA5 : 1;  /*!< [5..5] RAM Standby Control Security Attribute bit 5 Target register:
                                         *   PDRAMSCR0.bn (n:5)                                                        */
            __IOM uint32_t RSCSA6 : 1;  /*!< [6..6] RAM Standby Control Security Attribute bit 6 Target register:
                                         *   PDRAMSCR0.bn (n:6)                                                        */
            __IOM uint32_t RSCSA7 : 1;  /*!< [7..7] RAM Standby Control Security Attribute bit 7 Target register:
                                         *   PDRAMSCR0.bn (n:7)                                                        */
            __IOM uint32_t RSCSA8 : 1;  /*!< [8..8] RAM Standby Control Security Attribute bit 8 Target register:
                                         *   PDRAMSCR0.bn (n:8)                                                        */
            __IOM uint32_t RSCSA9 : 1;  /*!< [9..9] RAM Standby Control Security Attribute bit 9 Target register:
                                         *   PDRAMSCR0.bn (n:9)                                                        */
            __IOM uint32_t RSCSA10 : 1; /*!< [10..10] RAM Standby Control Security Attribute bit 10 Target
                                         *   register: PDRAMSCR0.bn (n:10)                                             */
            __IOM uint32_t RSCSA11 : 1; /*!< [11..11] RAM Standby Control Security Attribute bit 11 Target
                                         *   register: PDRAMSCR0.bn (n:11)                                             */
            __IOM uint32_t RSCSA12 : 1; /*!< [12..12] RAM Standby Control Security Attribute bit 12 Target
                                         *   register: PDRAMSCR0.bn (n:12)                                             */
            __IOM uint32_t RSCSA13 : 1; /*!< [13..13] RAM Standby Control Security Attribute bit 13 Target
                                         *   register: PDRAMSCR0.bn (n:13)                                             */
            __IOM uint32_t RSCSA14 : 1; /*!< [14..14] RAM Standby Control Security Attribute bit 14 Target
                                         *   register: PDRAMSCR0.bn (n:14)                                             */
            uint32_t               : 9;
            __IOM uint32_t RSCSA24 : 1; /*!< [24..24] RAM Standby Control Security Attribute bit 24 Target
                                         *   register: PDRAMSCR2.bn (n:0)                                              */
            __IOM uint32_t RSCSA25 : 1; /*!< [25..25] RAM Standby Control Security Attribute bit 25 Target
                                         *   register: PDRAMSCR2.bn (n:1)                                              */
            uint32_t : 6;
        } RSCSAR_b;
    };

    union
    {
        __IOM uint32_t DPFSAR1;         /*!< (@ 0x000003E8) Deep Standby Interrupt Factor Security Attribution
                                         *                  Register 1                                                 */

        struct
        {
            __IOM uint32_t DPFSA0 : 1;  /*!< [0..0] Deep Standby Interrupt Factor Security Attribute bit
                                         *   0 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:0)Target factor : IRQm-DS Pin (m:16)                                   */
            __IOM uint32_t DPFSA1 : 1;  /*!< [1..1] Deep Standby Interrupt Factor Security Attribute bit
                                         *   1 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:1)Target factor : IRQm-DS Pin (m:17)                                   */
            __IOM uint32_t DPFSA2 : 1;  /*!< [2..2] Deep Standby Interrupt Factor Security Attribute bit
                                         *   2 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:2)Target factor : IRQm-DS Pin (m:18)                                   */
            __IOM uint32_t DPFSA3 : 1;  /*!< [3..3] Deep Standby Interrupt Factor Security Attribute bit
                                         *   3 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:3)Target factor : IRQm-DS Pin (m:19)                                   */
            __IOM uint32_t DPFSA4 : 1;  /*!< [4..4] Deep Standby Interrupt Factor Security Attribute bit
                                         *   4 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:4)Target factor : IRQm-DS Pin (m:20)                                   */
            __IOM uint32_t DPFSA5 : 1;  /*!< [5..5] Deep Standby Interrupt Factor Security Attribute bit
                                         *   5 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:5)Target factor : IRQm-DS Pin (m:21)                                   */
            __IOM uint32_t DPFSA6 : 1;  /*!< [6..6] Deep Standby Interrupt Factor Security Attribute bit
                                         *   6 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:6)Target factor : IRQm-DS Pin (m:22)                                   */
            __IOM uint32_t DPFSA7 : 1;  /*!< [7..7] Deep Standby Interrupt Factor Security Attribute bit
                                         *   7 Target register: DPSIER4.bn, DPSIFR4.bn, DPSIEGR3.bn
                                         *   (n:7)Target factor : IRQm-DS Pin (m:23)                                   */
            __IOM uint32_t DPFSA8 : 1;  /*!< [8..8] Deep Standby Interrupt Factor Security Attribute bit
                                         *   8 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 0)Target factor : IRQm-DS Pin (m:24)                                  */
            __IOM uint32_t DPFSA9 : 1;  /*!< [9..9] Deep Standby Interrupt Factor Security Attribute bit
                                         *   9 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 1)Target factor : IRQm-DS Pin (m:25)                                  */
            __IOM uint32_t DPFSA10 : 1; /*!< [10..10] Deep Standby Interrupt Factor Security Attribute bit
                                         *   10 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 2)Target factor : IRQm-DS Pin (m:26)                                  */
            __IOM uint32_t DPFSA11 : 1; /*!< [11..11] Deep Standby Interrupt Factor Security Attribute bit
                                         *   11 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 3)Target factor : IRQm-DS Pin (m:27)                                  */
            __IOM uint32_t DPFSA12 : 1; /*!< [12..12] Deep Standby Interrupt Factor Security Attribute bit
                                         *   12 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 4)Target factor : IRQm-DS Pin (m:28)                                  */
            __IOM uint32_t DPFSA13 : 1; /*!< [13..13] Deep Standby Interrupt Factor Security Attribute bit
                                         *   13 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 5)Target factor : IRQm-DS Pin (m:29)                                  */
            __IOM uint32_t DPFSA14 : 1; /*!< [14..14] Deep Standby Interrupt Factor Security Attribute bit
                                         *   14 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 6)Target factor : IRQm-DS Pin (m:30)                                  */
            __IOM uint32_t DPFSA15 : 1; /*!< [15..15] Deep Standby Interrupt Factor Security Attribute bit
                                         *   15 Target register: DPSIER5.bn, DPSIFR5.bn, DPSIEGR4.bn
                                         *   (n: 7)Target factor : IRQm-DS Pin (m:31)                                  */
            uint32_t : 16;
        } DPFSAR1_b;
    };
    __IM uint32_t RESERVED63[3];
    __IM uint16_t RESERVED64;

    union
    {
        __IOM uint16_t PRCR;           /*!< (@ 0x000003FA) Protect Register for Secure Register                       */

        struct
        {
            __IOM uint16_t PRC0 : 1;   /*!< [0..0] Protect Bit0 Enables writing to the registers related
                                        *   to the clock generation circuit.                                          */
            __IOM uint16_t PRC1 : 1;   /*!< [1..1] Protect Bit1 Enables writing to the registers related
                                        *   to the low power modes, the battery backup function, and
                                        *   the software reset register.                                              */
            uint16_t            : 1;
            __IOM uint16_t PRC3 : 1;   /*!< [3..3] Protect Bit3 Enables writing to the registers related
                                        *   to the PVD.                                                               */
            __IOM uint16_t PRC4 : 1;   /*!< [4..4] Protect Bit4 Enables writing to the registers related
                                        *   to the security and privilege setting registers.                          */
            __IOM uint16_t PRC5 : 1;   /*!< [5..5] Protect Bit5 Enables writing to the registers related
                                        *   the reset control.                                                        */
            uint16_t            : 2;
            __OM uint16_t PRKEY : 8;   /*!< [15..8] PRC Key Code                                                      */
        } PRCR_b;
    };
    __IM uint32_t RESERVED65;

    union
    {
        __IOM uint8_t LOCOCR;          /*!< (@ 0x00000400) Low-Speed On-Chip Oscillator Control Register              */

        struct
        {
            __IOM uint8_t LCSTP : 1;   /*!< [0..0] LOCO Stop                                                          */
            uint8_t             : 7;
        } LOCOCR_b;
    };
    __IM uint8_t RESERVED66;

    union
    {
        __IOM uint8_t LOCOUTCR;         /*!< (@ 0x00000402) LOCO User Trimming Control Register                        */

        struct
        {
            __IOM uint8_t LOCOUTRM : 8; /*!< [7..0] LOCO User Trimming                                                 */
        } LOCOUTCR_b;
    };
    __IM uint8_t  RESERVED67;
    __IM uint32_t RESERVED68[383];

    union
    {
        __IOM uint8_t DPSBYCR;           /*!< (@ 0x00000A00) Deep Standby Control Register                              */

        struct
        {
            __IOM uint8_t LDOSSMODE : 2; /*!< [1..0] LDO SSMODESet the time required to recover from the Deep
                                          *   Software Standby mode in LDO mode.                                        */
            __IOM uint8_t DCSSMODE : 2;  /*!< [3..2] DCDC SSMODESet the time required to recover from the
                                          *   Deep Software Standby mode in DCDC mode.                                  */
            uint8_t              : 2;
            __IOM uint8_t IOKEEP : 1;    /*!< [6..6] I/O Port Retention                                                 */
            uint8_t              : 1;
        } DPSBYCR_b;
    };
    __IM uint8_t  RESERVED69;
    __IM uint16_t RESERVED70;
    __IM uint32_t RESERVED71;

    union
    {
        __IOM uint8_t DPSIER0;         /*!< (@ 0x00000A08) Deep Standby Interrupt Enable Register 0                   */

        struct
        {
            __IOM uint8_t DIRQ0E : 1;  /*!< [0..0] IRQ0-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ1E : 1;  /*!< [1..1] IRQ1-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ2E : 1;  /*!< [2..2] IRQ2-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ3E : 1;  /*!< [3..3] IRQ3-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ4E : 1;  /*!< [4..4] IRQ4-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ5E : 1;  /*!< [5..5] IRQ5-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ6E : 1;  /*!< [6..6] IRQ6-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ7E : 1;  /*!< [7..7] IRQ7-DS Pin Enable                                                 */
        } DPSIER0_b;
    };
    __IM uint8_t  RESERVED72;
    __IM uint16_t RESERVED73;

    union
    {
        __IOM uint8_t DPSIER1;         /*!< (@ 0x00000A0C) Deep Standby Interrupt Enable Register 1                   */

        struct
        {
            __IOM uint8_t DIRQ8E  : 1; /*!< [0..0] IRQ8-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ9E  : 1; /*!< [1..1] IRQ9-DS Pin Enable                                                 */
            __IOM uint8_t DIRQ10E : 1; /*!< [2..2] IRQ10-DS Pin Enable                                                */
            __IOM uint8_t DIRQ11E : 1; /*!< [3..3] IRQ11-DS Pin Enable                                                */
            __IOM uint8_t DIRQ12E : 1; /*!< [4..4] IRQ12-DS Pin Enable                                                */
            __IOM uint8_t DIRQ13E : 1; /*!< [5..5] IRQ13-DS Pin Enable                                                */
            __IOM uint8_t DIRQ14E : 1; /*!< [6..6] IRQ14-DS Pin Enable                                                */
            __IOM uint8_t DIRQ15E : 1; /*!< [7..7] IRQ15-DS Pin Enable                                                */
        } DPSIER1_b;
    };
    __IM uint8_t  RESERVED74;
    __IM uint16_t RESERVED75;

    union
    {
        __IOM uint8_t DPSIER2;         /*!< (@ 0x00000A10) Deep Standby Interrupt Enable Register 2                   */

        struct
        {
            __IOM uint8_t DPVD1IE : 1; /*!< [0..0] PVD1 Deep Standby Cancel Signal Enable                             */
            __IOM uint8_t DPVD2IE : 1; /*!< [1..1] PVD2 Deep Standby Cancel Signal Enable                             */
            __IOM uint8_t DRTCIIE : 1; /*!< [2..2] RTC Interval interrupt Deep Standby Cancel Signal Enable           */
            __IOM uint8_t DRTCAIE : 1; /*!< [3..3] RTC Alarm interrupt Deep Standby Cancel Signal Enable              */
            __IOM uint8_t DNMIE   : 1; /*!< [4..4] NMI Pin Deep Standby Cancel Signal Enable                          */
            uint8_t               : 3;
        } DPSIER2_b;
    };
    __IM uint8_t  RESERVED76;
    __IM uint16_t RESERVED77;

    union
    {
        __IOM uint8_t DPSIER3;            /*!< (@ 0x00000A14) Deep Standby Interrupt Enable Register 3                   */

        struct
        {
            __IOM uint8_t DUSBFSIE : 1;   /*!< [0..0] USBFS Suspend/Resume Deep Standby Cancel Signal Enable             */
            __IOM uint8_t DUSBHSIE : 1;   /*!< [1..1] USBHS Suspend/Resume Deep Standby Cancel Signal Enable             */
            uint8_t                : 3;
            __IOM uint8_t DIWDTIE  : 1;   /*!< [5..5] IWDT Overflow Deep Standby Cancel Signal Enable                    */
            __IOM uint8_t DSOSTDIE : 1;   /*!< [6..6] Sub-clock Oscillation stop detection Deep Standby Cancel
                                           *   Signal Enable                                                             */
            __IOM uint8_t DVBATTADIE : 1; /*!< [7..7] VBATT Tamper Detection Deep Standby Cancel Signal Enable           */
        } DPSIER3_b;
    };
    __IM uint8_t  RESERVED78;
    __IM uint16_t RESERVED79;

    union
    {
        __IOM uint8_t DPSIFR0;         /*!< (@ 0x00000A18) Deep Standby Interrupt Flag Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0F : 1;  /*!< [0..0] IRQ0-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ1F : 1;  /*!< [1..1] IRQ1-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ2F : 1;  /*!< [2..2] IRQ2-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ3F : 1;  /*!< [3..3] IRQ3-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ4F : 1;  /*!< [4..4] IRQ4-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ5F : 1;  /*!< [5..5] IRQ5-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ6F : 1;  /*!< [6..6] IRQ6-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ7F : 1;  /*!< [7..7] IRQ7-DS Pin Deep Standby Cancel Flag                               */
        } DPSIFR0_b;
    };
    __IM uint8_t  RESERVED80;
    __IM uint16_t RESERVED81;

    union
    {
        __IOM uint8_t DPSIFR1;         /*!< (@ 0x00000A1C) Deep Standby Interrupt Flag Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8F  : 1; /*!< [0..0] IRQ8-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ9F  : 1; /*!< [1..1] IRQ9-DS Pin Deep Standby Cancel Flag                               */
            __IOM uint8_t DIRQ10F : 1; /*!< [2..2] IRQ10-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ11F : 1; /*!< [3..3] IRQ11-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ12F : 1; /*!< [4..4] IRQ12-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ13F : 1; /*!< [5..5] IRQ13-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ14F : 1; /*!< [6..6] IRQ14-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ15F : 1; /*!< [7..7] IRQ15-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR1_b;
    };
    __IM uint8_t  RESERVED82;
    __IM uint16_t RESERVED83;

    union
    {
        __IOM uint8_t DPSIFR2;         /*!< (@ 0x00000A20) Deep Standby Interrupt Flag Register 2                     */

        struct
        {
            __IOM uint8_t DPVD1IF : 1; /*!< [0..0] PVD1 Deep Standby Cancel Flag                                      */
            __IOM uint8_t DPVD2IF : 1; /*!< [1..1] PVD2 Deep Standby Cancel Flag                                      */
            __IOM uint8_t DRTCIIF : 1; /*!< [2..2] RTC Interval Interrupt Deep Standby Cancel Flag                    */
            __IOM uint8_t DRTCAIF : 1; /*!< [3..3] RTC Alarm Interrupt Deep Standby Cancel Flag                       */
            __IOM uint8_t DNMIF   : 1; /*!< [4..4] NMI Pin Deep Standby Cancel Flag                                   */
            uint8_t               : 3;
        } DPSIFR2_b;
    };
    __IM uint8_t  RESERVED84;
    __IM uint16_t RESERVED85;

    union
    {
        __IOM uint8_t DPSIFR3;            /*!< (@ 0x00000A24) Deep Standby Interrupt Flag Register 3                     */

        struct
        {
            __IOM uint8_t DUSBFSIF : 1;   /*!< [0..0] USBFS Suspend/Resume Deep Standby Cancel Flag                      */
            __IOM uint8_t DUSBHSIF : 1;   /*!< [1..1] USBHS Suspend/Resume Deep Standby Cancel Flag                      */
            uint8_t                : 3;
            __IOM uint8_t DIWDTIF  : 1;   /*!< [5..5] IWDT Overflow Deep Standby Cancel Flag                             */
            __IOM uint8_t DSOSTDIF : 1;   /*!< [6..6] Sub-clock Oscillation stop detection Deep Standby Cancel
                                           *   Flag                                                                      */
            __IOM uint8_t DVBATTADIF : 1; /*!< [7..7] VBATT Tamper Detection Deep Standby Cancel Flag                    */
        } DPSIFR3_b;
    };
    __IM uint8_t  RESERVED86;
    __IM uint16_t RESERVED87;

    union
    {
        __IOM uint8_t DPSIEGR0;        /*!< (@ 0x00000A28) Deep Standby Interrupt Edge Register 0                     */

        struct
        {
            __IOM uint8_t DIRQ0EG : 1; /*!< [0..0] IRQ0-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ1EG : 1; /*!< [1..1] IRQ1-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ2EG : 1; /*!< [2..2] IRQ2-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ3EG : 1; /*!< [3..3] IRQ3-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ4EG : 1; /*!< [4..4] IRQ4-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ5EG : 1; /*!< [5..5] IRQ5-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ6EG : 1; /*!< [6..6] IRQ6-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ7EG : 1; /*!< [7..7] IRQ7-DS Pin Edge Select                                            */
        } DPSIEGR0_b;
    };
    __IM uint8_t  RESERVED88;
    __IM uint16_t RESERVED89;

    union
    {
        __IOM uint8_t DPSIEGR1;         /*!< (@ 0x00000A2C) Deep Standby Interrupt Edge Register 1                     */

        struct
        {
            __IOM uint8_t DIRQ8EG  : 1; /*!< [0..0] IRQ8-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ9EG  : 1; /*!< [1..1] IRQ9-DS Pin Edge Select                                            */
            __IOM uint8_t DIRQ10EG : 1; /*!< [2..2] IRQ10-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ11EG : 1; /*!< [3..3] IRQ11-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ12EG : 1; /*!< [4..4] IRQ12-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ13EG : 1; /*!< [5..5] IRQ13-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ14EG : 1; /*!< [6..6] IRQ14-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ15EG : 1; /*!< [7..7] IRQ15-DS Pin Edge Select                                           */
        } DPSIEGR1_b;
    };
    __IM uint8_t  RESERVED90;
    __IM uint16_t RESERVED91;

    union
    {
        __IOM uint8_t DPSIEGR2;        /*!< (@ 0x00000A30) Deep Standby Interrupt Edge Register 2                     */

        struct
        {
            __IOM uint8_t DPVD1EG : 1; /*!< [0..0] PVD1 Edge Select                                                   */
            __IOM uint8_t DPVD2EG : 1; /*!< [1..1] PVD2 Edge Select                                                   */
            uint8_t               : 2;
            __IOM uint8_t DNMIEG  : 1; /*!< [4..4] NMI Pin Edge Select                                                */
            uint8_t               : 3;
        } DPSIEGR2_b;
    };
    __IM uint8_t  RESERVED92;
    __IM uint16_t RESERVED93;

    union
    {
        __IOM uint8_t DPSIEGR3;         /*!< (@ 0x00000A34) Deep Standby Interrupt Edge Register 3                     */

        struct
        {
            __IOM uint8_t DIRQ16EG : 1; /*!< [0..0] IRQ16-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ17EG : 1; /*!< [1..1] IRQ17-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ18EG : 1; /*!< [2..2] IRQ18-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ19EG : 1; /*!< [3..3] IRQ19-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ20EG : 1; /*!< [4..4] IRQ20-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ21EG : 1; /*!< [5..5] IRQ21-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ22EG : 1; /*!< [6..6] IRQ22-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ23EG : 1; /*!< [7..7] IRQ23-DS Pin Edge Select                                           */
        } DPSIEGR3_b;
    };
    __IM uint8_t  RESERVED94;
    __IM uint16_t RESERVED95;
    __IM uint32_t RESERVED96[2];

    union
    {
        __IOM uint8_t RSTSR0;          /*!< (@ 0x00000A40) Reset Status Register 0                                    */

        struct
        {
            __IOM uint8_t PORF : 1;    /*!< [0..0] Power-On Reset Detect FlagNOTE: Writable only to clear
                                        *   the flag. Confirm the value is 1 and then write 0.                        */
            __IOM uint8_t PVD0RF : 1;  /*!< [1..1] Voltage Monitor 0 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            __IOM uint8_t PVD1RF : 1;  /*!< [2..2] Voltage Monitor 1 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            __IOM uint8_t PVD2RF : 1;  /*!< [3..3] Voltage Monitor 2 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            uint8_t              : 1;
            __IOM uint8_t PVD4RF : 1;  /*!< [5..5] Voltage Monitor 4 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            __IOM uint8_t PVD5RF : 1;  /*!< [6..6] Voltage Monitor 5 Reset Detect FlagNOTE: Writable only
                                        *   to clear the flag. Confirm the value is 1 and then write
                                        *   0.                                                                        */
            __IOM uint8_t DPSRSTF : 1; /*!< [7..7] Deep Software Standby Reset FlagNOTE: Writable only to
                                        *   clear the flag. Confirm the value is 1 and then write 0.                  */
        } RSTSR0_b;
    };
    __IM uint8_t  RESERVED97;
    __IM uint16_t RESERVED98;

    union
    {
        __IOM uint8_t RSTSR2;          /*!< (@ 0x00000A44) Reset Status Register 2                                    */

        struct
        {
            __IOM uint8_t CWSF : 1;    /*!< [0..0] Cold/Warm Start Determination Flag                                 */
            uint8_t            : 7;
        } RSTSR2_b;
    };
    __IM uint8_t  RESERVED99;
    __IM uint16_t RESERVED100;

    union
    {
        __IOM uint8_t RSTSR3;          /*!< (@ 0x00000A48) Reset Status Register 3                                    */

        struct
        {
            __IOM uint8_t CVMRF  : 1;  /*!< [0..0] Core Voltage Monitor Reset Detect Flag                             */
            uint8_t              : 6;
            __IOM uint8_t TEMPRF : 1;  /*!< [7..7] Temperature MonitorReset Detect Flag                               */
        } RSTSR3_b;
    };
    __IM uint8_t  RESERVED101;
    __IM uint16_t RESERVED102;
    __IM uint32_t RESERVED103;

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
    __IM uint8_t  RESERVED104;
    __IM uint16_t RESERVED105;
    __IM uint32_t RESERVED106;

    union
    {
        __IOM uint8_t PVD1CMPCR;       /*!< (@ 0x00000A58) Voltage Monitor 1 Comparator Control Register              */

        struct
        {
            __IOM uint8_t PVDLVL : 5;  /*!< [4..0] Detection Voltage m Level Select                                   */
            uint8_t              : 2;
            __IOM uint8_t PVDE   : 1;  /*!< [7..7] Voltage Detection m Enable                                         */
        } PVD1CMPCR_b;
    };
    __IM uint8_t  RESERVED107;
    __IM uint16_t RESERVED108;

    union
    {
        __IOM uint8_t PVD2CMPCR;       /*!< (@ 0x00000A5C) Voltage Monitor 2 Comparator Control Register              */

        struct
        {
            __IOM uint8_t PVDLVL : 5;  /*!< [4..0] Detection Voltage m Level Select                                   */
            uint8_t              : 2;
            __IOM uint8_t PVDE   : 1;  /*!< [7..7] Voltage Detection m Enable                                         */
        } PVD2CMPCR_b;
    };
    __IM uint8_t  RESERVED109;
    __IM uint16_t RESERVED110;

    union
    {
        __IOM uint8_t PVD3CMPCR;       /*!< (@ 0x00000A60) Voltage Monitor 3 Comparator Control Register              */

        struct
        {
            __IOM uint8_t PVDLVL : 5;  /*!< [4..0] Detection Voltage m Level Select                                   */
            uint8_t              : 2;
            __IOM uint8_t PVDE   : 1;  /*!< [7..7] Voltage Detection m Enable                                         */
        } PVD3CMPCR_b;
    };
    __IM uint8_t  RESERVED111;
    __IM uint16_t RESERVED112;

    union
    {
        __IOM uint8_t PVD4CMPCR;       /*!< (@ 0x00000A64) Voltage Monitor 4 Comparator Control Register              */

        struct
        {
            __IOM uint8_t PVDLVL : 5;  /*!< [4..0] Detection Voltage m Level Select                                   */
            uint8_t              : 2;
            __IOM uint8_t PVDE   : 1;  /*!< [7..7] Voltage Detection m Enable                                         */
        } PVD4CMPCR_b;
    };
    __IM uint8_t  RESERVED113;
    __IM uint16_t RESERVED114;

    union
    {
        __IOM uint8_t PVD5CMPCR;       /*!< (@ 0x00000A68) Voltage Monitor 5 Comparator Control Register              */

        struct
        {
            __IOM uint8_t PVDLVL : 5;  /*!< [4..0] Detection Voltage m Level Select                                   */
            uint8_t              : 2;
            __IOM uint8_t PVDE   : 1;  /*!< [7..7] Voltage Detection m Enable                                         */
        } PVD5CMPCR_b;
    };
    __IM uint8_t  RESERVED115;
    __IM uint16_t RESERVED116;
    __IM uint32_t RESERVED117;

    union
    {
        __IOM uint8_t PVD1CR0;         /*!< (@ 0x00000A70) Voltage Monitor 1 Circuit Control Register 0               */

        struct
        {
            __IOM uint8_t RIE   : 1;   /*!< [0..0] Voltage Monitor m Interrupt/Reset Enable                           */
            __IOM uint8_t DFDIS : 1;   /*!< [1..1] Voltage Monitor m Digital Filter Disable Mode Select               */
            __IOM uint8_t CMPE  : 1;   /*!< [2..2] Voltage Monitor m Circuit Comparison Result Output Enable          */
            uint8_t             : 1;
            __IOM uint8_t FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t RI    : 1;   /*!< [6..6] Voltage Monitor m Circuit Mode Select                              */
            __IOM uint8_t RN    : 1;   /*!< [7..7] Voltage Monitor m Reset Negate Select                              */
        } PVD1CR0_b;
    };
    __IM uint8_t  RESERVED118;
    __IM uint16_t RESERVED119;

    union
    {
        __IOM uint8_t PVD2CR0;         /*!< (@ 0x00000A74) Voltage Monitor 2 Circuit Control Register 0               */

        struct
        {
            __IOM uint8_t RIE   : 1;   /*!< [0..0] Voltage Monitor m Interrupt/Reset Enable                           */
            __IOM uint8_t DFDIS : 1;   /*!< [1..1] Voltage Monitor m Digital Filter Disable Mode Select               */
            __IOM uint8_t CMPE  : 1;   /*!< [2..2] Voltage Monitor m Circuit Comparison Result Output Enable          */
            uint8_t             : 1;
            __IOM uint8_t FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t RI    : 1;   /*!< [6..6] Voltage Monitor m Circuit Mode Select                              */
            __IOM uint8_t RN    : 1;   /*!< [7..7] Voltage Monitor m Reset Negate Select                              */
        } PVD2CR0_b;
    };
    __IM uint8_t  RESERVED120;
    __IM uint16_t RESERVED121;

    union
    {
        __IOM uint8_t PVD3CR0;         /*!< (@ 0x00000A78) Voltage Monitor 3 Circuit Control Register 0               */

        struct
        {
            __IOM uint8_t RIE   : 1;   /*!< [0..0] Voltage Monitor m Interrupt/Reset Enable                           */
            __IOM uint8_t DFDIS : 1;   /*!< [1..1] Voltage Monitor m Digital Filter Disable Mode Select               */
            __IOM uint8_t CMPE  : 1;   /*!< [2..2] Voltage Monitor m Circuit Comparison Result Output Enable          */
            uint8_t             : 1;
            __IOM uint8_t FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t RI    : 1;   /*!< [6..6] Voltage Monitor m Circuit Mode Select                              */
            __IOM uint8_t RN    : 1;   /*!< [7..7] Voltage Monitor m Reset Negate Select                              */
        } PVD3CR0_b;
    };
    __IM uint8_t  RESERVED122;
    __IM uint16_t RESERVED123;

    union
    {
        __IOM uint8_t PVD4CR0;         /*!< (@ 0x00000A7C) Voltage Monitor 4 Circuit Control Register 0               */

        struct
        {
            __IOM uint8_t RIE   : 1;   /*!< [0..0] Voltage Monitor m Interrupt/Reset Enable                           */
            __IOM uint8_t DFDIS : 1;   /*!< [1..1] Voltage Monitor m Digital Filter Disable Mode Select               */
            __IOM uint8_t CMPE  : 1;   /*!< [2..2] Voltage Monitor m Circuit Comparison Result Output Enable          */
            uint8_t             : 1;
            __IOM uint8_t FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t RI    : 1;   /*!< [6..6] Voltage Monitor m Circuit Mode Select                              */
            __IOM uint8_t RN    : 1;   /*!< [7..7] Voltage Monitor m Reset Negate Select                              */
        } PVD4CR0_b;
    };
    __IM uint8_t  RESERVED124;
    __IM uint16_t RESERVED125;

    union
    {
        __IOM uint8_t PVD5CR0;         /*!< (@ 0x00000A80) Voltage Monitor 5 Circuit Control Register 0               */

        struct
        {
            __IOM uint8_t RIE   : 1;   /*!< [0..0] Voltage Monitor Interrupt/Reset Enable                             */
            __IOM uint8_t DFDIS : 1;   /*!< [1..1] Voltage Monitor Digital Filter Disable Mode Select                 */
            __IOM uint8_t CMPE  : 1;   /*!< [2..2] Voltage Monitor Circuit Comparison Result Output Enable            */
            uint8_t             : 1;
            __IOM uint8_t FSAMP : 2;   /*!< [5..4] Sampling Clock Select                                              */
            __IOM uint8_t RI    : 1;   /*!< [6..6] Voltage Monitor Circuit Mode Select                                */
            __IOM uint8_t RN    : 1;   /*!< [7..7] Voltage Monitor Reset Negate Select                                */
        } PVD5CR0_b;
    };
    __IM uint8_t  RESERVED126;
    __IM uint16_t RESERVED127;

    union
    {
        __IOM uint8_t VBATTMNSELR;        /*!< (@ 0x00000A84) Battery Backup Voltage Monitor Function Select
                                           *                  Register                                                   */

        struct
        {
            __IOM uint8_t VBATTMNSEL : 1; /*!< [0..0] VBATT Voltage Monitor Function Select Bit                          */
            uint8_t                  : 7;
        } VBATTMNSELR_b;
    };
    __IM uint8_t  RESERVED128;
    __IM uint16_t RESERVED129;

    union
    {
        __IOM uint8_t VBTBPCR1;         /*!< (@ 0x00000A88) VBATT Battery Power Supply Control Register 1              */

        struct
        {
            __IOM uint8_t BPWSWSTP : 1; /*!< [0..0] Battery Power Supply Switch Stop                                   */
            uint8_t                : 7;
        } VBTBPCR1_b;
    };
    __IM uint8_t  RESERVED130;
    __IM uint16_t RESERVED131;
    __IM uint32_t RESERVED132;

    union
    {
        __IOM uint8_t LPSCR;           /*!< (@ 0x00000A90) Low Power State Control Register                           */

        struct
        {
            __IOM uint8_t LPMD : 4;    /*!< [3..0] Low power mode setting bit                                         */
            uint8_t            : 4;
        } LPSCR_b;
    };
    __IM uint8_t  RESERVED133;
    __IM uint16_t RESERVED134;
    __IM uint32_t RESERVED135;

    union
    {
        __IOM uint8_t SSCR1;           /*!< (@ 0x00000A98) Software Standby Control Register 1                        */

        struct
        {
            __IOM uint8_t SS2FR : 1;   /*!< [0..0] Software Standby 2 Fast Return                                     */
            uint8_t             : 1;
            __IOM uint8_t SS2LP : 2;   /*!< [3..2] Software Standby 2 low power select                                */
            uint8_t             : 4;
        } SSCR1_b;
    };
    __IM uint8_t  RESERVED136;
    __IM uint16_t RESERVED137;

    union
    {
        __IOM uint8_t SVSCR;           /*!< (@ 0x00000A9C) SSTBY Voltage Scaling Control Register                     */

        struct
        {
            __IOM uint8_t SVSCM : 3;   /*!< [2..0] SSTBY Voltage Scaling Control Mode Bit                             */
            uint8_t             : 5;
        } SVSCR_b;
    };
    __IM uint8_t  RESERVED138;
    __IM uint16_t RESERVED139;
    __IM uint32_t RESERVED140[4];

    union
    {
        __IOM uint8_t LVOCR;           /*!< (@ 0x00000AB0) Low Power State Control Register                           */

        struct
        {
            __IOM uint8_t LVO0E : 1;   /*!< [0..0] Low Voltage Operation 0 Enable                                     */
            uint8_t             : 7;
        } LVOCR_b;
    };
    __IM uint8_t  RESERVED141;
    __IM uint16_t RESERVED142;

    union
    {
        __IOM uint8_t MWMCR;           /*!< (@ 0x00000AB4) MRAM-OTP Write Mode Control Register                       */

        struct
        {
            __IOM uint8_t MWM : 2;     /*!< [1..0] MRAM-OTP Write Mode [1:0]                                          */
            uint8_t           : 6;
        } MWMCR_b;
    };
    __IM uint8_t  RESERVED143;
    __IM uint16_t RESERVED144;

    union
    {
        __IOM uint8_t DCCR;             /*!< (@ 0x00000AB8) DSTBY CVMR Control Register                                */

        struct
        {
            __IOM uint8_t DSTBYCON : 1; /*!< [0..0] DSTBY Control                                                      */
            __IOM uint8_t WCODE    : 7; /*!< [7..1] Write CODE                                                         */
        } DCCR_b;
    };
    __IM uint8_t  RESERVED145;
    __IM uint16_t RESERVED146;
    __IM uint32_t RESERVED147[5];

    union
    {
        __IOM uint8_t SYRSTMSK0;        /*!< (@ 0x00000AD0) System Reset Mask Control Register0                        */

        struct
        {
            __IOM uint8_t IWDTMASK : 1; /*!< [0..0] Independent watchdog timer Reset Mask                              */
            __IOM uint8_t WDT0MASK : 1; /*!< [1..1] CPU0 Watchdog timer Reset Mask                                     */
            __IOM uint8_t SWMASK   : 1; /*!< [2..2] Software Reset Mask                                                */
            uint8_t                : 2;
            __IOM uint8_t LM0MASK  : 1; /*!< [5..5] Local memory 0 error Reset Mask                                    */
            __IOM uint8_t CMMASK   : 1; /*!< [6..6] Common memory error Reset Mask                                     */
            __IOM uint8_t BUSMASK  : 1; /*!< [7..7] BUS error Reset Mask                                               */
        } SYRSTMSK0_b;
    };
    __IM uint8_t  RESERVED148;
    __IM uint16_t RESERVED149;
    __IM uint32_t RESERVED150;

    union
    {
        __IOM uint8_t SYRSTMSK2;        /*!< (@ 0x00000AD8) System Reset Mask Control Register2                        */

        struct
        {
            __IOM uint8_t PVD1MASK : 1; /*!< [0..0] Voltage Monitor 1 Reset Mask                                       */
            __IOM uint8_t PVD2MASK : 1; /*!< [1..1] Voltage Monitor 2 Reset Mask                                       */
            uint8_t                : 6;
        } SYRSTMSK2_b;
    };
    __IM uint8_t  RESERVED151;
    __IM uint16_t RESERVED152;

    union
    {
        __IOM uint8_t TEMPRCR;         /*!< (@ 0x00000ADC) Temperature monitor reset control register                 */

        struct
        {
            __IOM uint8_t TEMPREN : 1; /*!< [0..0] Temperature reset enable bit                                       */
            __IOM uint8_t TSNEN   : 1; /*!< [1..1] Temperature monitor sensor enable bit                              */
            __IOM uint8_t CMPEN   : 1; /*!< [2..2] Comparator enable bit                                              */
            __IOM uint8_t TSNKEEP : 1; /*!< [3..3] Temperature monitor setting bit                                    */
            uint8_t               : 4;
        } TEMPRCR_b;
    };
    __IM uint8_t  RESERVED153;
    __IM uint16_t RESERVED154;

    union
    {
        __IOM uint8_t TEMPRLR;         /*!< (@ 0x00000AE0) Temperature monitor reset lock register                    */

        struct
        {
            __IOM uint8_t LOCK : 1;    /*!< [0..0] TEMPRCR lock bit                                                   */
            uint8_t            : 7;
        } TEMPRLR_b;
    };
    __IM uint8_t  RESERVED155;
    __IM uint16_t RESERVED156;
    __IM uint32_t RESERVED157[4];

    union
    {
        __IOM uint8_t LDOECR;          /*!< (@ 0x00000AF4) LDO Efficiency Control Register                            */

        struct
        {
            __IOM uint8_t EFUPE  : 1;  /*!< [0..0] LDO Efficiency Up Enable                                           */
            uint8_t              : 6;
            __IM uint8_t EFUPTSF : 1;  /*!< [7..7] LDO Efficiency Up Transition Status Flag                           */
        } LDOECR_b;
    };
    __IM uint8_t  RESERVED158;
    __IM uint16_t RESERVED159;
    __IM uint32_t RESERVED160[3];

    union
    {
        __IOM uint8_t PLL1LDOCR;       /*!< (@ 0x00000B04) PLL1-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } PLL1LDOCR_b;
    };
    __IM uint8_t  RESERVED161;
    __IM uint16_t RESERVED162;

    union
    {
        __IOM uint8_t PLL2LDOCR;       /*!< (@ 0x00000B08) PLL2-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } PLL2LDOCR_b;
    };
    __IM uint8_t  RESERVED163;
    __IM uint16_t RESERVED164;

    union
    {
        __IOM uint8_t HOCOLDOCR;       /*!< (@ 0x00000B0C) HOCO-LDO Control Register                                  */

        struct
        {
            __IOM uint8_t LDOSTP : 1;  /*!< [0..0] LDO Stop                                                           */
            __IOM uint8_t SKEEP  : 1;  /*!< [1..1] STBY Keep                                                          */
            uint8_t              : 6;
        } HOCOLDOCR_b;
    };
    __IM uint8_t  RESERVED165;
    __IM uint16_t RESERVED166;
    __IM uint32_t RESERVED167[4];

    union
    {
        __IOM uint8_t PVD1FCR;         /*!< (@ 0x00000B20) Voltage Monitor 1 Function Control Register                */

        struct
        {
            __IOM uint8_t RHSEL : 1;   /*!< [0..0] Rise Hysteresis Select                                             */
            uint8_t             : 7;
        } PVD1FCR_b;
    };
    __IM uint8_t  RESERVED168;
    __IM uint16_t RESERVED169;

    union
    {
        __IOM uint8_t PVD2FCR;         /*!< (@ 0x00000B24) Voltage Monitor 2 Function Control Register                */

        struct
        {
            __IOM uint8_t RHSEL : 1;   /*!< [0..0] Rise Hysteresis Select                                             */
            uint8_t             : 7;
        } PVD2FCR_b;
    };
    __IM uint8_t  RESERVED170;
    __IM uint16_t RESERVED171;

    union
    {
        __IOM uint8_t PVD3FCR;         /*!< (@ 0x00000B28) Voltage Monitor 3 Function Control Register                */

        struct
        {
            __IOM uint8_t RHSEL : 1;   /*!< [0..0] Rise Hysteresis Select                                             */
            uint8_t             : 7;
        } PVD3FCR_b;
    };
    __IM uint8_t  RESERVED172;
    __IM uint16_t RESERVED173;

    union
    {
        __IOM uint8_t PVD4FCR;         /*!< (@ 0x00000B2C) Voltage Monitor 4 Function Control Register                */

        struct
        {
            __IOM uint8_t RHSEL : 1;   /*!< [0..0] Rise Hysteresis Select                                             */
            uint8_t             : 7;
        } PVD4FCR_b;
    };
    __IM uint8_t  RESERVED174;
    __IM uint16_t RESERVED175;

    union
    {
        __IOM uint8_t PVD5FCR;         /*!< (@ 0x00000B30) Voltage Monitor 5 Function Control Register                */

        struct
        {
            __IOM uint8_t RHSEL : 1;   /*!< [0..0] Rise Hysteresis Select                                             */
            uint8_t             : 7;
        } PVD5FCR_b;
    };
    __IM uint8_t  RESERVED176;
    __IM uint16_t RESERVED177;

    union
    {
        __IOM uint8_t PVDLR;           /*!< (@ 0x00000B34) Voltage Monitor Lock Register                              */

        struct
        {
            __IOM uint8_t LOCK : 1;    /*!< [0..0] LOCK control                                                       */
            uint8_t            : 7;
        } PVDLR_b;
    };
    __IM uint8_t  RESERVED178;
    __IM uint16_t RESERVED179;
    __IM uint32_t RESERVED180[2];

    union
    {
        __IOM uint8_t DPSIER4;         /*!< (@ 0x00000B40) Deep Standby Interrupt Enable Register 4                   */

        struct
        {
            __IOM uint8_t DIRQ16E : 1; /*!< [0..0] IRQ16-DS Pin Enable                                                */
            __IOM uint8_t DIRQ17E : 1; /*!< [1..1] IRQ17-DS Pin Enable                                                */
            __IOM uint8_t DIRQ18E : 1; /*!< [2..2] IRQ18-DS Pin Enable                                                */
            __IOM uint8_t DIRQ19E : 1; /*!< [3..3] IRQ19-DS Pin Enable                                                */
            __IOM uint8_t DIRQ20E : 1; /*!< [4..4] IRQ20-DS Pin Enable                                                */
            __IOM uint8_t DIRQ21E : 1; /*!< [5..5] IRQ21-DS Pin Enable                                                */
            __IOM uint8_t DIRQ22E : 1; /*!< [6..6] IRQ22-DS Pin Enable                                                */
            __IOM uint8_t DIRQ23E : 1; /*!< [7..7] IRQ23-DS Pin Enable                                                */
        } DPSIER4_b;
    };
    __IM uint8_t  RESERVED181;
    __IM uint16_t RESERVED182;

    union
    {
        __IOM uint8_t DPSIER5;         /*!< (@ 0x00000B44) Deep Standby Interrupt Enable Register 5                   */

        struct
        {
            __IOM uint8_t DIRQ24E : 1; /*!< [0..0] IRQ24-DS Pin Enable                                                */
            __IOM uint8_t DIRQ25E : 1; /*!< [1..1] IRQ25-DS Pin Enable                                                */
            __IOM uint8_t DIRQ26E : 1; /*!< [2..2] IRQ26-DS Pin Enable                                                */
            __IOM uint8_t DIRQ27E : 1; /*!< [3..3] IRQ27-DS Pin Enable                                                */
            __IOM uint8_t DIRQ28E : 1; /*!< [4..4] IRQ28-DS Pin Enable                                                */
            __IOM uint8_t DIRQ29E : 1; /*!< [5..5] IRQ29-DS Pin Enable                                                */
            __IOM uint8_t DIRQ30E : 1; /*!< [6..6] IRQ30-DS Pin Enable                                                */
            __IOM uint8_t DIRQ31E : 1; /*!< [7..7] IRQ31-DS Pin Enable                                                */
        } DPSIER5_b;
    };
    __IM uint8_t  RESERVED183;
    __IM uint16_t RESERVED184;

    union
    {
        __IOM uint8_t DPSIFR4;         /*!< (@ 0x00000B48) Deep Standby Interrupt Flag Register 4                     */

        struct
        {
            __IOM uint8_t DIRQ16F : 1; /*!< [0..0] IRQ16-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ17F : 1; /*!< [1..1] IRQ17-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ18F : 1; /*!< [2..2] IRQ18-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ19F : 1; /*!< [3..3] IRQ19-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ20F : 1; /*!< [4..4] IRQ20-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ21F : 1; /*!< [5..5] IRQ21-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ22F : 1; /*!< [6..6] IRQ22-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ23F : 1; /*!< [7..7] IRQ23-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR4_b;
    };
    __IM uint8_t  RESERVED185;
    __IM uint16_t RESERVED186;

    union
    {
        __IOM uint8_t DPSIFR5;         /*!< (@ 0x00000B4C) Deep Standby Interrupt Flag Register 5                     */

        struct
        {
            __IOM uint8_t DIRQ24F : 1; /*!< [0..0] IRQ24-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ25F : 1; /*!< [1..1] IRQ25-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ26F : 1; /*!< [2..2] IRQ26-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ27F : 1; /*!< [3..3] IRQ27-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ28F : 1; /*!< [4..4] IRQ28-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ29F : 1; /*!< [5..5] IRQ29-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ30F : 1; /*!< [6..6] IRQ30-DS Pin Deep Standby Cancel Flag                              */
            __IOM uint8_t DIRQ31F : 1; /*!< [7..7] IRQ31-DS Pin Deep Standby Cancel Flag                              */
        } DPSIFR5_b;
    };
    __IM uint8_t  RESERVED187;
    __IM uint16_t RESERVED188;

    union
    {
        __IOM uint8_t DPSIEGR4;         /*!< (@ 0x00000B50) Deep Standby Interrupt Edge Register 4                     */

        struct
        {
            __IOM uint8_t DIRQ24EG : 1; /*!< [0..0] IRQ24-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ25EG : 1; /*!< [1..1] IRQ25-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ26EG : 1; /*!< [2..2] IRQ26-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ27EG : 1; /*!< [3..3] IRQ27-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ28EG : 1; /*!< [4..4] IRQ28-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ29EG : 1; /*!< [5..5] IRQ29-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ30EG : 1; /*!< [6..6] IRQ30-DS Pin Edge Select                                           */
            __IOM uint8_t DIRQ31EG : 1; /*!< [7..7] IRQ31-DS Pin Edge Select                                           */
        } DPSIEGR4_b;
    };
    __IM uint8_t  RESERVED189;
    __IM uint16_t RESERVED190;
    __IM uint32_t RESERVED191[9];

    union
    {
        __IOM uint8_t ROUTSR0;         /*!< (@ 0x00000B78) Reset Out Status Register0                                 */

        struct
        {
            uint8_t                : 1;
            __IOM uint8_t ROPVD0RF : 1; /*!< [1..1] Voltage Monitor 0 Reset Detect　Flag                              */
            __IOM uint8_t ROPVD1RF : 1; /*!< [2..2] Voltage Monitor 1 Reset Detect Flag                                */
            __IOM uint8_t ROPVD2RF : 1; /*!< [3..3] Voltage Monitor 2 Reset Detect Flag                                */
            uint8_t                : 1;
            __IOM uint8_t ROPVD4RF : 1; /*!< [5..5] Voltage Monitor 4 Reset Detect Flag                                */
            __IOM uint8_t ROPVD5RF : 1; /*!< [6..6] Voltage Monitor 5 Reset Detect Flag                                */
            uint8_t                : 1;
        } ROUTSR0_b;
    };
    __IM uint8_t  RESERVED192;
    __IM uint16_t RESERVED193;

    union
    {
        __IOM uint32_t ROUTSR1;          /*!< (@ 0x00000B7C) Reset Out Status Register1                                 */

        struct
        {
            __IOM uint32_t ROIWDTRF : 1; /*!< [0..0] Independent Watchdog Timer Reset Detect Flag                       */
            __IOM uint32_t ROWDT0RF : 1; /*!< [1..1] Watchdog Timer0 Reset Detect Flag                                  */
            __IOM uint32_t ROSWRF   : 1; /*!< [2..2] Software Reset Detect Flag                                         */
            uint32_t                : 2;
            __IOM uint32_t ROLM0RF  : 1; /*!< [5..5] Local memory 0 error Reset Detect Flag                             */
            uint32_t                : 4;
            __IOM uint32_t ROBUSRF  : 1; /*!< [10..10] Bus error Reset Detect Flag                                      */
            uint32_t                : 3;
            __IOM uint32_t ROCMRF   : 1; /*!< [14..14] Common memory error Reset Detect Flag                            */
            uint32_t                : 17;
        } ROUTSR1_b;
    };

    union
    {
        __IOM uint8_t ROUTSR2;          /*!< (@ 0x00000B80) Reset Out Status Register2                                 */

        struct
        {
            __IOM uint8_t ROCVMRF  : 1; /*!< [0..0] Core Voltage Monitor Reset Detect Flag                             */
            uint8_t                : 6;
            __IOM uint8_t ROTEMPRF : 1; /*!< [7..7] Temperature MonitorReset Detect Flag                               */
        } ROUTSR2_b;
    };
    __IM uint8_t  RESERVED194;
    __IM uint16_t RESERVED195;
    __IM uint32_t RESERVED196[31];

    union
    {
        __IOM uint8_t SOSCCR;          /*!< (@ 0x00000C00) Sub-clock oscillator control register                      */

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
            __IOM uint8_t SOSEL : 1;   /*!< [6..6] Sub Clock Oscillator Switching                                     */
            uint8_t             : 1;
        } SOMCR_b;
    };
    __IM uint16_t RESERVED197;

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
    __IM uint16_t RESERVED198;
    __IM uint32_t RESERVED199[14];

    union
    {
        __IOM uint8_t VBTBER;          /*!< (@ 0x00000C40) VBATT Backup Enable Register                               */

        struct
        {
            uint8_t            : 3;
            __IOM uint8_t VBAE : 1;    /*!< [3..3] VBATT backup register access enable bit                            */
            uint8_t            : 4;
        } VBTBER_b;
    };
    __IM uint8_t  RESERVED200;
    __IM uint16_t RESERVED201;
    __IM uint8_t  RESERVED202;

    union
    {
        __IOM uint8_t VBTBPCR2;        /*!< (@ 0x00000C45) VBATT Battery Power Supply Control Register 2              */

        struct
        {
            __IOM uint8_t VDETLVL : 3; /*!< [2..0] VDETBAT Level Select                                               */
            uint8_t               : 1;
            __IOM uint8_t VDETE   : 1; /*!< [4..4] Voltage drop detection enable                                      */
            uint8_t               : 3;
        } VBTBPCR2_b;
    };

    union
    {
        __IOM uint8_t VBTBPSR;         /*!< (@ 0x00000C46) VBATT Battery Power Supply Status Register                 */

        struct
        {
            __IOM uint8_t VBPORF : 1;  /*!< [0..0] VBATT_POR Flag                                                     */
            uint8_t              : 3;
            __IM uint8_t VBPORM  : 1;  /*!< [4..4] VBATT_POR Monitor                                                  */
            __IM uint8_t BPWSWM  : 1;  /*!< [5..5] Battery Power Supply Switch Status Monitor                         */
            uint8_t              : 2;
        } VBTBPSR_b;
    };
    __IM uint8_t RESERVED203;

    union
    {
        __IOM uint8_t VBTADSR;         /*!< (@ 0x00000C48) VBATT Tamper detection Status Register                     */

        struct
        {
            __IOM uint8_t VBTADF0 : 1; /*!< [0..0] VBATT Tamper Detection flag 0                                      */
            __IOM uint8_t VBTADF1 : 1; /*!< [1..1] VBATT Tamper Detection flag 1                                      */
            __IOM uint8_t VBTADF2 : 1; /*!< [2..2] VBATT Tamper Detection flag 2                                      */
            uint8_t               : 5;
        } VBTADSR_b;
    };

    union
    {
        __IOM uint8_t VBTADCR1;         /*!< (@ 0x00000C49) VBATT Tamper detection Control Register 1                  */

        struct
        {
            __IOM uint8_t VBTADIE0 : 1; /*!< [0..0] VBATT Tamper Detection Interrupt Enable 0                          */
            __IOM uint8_t VBTADIE1 : 1; /*!< [1..1] VBATT Tamper Detection Interrupt Enable 1                          */
            __IOM uint8_t VBTADIE2 : 1; /*!< [2..2] VBATT Tamper Detection Interrupt Enable 2                          */
            uint8_t                : 1;
            __IOM uint8_t VBTADCE0 : 1; /*!< [4..4] VBATT Tamper Detection Backup Register Clear Enable 0              */
            __IOM uint8_t VBTADCE1 : 1; /*!< [5..5] VBATT Tamper Detection Backup Register Clear Enable 1              */
            __IOM uint8_t VBTADCE2 : 1; /*!< [6..6] VBATT Tamper Detection Backup Register Clear Enable 2              */
            uint8_t                : 1;
        } VBTADCR1_b;
    };

    union
    {
        __IOM uint8_t VBTADCR2;         /*!< (@ 0x00000C4A) VBATT Tamper detection Control Register 2                  */

        struct
        {
            __IOM uint8_t VBRTCES0 : 1; /*!< [0..0] VBATT RTC Time Capture Event Source Select 0                       */
            __IOM uint8_t VBRTCES1 : 1; /*!< [1..1] VBATT RTC Time Capture Event Source Select 1                       */
            __IOM uint8_t VBRTCES2 : 1; /*!< [2..2] VBATT RTC Time Capture Event Source Select 2                       */
            uint8_t                : 5;
        } VBTADCR2_b;
    };
    __IM uint8_t RESERVED204;

    union
    {
        __IOM uint8_t VBTICTLR;         /*!< (@ 0x00000C4C) VBATT Input Control Register                               */

        struct
        {
            __IOM uint8_t VCH0INEN : 1; /*!< [0..0] VBATT CH0 Input Enable                                             */
            __IOM uint8_t VCH1INEN : 1; /*!< [1..1] VBATT CH1 Input Enable                                             */
            __IOM uint8_t VCH2INEN : 1; /*!< [2..2] VBATT CH2 Input Enable                                             */
            uint8_t                : 5;
        } VBTICTLR_b;
    };

    union
    {
        __IOM uint8_t VBTICTLR2;       /*!< (@ 0x00000C4D) VBATT Input Control Register 2                             */

        struct
        {
            __IOM uint8_t VCH0NCE : 1; /*!< [0..0] VBATT CH0 Input Noise Canceler Enable                              */
            __IOM uint8_t VCH1NCE : 1; /*!< [1..1] VBATT CH1 Input Noise Canceler Enable                              */
            __IOM uint8_t VCH2NCE : 1; /*!< [2..2] VBATT CH2 Input Noise Canceler Enable                              */
            uint8_t               : 1;
            __IOM uint8_t VCH0EG  : 1; /*!< [4..4] VBATT CH0 Input Edge Select                                        */
            __IOM uint8_t VCH1EG  : 1; /*!< [5..5] VBATT CH1 Input Edge Select                                        */
            __IOM uint8_t VCH2EG  : 1; /*!< [6..6] VBATT CH2 Input Edge Select                                        */
            uint8_t               : 1;
        } VBTICTLR2_b;
    };

    union
    {
        __IOM uint8_t VBTIMONR;        /*!< (@ 0x00000C4E) VBATT Input Monitor Register                               */

        struct
        {
            __IM uint8_t VCH0MON : 1;  /*!< [0..0] VBATT CH0 Input monitor                                            */
            __IM uint8_t VCH1MON : 1;  /*!< [1..1] VBATT CH1 Input monitor                                            */
            __IM uint8_t VCH2MON : 1;  /*!< [2..2] VBATT CH2 Input monitor                                            */
            uint8_t              : 5;
        } VBTIMONR_b;
    };
    __IM uint8_t RESERVED205;

    union
    {
        __IOM uint8_t VBTNCWCR;        /*!< (@ 0x00000C50) VBATT Noise Canceler Width Control Register                */

        struct
        {
            __IOM uint8_t VINCW : 3;   /*!< [2..0] VBATT Input Noise Canceler Width select                            */
            uint8_t             : 5;
        } VBTNCWCR_b;
    };
    __IM uint8_t  RESERVED206;
    __IM uint16_t RESERVED207;

    union
    {
        __IOM uint8_t VBTADCR3;         /*!< (@ 0x00000C54) VBATT Tamper detection Control Register 3                  */

        struct
        {
            __IOM uint8_t VBTADZE0 : 1; /*!< [0..0] VBATT Tamper Detection Zeroization Enable 0                        */
            __IOM uint8_t VBTADZE1 : 1; /*!< [1..1] VBATT Tamper Detection Zeroization Enable 1                        */
            __IOM uint8_t VBTADZE2 : 1; /*!< [2..2] VBATT Tamper Detection Zeroization Enable 2                        */
            uint8_t                : 5;
        } VBTADCR3_b;
    };
    __IM uint8_t  RESERVED208;
    __IM uint16_t RESERVED209;
    __IM uint32_t RESERVED300[42];
    __IOM uint8_t VBTBKR[128];         /*!< (@ 0x00000D00) VBATT Backup Register [0..127]                             */
} R_SYSTEM_Type;                       /*!< Size = 3456 (0xd80)                                                       */

/* =========================================================================================================================== */
/* ================                                          R_MSTP                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Module Stop Control A,B,C,D,E,F (R_MSTP)
 */

typedef struct                          /*!< (@ 0x87803000) R_MSTP Structure                                           */
{
    union
    {
        __IOM uint32_t MSTPCRA;         /*!< (@ 0x00000000) Module Stop Control Register A                             */

        struct
        {
            __IOM uint32_t MSTPA0  : 1; /*!< [0..0] SRAM0 Module Stop　Target module: SRAM0                           */
            __IOM uint32_t MSTPA1  : 1; /*!< [1..1] SRAM1 Module Stop　Target module: SRAM1                           */
            uint32_t               : 20;
            __IOM uint32_t MSTPA22 : 1; /*!< [22..22] DMA Controller/Data Transfer Controller unit0 Module
                                         *   Stop Target modules: DMAC, DTC unit0                                      */
            uint32_t               : 6;
            __IOM uint32_t MSTPA29 : 1; /*!< [29..29] EXDMAC Module Stop Target module: EXDMAC                         */
            uint32_t               : 2;
        } MSTPCRA_b;
    };

    union
    {
        __IOM uint32_t MSTPCRB;         /*!< (@ 0x00000004) Module Stop Control Register B                             */

        struct
        {
            __IOM uint32_t MSTPB0 : 1;  /*!< [0..0] Serial Communication Interface 12 Module Stop Target
                                         *   module: SCI12                                                             */
            uint32_t               : 3;
            __IOM uint32_t MSTPB4  : 1; /*!< [4..4] RI3C Bus Interface Module Stop Target module: RI3C                 */
            uint32_t               : 2;
            __IOM uint32_t MSTPB7  : 1; /*!< [7..7] RI2C Bus Interface 2 Module Stop Target module: RI2C2              */
            __IOM uint32_t MSTPB8  : 1; /*!< [8..8] RI2C Bus Interface 1 Module Stop Target module: RI2C1              */
            uint32_t               : 2;
            __IOM uint32_t MSTPB11 : 1; /*!< [11..11] Universal Serial Bus 2.0 FS Interface 0 Module Stop
                                         *   Target module: USBFS0                                                     */
            __IOM uint32_t MSTPB12 : 1; /*!< [12..12] Universal Serial Bus 2.0 HS Interface Module Stop Target
                                         *   module: USBHS                                                             */
            uint32_t               : 3;
            __IOM uint32_t MSTPB16 : 1; /*!< [16..16] OSPI0 and DOTF0 Module Stop Target module: OSPI0 and
                                         *   DOTF0                                                                     */
            __IOM uint32_t MSTPB17 : 1; /*!< [17..17] OSPI1 and DOTF1 Module Stop Target module: OSPI1 and
                                         *   DOTF1                                                                     */
            __IOM uint32_t MSTPB18 : 1; /*!< [18..18] Serial Peripheral Interface 1 Module Stop Target module:
                                         *   SPI1                                                                      */
            __IOM uint32_t MSTPB19 : 1; /*!< [19..19] Serial Peripheral Interface 0 Module Stop Target module:
                                         *   SPI0                                                                      */
            __IOM uint32_t MSTPB20 : 1; /*!< [20..20] Serial Communication Interface 11 Module Stop Target
                                         *   module: SCI11                                                             */
            __IOM uint32_t MSTPB21 : 1; /*!< [21..21] Serial Communication Interface 10 Module Stop Target
                                         *   module: SCI10                                                             */
            __IOM uint32_t MSTPB22 : 1; /*!< [22..22] Serial Communication Interface 9 Module Stop Target
                                         *   module: SCI9                                                              */
            __IOM uint32_t MSTPB23 : 1; /*!< [23..23] Serial Communication Interface 8 Module Stop Target
                                         *   module: SCI8                                                              */
            __IOM uint32_t MSTPB24 : 1; /*!< [24..24] Serial Communication Interface 7 Module Stop Target
                                         *   module: SCI7                                                              */
            __IOM uint32_t MSTPB25 : 1; /*!< [25..25] Serial Communication Interface 6 Module Stop Target
                                         *   module: SCI6                                                              */
            __IOM uint32_t MSTPB26 : 1; /*!< [26..26] Serial Communication Interface 5 Module Stop Target
                                         *   module: SCI5                                                              */
            __IOM uint32_t MSTPB27 : 1; /*!< [27..27] Serial Communication Interface 4 Module Stop Target
                                         *   module: SCI4                                                              */
            __IOM uint32_t MSTPB28 : 1; /*!< [28..28] Serial Communication Interface 3 Module Stop Target
                                         *   module: SCI3                                                              */
            __IOM uint32_t MSTPB29 : 1; /*!< [29..29] Serial Communication Interface 2 Module Stop Target
                                         *   module: SCI2                                                              */
            __IOM uint32_t MSTPB30 : 1; /*!< [30..30] Serial Communication Interface 1 Module Stop Target
                                         *   module: SCI1                                                              */
            __IOM uint32_t MSTPB31 : 1; /*!< [31..31] Serial Communication Interface 0 Module Stop Target
                                         *   module: SCI0                                                              */
        } MSTPCRB_b;
    };

    union
    {
        __IOM uint32_t MSTPCRC;         /*!< (@ 0x00000008) Module Stop Control Register C                             */

        struct
        {
            __IOM uint32_t MSTPC0 : 1;  /*!< [0..0] Clock Frequency Accuracy Measurement Circuit Module Stop
                                         *   Target module: CAC0/1/2                                                   */
            __IOM uint32_t MSTPC1 : 1;  /*!< [1..1] Cyclic Redundancy Check Calculator Module Stop Target
                                         *   module: CRC                                                               */
            __IOM uint32_t MSTPC2 : 1;  /*!< [2..2] Parallel Data Capture unit Module Stop Target module:
                                         *   PDC                                                                       */
            uint32_t              : 4;
            __IOM uint32_t MSTPC7 : 1;  /*!< [7..7] Serial Sound Interface Enhanced 1 Module Stop Target
                                         *   module: SSIE1                                                             */
            __IOM uint32_t MSTPC8 : 1;  /*!< [8..8] Serial Sound Interface Enhanced 0 Module Stop Target
                                         *   module: SSIE0                                                             */
            uint32_t               : 3;
            __IOM uint32_t MSTPC12 : 1; /*!< [12..12] Secure Digital Host IF 0 Module Stop Target module:
                                         *   SDHI0                                                                     */
            __IOM uint32_t MSTPC13 : 1; /*!< [13..13] Data Operation Circuit Module Stop Target module: DOC            */
            __IOM uint32_t MSTPC14 : 1; /*!< [14..14] Event Link Controller Module Stop Target module: ELC             */
            uint32_t               : 8;
            __IOM uint32_t MSTPC23 : 1; /*!< [23..23] Serial Peripheral Interface 2 Module Stop Target module:
                                         *   SPI2                                                                      */
            __IOM uint32_t MSTPC24 : 1; /*!< [24..24] PDMIF Module Stop Target module: PDMIF                           */
            __IOM uint32_t MSTPC25 : 1; /*!< [25..25] Controller Area Network with Flexible Data-Rate Module
                                         *   Stop Target module: CANFD                                                 */
            uint32_t               : 1;
            __IOM uint32_t MSTPC27 : 1; /*!< [27..27] Controller Area Network with Flexible Data-Rate 0 Module
                                         *   Stop Target module: CANFD Lite 0                                          */
            __IOM uint32_t MSTPC28 : 1; /*!< [28..28] ETHPHY Module Stop Target module: ETHPHY                         */
            __IOM uint32_t MSTPC29 : 1; /*!< [29..29] EtherCAT Module Stop Target module: ESC                          */
            __IOM uint32_t MSTPC30 : 1; /*!< [30..30] R-Switch2(ESWM) Module Stop Target module: ESWM                  */
            __IOM uint32_t MSTPC31 : 1; /*!< [31..31] SHIP Module Stop Target module: SHIP                             */
        } MSTPCRC_b;
    };

    union
    {
        __IOM uint32_t MSTPCRD;        /*!< (@ 0x0000000C) Module Stop Control Register D                             */

        struct
        {
            uint32_t               : 6;
            __IOM uint32_t MSTPD6  : 1; /*!< [6..6] Pulse Width Modulation Module Stop Target module: HRPWM              */
            uint32_t               : 1;
            __IOM uint32_t MSTPD8  : 1; /*!< [8..8] DSMIF1 Module Stop Target module: DSMIF1                           */
            __IOM uint32_t MSTPD9  : 1; /*!< [9..9] DSMIF0 Module Stop Target module: DSMIF0                           */
            uint32_t               : 1;
            __IOM uint32_t MSTPD11 : 1; /*!< [11..11] Port Output Enable for GPT3 Module Stop Target module:
                                         *   POEG3                                                                     */
            __IOM uint32_t MSTPD12 : 1; /*!< [12..12] Port Output Enable for GPT2 Module Stop Target module:
                                         *   POEG2                                                                     */
            __IOM uint32_t MSTPD13 : 1; /*!< [13..13] Port Output Enable for GPT1 Module Stop Target module:
                                         *   POEG1                                                                     */
            __IOM uint32_t MSTPD14 : 1; /*!< [14..14] Port Output Enable for GPT0 Module Stop Target module:
                                         *   POEG0                                                                     */
            uint32_t               : 4;
            __IOM uint32_t MSTPD19 : 1; /*!< [19..19] 12-Bit D/A Converter 1 Module Stop Target module: DAC12_1        */
            __IOM uint32_t MSTPD20 : 1; /*!< [20..20] 12-Bit D/A Converter 0 Module Stop Target module: DAC12_0        */
            __IOM uint32_t MSTPD21 : 1; /*!< [21..21] 16-Bit A/D Converter Module Stop Target module: ADC16            */
            __IOM uint32_t MSTPD22 : 1; /*!< [22..22] Temperature Sensor Module Stop Target module: TSN                */
            uint32_t               : 9;
        } MSTPCRD_b;
    };

    union
    {
        __IOM uint32_t MSTPCRE;        /*!< (@ 0x00000010) Module Stop Control Register E                             */

        struct
        {
            uint32_t               : 31;
            __IOM uint32_t MSTPE31 : 1; /*!< [31..31] General PWM Timer 0 Module Stop Target module: GPT0              */
        } MSTPCRE_b;
    };

    union
    {
        __IOM uint32_t MSTPCRF;        /*!< (@ 0x00000014) Module Stop Control Register F                             */

        struct
        {
            uint32_t              : 6;
            __IOM uint32_t MSTPF6 : 1;  /*!< [6..6] TMR2 and TMR3 Module Stop Target module: TMR2 and TMR3             */
            __IOM uint32_t MSTPF7 : 1;  /*!< [7..7] TMR0 and TMR1 Module Stop Target module: TMR0 and TMR1             */
            __IOM uint32_t MSTPF8 : 1;  /*!< [8..8] Multi Function Timer pulse Unit Module Stop Target module:
                                         *   MTU                                                                       */
            uint32_t               : 3;
            __IOM uint32_t MSTPF12 : 1; /*!< [12..12] Compare Match Timer W 1 Module Stop Target module:
                                         *   CMTW1                                                                     */
            __IOM uint32_t MSTPF13 : 1; /*!< [13..13] Compare Match Timer W 0 Module Stop Target module:
                                         *   CMTW0                                                                     */
            __IOM uint32_t MSTPF14 : 1; /*!< [14..14] CMT2 and CMT3 Module Stop Target module: CMT2 and CMT3           */
            __IOM uint32_t MSTPF15 : 1; /*!< [15..15] CMT0 and CMT1 Module Stop Target module: CMT0 and CMT1           */
            __IOM uint32_t MSTPF16 : 1; /*!< [16..16] PIO0 Module Stop Target module: PIO0                           */
            uint32_t               : 13;
            __IOM uint32_t MSTPF30 : 1; /*!< [30..30] PCIF 1 Module Stop Target module: PCIF1                          */
            __IOM uint32_t MSTPF31 : 1; /*!< [31..31] PCIF 0 Module StopTarget module: PCIF0                           */
        } MSTPCRF_b;
    };
} R_MSTP_Type;                          /*!< Size = 24 (0x18)                                                          */

#endif /* R_SYSTEM_REG_H */

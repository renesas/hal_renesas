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

#ifndef R_RTC_REG_H
#define R_RTC_REG_H

/* =========================================================================================================================== */
/* ================                              Device Specific Cluster Section                              ================ */
/* =========================================================================================================================== */

/**
 * @brief R_RTC_RTCCR [RTCCR] (Time Capture Control Register)
 */
typedef struct
{
    union
    {
        __IOM uint8_t RTCCR;           /*!< (@ 0x00000000) Time Capture Control Register                              */

        struct
        {
            __IOM uint8_t TCCT : 2;    /*!< [1..0] Time Capture Control                                               */
            __IOM uint8_t TCST : 1;    /*!< [2..2] Time Capture Status                                                */
            uint8_t            : 1;
            __IOM uint8_t TCNF : 2;    /*!< [5..4] Time Capture Noise Filter Control                                  */
            uint8_t            : 1;
            __IOM uint8_t TCEN : 1;    /*!< [7..7] Time Capture Event Input Pin Enable                                */
        } RTCCR_b;
    };
    __IM uint8_t RESERVED;
} R_RTC_RTCCR_Type;                    /*!< Size = 2 (0x2)                                                            */

/**
 * @brief R_RTC_CP [CP] (Capture registers)
 */
typedef struct
{
    __IM uint8_t RESERVED[2];

    union
    {
        union
        {
            __IM uint8_t BCNT0;          /*!< (@ 0x00000002) BCNT0 Capture Register                                     */

            struct
            {
                __IM uint8_t BCNTCP : 8; /*!< [7..0] The BCNT0CP register is a read-only register that captures
                                          *   the BCNT0 value when a time capture event is detected.                    */
            } BCNT0_b;
        };

        union
        {
            __IM uint8_t RSEC;          /*!< (@ 0x00000002) Second Capture Register                                    */

            struct
            {
                __IM uint8_t SEC1 : 4;  /*!< [3..0] 1-Second Capture Capture value for the ones place of
                                         *   seconds                                                                   */
                __IM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Capture Capture value for the tens place of
                                         *   seconds                                                                   */
                uint8_t : 1;
            } RSEC_b;
        };
    };
    __IM uint8_t RESERVED1;

    union
    {
        union
        {
            __IM uint8_t BCNT1;          /*!< (@ 0x00000004) BCNT1 Capture Register                                     */

            struct
            {
                __IM uint8_t BCNTCP : 8; /*!< [7..0] The BCNT1CP register is a read-only register that captures
                                          *   the BCNT1 value when a time capture event is detected.                    */
            } BCNT1_b;
        };

        union
        {
            __IM uint8_t RMIN;          /*!< (@ 0x00000004) Minute Capture Register                                    */

            struct
            {
                __IM uint8_t MIN1 : 4;  /*!< [3..0] 1-Minute Capture Capture value for the ones place of
                                         *   minutes                                                                   */
                __IM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Capture Capture value for the tens place of
                                         *   minutes                                                                   */
                uint8_t : 1;
            } RMIN_b;
        };
    };
    __IM uint8_t RESERVED2;

    union
    {
        union
        {
            __IM uint8_t BCNT2;          /*!< (@ 0x00000006) BCNT2 Capture Register                                     */

            struct
            {
                __IM uint8_t BCNTCP : 8; /*!< [7..0] The BCNT2CP register is a read-only register that captures
                                          *   the BCNT2 value when a time capture event is detected.                    */
            } BCNT2_b;
        };

        union
        {
            __IM uint8_t RHR;          /*!< (@ 0x00000006) Hour Capture Register                                      */

            struct
            {
                __IM uint8_t HR1 : 4;  /*!< [3..0] 1-Minute Capture Capture value for the ones place of
                                        *   hours                                                                     */
                __IM uint8_t HR10 : 2; /*!< [5..4] 10-Minute Capture Capture value for the tens place of
                                        *   hours                                                                     */
                __IM uint8_t PM : 1;   /*!< [6..6] PM                                                                 */
                uint8_t         : 1;
            } RHR_b;
        };
    };
    __IM uint8_t RESERVED3[3];

    union
    {
        union
        {
            __IM uint8_t BCNT3;          /*!< (@ 0x0000000A) BCNT3 Capture Register                                     */

            struct
            {
                __IM uint8_t BCNTCP : 8; /*!< [7..0] The BCNT3CP register is a read-only register that captures
                                          *   the BCNT3 value when a time capture event is detected.                    */
            } BCNT3_b;
        };

        union
        {
            __IM uint8_t RDAY;           /*!< (@ 0x0000000A) Date Capture Register                                      */

            struct
            {
                __IM uint8_t DATE1  : 4; /*!< [3..0] 1-Day Capture Capture value for the ones place of days             */
                __IM uint8_t DATE10 : 2; /*!< [5..4] 10-Day Capture Capture value for the tens place of days            */
                uint8_t             : 2;
            } RDAY_b;
        };
    };
    __IM uint8_t RESERVED4;

    union
    {
        __IM uint8_t RMON;             /*!< (@ 0x0000000C) Month Capture Register                                     */

        struct
        {
            __IM uint8_t MON1  : 4;    /*!< [3..0] 1-Month Capture Capture value for the ones place of months         */
            __IM uint8_t MON10 : 1;    /*!< [4..4] 10-Month Capture Capture value for the tens place of
                                        *   months                                                                    */
            uint8_t : 3;
        } RMON_b;
    };
    __IM uint8_t RESERVED5[3];
} R_RTC_CP_Type;                       /*!< Size = 16 (0x10)                                                          */

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/* =========================================================================================================================== */
/* ================                                           R_RTC                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Realtime Clock (R_RTC)
 */

typedef struct                         /*!< (@ 0x87802000) R_RTC Structure                                            */
{
    union
    {
        __IM uint8_t R64CNT;           /*!< (@ 0x00000000) 64-Hz Counter                                              */

        struct
        {
            __IM uint8_t F64HZ : 1;    /*!< [0..0] 64-Hz Flag                                                         */
            __IM uint8_t F32HZ : 1;    /*!< [1..1] 32-Hz Flag                                                         */
            __IM uint8_t F16HZ : 1;    /*!< [2..2] 16-Hz Flag                                                         */
            __IM uint8_t F8HZ  : 1;    /*!< [3..3] 8-Hz Flag                                                          */
            __IM uint8_t F4HZ  : 1;    /*!< [4..4] 4-Hz Flag                                                          */
            __IM uint8_t F2HZ  : 1;    /*!< [5..5] 2-Hz Flag                                                          */
            __IM uint8_t F1HZ  : 1;    /*!< [6..6] 1-Hz Flag                                                          */
            uint8_t            : 1;
        } R64CNT_b;
    };
    __IM uint8_t RESERVED;

    union
    {
        union
        {
            __IOM uint8_t RSECCNT;       /*!< (@ 0x00000002) Second Counter                                             */

            struct
            {
                __IOM uint8_t SEC1 : 4;  /*!< [3..0] 1-Second Count Counts from 0 to 9 every second. When
                                          *   a carry is generated, 1 is added to the tens place.                       */
                __IOM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Count Counts from 0 to 5 for 60-second counting.         */
                uint8_t             : 1;
            } RSECCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT0;        /*!< (@ 0x00000002) Binary Counter 0                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] The BCNT0 counter is a readable/writable 32-bit binary
                                         *   counter b7 to b0.                                                         */
            } BCNT0_b;
        };
    };
    __IM uint8_t RESERVED1;

    union
    {
        union
        {
            __IOM uint8_t RMINCNT;       /*!< (@ 0x00000004) Minute Counter                                             */

            struct
            {
                __IOM uint8_t MIN1 : 4;  /*!< [3..0] 1-Minute Count Counts from 0 to 9 every minute. When
                                          *   a carry is generated, 1 is added to the tens place.                       */
                __IOM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Count Counts from 0 to 5 for 60-minute counting.         */
                uint8_t             : 1;
            } RMINCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT1;        /*!< (@ 0x00000004) Binary Counter 1                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] The BCNT1 counter is a readable/writable 32-bit binary
                                         *   counter b15 to b8.                                                        */
            } BCNT1_b;
        };
    };
    __IM uint8_t RESERVED2;

    union
    {
        union
        {
            __IOM uint8_t RHRCNT;       /*!< (@ 0x00000006) Hour Counter                                               */

            struct
            {
                __IOM uint8_t HR1 : 4;  /*!< [3..0] 1-Hour Count Counts from 0 to 9 once per hour. When a
                                         *   carry is generated, 1 is added to the tens place.                         */
                __IOM uint8_t HR10 : 2; /*!< [5..4] 10-Hour Count Counts from 0 to 2 once per carry from
                                         *   the ones place.                                                           */
                __IOM uint8_t PM : 1;   /*!< [6..6] Time Counter Setting for a.m./p.m.                                 */
                uint8_t          : 1;
            } RHRCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT2;        /*!< (@ 0x00000006) Binary Counter 2                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] The BCNT2 counter is a readable/writable 32-bit binary
                                         *   counter b23 to b16.                                                       */
            } BCNT2_b;
        };
    };
    __IM uint8_t RESERVED3;

    union
    {
        union
        {
            __IOM uint8_t RWKCNT;       /*!< (@ 0x00000008) Day-of-Week Counter                                        */

            struct
            {
                __IOM uint8_t DAYW : 3; /*!< [2..0] Day-of-Week Counting                                               */
                uint8_t            : 5;
            } RWKCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT3;        /*!< (@ 0x00000008) Binary Counter 3                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] The BCNT3 counter is a readable/writable 32-bit binary
                                         *   counter b31 to b24.                                                       */
            } BCNT3_b;
        };
    };
    __IM uint8_t RESERVED4;

    union
    {
        __IOM uint8_t RDAYCNT;         /*!< (@ 0x0000000A) Date Counter                                               */

        struct
        {
            __IOM uint8_t DATE1 : 4;   /*!< [3..0] 1-Day Count Counts from 0 to 9 once per day. When a carry
                                        *   is generated, 1 is added to the tens place.                               */
            __IOM uint8_t DATE10 : 2;  /*!< [5..4] 10-Day Count Counts from 0 to 3 once per carry from the
                                        *   ones place.                                                               */
            uint8_t : 2;
        } RDAYCNT_b;
    };
    __IM uint8_t RESERVED5;

    union
    {
        __IOM uint8_t RMONCNT;         /*!< (@ 0x0000000C) Month Counter                                              */

        struct
        {
            __IOM uint8_t MON1 : 4;    /*!< [3..0] 1-Month Count Counts from 0 to 9 once per month. When
                                        *   a carry is generated, 1 is added to the tens place.                       */
            __IOM uint8_t MON10 : 1;   /*!< [4..4] 10-Month Count Counts from 0 to 1 once per carry from
                                        *   the ones place.                                                           */
            uint8_t : 3;
        } RMONCNT_b;
    };
    __IM uint8_t RESERVED6;

    union
    {
        __IOM uint16_t RYRCNT;         /*!< (@ 0x0000000E) Year Counter                                               */

        struct
        {
            __IOM uint16_t YR1 : 4;    /*!< [3..0] 1-Year Count Counts from 0 to 9 once per year. When a
                                        *   carry is generated, 1 is added to the tens place.                         */
            __IOM uint16_t YR10 : 4;   /*!< [7..4] 10-Year Count Counts from 0 to 9 once per carry from
                                        *   ones place. When a carry is generated in the tens place,
                                        *   1 is added to the hundreds place.                                         */
            uint16_t : 8;
        } RYRCNT_b;
    };

    union
    {
        union
        {
            __IOM uint8_t RSECAR;        /*!< (@ 0x00000010) Second Alarm Register                                      */

            struct
            {
                __IOM uint8_t SEC1  : 4; /*!< [3..0] 1 Second Value for the ones place of seconds                       */
                __IOM uint8_t SEC10 : 3; /*!< [6..4] 10 Seconds Value for the tens place of seconds                     */
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RSECAR_b;
        };

        union
        {
            __IOM uint8_t BCNT0AR;        /*!< (@ 0x00000010) Binary Counter 0 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] The BCNT0AR register is a readable/writable alarm register
                                           *   corresponding to 32-bit binary counter b7 to b0.                          */
            } BCNT0AR_b;
        };
    };
    __IM uint8_t RESERVED7;

    union
    {
        union
        {
            __IOM uint8_t RMINAR;        /*!< (@ 0x00000012) Minute Alarm Register                                      */

            struct
            {
                __IOM uint8_t MIN1  : 4; /*!< [3..0] 1 Minute Value for the ones place of minutes                       */
                __IOM uint8_t MIN10 : 3; /*!< [6..4] 10 Minutes Value for the tens place of minutes                     */
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RMINAR_b;
        };

        union
        {
            __IOM uint8_t BCNT1AR;        /*!< (@ 0x00000012) Binary Counter 1 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] The BCNT1AR register is a readable/writable alarm register
                                           *   corresponding to 32-bit binary counter b15 to b8.                         */
            } BCNT1AR_b;
        };
    };
    __IM uint8_t RESERVED8;

    union
    {
        union
        {
            __IOM uint8_t RHRAR;        /*!< (@ 0x00000014) Hour Alarm Register                                        */

            struct
            {
                __IOM uint8_t HR1  : 4; /*!< [3..0] 1-Hour Count Value for the ones place of hours                     */
                __IOM uint8_t HR10 : 2; /*!< [5..4] 10-Hour Count Value for the tens place of hours                    */
                __IOM uint8_t PM   : 1; /*!< [6..6] Time Alarm Setting for a.m./p.m.                                   */
                __IOM uint8_t ENB  : 1; /*!< [7..7] ENB                                                                */
            } RHRAR_b;
        };

        union
        {
            __IOM uint8_t BCNT2AR;        /*!< (@ 0x00000014) Binary Counter 2 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] The BCNT2AR register is a readable/writable alarm register
                                           *   corresponding to 32-bit binary counter b23 to b16.                        */
            } BCNT2AR_b;
        };
    };
    __IM uint8_t RESERVED9;

    union
    {
        union
        {
            __IOM uint8_t RWKAR;        /*!< (@ 0x00000016) Day-of-Week Alarm Register                                 */

            struct
            {
                __IOM uint8_t DAYW : 3; /*!< [2..0] Day-of-Week Counting                                               */
                uint8_t            : 4;
                __IOM uint8_t ENB  : 1; /*!< [7..7] ENB                                                                */
            } RWKAR_b;
        };

        union
        {
            __IOM uint8_t BCNT3AR;        /*!< (@ 0x00000016) Binary Counter 3 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] The BCNT3AR register is a readable/writable alarm register
                                           *   corresponding to 32-bit binary counter b31 to b24.                        */
            } BCNT3AR_b;
        };
    };
    __IM uint8_t RESERVED10;

    union
    {
        union
        {
            __IOM uint8_t RDAYAR;         /*!< (@ 0x00000018) Date Alarm Register                                        */

            struct
            {
                __IOM uint8_t DATE1  : 4; /*!< [3..0] 1 Day Value for the ones place of days                             */
                __IOM uint8_t DATE10 : 2; /*!< [5..4] 10 Days Value for the tens place of days                           */
                uint8_t              : 1;
                __IOM uint8_t ENB    : 1; /*!< [7..7] ENB                                                                */
            } RDAYAR_b;
        };

        union
        {
            __IOM uint8_t BCNT0AER;    /*!< (@ 0x00000018) Binary Counter 0 Alarm Enable Register                     */

            struct
            {
                __IOM uint8_t ENB : 8; /*!< [7..0] The BCNT0AER register is a readable/writable register
                                        *   for setting the alarm enable corresponding to 32-bit binary
                                        *   counter b7 to b0.                                                         */
            } BCNT0AER_b;
        };
    };
    __IM uint8_t RESERVED11;

    union
    {
        union
        {
            __IOM uint8_t RMONAR;        /*!< (@ 0x0000001A) Month Alarm Register                                       */

            struct
            {
                __IOM uint8_t MON1  : 4; /*!< [3..0] 1 Month Value for the ones place of months                         */
                __IOM uint8_t MON10 : 1; /*!< [4..4] 10 Months Value for the tens place of months                       */
                uint8_t             : 2;
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RMONAR_b;
        };

        union
        {
            __IOM uint8_t BCNT1AER;    /*!< (@ 0x0000001A) Binary Counter 1 Alarm Enable Register                     */

            struct
            {
                __IOM uint8_t ENB : 8; /*!< [7..0] The BCNT1AER register is a readable/writable register
                                        *   for setting the alarm enable corresponding to 32-bit binary
                                        *   counter b15 to b8.                                                        */
            } BCNT1AER_b;
        };
    };
    __IM uint8_t RESERVED12;

    union
    {
        union
        {
            __IOM uint16_t RYRAR;        /*!< (@ 0x0000001C) Year Alarm Register                                        */

            struct
            {
                __IOM uint16_t YR1  : 4; /*!< [3..0] 1 Year Value for the ones place of years                           */
                __IOM uint16_t YR10 : 4; /*!< [7..4] 10 Years Value for the tens place of years                         */
                uint16_t            : 8;
            } RYRAR_b;
        };

        union
        {
            __IOM uint16_t BCNT2AER;    /*!< (@ 0x0000001C) Binary Counter 2 Alarm Enable Register                     */

            struct
            {
                __IOM uint16_t ENB : 8; /*!< [7..0] The BCNT2AER register is a readable/writable register
                                         *   for setting the alarm enable corresponding to 32-bit binary
                                         *   counter b23 to b16.                                                       */
                uint16_t : 8;
            } BCNT2AER_b;
        };
    };

    union
    {
        union
        {
            __IOM uint8_t RYRAREN;     /*!< (@ 0x0000001E) Year Alarm Enable Register                                 */

            struct
            {
                uint8_t           : 7;
                __IOM uint8_t ENB : 1; /*!< [7..7] ENB                                                                */
            } RYRAREN_b;
        };

        union
        {
            __IOM uint8_t BCNT3AER;    /*!< (@ 0x0000001E) Binary Counter 3 Alarm Enable Register                     */

            struct
            {
                __IOM uint8_t ENB : 8; /*!< [7..0] The BCNT3AER register is a readable/writable register
                                        *   for setting the alarm enable corresponding to 32-bit binary
                                        *   counter b31 to b24.                                                       */
            } BCNT3AER_b;
        };
    };
    __IM uint8_t  RESERVED13;
    __IM uint16_t RESERVED14;

    union
    {
        __IOM uint8_t RCR1;            /*!< (@ 0x00000022) RTC Control Register 1                                     */

        struct
        {
            __IOM uint8_t AIE   : 1;   /*!< [0..0] Alarm Interrupt Enable                                             */
            __IOM uint8_t CIE   : 1;   /*!< [1..1] Carry Interrupt Enable                                             */
            __IOM uint8_t PIE   : 1;   /*!< [2..2] Periodic Interrupt Enable                                          */
            __IOM uint8_t RTCOS : 1;   /*!< [3..3] RTCOUT Output Select                                               */
            __IOM uint8_t PES   : 4;   /*!< [7..4] Periodic Interrupt Select                                          */
        } RCR1_b;
    };
    __IM uint8_t RESERVED15;

    union
    {
        __IOM uint8_t RCR2;            /*!< (@ 0x00000024) RTC Control Register 2                                     */

        struct
        {
            __IOM uint8_t START : 1;   /*!< [0..0] Start                                                              */
            __IOM uint8_t RESET : 1;   /*!< [1..1] RTC Software Reset                                                 */
            __IOM uint8_t ADJ30 : 1;   /*!< [2..2] 30-Second Adjustment                                               */
            __IOM uint8_t RTCOE : 1;   /*!< [3..3] RTCOUT Output Enable                                               */
            __IOM uint8_t AADJE : 1;   /*!< [4..4] Automatic Adjustment Enable                                        */
            __IOM uint8_t AADJP : 1;   /*!< [5..5] Automatic Adjustment Period Select                                 */
            __IOM uint8_t HR24  : 1;   /*!< [6..6] Hours Mode                                                         */
            __IOM uint8_t CNTMD : 1;   /*!< [7..7] Count Mode Select                                                  */
        } RCR2_b;
    };
    __IM uint8_t  RESERVED16;
    __IM uint16_t RESERVED17;

    union
    {
        __IOM uint8_t RCR4;            /*!< (@ 0x00000028) RTC Control Register 4                                     */

        struct
        {
            __IOM uint8_t RCKSEL : 1;  /*!< [0..0] Count Source Select                                                */
            uint8_t              : 7;
        } RCR4_b;
    };
    __IM uint8_t RESERVED18;

    union
    {
        __IOM uint16_t RFRH;           /*!< (@ 0x0000002A) Frequency Register H                                       */

        struct
        {
            __IOM uint16_t RFC : 1;    /*!< [0..0] Frequency Divide Ratio Setting Set the divide ratio of
                                        *   the LOCO clock to generate the reference clock of 128 Hz.
                                        *   If the setting value in the RFC[16:0] bits is n, the prescaler
                                        *   divides the LOCO clock by n + 1.                                          */
            uint16_t : 15;
        } RFRH_b;
    };

    union
    {
        __IOM uint16_t RFRL;           /*!< (@ 0x0000002C) Frequency Register L                                       */

        struct
        {
            __IOM uint16_t RFC : 16;   /*!< [15..0] Frequency Divide Ratio Setting Set the divide ratio
                                        *   of the LOCO clock to generate the reference clock of 128
                                        *   Hz. If the setting value in the RFC[16:0] bits is n, the
                                        *   prescaler divides the LOCO clock by n + 1.                                */
        } RFRL_b;
    };

    union
    {
        __IOM uint8_t RADJ;            /*!< (@ 0x0000002E) Time Error Adjustment Register                             */

        struct
        {
            __IOM uint8_t ADJ : 6;     /*!< [5..0] Adjustment Value These bits specify the adjustment value
                                        *   from the prescaler.                                                       */
            __IOM uint8_t PMADJ : 2;   /*!< [7..6] Plus-Minus                                                         */
        } RADJ_b;
    };
    __IM uint8_t           RESERVED19;
    __IM uint16_t          RESERVED20[8];
    __IOM R_RTC_RTCCR_Type RTCCR[3];   /*!< (@ 0x00000040) Time Capture Control Register                              */
    __IM uint16_t          RESERVED21[5];
    __IOM R_RTC_CP_Type    CP[3];      /*!< (@ 0x00000050) Capture registers                                          */
} R_RTC_Type;                          /*!< Size = 128 (0x80)                                                         */

#endif /* R_RTC_REG_H */

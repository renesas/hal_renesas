/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef R_RTC_REG_H
#define R_RTC_REG_H

/* =========================================================================================================================== */
/* ================                              Device Specific Cluster Section                              ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_clusters
 * @{
 */

/**
 * @brief R_RTC_RTCCR [RTCCR] (Time Capture Control Register [0..2])
 */
typedef struct
{
    union
    {
        __IOM uint8_t RTCCR;           /*!< (@ 0x00000000) Interrupt request register                                 */

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
} R_RTC_RTCCR_Type;                    /*!< Size = 1 (0x1)                                                            */

/** @} */ /* End of group Device_Peripheral_clusters */

/* =========================================================================================================================== */
/* ================                            Device Specific Peripheral Section                             ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_peripherals
 * @{
 */

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
                __IOM uint8_t SEC1  : 4; /*!< [3..0] 1-Second Count                                                     */
                __IOM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Count                                                    */
                uint8_t             : 1;
            } RSECCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT0;        /*!< (@ 0x00000002) Binary Counter 0                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t MIN1  : 4; /*!< [3..0] 1-Minute Count                                                     */
                __IOM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Count                                                    */
                uint8_t             : 1;
            } RMINCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT1;        /*!< (@ 0x00000004) Binary Counter 1                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t HR1  : 4; /*!< [3..0] 1-Hour Count                                                       */
                __IOM uint8_t HR10 : 2; /*!< [5..4] 10-Hour Count                                                      */
                __IOM uint8_t PM   : 1; /*!< [6..6] PM Time Counter Setting for a.m./p.m.                              */
                uint8_t            : 1;
            } RHRCNT_b;
        };

        union
        {
            __IOM uint8_t BCNT2;        /*!< (@ 0x00000006) Binary Counter 2                                           */

            struct
            {
                __IOM uint8_t BCNT : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t BCNT : 8; /*!< [7..0] Binary Counter                                                     */
            } BCNT3_b;
        };
    };
    __IM uint8_t RESERVED4;

    union
    {
        __IOM uint8_t RDAYCNT;         /*!< (@ 0x0000000A) Date Counter                                               */

        struct
        {
            __IOM uint8_t DATE1  : 4;  /*!< [3..0] 1-Day Count                                                        */
            __IOM uint8_t DATE10 : 2;  /*!< [5..4] 10-Day Count                                                       */
            uint8_t              : 2;
        } RDAYCNT_b;
    };
    __IM uint8_t RESERVED5;

    union
    {
        __IOM uint8_t RMONCNT;         /*!< (@ 0x0000000C) Month Counter                                              */

        struct
        {
            __IOM uint8_t MON1  : 4;   /*!< [3..0] 1-Month Count                                                      */
            __IOM uint8_t MON10 : 1;   /*!< [4..4] 10-Month Count                                                     */
            uint8_t             : 3;
        } RMONCNT_b;
    };
    __IM uint8_t RESERVED6;

    union
    {
        __IOM uint16_t RYRCNT;         /*!< (@ 0x0000000E) Year Counter                                               */

        struct
        {
            __IOM uint16_t YR1  : 4;   /*!< [3..0] 1-Year Count                                                       */
            __IOM uint16_t YR10 : 4;   /*!< [7..4] 10-Year Count                                                      */
            uint16_t            : 8;
        } RYRCNT_b;
    };

    union
    {
        union
        {
            __IOM uint8_t RSECAR;        /*!< (@ 0x00000010) Second Alarm Register                                      */

            struct
            {
                __IOM uint8_t SEC1  : 4; /*!< [3..0] 1 Second                                                           */
                __IOM uint8_t SEC10 : 3; /*!< [6..4] 10 Seconds                                                         */
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RSECAR_b;
        };

        union
        {
            __IOM uint8_t BCNT0AR;        /*!< (@ 0x00000010) Binary Counter 0 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t MIN1  : 4; /*!< [3..0] 1 Minute                                                           */
                __IOM uint8_t MIN10 : 3; /*!< [6..4] 10 Minutes                                                         */
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RMINAR_b;
        };

        union
        {
            __IOM uint8_t BCNT1AR;        /*!< (@ 0x00000012) Binary Counter 1 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t HR1  : 4; /*!< [3..0] 1-Hour Count                                                       */
                __IOM uint8_t HR10 : 2; /*!< [5..4] 10-Hour Count                                                      */
                __IOM uint8_t PM   : 1; /*!< [6..6] PM                                                                 */
                __IOM uint8_t ENB  : 1; /*!< [7..7] ENB                                                                */
            } RHRAR_b;
        };

        union
        {
            __IOM uint8_t BCNT2AR;        /*!< (@ 0x00000014) Binary Counter 2 Alarm Register                            */

            struct
            {
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t BCNTAR : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t DATE1  : 4; /*!< [3..0] 1 Day                                                              */
                __IOM uint8_t DATE10 : 2; /*!< [5..4] 10 Days                                                            */
                uint8_t              : 1;
                __IOM uint8_t ENB    : 1; /*!< [7..7] ENB                                                                */
            } RDAYAR_b;
        };

        union
        {
            __IOM uint8_t BCNT0AER;    /*!< (@ 0x00000018) Binary Counter 0 Alarm Enable Register                     */

            struct
            {
                __IOM uint8_t ENB : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint8_t MON1  : 4; /*!< [3..0] 1 Month                                                            */
                __IOM uint8_t MON10 : 1; /*!< [4..4] 10 Months                                                          */
                uint8_t             : 2;
                __IOM uint8_t ENB   : 1; /*!< [7..7] ENB                                                                */
            } RMONAR_b;
        };

        union
        {
            __IOM uint8_t BCNT1AER;    /*!< (@ 0x0000001A) Binary Counter 1 Alarm Enable Register                     */

            struct
            {
                __IOM uint8_t ENB : 8; /*!< [7..0] Binary Counter                                                     */
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
                __IOM uint16_t YR1  : 4; /*!< [3..0] 1 Year                                                             */
                __IOM uint16_t YR10 : 4; /*!< [7..4] 10 Years                                                           */
                uint16_t            : 8;
            } RYRAR_b;
        };

        union
        {
            __IOM uint16_t BCNT2AER;    /*!< (@ 0x0000001C) Binary Counter 2 Alarm Enable Register                     */

            struct
            {
                __IOM uint16_t ENB : 8; /*!< [7..0] Binary Counter                                                     */
                uint16_t           : 8;
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
                __IOM uint8_t ENB : 8; /*!< [7..0] Binary Counter                                                     */
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
            __IOM uint16_t RFC : 1;    /*!< [0..0] Frequency Divide Ratio Setting                                     */
            uint16_t           : 15;
        } RFRH_b;
    };

    union
    {
        __IOM uint16_t RFRL;           /*!< (@ 0x0000002C) Frequency Register L                                       */

        struct
        {
            __IOM uint16_t RFC : 16;   /*!< [15..0] Frequency Divide Ratio Setting                                    */
        } RFRL_b;
    };

    union
    {
        __IOM uint8_t RADJ;            /*!< (@ 0x0000002E) Time Error Adjustment Register                             */

        struct
        {
            __IOM uint8_t ADJ   : 6;   /*!< [5..0] Adjustment Value                                                   */
            __IOM uint8_t PMADJ : 2;   /*!< [7..6] Plus-Minus                                                         */
        } RADJ_b;
    };
    __IM uint8_t           RESERVED19;
    __IM uint16_t          RESERVED20[8];
    __IOM R_RTC_RTCCR_Type RTCCR[3];   /*!< (@ 0x00000040) Time Capture Control Register [0..2]                       */
    __IM uint8_t           RESERVED21;
    __IM uint16_t          RESERVED22[7];

    union
    {
        union
        {
            __IM uint8_t RSECCP0;       /*!< (@ 0x00000052) Second Capture Register 0                                  */

            struct
            {
                __IM uint8_t SEC1  : 4; /*!< [3..0] 1-Second Capture                                                   */
                __IM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Capture                                                  */
                uint8_t            : 1;
            } RSECCP0_b;
        };
        __IM uint8_t BCNT0CP0;          /*!< (@ 0x00000052) BCNT0 Capture Register 0                                   */
    };
    __IM uint8_t RESERVED23;

    union
    {
        union
        {
            __IM uint8_t RSECCP1;       /*!< (@ 0x00000054) Second Capture Register 1                                  */

            struct
            {
                __IM uint8_t SEC1  : 4; /*!< [3..0] 1-Second Capture                                                   */
                __IM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Capture                                                  */
                uint8_t            : 1;
            } RSECCP1_b;
        };
        __IM uint8_t BCNT0CP1;          /*!< (@ 0x00000054) BCNT0 Capture Register 1                                   */

        union
        {
            __IM uint8_t RMINCP0;       /*!< (@ 0x00000054) Minute Capture Register 0                                  */

            struct
            {
                __IM uint8_t MIN1  : 4; /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Capture                                                  */
                uint8_t            : 1;
            } RMINCP0_b;
        };
        __IM uint8_t BCNT1CP0;          /*!< (@ 0x00000054) BCNT1 Capture Register 0                                   */
    };
    __IM uint8_t RESERVED24;

    union
    {
        union
        {
            __IM uint8_t RSECCP2;       /*!< (@ 0x00000056) Second Capture Register 2                                  */

            struct
            {
                __IM uint8_t SEC1  : 4; /*!< [3..0] 1-Second Capture                                                   */
                __IM uint8_t SEC10 : 3; /*!< [6..4] 10-Second Capture                                                  */
                uint8_t            : 1;
            } RSECCP2_b;
        };
        __IM uint8_t BCNT0CP2;          /*!< (@ 0x00000056) BCNT0 Capture Register 2                                   */

        union
        {
            __IM uint8_t RMINCP1;       /*!< (@ 0x00000056) Minute Capture Register 1                                  */

            struct
            {
                __IM uint8_t MIN1  : 4; /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Capture                                                  */
                uint8_t            : 1;
            } RMINCP1_b;
        };
        __IM uint8_t BCNT1CP1;          /*!< (@ 0x00000056) BCNT1 Capture Register 1                                   */

        union
        {
            __IM uint8_t RHRCP0;        /*!< (@ 0x00000056) Hour Capture Register 0                                    */

            struct
            {
                __IM uint8_t HR1  : 4;  /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t HR10 : 2;  /*!< [5..4] 10-Minute Capture                                                  */
                __IM uint8_t PM   : 1;  /*!< [6..6] PM                                                                 */
                uint8_t           : 1;
            } RHRCP0_b;
        };
        __IM uint8_t BCNT2CP0;          /*!< (@ 0x00000056) BCNT2 Capture Register 0                                   */
    };
    __IM uint8_t RESERVED25;

    union
    {
        union
        {
            __IM uint8_t RMINCP2;       /*!< (@ 0x00000058) Minute Capture Register 2                                  */

            struct
            {
                __IM uint8_t MIN1  : 4; /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t MIN10 : 3; /*!< [6..4] 10-Minute Capture                                                  */
                uint8_t            : 1;
            } RMINCP2_b;
        };
        __IM uint8_t BCNT1CP2;          /*!< (@ 0x00000058) BCNT1 Capture Register 2                                   */

        union
        {
            __IM uint8_t RHRCP1;        /*!< (@ 0x00000058) Hour Capture Register 1                                    */

            struct
            {
                __IM uint8_t HR1  : 4;  /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t HR10 : 2;  /*!< [5..4] 10-Minute Capture                                                  */
                __IM uint8_t PM   : 1;  /*!< [6..6] PM                                                                 */
                uint8_t           : 1;
            } RHRCP1_b;
        };
        __IM uint8_t BCNT2CP1;          /*!< (@ 0x00000058) BCNT2 Capture Register 1                                   */
    };
    __IM uint8_t RESERVED26;

    union
    {
        union
        {
            __IM uint8_t RHRCP2;         /*!< (@ 0x0000005A) Hour Capture Register 2                                    */

            struct
            {
                __IM uint8_t HR1  : 4;   /*!< [3..0] 1-Minute Capture                                                   */
                __IM uint8_t HR10 : 2;   /*!< [5..4] 10-Minute Capture                                                  */
                __IM uint8_t PM   : 1;   /*!< [6..6] PM                                                                 */
                uint8_t           : 1;
            } RHRCP2_b;
        };
        __IM uint8_t BCNT2CP2;           /*!< (@ 0x0000005A) BCNT2 Capture Register 2                                   */

        union
        {
            __IM uint8_t RDAYCP0;        /*!< (@ 0x0000005A) Date Capture Register 0                                    */

            struct
            {
                __IM uint8_t DATE1  : 4; /*!< [3..0] 1-Day Capture                                                      */
                __IM uint8_t DATE10 : 2; /*!< [5..4] 10-Day Capture                                                     */
                uint8_t             : 2;
            } RDAYCP0_b;
        };
        __IM uint8_t BCNT3CP0;           /*!< (@ 0x0000005A) BCNT3 Capture Register 0                                   */
    };
    __IM uint8_t RESERVED27;

    union
    {
        union
        {
            __IM uint8_t RDAYCP1;        /*!< (@ 0x0000005C) Date Capture Register 1                                    */

            struct
            {
                __IM uint8_t DATE1  : 4; /*!< [3..0] 1-Day Capture                                                      */
                __IM uint8_t DATE10 : 2; /*!< [5..4] 10-Day Capture                                                     */
                uint8_t             : 2;
            } RDAYCP1_b;
        };
        __IM uint8_t BCNT3CP1;           /*!< (@ 0x0000005C) BCNT3 Capture Register 1                                   */

        union
        {
            __IM uint8_t RMONCP0;        /*!< (@ 0x0000005C) Month Capture Register 0                                   */

            struct
            {
                __IM uint8_t MON1  : 4;  /*!< [3..0] 1-Month Capture                                                    */
                __IM uint8_t MON10 : 1;  /*!< [4..4] 10-Month Capture                                                   */
                uint8_t            : 3;
            } RMONCP0_b;
        };
    };
    __IM uint8_t RESERVED28;

    union
    {
        union
        {
            __IM uint8_t RDAYCP2;        /*!< (@ 0x0000005E) Date Capture Register 2                                    */

            struct
            {
                __IM uint8_t DATE1  : 4; /*!< [3..0] 1-Day Capture                                                      */
                __IM uint8_t DATE10 : 2; /*!< [5..4] 10-Day Capture                                                     */
                uint8_t             : 2;
            } RDAYCP2_b;
        };
        __IM uint8_t BCNT3CP2;           /*!< (@ 0x0000005E) BCNT3 Capture Register 2                                   */

        union
        {
            __IM uint8_t RMONCP1;        /*!< (@ 0x0000005E) Month Capture Register 1                                   */

            struct
            {
                __IM uint8_t MON1  : 4;  /*!< [3..0] 1-Month Capture                                                    */
                __IM uint8_t MON10 : 1;  /*!< [4..4] 10-Month Capture                                                   */
                uint8_t            : 3;
            } RMONCP1_b;
        };
    };
    __IM uint8_t RESERVED29;

    union
    {
        __IM uint8_t RMONCP2;          /*!< (@ 0x00000060) Month Capture Register 2                                   */

        struct
        {
            __IM uint8_t MON1  : 4;    /*!< [3..0] 1-Month Capture                                                    */
            __IM uint8_t MON10 : 1;    /*!< [4..4] 10-Month Capture                                                   */
            uint8_t            : 3;
        } RMONCP2_b;
    };
    __IM uint8_t  RESERVED30;
    __IM uint16_t RESERVED31;
} R_RTC_Type;                          /*!< Size = 100 (0x64)                                                         */

/** @} */ /* End of group Device_Peripheral_peripherals */

#endif /* R_RTC_REG_H */

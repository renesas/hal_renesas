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

#ifndef BSP_ELC_H
#define BSP_ELC_H

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_RX74M
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define ELC_PERIPHERAL_MASK_SIZE    (64)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern uint64_t elc_peripheral_mask[];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

typedef enum e_elc_event_rx74m
{
    ELC_EVENT_NONE                    = (0x00),  // Link disabled
    ELC_EVENT_DMAC0_INT               = (0x21),  // DMAC0 transfer end
    ELC_EVENT_DMAC1_INT               = (0x22),  // DMAC1 transfer end
    ELC_EVENT_DMAC2_INT               = (0x23),  // DMAC2 transfer end
    ELC_EVENT_DMAC3_INT               = (0x24),  // DMAC3 transfer end
    ELC_EVENT_DMAC74_INT_4            = (0x25),  // DMAC4 transfer end
    ELC_EVENT_DMAC74_INT_5            = (0x26),  // DMAC5 transfer end
    ELC_EVENT_DMAC74_INT_6            = (0x27),  // DMAC6 transfer end
    ELC_EVENT_DMAC74_INT_7            = (0x28),  // DMAC7 transfer end
    ELC_EVENT_DTC_END                 = (0x31),  // DTC transfer end
    ELC_EVENT_PVD_PVD1                = (0x33),  // Voltage monitor 1 interrupt
    ELC_EVENT_PVD_PVD2                = (0x34),  // Voltage monitor 2 interrupt
    ELC_EVENT_CGC_MOSTD_STOP          = (0x38),  // Main Clock oscillation stop
    ELC_EVENT_IWDT_IWUNI              = (0x4C),  // IWDT underflow
    ELC_EVENT_WDT0_WUNI               = (0x4D),  // WDT0 underflow
    ELC_EVENT_RTC_PERIOD              = (0x4F),  // Periodic interrupt
    ELC_EVENT_ELC_SOFTWARE_EVENT_0    = (0x50),  // Software event 0
    ELC_EVENT_ELC_SOFTWARE_EVENT_1    = (0x51),  // Software event 1
    ELC_EVENT_ELC_SOFTWARE_EVENT_2    = (0x52),  // Software event 2
    ELC_EVENT_ELC_SOFTWARE_EVENT_3    = (0x53),  // Software event 3
    ELC_EVENT_IOPORT_EVENT_B          = (0x54),  // Port B event
    ELC_EVENT_IOPORT_EVENT_C          = (0x55),  // Port C event
    ELC_EVENT_IOPORT_EVENT_D          = (0x56),  // Port D event
    ELC_EVENT_IOPORT_EVENT_E          = (0x57),  // Port E event
    ELC_EVENT_CMT1_CMI                = (0x58),  // Compare match
    ELC_EVENT_CMTW0_CMWI              = (0x59),  // Compare match
    ELC_EVENT_TMR0_COMPARE_A          = (0x5A),  // Compare match A
    ELC_EVENT_TMR0_COMPARE_B          = (0x5B),  // Compare match B
    ELC_EVENT_TMR0_OVERFLOW           = (0x5C),  // Overflow
    ELC_EVENT_TMR1_COMPARE_A          = (0x5D),  // Compare match A
    ELC_EVENT_TMR1_COMPARE_B          = (0x5E),  // Compare match B
    ELC_EVENT_TMR1_OVERFLOW           = (0x5F),  // Overflow
    ELC_EVENT_TMR2_COMPARE_A          = (0x60),  // Compare match A
    ELC_EVENT_TMR2_COMPARE_B          = (0x61),  // Compare match B
    ELC_EVENT_TMR2_OVERFLOW           = (0x62),  // Overflow
    ELC_EVENT_TMR3_COMPARE_A          = (0x63),  // Compare match A
    ELC_EVENT_TMR3_COMPARE_B          = (0x64),  // Compare match B
    ELC_EVENT_TMR3_OVERFLOW           = (0x65),  // Overflow
    ELC_EVENT_OPS_UVW_EDGE            = (0xC9),  // UVW edge event
    ELC_EVENT_GPT0_CAPTURE_COMPARE_A  = (0xCA),  // Compare match A
    ELC_EVENT_GPT0_CAPTURE_COMPARE_B  = (0xCB),  // Compare match B
    ELC_EVENT_GPT0_COMPARE_C          = (0xCC),  // Compare match C
    ELC_EVENT_GPT0_COMPARE_D          = (0xCD),  // Compare match D
    ELC_EVENT_GPT0_COMPARE_E          = (0xCE),  // Compare match E
    ELC_EVENT_GPT0_COMPARE_F          = (0xCF),  // Compare match F
    ELC_EVENT_GPT0_COUNTER_OVERFLOW   = (0xD0),  // Overflow
    ELC_EVENT_GPT0_COUNTER_UNDERFLOW  = (0xD1),  // Underflow
    ELC_EVENT_GPT0_GTCEI0             = (0xD2),  // Period count function finish
    ELC_EVENT_GPT0_AD_TRIG_A          = (0xD4),  // A/D converter start request A
    ELC_EVENT_GPT0_AD_TRIG_B          = (0xD5),  // A/D converter start request B
    ELC_EVENT_GPT1_CAPTURE_COMPARE_A  = (0xD6),  // Compare match A
    ELC_EVENT_GPT1_CAPTURE_COMPARE_B  = (0xD7),  // Compare match B
    ELC_EVENT_GPT1_COMPARE_C          = (0xD8),  // Compare match C
    ELC_EVENT_GPT1_COMPARE_D          = (0xD9),  // Compare match D
    ELC_EVENT_GPT1_COMPARE_E          = (0xDA),  // Compare match E
    ELC_EVENT_GPT1_COMPARE_F          = (0xDB),  // Compare match F
    ELC_EVENT_GPT1_COUNTER_OVERFLOW   = (0xDC),  // Overflow
    ELC_EVENT_GPT1_COUNTER_UNDERFLOW  = (0xDD),  // Underflow
    ELC_EVENT_GPT1_GTCEI1             = (0xDE),  // Period count function finish
    ELC_EVENT_GPT1_AD_TRIG_A          = (0xE0),  // A/D converter start request A
    ELC_EVENT_GPT1_AD_TRIG_B          = (0xE1),  // A/D converter start request B
    ELC_EVENT_GPT2_CAPTURE_COMPARE_A  = (0xE2),  // Compare match A
    ELC_EVENT_GPT2_CAPTURE_COMPARE_B  = (0xE3),  // Compare match B
    ELC_EVENT_GPT2_COMPARE_C          = (0xE4),  // Compare match C
    ELC_EVENT_GPT2_COMPARE_D          = (0xE5),  // Compare match D
    ELC_EVENT_GPT2_COMPARE_E          = (0xE6),  // Compare match E
    ELC_EVENT_GPT2_COMPARE_F          = (0xE7),  // Compare match F
    ELC_EVENT_GPT2_COUNTER_OVERFLOW   = (0xE8),  // Overflow
    ELC_EVENT_GPT2_COUNTER_UNDERFLOW  = (0xE9),  // Underflow
    ELC_EVENT_GPT2_GTCEI2             = (0xEA),  // Period count function finish
    ELC_EVENT_GPT2_AD_TRIG_A          = (0xEC),  // A/D converter start request A
    ELC_EVENT_GPT2_AD_TRIG_B          = (0xED),  // A/D converter start request B
    ELC_EVENT_GPT3_CAPTURE_COMPARE_A  = (0xEE),  // Compare match A
    ELC_EVENT_GPT3_CAPTURE_COMPARE_B  = (0xEF),  // Compare match B
    ELC_EVENT_GPT3_COMPARE_C          = (0xF0),  // Compare match C
    ELC_EVENT_GPT3_COMPARE_D          = (0xF1),  // Compare match D
    ELC_EVENT_GPT3_COMPARE_E          = (0xF2),  // Compare match E
    ELC_EVENT_GPT3_COMPARE_F          = (0xF3),  // Compare match F
    ELC_EVENT_GPT3_COUNTER_OVERFLOW   = (0xF4),  // Overflow
    ELC_EVENT_GPT3_COUNTER_UNDERFLOW  = (0xF5),  // Underflow
    ELC_EVENT_GPT3_GTCEI3             = (0xF6),  // Period count function finish
    ELC_EVENT_GPT3_AD_TRIG_A          = (0xF8),  // A/D converter start request A
    ELC_EVENT_GPT3_AD_TRIG_B          = (0xF9),  // A/D converter start request B
    ELC_EVENT_GPT4_CAPTURE_COMPARE_A  = (0xFA),  // Compare match A
    ELC_EVENT_GPT4_CAPTURE_COMPARE_B  = (0xFB),  // Compare match B
    ELC_EVENT_GPT4_COMPARE_C          = (0xFC),  // Compare match C
    ELC_EVENT_GPT4_COMPARE_D          = (0xFD),  // Compare match D
    ELC_EVENT_GPT4_COMPARE_E          = (0xFE),  // Compare match E
    ELC_EVENT_GPT4_COMPARE_F          = (0xFF),  // Compare match F
    ELC_EVENT_GPT4_COUNTER_OVERFLOW   = (0x100), // Overflow
    ELC_EVENT_GPT4_COUNTER_UNDERFLOW  = (0x101), // Underflow
    ELC_EVENT_GPT4_GTCEI4             = (0x102), // Period count function finish
    ELC_EVENT_GPT4_AD_TRIG_A          = (0x104), // A/D converter start request A
    ELC_EVENT_GPT4_AD_TRIG_B          = (0x105), // A/D converter start request B
    ELC_EVENT_GPT5_CAPTURE_COMPARE_A  = (0x106), // Compare match A
    ELC_EVENT_GPT5_CAPTURE_COMPARE_B  = (0x107), // Compare match B
    ELC_EVENT_GPT5_COMPARE_C          = (0x108), // Compare match C
    ELC_EVENT_GPT5_COMPARE_D          = (0x109), // Compare match D
    ELC_EVENT_GPT5_COMPARE_E          = (0x10A), // Compare match E
    ELC_EVENT_GPT5_COMPARE_F          = (0x10B), // Compare match F
    ELC_EVENT_GPT5_COUNTER_OVERFLOW   = (0x10C), // Overflow
    ELC_EVENT_GPT5_COUNTER_UNDERFLOW  = (0x10D), // Underflow
    ELC_EVENT_GPT5_GTCEI5             = (0x10E), // Period count function finish
    ELC_EVENT_GPT5_AD_TRIG_A          = (0x110), // A/D converter start request A
    ELC_EVENT_GPT5_AD_TRIG_B          = (0x111), // A/D converter start request B
    ELC_EVENT_GPT6_CAPTURE_COMPARE_A  = (0x112), // Compare match A
    ELC_EVENT_GPT6_CAPTURE_COMPARE_B  = (0x113), // Compare match B
    ELC_EVENT_GPT6_COMPARE_C          = (0x114), // Compare match C
    ELC_EVENT_GPT6_COMPARE_D          = (0x115), // Compare match D
    ELC_EVENT_GPT6_COMPARE_E          = (0x116), // Compare match E
    ELC_EVENT_GPT6_COMPARE_F          = (0x117), // Compare match F
    ELC_EVENT_GPT6_COUNTER_OVERFLOW   = (0x118), // Overflow
    ELC_EVENT_GPT6_COUNTER_UNDERFLOW  = (0x119), // Underflow
    ELC_EVENT_GPT6_GTCEI6             = (0x11A), // Period count function finish
    ELC_EVENT_GPT6_AD_TRIG_A          = (0x11C), // A/D converter start request A
    ELC_EVENT_GPT6_AD_TRIG_B          = (0x11D), // A/D converter start request B
    ELC_EVENT_GPT7_CAPTURE_COMPARE_A  = (0x11E), // Compare match A
    ELC_EVENT_GPT7_CAPTURE_COMPARE_B  = (0x11F), // Compare match B
    ELC_EVENT_GPT7_COMPARE_C          = (0x120), // Compare match C
    ELC_EVENT_GPT7_COMPARE_D          = (0x121), // Compare match D
    ELC_EVENT_GPT7_COMPARE_E          = (0x122), // Compare match E
    ELC_EVENT_GPT7_COMPARE_F          = (0x123), // Compare match F
    ELC_EVENT_GPT7_COUNTER_OVERFLOW   = (0x124), // Overflow
    ELC_EVENT_GPT7_COUNTER_UNDERFLOW  = (0x125), // Underflow
    ELC_EVENT_GPT7_GTCEI7             = (0x126), // Period count function finish
    ELC_EVENT_GPT7_AD_TRIG_A          = (0x128), // A/D converter start request A
    ELC_EVENT_GPT7_AD_TRIG_B          = (0x129), // A/D converter start request B
    ELC_EVENT_GPT8_CAPTURE_COMPARE_A  = (0x12A), // Compare match A
    ELC_EVENT_GPT8_CAPTURE_COMPARE_B  = (0x12B), // Compare match B
    ELC_EVENT_GPT8_COMPARE_C          = (0x12C), // Compare match C
    ELC_EVENT_GPT8_COMPARE_D          = (0x12D), // Compare match D
    ELC_EVENT_GPT8_COMPARE_E          = (0x12E), // Compare match E
    ELC_EVENT_GPT8_COMPARE_F          = (0x12F), // Compare match F
    ELC_EVENT_GPT8_COUNTER_OVERFLOW   = (0x130), // Overflow
    ELC_EVENT_GPT8_COUNTER_UNDERFLOW  = (0x131), // Underflow
    ELC_EVENT_GPT8_GTCEI8             = (0x132), // Period count function finish
    ELC_EVENT_GPT8_AD_TRIG_A          = (0x134), // A/D converter start request A
    ELC_EVENT_GPT8_AD_TRIG_B          = (0x135), // A/D converter start request B
    ELC_EVENT_GPT9_CAPTURE_COMPARE_A  = (0x136), // Compare match A
    ELC_EVENT_GPT9_CAPTURE_COMPARE_B  = (0x137), // Compare match B
    ELC_EVENT_GPT9_COMPARE_C          = (0x138), // Compare match C
    ELC_EVENT_GPT9_COMPARE_D          = (0x139), // Compare match D
    ELC_EVENT_GPT9_COMPARE_E          = (0x13A), // Compare match E
    ELC_EVENT_GPT9_COMPARE_F          = (0x13B), // Compare match F
    ELC_EVENT_GPT9_COUNTER_OVERFLOW   = (0x13C), // Overflow
    ELC_EVENT_GPT9_COUNTER_UNDERFLOW  = (0x13D), // Underflow
    ELC_EVENT_GPT9_GTCEI9             = (0x13E), // Period count function finish
    ELC_EVENT_GPT9_AD_TRIG_A          = (0x140), // A/D converter start request A
    ELC_EVENT_GPT9_AD_TRIG_B          = (0x141), // A/D converter start request B
    ELC_EVENT_GPT10_CAPTURE_COMPARE_A = (0x142), // Compare match A
    ELC_EVENT_GPT10_CAPTURE_COMPARE_B = (0x143), // Compare match B
    ELC_EVENT_GPT10_COMPARE_C         = (0x144), // Compare match C
    ELC_EVENT_GPT10_COMPARE_D         = (0x145), // Compare match D
    ELC_EVENT_GPT10_COMPARE_E         = (0x146), // Compare match E
    ELC_EVENT_GPT10_COMPARE_F         = (0x147), // Compare match F
    ELC_EVENT_GPT10_COUNTER_OVERFLOW  = (0x148), // Overflow
    ELC_EVENT_GPT10_COUNTER_UNDERFLOW = (0x149), // Underflow
    ELC_EVENT_GPT10_GTCEI10           = (0x14A), // Period count function finish
    ELC_EVENT_GPT10_AD_TRIG_A         = (0x14C), // A/D converter start request A
    ELC_EVENT_GPT10_AD_TRIG_B         = (0x14D), // A/D converter start request B
    ELC_EVENT_GPT11_CAPTURE_COMPARE_A = (0x14E), // Compare match A
    ELC_EVENT_GPT11_CAPTURE_COMPARE_B = (0x14F), // Compare match B
    ELC_EVENT_GPT11_COMPARE_C         = (0x150), // Compare match C
    ELC_EVENT_GPT11_COMPARE_D         = (0x151), // Compare match D
    ELC_EVENT_GPT11_COMPARE_E         = (0x152), // Compare match E
    ELC_EVENT_GPT11_COMPARE_F         = (0x153), // Compare match F
    ELC_EVENT_GPT11_COUNTER_OVERFLOW  = (0x154), // Overflow
    ELC_EVENT_GPT11_COUNTER_UNDERFLOW = (0x155), // Underflow
    ELC_EVENT_GPT11_GTCEI11           = (0x156), // Period count function finish
    ELC_EVENT_GPT11_AD_TRIG_A         = (0x158), // A/D converter start request A
    ELC_EVENT_GPT11_AD_TRIG_B         = (0x159), // A/D converter start request B
    ELC_EVENT_GPT12_CAPTURE_COMPARE_A = (0x15A), // Compare match A
    ELC_EVENT_GPT12_CAPTURE_COMPARE_B = (0x15B), // Compare match B
    ELC_EVENT_GPT12_COMPARE_C         = (0x15C), // Compare match C
    ELC_EVENT_GPT12_COMPARE_D         = (0x15D), // Compare match D
    ELC_EVENT_GPT12_COMPARE_E         = (0x15E), // Compare match E
    ELC_EVENT_GPT12_COMPARE_F         = (0x15F), // Compare match F
    ELC_EVENT_GPT12_COUNTER_OVERFLOW  = (0x160), // Overflow
    ELC_EVENT_GPT12_COUNTER_UNDERFLOW = (0x161), // Underflow
    ELC_EVENT_GPT12_GTCEI12           = (0x162), // Period count function finish
    ELC_EVENT_GPT12_AD_TRIG_A         = (0x164), // A/D converter start request A
    ELC_EVENT_GPT12_AD_TRIG_B         = (0x165), // A/D converter start request B
    ELC_EVENT_GPT13_CAPTURE_COMPARE_A = (0x166), // Compare match A
    ELC_EVENT_GPT13_CAPTURE_COMPARE_B = (0x167), // Compare match B
    ELC_EVENT_GPT13_COMPARE_C         = (0x168), // Compare match C
    ELC_EVENT_GPT13_COMPARE_D         = (0x169), // Compare match D
    ELC_EVENT_GPT13_COMPARE_E         = (0x16A), // Compare match E
    ELC_EVENT_GPT13_COMPARE_F         = (0x16B), // Compare match F
    ELC_EVENT_GPT13_COUNTER_OVERFLOW  = (0x16C), // Overflow
    ELC_EVENT_GPT13_COUNTER_UNDERFLOW = (0x16D), // Underflow
    ELC_EVENT_GPT13_GTCEI13           = (0x16E), // Period count function finish
    ELC_EVENT_GPT13_AD_TRIG_A         = (0x170), // A/D converter start request A
    ELC_EVENT_GPT13_AD_TRIG_B         = (0x171), // A/D converter start request B
    ELC_EVENT_GPT14_CAPTURE_COMPARE_A = (0x172), // Compare match A
    ELC_EVENT_GPT14_CAPTURE_COMPARE_B = (0x173), // Compare match B
    ELC_EVENT_GPT14_COMPARE_C         = (0x174), // Compare match C
    ELC_EVENT_GPT14_COMPARE_D         = (0x175), // Compare match D
    ELC_EVENT_GPT14_COMPARE_E         = (0x176), // Compare match E
    ELC_EVENT_GPT14_COMPARE_F         = (0x177), // Compare match F
    ELC_EVENT_GPT14_COUNTER_OVERFLOW  = (0x178), // Overflow
    ELC_EVENT_GPT14_COUNTER_UNDERFLOW = (0x179), // Underflow
    ELC_EVENT_GPT14_GTCEI14           = (0x17A), // Period count function finish
    ELC_EVENT_GPT14_AD_TRIG_A         = (0x17C), // A/D converter start request A
    ELC_EVENT_GPT14_AD_TRIG_B         = (0x17D), // A/D converter start request B
    ELC_EVENT_GPT15_CAPTURE_COMPARE_A = (0x17E), // Compare match A
    ELC_EVENT_GPT15_CAPTURE_COMPARE_B = (0x17F), // Compare match B
    ELC_EVENT_GPT15_COMPARE_C         = (0x180), // Compare match C
    ELC_EVENT_GPT15_COMPARE_D         = (0x181), // Compare match D
    ELC_EVENT_GPT15_COMPARE_E         = (0x182), // Compare match E
    ELC_EVENT_GPT15_COMPARE_F         = (0x183), // Compare match F
    ELC_EVENT_GPT15_COUNTER_OVERFLOW  = (0x184), // Overflow
    ELC_EVENT_GPT15_COUNTER_UNDERFLOW = (0x185), // Underflow
    ELC_EVENT_GPT15_GTCEI15           = (0x186), // Period count function finish
    ELC_EVENT_GPT15_AD_TRIG_A         = (0x188), // A/D converter start request A
    ELC_EVENT_GPT15_AD_TRIG_B         = (0x189), // A/D converter start request B
    ELC_EVENT_GPTP_PTPOUT0            = (0x259), // PTP Pulse Out 0
    ELC_EVENT_GPTP_PTPOUT1            = (0x25A), // PTP Pulse Out 1
    ELC_EVENT_GPTP_PTPOUT2            = (0x25B), // PTP Pulse Out 2
    ELC_EVENT_GPTP_PTPOUT3            = (0x25C), // PTP Pulse Out 3
    ELC_EVENT_GPTP0_MATCH             = (0x25D), // Media Clock Recovery Match
    ELC_EVENT_GPTP1_MATCH             = (0x25E), // Media Clock Recovery Match
    ELC_EVENT_ESC_SYNC0               = (0x26B), // Sync0 Interrupt
    ELC_EVENT_ESC_SYNC1               = (0x26C), // Sync1 Interrupt
    ELC_EVENT_RSCI0_RXI               = (0x26D), // Receive data full
    ELC_EVENT_RSCI0_TXI               = (0x26E), // Transmit data empty
    ELC_EVENT_RSCI0_TEI               = (0x26F), // Transmit end
    ELC_EVENT_RSCI0_ERI               = (0x270), // Receive error
    ELC_EVENT_RSCI0_AED               = (0x271), // Active edge detection
    ELC_EVENT_RSCI0_RX_DCMPM          = (0x272), // Data compare match
    ELC_EVENT_RSCI0_RX_DCMPUM         = (0x273), // Data compare mismatch
    ELC_EVENT_RSCI1_RXI               = (0x274), // Receive data full
    ELC_EVENT_RSCI1_TXI               = (0x275), // Transmit data empty
    ELC_EVENT_RSCI1_TEI               = (0x276), // Transmit end
    ELC_EVENT_RSCI1_ERI               = (0x277), // Receive error
    ELC_EVENT_RSCI1_AED               = (0x278), // Active edge detection
    ELC_EVENT_RSCI1_RX_DCMPM          = (0x279), // Data compare match
    ELC_EVENT_RSCI1_RX_DCMPUM         = (0x27A), // Data compare mismatch
    ELC_EVENT_RSCI2_RXI               = (0x27B), // Receive data full
    ELC_EVENT_RSCI2_TXI               = (0x27C), // Transmit data empty
    ELC_EVENT_RSCI2_TEI               = (0x27D), // Transmit end
    ELC_EVENT_RSCI2_ERI               = (0x27E), // Receive error
    ELC_EVENT_RSCI2_AED               = (0x27F), // Active edge detection
    ELC_EVENT_RSCI2_RX_DCMPM          = (0x280), // Data compare match
    ELC_EVENT_RSCI2_RX_DCMPUM         = (0x281), // Data compare mismatch
    ELC_EVENT_RSCI3_RXI               = (0x282), // Receive data full
    ELC_EVENT_RSCI3_TXI               = (0x283), // Transmit data empty
    ELC_EVENT_RSCI3_TEI               = (0x284), // Transmit end
    ELC_EVENT_RSCI3_ERI               = (0x285), // Receive error
    ELC_EVENT_RSCI3_AED               = (0x286), // Active edge detection
    ELC_EVENT_RSCI3_RX_DCMPM          = (0x287), // Data compare match
    ELC_EVENT_RSCI3_RX_DCMPUM         = (0x288), // Data compare mismatch
    ELC_EVENT_RSCI4_RXI               = (0x289), // Receive data full
    ELC_EVENT_RSCI4_TXI               = (0x28A), // Transmit data empty
    ELC_EVENT_RSCI4_TEI               = (0x28B), // Transmit end
    ELC_EVENT_RSCI4_ERI               = (0x28C), // Receive error
    ELC_EVENT_RSCI4_AED               = (0x28D), // Active edge detection
    ELC_EVENT_RSCI4_RX_DCMPM          = (0x28E), // Data compare match
    ELC_EVENT_RSCI4_RX_DCMPUM         = (0x28F), // Data compare mismatch
    ELC_EVENT_RSCI5_RXI               = (0x290), // Receive data full
    ELC_EVENT_RSCI5_TXI               = (0x291), // Transmit data empty
    ELC_EVENT_RSCI5_TEI               = (0x292), // Transmit end
    ELC_EVENT_RSCI5_ERI               = (0x293), // Receive error
    ELC_EVENT_RSCI5_AED               = (0x294), // Active edge detection
    ELC_EVENT_RSCI5_RX_DCMPM          = (0x295), // Data compare match
    ELC_EVENT_RSCI5_RX_DCMPUM         = (0x296), // Data compare mismatch
    ELC_EVENT_RSCI6_RXI               = (0x297), // Receive data full
    ELC_EVENT_RSCI6_TXI               = (0x298), // Transmit data empty
    ELC_EVENT_RSCI6_TEI               = (0x299), // Transmit end
    ELC_EVENT_RSCI6_ERI               = (0x29A), // Receive error
    ELC_EVENT_RSCI6_AED               = (0x29B), // Active edge detection
    ELC_EVENT_RSCI6_RX_DCMPM          = (0x29C), // Data compare match
    ELC_EVENT_RSCI6_RX_DCMPUM         = (0x29D), // Data compare mismatch
    ELC_EVENT_RSCI7_RXI               = (0x29E), // Receive data full
    ELC_EVENT_RSCI7_TXI               = (0x29F), // Transmit data empty
    ELC_EVENT_RSCI7_TEI               = (0x2A0), // Transmit end
    ELC_EVENT_RSCI7_ERI               = (0x2A1), // Receive error
    ELC_EVENT_RSCI7_AED               = (0x2A2), // Active edge detection
    ELC_EVENT_RSCI7_RX_DCMPM          = (0x2A3), // Data compare match
    ELC_EVENT_RSCI7_RX_DCMPUM         = (0x2A4), // Data compare mismatch
    ELC_EVENT_RSCI8_RXI               = (0x2A5), // Receive data full
    ELC_EVENT_RSCI8_TXI               = (0x2A6), // Transmit data empty
    ELC_EVENT_RSCI8_TEI               = (0x2A7), // Transmit end
    ELC_EVENT_RSCI8_ERI               = (0x2A8), // Receive error
    ELC_EVENT_RSCI8_AED               = (0x2A9), // Active edge detection
    ELC_EVENT_RSCI8_RX_DCMPM          = (0x2AA), // Data compare match
    ELC_EVENT_RSCI8_RX_DCMPUM         = (0x2AB), // Data compare mismatch
    ELC_EVENT_RSCI9_RXI               = (0x2AC), // Receive data full
    ELC_EVENT_RSCI9_TXI               = (0x2AD), // Transmit data empty
    ELC_EVENT_RSCI9_TEI               = (0x2AE), // Transmit end
    ELC_EVENT_RSCI9_ERI               = (0x2AF), // Receive error
    ELC_EVENT_RSCI9_AED               = (0x2B0), // Active edge detection
    ELC_EVENT_RSCI9_RX_DCMPM          = (0x2B1), // Data compare match
    ELC_EVENT_RSCI9_RX_DCMPUM         = (0x2B2), // Data compare mismatch
    ELC_EVENT_RSCI10_RXI              = (0x2B3), // Receive data full
    ELC_EVENT_RSCI10_TXI              = (0x2B4), // Transmit data empty
    ELC_EVENT_RSCI10_TEI              = (0x2B5), // Transmit end
    ELC_EVENT_RSCI10_ERI              = (0x2B6), // Receive error
    ELC_EVENT_RSCI10_AED              = (0x2B7), // Active edge detection
    ELC_EVENT_RSCI10_RX_DCMPM         = (0x2B8), // Data compare match
    ELC_EVENT_RSCI10_RX_DCMPUM        = (0x2B9), // Data compare mismatch
    ELC_EVENT_RSCI11_RXI              = (0x2BA), // Receive data full
    ELC_EVENT_RSCI11_TXI              = (0x2BB), // Transmit data empty
    ELC_EVENT_RSCI11_TEI              = (0x2BC), // Transmit end
    ELC_EVENT_RSCI11_ERI              = (0x2BD), // Receive error
    ELC_EVENT_RSCI11_AED              = (0x2BE), // Active edge detection
    ELC_EVENT_RSCI11_RX_DCMPM         = (0x2BF), // Data compare match
    ELC_EVENT_RSCI11_RX_DCMPUM        = (0x2C0), // Data compare mismatch
    ELC_EVENT_RSCI12_RXI              = (0x2C1), // Receive data full
    ELC_EVENT_RSCI12_TXI              = (0x2C2), // Transmit data empty
    ELC_EVENT_RSCI12_TEI              = (0x2C3), // Transmit end
    ELC_EVENT_RSCI12_ERI              = (0x2C4), // Receive error
    ELC_EVENT_RSCI12_AED              = (0x2C5), // Active edge detection
    ELC_EVENT_RSCI12_RX_DCMPM         = (0x2C6), // Data compare match
    ELC_EVENT_RSCI12_RX_DCMPUM        = (0x2C7), // Data compare mismatch
    ELC_EVENT_RSPI0_SPRI              = (0x2C8), // Receive buffer full
    ELC_EVENT_RSPI0_SPTI              = (0x2C9), // Transmit buffer empty
    ELC_EVENT_RSPI0_IDLE              = (0x2CA), // Idle
    ELC_EVENT_RSPI0_ERI               = (0x2CB), // Error
    ELC_EVENT_RSPI0_SPCI              = (0x2CC), // Transmission complete event
    ELC_EVENT_RSPI1_SPRI              = (0x2CD), // Receive buffer full
    ELC_EVENT_RSPI1_SPTI              = (0x2CE), // Transmit buffer empty
    ELC_EVENT_RSPI1_IDLE              = (0x2CF), // Idle
    ELC_EVENT_RSPI1_ERI               = (0x2D0), // Error
    ELC_EVENT_RSPI1_SPCI              = (0x2D1), // Transmission complete event
    ELC_EVENT_RSPI2_SPRI              = (0x2D2), // Receive buffer full
    ELC_EVENT_RSPI2_SPTI              = (0x2D3), // Transmit buffer empty
    ELC_EVENT_RSPI2_IDLE              = (0x2D4), // Idle
    ELC_EVENT_RSPI2_ERI               = (0x2D5), // Error
    ELC_EVENT_RSPI2_SPCI              = (0x2D6), // Transmission complete event
    ELC_EVENT_RIIC1_RXI               = (0x2DB), // Receive data full
    ELC_EVENT_RIIC1_TXI               = (0x2DC), // Transmit data empty
    ELC_EVENT_RIIC1_TEI               = (0x2DD), // Transmit end
    ELC_EVENT_RIIC1_EEI               = (0x2DE), // Error event
    ELC_EVENT_RIIC2_RXI               = (0x2DF), // Receive data full
    ELC_EVENT_RIIC2_TXI               = (0x2E0), // Transmit data empty
    ELC_EVENT_RIIC2_TEI               = (0x2E1), // Transmit end
    ELC_EVENT_RIIC2_EEI               = (0x2E2), // Error event
    ELC_EVENT_RI3C0_RESPI             = (0x2E3), // Response status buffer full
    ELC_EVENT_RI3C0_CMDI              = (0x2E4), // Command buffer empty
    ELC_EVENT_RI3C0_IBII              = (0x2E5), // IBI status buffer full
    ELC_EVENT_RIIC0_RXI               = (0x2E6), // Receive
    ELC_EVENT_RIIC0_TXI               = (0x2E7), // Transmit
    ELC_EVENT_RI3C0_RXI               = (0x2E6), // Receive
    ELC_EVENT_RI3C0_TXI               = (0x2E7), // Transmit
    ELC_EVENT_RI3C0_RCVI              = (0x2E8), // Receive status buffer full
    ELC_EVENT_RI3C0_HRESPI            = (0x2E9), // High priority response queue full
    ELC_EVENT_RI3C0_HCMDI             = (0x2EA), // High priority command queue empty
    ELC_EVENT_RI3C0_HRXI              = (0x2EB), // High priority rx data buffer full
    ELC_EVENT_RI3C0_HTXI              = (0x2EC), // High priority tx data buffer empty
    ELC_EVENT_RIIC0_TEI               = (0x2ED), // Transmit end
    ELC_EVENT_RIIC0_EEI               = (0x2EE), // Error
    ELC_EVENT_RI3C0_TEI               = (0x2ED), // Transmit end
    ELC_EVENT_RI3C0_EEI               = (0x2EE), // Error
    ELC_EVENT_RI3C0_STEV              = (0x2EF), // Synchronous timing
    ELC_EVENT_RI3C0_MREFOVF           = (0x2F0), // MREF counter overflow
    ELC_EVENT_RI3C0_MREFCPT           = (0x2F1), // MREF capture
    ELC_EVENT_RI3C0_AMEV              = (0x2F2), // Additional master-initiated bus event
    ELC_EVENT_DOC_INT                 = (0x313), // Data operation circuit interrupt
    ELC_EVENT_ADC_ADI0                = (0x314), // End of A/D scanning operation(Gr.0)
    ELC_EVENT_ADC_ADI1                = (0x315), // End of A/D scanning operation(Gr.1)
    ELC_EVENT_ADC_ADI2                = (0x316), // End of A/D scanning operation(Gr.2)
    ELC_EVENT_ADC_ADI3                = (0x317), // End of A/D scanning operation(Gr.3)
    ELC_EVENT_ADC_ADI4                = (0x318), // End of A/D scanning operation(Gr.4)
    ELC_EVENT_ADC_ADI5                = (0x319), // End of A/D scanning operation(Gr.5)
    ELC_EVENT_ADC_ADI6                = (0x31A), // End of A/D scanning operation(Gr.6)
    ELC_EVENT_ADC_ADI7                = (0x31B), // End of A/D scanning operation(Gr.7)
    ELC_EVENT_ADC_ADI8                = (0x31C), // End of A/D scanning operation(Gr.8)
    ELC_EVENT_ADC_ADI9                = (0x31D), // End of A/D scanning operation(Gr.9)
    ELC_EVENT_ADC_ADI10               = (0x31E), // End of A/D scanning operation(Gr.10)
    ELC_EVENT_ADC_ADI11               = (0x31F), // End of A/D scanning operation(Gr.11)
    ELC_EVENT_ADC_CCMPM0              = (0x324), // Composite compare match 0
    ELC_EVENT_ADC_CCMPM1              = (0x325), // Composite compare match 1
    ELC_EVENT_ADC_CCMPM2              = (0x326), // Composite compare match 2
    ELC_EVENT_ADC_CCMPM3              = (0x327), // Composite compare match 3
    ELC_EVENT_ADC_CCMPM4              = (0x328), // Composite compare match 4
    ELC_EVENT_ADC_CCMPM5              = (0x329), // Composite compare match 5
    ELC_EVENT_ADC_CCMPM6              = (0x32A), // Composite compare match 6
    ELC_EVENT_ADC_CCMPM7              = (0x32B), // Composite compare match 7
    ELC_EVENT_ADC_CCMPUM0             = (0x334), // Composite condition compare mismatch 0
    ELC_EVENT_ADC_CCMPUM1             = (0x335), // Composite condition compare mismatch 1
    ELC_EVENT_ADC_CCMPUM2             = (0x336), // Composite condition compare mismatch 2
    ELC_EVENT_ADC_CCMPUM3             = (0x337), // Composite condition compare mismatch 3
    ELC_EVENT_ADC_CCMPUM4             = (0x338), // Composite condition compare mismatch 4
    ELC_EVENT_ADC_CCMPUM5             = (0x339), // Composite condition compare mismatch 5
    ELC_EVENT_ADC_CCMPUM6             = (0x33A), // Composite condition compare mismatch 6
    ELC_EVENT_ADC_CCMPUM7             = (0x33B), // Composite condition compare mismatch 7
} elc_event_t;

#define BSP_PRV_VECT_ENUM(event, group)    (ELC_ ## event)

#define ELC_PERIPHERAL_NUM    (92U)
#define BSP_OVERRIDE_ELC_PERIPHERAL_T

/** Possible peripherals to be linked to event signals
 * @note This list is device specific.
 * */
typedef enum e_elc_peripheral
{
    ELC_PERIPHERAL_CMT1         = (0),
    ELC_PERIPHERAL_CMTW0        = (1),
    ELC_PERIPHERAL_TMR0         = (2),
    ELC_PERIPHERAL_TMR1         = (3),
    ELC_PERIPHERAL_TMR2         = (4),
    ELC_PERIPHERAL_TMR3         = (5),
    ELC_PERIPHERAL_GPT_A        = (6),
    ELC_PERIPHERAL_GPT_B        = (7),
    ELC_PERIPHERAL_GPT_C        = (8),
    ELC_PERIPHERAL_GPT_D        = (9),
    ELC_PERIPHERAL_GPT_E        = (10),
    ELC_PERIPHERAL_GPT_F        = (11),
    ELC_PERIPHERAL_GPT_G        = (12),
    ELC_PERIPHERAL_GPT_H        = (13),
    ELC_PERIPHERAL_IOPORT_B     = (14),
    ELC_PERIPHERAL_IOPORT_C     = (15),
    ELC_PERIPHERAL_IOPORT_D     = (16),
    ELC_PERIPHERAL_IOPORT_E     = (17),
    ELC_PERIPHERAL_ADC0         = (18),
    ELC_PERIPHERAL_ADC1         = (19),
    ELC_PERIPHERAL_ADC2         = (20),
    ELC_PERIPHERAL_ADC3         = (21),
    ELC_PERIPHERAL_ADC4         = (22),
    ELC_PERIPHERAL_ADC5         = (23),
    ELC_PERIPHERAL_ADC6         = (24),
    ELC_PERIPHERAL_ADC7         = (25),
    ELC_PERIPHERAL_ADC8         = (26),
    ELC_PERIPHERAL_DAC0         = (46),
    ELC_PERIPHERAL_DAC1         = (47),
    ELC_PERIPHERAL_RI3C0        = (50),
    ELC_PERIPHERAL_GPTP0        = (51),
    ELC_PERIPHERAL_GPTP1        = (52),
    ELC_PERIPHERAL_ESC0         = (53),
    ELC_PERIPHERAL_ESC1         = (54),
    ELC_PERIPHERAL_DSMIF0_CAP0  = (55),
    ELC_PERIPHERAL_DSMIF0_CAP1  = (56),
    ELC_PERIPHERAL_DSMIF0_CAP2  = (57),
    ELC_PERIPHERAL_DSMIF0_CAP3  = (58),
    ELC_PERIPHERAL_DSMIF0_CAP4  = (59),
    ELC_PERIPHERAL_DSMIF0_CAP5  = (60),
    ELC_PERIPHERAL_DSMIF0_CNT0  = (61),
    ELC_PERIPHERAL_DSMIF0_CNT1  = (62),
    ELC_PERIPHERAL_DSMIF0_CNT2  = (63),
    ELC_PERIPHERAL_DSMIF1_CAP0  = (64),
    ELC_PERIPHERAL_DSMIF1_CAP1  = (65),
    ELC_PERIPHERAL_DSMIF1_CAP2  = (66),
    ELC_PERIPHERAL_DSMIF1_CAP3  = (67),
    ELC_PERIPHERAL_DSMIF1_CAP4  = (68),
    ELC_PERIPHERAL_DSMIF1_CAP5  = (69),
    ELC_PERIPHERAL_DSMIF1_CNT0  = (70),
    ELC_PERIPHERAL_DSMIF1_CNT1  = (71),
    ELC_PERIPHERAL_DSMIF1_CNT2  = (72),
    ELC_PERIPHERAL_PCIF0_ELC_IN = (73),
    ELC_PERIPHERAL_PCIF0_PEK_IN = (74),
    ELC_PERIPHERAL_PCIF0_VLY_IN = (75),
    ELC_PERIPHERAL_PCIF1_ELC_IN = (76),
    ELC_PERIPHERAL_PCIF1_PEK_IN = (77),
    ELC_PERIPHERAL_PCIF1_VLY_IN = (78),
    ELC_PERIPHERAL_ELC1         = (88),
    ELC_PERIPHERAL_ELC2         = (89),
    ELC_PERIPHERAL_ELC3         = (90),
    ELC_PERIPHERAL_ELC4         = (91)
} elc_peripheral_t;

/** Positions of event link set registers (ELSRs) available on this MCU */
#define BSP_ELC_PERIPHERAL_MASK     (0xFFFCC00007FFFFFFU)
#define BSP_ELC_PERIPHERAL_MASK2    (0x000000000F007FFFU)

#define BSP_OVERRIDE_ELC_SOFTWARE_EVENT_T

/** Possible software events
 * @note This list is device specific.
 * */
typedef enum e_elc_software_event
{
    ELC_SOFTWARE_EVENT_0,              ///< Software event 0
    ELC_SOFTWARE_EVENT_1,              ///< Software event 1
    ELC_SOFTWARE_EVENT_2,              ///< Software event 2
    ELC_SOFTWARE_EVENT_3,              ///< Software event 3
} elc_software_event_t;

/** @} (end addtogroup BSP_MCU_RX74M) */

#endif

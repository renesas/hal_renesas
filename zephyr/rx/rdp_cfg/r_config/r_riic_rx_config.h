/***********************************************************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
 * No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
 * applicable laws, including copyright laws. 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
 * EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
 * SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO 
 * THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
 * this software. By using this software, you agree to the additional terms and conditions found by accessing the 
 * following link:
 * http://www.renesas.com/disclaimer 
 *
 * Copyright (C) 2013-2023 Renesas Electronics Corporation. All rights reserved.
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * File Name    : r_riic_rx_config.h
 * Description  : Configures the RIIC drivers
 **********************************************************************************************************************/
/***********************************************************************************************************************
 * History : DD.MM.YYYY Version  Description
 *         : 01.07.2013 1.00     First Release
 *         : 30.09.2013 1.10     Change symbol of return value and status
 *         : 08.10.2013 1.20     Modified processing for the I/O register initialization and mode transition 
 *                                when a stop condition is detected while the slave is communicating.
 *                               Modified processing for mode transition when an API function is called
 *                                while the bus is busy.
 *                               Modified processing for mode transition
 *                                when an arbitration lost occurred and the addresses do not match.
 *                               Modified incorrect slave transmission after the master reception.
 *                               Modified processing for the I/O register initialization
 *                                when generating a start condition and receiving the slave address.
 *         : 17.07.2014 1.30     Added the parameters of channel 2.
 *                               Deleted the parameters of PCLK.
 *                               Added the parameters of the port function assignment.
 *                               Changed the parameters of interrupt priority level.
 *                               Added the parameters for the time out detection.
 *         : 22.09.2014 1.40     The module is updated to measure the issue that slave communication 
 *                               is not available after an arbitration-lost occurs and the bus is locked. 
 *                                 The issue occurs when the following four conditions are all met.
 *                                    - RIIC FIT module rev. 1.30 or earlier is used.
 *                                    - RX device operates as both the master and the slave 
 *                                      in multi-master communication.
 *                                    - An arbitration-lost is detected when communicating as the master.
 *                                    - Communication other than master reception or slave reception is performed.
 *         : 14.11.2014 1.50     Added RX113 support.
 *         : 09.10.2014 1.60     Added RX71M support.
 *         : 20.10.2014 1.70     Added RX231 support.
 *         : 31.10.2015 1.80     Added RX130, RX230, RX23T support.
 *         : 04.03.2016 1.90     Added RX24T support.Changed about the pin definisions.
 *         : 01.10.2016 2.00     Added RX65N support.
 *         : 02.06.2017 2.10     Added RX24U support.
 *         : 31.08.2017 2.20     Added definitions for Channel 1.
 *         : 30.10.2017 2.30     Added RX66T support.
 *         : 30.01.2018 2.30     Remove integer literal of setting value.
 *         : 15.10.2018 2.40     Added RX72T support.
 *         : 20.06.2019 2.42     Added RX23W support.
 *         : 30.07.2019 2.43     Added RX72M support.
 *         : 10.10.2019 2.44     Added RX13T support.
 *         : 22.11.2019 2.45     Added RX66N, RX72N support.
 *         : 10.03.2020 2.46     Added RX23E-A support.
 *         : 30.06.2021 2.48     Added RX671 support.
 *         : 31.07.2021 2.49     Added RX140 support.
 *         : 31.12.2021 2.50     Added RX660 support.
 *         : 31.03.2023 2.70     Added RX26T support.
 *                               Added new macros for SCL rise time and SCL fall time.
 *         : 29.05.2023 2.80     Added RX23E-B support.
 *         : 10.10.2023 2.90     Modified source code comments of RIIC_CFG_CHi_RXI_INT_PRIORITY,
 *                                RIIC_CFG_CHi_TXI_INT_PRIORITY, RIIC_CFG_CHi_EEI_INT_PRIORITY,
 *                                RIIC_CFG_CHi_TEI_INT_PRIORITY (i = 0 to 2).
 **********************************************************************************************************************/
/* Guards against multiple inclusion */
#ifndef RIIC_CONFIG_H
    #define RIIC_CONFIG_H
/***********************************************************************************************************************
 Configuration Options
 **********************************************************************************************************************/
/* SPECIFY WHETHER TO INCLUDE CODE FOR API PARAMETER CHECKING */
/* Setting to BSP_CFG_PARAM_CHECKING_ENABLE utilizes the system default setting */
/* Setting to 1 includes parameter checking; 0 compiles out parameter checking */
    #define RIIC_CFG_PARAM_CHECKING_ENABLE  (1)

/* SPECIFY CHANNELS TO INCLUDE SOFTWARE SUPPORT FOR 1=included, 0=not */
/* mcu supported channels */
/*  RX110: ch0,    ,     */
/*  RX111: ch0,    ,     */
/*  RX113: ch0,    ,     */
/*  RX130: ch0,    ,     */
/*  RX140: ch0,    ,     */
/*  RX13T: ch0,    ,     */
/*  RX230: ch0,    ,     */
/*  RX231: ch0,    ,     */
/*  RX23E-A: ch0,    ,   */
/*  RX23E-B: ch0,    ,   */
/*  RX23T: ch0,    ,     */
/*  RX23W: ch0,    ,     */
/*  RX24T: ch0,    ,     */
/*  RX24U: ch0,    ,     */
/*  RX26T: ch0,    ,     */
/*  RX64M: ch0,    , ch2 */
/*  RX65N: ch0, ch1, ch2 */
/*  RX660: ch0,    , ch2 */
/*  RX66T: ch0,    ,     */
/*  RX66N: ch0, ch1, ch2 */
/*  RX671: ch0, ch1, ch2 */
/*  RX71M: ch0,    , ch2 */
/*  RX72T: ch0,    ,     */
/*  RX72M: ch0, ch1, ch2 */
/*  RX72N: ch0, ch1, ch2 */
    #define RIIC_CFG_CH0_INCLUDED           (1)
    #define RIIC_CFG_CH1_INCLUDED           (0)
    #define RIIC_CFG_CH2_INCLUDED           (0)

/* Set RIIC bps(kbps) */
    #define RIIC_CFG_CH0_kBPS               (400)
    #define RIIC_CFG_CH1_kBPS               (400)
    #define RIIC_CFG_CH2_kBPS               (400)

/* Set SCL rise time (s) and SCL fall time (s) */
    #define RIIC_CFG_SCL100K_UP_TIME        (1000E-9)
    #define RIIC_CFG_SCL100K_DOWN_TIME      (300E-9)
    #define RIIC_CFG_SCL400K_UP_TIME        (300E-9)
    #define RIIC_CFG_SCL400K_DOWN_TIME      (300E-9)
    #define RIIC_CFG_SCL1M_UP_TIME          (120E-9)
    #define RIIC_CFG_SCL1M_DOWN_TIME        (120E-9)    

/* Set using digital filter(Selected IIC phi cycle is filtered out) */
/* 0 = not, 1 = one IIC phi, 2 = two IIC phi, 3 = three IIC phi, 4 = four IIC phi */
    #define RIIC_CFG_CH0_DIGITAL_FILTER     (2)
    #define RIIC_CFG_CH1_DIGITAL_FILTER     (2)
    #define RIIC_CFG_CH2_DIGITAL_FILTER     (2)

/* Setting to */
/* 1: includes riic port setting processing */
/* 0: compiles out riic port setting processing */
    #define RIIC_CFG_PORT_SET_PROCESSING    (1)

/* Set mode */
/* 0 = single master mode, 1 = multi master mode(Master arbitration-lost detection is enabled.) */
    #define RIIC_CFG_CH0_MASTER_MODE        (1)
    #define RIIC_CFG_CH1_MASTER_MODE        (1)
    #define RIIC_CFG_CH2_MASTER_MODE        (1)

/* Set slave address */
/* 0 = not, 1 = 7bit address format, 2 = 10bit address format */
    #define RIIC_CFG_CH0_SLV_ADDR0_FORMAT   (1)
    #define RIIC_CFG_CH0_SLV_ADDR1_FORMAT   (0)
    #define RIIC_CFG_CH0_SLV_ADDR2_FORMAT   (0)

    #define RIIC_CFG_CH0_SLV_ADDR0          (0x0025)
    #define RIIC_CFG_CH0_SLV_ADDR1          (0x0000)
    #define RIIC_CFG_CH0_SLV_ADDR2          (0x0000)

    #define RIIC_CFG_CH1_SLV_ADDR0_FORMAT   (1)
    #define RIIC_CFG_CH1_SLV_ADDR1_FORMAT   (0)
    #define RIIC_CFG_CH1_SLV_ADDR2_FORMAT   (0)

    #define RIIC_CFG_CH1_SLV_ADDR0          (0x0025)
    #define RIIC_CFG_CH1_SLV_ADDR1          (0x0000)
    #define RIIC_CFG_CH1_SLV_ADDR2          (0x0000)

    #define RIIC_CFG_CH2_SLV_ADDR0_FORMAT   (1)
    #define RIIC_CFG_CH2_SLV_ADDR1_FORMAT   (0)
    #define RIIC_CFG_CH2_SLV_ADDR2_FORMAT   (0)

    #define RIIC_CFG_CH2_SLV_ADDR0          (0x0025)
    #define RIIC_CFG_CH2_SLV_ADDR1          (0x0000)
    #define RIIC_CFG_CH2_SLV_ADDR2          (0x0000)

/* Select General call address */
/* 0 = not use, 1 = use(General call address detection is enabled.) */
    #define RIIC_CFG_CH0_SLV_GCA_ENABLE     (0)
    #define RIIC_CFG_CH1_SLV_GCA_ENABLE     (0)
    #define RIIC_CFG_CH2_SLV_GCA_ENABLE     (0)

/* This #define sets the priority level for the riic interrupt */
/* 1 lowest, 15 highest */
/* The following devices can not individually specify the interrupt priority level for EEI0, TEI0, EEI2, TEI2. */
/* EEI and TEI interrupts are grouped as the BL1 interrupt in the RX26T, RX64M, RX65N, RX660, RX66N, RX66T, RX671,
   RX71M, RX72M, RX72N, and RX72T group. */
    #define RIIC_CFG_CH0_RXI_INT_PRIORITY   (1)
    #define RIIC_CFG_CH0_TXI_INT_PRIORITY   (1)
/* The priority level of the EEI, please do not lower than the priority level of TXI and RXI.
   For devices with EEI assigned to group interrupts, set the priority level of the EEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH0_EEI_INT_PRIORITY   (1)
/* The priority level of the TEI, please do not lower than the priority level of TXI and RXI.
   For devices with TEI assigned to group interrupts, set the priority level of the TEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH0_TEI_INT_PRIORITY   (1)

    #define RIIC_CFG_CH1_RXI_INT_PRIORITY   (1)
    #define RIIC_CFG_CH1_TXI_INT_PRIORITY   (1)
/* The priority level of the EEI, please do not lower than the priority level of TXI and RXI.
   For devices with EEI assigned to group interrupts, set the priority level of the EEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH1_EEI_INT_PRIORITY   (1)
/* The priority level of the TEI, please do not lower than the priority level of TXI and RXI.
   For devices with TEI assigned to group interrupts, set the priority level of the TEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH1_TEI_INT_PRIORITY   (1)

    #define RIIC_CFG_CH2_RXI_INT_PRIORITY   (1)
    #define RIIC_CFG_CH2_TXI_INT_PRIORITY   (1)
/* The priority level of the EEI, please do not lower than the priority level of TXI and RXI.
   For devices with EEI assigned to group interrupts, set the priority level of the EEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH2_EEI_INT_PRIORITY   (1)
/* The priority level of the TEI, please do not lower than the priority level of TXI and RXI.
   For devices with TEI assigned to group interrupts, set the priority level of the TEI higher than the priority level of TXI and RXI. */
    #define RIIC_CFG_CH2_TEI_INT_PRIORITY   (1)

/* Select Timeout function enable or disable */
/* 0 = disable, 1 = enable */
    #define RIIC_CFG_CH0_TMO_ENABLE         (1)
    #define RIIC_CFG_CH1_TMO_ENABLE         (1)
    #define RIIC_CFG_CH2_TMO_ENABLE         (1)

/* Select long mode or short mode for the timeout detection time */
/*    when the timeout function is enabled. */
/* 0 = Long mode, 1 = short mode */
    #define RIIC_CFG_CH0_TMO_DET_TIME       (0)
    #define RIIC_CFG_CH1_TMO_DET_TIME       (0)
    #define RIIC_CFG_CH2_TMO_DET_TIME       (0)

/* Select enable or disable the internal counter of the timeout function to count up while the */
/* SCL line is held LOW when the timeout function is enabled. */
/* 0 = Count is disabled, 1 = Count is enabled */
    #define RIIC_CFG_CH0_TMO_LCNT           (1)
    #define RIIC_CFG_CH1_TMO_LCNT           (1)
    #define RIIC_CFG_CH2_TMO_LCNT           (1)

/* Select enable or disable the internal counter of the timeout function to count up while the */
/* SCL line is held HIGH when the timeout function is enabled. */
/* 0 = Count is disabled, 1 = Count is enabled */
    #define RIIC_CFG_CH0_TMO_HCNT           (1)
    #define RIIC_CFG_CH1_TMO_HCNT           (1)
    #define RIIC_CFG_CH2_TMO_HCNT           (1)

/* Define software bus busy check counter. */
    #define RIIC_CFG_BUS_CHECK_COUNTER      (1000)     /* Counter of checking bus busy */

#endif /* RIIC_CONFIG_H */


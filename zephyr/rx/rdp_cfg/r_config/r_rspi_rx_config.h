/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name     : r_rspi_rx_config.h
* Description   : Configures the RSPI drivers
************************************************************************************************************************
* History : DD.MM.YYYY Version Description
*         : 25.10.2013 1.00     First Release
*         : 31.07.2017 1.70     Supported RX65N-2MB and RX130N-512KB.
*         : 20.09.2018 1.80     Supported RX66T.
*         : 20.12.2018 2.00     Added double buffer and dmadtc transfer mode.
*         : 31.03.2023 3.10     Supported RX26T.
*                               Update copyright year follow new rule.
*         : 15.03.2025 3.51     Updated disclaimer.
***********************************************************************************************************************/
#ifndef RSPI_CONFIG_HEADER_FILE
#define RSPI_CONFIG_HEADER_FILE

/***********************************************************************************************************************
Configuration Options
***********************************************************************************************************************/
/* Checking of arguments passed to RSPI API functions can be enable or disabled.
 * Disabling argument checking is provided for systems that absolutely require faster and smaller code.
 * By default the module is configured to use the setting of the system-wide BSP_CFG_PARAM_CHECKING_ENABLE macro.
 * This can be overridden for the local module by redefining RSPI_CFG_PARAM_CHECKING_ENABLE.
 * To control parameter checking locally, set RSPI_CFG_PARAM_CHECKING_ENABLE to 1 to enable it,
 * otherwise set to 0 skip checking.
 */
#define RSPI_CFG_PARAM_CHECKING_ENABLE  (BSP_CFG_PARAM_CHECKING_ENABLE)

/* If this is defined, then High Speed Read Operation is valid.
   RSPI can speed up the consecutive reception with the function of receive
   buffer. If the MCU does not have RSPCK Auto-Stop Function, this module
   disables interrupts during the High Speed Read Operation. */
#define RSPI_CFG_HIGH_SPEED_READ (CONFIG_SPI_RENESAS_RX_HIGH_SPEED_READ)

#define RSPI_CFG_REQUIRE_LOCK (0)

/* Fixed data that is transmitted during receive-only operations. Change as needed. */
#define RSPI_CFG_DUMMY_TXDATA (0xFFFFFFFF)

/* Enable the RSPI channels to use in this build. (0) = not used. (1) = used. */
#define RSPI_CFG_USE_CHAN0  (DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(rspi0)))
#define RSPI_CFG_USE_CHAN1  (DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(rspi1)))
#define RSPI_CFG_USE_CHAN2  (DT_NODE_HAS_STATUS_OKAY(DT_NODELABEL(rspi2)))

/* Set interrupt priority levels for each channel present.
 * Priority is shared by all interrupt sources in a channel.
 * Values must be in the range 0 (interrupt disabled) to 15 (highest)*/
#define RSPI_CFG_IR_PRIORITY_CHAN0 (3)
#define RSPI_CFG_IR_PRIORITY_CHAN1 (3)
#define RSPI_CFG_IR_PRIORITY_CHAN2 (3)

/******************************************************************************
ENABLE DEBUGGING INFORMATION
******************************************************************************/
/* Stores error log information using LONGQ driver. */
#define RSPI_CFG_LONGQ_ENABLE  (0)


/* When  reading the RSPI received data register for data frame bit lengths
 * that are not 8, 16, or 32-bits the unused upper bit will contain residual
 * values from the transmit data. As a convenience, these unused upper bits
 * may be optionally masked off (cleared to 0) by the driver when the data
 * is transferred to the user-data buffer.
 * Since this takes additional processing time in the data transfer interrupt handler
 * it will cause a reduction in performance for the highest bit rate settings.
 * This is not needed for 8, 16, or 32-bit transfers. So only enable this option
 * when using data frame bit lengths that are not 8, 16, or 32-bits. */
#define RSPI_CFG_MASK_UNUSED_BITS (0)

/* For RX63 group MCUs the RSPI error interrupt is a group interrupt shared with the SCI peripheral
 * So this is disabled by default for RX63 group to prevent conflict with SCI module. If not using the SCI module
 * This may be enabled by setting RSPI_CFG_USE_RX63_ERROR_INTERRUPT to (1).
 */
#define RSPI_CFG_USE_RX63_ERROR_INTERRUPT (0)
/**********************************************************************************************************************/
#endif /* RSPI_CONFIG_HEADER_FILE */

/***********************************************************************************************************************
* Copyright (c) 2013 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_mpc_rx_if.h
* Description  : Multi-Function Pin Controller (MPC) driver. This module uses pin definitions from the r_gpio_rx module
*                to allow users to easily control pin functionality.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History : DD.MM.YYYY Version Description           
*         : 17.07.2013 1.00    First Release
*         : 24.04.2014 1.20    Replaced inclusion of stdint.h and stdbool.h with platform.h
*         : 13.06.2014 1.30    Added support for RX64M
*         : 04.12.2014 1.40    Added support for RX113
*         : 02.19.2015 1.50    Added support for RX71M
*         : 29.04.2015 1.60    Added support for RX231
*         : 30.09.2015 1.70    Added support for RX23T
*         : 01.10.2015 1.80    Added support for RX130
*         : 01.12.2015 1.90    Added support for RX24T
*         : 01.02.2016 2.00    Added support for RX230
*         : 15.03.2016 2.10    Added support for RX65N
*         : 19.12.2016 2.20    Added support for RX24U, RX24T(512KB)
*         : 21.07.2017 2.30    Added support for the RX65N-2M, RX130-512KB.
*         : 31.10.2017 2.31    Added the demo for RX65N, RX65N-2M
*         : 28.09.2018 2.40    Added support for RX66T
*         : 16.11.2018 2.41    Added XML document number
*         : 01.02.2019 2.50    Added support for RX72T, RX65N-64pin
*         : 20.05.2019 3.00    Added support for GNUC and ICCRX.
*         : 28.06.2019 3.10    Added support RX23W
*         : 15.08.2019 3.20    Added support RX72M
*         : 25.11.2019 3.30    Added support RX13T
*         : 30.12.2019 3.40    Added support RX72N, RX66N
*         : 31.03.2020 3.50    Added support RX23E-A
*         : 30.06.2020 3.60    Changed revision to reflect demo upgrade.
*         : 01.04.2021 3.70    Added support RX23W-83pins
*         : 01.04.2021 3.80    Added support RX72M 144pins, 100pins.
*         : 07.04.2021 3.90    Added support RX671.
*         : 15.04.2021 4.00    Added support RX140.
*         : 13.09.2021 4.10    Added the demo for RX671.
*         : 11.11.2021 4.20    Added support for RX140-256KB.
*         : 14.03.2022 4.30    Added support for RX66T 48pins.
*         : 31.03.2022 4.40    Added support for RX660.
*         : 28.06.2022 4.50    Updated demo projects.
*         : 15.12.2022 4.60    Updated dependency module version.
*         : 28.02.2023 4.70    Updated dependency module version.
*         : 07.04.2023 4.80    Added support for RX26T.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0
*                              Updated dependency module version.
*         : 29.05.2023 4.90    Added support for RX23E-B.
*                              Fixed to comply with GSCE Coding Standards Rev.6.5.0
*         : 28.06.2024 5.00    Added support for RX260, RX261.
*         : 15.03.2025 5.01    Updated disclaimer.
***********************************************************************************************************************/

#ifndef MPC_RX_INTERFACE_HEADER_FILE
#define MPC_RX_INTERFACE_HEADER_FILE

/***********************************************************************************************************************
Includes   <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* MCU specific platform support, and C lib includes. */
#include "platform.h"
/* r_gpio_rx module is required for getting pin definitions. */
#include "r_gpio_rx_if.h"

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/

#if R_BSP_VERSION_MAJOR < 5
    #error "This module must use BSP module of Rev.5.00 or higher. Please use the BSP module of Rev.5.00 or higher."
#endif

/* Version Number of API. */
#define MPC_RX_VERSION_MAJOR           (5)
#define MPC_RX_VERSION_MINOR           (01)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
/* Options for configuring the MPC register of a pin. To find valid settings for pin_function, refer to the
 * Multi-Function Pin Controller (MPC) section of your MCU's hardware manual. Select the Pin Function Control Register
 * for the port that your pin is on. On this page you will find a table with available functions for each pin on the
 * selected port. */
typedef struct
{
    uint8_t pin_function;       // Assigns a peripheral function to this pin.
    bool    irq_enable;         // Enables a pin to be used as an IRQ pin.
    bool    analog_enable;      // Enables a pin to be used as an ADC input, DAC output, or for LVD (CMPA2)
} mpc_config_t;

/* Function return type. */
typedef enum
{
    MPC_SUCCESS = 0,
    MPC_ERR_INVALID_CFG         // The configuration specified cannot be applied to this pin
} mpc_err_t;

/***********************************************************************************************************************
Exported global functions (to be accessed by other files)
***********************************************************************************************************************/
/******************************************************************************
 * Function Name: R_MPC_Write
 * Description  : .
 * Arguments    : pin
 *              : pconfig
 * Return Value : .
 *****************************************************************************/
mpc_err_t   R_MPC_Write (gpio_port_pin_t pin, mpc_config_t * pconfig);

/******************************************************************************
 * Function Name: R_MPC_Read
 * Description  : .
 * Arguments    : pin
 *              : pconfig
 * Return Value : .
 *****************************************************************************/
void        R_MPC_Read (gpio_port_pin_t  pin, mpc_config_t * pconfig);

/******************************************************************************
 * Function Name: R_MPC_GetVersion
 * Description  : .
 * Return Value : .
 *****************************************************************************/
uint32_t    R_MPC_GetVersion (void);

#endif /* MPC_RX_INTERFACE_HEADER_FILE */



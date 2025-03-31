/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products. No 
* other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all 
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, 
* FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED. TO THE MAXIMUM 
* EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES 
* SHALL BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS 
* SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability of 
* this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2016-2024 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/
/***********************************************************************************************************************
* File Name    : r_lvd_rx_private_targets.h
* Description  : Header file for include to do a target product header.
***********************************************************************************************************************/
/***********************************************************************************************************************
* History      : DD.MM.YYYY Version  Description
*              : 15.06.2016 2.00     First Release
*              : 01.10.2016 2.10     Added support for RX65N.
*              :                     Deleted Tool-Chain version.
*              : 19.12.2016 2.20     Added support for RX24U, RX24T(512KB).
*              :                     Deleted unnecessary header information.
*              : 09.06.2017 2.30     Added support for RX130-512K, RX65N-2M.
*              : 31.10.2017 2.31     Added the demo for RX65N, RX65N-2M.
*              : 28.09.2018 2.40     Added support for RX66T.
*              : 16.11.2018 2.41     Added XML document number.
*              : 01.02.2019 2.50     Added support for RX72T, RX65N-64pin.
*              : 20.05.2019 3.00     Added support for GNUC and ICCRX.
*              : 28.06.2019 3.10     Added support for RX23W.
*              : 15.08.2019 3.20     Added support for RX72M.
*              : 25.11.2019 3.30     Added support RX13T
*                                    Removed support for Generation 1 devices.
*              : 30.12.2019 3.40     Added support for RX66N, RX72N.
*              : 31.03.2020 3.50     Added support for RX23E-A.
*              : 30.06.2020 3.60     Changed revision to reflect demo upgrade.
*              : 31.03.2021 3.70     Added support for RX671.
*              : 15.04.2021 3.80     Added support for RX140.
*              : 13.09.2021 3.90     Changed revision to reflect demo upgrade.
*              : 14.03.2022 4.00     Added support for RX66T 48pins.
*              : 31.03.2022 4.10     Added support for RX660.
*              : 28.06.2022 4.20     Updated demo projects.
*              : 12.12.2022 4.30     Fixed default register value of voltage level for RX13T/RX23E-A/RX660/RX72T/RX66T.
*                                    Fixed condition of the voltage detection level for RX130 channel 2.
*              : 31.03.2023 4.40     Added support for RX26T.
*              : 29.05.2023 4.50     Added support for RX23E-B.
*                                    Fixed to comply with GSCE Coding Standards Rev.6.5.0.
*              : 28.09.2023 4.60     Changed version for update of APN.
*                                    Fixed bug in MDF file which causes abnormal behavior in Smart Configurator when
*                                    setting the “voltage detection level” for RX23E-B.
*              : 30.01.2024 4.70     Changed LVD circuit enable wait value for RX13T/RX23E-A/RX26T/RX66T/RX72T.
*                                    Changed ICU group from LVD_GROUP_INT_ICUB to LVD_GROUP_INT_ICUC for RX66T/RX72T.
***********************************************************************************************************************/
#ifndef LVD_PRIVATE_TARGETS_HEADER_FILE
#define LVD_PRIVATE_TARGETS_HEADER_FILE

/***********************************************************************************************************************
Includes <System Includes> , "Project Includes"
***********************************************************************************************************************/
/* Includes board and MCU related header files. */
#include "platform.h"

#if defined(BSP_MCU_RX110)
    #include "./targets/rx110/r_lvd_rx110.h"
#elif defined(BSP_MCU_RX111)
    #include "./targets/rx111/r_lvd_rx111.h"
#elif defined(BSP_MCU_RX113)
    #include "./targets/rx113/r_lvd_rx113.h"
#elif defined(BSP_MCU_RX130)
    #include "./targets/rx130/r_lvd_rx130.h"
#elif defined(BSP_MCU_RX13T)
    #include "./targets/rx13t/r_lvd_rx13t.h"
#elif defined(BSP_MCU_RX140)
    #include "./targets/rx140/r_lvd_rx140.h"
#elif defined(BSP_MCU_RX23T)
    #include "./targets/rx23t/r_lvd_rx23t.h"
#elif defined(BSP_MCU_RX230)
    #include "./targets/rx230/r_lvd_rx230.h"
#elif defined(BSP_MCU_RX231)
    #include "./targets/rx231/r_lvd_rx231.h"
#elif defined(BSP_MCU_RX23E_A)
    #include "./targets/rx23e-a/r_lvd_rx23e-a.h"
#elif defined(BSP_MCU_RX23E_B)
    #include "./targets/rx23e-b/r_lvd_rx23e-b.h"
#elif defined(BSP_MCU_RX23W)
    #include "./targets/rx23w/r_lvd_rx23w.h"
#elif defined(BSP_MCU_RX24T)
    #include "./targets/rx24t/r_lvd_rx24t.h"
#elif defined(BSP_MCU_RX24U)
    #include "./targets/rx24u/r_lvd_rx24u.h"
#elif defined(BSP_MCU_RX26T)
    #include "./targets/rx26t/r_lvd_rx26t.h"
#elif defined(BSP_MCU_RX64M)
    #include "./targets/rx64m/r_lvd_rx64m.h"
#elif defined(BSP_MCU_RX65N)
    #include "./targets/rx65n/r_lvd_rx65n.h"
#elif defined(BSP_MCU_RX66N)
    #include "./targets/rx66n/r_lvd_rx66n.h"
#elif defined(BSP_MCU_RX66T)
    #include "./targets/rx66t/r_lvd_rx66t.h"
#elif defined(BSP_MCU_RX660)
    #include "./targets/rx660/r_lvd_rx660.h"
#elif defined(BSP_MCU_RX671)
    #include "./targets/rx671/r_lvd_rx671.h"
#elif defined(BSP_MCU_RX71M)
    #include "./targets/rx71m/r_lvd_rx71m.h"
#elif defined(BSP_MCU_RX72T)
    #include "./targets/rx72t/r_lvd_rx72t.h"
#elif defined(BSP_MCU_RX72M)
    #include "./targets/rx72m/r_lvd_rx72m.h"
#elif defined(BSP_MCU_RX72N)
    #include "./targets/rx72n/r_lvd_rx72n.h"
#else
    #error "This MCU is not supported by the current r_lvd_rx module."
#endif /* definedBSP_MCU_RX110 */

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
/* Version Number of API. */
#define LVD_RX_VERSION_MAJOR                    (4)
#define LVD_RX_VERSION_MINOR                    (70)

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/

/***********************************************************************************************************************
Imported global variables and functions (from other files)
***********************************************************************************************************************/

/***********************************************************************************************************************
Exported global variables and functions (to be accessed by other files)
***********************************************************************************************************************/

#endif/* LVD_PRIVATE_TARGETS_HEADER_FILE */

/* End of File */


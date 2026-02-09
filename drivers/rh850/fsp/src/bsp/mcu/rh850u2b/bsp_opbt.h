/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_OPBT_H
#define BSP_OPBT_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2BX
 * @defgroup BSP_OPBT_U2BX BSP Option Bytes
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** OPBT Configuration */
typedef const struct st_bsp_opbt_setting_cfg
{
    /* Software Configuration Option Byte (OPBT n = 0 to 31) */
    const uint32_t SW_CFG_OPBT_0_setting;            ///< Software configuration option byte 0
    const uint32_t SW_CFG_OPBT_1_setting;            ///< Software configuration option byte 1
    const uint32_t SW_CFG_OPBT_2_setting;            ///< Software configuration option byte 2
    const uint32_t SW_CFG_OPBT_3_setting;            ///< Software configuration option byte 3
    const uint32_t SW_CFG_OPBT_4_setting;            ///< Software configuration option byte 4
    const uint32_t SW_CFG_OPBT_5_setting;            ///< Software configuration option byte 5
    const uint32_t SW_CFG_OPBT_6_setting;            ///< Software configuration option byte 6
    const uint32_t SW_CFG_OPBT_7_setting;            ///< Software configuration option byte 7
    const uint32_t SW_CFG_OPBT_8_setting;            ///< Software configuration option byte 8
    const uint32_t SW_CFG_OPBT_9_setting;            ///< Software configuration option byte 9
    const uint32_t SW_CFG_OPBT_10_setting;           ///< Software configuration option byte 10
    const uint32_t SW_CFG_OPBT_11_setting;           ///< Software configuration option byte 11
    const uint32_t SW_CFG_OPBT_12_setting;           ///< Software configuration option byte 12
    const uint32_t SW_CFG_OPBT_13_setting;           ///< Software configuration option byte 13
    const uint32_t SW_CFG_OPBT_14_setting;           ///< Software configuration option byte 14
    const uint32_t SW_CFG_OPBT_15_setting;           ///< Software configuration option byte 15
    const uint32_t SW_CFG_OPBT_16_setting;           ///< Software configuration option byte 16
    const uint32_t SW_CFG_OPBT_17_setting;           ///< Software configuration option byte 17
    const uint32_t SW_CFG_OPBT_18_setting;           ///< Software configuration option byte 18
    const uint32_t SW_CFG_OPBT_19_setting;           ///< Software configuration option byte 19
    const uint32_t SW_CFG_OPBT_20_setting;           ///< Software configuration option byte 20
    const uint32_t SW_CFG_OPBT_21_setting;           ///< Software configuration option byte 21
    const uint32_t SW_CFG_OPBT_22_setting;           ///< Software configuration option byte 22
    const uint32_t SW_CFG_OPBT_23_setting;           ///< Software configuration option byte 23
    const uint32_t SW_CFG_OPBT_24_setting;           ///< Software configuration option byte 24
    const uint32_t SW_CFG_OPBT_25_setting;           ///< Software configuration option byte 25
    const uint32_t SW_CFG_OPBT_26_setting;           ///< Software configuration option byte 26
    const uint32_t SW_CFG_OPBT_27_setting;           ///< Software configuration option byte 27
    const uint32_t SW_CFG_OPBT_28_setting;           ///< Software configuration option byte 28
    const uint32_t SW_CFG_OPBT_29_setting;           ///< Software configuration option byte 29
    const uint32_t SW_CFG_OPBT_30_setting;           ///< Software configuration option byte 30
    const uint32_t SW_CFG_OPBT_31_setting;           ///< Software configuration option byte 31

    /* Reset Vector Address (PE0 to PE5) */
    const uint32_t RESET_VECTOR_ADDRESS_PE0_setting; ///< Reset vector address for PE0
    const uint32_t RESET_VECTOR_ADDRESS_PE1_setting; ///< Reset vector address for PE1
    const uint32_t RESET_VECTOR_ADDRESS_PE2_setting; ///< Reset vector address for PE2
    const uint32_t RESET_VECTOR_ADDRESS_PE3_setting; ///< Reset vector address for PE3
    const uint32_t RESET_VECTOR_ADDRESS_PE4_setting; ///< Reset vector address for PE4
    const uint32_t RESET_VECTOR_ADDRESS_PE5_setting; ///< Reset vector address for PE5

    const uint32_t Reserved_0;                       ///< Reserved field 0
    const uint32_t Reserved_1;                       ///< Reserved field 1

    /* OPBT setting values (OPBT0 to OPBT40) */
    const uint32_t OPBT_OPBT0_setting;               ///< Option byte setting 0
    const uint32_t OPBT_OPBT1_setting;               ///< Option byte setting 1
    const uint32_t OPBT_OPBT2_setting;               ///< Option byte setting 2
    const uint32_t OPBT_OPBT3_setting;               ///< Option byte setting 3
    const uint32_t OPBT_OPBT4_setting;               ///< Option byte setting 4
    const uint32_t OPBT_OPBT5_setting;               ///< Option byte setting 5
    const uint32_t OPBT_OPBT6_setting;               ///< Option byte setting 6
    const uint32_t OPBT_OPBT7_setting;               ///< Option byte setting 7
    const uint32_t OPBT_OPBT8_setting;               ///< Option byte setting 8
    const uint32_t OPBT_OPBT9_setting;               ///< Option byte setting 9
    const uint32_t OPBT_OPBT10_setting;              ///< Option byte setting 10
    const uint32_t OPBT_OPBT11_setting;              ///< Option byte setting 11
    const uint32_t OPBT_OPBT12_setting;              ///< Option byte setting 12
    const uint32_t OPBT_OPBT13_setting;              ///< Option byte setting 13
    const uint32_t OPBT_OPBT14_setting;              ///< Option byte setting 14
    const uint32_t OPBT_OPBT15_setting;              ///< Option byte setting 15
    const uint32_t OPBT_OPBT16_setting;              ///< Option byte setting 16
    const uint32_t OPBT_OPBT17_setting;              ///< Option byte setting 17
    const uint32_t OPBT_OPBT18_setting;              ///< Option byte setting 18
    const uint32_t OPBT_OPBT19_setting;              ///< Option byte setting 19
    const uint32_t OPBT_OPBT20_setting;              ///< Option byte setting 20
    const uint32_t OPBT_OPBT21_setting;              ///< Option byte setting 21
    const uint32_t OPBT_OPBT22_setting;              ///< Option byte setting 22
    const uint32_t OPBT_OPBT23_setting;              ///< Option byte setting 23
    const uint32_t OPBT_OPBT24_setting;              ///< Option byte setting 24
    const uint32_t OPBT_OPBT25_setting;              ///< Option byte setting 25
    const uint32_t OPBT_OPBT26_setting;              ///< Option byte setting 26
    const uint32_t OPBT_OPBT27_setting;              ///< Option byte setting 27
    const uint32_t OPBT_OPBT28_setting;              ///< Option byte setting 28
    const uint32_t OPBT_OPBT29_setting;              ///< Option byte setting 29
    const uint32_t OPBT_OPBT30_setting;              ///< Option byte setting 30
    const uint32_t OPBT_OPBT31_setting;              ///< Option byte setting 31
    const uint32_t OPBT_OPBT32_setting;              ///< Option byte setting 32
    const uint32_t OPBT_OPBT33_setting;              ///< Option byte setting 33
    const uint32_t OPBT_OPBT34_setting;              ///< Option byte setting 34
    const uint32_t OPBT_OPBT35_setting;              ///< Option byte setting 35
    const uint32_t OPBT_OPBT36_setting;              ///< Option byte setting 36
    const uint32_t OPBT_OPBT37_setting;              ///< Option byte setting 37
    const uint32_t OPBT_OPBT38_setting;              ///< Option byte setting 38
    const uint32_t OPBT_OPBT39_setting;              ///< Option byte setting 39
    const uint32_t OPBT_OPBT40_setting;              ///< Option byte setting 40
} bsp_opbt_setting_t;

/** @} (end addtogroup BSP_MCU_RH850U2BX) */

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const bsp_opbt_setting_t g_bsp_OPBT_setting;

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif

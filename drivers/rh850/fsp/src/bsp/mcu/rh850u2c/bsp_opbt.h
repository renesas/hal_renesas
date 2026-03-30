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
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_OPBT_U2CX BSP Option Bytes
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Option Byte (OPBT) configuration settings */
typedef const struct bsp_opbt_setting_cfg
{
    /* Software Configuration Option Byte (n = 0 to 31) */
    const uint32_t SW_CFG_OPBT_0_setting;            ///< Software Configuration Option Byte 0
    const uint32_t SW_CFG_OPBT_1_setting;            ///< Software Configuration Option Byte 1
    const uint32_t SW_CFG_OPBT_2_setting;            ///< Software Configuration Option Byte 2
    const uint32_t SW_CFG_OPBT_3_setting;            ///< Software Configuration Option Byte 3
    const uint32_t SW_CFG_OPBT_4_setting;            ///< Software Configuration Option Byte 4
    const uint32_t SW_CFG_OPBT_5_setting;            ///< Software Configuration Option Byte 5
    const uint32_t SW_CFG_OPBT_6_setting;            ///< Software Configuration Option Byte 6
    const uint32_t SW_CFG_OPBT_7_setting;            ///< Software Configuration Option Byte 7
    const uint32_t SW_CFG_OPBT_8_setting;            ///< Software Configuration Option Byte 8
    const uint32_t SW_CFG_OPBT_9_setting;            ///< Software Configuration Option Byte 9
    const uint32_t SW_CFG_OPBT_10_setting;           ///< Software Configuration Option Byte 10
    const uint32_t SW_CFG_OPBT_11_setting;           ///< Software Configuration Option Byte 11
    const uint32_t SW_CFG_OPBT_12_setting;           ///< Software Configuration Option Byte 12
    const uint32_t SW_CFG_OPBT_13_setting;           ///< Software Configuration Option Byte 13
    const uint32_t SW_CFG_OPBT_14_setting;           ///< Software Configuration Option Byte 14
    const uint32_t SW_CFG_OPBT_15_setting;           ///< Software Configuration Option Byte 15
    const uint32_t SW_CFG_OPBT_16_setting;           ///< Software Configuration Option Byte 16
    const uint32_t SW_CFG_OPBT_17_setting;           ///< Software Configuration Option Byte 17
    const uint32_t SW_CFG_OPBT_18_setting;           ///< Software Configuration Option Byte 18
    const uint32_t SW_CFG_OPBT_19_setting;           ///< Software Configuration Option Byte 19
    const uint32_t SW_CFG_OPBT_20_setting;           ///< Software Configuration Option Byte 20
    const uint32_t SW_CFG_OPBT_21_setting;           ///< Software Configuration Option Byte 21
    const uint32_t SW_CFG_OPBT_22_setting;           ///< Software Configuration Option Byte 22
    const uint32_t SW_CFG_OPBT_23_setting;           ///< Software Configuration Option Byte 23
    const uint32_t SW_CFG_OPBT_24_setting;           ///< Software Configuration Option Byte 24
    const uint32_t SW_CFG_OPBT_25_setting;           ///< Software Configuration Option Byte 25
    const uint32_t SW_CFG_OPBT_26_setting;           ///< Software Configuration Option Byte 26
    const uint32_t SW_CFG_OPBT_27_setting;           ///< Software Configuration Option Byte 27
    const uint32_t SW_CFG_OPBT_28_setting;           ///< Software Configuration Option Byte 28
    const uint32_t SW_CFG_OPBT_29_setting;           ///< Software Configuration Option Byte 29
    const uint32_t SW_CFG_OPBT_30_setting;           ///< Software Configuration Option Byte 30
    const uint32_t SW_CFG_OPBT_31_setting;           ///< Software Configuration Option Byte 31

    /* Reset Vector Address */
    const uint32_t RESET_VECTOR_ADDRESS_PE0_setting; ///< Reset vector address for PE0
    const uint32_t RESET_VECTOR_ADDRESS_PE1_setting; ///< Reset vector address for PE1

    const uint32_t Reserved_0;                       ///< Reserved field 0
    const uint32_t Reserved_1;                       ///< Reserved field 1
    const uint32_t Reserved_2;                       ///< Reserved field 2
    const uint32_t Reserved_3;                       ///< Reserved field 3
    const uint32_t Reserved_4;                       ///< Reserved field 4
    const uint32_t Reserved_5;                       ///< Reserved field 5

    /* OPBT setting value */
    const uint32_t OPBT_OPBT0_setting;               ///< OPBT setting 0
    const uint32_t OPBT_OPBT1_setting;               ///< OPBT setting 1
    const uint32_t OPBT_OPBT2_setting;               ///< OPBT setting 2
    const uint32_t OPBT_OPBT3_setting;               ///< OPBT setting 3
    const uint32_t OPBT_OPBT4_setting;               ///< OPBT setting 4
    const uint32_t OPBT_OPBT5_setting;               ///< OPBT setting 5
    const uint32_t OPBT_OPBT6_setting;               ///< OPBT setting 6
    const uint32_t OPBT_OPBT7_setting;               ///< OPBT setting 7
    const uint32_t OPBT_OPBT8_setting;               ///< OPBT setting 8
    const uint32_t OPBT_OPBT9_setting;               ///< OPBT setting 9
    const uint32_t OPBT_OPBT10_setting;              ///< OPBT setting 10
    const uint32_t OPBT_OPBT11_setting;              ///< OPBT setting 11
    const uint32_t OPBT_OPBT12_setting;              ///< OPBT setting 12
    const uint32_t OPBT_OPBT13_setting;              ///< OPBT setting 13
    const uint32_t OPBT_OPBT14_setting;              ///< OPBT setting 14
    const uint32_t OPBT_OPBT15_setting;              ///< OPBT setting 15
    const uint32_t OPBT_OPBT16_setting;              ///< OPBT setting 16
    const uint32_t OPBT_OPBT17_setting;              ///< OPBT setting 17
    const uint32_t OPBT_OPBT18_setting;              ///< OPBT setting 18
    const uint32_t OPBT_OPBT19_setting;              ///< OPBT setting 19
    const uint32_t OPBT_OPBT20_setting;              ///< OPBT setting 20
    const uint32_t OPBT_OPBT21_setting;              ///< OPBT setting 21
    const uint32_t OPBT_OPBT22_setting;              ///< OPBT setting 22
    const uint32_t OPBT_OPBT23_setting;              ///< OPBT setting 23
    const uint32_t OPBT_OPBT24_setting;              ///< OPBT setting 24
    const uint32_t OPBT_OPBT25_setting;              ///< OPBT setting 25
    const uint32_t OPBT_OPBT26_setting;              ///< OPBT setting 26
    const uint32_t OPBT_OPBT27_setting;              ///< OPBT setting 27
    const uint32_t OPBT_OPBT28_setting;              ///< OPBT setting 28
    const uint32_t OPBT_OPBT29_setting;              ///< OPBT setting 29
    const uint32_t OPBT_OPBT30_setting;              ///< OPBT setting 30
    const uint32_t OPBT_OPBT31_setting;              ///< OPBT setting 31
    const uint32_t OPBT_OPBT32_setting;              ///< OPBT setting 32
    const uint32_t OPBT_OPBT33_setting;              ///< OPBT setting 33
    const uint32_t OPBT_OPBT34_setting;              ///< OPBT setting 34
    const uint32_t OPBT_OPBT35_setting;              ///< OPBT setting 35
    const uint32_t OPBT_OPBT36_setting;              ///< OPBT setting 36
    const uint32_t OPBT_OPBT37_setting;              ///< OPBT setting 37
    const uint32_t OPBT_OPBT38_setting;              ///< OPBT setting 38
    const uint32_t OPBT_OPBT39_setting;              ///< OPBT setting 39
    const uint32_t OPBT_OPBT40_setting;              ///< OPBT setting 40
    const uint32_t OPBT_OPBT41_setting;              ///< OPBT setting 41
    const uint32_t OPBT_OPBT42_setting;              ///< OPBT setting 42
    const uint32_t OPBT_OPBT43_setting;              ///< OPBT setting 43
} bsp_opbt_setting_t;

/** @} (end addtogroup BSP_MCU_RH850U2CX) */

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

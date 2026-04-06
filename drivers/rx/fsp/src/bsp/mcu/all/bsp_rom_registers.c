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

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global variables (to be accessed by other files)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables and functions
 **********************************************************************************************************************/

#if !BSP_CFG_BOOT_IMAGE

 #if defined(__GNUC__) || defined(__ICCRX__)

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_dualsel") g_bsp_rom_dualsel =
    BSP_CFG_ROM_REG_DUALSEL;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_sas") g_bsp_rom_sas =
    BSP_CFG_ROM_REG_SAS;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_sas2") g_bsp_rom_sas2 =
    0xFFFFFFFF;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_banksel_sec") g_bsp_rom_banksel_sec =
    0xFFFFFFFF;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_blsel_sec") g_bsp_rom_blsel_sec =
    BSP_CFG_ROM_REG_BLSEL_SEC;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_banksel_sel") g_bsp_rom_banksel_sel =
    BSP_CFG_ROM_REG_BANKSEL_SEL;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_blsel_sel") g_bsp_rom_blsel_sel =
    BSP_CFG_ROM_REG_BLSEL_SEL;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_bpsdm_sec") g_bsp_rom_bpsdm_sec =
    BSP_CFG_ROM_REG_BPSDM_SEC;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_bps_sec") g_bsp_rom_bps_sec[] =
{
    BSP_CFG_ROM_REG_BPS_SEC0,
    BSP_CFG_ROM_REG_BPS_SEC1,
    BSP_CFG_ROM_REG_BPS_SEC2,
    BSP_CFG_ROM_REG_BPS_SEC3,
    BSP_CFG_ROM_REG_BPS_SEC4,
    BSP_CFG_ROM_REG_BPS_SEC5,
    BSP_CFG_ROM_REG_BPS_SEC6,
    BSP_CFG_ROM_REG_BPS_SEC7,
    BSP_CFG_ROM_REG_BPS_SEC8,
    BSP_CFG_ROM_REG_BPS_SEC9,
    BSP_CFG_ROM_REG_BPS_SEC10,
    BSP_CFG_ROM_REG_BPS_SEC11,
    BSP_CFG_ROM_REG_BPS_SEC12,
    BSP_CFG_ROM_REG_BPS_SEC13,
    BSP_CFG_ROM_REG_BPS_SEC14,
    BSP_CFG_ROM_REG_BPS_SEC15
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_bps_sec_dual") g_bsp_rom_bps_sec_dual[] =
{
    BSP_CFG_ROM_REG_BPS_SEC64,
    BSP_CFG_ROM_REG_BPS_SEC65,
    BSP_CFG_ROM_REG_BPS_SEC66,
    BSP_CFG_ROM_REG_BPS_SEC67,
    BSP_CFG_ROM_REG_BPS_SEC68,
    BSP_CFG_ROM_REG_BPS_SEC69,
    BSP_CFG_ROM_REG_BPS_SEC70,
    BSP_CFG_ROM_REG_BPS_SEC71
};

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_banksel") g_bsp_rom_banksel =
    0xFFFFFFFF;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_blsel") g_bsp_rom_blsel =
    BSP_CFG_ROM_REG_BLSEL;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_bpsdm") g_bsp_rom_bpsdm =
    BSP_CFG_ROM_REG_BPSDM;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_bps") g_bsp_rom_bps[] =
{
    BSP_CFG_ROM_REG_BPS0,
    BSP_CFG_ROM_REG_BPS1,
    BSP_CFG_ROM_REG_BPS2,
    BSP_CFG_ROM_REG_BPS3,
    BSP_CFG_ROM_REG_BPS4,
    BSP_CFG_ROM_REG_BPS5,
    BSP_CFG_ROM_REG_BPS6,
    BSP_CFG_ROM_REG_BPS7,
    BSP_CFG_ROM_REG_BPS8,
    BSP_CFG_ROM_REG_BPS9,
    BSP_CFG_ROM_REG_BPS10,
    BSP_CFG_ROM_REG_BPS11,
    BSP_CFG_ROM_REG_BPS12,
    BSP_CFG_ROM_REG_BPS13,
    BSP_CFG_ROM_REG_BPS14,
    BSP_CFG_ROM_REG_BPS15
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_bps_dual") g_bsp_rom_bps_dual[] =
{
    BSP_CFG_ROM_REG_BPS64,
    BSP_CFG_ROM_REG_BPS65,
    BSP_CFG_ROM_REG_BPS66,
    BSP_CFG_ROM_REG_BPS67,
    BSP_CFG_ROM_REG_BPS68,
    BSP_CFG_ROM_REG_BPS69,
    BSP_CFG_ROM_REG_BPS70,
    BSP_CFG_ROM_REG_BPS71
};

 #else

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_dualsel") g_bsp_rom_dualsel =
    BSP_CFG_ROM_REG_DUALSEL;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_sas") g_bsp_rom_sas =
    BSP_CFG_ROM_REG_SAS;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_sas2") g_bsp_rom_sas2 =
    0xFFFFFFFF;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_banksel_sec") g_bsp_rom_banksel_sec =
    0xFFFFFFFF;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_blsel_sec") g_bsp_rom_blsel_sec =
    BSP_CFG_ROM_REG_BLSEL_SEC;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_banksel_sel") g_bsp_rom_banksel_sel =
    BSP_CFG_ROM_REG_BANKSEL_SEL;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_blsel_sel") g_bsp_rom_blsel_sel =
    BSP_CFG_ROM_REG_BLSEL_SEL;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_bpsdm_sec") g_bsp_rom_bpsdm_sec =
    BSP_CFG_ROM_REG_BPSDM_SEC;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_bps_sec") g_bsp_rom_bps_sec[] =
{
    BSP_CFG_ROM_REG_BPS_SEC0,
    BSP_CFG_ROM_REG_BPS_SEC1,
    BSP_CFG_ROM_REG_BPS_SEC2,
    BSP_CFG_ROM_REG_BPS_SEC3,
    BSP_CFG_ROM_REG_BPS_SEC4,
    BSP_CFG_ROM_REG_BPS_SEC5,
    BSP_CFG_ROM_REG_BPS_SEC6,
    BSP_CFG_ROM_REG_BPS_SEC7,
    BSP_CFG_ROM_REG_BPS_SEC8,
    BSP_CFG_ROM_REG_BPS_SEC9,
    BSP_CFG_ROM_REG_BPS_SEC10,
    BSP_CFG_ROM_REG_BPS_SEC11,
    BSP_CFG_ROM_REG_BPS_SEC12,
    BSP_CFG_ROM_REG_BPS_SEC13,
    BSP_CFG_ROM_REG_BPS_SEC14,
    BSP_CFG_ROM_REG_BPS_SEC15
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_bps_sec_dual") g_bsp_rom_bps_sec_dual[] =
{
    BSP_CFG_ROM_REG_BPS_SEC64,
    BSP_CFG_ROM_REG_BPS_SEC65,
    BSP_CFG_ROM_REG_BPS_SEC66,
    BSP_CFG_ROM_REG_BPS_SEC67,
    BSP_CFG_ROM_REG_BPS_SEC68,
    BSP_CFG_ROM_REG_BPS_SEC69,
    BSP_CFG_ROM_REG_BPS_SEC70,
    BSP_CFG_ROM_REG_BPS_SEC71
};

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_banksel") g_bsp_rom_banksel =
    0xFFFFFFFF;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_blsel") g_bsp_rom_blsel =
    BSP_CFG_ROM_REG_BLSEL;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_bpsdm") g_bsp_rom_bpsdm =
    BSP_CFG_ROM_REG_BPSDM;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_bps") g_bsp_rom_bps[] =
{
    BSP_CFG_ROM_REG_BPS0,
    BSP_CFG_ROM_REG_BPS1,
    BSP_CFG_ROM_REG_BPS2,
    BSP_CFG_ROM_REG_BPS3,
    BSP_CFG_ROM_REG_BPS4,
    BSP_CFG_ROM_REG_BPS5,
    BSP_CFG_ROM_REG_BPS6,
    BSP_CFG_ROM_REG_BPS7,
    BSP_CFG_ROM_REG_BPS8,
    BSP_CFG_ROM_REG_BPS9,
    BSP_CFG_ROM_REG_BPS10,
    BSP_CFG_ROM_REG_BPS11,
    BSP_CFG_ROM_REG_BPS12,
    BSP_CFG_ROM_REG_BPS13,
    BSP_CFG_ROM_REG_BPS14,
    BSP_CFG_ROM_REG_BPS15
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_bps_dual") g_bsp_rom_bps_dual[] =
{
    BSP_CFG_ROM_REG_BPS64,
    BSP_CFG_ROM_REG_BPS65,
    BSP_CFG_ROM_REG_BPS66,
    BSP_CFG_ROM_REG_BPS67,
    BSP_CFG_ROM_REG_BPS68,
    BSP_CFG_ROM_REG_BPS69,
    BSP_CFG_ROM_REG_BPS70,
    BSP_CFG_ROM_REG_BPS71
};

 #endif

#endif

/** @} (end addtogroup BSP_MCU) */

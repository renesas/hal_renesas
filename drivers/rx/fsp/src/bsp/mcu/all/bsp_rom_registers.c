/* ${REA_DISCLAIMER_PLACEHOLDER} */

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
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_data_flash_hoemrtpk") g_bsp_rom_hoemrtpk[]
    =
    {
    /* HOEMRTPK0n */
    BSP_CFG_ROM_REG_HOEMRTPK0_0,
    BSP_CFG_ROM_REG_HOEMRTPK0_1,
    BSP_CFG_ROM_REG_HOEMRTPK0_2,
    BSP_CFG_ROM_REG_HOEMRTPK0_3,
    BSP_CFG_ROM_REG_HOEMRTPK0_4,
    BSP_CFG_ROM_REG_HOEMRTPK0_5,
    BSP_CFG_ROM_REG_HOEMRTPK0_6,
    BSP_CFG_ROM_REG_HOEMRTPK0_7,
    BSP_CFG_ROM_REG_HOEMRTPK0_8,
    BSP_CFG_ROM_REG_HOEMRTPK0_9,
    BSP_CFG_ROM_REG_HOEMRTPK0_10,
    BSP_CFG_ROM_REG_HOEMRTPK0_11,

    /* HOEMRTPK1n */
    BSP_CFG_ROM_REG_HOEMRTPK1_0,
    BSP_CFG_ROM_REG_HOEMRTPK1_1,
    BSP_CFG_ROM_REG_HOEMRTPK1_2,
    BSP_CFG_ROM_REG_HOEMRTPK1_3,
    BSP_CFG_ROM_REG_HOEMRTPK1_4,
    BSP_CFG_ROM_REG_HOEMRTPK1_5,
    BSP_CFG_ROM_REG_HOEMRTPK1_6,
    BSP_CFG_ROM_REG_HOEMRTPK1_7,
    BSP_CFG_ROM_REG_HOEMRTPK1_8,
    BSP_CFG_ROM_REG_HOEMRTPK1_9,
    BSP_CFG_ROM_REG_HOEMRTPK1_10,
    BSP_CFG_ROM_REG_HOEMRTPK1_11,

    /* HOEMRTPK2n */
    BSP_CFG_ROM_REG_HOEMRTPK2_0,
    BSP_CFG_ROM_REG_HOEMRTPK2_1,
    BSP_CFG_ROM_REG_HOEMRTPK2_2,
    BSP_CFG_ROM_REG_HOEMRTPK2_3,
    BSP_CFG_ROM_REG_HOEMRTPK2_4,
    BSP_CFG_ROM_REG_HOEMRTPK2_5,
    BSP_CFG_ROM_REG_HOEMRTPK2_6,
    BSP_CFG_ROM_REG_HOEMRTPK2_7,
    BSP_CFG_ROM_REG_HOEMRTPK2_8,
    BSP_CFG_ROM_REG_HOEMRTPK2_9,
    BSP_CFG_ROM_REG_HOEMRTPK2_10,
    BSP_CFG_ROM_REG_HOEMRTPK2_11,

    /* HOEMRTPK3n */
    BSP_CFG_ROM_REG_HOEMRTPK3_0,
    BSP_CFG_ROM_REG_HOEMRTPK3_1,
    BSP_CFG_ROM_REG_HOEMRTPK3_2,
    BSP_CFG_ROM_REG_HOEMRTPK3_3,
    BSP_CFG_ROM_REG_HOEMRTPK3_4,
    BSP_CFG_ROM_REG_HOEMRTPK3_5,
    BSP_CFG_ROM_REG_HOEMRTPK3_6,
    BSP_CFG_ROM_REG_HOEMRTPK3_7,
    BSP_CFG_ROM_REG_HOEMRTPK3_8,
    BSP_CFG_ROM_REG_HOEMRTPK3_9,
    BSP_CFG_ROM_REG_HOEMRTPK3_10,
    BSP_CFG_ROM_REG_HOEMRTPK3_11
    };
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_fsblctrl0") g_bsp_rom_fsblctrl0
    =
        BSP_CFG_ROM_REG_FSBLCTRL0;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_fsblctrl1") g_bsp_rom_fsblctrl1
    =
        BSP_CFG_ROM_REG_FSBLCTRL1;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_fsblctrl2") g_bsp_rom_fsblctrl2
    =
        BSP_CFG_ROM_REG_FSBLCTRL2;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_samr") g_bsp_rom_samr =
    BSP_CFG_ROM_REG_SAMR;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc0") g_bsp_rom_sacc0 =
    BSP_CFG_ROM_REG_SACC0;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc10") g_bsp_rom_sacc10 =
    BSP_CFG_ROM_REG_SACC10;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc1") g_bsp_rom_sacc1 =
    BSP_CFG_ROM_REG_SACC1;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc11") g_bsp_rom_sacc11 =
    BSP_CFG_ROM_REG_SACC11;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc2") g_bsp_rom_sacc2 =
    BSP_CFG_ROM_REG_SACC2;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc12") g_bsp_rom_sacc12 =
    BSP_CFG_ROM_REG_SACC12;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc3") g_bsp_rom_sacc3 =
    BSP_CFG_ROM_REG_SACC3;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_data_flash_sacc13") g_bsp_rom_sacc13 =
    BSP_CFG_ROM_REG_SACC13;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_gpotp") g_bsp_rom_gpotp[] =
{
    BSP_CFG_ROM_REG_GPOTP_0,
    BSP_CFG_ROM_REG_GPOTP_1,
    BSP_CFG_ROM_REG_GPOTP_2,
    BSP_CFG_ROM_REG_GPOTP_3,
    BSP_CFG_ROM_REG_GPOTP_4,
    BSP_CFG_ROM_REG_GPOTP_5,
    BSP_CFG_ROM_REG_GPOTP_6,
    BSP_CFG_ROM_REG_GPOTP_7,
    BSP_CFG_ROM_REG_GPOTP_8,
    BSP_CFG_ROM_REG_GPOTP_9,
    BSP_CFG_ROM_REG_GPOTP_10,
    BSP_CFG_ROM_REG_GPOTP_11,
    BSP_CFG_ROM_REG_GPOTP_12,
    BSP_CFG_ROM_REG_GPOTP_13,
    BSP_CFG_ROM_REG_GPOTP_14,
    BSP_CFG_ROM_REG_GPOTP_15,
    BSP_CFG_ROM_REG_GPOTP_16,
    BSP_CFG_ROM_REG_GPOTP_17,
    BSP_CFG_ROM_REG_GPOTP_18,
    BSP_CFG_ROM_REG_GPOTP_19,
    BSP_CFG_ROM_REG_GPOTP_20,
    BSP_CFG_ROM_REG_GPOTP_21,
    BSP_CFG_ROM_REG_GPOTP_22,
    BSP_CFG_ROM_REG_GPOTP_23
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs0") g_bsp_rom_ofs0 =
    BSP_CFG_ROM_REG_OFS0;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs2") g_bsp_rom_ofs2 =
    BSP_CFG_ROM_REG_OFS2;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_mde") g_bsp_rom_mde =
    BSP_CFG_ROM_REG_MDE;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs1_sec") g_bsp_rom_ofs1_sec =
    BSP_CFG_ROM_REG_OFS1_SEC;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs3_sec") g_bsp_rom_ofs3_sec =
    BSP_CFG_ROM_REG_OFS3_SEC;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs1_sel") g_bsp_rom_ofs1_sel =
    BSP_CFG_ROM_REG_OFS1_SEL;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs3_sel") g_bsp_rom_ofs3_sel =
    BSP_CFG_ROM_REG_OFS3_SEL;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_pbps_sec") g_bsp_rom_pbps_sec[] =
{
    BSP_CFG_ROM_REG_PBPS_SEC0,
    BSP_CFG_ROM_REG_PBPS_SEC1,
    BSP_CFG_ROM_REG_PBPS_SEC2,
    BSP_CFG_ROM_REG_PBPS_SEC3
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_pbps_sec_dual") g_bsp_rom_pbps_sec_dual
[] =
{
    BSP_CFG_ROM_REG_PBPS_SEC16,
    BSP_CFG_ROM_REG_PBPS_SEC17
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_pbpsdm_sec") g_bsp_rom_pbpsdm_sec =
    BSP_CFG_ROM_REG_PBPSDM_SEC;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_revoke") g_bsp_rom_revoke =
    BSP_CFG_ROM_REG_REVOKE;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_zhuk") g_bsp_rom_zhuk =
    BSP_CFG_ROM_REG_ZHUK;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_arccs") g_bsp_rom_arccs =
    BSP_CFG_ROM_REG_ARCCS;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ubcsr") g_bsp_rom_ubcsr =
    BSP_CFG_ROM_REG_UBCSR;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_ofsps") g_bsp_rom_ofsps[] =
{
    BSP_CFG_ROM_REG_OFSPS_0,
    BSP_CFG_ROM_REG_OFSPS_1,
    BSP_CFG_ROM_REG_OFSPS_2,
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_ofsps2") g_bsp_rom_ofsps2[] =
{
    BSP_CFG_ROM_REG_OFSPS2_0,
    BSP_CFG_ROM_REG_OFSPS2_1,
    BSP_CFG_ROM_REG_OFSPS2_2,
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
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs1") g_bsp_rom_ofs1 =
    BSP_CFG_ROM_REG_OFS1;
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_ofs3") g_bsp_rom_ofs3 =
    BSP_CFG_ROM_REG_OFS3;

BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_pbps") g_bsp_rom_pbps[] =
{
    BSP_CFG_ROM_REG_PBPS0,
    BSP_CFG_ROM_REG_PBPS1,
    BSP_CFG_ROM_REG_PBPS2,
    BSP_CFG_ROM_REG_PBPS3
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION (".option_setting_pbps_dual") g_bsp_rom_pbps_dual[] =
{
    BSP_CFG_ROM_REG_PBPS16,
    BSP_CFG_ROM_REG_PBPS17
};
BSP_DONT_REMOVE static const uint32_t BSP_PLACE_IN_SECTION(".option_setting_pbpsdm") g_bsp_rom_pbpsdm =
    BSP_CFG_ROM_REG_PBPSDM;

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
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_data_flash_hoemrtpk") g_bsp_rom_hoemrtpk
[] =
{
    /* HOEMRTPK0n */
    BSP_CFG_ROM_REG_HOEMRTPK0_0,
    BSP_CFG_ROM_REG_HOEMRTPK0_1,
    BSP_CFG_ROM_REG_HOEMRTPK0_2,
    BSP_CFG_ROM_REG_HOEMRTPK0_3,
    BSP_CFG_ROM_REG_HOEMRTPK0_4,
    BSP_CFG_ROM_REG_HOEMRTPK0_5,
    BSP_CFG_ROM_REG_HOEMRTPK0_6,
    BSP_CFG_ROM_REG_HOEMRTPK0_7,
    BSP_CFG_ROM_REG_HOEMRTPK0_8,
    BSP_CFG_ROM_REG_HOEMRTPK0_9,
    BSP_CFG_ROM_REG_HOEMRTPK0_10,
    BSP_CFG_ROM_REG_HOEMRTPK0_11,

    /* HOEMRTPK1n */
    BSP_CFG_ROM_REG_HOEMRTPK1_0,
    BSP_CFG_ROM_REG_HOEMRTPK1_1,
    BSP_CFG_ROM_REG_HOEMRTPK1_2,
    BSP_CFG_ROM_REG_HOEMRTPK1_3,
    BSP_CFG_ROM_REG_HOEMRTPK1_4,
    BSP_CFG_ROM_REG_HOEMRTPK1_5,
    BSP_CFG_ROM_REG_HOEMRTPK1_6,
    BSP_CFG_ROM_REG_HOEMRTPK1_7,
    BSP_CFG_ROM_REG_HOEMRTPK1_8,
    BSP_CFG_ROM_REG_HOEMRTPK1_9,
    BSP_CFG_ROM_REG_HOEMRTPK1_10,
    BSP_CFG_ROM_REG_HOEMRTPK1_11,

    /* HOEMRTPK2n */
    BSP_CFG_ROM_REG_HOEMRTPK2_0,
    BSP_CFG_ROM_REG_HOEMRTPK2_1,
    BSP_CFG_ROM_REG_HOEMRTPK2_2,
    BSP_CFG_ROM_REG_HOEMRTPK2_3,
    BSP_CFG_ROM_REG_HOEMRTPK2_4,
    BSP_CFG_ROM_REG_HOEMRTPK2_5,
    BSP_CFG_ROM_REG_HOEMRTPK2_6,
    BSP_CFG_ROM_REG_HOEMRTPK2_7,
    BSP_CFG_ROM_REG_HOEMRTPK2_8,
    BSP_CFG_ROM_REG_HOEMRTPK2_9,
    BSP_CFG_ROM_REG_HOEMRTPK2_10,
    BSP_CFG_ROM_REG_HOEMRTPK2_11,

    /* HOEMRTPK3n */
    BSP_CFG_ROM_REG_HOEMRTPK3_0,
    BSP_CFG_ROM_REG_HOEMRTPK3_1,
    BSP_CFG_ROM_REG_HOEMRTPK3_2,
    BSP_CFG_ROM_REG_HOEMRTPK3_3,
    BSP_CFG_ROM_REG_HOEMRTPK3_4,
    BSP_CFG_ROM_REG_HOEMRTPK3_5,
    BSP_CFG_ROM_REG_HOEMRTPK3_6,
    BSP_CFG_ROM_REG_HOEMRTPK3_7,
    BSP_CFG_ROM_REG_HOEMRTPK3_8,
    BSP_CFG_ROM_REG_HOEMRTPK3_9,
    BSP_CFG_ROM_REG_HOEMRTPK3_10,
    BSP_CFG_ROM_REG_HOEMRTPK3_11
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_fsblctrl0")
g_bsp_rom_fsblctrl0 =
    BSP_CFG_ROM_REG_FSBLCTRL0;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_fsblctrl1")
g_bsp_rom_fsblctrl1 =
    BSP_CFG_ROM_REG_FSBLCTRL1;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_fsblctrl2")
g_bsp_rom_fsblctrl2 =
    BSP_CFG_ROM_REG_FSBLCTRL2;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_samr") g_bsp_rom_samr =
    BSP_CFG_ROM_REG_SAMR;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc0") g_bsp_rom_sacc0 =
    BSP_CFG_ROM_REG_SACC0;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc10") g_bsp_rom_sacc10 =
    BSP_CFG_ROM_REG_SACC10;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc1") g_bsp_rom_sacc1 =
    BSP_CFG_ROM_REG_SACC1;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc11") g_bsp_rom_sacc11 =
    BSP_CFG_ROM_REG_SACC11;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc2") g_bsp_rom_sacc2 =
    BSP_CFG_ROM_REG_SACC2;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc12") g_bsp_rom_sacc12 =
    BSP_CFG_ROM_REG_SACC12;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc3") g_bsp_rom_sacc3 =
    BSP_CFG_ROM_REG_SACC3;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_data_flash_sacc13") g_bsp_rom_sacc13 =
    BSP_CFG_ROM_REG_SACC13;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_gpotp") g_bsp_rom_gpotp[] =
{
    BSP_CFG_ROM_REG_GPOTP_0,
    BSP_CFG_ROM_REG_GPOTP_1,
    BSP_CFG_ROM_REG_GPOTP_2,
    BSP_CFG_ROM_REG_GPOTP_3,
    BSP_CFG_ROM_REG_GPOTP_4,
    BSP_CFG_ROM_REG_GPOTP_5,
    BSP_CFG_ROM_REG_GPOTP_6,
    BSP_CFG_ROM_REG_GPOTP_7,
    BSP_CFG_ROM_REG_GPOTP_8,
    BSP_CFG_ROM_REG_GPOTP_9,
    BSP_CFG_ROM_REG_GPOTP_10,
    BSP_CFG_ROM_REG_GPOTP_11,
    BSP_CFG_ROM_REG_GPOTP_12,
    BSP_CFG_ROM_REG_GPOTP_13,
    BSP_CFG_ROM_REG_GPOTP_14,
    BSP_CFG_ROM_REG_GPOTP_15,
    BSP_CFG_ROM_REG_GPOTP_16,
    BSP_CFG_ROM_REG_GPOTP_17,
    BSP_CFG_ROM_REG_GPOTP_18,
    BSP_CFG_ROM_REG_GPOTP_19,
    BSP_CFG_ROM_REG_GPOTP_20,
    BSP_CFG_ROM_REG_GPOTP_21,
    BSP_CFG_ROM_REG_GPOTP_22,
    BSP_CFG_ROM_REG_GPOTP_23
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs0") g_bsp_rom_ofs0 =
    BSP_CFG_ROM_REG_OFS0;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs2") g_bsp_rom_ofs2 =
    BSP_CFG_ROM_REG_OFS2;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_mde") g_bsp_rom_mde =
    BSP_CFG_ROM_REG_MDE;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs1_sec") g_bsp_rom_ofs1_sec =
    BSP_CFG_ROM_REG_OFS1_SEC;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs3_sec") g_bsp_rom_ofs3_sec =
    BSP_CFG_ROM_REG_OFS3_SEC;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs1_sel") g_bsp_rom_ofs1_sel =
    BSP_CFG_ROM_REG_OFS1_SEL;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs3_sel") g_bsp_rom_ofs3_sel =
    BSP_CFG_ROM_REG_OFS3_SEL;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_pbps_sec") g_bsp_rom_pbps_sec[] =
{
    BSP_CFG_ROM_REG_PBPS_SEC0,
    BSP_CFG_ROM_REG_PBPS_SEC1,
    BSP_CFG_ROM_REG_PBPS_SEC2,
    BSP_CFG_ROM_REG_PBPS_SEC3
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_pbps_sec_dual")
g_bsp_rom_pbps_sec_dual[]
    =
    {
    BSP_CFG_ROM_REG_PBPS_SEC16,
    BSP_CFG_ROM_REG_PBPS_SEC17
    };
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_pbpsdm_sec") g_bsp_rom_pbpsdm_sec =
    BSP_CFG_ROM_REG_PBPSDM_SEC;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_revoke") g_bsp_rom_revoke =
    BSP_CFG_ROM_REG_REVOKE;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_zhuk") g_bsp_rom_zhuk =
    BSP_CFG_ROM_REG_ZHUK;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_arccs") g_bsp_rom_arccs =
    BSP_CFG_ROM_REG_ARCCS;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ubcsr") g_bsp_rom_ubcsr =
    BSP_CFG_ROM_REG_UBCSR;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_ofsps") g_bsp_rom_ofsps[] =
{
    BSP_CFG_ROM_REG_OFSPS_0,
    BSP_CFG_ROM_REG_OFSPS_1,
    BSP_CFG_ROM_REG_OFSPS_2,
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_ofsps2") g_bsp_rom_ofsps2[] =
{
    BSP_CFG_ROM_REG_OFSPS2_0,
    BSP_CFG_ROM_REG_OFSPS2_1,
    BSP_CFG_ROM_REG_OFSPS2_2,
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
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs1") g_bsp_rom_ofs1 =
    BSP_CFG_ROM_REG_OFS1;
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_ofs3") g_bsp_rom_ofs3 =
    BSP_CFG_ROM_REG_OFS3;

BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_pbps") g_bsp_rom_pbps[] =
{
    BSP_CFG_ROM_REG_PBPS0,
    BSP_CFG_ROM_REG_PBPS1,
    BSP_CFG_ROM_REG_PBPS2,
    BSP_CFG_ROM_REG_PBPS3
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION ("__period__option_setting_pbps_dual") g_bsp_rom_pbps_dual[] =
{
    BSP_CFG_ROM_REG_PBPS16,
    BSP_CFG_ROM_REG_PBPS17
};
BSP_DONT_REMOVE const uint32_t BSP_PLACE_IN_SECTION("__period__option_setting_pbpsdm") g_bsp_rom_pbpsdm =
    BSP_CFG_ROM_REG_PBPSDM;

 #endif

#endif

/** @} (end addtogroup BSP_MCU) */

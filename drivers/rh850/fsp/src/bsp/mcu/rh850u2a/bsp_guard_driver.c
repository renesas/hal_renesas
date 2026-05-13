/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "bsp_guard_driver.h"

/***********************************************************************************************************************
 * Macro definitions
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

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/

/** Array of all PBG groups base address */
const uint32_t g_pbgprot_baseaddress[] =
{
    [BSP_PBG_GROUP_PBG00]  = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG01]  = (uint32_t) R_PBG01_BASE,
    [BSP_PBG_GROUP_PBG10]  = (uint32_t) R_PBG10_BASE,
    [BSP_PBG_GROUP_PBG20]  = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG21]  = (uint32_t) R_PBG21_BASE,
    [BSP_PBG_GROUP_PBG22]  = (uint32_t) R_PBG22_BASE,
    [BSP_PBG_GROUP_PBG30]  = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG31]  = (uint32_t) R_PBG31_BASE,
    [BSP_PBG_GROUP_PBG32]  = (uint32_t) R_PBG32_BASE,
    [BSP_PBG_GROUP_PBG33]  = (uint32_t) R_PBG33_BASE,
    [BSP_PBG_GROUP_PBG40]  = (uint32_t) R_PBG40_BASE,
    [BSP_PBG_GROUP_PBG50]  = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG51]  = (uint32_t) R_PBG51_BASE,
    [BSP_PBG_GROUP_PBG52]  = (uint32_t) R_PBG52_BASE,
    [BSP_PBG_GROUP_PBG53]  = (uint32_t) R_PBG53_BASE,
    [BSP_PBG_GROUP_PBG6L0] = (uint32_t) R_PBG6L0_BASE,
    [BSP_PBG_GROUP_PBG6L1] = (uint32_t) R_PBG6L1_BASE,
    [BSP_PBG_GROUP_PBG6H0] = (uint32_t) R_PBG6H0_BASE,
    [BSP_PBG_GROUP_PBG70]  = (uint32_t) R_PBG70_BASE,
    [BSP_PBG_GROUP_PBG80]  = (uint32_t) R_PBG80_BASE,
    [BSP_PBG_GROUP_PBG90]  = (uint32_t) R_PBG90_BASE,
    [BSP_HBG_GROUP_HBG91]  = (uint32_t) R_HBG91_BASE,
    [BSP_HBG_GROUP_HBG92]  = (uint32_t) R_HBG92_BASE,
    [BSP_HBG_GROUP_HBG93]  = (uint32_t) R_HBG93_BASE,
    [BSP_HBG_GROUP_HBG96]  = (uint32_t) R_HBG96_BASE
};

/** Array of all PBG control groups base address */
const uint32_t g_pbgprot_control_baseaddress[] =
{
    [BSP_PBG_GROUP_PBG00]  = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG01]  = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG10]  = (uint32_t) R_PBG10_BASE,
    [BSP_PBG_GROUP_PBG20]  = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG21]  = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG22]  = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG30]  = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG31]  = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG32]  = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG33]  = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG40]  = (uint32_t) R_PBG40_BASE,
    [BSP_PBG_GROUP_PBG50]  = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG51]  = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG52]  = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG53]  = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG6L0] = (uint32_t) R_PBG6L0_BASE,
    [BSP_PBG_GROUP_PBG6L1] = (uint32_t) R_PBG6L1_BASE,
    [BSP_PBG_GROUP_PBG6H0] = (uint32_t) R_PBG6H0_BASE,
    [BSP_PBG_GROUP_PBG70]  = (uint32_t) R_PBG70_BASE,
    [BSP_PBG_GROUP_PBG80]  = (uint32_t) R_PBG80_BASE,
    [BSP_PBG_GROUP_PBG90]  = (uint32_t) R_PBG90_BASE,
    [BSP_HBG_GROUP_HBG91]  = (uint32_t) R_HBG91_BASE,
    [BSP_HBG_GROUP_HBG92]  = (uint32_t) R_HBG92_BASE,
    [BSP_HBG_GROUP_HBG93]  = (uint32_t) R_HBG93_BASE,
    [BSP_HBG_GROUP_HBG96]  = (uint32_t) R_HBG96_BASE
};

/** Array of all PBG Key protection groups base address */
const uint32_t g_pbgkcprot_baseaddress[] =
{
    [BSP_PBG_GROUP_PBG00]  = (uint32_t) R_PBGERRSLV00_BASE,
    [BSP_PBG_GROUP_PBG01]  = (uint32_t) R_PBGERRSLV00_BASE,
    [BSP_PBG_GROUP_PBG10]  = (uint32_t) R_PBGERRSLV10_BASE,
    [BSP_PBG_GROUP_PBG20]  = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG21]  = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG22]  = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG30]  = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG31]  = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG32]  = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG33]  = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG40]  = (uint32_t) R_PBGERRSLV40_BASE,
    [BSP_PBG_GROUP_PBG50]  = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG51]  = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG52]  = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG53]  = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG6L0] = (uint32_t) R_PBGERRSLV6L0_BASE,
    [BSP_PBG_GROUP_PBG6L1] = (uint32_t) R_PBGERRSLV6L1_BASE,
    [BSP_PBG_GROUP_PBG6H0] = (uint32_t) R_PBGERRSLV6H0_BASE,
    [BSP_PBG_GROUP_PBG70]  = (uint32_t) R_PBGERRSLV70_BASE,
    [BSP_PBG_GROUP_PBG80]  = (uint32_t) R_PBGERRSLV80_BASE,
    [BSP_PBG_GROUP_PBG90]  = (uint32_t) R_PBGERRSLV90_BASE,
    [BSP_HBG_GROUP_HBG91]  = (uint32_t) R_HBGERRSLV91_BASE,
    [BSP_HBG_GROUP_HBG92]  = (uint32_t) R_HBGERRSLV92_BASE,
    [BSP_HBG_GROUP_HBG93]  = (uint32_t) R_HBGERRSLV93_BASE,
    [BSP_HBG_GROUP_HBG96]  = (uint32_t) R_HBGERRSLV96_BASE
};

/** Array of all IBG control base address */
const uint32_t g_ibg_baseaddress[] =
{
    [BSP_IBG_CONTROL_IPIR_Rn] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_IBG_CONTROL_IPIR_Tn] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_IBG_CONTROL_IPIR_4]  = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_IBG_CONTROL_BARR_n]  = (uint32_t) R_GUARD_IBG_BARR_BASE,
    [BSP_IBG_CONTROL_BARR_16] = (uint32_t) R_GUARD_IBG_BARR_BASE,
    [BSP_IBG_CONTROL_TPTM_n]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
    [BSP_IBG_CONTROL_TPTM_8]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
    [BSP_IBG_CONTROL_TPTM_9]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
};

/** Array of all IBG protection control base address */
const uint32_t g_ibgprot_baseaddress[] =
{
    [BSP_IBG_CONTROL_IPIR_Rn] = (uint32_t) R_IBG00_IPIGPROT0_Rn_BASE,
    [BSP_IBG_CONTROL_IPIR_Tn] = (uint32_t) R_IBG00_IPIGPROT0_Tn_BASE,
    [BSP_IBG_CONTROL_IPIR_4]  = (uint32_t) R_IBG00_IPIGPROT0_4_BASE,
    [BSP_IBG_CONTROL_BARR_n]  = (uint32_t) R_IBG01_BRGPROT0_n_BASE,
    [BSP_IBG_CONTROL_BARR_16] = (uint32_t) R_IBG01_BRGPROT0_16_BASE,
    [BSP_IBG_CONTROL_TPTM_n]  = (uint32_t) R_IBG02_TPTGPROT0_n_BASE,
    [BSP_IBG_CONTROL_TPTM_U8] = (uint32_t) R_IBG02_TPTGPROT0_8_BASE,
    [BSP_IBG_CONTROL_TPTM_U9] = (uint32_t) R_IBG02_TPTGPROT0_9_BASE,
};

/** Array of all IBG module base address */
const uint32_t g_ibg_module_baseaddress[] =
{
    [BSP_GUARD_IPIR_MODULE] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_GUARD_BARR_MODULE] = (uint32_t) R_GUARD_IBG_BARR_BASE,
    [BSP_GUARD_TPTM_MODULE] = (uint32_t) R_GUARD_IBG_TPTM_BASE,
};

/** Array of all PEG base address */
const uint32_t g_peg_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_GUARD_PE0_BASE,
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_GUARD_PE1_BASE,
#if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE2] = (uint32_t) R_GUARD_PE2_BASE,
#endif
#if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE3] = (uint32_t) R_GUARD_PE3_BASE,
#endif
};

/** Array of all PEGCAP_S base address */
const uint32_t g_pegcap_s_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_PEGCAP_S_PE0CL0_BASE,
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_PEGCAP_S_PE1CL0_BASE,
#if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE2] = (uint32_t) R_PEGCAP_S_PE2CL1_BASE,
#endif
#if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE3] = (uint32_t) R_PEGCAP_S_PE3CL1_BASE,
#endif
};

/** Array of all PEGCAP_M base address */
const uint32_t g_pegcap_m_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_PEGCAP_M_PE0CL0_BASE,
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_PEGCAP_M_PE1CL0_BASE,
#if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE2] = (uint32_t) R_PEGCAP_M_PE2CL1_BASE,
#endif
#if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE3] = (uint32_t) R_PEGCAP_M_PE3CL1_BASE,
#endif
};

/** Array of all Local Ram base address */
const uint32_t g_peg_lram_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_GUARD_PE0_LRAM_BASE,
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_GUARD_PE1_LRAM_BASE,
#if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE2] = (uint32_t) R_GUARD_PE2_LRAM_BASE,
#endif
#if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE3] = (uint32_t) R_GUARD_PE3_LRAM_BASE,
#endif
};

/** Array of all CRG base address */
const uint32_t g_crg_baseaddress[] =
{
#if (BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM0] = (uint32_t) R_GUARD_CRAMCRG0_BASE,
#endif
#if (BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM1] = (uint32_t) R_GUARD_CRAMCRG1_BASE,
#endif
#if (BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM2] = (uint32_t) R_GUARD_CRAMCRG2_BASE,
#endif
#if (BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM3] = (uint32_t) R_GUARD_CRAMCRG3_BASE
#endif
};

/** Array of all Cluster Ram base address */
const uint32_t g_crg_cram_baseaddress[] =
{
#if (BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM0] = (uint32_t) R_GUARD_CRG_CRAM0_BASE,
#endif
#if (BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM1] = (uint32_t) R_GUARD_CRG_CRAM1_BASE,
#endif
#if (BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM2] = (uint32_t) R_GUARD_CRG_CRAM2_BASE,
#endif
#if (BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM3] = (uint32_t) R_GUARD_CRG_CRAM3_BASE,
#endif
};

/** Array of all CRG_CAP base address */
const uint32_t g_crgcap_baseaddress[][BSP_FEATURE_CRAM_GUARD_STATUS_MAX] =
{
#if (BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM0] =
    {
    R_CRGCAP_CRAMHCL0_BASE,
    R_CRGCAP_CRAMLCL0_BASE,
    R_CRGCAP_SX2MBHCL0_BASE,
    R_CRGCAP_SX2MBLCL0_BASE,
    R_CRGCAP_PE0CL0_BASE,
    R_CRGCAP_PE1CL0_BASE,
 #if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE2CL1_BASE,
 #endif
 #if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE3CL1_BASE
 #endif
    },
#endif
#if (BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM1] =
    {
    R_CRGCAP_CRAMHCL1_BASE,
    R_CRGCAP_CRAMLCL1_BASE,
    R_CRGCAP_SX2MBHCL1_BASE,
    R_CRGCAP_SX2MBLCL1_BASE,
    R_CRGCAP_PE0CL0_BASE,
    R_CRGCAP_PE1CL0_BASE,
 #if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE2CL1_BASE,
 #endif
 #if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE3CL1_BASE
 #endif
    },
#endif
#if (BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM2] =
    {
    R_CRGCAP_CRAMHCL2_BASE,
    R_CRGCAP_CRAMLCL2_BASE,
    R_CRGCAP_SX2MBHCL2_BASE,
    R_CRGCAP_SX2MBLCL2_BASE,
    R_CRGCAP_PE0CL0_BASE,
    R_CRGCAP_PE1CL0_BASE,
 #if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE2CL1_BASE,
 #endif
 #if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE3CL1_BASE
 #endif
    },
#endif
#if (BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM3] =
    {
    R_CRGCAP_CRAMHCL3_BASE,
    R_CRGCAP_CRAMLCL3_BASE,
    R_CRGCAP_SX2MBHCL3_BASE,
    R_CRGCAP_SX2MBLCL3_BASE,
    R_CRGCAP_PE0CL0_BASE,
    R_CRGCAP_PE1CL0_BASE,
 #if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE2CL1_BASE,
 #endif
 #if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    R_CRGCAP_PE3CL1_BASE
 #endif
    },
#endif
};

/** Array of all DMA Guard base address */
const uint32_t g_dmacg_baseaddress[] =
{
    [BSP_DMAG_UNIT_0] = (uint32_t) R_GUARD_DMAC0_BASE,
    [BSP_DMAG_UNIT_1] = (uint32_t) R_GUARD_DMAC1_BASE
};

const bsp_module_pbg_info_t g_bsp_module_pgb_info_table[] =
{
    [BSP_GUARD_BSP_MODULE] =
            {
            .no_of_element     = 13,
            .initial_cfg_value = BSP_GUARD_BSP_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG00,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG01,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG10,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_08)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L1,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6H0,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG70,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG80,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            }
            }
            },
    [BSP_GUARD_BIST_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_BIST_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            }
            },
    [BSP_GUARD_TAUJ_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_TAUJ_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_LPS_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_LPS_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_07)
            },
            }
            },
    [BSP_GUARD_ADC_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_ADC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_09)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG70,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_RTCA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_RTCA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_10)
            },
            }
            },
    [BSP_GUARD_WDTBA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_WDTBA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_11)
            },
            }
            },
    [BSP_GUARD_PORT_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_PORT_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG21,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG22,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13)
            },
            }
            },
    [BSP_GUARD_PSI5_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PSI5_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_PSI5S_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PSI5S_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08)
            },
            }
            },
#if (BSP_FEATURE_SCI3_IS_AVAILABLE)
    [BSP_GUARD_SCI3_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SCI3_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_SCI3_IS_AVAILABLE) */
    [BSP_GUARD_ECM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ECM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_12)
            },
            }
            },
    [BSP_GUARD_KCRC_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_KCRC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG31,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
#if (BSP_FEATURE_CXPI_IS_AVAILABLE)
    [BSP_GUARD_CXPI_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_CXPI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG31,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_CXPI_IS_AVAILABLE) */
    [BSP_GUARD_RSENT_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_RSENT_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG31,
            .pbg_channel = (BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13)
            },
            }
            },
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    [BSP_GUARD_MMCA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_MMCA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG32,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */
    [BSP_GUARD_RLIN3_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_RLIN3_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG32,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG53,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_RSCFD_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_RSCFD_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG32,
            .pbg_channel = (BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG33,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG80,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09)
            },
            }
            },
    [BSP_GUARD_MSPI_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_MSPI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10)
            },
            }
            },
    [BSP_GUARD_WTDB_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_WTDB_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_LTSC_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_LTSC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_OSTM_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_OSTM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            }
            },
    [BSP_GUARD_TAUD_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_TAUD_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_TSG_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_TSG_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07)
            },
            }
            },
#if (BSP_FEATURE_ENCA_IS_AVAILABLE)
    [BSP_GUARD_ENCA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ENCA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_ENCA_IS_AVAILABLE) */
    [BSP_GUARD_TAPA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_TAPA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13)
            },
            }
            },
    [BSP_GUARD_TPBA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_TPBA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            }
            },
    [BSP_GUARD_PIC_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PIC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            }
            },
    [BSP_GUARD_FLASH_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FLASH_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_01)
            },
            }
            },
    [BSP_GUARD_OTS_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_OTS_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_EINT_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_EINT_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_PWM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PWM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_FLMD_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FLMD_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L0,
            .pbg_channel = (BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_FACI_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FACI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6L1,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_GTM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_GTM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6H0,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_RIIC_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_RIIC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG6H0,
            .pbg_channel = (BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04)
            },
            }
            },
#if (BSP_FEATURE_RHSIF_IS_AVAILABLE)
    [BSP_GUARD_RHSIF_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_RHSIF_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            {
            .pbg_group   = BSP_HBG_GROUP_HBG91,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
#endif                                 /* #if (BSP_FEATURE_RHSIF_IS_AVAILABLE) */
    [BSP_GUARD_SPIDCTL_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SPIDCTL_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_DMA_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_DMA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG00,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_DTS_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_DTS_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG00,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            }
            }
            },
    [BSP_GUARD_ICUM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ICUM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
#if (BSP_FEATURE_ETNB_IS_AVAILABLE)
    [BSP_GUARD_ETNB_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ETNB_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_ETNB_IS_AVAILABLE) */
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    [BSP_GUARD_FLXA_MODULE] =
            {
 #if (BSP_FEATURE_FLXA_MAX_UNIT == 2)
            .no_of_element       = 2,
 #else
            .no_of_element       = 1,
 #endif
            .initial_cfg_value = BSP_GUARD_FLXA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_HBG_GROUP_HBG92,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
 #if (BSP_FEATURE_FLXA_MAX_UNIT == 2)
            {
            .pbg_group   = BSP_HBG_GROUP_HBG93,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
 #endif
            }
            },
#endif                                 /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE) */
#if (BSP_FEATURE_SFMA_IS_AVAILABLE)
    [BSP_GUARD_SFMA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SFMA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_HBG_GROUP_HBG96,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            }
            },
#endif
};

const bsp_module_ibg_info_t g_bsp_module_ibg_info_table[] =
{
    [BSP_GUARD_IPIR_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_IBG_IPIR_WRITE_PROTECTION,
            .properties        = (bsp_module_ibg_properties_t[]) {
            {
            .ibg_control = BSP_IBG_CONTROL_IPIR_Rn,
            .ibg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_IPIR_Tn,
            .ibg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_IPIR_4,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
    [BSP_GUARD_BARR_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_IBG_BARR_WRITE_PROTECTION,
            .properties        = (bsp_module_ibg_properties_t[]) {
            {
            .ibg_control = BSP_IBG_CONTROL_BARR_n,
            .ibg_channel =
                (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                 BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                 BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                 BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_BARR_16,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
    [BSP_GUARD_TPTM_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_IBG_BARR_WRITE_PROTECTION,
            .properties        = (bsp_module_ibg_properties_t[]) {
#if (BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL == 2U)
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_n,
            .ibg_channel =
                (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
#else
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_n,
            .ibg_channel =
                (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
#endif
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_8,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_9,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
};

const bsp_intc1_lram_info_t g_bsp_intc1_lram_info_table[] =
{
    [BSP_PEG_CONTROL_PE0] =
    {
    .initial_cfg_value = BSP_GUARD_INTC1_LRAM_PE0_WRITE_PROTECTION,
    .lram_peg          = BSP_PEG_CONTROL_PE0,
    },
    [BSP_PEG_CONTROL_PE1] =
    {
    .initial_cfg_value = BSP_GUARD_INTC1_LRAM_PE1_WRITE_PROTECTION,
    .lram_peg          = BSP_PEG_CONTROL_PE1,
    },
#if (BSP_FEATURE_PE2_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE2] =
    {
    .initial_cfg_value = BSP_GUARD_INTC1_LRAM_PE2_WRITE_PROTECTION,
    .lram_peg          = BSP_PEG_CONTROL_PE2,
    },
#endif
#if (BSP_FEATURE_PE3_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE3] =
    {
    .initial_cfg_value = BSP_GUARD_INTC1_LRAM_PE3_WRITE_PROTECTION,
    .lram_peg          = BSP_PEG_CONTROL_PE3,
    }
#endif
};

const bsp_cram_info_t g_bsp_cram_info_table[] =
{
#if (BSP_FEATURE_CRAM0_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM0] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM0_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM0,
    },
#endif
#if (BSP_FEATURE_CRAM1_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM1] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM1_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM1,
    },
#endif
#if (BSP_FEATURE_CRAM2_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM2] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM2_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM2,
    },
#endif
#if (BSP_FEATURE_CRAM3_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM3] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM3_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM3,
    },
#endif
};

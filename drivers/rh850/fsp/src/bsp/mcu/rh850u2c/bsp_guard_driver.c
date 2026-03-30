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
    [BSP_PBG_GROUP_PBG00]   = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG01]   = (uint32_t) R_PBG01_BASE,
    [BSP_PBG_GROUP_PBG10]   = (uint32_t) R_PBG10_BASE,
    [BSP_PBG_GROUP_PBG20]   = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG21]   = (uint32_t) R_PBG21_BASE,
    [BSP_PBG_GROUP_PBG22]   = (uint32_t) R_PBG22_BASE,
    [BSP_PBG_GROUP_PBG30]   = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG31]   = (uint32_t) R_PBG31_BASE,
    [BSP_PBG_GROUP_PBG40]   = (uint32_t) R_PBG40_BASE,
    [BSP_PBG_GROUP_PBG41]   = (uint32_t) R_PBG41_BASE,
    [BSP_PBG_GROUP_PBG50]   = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG51]   = (uint32_t) R_PBG51_BASE,
    [BSP_PBG_GROUP_PBG52]   = (uint32_t) R_PBG52_BASE,
    [BSP_PBG_GROUP_PBG60]   = (uint32_t) R_PBG60_BASE,
    [BSP_PBG_GROUP_PBG61]   = (uint32_t) R_PBG61_BASE,
    [BSP_PBG_GROUP_PBG7H0]  = (uint32_t) R_PBG7H0_BASE,
    [BSP_PBG_GROUP_PBG7L0]  = (uint32_t) R_PBG7L0_BASE,
    [BSP_PBG_GROUP_PBG8H0]  = (uint32_t) R_PBG8H0_BASE,
    [BSP_PBG_GROUP_PBG8L0]  = (uint32_t) R_PBG8L0_BASE,
    [BSP_PBG_GROUP_PBG90]   = (uint32_t) R_PBG90_BASE,
    [BSP_PBG_GROUP_PBG91]   = (uint32_t) R_PBG91_BASE,
    [BSP_PBG_GROUP_PBG100]  = (uint32_t) R_PBG100_BASE,
    [BSP_PBG_GROUP_PBG101]  = (uint32_t) R_PBG101_BASE,
    [BSP_PBG_GROUP_PBG11H0] = (uint32_t) R_PBG11H0_BASE,
    [BSP_PBG_GROUP_PBG11H1] = (uint32_t) R_PBG11H1_BASE,
    [BSP_PBG_GROUP_PBG11L0] = (uint32_t) R_PBG11L0_BASE,
    [BSP_HBG_GROUP_HBG90]   = (uint32_t) R_HBG90_BASE,
    [BSP_HBG_GROUP_HBG91]   = (uint32_t) R_HBG91_BASE,
    [BSP_HBG_GROUP_HBG92]   = (uint32_t) R_HBG92_BASE,
    [BSP_HBG_GROUP_HBG93]   = (uint32_t) R_HBG93_BASE,
    [BSP_HBG_GROUP_HBG96]   = (uint32_t) R_HBG96_BASE,
};

/** Array of all PBG control groups base address */
const uint32_t g_pbgprot_control_baseaddress[] =
{
    [BSP_PBG_GROUP_PBG00]   = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG01]   = (uint32_t) R_PBG00_BASE,
    [BSP_PBG_GROUP_PBG10]   = (uint32_t) R_PBG10_BASE,
    [BSP_PBG_GROUP_PBG20]   = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG21]   = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG22]   = (uint32_t) R_PBG20_BASE,
    [BSP_PBG_GROUP_PBG30]   = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG31]   = (uint32_t) R_PBG30_BASE,
    [BSP_PBG_GROUP_PBG40]   = (uint32_t) R_PBG40_BASE,
    [BSP_PBG_GROUP_PBG41]   = (uint32_t) R_PBG40_BASE,
    [BSP_PBG_GROUP_PBG50]   = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG51]   = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG52]   = (uint32_t) R_PBG50_BASE,
    [BSP_PBG_GROUP_PBG60]   = (uint32_t) R_PBG60_BASE,
    [BSP_PBG_GROUP_PBG61]   = (uint32_t) R_PBG61_BASE,
    [BSP_PBG_GROUP_PBG7H0]  = (uint32_t) R_PBG7H0_BASE,
    [BSP_PBG_GROUP_PBG7L0]  = (uint32_t) R_PBG7L0_BASE,
    [BSP_PBG_GROUP_PBG8H0]  = (uint32_t) R_PBG8H0_BASE,
    [BSP_PBG_GROUP_PBG8L0]  = (uint32_t) R_PBG8L0_BASE,
    [BSP_PBG_GROUP_PBG90]   = (uint32_t) R_PBG90_BASE,
    [BSP_PBG_GROUP_PBG91]   = (uint32_t) R_PBG91_BASE,
    [BSP_PBG_GROUP_PBG100]  = (uint32_t) R_PBG100_BASE,
    [BSP_PBG_GROUP_PBG101]  = (uint32_t) R_PBG100_BASE,
    [BSP_PBG_GROUP_PBG11H0] = (uint32_t) R_PBG11H0_BASE,
    [BSP_PBG_GROUP_PBG11H1] = (uint32_t) R_PBG11H0_BASE,
    [BSP_PBG_GROUP_PBG11L0] = (uint32_t) R_PBG11L0_BASE,
    [BSP_HBG_GROUP_HBG90]   = (uint32_t) R_HBG90_BASE,
    [BSP_HBG_GROUP_HBG91]   = (uint32_t) R_HBG91_BASE,
    [BSP_HBG_GROUP_HBG92]   = (uint32_t) R_HBG92_BASE,
    [BSP_HBG_GROUP_HBG93]   = (uint32_t) R_HBG93_BASE,
    [BSP_HBG_GROUP_HBG96]   = (uint32_t) R_HBG96_BASE,
};

/** Array of all PBG Key protection groups base address */
const uint32_t g_pbgkcprot_baseaddress[] =
{
    [BSP_PBG_GROUP_PBG00]   = (uint32_t) R_PBGERRSLV00_BASE,
    [BSP_PBG_GROUP_PBG01]   = (uint32_t) R_PBGERRSLV00_BASE,
    [BSP_PBG_GROUP_PBG10]   = (uint32_t) R_PBGERRSLV10_BASE,
    [BSP_PBG_GROUP_PBG20]   = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG21]   = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG22]   = (uint32_t) R_PBGERRSLV20_BASE,
    [BSP_PBG_GROUP_PBG30]   = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG31]   = (uint32_t) R_PBGERRSLV30_BASE,
    [BSP_PBG_GROUP_PBG40]   = (uint32_t) R_PBGERRSLV40_BASE,
    [BSP_PBG_GROUP_PBG41]   = (uint32_t) R_PBGERRSLV40_BASE,
    [BSP_PBG_GROUP_PBG50]   = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG51]   = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG52]   = (uint32_t) R_PBGERRSLV50_BASE,
    [BSP_PBG_GROUP_PBG60]   = (uint32_t) R_PBGERRSLV60_BASE,
    [BSP_PBG_GROUP_PBG61]   = (uint32_t) R_PBGERRSLV61_BASE,
    [BSP_PBG_GROUP_PBG7H0]  = (uint32_t) R_PBGERRSLV7H0_BASE,
    [BSP_PBG_GROUP_PBG7L0]  = (uint32_t) R_PBGERRSLV7L0_BASE,
    [BSP_PBG_GROUP_PBG8H0]  = (uint32_t) R_PBGERRSLV8H0_BASE,
    [BSP_PBG_GROUP_PBG8L0]  = (uint32_t) R_PBGERRSLV8L0_BASE,
    [BSP_PBG_GROUP_PBG90]   = (uint32_t) R_PBGERRSLV90_BASE,
    [BSP_PBG_GROUP_PBG91]   = (uint32_t) R_PBGERRSLV91_BASE,
    [BSP_PBG_GROUP_PBG100]  = (uint32_t) R_PBGERRSLV100_BASE,
    [BSP_PBG_GROUP_PBG101]  = (uint32_t) R_PBGERRSLV100_BASE,
    [BSP_PBG_GROUP_PBG11H0] = (uint32_t) R_PBGERRSLV11H0_BASE,
    [BSP_PBG_GROUP_PBG11H1] = (uint32_t) R_PBGERRSLV11H0_BASE,
    [BSP_PBG_GROUP_PBG11L0] = (uint32_t) R_PBGERRSLV11L0_BASE,
    [BSP_HBG_GROUP_HBG90]   = (uint32_t) R_HBGERRSLV90_BASE,
    [BSP_HBG_GROUP_HBG91]   = (uint32_t) R_HBGERRSLV91_BASE,
    [BSP_HBG_GROUP_HBG92]   = (uint32_t) R_HBGERRSLV92_BASE,
    [BSP_HBG_GROUP_HBG93]   = (uint32_t) R_HBGERRSLV93_BASE,
    [BSP_HBG_GROUP_HBG96]   = (uint32_t) R_HBGERRSLV96_BASE,
};

/** Array of all IBG control base address */
const uint32_t g_ibg_baseaddress[] =
{
#if (BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE)
    [BSP_IBG_CONTROL_IPIR_Rn] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_IBG_CONTROL_IPIR_Tn] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
    [BSP_IBG_CONTROL_IPIR_4]  = (uint32_t) R_GUARD_IBG_IPIR_BASE,
#endif
#if (BSP_FEATURE_BARR_GUARD_IS_AVAILABLE)
    [BSP_IBG_CONTROL_BARR_n]  = (uint32_t) R_GUARD_IBG_BARR_BASE,
    [BSP_IBG_CONTROL_BARR_16] = (uint32_t) R_GUARD_IBG_BARR_BASE,
#endif
    [BSP_IBG_CONTROL_TPTM_IFn] = (uint32_t) R_GUARD_IBG_TPTM_BASE,
    [BSP_IBG_CONTROL_TPTM_Un]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
    [BSP_IBG_CONTROL_TPTM_U8]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
    [BSP_IBG_CONTROL_TPTM_U9]  = (uint32_t) R_GUARD_IBG_TPTM_BASE,
};

/** Array of all IBG protection control base address */
const uint32_t g_ibgprot_baseaddress[] =
{
#if (BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE)
    [BSP_IBG_CONTROL_IPIR_Rn] = (uint32_t) R_IBG00_IPIGPROT0_Rn_BASE,
    [BSP_IBG_CONTROL_IPIR_Tn] = (uint32_t) R_IBG00_IPIGPROT0_Tn_BASE,
    [BSP_IBG_CONTROL_IPIR_4]  = (uint32_t) R_IBG00_IPIGPROT0_4_BASE,
#endif
#if (BSP_FEATURE_BARR_GUARD_IS_AVAILABLE)
    [BSP_IBG_CONTROL_BARR_n]  = (uint32_t) R_IBG01_BRGPROT0_n_BASE,
    [BSP_IBG_CONTROL_BARR_16] = (uint32_t) R_IBG01_BRGPROT0_16_BASE,
#endif
    [BSP_IBG_CONTROL_TPTM_IFn] = (uint32_t) R_IBG02_TPTGPROT0_IFn_BASE,
    [BSP_IBG_CONTROL_TPTM_Un]  = (uint32_t) R_IBG02_TPTGPROT0_Un_BASE,
    [BSP_IBG_CONTROL_TPTM_U8]  = (uint32_t) R_IBG02_TPTGPROT0_U8_BASE,
    [BSP_IBG_CONTROL_TPTM_U9]  = (uint32_t) R_IBG02_TPTGPROT0_U9_BASE,
};

/** Array of all IBG module base address */
const uint32_t g_ibg_module_baseaddress[] =
{
#if (BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE)
    [BSP_GUARD_IPIR_MODULE] = (uint32_t) R_GUARD_IBG_IPIR_BASE,
#endif
#if (BSP_FEATURE_BARR_GUARD_IS_AVAILABLE)
    [BSP_GUARD_BARR_MODULE] = (uint32_t) R_GUARD_IBG_BARR_BASE,
#endif
    [BSP_GUARD_TPTM_MODULE] = (uint32_t) R_GUARD_IBG_TPTM_BASE,
};

/** Array of all PEG base address */
const uint32_t g_peg_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_GUARD_PE0_BASE,
#if (BSP_FEATURE_PE1_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_GUARD_PE1_BASE,
#endif
};

/** Array of all PEGCAP_S base address */
const uint32_t g_pegcap_s_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_PEGCAP_S_PE0CL0_BASE,
#if (BSP_FEATURE_PE1_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_PEGCAP_S_PE1CL0_BASE,
#endif
};

/** Array of all Local Ram base address */
const uint32_t g_peg_lram_baseaddress[] =
{
    [BSP_PEG_CONTROL_PE0] = (uint32_t) R_GUARD_PE0_LRAM_BASE,
#if (BSP_FEATURE_PE1_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE1] = (uint32_t) R_GUARD_PE1_LRAM_BASE,
#endif
};

/** Array of all CRG base address */
const uint32_t g_crg_baseaddress[] =
{
#if (BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM00] = (uint32_t) R_GUARD_CRAMCRG00_BASE,
#endif
#if (BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM01] = (uint32_t) R_GUARD_CRAMCRG01_BASE,
#endif
#if (BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM02] = (uint32_t) R_GUARD_CRAMCRG02_BASE
#endif
};

/** Array of all Cluster Ram base address */
const uint32_t g_crg_cram_baseaddress[] =
{
#if (BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM00] = (uint32_t) R_GUARD_CRG_CRAM00_BASE,
#endif
#if (BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM01] = (uint32_t) R_GUARD_CRG_CRAM01_BASE,
#endif
#if (BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM02] = (uint32_t) R_GUARD_CRG_CRAM02_BASE
#endif
};

const uint32_t g_crgcap_baseaddress[][BSP_FEATURE_CRAM_GUARD_STATUS_MAX] =
{
#if (BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM00] =
    {
    R_CRGCAP_CRAMHCL00_BASE,
    R_CRGCAP_CRAMLCL00_BASE,
    },
#endif
#if (BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM01] =
    {
    R_CRGCAP_CRAMHCL01_BASE,
    R_CRGCAP_CRAMLCL01_BASE,
    },
#endif
#if (BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM02] =
    {
    R_CRGCAP_CRAMHCL02_BASE,
    R_CRGCAP_CRAMLCL02_BASE,
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
            .no_of_element     = 18,
            .initial_cfg_value = BSP_GUARD_BSP_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG00,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG01,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
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
            .pbg_group   = BSP_PBG_GROUP_PBG60,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG61,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG7H0,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG7L0,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8H0,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8L0,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG91,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG100,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H0,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11L0,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
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
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11)
            },
            }
            },
    [BSP_GUARD_LPS_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_LPS_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_07)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG7L0,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_ADC_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ADC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_09)
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
    [BSP_GUARD_WDTB_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_WDTB_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_11)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H0,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_AVSEG_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_AVSEG_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG20,
            .pbg_channel = (BSP_PBG_CHANNEL_12)
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
            .pbg_channel = (BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG21,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG22,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11)
            },
            }
            },
    [BSP_GUARD_KCRC_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_KCRC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_MSPI_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_MSPI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10)
            },
            }
            },
    [BSP_GUARD_RSENT_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_RSENT_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG30,
            .pbg_channel = (BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            }
            },
    [BSP_GUARD_SSIF_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_SSIF_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG31,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG41,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
            }
            },
    [BSP_GUARD_DMA_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_DMA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_11)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
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
            .pbg_group   = BSP_PBG_GROUP_PBG40,
            .pbg_channel = (BSP_PBG_CHANNEL_11)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            }
            },
    [BSP_GUARD_PIC_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PIC_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_ENCA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ENCA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
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
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_SWDT_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SWDT_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_ECM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ECM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_07)
            },
            }
            },
    [BSP_GUARD_TAUD_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_TAUD_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H0,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_TSG_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_TSG_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_12)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H1,
            .pbg_channel = (BSP_PBG_CHANNEL_06)
            },
            }
            },
    [BSP_GUARD_TAPA_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_TAPA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG50,
            .pbg_channel = (BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H1,
            .pbg_channel = (BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_OSTM_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_OSTM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H0,
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_RLIN3_MODULE] =
            {
            .no_of_element     = 3,
            .initial_cfg_value = BSP_GUARD_RLIN3_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8L0,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H0,
            .pbg_channel = (BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_12 | BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
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
            .pbg_group   = BSP_PBG_GROUP_PBG51,
            .pbg_channel = (BSP_PBG_CHANNEL_15)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */
    [BSP_GUARD_OTS_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_OTS_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG60,
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
            .pbg_group   = BSP_PBG_GROUP_PBG60,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_FLASH_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FLASH_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG60,
            .pbg_channel = (BSP_PBG_CHANNEL_04)
            },
            }
            },
    [BSP_GUARD_FLMD_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FLMD_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG60,
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_FACI_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FACI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG61,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_GTM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_GTM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8H0,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    [BSP_GUARD_ETND_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ETND_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8L0,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_ETND_IS_AVAILABLE) */
    [BSP_GUARD_RI3C_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_RI3C_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8L0,
            .pbg_channel = (BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11)
            },
            }
            },
    [BSP_GUARD_ETNF_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_ETNF_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG8L0,
            .pbg_channel = (BSP_PBG_CHANNEL_12)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_05)
            },
            }
            },
    [BSP_GUARD_ICUM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ICUM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_SPID_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SPID_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_ACEU_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_ACEU_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG91,
            .pbg_channel = (BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02)
            },
            }
            },
    [BSP_GUARD_RSCAN_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_RSCAN_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG100,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05 | BSP_PBG_CHANNEL_06 | BSP_PBG_CHANNEL_07 |
                            BSP_PBG_CHANNEL_08 | BSP_PBG_CHANNEL_09 | BSP_PBG_CHANNEL_10 | BSP_PBG_CHANNEL_11 |
                            BSP_PBG_CHANNEL_13 | BSP_PBG_CHANNEL_14 | BSP_PBG_CHANNEL_15)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG101,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01 | BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03 |
                            BSP_PBG_CHANNEL_04)
            },
            }
            },
    [BSP_GUARD_CXPI_MODULE] =
            {
            .no_of_element     = 2,
            .initial_cfg_value = BSP_GUARD_CXPI_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11H1,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_03)
            },
            }
            },
    [BSP_GUARD_PWM_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PWM_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG11L0,
            .pbg_channel = (BSP_PBG_CHANNEL_01)
            },
            }
            },
    [BSP_GUARD_PSI5_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PSI5_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_02 | BSP_PBG_CHANNEL_04 | BSP_PBG_CHANNEL_05)
            },
            }
            },
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    [BSP_GUARD_PSI5S_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_PSI5S_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_PBG_GROUP_PBG52,
            .pbg_channel = (BSP_PBG_CHANNEL_03)
            },
            }
            },
#endif                                 /* #if (BSP_FEATURE_PSI5S_IS_AVAILABLE)*/
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    [BSP_GUARD_FLXA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_FLXA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_HBG_GROUP_HBG90,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
#endif                                 /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE)*/
    [BSP_GUARD_SFMA_MODULE] =
            {
            .no_of_element     = 1,
            .initial_cfg_value = BSP_GUARD_SFMA_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_HBG_GROUP_HBG91,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            }
            }
            },
    [BSP_GUARD_CANXL_MODULE] =
            {
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
            .no_of_element       = 2,
#else
            .no_of_element       = 1,
#endif
            .initial_cfg_value = BSP_GUARD_CANXL_WRITE_PROTECTION,
            .properties        = (bsp_module_pbg_properties_t[]) {
            {
            .pbg_group   = BSP_HBG_GROUP_HBG92,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            },
#if (BSP_FEATURE_CANXL_CH1_IS_AVAILABLE)
            {
            .pbg_group   = BSP_HBG_GROUP_HBG93,
            .pbg_channel = (BSP_PBG_CHANNEL_00)
            }
#endif
            }
            },
};

const bsp_module_ibg_info_t g_bsp_module_ibg_info_table[] =
{
#if (BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE)
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
#endif
#if (BSP_FEATURE_BARR_GUARD_IS_AVAILABLE)
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
#endif
    [BSP_GUARD_TPTM_MODULE] =
            {
            .no_of_element     = 4,
            .initial_cfg_value = BSP_GUARD_IBG_TPTM_WRITE_PROTECTION,
            .properties        = (bsp_module_ibg_properties_t[]) {
#if (BSP_FEATURE_TPTMn_GUARD_MAX_CHANNEL == 1)
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_IFn,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_Un,
            .ibg_channel =
                (BSP_PBG_CHANNEL_00)
            },
#else
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_IFn,
            .ibg_channel = (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_Un,
            .ibg_channel =
                (BSP_PBG_CHANNEL_00 | BSP_PBG_CHANNEL_01)
            },
#endif
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_U8,
            .ibg_channel = (BSP_PBG_CHANNEL_00)
            },
            {
            .ibg_control = BSP_IBG_CONTROL_TPTM_U9,
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
#if (BSP_FEATURE_PE1_GUARD_IS_AVAILABLE)
    [BSP_PEG_CONTROL_PE1] =
    {
    .initial_cfg_value = BSP_GUARD_INTC1_LRAM_PE1_WRITE_PROTECTION,
    .lram_peg          = BSP_PEG_CONTROL_PE1,
    },
#endif
};

const bsp_cram_info_t g_bsp_cram_info_table[] =
{
#if (BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM00] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM0_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM00,
    },
#endif
#if (BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM01] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM0_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM01,
    },
#endif
#if (BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE)
    [BSP_CRG_CONTROL_CRAM02] =
    {
    .initial_cfg_value = BSP_GUARD_CRAM0_WRITE_PROTECTION,
    .cram_crg          = BSP_CRG_CONTROL_CRAM02,
    },
#endif
};

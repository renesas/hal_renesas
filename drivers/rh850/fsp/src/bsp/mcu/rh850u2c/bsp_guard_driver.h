/*
* Copyright (c) 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_GUARD_DRIVER_H
#define BSP_GUARD_DRIVER_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Guard and Protection related base values and offsets */
#define BSP_GUARD_READ_WRITE_ACCESS              0x153                                 ///< Full Read/Write access in Supervisor and User modes

#define BSP_GUARD_CHANNEL_DEFAULT                (0)                                   ///< Default guard channel index

#define BSP_GUARD_KCPROT_ADDRESS_OFFSET          0x18                                  ///< KCProt register offset for Guard

#define BSP_PBG_CHANNEL_ADDRESS_OFFSET           0x08                                  ///< PBG channel register offset
#define BSP_PBG_KCPROT_ADDRESS_OFFSET            0x18                                  ///< PBG KCProt register offset

#define BSP_IBG_IPIR_PROT_CHANNEL_OFFSET         0x10                                  ///< IBG IPIR protection channel offset
#define BSP_IBG_BARR_TPTM_PROT_CHANNEL_OFFSET    0x08                                  ///< IBG BARR/TPTM protection channel offset

/** Base addresses for Guard RAM and CRAM regions */
#define R_GUARD_PE0_LRAM_BASE                    0xFDC00000UL                          ///< Base address of PE0 Local RAM Guard
#define R_GUARD_PE1_LRAM_BASE                    0xFDA00000UL                          ///< Base address of PE1 Local RAM Guard
#define R_GUARD_CRG_CRAM00_BASE                  0xFE000000UL                          ///< Base address of CRG CRAM00 Guard
#define R_GUARD_CRG_CRAM01_BASE                  0xFE028000UL                          ///< Base address of CRG CRAM01 Guard
#define R_GUARD_CRG_CRAM02_BASE                  0xFE070000UL                          ///< Base address of CRG CRAM02 Guard

/** IBG base addresses */
#define R_IBG00_IPIGPROT0_Rn_BASE                0xFFC6A280UL                          ///< IBG00 IPIGPROT0 Read channel register base
#define R_IBG00_IPIGPROT0_Tn_BASE                0xFFC6A288UL                          ///< IBG00 IPIGPROT0 Target channel register base
#define R_IBG00_IPIGPROT0_4_BASE                 0xFFC6A2C0UL                          ///< IBG00 IPIGPROT0 channel 4 base

#define R_IBG01_BRGPROT0_n_BASE                  0xFFC6A080UL                          ///< IBG01 BRGPROT0 channel n base
#define R_IBG01_BRGPROT0_16_BASE                 0xFFC6A100UL                          ///< IBG01 BRGPROT0 channel 16 base

#define R_IBG02_TPTGPROT0_IFn_BASE               0xFFC6A680UL                          ///< IBG02 TPTGPROT0 IFn base
#define R_IBG02_TPTGPROT0_Un_BASE                0xFFC6A6C0UL                          ///< IBG02 TPTGPROT0 Un base
#define R_IBG02_TPTGPROT0_U8_BASE                0xFFC6A700UL                          ///< IBG02 TPTGPROT0 U8 base
#define R_IBG02_TPTGPROT0_U9_BASE                0xFFC6A708UL                          ///< IBG02 TPTGPROT0 U9 base

/** Utility constants */
#define BSP_GUARD_MAX_32BITS                     0xFFFFFFFFUL                          ///< Maximum 32-bit unsigned value

/** Alias definitions for CRAM Guard registers and types */
#define R_GUARD_CRAMCRG0_CRG_CTR_ADV_ADV_Pos     R_GUARD_CRAMCRG00_CRG_CTR_ADV_ADV_Pos ///< Alias for CRAM Control ADV position
#define R_GUARD_CRAMCRG0_Type                    R_GUARD_CRAMCRG00_Type                ///< Alias for CRAMCRG0 type
#define R_CRGCAP_PE0CL0_Type                     R_CRGCAP_CRAMHCL00_Type               ///< Alias for PE0CL0 CRGCAP type
#define R_CRGCAP_CRAMHCL0_Type                   R_CRGCAP_CRAMHCL00_Type               ///< Alias for CRAMHCL0 CRGCAP type
#define BSP_CRG_CONTROL_CRAM0                    BSP_CRG_CONTROL_CRAM00                ///< Alias for CRAM0 Control definition

/** Utility macro to get bit index (SCH1R encoding) */
#define BSP_GUARD_GET_BIT(value)    (SCH1R(value) - 1)                                 ///< Search bit 1 from the LSB

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU_RH850U2CX
 * @defgroup BSP_GUARD_U2CX BSP Guard Protection
 *
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** PBus and HBus Guard group */
typedef enum e_bsp_pbg_group
{
    BSP_PBG_GROUP_PBG00 = 0,           ///< PBG Group PBG00
    BSP_PBG_GROUP_PBG01,               ///< PBG Group PBG01
    BSP_PBG_GROUP_PBG10,               ///< PBG Group PBG10
    BSP_PBG_GROUP_PBG20,               ///< PBG Group PBG20
    BSP_PBG_GROUP_PBG21,               ///< PBG Group PBG21
    BSP_PBG_GROUP_PBG22,               ///< PBG Group PBG22
    BSP_PBG_GROUP_PBG30,               ///< PBG Group PBG30
    BSP_PBG_GROUP_PBG31,               ///< PBG Group PBG31
    BSP_PBG_GROUP_PBG40,               ///< PBG Group PBG40
    BSP_PBG_GROUP_PBG41,               ///< PBG Group PBG41
    BSP_PBG_GROUP_PBG50,               ///< PBG Group PBG50
    BSP_PBG_GROUP_PBG51,               ///< PBG Group PBG51
    BSP_PBG_GROUP_PBG52,               ///< PBG Group PBG52
    BSP_PBG_GROUP_PBG60,               ///< PBG Group PBG60
    BSP_PBG_GROUP_PBG61,               ///< PBG Group PBG61
    BSP_PBG_GROUP_PBG7H0,              ///< PBG Group PBG7H0
    BSP_PBG_GROUP_PBG7L0,              ///< PBG Group PBG7L0
    BSP_PBG_GROUP_PBG8H0,              ///< PBG Group PBG8H0
    BSP_PBG_GROUP_PBG8L0,              ///< PBG Group PBG8L0
    BSP_PBG_GROUP_PBG90,               ///< PBG Group PBG90
    BSP_PBG_GROUP_PBG91,               ///< PBG Group PBG91
    BSP_PBG_GROUP_PBG100,              ///< PBG Group PBG100
    BSP_PBG_GROUP_PBG101,              ///< PBG Group PBG101
    BSP_PBG_GROUP_PBG11H0,             ///< PBG Group PBG11H0
    BSP_PBG_GROUP_PBG11H1,             ///< PBG Group PBG11H1
    BSP_PBG_GROUP_PBG11L0,             ///< PBG Group PBG11L0

    BSP_HBG_GROUP_HBG90,               ///< HBG Group HBG90
    BSP_HBG_GROUP_HBG91,               ///< HBG Group HBG91
    BSP_HBG_GROUP_HBG92,               ///< HBG Group HBG92
    BSP_HBG_GROUP_HBG93,               ///< HBG Group HBG93
    BSP_HBG_GROUP_HBG96,               ///< HBG Group HBG96

    BSP_PBG_GROUP_NUM                  ///< Total number of PBG/HBG groups
} bsp_pbg_group_t;

/** Guard module IDs*/
typedef enum e_bsp_module_pbg_id
{
    BSP_GUARD_BSP_MODULE,              ///< Guard module BSP
    BSP_GUARD_BIST_MODULE,             ///< Guard module BIST
    BSP_GUARD_TAUJ_MODULE,             ///< Guard module TAUJ
    BSP_GUARD_LPS_MODULE,              ///< Guard module LPS
    BSP_GUARD_ADC_MODULE,              ///< Guard module ADC
    BSP_GUARD_RTCA_MODULE,             ///< Guard module RTCA
    BSP_GUARD_WDTB_MODULE,             ///< Guard module WDTB
    BSP_GUARD_AVSEG_MODULE,            ///< Guard module AVSEG
    BSP_GUARD_PORT_MODULE,             ///< Guard module PORT
    BSP_GUARD_KCRC_MODULE,             ///< Guard module KCRC
    BSP_GUARD_MSPI_MODULE,             ///< Guard module MSPI
    BSP_GUARD_RSENT_MODULE,            ///< Guard module RSENT
    BSP_GUARD_SSIF_MODULE,             ///< Guard module SSIF
    BSP_GUARD_DMA_MODULE,              ///< Guard module DMA
    BSP_GUARD_DTS_MODULE,              ///< Guard module DTS
    BSP_GUARD_PIC_MODULE,              ///< Guard module PIC
    BSP_GUARD_ENCA_MODULE,             ///< Guard module ENCA
    BSP_GUARD_LTSC_MODULE,             ///< Guard module LTSC
    BSP_GUARD_SWDT_MODULE,             ///< Guard module SWDT
    BSP_GUARD_ECM_MODULE,              ///< Guard module ECM
    BSP_GUARD_TAUD_MODULE,             ///< Guard module TAUD
    BSP_GUARD_TSG_MODULE,              ///< Guard module TSG
    BSP_GUARD_TAPA_MODULE,             ///< Guard module TAPA
    BSP_GUARD_OSTM_MODULE,             ///< Guard module OSTM
    BSP_GUARD_RLIN3_MODULE,            ///< Guard module RLIN3
#if (BSP_FEATURE_MMCA_IS_AVAILABLE)
    BSP_GUARD_MMCA_MODULE,             ///< Guard module MMCA
#endif /* #if (BSP_FEATURE_MMCA_IS_AVAILABLE) */
    BSP_GUARD_OTS_MODULE,              ///< Guard module OTS
    BSP_GUARD_EINT_MODULE,             ///< Guard module EINT
    BSP_GUARD_FLASH_MODULE,            ///< Guard module FLASH
    BSP_GUARD_FLMD_MODULE,             ///< Guard module FLMD
    BSP_GUARD_FACI_MODULE,             ///< Guard module FACI
    BSP_GUARD_GTM_MODULE,              ///< Guard module GTM
#if (BSP_FEATURE_ETND_IS_AVAILABLE)
    BSP_GUARD_ETND_MODULE,             ///< Guard module ETND
#endif /* #if (BSP_FEATURE_ETND_IS_AVAILABLE) */
    BSP_GUARD_RI3C_MODULE,             ///< Guard module RI3C
    BSP_GUARD_ETNF_MODULE,             ///< Guard module ETNF
    BSP_GUARD_ICUM_MODULE,             ///< Guard module ICUM
    BSP_GUARD_SPID_MODULE,             ///< Guard module SPID
    BSP_GUARD_ACEU_MODULE,             ///< Guard module ACEU
    BSP_GUARD_RSCAN_MODULE,            ///< Guard module RSCAN
    BSP_GUARD_CXPI_MODULE,             ///< Guard module CXPI
    BSP_GUARD_PWM_MODULE,              ///< Guard module PWM
    BSP_GUARD_PSI5_MODULE,             ///< Guard module PSI5
#if (BSP_FEATURE_PSI5S_IS_AVAILABLE)
    BSP_GUARD_PSI5S_MODULE,            ///< Guard module PSI5S
#endif /* #if (BSP_FEATURE_PSI5S_IS_AVAILABLE) */
#if (BSP_FEATURE_FLXA_IS_AVAILABLE)
    BSP_GUARD_FLXA_MODULE,             ///< Guard module FLXA
#endif /* #if (BSP_FEATURE_FLXA_IS_AVAILABLE) */
    BSP_GUARD_SFMA_MODULE,             ///< Guard module SFMA
    BSP_GUARD_CANXL_MODULE,            ///< Guard module CANXL
    BSP_MODULE_PBG_TOTAL_INDEX_END     ///< Total number of PBG module IDs
} bsp_module_pbg_id_t;

/** Guard types for configuring/status protection */
typedef enum e_bsp_guard_type
{
    BSP_GUARD_TYPE_PE_GUARD,           ///< PE Guard: Protects PE Local RAM and INTC1 registers
    BSP_GUARD_TYPE_CRAM_GUARD,         ///< Cluster RAM Guard: Protects Cluster RAM area
    BSP_GUARD_TYPE_IBUS_GUARD,         ///< I-Bus Guard: Protects I-Bus modules
    BSP_GUARD_TYPE_PBUS_GUARD,         ///< Peripheral Bus Guard: Protects PBG/HBG groups
    BSP_GUARD_TYPE_INTC2_GUARD,        ///< INTC2 Guard: Protects INTC2 registers
    BSP_GUARD_TYPE_DTS_GUARD,          ///< DTS Guard: Protects DTS registers
    BSP_GUARD_TYPE_DMAC_GUARD,         ///< sDMAC Guard: Protects sDMAC registers
    BSP_GUARD_TYPE_NUM                 ///< Number of guard types
} bsp_guard_type_t;

/** Guard control register types */
typedef enum e_bsp_guard_ctr_reg
{
    BSP_GUARD_REG_PROT = 0,            ///< Protection control register
    BSP_GUARD_REG_SPID,                ///< SPID setting register
    BSP_GUARD_REG_BAD,                 ///< Base address register
    BSP_GUARD_REG_ADV,                 ///< Address valid setting register
    BSP_GUARD_REG_COUNT                ///< Total number of guard control registers
} bsp_guard_ctr_reg_t;

/** I-Bus guard control target */
typedef enum e_bsp_ibg_control
{
    BSP_IBG_CONTROL_IPIR_Rn = 0,       ///< I-Bus group0 registers (Rn)
    BSP_IBG_CONTROL_IPIR_Tn,           ///< I-Bus group0 registers (Tn)
    BSP_IBG_CONTROL_IPIR_4,            ///< I-Bus group0 register (4)
    BSP_IBG_CONTROL_BARR_n,            ///< I-Bus group1 registers (n)
    BSP_IBG_CONTROL_BARR_16,           ///< I-Bus group1 register (16)
    BSP_IBG_CONTROL_TPTM_IFn,          ///< I-Bus group2 registers (IFn)
    BSP_IBG_CONTROL_TPTM_Un,           ///< I-Bus group2 registers (Un)
    BSP_IBG_CONTROL_TPTM_U8,           ///< I-Bus group2 register (U8)
    BSP_IBG_CONTROL_TPTM_U9,           ///< I-Bus group2 register (U9)
    BSP_IBG_CONTROL_NUM
} bsp_ibg_control_t;

/** I-Bus guard module IDs */
typedef enum e_bsp_module_ibg_id
{
#if (BSP_FEATURE_IPIR_GUARD_IS_AVAILABLE)
    BSP_GUARD_IPIR_MODULE = 0,         ///< IPIR module
#endif
#if (BSP_FEATURE_BARR_GUARD_IS_AVAILABLE)
    BSP_GUARD_BARR_MODULE,             ///< BARR module
#endif
    BSP_GUARD_TPTM_MODULE,             ///< TPTM module
    BSP_MODULE_IBG_TOTAL_INDEX_END
} bsp_module_ibg_id_t;

/** PE guard control targets */
typedef enum e_bsp_peg_control
{
    BSP_PEG_CONTROL_PE0 = 0,           ///< Guard for PE0 Local RAM area & INTC1 register area
#if (BSP_FEATURE_PE1_GUARD_IS_AVAILABLE)
    BSP_PEG_CONTROL_PE1,               ///< Guard for PE1 Local RAM area & INTC1 register area
#endif
    BSP_PEG_CONTROL_NUM
} bsp_peg_control_t;

/** Cluster RAM guard control targets */
typedef enum e_bsp_crg_control
{
#if (BSP_FEATURE_CRAM00_GUARD_IS_AVAILABLE)
    BSP_CRG_CONTROL_CRAM00 = 0,        ///< Guard for Cluster 0 RAM0
#endif
#if (BSP_FEATURE_CRAM01_GUARD_IS_AVAILABLE)
    BSP_CRG_CONTROL_CRAM01,            ///< Guard for Cluster 0 RAM1
#endif
#if (BSP_FEATURE_CRAM02_GUARD_IS_AVAILABLE)
    BSP_CRG_CONTROL_CRAM02,            ///< Guard for Cluster 0 RAM2
#endif
    BSP_CRG_CONTROL_NUM
} bsp_crg_control_t;

/** INTC2 guard control targets (Group register) */
typedef enum e_bsp_intc2g_control
{
    BSP_INTC2G_CONTROL_EIBD_I2EIBG = 0, ///< Guard access to EIBDn/I2EIBGn register
    BSP_INTC2G_CONTROL_IMR,             ///< Guard access to IMRn register
    BSP_INTC2G_CONTROL_EIC_EEIC,        ///< Guard access to EICn/EEICn register
    BSP_INTC2G_CONTROL_NUM
} bsp_intc2g_control_t;

/** DTS guard control targets (Group register) */
typedef enum e_bsp_dtsg_control
{
    BSP_DTSG_CONTROL_COMMON_REG = 0,   ///< Guard access to DTS common registers
    BSP_DTSG_CONTROL_CHANNEL_REG,      ///< Guard access to DTS channel registers
    BSP_DTSG_CONTROL_NUM
} bsp_dtsg_control_t;

/** DMA guard control targets (Group register) */
typedef enum e_bsp_dmag_control
{
    BSP_DMAG_CONTROL_GLOBAL_REG = 0,   ///< Guard access to sDMAC global registers
    BSP_DMAG_CONTROL_DRAM_REG,         ///< Guard access to sDMAC descriptor RAM
    BSP_DMAG_CONTROL_REDUNDANT_REG,    ///< Guard access to sDMAC redundant channel register
    BSP_DMAG_CONTROL_CHANNEL_REG,      ///< Guard access to sDMAC channel registers
    BSP_DMAG_CONTROL_NUM
} bsp_dmag_control_t;

/** sDMAC guard control targets (Units) */
typedef enum e_bsp_dmag_unit
{
    BSP_DMAG_UNIT_0 = 0,               ///< sDMAC Unit 0
    BSP_DMAG_UNIT_1,                   ///< sDMAC Unit 1
    BSP_DMAG_UNIT_NUM
} bsp_dmag_unit_t;

/** Guard enable setting */
typedef enum e_bsp_guard_setting
{
    BSP_GUARD_SETTING_DISABLE = 0,     ///< Disable guard setting
    BSP_GUARD_SETTING_ENABLE  = 1,     ///< Enable guard setting
} bsp_guard_setting_t;

/** Guard debug access control */
typedef enum e_bsp_guard_debug
{
    BSP_GUARD_DEBUG_DEPENDS = 0,       ///< R/W depends on other protection settings
    BSP_GUARD_DEBUG_ENABLE  = 1        ///< R/W enabled for debug master
} bsp_guard_debug_t;

/** Guard user mode access control */
typedef enum e_bsp_guard_um
{
    BSP_GUARD_UM_DISABLE = 0,          ///< Disable user mode access (R/W)
    BSP_GUARD_UM_ENABLE  = 1           ///< Enable user mode access (R/W depends on protection settings)
} bsp_guard_um_t;

/** Guard write access control */
typedef enum e_bsp_guard_write
{
    BSP_GUARD_WRITE_USE_SPID  = 0,     ///< Write uses SPID check (SPID is used as judgment condition)
    BSP_GUARD_WRITE_ALLOW_ALL = 1      ///< Write allows all (SPID is NOT used as judgment condition)
} bsp_guard_write_t;

/** Guard read access control */
typedef enum e_bsp_guard_read
{
    BSP_GUARD_READ_USE_SPID  = 0,      ///< Read uses SPID check (SPID is used as judgment condition)
    BSP_GUARD_READ_ALLOW_ALL = 1       ///< Read allows all (SPID is NOT used as judgment condition)
} bsp_guard_read_t;

/** Guard secure access control */
typedef enum e_bsp_guard_secure
{
    BSP_GUARD_SECURE_DISABLE = 0,      ///< R/W disabled for non-secure master
    BSP_GUARD_SECURE_ENABLE  = 1,      ///< R/W depends on other protection settings
} bsp_guard_secure_t;

/** Guard lock setting control */
typedef enum e_bsp_guard_lock
{
    BSP_GUARD_LOCK_DISABLE = 0,        ///< Allow re-configure protect setting rewrite
    BSP_GUARD_LOCK_ENABLE  = 1,        ///< Block further writes until reset
} bsp_guard_lock_t;

/** MPID permissions for SPID entries */
typedef enum e_bsp_guard_mpid
{
    BSP_GUARD_MPID_NONE = 0,           ///< No SPID permitted (MPID[7:0] = 00000000B)

    BSP_GUARD_MPID_0 = (1 << 0),       ///< Permit SPID set in MPID0 (MPID[0] = 1)
    BSP_GUARD_MPID_1 = (1 << 1),       ///< Permit SPID set in MPID1 (MPID[1] = 1)
    BSP_GUARD_MPID_2 = (1 << 2),       ///< Permit SPID set in MPID2 (MPID[2] = 1)
    BSP_GUARD_MPID_3 = (1 << 3),       ///< Permit SPID set in MPID3 (MPID[3] = 1)
    BSP_GUARD_MPID_4 = (1 << 4),       ///< Permit SPID set in MPID4 (MPID[4] = 1)
    BSP_GUARD_MPID_5 = (1 << 5),       ///< Permit SPID set in MPID5 (MPID[5] = 1)
    BSP_GUARD_MPID_6 = (1 << 6),       ///< Permit SPID set in MPID6 (MPID[6] = 1)
    BSP_GUARD_MPID_7 = (1 << 7),       ///< Permit SPID set in MPID7 (MPID[7] = 1)
} bsp_guard_mpid_t;

/** SPID access permissions (0–31) */
typedef enum e_bsp_guard_set_spid
{
    BSP_GUARD_SET_SPID_NONE = 0,       ///< No SPID access

    BSP_GUARD_SET_SPID0  = (1 << 0),   ///< Enable access for SPID = 0
    BSP_GUARD_SET_SPID1  = (1 << 1),   ///< Enable access for SPID = 1
    BSP_GUARD_SET_SPID2  = (1 << 2),   ///< Enable access for SPID = 2
    BSP_GUARD_SET_SPID3  = (1 << 3),   ///< Enable access for SPID = 3
    BSP_GUARD_SET_SPID4  = (1 << 4),   ///< Enable access for SPID = 4
    BSP_GUARD_SET_SPID5  = (1 << 5),   ///< Enable access for SPID = 5
    BSP_GUARD_SET_SPID6  = (1 << 6),   ///< Enable access for SPID = 6
    BSP_GUARD_SET_SPID7  = (1 << 7),   ///< Enable access for SPID = 7
    BSP_GUARD_SET_SPID8  = (1 << 8),   ///< Enable access for SPID = 8
    BSP_GUARD_SET_SPID9  = (1 << 9),   ///< Enable access for SPID = 9
    BSP_GUARD_SET_SPID10 = (1 << 10),  ///< Enable access for SPID = 10
    BSP_GUARD_SET_SPID11 = (1 << 11),  ///< Enable access for SPID = 11
    BSP_GUARD_SET_SPID12 = (1 << 12),  ///< Enable access for SPID = 12
    BSP_GUARD_SET_SPID13 = (1 << 13),  ///< Enable access for SPID = 13
    BSP_GUARD_SET_SPID14 = (1 << 14),  ///< Enable access for SPID = 14
    BSP_GUARD_SET_SPID15 = (1 << 15),  ///< Enable access for SPID = 15
    BSP_GUARD_SET_SPID16 = (1 << 16),  ///< Enable access for SPID = 16
    BSP_GUARD_SET_SPID17 = (1 << 17),  ///< Enable access for SPID = 17
    BSP_GUARD_SET_SPID18 = (1 << 18),  ///< Enable access for SPID = 18
    BSP_GUARD_SET_SPID19 = (1 << 19),  ///< Enable access for SPID = 19
    BSP_GUARD_SET_SPID20 = (1 << 20),  ///< Enable access for SPID = 20
    BSP_GUARD_SET_SPID21 = (1 << 21),  ///< Enable access for SPID = 21
    BSP_GUARD_SET_SPID22 = (1 << 22),  ///< Enable access for SPID = 22
    BSP_GUARD_SET_SPID23 = (1 << 23),  ///< Enable access for SPID = 23
    BSP_GUARD_SET_SPID24 = (1 << 24),  ///< Enable access for SPID = 24
    BSP_GUARD_SET_SPID25 = (1 << 25),  ///< Enable access for SPID = 25
    BSP_GUARD_SET_SPID26 = (1 << 26),  ///< Enable access for SPID = 26
    BSP_GUARD_SET_SPID27 = (1 << 27),  ///< Enable access for SPID = 27
    BSP_GUARD_SET_SPID28 = (1 << 28),  ///< Enable access for SPID = 28
    BSP_GUARD_SET_SPID29 = (1 << 29),  ///< Enable access for SPID = 29
    BSP_GUARD_SET_SPID30 = (1 << 30),  ///< Enable access for SPID = 30
    BSP_GUARD_SET_SPID31 = (1 << 31)   ///< Enable access for SPID = 31
} bsp_guard_set_spid_t;

/** PE guard region size settings */
typedef enum e_bsp_peg_region_size
{
    BSP_PEG_REGION_SIZE_4KB  = 0x3FFF, ///< 4KB   - Requires Address 4KB alignment
    BSP_PEG_REGION_SIZE_8KB  = 0x3FFE, ///< 8KB   - Requires Address 8KB alignment
    BSP_PEG_REGION_SIZE_16KB = 0x3FFC, ///< 16KB  - Requires Address 16KB alignment
    BSP_PEG_REGION_SIZE_32KB = 0x3FF8, ///< 32KB  - Requires Address 32KB alignment
    BSP_PEG_REGION_SIZE_64KB = 0x3FF0, ///< 64KB  - Requires Address 64KB alignment
    BSP_PEG_REGION_SIZE_MAX  = 0x0000  ///< MAX    - Protects entire Local RAM region
} bsp_peg_region_size_t;

/** Cluster RAM guard region size settings */
typedef enum e_bsp_crg_region_size
{
    BSP_CRG_REGION_SIZE_256B  = 0x7FF, ///< 256B  - Requires Address 256B alignment
    BSP_CRG_REGION_SIZE_512B  = 0x7FE, ///< 512B  - Requires Address 512B alignment
    BSP_CRG_REGION_SIZE_1KB   = 0x7FC, ///< 1KB   - Requires Address 1KB alignment
    BSP_CRG_REGION_SIZE_2KB   = 0x7F8, ///< 2KB   - Requires Address 2KB alignment
    BSP_CRG_REGION_SIZE_4KB   = 0x7F0, ///< 4KB   - Requires Address 4KB alignment
    BSP_CRG_REGION_SIZE_8KB   = 0x7E0, ///< 8KB   - Requires Address 8KB alignment
    BSP_CRG_REGION_SIZE_16KB  = 0x7C0, ///< 16KB  - Requires Address 16KB alignment
    BSP_CRG_REGION_SIZE_32KB  = 0x780, ///< 32KB  - Requires Address 32KB alignment
    BSP_CRG_REGION_SIZE_64KB  = 0x700, ///< 64KB  - Requires Address 64KB alignment
    BSP_CRG_REGION_SIZE_128KB = 0x600, ///< 128KB - Requires Address 128KB alignment
    BSP_CRG_REGION_SIZE_256KB = 0x400, ///< 256KB - Requires Address 256KB alignment
    BSP_CRG_REGION_SIZE_512KB = 0x000, ///< 512KB - Protects entire Cluster RAM region
} bsp_crg_region_size_t;

/** PE guard protection configuration */
typedef struct st_bsp_peg_protection_setting
{
    /** Specifies which PE's Local RAM and INTC1 areas to protect */
    bsp_peg_control_t control;

    /** Channel number (0-7) to configure protection region */
    uint8_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global Enable
            bsp_guard_write_t write_global   : 1; ///< Write Global Enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master Enable
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 23;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    uint32_t spid;                                ///< SPIDs permitted to access this region
    uint32_t base_address;                        ///< Start address of protected region (4KB aligned)
    uint32_t valid_address;                       ///< Address mask defining region size
} bsp_peg_protection_setting_t;

/** Cluster RAM guard protection configuration */
typedef struct st_bsp_crg_protection_setting
{
    /** Specifies which Cluster RAM areas to protect */
    bsp_crg_control_t control;

    /** Channel number (0-7) to configure protection region */
    uint8_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global Enable
            bsp_guard_write_t write_global   : 1; ///< Write Global Enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master Enable
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 23;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    uint32_t spid;                                ///< SPIDs permitted to access this region
    uint32_t base_address;                        ///< Start address of protected region (256B aligned)
    uint32_t valid_address;                       ///< Address mask defining region size
} bsp_crg_protection_setting_t;

/** INTC2 guard protection configuration */
typedef struct st_bsp_intc2g_protection_setting
{
    /** Specifies which INTC2 register group to protect (EIC/EEIC, EIBD/I2EIBG, or IMR) */
    bsp_intc2g_control_t control;

    /** Specifies channel number (32-1023 for EIC/EEIC, ignored for other groups) */
    uint16_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global enable
            bsp_guard_write_t write_global   : 1; ///< Write Global enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master setting
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 7;
            uint32_t mpid_permit             : 8; ///< MPID bitmap
            uint32_t                         : 8;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    /** SPID settings array (8 elements): Each element specifies one SPID value (0-31) that can be referenced by MPID bitmap */
    uint32_t spid[8];
} bsp_intc2g_protection_setting_t;

/** sDMAC guard protection configuration */
typedef struct st_bsp_dmag_protection_setting
{
    /** Specifies which sDMAC unit to protect */
    bsp_dmag_unit_t unit;

    /** sSpecifies which sDMAC register group to protect (global, DRAM, redundant, or channel) */
    bsp_dmag_control_t control;

    /** Specifies channel number (0-15 for channel registers, ignored for global/DRAM/redundant registers) */
    uint16_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global Enable
            bsp_guard_write_t write_global   : 1; ///< Write Global Enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master setting
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 7;
            uint32_t mpid_permit             : 8; ///< MPID bitmap
            uint32_t                         : 8;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    /** SPID settings array (8 elements): Each element specifies one SPID value (0-31) that can be referenced by MPID bitmap */
    uint32_t spid[8];
} bsp_dmag_protection_setting_t;

/** DTS guard protection configuration */
typedef struct st_bsp_dtsg_protection_setting
{
    /** Specifies which DTS register group to protect (common or channel-specific) */
    bsp_dtsg_control_t control;

    /** Specifies channel number (0-127 for channel registers, ignored for common registers) */
    uint16_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global Enable
            bsp_guard_write_t write_global   : 1; ///< Write Global Enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master setting
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 7;
            uint32_t mpid_permit             : 8; ///< MPID bitmap
            uint32_t                         : 8;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    /** SPID settings array (8 elements): Each element specifies one SPID value (0-31) that can be referenced by MPID bitmap */
    uint32_t spid[8];
} bsp_dtsg_protection_setting_t;

/** PBG/HBG guard protection configuration */
typedef struct st_bsp_pbg_protection_setting
{
    /** Bus group target: Specifies which PBG or HBG group to protect */
    bsp_pbg_group_t group;

    /** Specifies which channel number within PBG group */
    uint8_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global     : 1; ///< Read Global enable
            bsp_guard_write_t write_global    : 1; ///< Write Global enable
            uint32_t                          : 2;
            bsp_guard_um_t user_mode          : 1; ///< User Mode setting
            uint32_t                          : 1;
            bsp_guard_debug_t   debug_mode    : 1; ///< Debug Master setting
            bsp_guard_secure_t  secure_master : 1; ///< Secure Master setting
            bsp_guard_setting_t guard_enable  : 1; ///< Guard Enable: Disable
            uint32_t                          : 22;
            bsp_guard_lock_t lock             : 1; ///< Lock Prevents modification when set
        }        protect_setting_b;                ///< Protection attribute setting
        uint32_t protect_setting;                  ///< Direct access to all protection control settings
    };

    uint32_t spid;                                 ///< SPIDs permitted to access this region
} bsp_pbg_protection_setting_t;

/** I-Bus guard protection configuration */
typedef struct st_bsp_ibg_protection_setting
{
    /** Specifies which I-Bus module register group to protect
     *  - IPIR (Rn/Tn/4), BarrierSync (n/16), or TPTM (IFn/Un/U8/U9)
     */
    bsp_ibg_control_t control;

    /** Specifies channel number for channel-based targets. Ignored for common targets (IPIR_4, BARR_16, TPTM_U8, TPTM_U9) */
    uint8_t channel;

    /** Protection Control setting */
    union
    {
        struct
        {
            bsp_guard_read_t  read_global    : 1; ///< Read Global Enable
            bsp_guard_write_t write_global   : 1; ///< Write Global Enable
            uint32_t                         : 2;
            bsp_guard_um_t user_mode         : 1; ///< User Mode setting
            uint32_t                         : 1;
            bsp_guard_debug_t debug_mode     : 1; ///< Debug Master setting
            uint32_t                         : 1;
            bsp_guard_setting_t guard_enable : 1; ///< Guard Enable
            uint32_t                         : 23;
        }        protect_setting_b;               ///< Protection attribute setting
        uint32_t protect_setting;                 ///< Direct access to all protection control settings
    };

    uint32_t spid;                                ///< SPIDs permitted to access this region
} bsp_ibg_protection_setting_t;

/** Guard protection setting structure for all guard types. Use the appropriate member based on the target guard module. */
typedef struct st_bsp_guard_protection_setting
{
    /** Guard module-specific protection settings */
    union
    {
        /** PE Guard settings */
        bsp_peg_protection_setting_t pegrd_setting;

        /** Cluster RAM Guard settings */
        bsp_crg_protection_setting_t cramgrd_setting;

        /** I-Bus Guard settings */
        bsp_ibg_protection_setting_t ibusgrd_setting;

        /** Peripheral Bus Guard settings */
        bsp_pbg_protection_setting_t pbusgrd_setting;

        /** INTC2 Guard settings */
        bsp_intc2g_protection_setting_t intc2grd_setting;

        /** DTS Guard settings */
        bsp_dtsg_protection_setting_t dtsgrd_setting;

        /** sDMAC Guard settings */
        bsp_dmag_protection_setting_t dmagrd_setting;
    };
} bsp_guard_protection_setting_t;

/** Guard protection status error codes */
typedef enum e_bsp_protection_status_error
{
    BSP_PROTECTION_STATUS_ERROR_NONE = 0, ///< No guard error detected
    BSP_PROTECTION_STATUS_ERROR_DETECTED, ///< Guard error detected
    BSP_PROTECTION_STATUS_ERROR_OVERFLOW  ///< Guard error overflow occurred (new error while previous error not cleared)
} bsp_protection_status_error_t;

/** Guard protection error status information */
typedef struct st_bsp_protection_status
{
    bsp_protection_status_error_t errstatus; ///< Guard error status

    uint32_t err_access_address;             ///< Address without access permission that caused the guard error

    /** Error access attributes */
    union
    {
        struct
        {
            bsp_guard_write_t write : 1; ///< Access type (0=Read, 1=Write) when guard error occurred
            uint32_t                : 5;
            uint32_t           spid : 5; ///< SPID[4:0] - Access attribute of SPID when guard error occurred
            bsp_guard_um_t     um   : 1; ///< UM - User Mode access attribute when guard error occurred
            bsp_guard_debug_t  dbg  : 1; ///< DBG - Debug access attribute when guard error occurred
            bsp_guard_secure_t sec  : 1; ///< SEC - Secure access attribute when guard error occurred
            uint32_t                : 18;
        } err_attribute_b;               ///< Error attribute fields

        uint32_t err_attribute;          ///< Direct access to error attribute register
    };

    uint32_t spid_error_status;          ///< SPID error status bitmap showing which SPID(s) caused guard errors
} bsp_protection_status_t;

/** PE Guard protection error status information */
typedef struct st_bsp_peg_protection_status
{
    /** Specifies which PE's Local RAM and INTC1 areas to get protect status */
    bsp_peg_control_t control;

    /**
     * PEG protection status for PEx (x = 0, 1):
     *
     * [0] = PEGCAP_S_PEx:     PEx INTC1 registers + PEx LRAM (Access from other than PEx / Fetch from PEx)
     */
    bsp_protection_status_t peg_status[BSP_FEATURE_PE_GUARD_STATUS_MAX];
} bsp_peg_protection_status_t;

/** CLuster RAM Guard protection error status information */
typedef struct st_bsp_crg_protection_status
{
    /** Specifies which Cluster RAM areas to get protect status */
    bsp_crg_control_t control;

    /**
     * CRG protection status for Cluster0 RAMn (n = 0, 1, 2):
     *
     * [0] = CRGCAP_CRAMHCLn:    Access request from all masters for odd words (bank1, bank3)
     * [1] = CRGCAP_CRAMLCLn:    Access request from all masters for even words (bank0, bank2)
     */
    bsp_protection_status_t crg_status[BSP_FEATURE_CRAM_GUARD_STATUS_MAX];
} bsp_crg_protection_status_t;

/** sDMAC guard protection status for a specific DMA unit */
typedef struct st_bsp_dmac_guard_protection_status
{
    /** Specifies which sDMAC unit to get protect staus*/
    bsp_dmag_unit_t dma_guard_unit;

    /** DMAC Guard protection status for specific unit */
    bsp_protection_status_t status;
} bsp_dmac_guard_protection_status_t;

/** I-Bus guard protection status for a specific module */
typedef struct st_bsp_ibg_protection_status
{
    /** Specifies which I-Bus module to get protect staus*/
    bsp_module_ibg_id_t module;

    /** IBG protection status for specific module */
    bsp_protection_status_t status;
} bsp_ibg_protection_status_t;

/** PBG/HBG guard protection status for a specific group */
typedef struct st_bsp_pbg_protection_status
{
    /**  Specifies which PBG or HBG group to get protect status*/
    bsp_pbg_group_t group;

    /** PBG protection status for specific group */
    bsp_protection_status_t status;
} bsp_pbg_protection_status_t;

/** Guard protection status structure for all guard types. Use the appropriate member based on the target guard module. */
typedef struct st_bsp_guard_protection_status
{
    /** Guard module-specific protection status */
    union
    {
        /** PE Guard status */
        bsp_peg_protection_status_t pegrd_status;

        /** Cluster RAM Guard status */
        bsp_crg_protection_status_t cramgrd_status;

        /** I-Bus Guard status */
        bsp_ibg_protection_status_t ibusgrd_status;

        /** Peripheral Bus Guard status */
        bsp_pbg_protection_status_t pbusgrd_status;

        /** INTC2 Guard status */
        bsp_protection_status_t intc2grd_status;

        /** DTS Guard status */
        bsp_protection_status_t dtsgrd_status;

        /** sDMAC Guard status */
        bsp_dmac_guard_protection_status_t dmagrd_status;
    };
} bsp_guard_protection_status_t;

/** Function pointer type for applying guard protection settings */
typedef void (* bsp_protection_setting)(const bsp_guard_protection_setting_t * p_setting);

/** Function pointer type for retrieving guard protection status */
typedef void (* bsp_protection_status)(bsp_guard_protection_status_t * p_status);

/** Properties of a PBG module including group and channel information */
typedef struct st_bsp_module_pbg_properties
{
    bsp_pbg_group_t pbg_group;         ///< Guard group
    uint32_t        pbg_channel;       ///< Guard channel, bit position indicate the channel.
} bsp_module_pbg_properties_t;

/** PBG module info */
typedef struct st_bsp_module_pbg_info
{
    uint8_t no_of_element;                    ///< Number of elements in this PBG module
    uint8_t initial_cfg_value;                ///< Initial configuration value for access control
    bsp_module_pbg_properties_t * properties; ///< Pointer to module PBG property table
} bsp_module_pbg_info_t;

/** IBG module properties */
typedef struct st_bsp_module_ibg_properties
{
    bsp_ibg_control_t ibg_control;     ///< IBGuard target
    uint16_t          ibg_channel;     ///< IBGuard target channel, bit position indicate the channel.
} bsp_module_ibg_properties_t;

/** IBG module info */
typedef struct st_bsp_module_ibg_info
{
    uint8_t no_of_element;                    ///< Number of elements in this PBG module
    uint8_t initial_cfg_value;                ///< Initial configuration value for access control
    bsp_module_ibg_properties_t * properties; ///< Pointer to module IBG property table
} bsp_module_ibg_info_t;

/** INTC1 & Local RAM protection info */
typedef struct st_bsp_intc1_lram_info
{
    uint8_t           initial_cfg_value; ///< Initial configuration flag
    bsp_peg_control_t lram_peg;          ///< protection control identifier for INTC1 and Local RAM protection
} bsp_intc1_lram_info_t;

/** Cluster RAM protection info */
typedef struct st_bsp_cram_info
{
    uint8_t           initial_cfg_value; ///< Initial configuration flag
    bsp_crg_control_t cram_crg;          ///< protection control identifier for Cluster RAM protection
} bsp_cram_info_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/
extern const bsp_module_pbg_info_t  g_bsp_module_pgb_info_table[BSP_MODULE_PBG_TOTAL_INDEX_END];
extern const bsp_module_ibg_info_t  g_bsp_module_ibg_info_table[BSP_MODULE_IBG_TOTAL_INDEX_END];
extern const uint32_t               g_pbgprot_baseaddress[BSP_PBG_GROUP_NUM];
extern const uint32_t               g_pbgkcprot_baseaddress[BSP_PBG_GROUP_NUM];
extern const uint32_t               g_pbgprot_control_baseaddress[BSP_PBG_GROUP_NUM];
extern const bsp_intc1_lram_info_t  g_bsp_intc1_lram_info_table[BSP_PEG_CONTROL_NUM];
extern const bsp_cram_info_t        g_bsp_cram_info_table[BSP_CRG_CONTROL_NUM];
extern const uint32_t               g_peg_lram_baseaddress[BSP_PEG_CONTROL_NUM];
extern const uint32_t               g_ibg_baseaddress[BSP_IBG_CONTROL_NUM];
extern const uint32_t               g_ibgprot_baseaddress[BSP_IBG_CONTROL_NUM];
extern const uint32_t               g_ibg_module_baseaddress[BSP_MODULE_IBG_TOTAL_INDEX_END];
extern const uint32_t               g_peg_baseaddress[BSP_PEG_CONTROL_NUM];
extern const uint32_t               g_pegcap_s_baseaddress[BSP_PEG_CONTROL_NUM];
extern const uint32_t               g_crg_baseaddress[BSP_CRG_CONTROL_NUM];
extern const uint32_t               g_crgcap_baseaddress[BSP_CRG_CONTROL_NUM][BSP_FEATURE_CRAM_GUARD_STATUS_MAX];
extern const uint32_t               g_crg_cram_baseaddress[BSP_CRG_CONTROL_NUM];
extern const uint32_t               g_dmacg_baseaddress[BSP_DMAG_UNIT_NUM];
extern const bsp_protection_setting guard_protection_setting[BSP_GUARD_TYPE_NUM];
extern const bsp_protection_status  guard_protection_status[BSP_GUARD_TYPE_NUM];

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_RH850U2CX) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /** End of BSP_GUARD_DRIVER_H */

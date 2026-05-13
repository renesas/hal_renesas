/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef BSP_MPU_H
#define BSP_MPU_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"

/** Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/*******************************************************************************************************************//**
 * @ingroup BSP_MCU
 * @defgroup BSP_MPU BSP MPU
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

#define BSP_MPU_ENABLE                       (0x01)                              ///< Enable MPU
#define BSP_MPU_REGION_ENABLE                (0x80)                              ///< Enable specific region in MPU
#define BSP_MPU_PRIV_MODE_ACCESS_ALL         (RH850_MPU_SV_MODE_ACCESS_ENABLE)   ///< Enable all accesses in Privilege mode
#define BSP_MPU_PRIV_MODE_RESTRICT_ACCESS    (RH850_MPU_SV_MODE_ACCESS_RESTRICT) ///< Restrict access according to the SX, SW, and SR bits in SV mode

#define BSP_MPU_MPID_MASK                    (0x1F)                              ///< Mask for MPID register

#define BSP_MPU_MAX_MPID_NUMBER              (8)                                 ///< Max number of MPID register supported in this device

#define BSP_MPU_WMPID_VALUE(spid)    ((spid) << (24))                            /// < Encodes SPID into WMPID field (bit position 24)
#define BSP_MPU_RMPID_VALUE(spid)    ((spid) << (16))                            /// < Encodes SPID into RMPID field (bit position 16)
#define BSP_MPU_UR_EN                        (1U << 0)                           ///< Reading is enabled for user mode
#define BSP_MPU_UW_EN                        (1U << 1)                           ///< Writing is enabled for user mode
#define BSP_MPU_UX_EN                        (1U << 2)                           ///< Executing is enabled for user mode
#define BSP_MPU_SR_EN                        (1U << 3)                           ///< Reading is enabled for supervisor mode
#define BSP_MPU_SW_EN                        (1U << 4)                           ///< Writing is enabled for supervisor mode
#define BSP_MPU_SX_EN                        (1U << 5)                           ///< Executing is enabled for supervisor mode

/** Execution and reading by any SPID is enabled regardless of the settings MPIDn */
#define BSP_MPU_RG_EN                        (1U << 14)

/** Writing by any SPID is enabled regardless of the settings MPIDn */
#define BSP_MPU_WG_EN                        (1U << 15)

/** Helper define to initialize the region structure. */
#define BSP_MPU_REGIONn_CONFIG(n)    BSP_REGION ## n ## _CONFIG

#define BSP_MPU_MPID_0                       (0U) ///< MPID value for MPU entry 0
#define BSP_MPU_MPID_1                       (1U) ///< MPID value for MPU entry 1
#define BSP_MPU_MPID_2                       (2U) ///< MPID value for MPU entry 2
#define BSP_MPU_MPID_3                       (3U) ///< MPID value for MPU entry 3
#define BSP_MPU_MPID_4                       (4U) ///< MPID value for MPU entry 4
#define BSP_MPU_MPID_5                       (5U) ///< MPID value for MPU entry 5
#define BSP_MPU_MPID_6                       (6U) ///< MPID value for MPU entry 6
#define BSP_MPU_MPID_7                       (7U) ///< MPID value for MPU entry 7

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** MPU Memory checking status */
typedef struct st_bsp_mpu_mem_check_status
{
    union
    {
        uint32_t mem_check_status;
        struct
        {
            uint32_t user_read_enable          : 1;  ///< User mode read permission
            uint32_t user_write_enable         : 1;  ///< User mode write permission
            uint32_t user_execute_enable       : 1;  ///< User mode execute permission
            uint32_t supervisor_read_enable    : 1;  ///< Supervisor mode  read permission
            uint32_t supervisor_write_enable   : 1;  ///< Supervisor mode write permission
            uint32_t supervisor_execute_enable : 1;  ///< Supervisor mode executepermission
            uint32_t                           : 2;  ///< Reserved bits
            uint32_t ov_bit                    : 1;  ///< Bit indicates the specified area includes 0000 0000H or 7FFF FFFFH
            uint32_t                           : 23; ///< Reserved bits
        } mem_check_status_b;
    } mpu_status;
} bsp_mpu_mem_check_status_t;

/** Region definition data structure */
typedef struct e_bsp_mpu_region
{
    const char * name;                               ///< Region's name
    uint32_t     start_address;                      ///< Region's start address
    uint32_t     size;                               ///< Region's size
    union
    {
        uint32_t attribute;                          ///< Region's attribute
        struct
        {
            uint32_t user_mode_permission       : 3; ///< User mode permission
            uint32_t supervisor_mode_permission : 3; ///< Supervisor mode permission
            uint32_t                            : 1; ///< Reserved bit
            uint32_t region_enable              : 1; ///< Region enable
            uint32_t                            : 6; ///< Reserved bits
            uint32_t rg_bit                     : 1; ///< Bit indicates the execution and read permissions for any SPID
            uint32_t wg_bit                     : 1; ///< Bit indicates the write permissions for any SPID
            uint32_t rmpid                      : 8; ///< Indicates the execution and read permissions for the SPID specified in the MPIDn register
            uint32_t wmpid                      : 8; ///< Indicates the write for the SPID specified in the MPIDn register
        } attribute_b;
    }       region_attr;
    uint8_t no_of_region;                            ///< Region's index in MPU
} bsp_mpu_region_t;

/** MPU configuration data structure */
typedef struct e_bsp_mpu_config
{
    bsp_mpu_region_t * mpu_region;     ///< Pointer to Regions
    uint8_t            num_of_regions; ///< Total number of regions to be configurated
} bsp_mpu_config_t;

/***********************************************************************************************************************
 * Exported global variable (to be accessed by other files)
 **********************************************************************************************************************/
extern bsp_mpu_config_t g_bsp_mpu_config;

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

fsp_err_t R_BSP_MPUEnable(const bsp_mpu_config_t * const p_config);
void      R_BSP_MPUDisable(void);
uint8_t   R_BSP_MPURegionCountGet(void);
fsp_err_t R_BSP_MPURegionEnable(bsp_mpu_region_t * const p_region);
fsp_err_t R_BSP_MPURegionDisable(bsp_mpu_region_t * const p_region);
fsp_err_t R_BSP_MPURegionSet(bsp_mpu_region_t * const p_region);
fsp_err_t R_BSP_MPURegionGet(bsp_mpu_region_t * const p_region_data, uint8_t r_index);
void      R_BSP_MPUSetMPIDx(uint8_t no_of_reg, uint8_t value);
void      R_BSP_MPUExecuteMemProtectionCheck(uint8_t                      spid,
                                             uint32_t                     lower_add,
                                             uint32_t                     size,
                                             bsp_mpu_mem_check_status_t * p_status);

/** @} (end addtogroup BSP_MPU) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif

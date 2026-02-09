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

#define BSP_MPU_ENABLE                       (0x01)    ///< Enable MPU
#define BSP_MPU_REGION_ENABLE                (0x80)    ///< Enable specific region in MPU
#define BSP_MPU_PRIV_MODE_ACCESS_ALL         (0x00)    ///< Enable all accesses in Privilege mode
#define BSP_MPU_PRIV_MODE_RESTRICT_ACCESS    (0x02)    ///< Restrict access according to the SX, SW, and SR bits in SV mode

#define BSP_MPU_MPID_MASK                    (0x1F)    ///< Mask for MPID register

#define BSP_MPU_MAX_MPID_NUMBER              (8)       ///< Max number of MPID register supported in this device

#define BSP_MPU_WMPID_VALUE(spid)    ((spid) << (24))  /// < Encodes SPID into WMPID field (bit position 24)
#define BSP_MPU_RMPID_VALUE(spid)    ((spid) << (16))  /// < Encodes SPID into RMPID field (bit position 16)
#define BSP_MPU_UR_EN                        (1U << 0) ///< Reading is enabled for user mode
#define BSP_MPU_UW_EN                        (1U << 1) ///< Writing is enabled for user mode
#define BSP_MPU_UX_EN                        (1U << 2) ///< Executing is enabled for user mode
#define BSP_MPU_SR_EN                        (1U << 3) ///< Reading is enabled for supervisor mode
#define BSP_MPU_SW_EN                        (1U << 4) ///< Writing is enabled for supervisor mode
#define BSP_MPU_SX_EN                        (1U << 5) ///< Executing is enabled for supervisor mode

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
    };
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
    };
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
 * Inline functions
 **********************************************************************************************************************/

/* Enable the MPU.
 * \param priv_access_type Access type for Privilege mode
 */
__STATIC_INLINE void RH850_MPU_Enable (uint8_t priv_access_type)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "ORI  1, r6, r6                     \n" /* Set bit 0 in parameter to 1 */
        "ldsr r6, 0, 5                      \n" /* SR0, 5  MPM */
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "ORI  1, %0, %0                     \n" /* Set bit 0 in parameter to 1 */
        "ldsr %0, 0, 5                      \n" /* SR0, 5  MPM */
        : "+r"(priv_access_type)
        :
        : "memory"
    );
#endif
}

/* Disable the MPU.
 */
__STATIC_INLINE void RH850_MPU_Disable (void)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "stsr   0, r15, 5                   \n" /* read SR0, 5 MPM */
        "ANDI   0, r15, r15                 \n" /* Clear bit 0 and 1 */
        "ldsr   r15, 0, 5                   \n" /* write to SR0, 5 MPM */
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "stsr   0, r15, 5                   \n" /* read SR0, 5 MPM */
        "ANDI   0, r15, r15                 \n" /* Clear bit 0 and 1 */
        "ldsr   r15, 0, 5                   \n" /* write to SR0, 5 MPM */
        :
        :
        : "r15"
    );
#endif
}

/* Set the memory attribute encoding.
 * \param region The region number to be set [0-31]
 * \param attr The attribute value to be set.
 */
__STATIC_INLINE void RH850_MPU_SetAttr (uint8_t region, uint32_t attr)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "ldsr r6, 16, 5                     \n" /* Set register SR16, 5 MPIDX to region value */
        "ldsr r7, 22, 5                     \n" /* Set register SR22, 5 MPAT to attr value */
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "ldsr %0, 16, 5                     \n" /* Set register SR16, 5 MPIDX to region value */
        "ldsr %1, 22, 5                     \n" /* Set register SR22, 5 MPAT to attr value */
        :
        : "r"(region), "r"(attr)
        : "memory"
    );
#endif
}

#if defined(__CCRH__)

/* Get the memory attribute encoding.
 * \param region The region number to be set [0-31]
 */
 #pragma inline_asm RH850_MPU_GetAttr
static uint32_t RH850_MPU_GetAttr (uint8_t region)
{
    ldsr r6, 16, 5;                    /* Set parameter region to register SR16, 5 MPIDX */
    stsr    22, r10, 5;                /* Get attribute */
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)

/* Get the memory attribute encoding.
 * \param region The region number to be set [0-31]
 */
__STATIC_INLINE uint32_t RH850_MPU_GetAttr (uint8_t region)
{
    uint32_t attr;

    __asm__ volatile (
        "ldsr  %1, 16, 5     \n"       /* Set parameter region to register SR16, 5 MPIDX */
        "stsr  22, %0, 5     \n"       /* Get attribute */
        : "=&r"(attr)
        : "r" (region)
        : "memory"
    );

    return attr;
}

#endif

/* Clear and disable the given MPU region.
 * \param region Region number to be cleared.
 */
__STATIC_INLINE void RH850_MPU_ClrRegion (uint8_t region)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "ldsr r6, 16, 5                     \n" /* Set parameter region to register SR16, 5 MPIDX */
        "mov 0, r7                          \n"
        "ldsr r7, 20, 5                     \n" /* Clear register SR20, 5 MPLA */
        "ldsr r7, 21, 5                     \n" /* Clear register SR21, 5 MPUA */
        "ldsr r7, 22, 5                     \n" /* Clear register SR22, 5 MPAT */
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "ldsr %0, 16, 5                     \n" /* Set parameter region to register SR16, 5 MPIDX */
        "mov 0, r7                          \n"
        "ldsr r7, 20, 5                     \n" /* Clear register SR20, 5 MPLA */
        "ldsr r7, 21, 5                     \n" /* Clear register SR21, 5 MPUA */
        "ldsr r7, 22, 5                     \n" /* Clear register SR22, 5 MPAT */
        :
        : "r"(region)
        : "r7", "memory"
    );
#endif
}

/* Configure the given MPU region.
 * \param region Region number to be configured.
 * \param lower_add Value for MPLA register.
 * \param size Size of region.
 */
__STATIC_INLINE void RH850_MPU_SetRegion (uint8_t region, uint32_t lower_add, uint32_t size)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "ldsr r6, 16, 5                     \n" /* Set parameter region to register SR16, 5 MPIDX */
        "ldsr r7, 20, 5                     \n" /* Set parameter lower_add to register SR20, 5 MPLA */
        "add  r8, r7                        \n" /* Add size to lower_add to find the upper address */
        "ldsr r7, 21, 5                     \n" /* Set upper address to register SR21, 5 MPUA */
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "ldsr %0, 16, 5                     \n" /* Set parameter region to register SR16, 5 MPIDX */
        "ldsr %1, 20, 5                     \n" /* Set parameter lower_add to register SR20, 5 MPLA */
        "add  %2, %1                        \n" /* Add size to lower_add to find the upper address */
        "ldsr %1, 21, 5                     \n" /* Set upper address to register SR21, 5 MPUA */
        :
        : "r"(region), "r"(lower_add), "r"(size)
        : "memory"
    );
#endif
}

#if defined(__CCRH__)

/* Get the lower address of specific region
 * \param region The region number to be set [0-31]
 */
 #pragma inline_asm RH850_MPU_GetLowerAddress
static uint32_t RH850_MPU_GetLowerAddress (uint8_t region)
{
    ldsr r6, 16, 5;                    /* Set parameter region to register SR16, 5 MPIDX */
    stsr    20, r10, 5;                /* Get value of SR20, 5 MPLA */
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)

/** Get the lower address of specific region
 * \param region The region number to be set [0-31]
 */
__STATIC_INLINE uint32_t RH850_MPU_GetLowerAddress (uint8_t region)
{
    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(region);

    uint32_t addr;

    __asm__ volatile (
        "ldsr  %1, 16, 5     \n"       /* Set parameter region to register SR16, 5 MPIDX */
        "stsr  20, %0, 5     \n"       /* Get value of SR20, 5 MPLA */
        : "=&r"(addr)
        : "r" (region)
        : "memory"
    );

    return addr;
}

#endif

#if defined(__CCRH__)

/* Get the upper address of specific region
 * \param region The region number to be set [0-31]
 */
 #pragma inline_asm RH850_MPU_GetUpperAddress
static uint32_t RH850_MPU_GetUpperAddress (uint8_t region)
{
    ldsr r6, 16, 5;                    /* Set parameter region to register SR16, 5 MPIDX */
    stsr    21, r10, 5;                /* Get value of SR21, 5 MPUA */
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)

/* Get the upper address of specific region
 * \param region The region number to be set [0-31]
 */
__STATIC_INLINE uint32_t RH850_MPU_GetUpperAddress (uint8_t region)
{
    uint32_t addr;

    __asm__ volatile (
        "ldsr  %1, 16, 5     \n"       /* Set parameter region to register SR16, 5 MPIDX */
        "stsr  21, %0, 5     \n"       /* Get value of SR20, 5 MPLA */
        : "=&r"(addr)
        : "r" (region)
        : "memory"
    );

    return addr;
}

#endif

#if defined(__CCRH__)

/* Execute memory protection checking.
 * \param spid System protection identifier.
 * \param lower_add Value for MCA register.
 * \param size Size of region.
 */
 #pragma inline_asm RH850_MPU_ExecuteMemProtectionCheck
static uint32_t RH850_MPU_ExecuteMemProtectionCheck (uint8_t spid, uint32_t lower_add, uint32_t size)
{
    ldsr r7, 8, 5;                     /* Set parameter lower_add to SR8, 5 MCA */
    ldsr r8, 9, 5;                     /* Set parameter size to  SR9, 5 MCS */
    ldsr r6, 12, 5;                    /* Set parameter spid to SR12, 5 MCI */
    ldsr r0, 10, 5;                    /* Start checking with   SR10, 5 MCC */
    stsr 11, r10, 5;                   /* Get the results from SR11, 5 MCR */
    ANDI 0x1FF, r10, r10
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)

/* Execute memory protection checking.
 * \param spid System protection identifier.
 * \param lower_add Value for MCA register.
 * \param size Size of region.
 */
__STATIC_INLINE uint32_t RH850_MPU_ExecuteMemProtectionCheck (uint8_t spid, uint32_t lower_add, uint32_t size)
{
    uint32_t status = 0;

    __asm__ volatile (
        "ldsr %1, 8, 5              \n" /* Set parameter lower_add to SR8, 5 MCA */
        "ldsr %2, 9, 5              \n" /* Set parameter size to  SR9, 5 MCS */
        "ldsr %3, 12, 5             \n" /* Set parameter spid to SR12, 5 MCI */
        "ldsr r0, 10, 5             \n" /* Start checking with   SR10, 5 MCC */
        "stsr 11, %0, 5             \n" /* Get the results from SR11, 5 MCR */
        "andi 0x1FF, %0, %0         \n"
        : "+r"(status)
        : "r"(lower_add), "r"(size), "r"(spid)
        : "memory"
    );

    return status;
}

#endif

/* Enable instruction cache.
 */
__STATIC_INLINE void RH850_MPU_InstructionCacheEnable (void)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "mov 3, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "mov 3, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
        :
        :
        : "r10"
    );
#endif
}

/* Disable instruction cache.
 */
__STATIC_INLINE void RH850_MPU_InstructionCacheDisable (void)
{
#if defined(__CCRH__)
    __asm__ volatile (
        "mov 0, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
    );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
    __asm__ volatile (
        "mov 0, r10                         \n"
        "ldsr r10, 24, 4                    \n" /* Set value SR24, 4 ICCTRL */
        "synci                              \n"
        :
        :
        : "r10"
    );
#endif
}

/* Get the total number of MPU region supported for this core.
 */
#if defined(__CCRH__)
 #pragma inline_asm RH850_MPU_GetTotalNumberofRegion
static uint8_t RH850_MPU_GetTotalNumberofRegion (void)
{
    stsr    2, r10, 5
    andi    31, r10, r10
    addi    1, r10, r10
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
__STATIC_INLINE uint8_t RH850_MPU_GetTotalNumberofRegion (void)
{
    uint8_t numOfRegions;

    __asm__ volatile (
        "stsr    2, %0, 5   \n"
        "andi    31, %0, %0 \n"
        "addi    1, %0, %0  \n"
        : "=&r"(numOfRegions)
        :
        : "memory"
    );

    return numOfRegions;
}

#endif

/* This function sets value for MPIDx register (x = 0 to 7).
 * \param no_of_reg No of MPID register.
 * \param value     Value for MPID register.
 */
__STATIC_INLINE void RH850_MPU_SetMPIDx (uint8_t no_of_reg, uint8_t value)
{
    /* Mark parameter as intentionally unused to avoid compiler warnings */
    FSP_PARAMETER_NOT_USED(value);

    /* No of MPID register */
    switch (no_of_reg)
    {
        case BSP_MPU_MPID_0:
        {
            /* Load r7 into MPID0 (SR24, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 24, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 24, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_1:
        {
            /* Load r7 into MPID1 (SR25, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 25, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 25, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_2:
        {
            /* Load r7 into MPID2 (SR26, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 26, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 26, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_3:
        {
            /* Load r7 into MPID3 (SR27, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 27, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 27, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_4:
        {
            /* Load r7 into MPID4 (SR28, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 28, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 28, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_5:
        {
            /* Load r7 into MPID5 (SR29, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 29, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 29, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_6:
        {
            /* Load r7 into MPID6 (SR30, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 30, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 30, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        case BSP_MPU_MPID_7:
        {
            /* Load r7 into MPID7 (SR31, select = 5) */
#if defined(__CCRH__)
            __asm__ volatile (
            " ldsr r7, 31, 5                 \n"
            );
#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
            __asm__ volatile (
                "ldsr %0, 31, 5                 \n"
                :
                : "r"(value)
                : "memory"
            );
#endif
            break;
        }

        default:
        {
            break;
        }
    }

    /* Synchronization barrier to ensure system register write completes */
    __asm__ volatile ("syncm \n");
}

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
void R_BSP_InstructionCacheEnable(void);
void R_BSP_InstructionCacheDisable(void);

/** @} (end addtogroup BSP_MPU) */

/** Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER
#endif

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/
#include "bsp_mpu.h"

/***********************************************************************************************************************
 * Variables and functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * MPU region configuration table.
 *
 * Each entry in this array defines a memory region with a name, base address, size and attributes.
 *
 * Maximum of 32 regions.
 **********************************************************************************************************************/
static bsp_mpu_region_t g_bsp_mpu_regions[] =
{
    BSP_MPU_REGIONn_CONFIG(00)
    BSP_MPU_REGIONn_CONFIG(01)
    BSP_MPU_REGIONn_CONFIG(02)
    BSP_MPU_REGIONn_CONFIG(03)
    BSP_MPU_REGIONn_CONFIG(04)
    BSP_MPU_REGIONn_CONFIG(05)
    BSP_MPU_REGIONn_CONFIG(06)
    BSP_MPU_REGIONn_CONFIG(07)
    BSP_MPU_REGIONn_CONFIG(08)
    BSP_MPU_REGIONn_CONFIG(09)
    BSP_MPU_REGIONn_CONFIG(10)
    BSP_MPU_REGIONn_CONFIG(11)
    BSP_MPU_REGIONn_CONFIG(12)
    BSP_MPU_REGIONn_CONFIG(13)
    BSP_MPU_REGIONn_CONFIG(14)
    BSP_MPU_REGIONn_CONFIG(15)
    BSP_MPU_REGIONn_CONFIG(16)
    BSP_MPU_REGIONn_CONFIG(17)
    BSP_MPU_REGIONn_CONFIG(18)
    BSP_MPU_REGIONn_CONFIG(19)
    BSP_MPU_REGIONn_CONFIG(20)
    BSP_MPU_REGIONn_CONFIG(21)
    BSP_MPU_REGIONn_CONFIG(22)
    BSP_MPU_REGIONn_CONFIG(23)
#if (32U == BSP_MPU_TOTAL_REGIONS_SUPPORTED)
    BSP_MPU_REGIONn_CONFIG(24)
    BSP_MPU_REGIONn_CONFIG(25)
    BSP_MPU_REGIONn_CONFIG(26)
    BSP_MPU_REGIONn_CONFIG(27)
    BSP_MPU_REGIONn_CONFIG(28)
    BSP_MPU_REGIONn_CONFIG(29)
    BSP_MPU_REGIONn_CONFIG(30)
    BSP_MPU_REGIONn_CONFIG(31)
#endif
};

bsp_mpu_config_t g_bsp_mpu_config =
{
    .mpu_region     = g_bsp_mpu_regions,
    .num_of_regions = BSP_MPU_NUMBER_OF_REGIONS_ENABLED,
};

/*******************************************************************************************************************//**
 * This function sets up the MPU by clearing non-configured regions and initializing all configured regions.
 * It prepares the MPU for enforcing memory access permissions based on predefined region settings.
 *
 * @param[in]  p_config              Pointer to MPU configuration structure.
 *
 * @retval  FSP_SUCCESS              Region was enabled successfully.
 * @retval  FSP_ERR_ASSERTION        Region's index exceeded max regions supported.
 **********************************************************************************************************************/
fsp_err_t R_BSP_MPUEnable (const bsp_mpu_config_t * const p_config)
{
    fsp_err_t          err                = FSP_SUCCESS;
    uint8_t            region_num         = 0U;
    bsp_mpu_region_t * mpu_regions        = (bsp_mpu_region_t *) (p_config->mpu_region);
    uint8_t            max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Check whether number of regions need to be configurated exceed max regions supported */
    FSP_ASSERT(p_config->num_of_regions <= max_num_of_regions);

    /* Disable the MPU */
    RH850_MPU_Disable();

    /* Initialize all configured regions */
    for (region_num = 0U; region_num < p_config->num_of_regions; region_num++)
    {
        /* Check whether region's index exceed max regions supported */
        FSP_ASSERT(mpu_regions->no_of_region < max_num_of_regions);

        /* Set lower address and upper address for region */
        RH850_MPU_SetRegion(mpu_regions->no_of_region, mpu_regions->start_address, mpu_regions->size);

        /* Set attribute for region */
        RH850_MPU_SetAttr(mpu_regions->no_of_region, mpu_regions->attribute);

        /* Process next region */
        mpu_regions++;
    }

    /* Enable the MPU */
    RH850_MPU_Enable(BSP_MPU_SV_MODE_RESTRICT_ACCESS);

    return err;
}

/*******************************************************************************************************************//**
 * This function disable the MPU by clearing all the regions.
 *
 * @retval None
 **********************************************************************************************************************/
void R_BSP_MPUDisable (void)
{
    uint8_t index;
    uint8_t max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Disable the MPU */
    RH850_MPU_Disable();

    /* Clear all MPID register */
    for (index = 0U; index < BSP_MPU_MAX_MPID_NUMBER; index++)
    {
        R_BSP_MPUSetMPIDx(index, 0);
    }

    /* Clear all regions */
    for (index = 0U; index < max_num_of_regions; index++)
    {
        RH850_MPU_SetRegion(index, 0, 0);
        RH850_MPU_SetAttr(index, 0);
    }
}

/*******************************************************************************************************************//**
 * This function return the number of regions enabled
 *
 * @retval Number of regions enabled
 **********************************************************************************************************************/
uint8_t R_BSP_MPURegionCountGet (void)
{
    uint8_t  region_num;
    uint8_t  region_count = 0U;
    uint32_t region_attr;
    uint8_t  max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    for (region_num = 0U; region_num < max_num_of_regions; region_num++)
    {
        /* Get attribute for specific region */
        region_attr = RH850_MPU_GetAttr(region_num);
        if (0U != (region_attr & BSP_MPU_REGION_ENABLE))
        {
            region_count++;
        }
    }

    return region_count;
}

/*******************************************************************************************************************//**
 * This function set value for specific MPIDx register (x = 0 to 7)
 *
 * @param[in]  no_of_reg            MPID register's number
 * @param[in]  value                Value to be set
 *
 * @retval None
 **********************************************************************************************************************/
void R_BSP_MPUSetMPIDx (uint8_t no_of_reg, uint8_t value)
{
    value &= BSP_MPU_MPID_MASK;

    /* Set value for specific MPID register */
    RH850_MPU_SetMPIDx(no_of_reg, value);
}

/*******************************************************************************************************************//**
 * This function enables a specific region.
 *
 * @param[in]  p_region                Pointer to specific region configuration structure.
 *
 * @retval  FSP_SUCCESS              Region was enabled successfully.
 * @retval  FSP_ERR_ASSERTION        r_index exceeded max region supported.
 **********************************************************************************************************************/
fsp_err_t R_BSP_MPURegionEnable (bsp_mpu_region_t * const p_region)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t  attr_value;
    uint8_t   max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Check whether region's index exceed max regions supported */
    FSP_ASSERT(p_region->no_of_region < max_num_of_regions);

    /* Get attribute */
    attr_value = RH850_MPU_GetAttr(p_region->no_of_region);

    /* Enable region (set bit 7 in attribute value to 1) */
    attr_value |= BSP_MPU_REGION_ENABLE;

    /* Set attribute value for region again */
    RH850_MPU_SetAttr(p_region->no_of_region, attr_value);

    p_region->attribute_b.region_enable = 1U;

    return err;
}

/*******************************************************************************************************************//**
 * This function disables a specific region.
 *
 * @param[in]  p_region                Pointer to specific region configuration structure.
 *
 * @retval  FSP_SUCCESS              Region was disabled successfully.
 * @retval  FSP_ERR_ASSERTION        r_index exceeded max region supported.
 **********************************************************************************************************************/
fsp_err_t R_BSP_MPURegionDisable (bsp_mpu_region_t * const p_region)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t  attr_value;
    uint8_t   max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Check whether region's index exceed max regions supported */
    FSP_ASSERT(p_region->no_of_region < max_num_of_regions);

    /* Get attribute */
    attr_value = RH850_MPU_GetAttr(p_region->no_of_region);

    /* Disable region (clear bit 7 in attribute value) */
    attr_value &= (uint32_t) ~BSP_MPU_REGION_ENABLE;

    /* Set attribute value for region again */
    RH850_MPU_SetAttr(p_region->no_of_region, attr_value);

    p_region->attribute_b.region_enable = 0U;

    return err;
}

/*******************************************************************************************************************//**
 * This function sets up the access rules for a specific region.
 *
 * @param[in]  p_region                Pointer to specific region configuration structure.
 *
 * @retval  FSP_SUCCESS              Region was configured successfully.
 * @retval  FSP_ERR_ASSERTION        r_index exceeded max region supported.
 **********************************************************************************************************************/
fsp_err_t R_BSP_MPURegionSet (bsp_mpu_region_t * const p_region)
{
    fsp_err_t err                = FSP_SUCCESS;
    uint8_t   max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Check whether region's index exceed max regions supported */
    FSP_ASSERT(p_region->no_of_region < max_num_of_regions);

    /* Set lower address and upper address for region */
    RH850_MPU_SetRegion(p_region->no_of_region, p_region->start_address, p_region->size);

    /* Set attribute for region */
    RH850_MPU_SetAttr(p_region->no_of_region, (p_region->attribute));

    return err;
}

/*******************************************************************************************************************//**
 * This function gets the current settings for a specific region and store it in the passing argument.
 *
 * @param[in]  p_region_data           Pointer to specific region configuration structure.
 * @param[in]  r_index                 Index of specific region that user want to get setting.
 *
 * @retval  FSP_SUCCESS              Get region successfully.
 * @retval  FSP_ERR_ASSERTION        r_index exceeded max region supported.
 **********************************************************************************************************************/
fsp_err_t R_BSP_MPURegionGet (bsp_mpu_region_t * const p_region_data, uint8_t r_index)
{
    fsp_err_t err = FSP_SUCCESS;
    uint32_t  attr_value;
    uint8_t   max_num_of_regions = RH850_MPU_GetTotalNumberofRegion();

    /* Check whether region's index exceed max regions supported */
    FSP_ASSERT(r_index < max_num_of_regions);

    p_region_data->start_address = RH850_MPU_GetLowerAddress(r_index);
    p_region_data->size          = RH850_MPU_GetUpperAddress(r_index) - RH850_MPU_GetLowerAddress(r_index);
    attr_value                  = RH850_MPU_GetAttr(r_index);
    p_region_data->attribute    = attr_value;
    p_region_data->no_of_region = r_index;

    return err;
}

/*******************************************************************************************************************//**
 * This function executes memory protection check for a SPID's specified address area. Checking range is from
 * "lower_add" to "lower_add" + "size" - 1.
 * (Value of SPID for each CPU can be read via register SR0, 1)
 *
 * @param[in]  spid                                System protection identifier.
 * @param[in]  lower_add                           Value for MPLA register.
 * @param[in]  size                                Size of region.
 * @param[out]  p_status                           Pointer to the variable that stores the status result.
 **********************************************************************************************************************/
void R_BSP_MPUExecuteMemProtectionCheck (uint8_t                      spid,
                                         uint32_t                     lower_add,
                                         uint32_t                     size,
                                         bsp_mpu_mem_check_status_t * p_status)
{
    uint32_t res;
    res = RH850_MPU_ExecuteMemProtectionCheck(spid, lower_add, size);
    p_status->mem_check_status = res;
}

/*******************************************************************************************************************//**
 * Enable Instruction Cache.
 *
 * @retval None
 **********************************************************************************************************************/
void R_BSP_InstructionCacheEnable (void)
{
    RH850_MPU_InstructionCacheEnable();
}

/*******************************************************************************************************************//**
 * Disable Instruction Cache.
 *
 * @retval None
 **********************************************************************************************************************/
void R_BSP_InstructionCacheDisable (void)
{
    RH850_MPU_InstructionCacheDisable();
}

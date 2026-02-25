/* generated configuration header file - do not edit */
#ifndef BSP_MCU_CPU_MEMORY_CFG_U2C8_H_
#define BSP_MCU_CPU_MEMORY_CFG_U2C8_H_

#ifndef BSP_REGION00_ENABLE
 #define BSP_REGION00_ENABLE    1U
#endif

#ifndef BSP_REGION01_ENABLE
 #define BSP_REGION01_ENABLE    1U
#endif

#ifndef BSP_REGION02_ENABLE
 #define BSP_REGION02_ENABLE    1U
#endif

#ifndef BSP_REGION03_ENABLE
 #define BSP_REGION03_ENABLE    1U
#endif

#ifndef BSP_REGION04_ENABLE
 #define BSP_REGION04_ENABLE    1U
#endif

#ifndef BSP_REGION05_ENABLE
 #define BSP_REGION05_ENABLE    1U
#endif

#ifndef BSP_REGION06_ENABLE
 #define BSP_REGION06_ENABLE    1U
#endif

#ifndef BSP_REGION07_ENABLE
 #define BSP_REGION07_ENABLE    1U
#endif

#ifndef BSP_REGION08_ENABLE
 #define BSP_REGION08_ENABLE    1U
#endif

#ifndef BSP_REGION09_ENABLE
 #define BSP_REGION09_ENABLE    1U
#endif

#ifndef BSP_REGION10_ENABLE
 #define BSP_REGION10_ENABLE    1U
#endif

#ifndef BSP_REGION11_ENABLE
 #define BSP_REGION11_ENABLE    0U
#endif

#ifndef BSP_REGION12_ENABLE
 #define BSP_REGION12_ENABLE    0U
#endif

#ifndef BSP_REGION13_ENABLE
 #define BSP_REGION13_ENABLE    0U
#endif

#ifndef BSP_REGION14_ENABLE
 #define BSP_REGION14_ENABLE    0U
#endif

#ifndef BSP_REGION15_ENABLE
 #define BSP_REGION15_ENABLE    0U
#endif

#ifndef BSP_REGION16_ENABLE
 #define BSP_REGION16_ENABLE    0U
#endif

#ifndef BSP_REGION17_ENABLE
 #define BSP_REGION17_ENABLE    0U
#endif

#ifndef BSP_REGION18_ENABLE
 #define BSP_REGION18_ENABLE    0U
#endif

#ifndef BSP_REGION19_ENABLE
 #define BSP_REGION19_ENABLE    0U
#endif

#ifndef BSP_REGION20_ENABLE
 #define BSP_REGION20_ENABLE    0U
#endif

#ifndef BSP_REGION21_ENABLE
 #define BSP_REGION21_ENABLE    0U
#endif

#ifndef BSP_REGION22_ENABLE
 #define BSP_REGION22_ENABLE    0U
#endif

#ifndef BSP_REGION23_ENABLE
 #define BSP_REGION23_ENABLE    0U
#endif

#if (BSP_REGION00_ENABLE == 0U)
 #ifndef BSP_REGION00_CONFIG
  #define BSP_REGION00_CONFIG
 #endif

#else
 #define BSP_REGION00_CONFIG                                                                                 \
    {                                                                                                        \
        .name          = "CODE_FLASH",                                                                       \
        .start_address = 0x00000000,                                                                         \
        .size          = 0x800000,                                                                           \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | (((BSP_MPU_SR_EN) | (BSP_MPU_UR_EN))) | \
                         ((BSP_MPU_UX_EN | BSP_MPU_SX_EN)),                                                  \
        .no_of_region = 0U                                                                                   \
    },
#endif

#if (BSP_REGION01_ENABLE == 0U)
 #ifndef BSP_REGION01_CONFIG
  #define BSP_REGION01_CONFIG
 #endif

#else
 #define BSP_REGION01_CONFIG                                                                         \
    {                                                                                                \
        .name          = "LOCAL_RAM_0",                                                              \
        .start_address = 0xFDC00000,                                                                 \
        .size          = 0x20000,                                                                    \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 1U                                                                           \
    },
#endif

#if (BSP_REGION02_ENABLE == 0U)
 #ifndef BSP_REGION02_CONFIG
  #define BSP_REGION02_CONFIG
 #endif

#else
 #define BSP_REGION02_CONFIG                                                                         \
    {                                                                                                \
        .name          = "LOCAL_RAM_1",                                                              \
        .start_address = 0xFDA00000,                                                                 \
        .size          = 0x20000,                                                                    \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 2U                                                                           \
    },
#endif

#if (BSP_REGION03_ENABLE == 0U)
 #ifndef BSP_REGION03_CONFIG
  #define BSP_REGION03_CONFIG
 #endif

#else
 #define BSP_REGION03_CONFIG                                                                         \
    {                                                                                                \
        .name          = "CLUSTER_RAM_0",                                                            \
        .start_address = 0xFE000000,                                                                 \
        .size          = 0x130000,                                                                   \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 3U                                                                           \
    },
#endif

#if (BSP_REGION04_ENABLE == 0U)
 #ifndef BSP_REGION04_CONFIG
  #define BSP_REGION04_CONFIG
 #endif

#else
 #define BSP_REGION04_CONFIG                                                                   \
    {                                                                                          \
        .name          = "RETENTION_RAM",                                                      \
        .start_address = 0x12000000,                                                           \
        .size          = 0x8000,                                                               \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) |                           \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | \
                         ((BSP_MPU_UX_EN | BSP_MPU_SX_EN)),                                    \
        .no_of_region = 4U                                                                     \
    },
#endif

#if (BSP_REGION05_ENABLE == 0U)
 #ifndef BSP_REGION05_CONFIG
  #define BSP_REGION05_CONFIG
 #endif

#else
 #define BSP_REGION05_CONFIG                                                                                   \
    {                                                                                                          \
        .name          = "P_Bus_Area",                                                                         \
        .start_address = 0xFF000000,                                                                           \
        .size          = 0xFB8000,                                                                             \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN | BSP_MPU_SW_EN)) | (0U), \
        .no_of_region  = 5U                                                                                    \
    },
#endif

#if (BSP_REGION06_ENABLE == 0U)
 #ifndef BSP_REGION06_CONFIG
  #define BSP_REGION06_CONFIG
 #endif

#else
 #define BSP_REGION06_CONFIG                                                                                   \
    {                                                                                                          \
        .name          = "I_Bus_Area",                                                                         \
        .start_address = 0xFFFB8000,                                                                           \
        .size          = 0x8000,                                                                               \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN | BSP_MPU_SW_EN)) | (0U), \
        .no_of_region  = 6U                                                                                    \
    },
#endif

#if (BSP_REGION07_ENABLE == 0U)
 #ifndef BSP_REGION07_CONFIG
  #define BSP_REGION07_CONFIG
 #endif

#else
 #define BSP_REGION07_CONFIG                                                                                   \
    {                                                                                                          \
        .name          = "PERIPHERAL",                                                                         \
        .start_address = 0xFFFC4000,                                                                           \
        .size          = 0x8000,                                                                               \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN | BSP_MPU_SW_EN)) | (0U), \
        .no_of_region  = 7U                                                                                    \
    },
#endif

#if (BSP_REGION08_ENABLE == 0U)
 #ifndef BSP_REGION08_CONFIG
  #define BSP_REGION08_CONFIG
 #endif

#else
 #define BSP_REGION08_CONFIG                                                                   \
    {                                                                                          \
        .name          = "OPBT",                                                               \
        .start_address = 0xFF321300,                                                           \
        .size          = 0x150,                                                                \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN)) | (0U), \
        .no_of_region  = 8U                                                                    \
    },
#endif

#if (BSP_REGION09_ENABLE == 0U)
 #ifndef BSP_REGION09_CONFIG
  #define BSP_REGION09_CONFIG
 #endif

#else
 #define BSP_REGION09_CONFIG                                                                                   \
    {                                                                                                          \
        .name          = "H_Bus_Area_1",                                                                       \
        .start_address = 0x10000000,                                                                           \
        .size          = 0x1FFFFFC,                                                                            \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN | BSP_MPU_SW_EN)) | (0U), \
        .no_of_region  = 9U                                                                                    \
    },
#endif

#if (BSP_REGION10_ENABLE == 0U)
 #ifndef BSP_REGION10_CONFIG
  #define BSP_REGION10_CONFIG
 #endif

#else
 #define BSP_REGION10_CONFIG                                                                                   \
    {                                                                                                          \
        .name          = "H_Bus_Area_2",                                                                       \
        .start_address = 0x12008000,                                                                           \
        .size          = 0x2DFF8000,                                                                           \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (1U << 7) | ((BSP_MPU_SR_EN | BSP_MPU_SW_EN)) | (0U), \
        .no_of_region  = 10U                                                                                   \
    },
#endif

#if (BSP_REGION11_ENABLE == 0U)
 #ifndef BSP_REGION11_CONFIG
  #define BSP_REGION11_CONFIG
 #endif

#else
 #define BSP_REGION11_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 11U                                                                          \
    },
#endif

#if (BSP_REGION12_ENABLE == 0U)
 #ifndef BSP_REGION12_CONFIG
  #define BSP_REGION12_CONFIG
 #endif

#else
 #define BSP_REGION12_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 12U                                                                          \
    },
#endif

#if (BSP_REGION13_ENABLE == 0U)
 #ifndef BSP_REGION13_CONFIG
  #define BSP_REGION13_CONFIG
 #endif

#else
 #define BSP_REGION13_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 13U                                                                          \
    },
#endif

#if (BSP_REGION14_ENABLE == 0U)
 #ifndef BSP_REGION14_CONFIG
  #define BSP_REGION14_CONFIG
 #endif

#else
 #define BSP_REGION14_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 14U                                                                          \
    },
#endif

#if (BSP_REGION15_ENABLE == 0U)
 #ifndef BSP_REGION15_CONFIG
  #define BSP_REGION15_CONFIG
 #endif

#else
 #define BSP_REGION15_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 15U                                                                          \
    },
#endif

#if (BSP_REGION16_ENABLE == 0U)
 #ifndef BSP_REGION16_CONFIG
  #define BSP_REGION16_CONFIG
 #endif

#else
 #define BSP_REGION16_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 16U                                                                          \
    },
#endif

#if (BSP_REGION17_ENABLE == 0U)
 #ifndef BSP_REGION17_CONFIG
  #define BSP_REGION17_CONFIG
 #endif

#else
 #define BSP_REGION17_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 17U                                                                          \
    },
#endif

#if (BSP_REGION18_ENABLE == 0U)
 #ifndef BSP_REGION18_CONFIG
  #define BSP_REGION18_CONFIG
 #endif

#else
 #define BSP_REGION18_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 18U                                                                          \
    },
#endif

#if (BSP_REGION19_ENABLE == 0U)
 #ifndef BSP_REGION19_CONFIG
  #define BSP_REGION19_CONFIG
 #endif

#else
 #define BSP_REGION19_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 19U                                                                          \
    },
#endif

#if (BSP_REGION20_ENABLE == 0U)
 #ifndef BSP_REGION20_CONFIG
  #define BSP_REGION20_CONFIG
 #endif

#else
 #define BSP_REGION20_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 20U                                                                          \
    },
#endif

#if (BSP_REGION21_ENABLE == 0U)
 #ifndef BSP_REGION21_CONFIG
  #define BSP_REGION21_CONFIG
 #endif

#else
 #define BSP_REGION21_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 21U                                                                          \
    },
#endif

#if (BSP_REGION22_ENABLE == 0U)
 #ifndef BSP_REGION22_CONFIG
  #define BSP_REGION22_CONFIG
 #endif

#else
 #define BSP_REGION22_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 22U                                                                          \
    },
#endif

#if (BSP_REGION23_ENABLE == 0U)
 #ifndef BSP_REGION23_CONFIG
  #define BSP_REGION23_CONFIG
 #endif

#else
 #define BSP_REGION23_CONFIG                                                                         \
    {                                                                                                \
        .name          = "Not_Used",                                                                 \
        .start_address = 0x00000000,                                                                 \
        .size          = 0x00000000,                                                                 \
        .attribute     = BSP_MPU_RG_EN | BSP_MPU_WG_EN | (0U << 7) |                                 \
                         ((BSP_MPU_SR_EN | BSP_MPU_SW_EN) | (BSP_MPU_UR_EN | BSP_MPU_UW_EN)) | (0U), \
        .no_of_region = 23U                                                                          \
    },
#endif

#ifndef BSP_MPU_MPID0_VALUE
 #define BSP_MPU_MPID0_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID1_VALUE
 #define BSP_MPU_MPID1_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID2_VALUE
 #define BSP_MPU_MPID2_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID3_VALUE
 #define BSP_MPU_MPID3_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID4_VALUE
 #define BSP_MPU_MPID4_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID5_VALUE
 #define BSP_MPU_MPID5_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID6_VALUE
 #define BSP_MPU_MPID6_VALUE                  0x00
#endif

#ifndef BSP_MPU_MPID7_VALUE
 #define BSP_MPU_MPID7_VALUE                  0x00
#endif

#ifndef BSP_MPU_NUMBER_OF_REGIONS_ENABLED
 #define BSP_MPU_NUMBER_OF_REGIONS_ENABLED    0xBU
#endif

#ifndef BSP_MPU_SV_MODE_RESTRICT_ACCESS
 #define BSP_MPU_SV_MODE_RESTRICT_ACCESS      BSP_MPU_PRIV_MODE_RESTRICT_ACCESS
#endif

#ifndef BSP_MPU_CONFIG_ENABLE
 #define BSP_MPU_CONFIG_ENABLE                BSP_MPU_ENABLE
#endif

#ifndef BSP_MPU_INSTRUCTION_CACHE_ENABLED
 #define BSP_MPU_INSTRUCTION_CACHE_ENABLED    1U
#endif

#ifndef BSP_MPU_TOTAL_REGIONS_SUPPORTED
 #define BSP_MPU_TOTAL_REGIONS_SUPPORTED      24U
#endif

#endif                                 /* BSP_MCU_CPU_MEMORY_CFG_H_ */

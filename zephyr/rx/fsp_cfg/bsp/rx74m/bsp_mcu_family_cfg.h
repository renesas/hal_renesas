/* generated configuration header file - do not edit */
#ifndef BSP_MCU_FAMILY_CFG_H_
#define BSP_MCU_FAMILY_CFG_H_
#include "bsp_mcu_device_pn_cfg.h"
#include "bsp_mcu_device_cfg.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/bsp_override.h"
#include "../../../rx/fsp/src/bsp/mcu/rx74m/bsp_mcu_info.h"
#include "bsp_clock_cfg.h"
#define BSP_MCU_GROUP_RX74M (1)

#define BSP_LOCO_HZ                 (32768)
#define BSP_MOCO_HZ                 (8000000)
#define BSP_SUB_CLOCK_HZ            (32768)
#if   BSP_CFG_HOCO_FREQUENCY == 0
#define BSP_HOCO_HZ                 (16000000)
#elif BSP_CFG_HOCO_FREQUENCY == 1
            #define BSP_HOCO_HZ                 (18000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 2
            #define BSP_HOCO_HZ                 (20000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 4
            #define BSP_HOCO_HZ                 (32000000)
        #elif BSP_CFG_HOCO_FREQUENCY == 7
            #define BSP_HOCO_HZ                 (48000000)
        #else
            #error "Invalid HOCO frequency chosen (BSP_CFG_HOCO_FREQUENCY) in bsp_clock_cfg.h"
        #endif

#define BSP_CFG_FLL_ENABLE                   (0)

#define BSP_CFG_CLOCK_SETTLING_DELAY_ENABLE  (1)
#define BSP_CFG_SLEEP_MODE_DELAY_ENABLE      (1)
#define BSP_CFG_RTOS_SLEEP_MODE_DELAY_ENABLE (1)
#define BSP_CFG_CLOCK_SETTLING_DELAY_US      (150)

#define OFS_SEQ1 0xA001A001 | (1 << 1) | (3 << 2)
#define OFS_SEQ2 (15 << 4) | (3 << 8) | (3 << 10)
#define OFS_SEQ3 (1 << 12) | (1 << 14) | (1 << 17)
#define OFS_SEQ4 (3 << 18) |(15 << 20) | (3 << 24) | (3 << 26)
#define OFS_SEQ5 (1 << 28) | (1 << 30)
#define BSP_CFG_ROM_REG_OFS0 (OFS_SEQ1 | OFS_SEQ2 | OFS_SEQ3 | OFS_SEQ4 | OFS_SEQ5)

#define BSP_CFG_ROM_REG_OFS2 ((1 << 0) | (0 << 1) | 0xFFFFFFECU)

/* Option Function Select Register 1 Security Attribution */
#ifndef BSP_CFG_ROM_REG_OFS1_SEL
#define BSP_CFG_ROM_REG_OFS1_SEL (0x00000000U)
#endif
#define BSP_CFG_ROM_REG_OFS1 (0x0FFFFED0 | (1 << 3) | (7) | (1 << 5) | (1 << 8) | (7 << 28) | (1 << 31))

#define BSP_CFG_ROM_REG_OFS1_SEC BSP_CFG_ROM_REG_OFS1

/* Option Function Select Register 3 Security Attribution */
#ifndef BSP_CFG_ROM_REG_OFS3_SEL
#define BSP_CFG_ROM_REG_OFS3_SEL (0x00000000U)
#endif
#define BSP_CFG_ROM_REG_OFS3 (0xA001FFFFU | (1 << 17) | (3 << 18) | (15 << 20) |  (3 << 24) | (3 << 26) | (1 << 28) | (1 << 30))

#define BSP_CFG_ROM_REG_OFS3_SEC BSP_CFG_ROM_REG_OFS3

/* Dual Mode Select Register */
#ifndef BSP_CFG_ROM_REG_DUALSEL
#define BSP_CFG_ROM_REG_DUALSEL (0xFFFFFFF8U | (0x7U))
#endif

/* Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_BPS0
#define BSP_CFG_ROM_REG_BPS0 (0xFFFFFFFF)
#endif
/* Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_BPS1
#define BSP_CFG_ROM_REG_BPS1 (0xFFFFFFFF)
#endif
/* Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_BPS2
#define BSP_CFG_ROM_REG_BPS2 (0xFFFFFFFF)
#endif
/* Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_BPS3
#define BSP_CFG_ROM_REG_BPS3 (0xFFFFFFFF)
#endif
/* Block Protection Register 4 */
#ifndef BSP_CFG_ROM_REG_BPS4
#define BSP_CFG_ROM_REG_BPS4 (0xFFFFFFFF)
#endif
/* Block Protection Register 5 */
#ifndef BSP_CFG_ROM_REG_BPS5
#define BSP_CFG_ROM_REG_BPS5 (0xFFFFFFFF)
#endif
/* Block Protection Register 6 */
#ifndef BSP_CFG_ROM_REG_BPS6
#define BSP_CFG_ROM_REG_BPS6 (0xFFFFFFFF)
#endif
/* Block Protection Register 7 */
#ifndef BSP_CFG_ROM_REG_BPS7
#define BSP_CFG_ROM_REG_BPS7 (0xFFFFFFFF)
#endif
/* Block Protection Register 8 */
#ifndef BSP_CFG_ROM_REG_BPS8
#define BSP_CFG_ROM_REG_BPS8 (0xFFFFFFFF)
#endif
/* Block Protection Register 9 */
#ifndef BSP_CFG_ROM_REG_BPS9
#define BSP_CFG_ROM_REG_BPS9 (0xFFFFFFFF)
#endif
/* Block Protection Register 10 */
#ifndef BSP_CFG_ROM_REG_BPS10
#define BSP_CFG_ROM_REG_BPS10 (0xFFFFFFFF)
#endif
/* Block Protection Register 11 */
#ifndef BSP_CFG_ROM_REG_BPS11
#define BSP_CFG_ROM_REG_BPS11 (0xFFFFFFFF)
#endif
/* Block Protection Register 12 */
#ifndef BSP_CFG_ROM_REG_BPS12
#define BSP_CFG_ROM_REG_BPS12 (0xFFFFFFFF)
#endif
/* Block Protection Register 13 */
#ifndef BSP_CFG_ROM_REG_BPS13
#define BSP_CFG_ROM_REG_BPS13 (0xFFFFFFFF)
#endif
/* Block Protection Register 14 */
#ifndef BSP_CFG_ROM_REG_BPS14
#define BSP_CFG_ROM_REG_BPS14 (0xFFFFFFFF)
#endif
/* Block Protection Register 15 */
#ifndef BSP_CFG_ROM_REG_BPS15
#define BSP_CFG_ROM_REG_BPS15 (0xFFFFFFFF)
#endif
/* Block Protection Register 64 */
#ifndef BSP_CFG_ROM_REG_BPS64
#define BSP_CFG_ROM_REG_BPS64 (0xFFFFFFFF)
#endif
/* Block Protection Register 65 */
#ifndef BSP_CFG_ROM_REG_BPS65
#define BSP_CFG_ROM_REG_BPS65 (0xFFFFFFFF)
#endif
/* Block Protection Register 66 */
#ifndef BSP_CFG_ROM_REG_BPS66
#define BSP_CFG_ROM_REG_BPS66 (0xFFFFFFFF)
#endif
/* Block Protection Register 67 */
#ifndef BSP_CFG_ROM_REG_BPS67
#define BSP_CFG_ROM_REG_BPS67 (0xFFFFFFFF)
#endif
/* Block Protection Register 68 */
#ifndef BSP_CFG_ROM_REG_BPS68
#define BSP_CFG_ROM_REG_BPS68 (0xFFFFFFFF)
#endif
/* Block Protection Register 69 */
#ifndef BSP_CFG_ROM_REG_BPS69
#define BSP_CFG_ROM_REG_BPS69 (0xFFFFFFFF)
#endif
/* Block Protection Register 70 */
#ifndef BSP_CFG_ROM_REG_BPS70
#define BSP_CFG_ROM_REG_BPS70 (0xFFFFFFFF)
#endif
/* Block Protection Register 71 */
#ifndef BSP_CFG_ROM_REG_BPS71
#define BSP_CFG_ROM_REG_BPS71 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_PBPS0
#define BSP_CFG_ROM_REG_PBPS0 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_PBPS1
#define BSP_CFG_ROM_REG_PBPS1 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_PBPS2
#define BSP_CFG_ROM_REG_PBPS2 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_PBPS3
#define BSP_CFG_ROM_REG_PBPS3 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 16 */
#ifndef BSP_CFG_ROM_REG_PBPS16
#define BSP_CFG_ROM_REG_PBPS16 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 17 */
#ifndef BSP_CFG_ROM_REG_PBPS17
#define BSP_CFG_ROM_REG_PBPS17 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC0
#define BSP_CFG_ROM_REG_BPS_SEC0 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC1
#define BSP_CFG_ROM_REG_BPS_SEC1 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC2
#define BSP_CFG_ROM_REG_BPS_SEC2 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC3
#define BSP_CFG_ROM_REG_BPS_SEC3 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 4 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC4
#define BSP_CFG_ROM_REG_BPS_SEC4 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 5 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC5
#define BSP_CFG_ROM_REG_BPS_SEC5 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 6 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC6
#define BSP_CFG_ROM_REG_BPS_SEC6 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 7 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC7
#define BSP_CFG_ROM_REG_BPS_SEC7 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 8 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC8
#define BSP_CFG_ROM_REG_BPS_SEC8 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 9 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC9
#define BSP_CFG_ROM_REG_BPS_SEC9 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 10 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC10
#define BSP_CFG_ROM_REG_BPS_SEC10 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 11 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC11
#define BSP_CFG_ROM_REG_BPS_SEC11 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 12 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC12
#define BSP_CFG_ROM_REG_BPS_SEC12 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 13 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC13
#define BSP_CFG_ROM_REG_BPS_SEC13 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 14 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC14
#define BSP_CFG_ROM_REG_BPS_SEC14 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 15 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC15
#define BSP_CFG_ROM_REG_BPS_SEC15 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 64 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC64
#define BSP_CFG_ROM_REG_BPS_SEC64 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 65 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC65
#define BSP_CFG_ROM_REG_BPS_SEC65 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 66 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC66
#define BSP_CFG_ROM_REG_BPS_SEC66 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 67 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC67
#define BSP_CFG_ROM_REG_BPS_SEC67 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 68 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC68
#define BSP_CFG_ROM_REG_BPS_SEC68 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 69 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC69
#define BSP_CFG_ROM_REG_BPS_SEC69 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 70 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC70
#define BSP_CFG_ROM_REG_BPS_SEC70 (0xFFFFFFFF)
#endif
/* Secure Block Protection Register 71 */
#ifndef BSP_CFG_ROM_REG_BPS_SEC71
#define BSP_CFG_ROM_REG_BPS_SEC71 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 0 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC0
#define BSP_CFG_ROM_REG_PBPS_SEC0 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 1 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC1
#define BSP_CFG_ROM_REG_PBPS_SEC1 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 2 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC2
#define BSP_CFG_ROM_REG_PBPS_SEC2 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 3 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC3
#define BSP_CFG_ROM_REG_PBPS_SEC3 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 16 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC16
#define BSP_CFG_ROM_REG_PBPS_SEC16 (0xFFFFFFFF)
#endif
/* Permanent Block Protection Register 17 */
#ifndef BSP_CFG_ROM_REG_PBPS_SEC17
#define BSP_CFG_ROM_REG_PBPS_SEC17 (0xFFFFFFFF)
#endif

/* Block Protection for Data MRAM Register  */
#ifndef BSP_CFG_ROM_REG_BPSDM
#define BSP_CFG_ROM_REG_BPSDM (0xFFFFFFFF)
#endif

/* Block Protection for Data MRAM Register  */
#ifndef BSP_CFG_ROM_REG_BPSDM_SEC
#define BSP_CFG_ROM_REG_BPSDM_SEC (0xFFFFFFFF)
#endif

/* Permanent Block Protection for Data MRAM Register */
#ifndef BSP_CFG_ROM_REG_PBPSDM
#define BSP_CFG_ROM_REG_PBPSDM (0xFFFFFFFF)
#endif

/* Permanent Block Protection for Data MRAM Register */
#ifndef BSP_CFG_ROM_REG_PBPSDM_SEC
#define BSP_CFG_ROM_REG_PBPSDM_SEC (0xFFFFFFFF)
#endif

/* Security Attribution for Bank Select Register */
#ifndef BSP_CFG_ROM_REG_BANKSEL_SEL
#define BSP_CFG_ROM_REG_BANKSEL_SEL (0x00000000U)
#endif

/* FSBL Control Register 0 */
#ifndef BSP_CFG_ROM_REG_FSBLCTRL0
#define BSP_CFG_ROM_REG_FSBLCTRL0 ( \
                (7 << (0UL)) | \
                (7 << (3UL)) | \
                (7 << (6UL)) | \
                (7 << (9UL)) | \
                0xFFFFF000)
#endif

/* FSBL Control Register 1 */
#ifndef BSP_CFG_ROM_REG_FSBLCTRL1
#define BSP_CFG_ROM_REG_FSBLCTRL1 ( \
                (3 << (0UL)) | \
                0xFFFFFFFC)
#endif

/* FSBL Control Register 2 */
#ifndef BSP_CFG_ROM_REG_FSBLCTRL2
#define BSP_CFG_ROM_REG_FSBLCTRL2 ( \
                (15 << (0UL)) | \
                (0x1F << (4UL)) | \
                0xFFFFFE00)
#endif
/* Start Address of Code Certificate Register 00 */
#ifndef BSP_CFG_ROM_REG_SACC0
#define BSP_CFG_ROM_REG_SACC0 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 01 */
#ifndef BSP_CFG_ROM_REG_SACC1
#define BSP_CFG_ROM_REG_SACC1 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 02 */
#ifndef BSP_CFG_ROM_REG_SACC2
#define BSP_CFG_ROM_REG_SACC2 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 03 */
#ifndef BSP_CFG_ROM_REG_SACC3
#define BSP_CFG_ROM_REG_SACC3 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 10 */
#ifndef BSP_CFG_ROM_REG_SACC10
#define BSP_CFG_ROM_REG_SACC10 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 11 */
#ifndef BSP_CFG_ROM_REG_SACC11
#define BSP_CFG_ROM_REG_SACC11 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 12 */
#ifndef BSP_CFG_ROM_REG_SACC12
#define BSP_CFG_ROM_REG_SACC12 (0xFFFFFFFF)
#endif
/* Start Address of Code Certificate Register 13 */
#ifndef BSP_CFG_ROM_REG_SACC13
#define BSP_CFG_ROM_REG_SACC13 (0xFFFFFFFF)
#endif

/* Start Address of Measurement Report Register */
#ifndef BSP_CFG_ROM_REG_SAMR
#define BSP_CFG_ROM_REG_SAMR (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_ARCCS
#define BSP_CFG_ROM_REG_ARCCS           (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL
#define BSP_CFG_ROM_REG_BLSEL           (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL_SEC
#define BSP_CFG_ROM_REG_BLSEL_SEC       (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_BLSEL_SEL
#define BSP_CFG_ROM_REG_BLSEL_SEL       (0x00000000)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_0
#define BSP_CFG_ROM_REG_GPOTP_0         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_1
#define BSP_CFG_ROM_REG_GPOTP_1         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_2
#define BSP_CFG_ROM_REG_GPOTP_2         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_3
#define BSP_CFG_ROM_REG_GPOTP_3         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_4
#define BSP_CFG_ROM_REG_GPOTP_4         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_5
#define BSP_CFG_ROM_REG_GPOTP_5         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_6
#define BSP_CFG_ROM_REG_GPOTP_6         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_7
#define BSP_CFG_ROM_REG_GPOTP_7         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_8
#define BSP_CFG_ROM_REG_GPOTP_8         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_9
#define BSP_CFG_ROM_REG_GPOTP_9         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_10
#define BSP_CFG_ROM_REG_GPOTP_10        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_11
#define BSP_CFG_ROM_REG_GPOTP_11        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_12
#define BSP_CFG_ROM_REG_GPOTP_12        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_13
#define BSP_CFG_ROM_REG_GPOTP_13        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_14
#define BSP_CFG_ROM_REG_GPOTP_14        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_15
#define BSP_CFG_ROM_REG_GPOTP_15        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_16
#define BSP_CFG_ROM_REG_GPOTP_16        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_17
#define BSP_CFG_ROM_REG_GPOTP_17        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_18
#define BSP_CFG_ROM_REG_GPOTP_18        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_19
#define BSP_CFG_ROM_REG_GPOTP_19        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_20
#define BSP_CFG_ROM_REG_GPOTP_20        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_21
#define BSP_CFG_ROM_REG_GPOTP_21        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_22
#define BSP_CFG_ROM_REG_GPOTP_22        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_GPOTP_23
#define BSP_CFG_ROM_REG_GPOTP_23        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_0
#define BSP_CFG_ROM_REG_HOEMRTPK0_0     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_1
#define BSP_CFG_ROM_REG_HOEMRTPK0_1     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_2
#define BSP_CFG_ROM_REG_HOEMRTPK0_2     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_3
#define BSP_CFG_ROM_REG_HOEMRTPK0_3     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_4
#define BSP_CFG_ROM_REG_HOEMRTPK0_4     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_5
#define BSP_CFG_ROM_REG_HOEMRTPK0_5     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_6
#define BSP_CFG_ROM_REG_HOEMRTPK0_6     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_7
#define BSP_CFG_ROM_REG_HOEMRTPK0_7     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_8
#define BSP_CFG_ROM_REG_HOEMRTPK0_8     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_9
#define BSP_CFG_ROM_REG_HOEMRTPK0_9     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_10
#define BSP_CFG_ROM_REG_HOEMRTPK0_10    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK0_11
#define BSP_CFG_ROM_REG_HOEMRTPK0_11    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_0
#define BSP_CFG_ROM_REG_HOEMRTPK1_0     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_1
#define BSP_CFG_ROM_REG_HOEMRTPK1_1     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_2
#define BSP_CFG_ROM_REG_HOEMRTPK1_2     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_3
#define BSP_CFG_ROM_REG_HOEMRTPK1_3     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_4
#define BSP_CFG_ROM_REG_HOEMRTPK1_4     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_5
#define BSP_CFG_ROM_REG_HOEMRTPK1_5     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_6
#define BSP_CFG_ROM_REG_HOEMRTPK1_6     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_7
#define BSP_CFG_ROM_REG_HOEMRTPK1_7     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_8
#define BSP_CFG_ROM_REG_HOEMRTPK1_8     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_9
#define BSP_CFG_ROM_REG_HOEMRTPK1_9     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_10
#define BSP_CFG_ROM_REG_HOEMRTPK1_10    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK1_11
#define BSP_CFG_ROM_REG_HOEMRTPK1_11    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_0
#define BSP_CFG_ROM_REG_HOEMRTPK2_0     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_1
#define BSP_CFG_ROM_REG_HOEMRTPK2_1     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_2
#define BSP_CFG_ROM_REG_HOEMRTPK2_2     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_3
#define BSP_CFG_ROM_REG_HOEMRTPK2_3     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_4
#define BSP_CFG_ROM_REG_HOEMRTPK2_4     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_5
#define BSP_CFG_ROM_REG_HOEMRTPK2_5     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_6
#define BSP_CFG_ROM_REG_HOEMRTPK2_6     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_7
#define BSP_CFG_ROM_REG_HOEMRTPK2_7     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_8
#define BSP_CFG_ROM_REG_HOEMRTPK2_8     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_9
#define BSP_CFG_ROM_REG_HOEMRTPK2_9     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_10
#define BSP_CFG_ROM_REG_HOEMRTPK2_10    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK2_11
#define BSP_CFG_ROM_REG_HOEMRTPK2_11    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_0
#define BSP_CFG_ROM_REG_HOEMRTPK3_0     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_1
#define BSP_CFG_ROM_REG_HOEMRTPK3_1     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_2
#define BSP_CFG_ROM_REG_HOEMRTPK3_2     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_3
#define BSP_CFG_ROM_REG_HOEMRTPK3_3     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_4
#define BSP_CFG_ROM_REG_HOEMRTPK3_4     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_5
#define BSP_CFG_ROM_REG_HOEMRTPK3_5     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_6
#define BSP_CFG_ROM_REG_HOEMRTPK3_6     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_7
#define BSP_CFG_ROM_REG_HOEMRTPK3_7     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_8
#define BSP_CFG_ROM_REG_HOEMRTPK3_8     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_9
#define BSP_CFG_ROM_REG_HOEMRTPK3_9     (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_10
#define BSP_CFG_ROM_REG_HOEMRTPK3_10    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_HOEMRTPK3_11
#define BSP_CFG_ROM_REG_HOEMRTPK3_11    (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_MDE
#define BSP_CFG_ROM_REG_MDE             (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS_0
#define BSP_CFG_ROM_REG_OFSPS_0         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS_1
#define BSP_CFG_ROM_REG_OFSPS_1         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS_2
#define BSP_CFG_ROM_REG_OFSPS_2         (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS2_0
#define BSP_CFG_ROM_REG_OFSPS2_0        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS2_1
#define BSP_CFG_ROM_REG_OFSPS2_1        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_OFSPS2_2
#define BSP_CFG_ROM_REG_OFSPS2_2        (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_REVOKE
#define BSP_CFG_ROM_REG_REVOKE          (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_SAS
#define BSP_CFG_ROM_REG_SAS             (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_UBCSR
#define BSP_CFG_ROM_REG_UBCSR           (0xFFFFFFFF)
#endif

#ifndef BSP_CFG_ROM_REG_ZHUK
#define BSP_CFG_ROM_REG_ZHUK            (0xFFFFFFFF)
#endif

#ifndef BSP_CLOCK_CFG_MAIN_OSC_WAIT
#define BSP_CLOCK_CFG_MAIN_OSC_WAIT (9)
#endif

/* Used to create IELS values for the interrupt initialization table g_interrupt_event_link_select. */
#define BSP_PRV_IELS_ENUM(vector)    (ICU_ ## vector)
#endif /* BSP_MCU_FAMILY_CFG_H_ */

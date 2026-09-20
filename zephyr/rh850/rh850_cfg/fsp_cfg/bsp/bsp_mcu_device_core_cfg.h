/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef BSP_MCU_DEVICE_CORE_CFG_H_
#define BSP_MCU_DEVICE_CORE_CFG_H_

#if defined(CONFIG_SOC_R7F702300AEBBC_C) || defined(CONFIG_SOC_R7F702300BEBBC_C) || defined(CONFIG_SOC_R7F702300EBBG_C) \
    || defined(CONFIG_SOC_R7F702300AEBBB_C)  || defined(CONFIG_SOC_R7F702300BEBBB_C) || defined(CONFIG_SOC_R7F702300EBBB_C) \
    || defined(CONFIG_SOC_R7F702300AFABA_C)  || defined(CONFIG_SOC_R7F702300BFABA_C) || defined(CONFIG_SOC_R7F702300EABA_C)
 #include "bsp_mcu_device_core_cfg_u2a16.h"
#elif defined(CONFIG_SOC_R7F702301AEBBA_C) || defined(CONFIG_SOC_R7F702301BEBBA_C) || defined(CONFIG_SOC_R7F702301EBBA_C) \
    || defined(CONFIG_SOC_R7F702301AFABG_C)  || defined(CONFIG_SOC_R7F702301BFABG_C) || defined(CONFIG_SOC_R7F702301EABG_C)
 #include "bsp_mcu_device_core_cfg_u2a8.h"
#elif defined(CONFIG_SOC_R7F702302FABB_C) || defined(CONFIG_SOC_R7F702302FAFK_C) || defined(CONFIG_SOC_R7F702302FABD_C) \
    || defined(CONFIG_SOC_R7F702302FAFM_C)
 #include "bsp_mcu_device_core_cfg_u2a6.h"
#elif defined(CONFIG_SOC_R7F70255XFABB_C) || defined(CONFIG_SOC_R7F70255XAFABB_C) || defined(CONFIG_SOC_R7F70255XBFABB_C)
 #include "bsp_mcu_device_core_cfg_u2b6.h"
#elif defined(CONFIG_SOC_R7F70254XFABG_C) || defined(CONFIG_SOC_R7F70254XFABA_C) || defined(CONFIG_SOC_R7F70254XFABB_C) \
    || defined(CONFIG_SOC_R7F70254XAFABG_C)  || defined(CONFIG_SOC_R7F70254XAFABA_C) || defined(CONFIG_SOC_R7F70254XAFABB_C)
 #include "bsp_mcu_device_core_cfg_u2b10.h"
#elif defined(CONFIG_SOC_R7F702600AFABB_C)
 #include "bsp_mcu_device_core_cfg_u2c8.h"
#elif defined(CONFIG_SOC_R7F702606AFABB_C) || defined(CONFIG_SOC_R7F702616AFABB_C) || defined(CONFIG_SOC_R7F702613AFABB_C) \
    || defined(CONFIG_SOC_R7F702606AFAFB_C)  || defined(CONFIG_SOC_R7F702606AFAFM_C)  || defined(CONFIG_SOC_R7F702616AFAFM_C) \
    || defined(CONFIG_SOC_R7F702616AFAFB_C) || defined(CONFIG_SOC_R7F702614AFAFM_C) || defined(CONFIG_SOC_R7F702614AFAFB_C) \
    || defined(CONFIG_SOC_R7F702606AFAFD_C)  || defined(CONFIG_SOC_R7F702616AFAFD_C)  || defined(CONFIG_SOC_R7F702613AFAFD_C)
 #include "bsp_mcu_device_core_cfg_u2c4.h"
#elif defined(CONFIG_SOC_R7F702615AFAFM_C) || defined(CONFIG_SOC_R7F702615AFAFB_C) \
    || defined(CONFIG_SOC_R7F702608AFAFD_C)
 #include "bsp_mcu_device_core_cfg_u2c2.h"
#endif

#endif                                 /* BSP_MCU_DEVICE_CORE_CFG_H_ */

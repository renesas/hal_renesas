/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#ifndef R_CGC_H
#define R_CGC_H

/*******************************************************************************************************************//**
 * @addtogroup CGC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_cgc_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define CGC_CLOCK_STABLED    (0x03)
#define CGC_CLOCK_STOPPED    (0x00)

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/* Private enumeration used to tell if a clock is running or not. */
typedef enum e_cgc_prv_clock_state
{
    CGC_PRV_CLOCK_STATE_STOPPED = 0,   ///< The clock source is stopped
    CGC_PRV_CLOCK_STATE_RUNNING = 1,   ///< The clock source is running
} cgc_prv_clock_state_t;

/* Private enumeration used to tell if a clock is stable or not. */
typedef enum e_cgc_prv_clock_stability_state
{
    CGC_PRV_CLOCK_STABILITY_STATE_NOT_STABLE = 0, ///< The clock source is not stable
    CGC_PRV_CLOCK_STABILITY_STATE_STABLE     = 1, ///< The clock source is stable
} cgc_prv_clock_stability_state_t;

#if BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS

/** CGC RLIN3 clock source */
typedef enum e_cgc_rlin_clk_sel
{
    CGC_RLIN_CLK_SEL_EMG        = 0,   ///< The RLIN3 input clock source is EMG
    CGC_RLIN_CLK_SEL_HSB        = 1,   ///< The RLIN3 input clock source is P080
    CGC_RLIN_CLK_SEL_HBUS       = 1,   ///< The RLIN23 input clock source is P080
    CGC_RLIN_CLK_SEL_MOSC       = 2,   ///< The RLIN3 input clock source is Main OSC
    CGC_RLIN_CLK_SEL_MOSC_DIV_4 = 3,   ///< The RLIN3 input clock source is CLK_MOSC/4
    CGC_RLIN_CLK_SEL_MOSC_DIV_8 = 4    ///< The RLIN3 input clock source is CLK_MOSC/8
} cgc_rlin_clk_sel_t;

/** CGC TAUJ clock source */
typedef enum e_cgc_tauj_clk_sel
{
    CGC_TAUJ_CLK_SEL_EMG           = 0, ///< The TAUJ input clock source is EMG
    CGC_TAUJ_CLK_SEL_LSIOSC        = 1, ///< The TAUJ input clock source is CLK_MOSC/8
    CGC_TAUJ_CLK_SEL_HSIOSC_DIV_20 = 2, ///< The TAUJ input clock source is CLK_MOSC/4
    CGC_TAUJ_CLK_SEL_MOSC          = 3, ///< The TAUJ input clock source is Main OSC
    CGC_TAUJ_CLK_SEL_HSB           = 4  ///< The TAUJ input clock source is CLK_MOSC/8
} cgc_tauj_clk_sel_t;

/** CGC RTCA clock source */
typedef enum e_cgc_rtca_clk_sel
{
    CGC_RTCA_CLK_SEL_EMG         = 0,  ///< The RTCA input clock source is EMG
    CGC_RTCA_CLK_SEL_LSIOSC      = 1,  ///< The RTCA input clock source is CLK_LSIOSC
    CGC_RTCA_CLK_SEL_MOSC_DIV_16 = 2,  ///< The RTCA input clock source is CLK_MOSC/16
    CGC_RTCA_CLK_SEL_SOSC        = 3   ///< The RTCA input clock source is Sub OSC
} cgc_rtca_clk_sel_t;

/** CGC AADC clock source */
typedef enum e_cgc_aadc_clk_sel
{
    CGC_AADC_CLK_SEL_EMG           = 0, ///< The AADC input clock source is EMG
    CGC_AADC_CLK_SEL_HSIOSC_DIV_20 = 1, ///< The AADC input clock source is CLK_HSIOSC/20
    CGC_AADC_CLK_SEL_MOSC          = 2, ///< The AADC input clock source is Main OSC
    CGC_AADC_CLK_SEL_CLKC_LSB      = 3  ///< The AADC input clock source is CLK_MOSC/8
} cgc_aadc_clk_sel_t;

/** CGC MSPI clock source */
typedef enum e_cgc_mspi_clk_sel
{
    CGC_MSPI_CLK_SEL_EMG  = 0,         ///< The MSPI input clock source is EMG
    CGC_MSPI_CLK_SEL_MOSC = 1,         ///< The MSPI input clock source is Main OSC
    CGC_MSPI_CLK_SEL_HSB  = 2          ///< The MSPI input clock source is CLK_MOSC/4
} cgc_mspi_clk_sel_t;

/** CGC EXTCLK0 clock source */
typedef enum e_cgc_extclk_clk_sel
{
    CGC_EXTCLK_CLK_SEL_MOSC          = 0, ///< The EXTCLK0 input clock source is Main OSC
    CGC_EXTCLK_CLK_SEL_LSIOSC        = 1, ///< The EXTCLK0 input clock source is LSIOSC
    CGC_EXTCLK_CLK_SEL_HSIOSC_DIV_20 = 2, ///< The EXTCLK0 input clock source is HSIOSC/20
 #if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)
    CGC_EXTCLK_CLK_SEL_SOSC = 3,          ///< The EXTCLK0 input clock source is CLK_HSB
    CGC_EXTCLK_CLK_SEL_HSB  = 4,          ///< The EXTCLK0 input clock source is CLK_HSB
    CGC_EXTCLK_CLK_SEL_CHSB = 5           ///< The EXTCLK0 input clock source is CLKC_HSB
 #else
    CGC_EXTCLK_CLK_SEL_HSB  = 3,          ///< The EXTCLK0 input clock source is CLK_HSB
    CGC_EXTCLK_CLK_SEL_CHSB = 4           ///< The EXTCLK0 input clock source is CLKC_HSB
 #endif
} cgc_extclk_clk_sel_t;

#else

/** CGC RLIN3 clock source */
typedef enum e_cgc_rlin_clk_sel
{
    CGC_RLIN_CLK_SEL_MOSC       = 0,   ///< The RLIN3 input clock source is Main OSC
    CGC_RLIN_CLK_SEL_HSB        = 1,   ///< The RLIN3 input clock source is P080
    CGC_RLIN_CLK_SEL_MOSC_DIV_4 = 2,   ///< The RLIN3 input clock source is CLK_MOSC/4
    CGC_RLIN_CLK_SEL_MOSC_DIV_8 = 3    ///< The RLIN3 input clock source is CLK_MOSC/8
} cgc_rlin_clk_sel_t;

/** CGC TAUJ clock source */
typedef enum e_cgc_tauj_clk_sel
{
    CGC_TAUJ_CLK_SEL_LSIOSC        = 0, ///< The TAUJ input clock source is EMG
    CGC_TAUJ_CLK_SEL_HSIOSC_DIV_20 = 1, ///< The TAUJ input clock source is CLK_MOSC/8
    CGC_TAUJ_CLK_SEL_MOSC          = 2, ///< The TAUJ input clock source is CLK_MOSC/4
    CGC_TAUJ_CLK_SEL_HSB           = 3  ///< The TAUJ input clock source is CLK_MOSC/8
} cgc_tauj_clk_sel_t;

/** CGC MSPI clock source */
typedef enum e_cgc_mspi_clk_sel
{
    CGC_MSPI_CLK_SEL_MOSC = 0,         ///< The MSPI input clock source is Main OSC
    CGC_MSPI_CLK_SEL_HSB  = 1          ///< The MSPI input clock source is CLK_MOSC/4
} cgc_mspi_clk_sel_t;

/** CGC RTCA clock source */
typedef enum e_cgc_rtca_clk_sel
{
    CGC_RTCA_CLK_SEL_MOSC_DIV_16 = 0,  ///< The RTCA input clock source is CLK_MOSC/16
    CGC_RTCA_CLK_SEL_LSIOSC      = 1,  ///< The RTCA input clock source is CLK_LSIOSC
} cgc_rtca_clk_sel_t;

/** CGC AADC clock source */
typedef enum e_cgc_aadc_clk_sel
{
    CGC_AADC_CLK_SEL_MOSC          = 0, ///< The AADC input clock source is EMG
    CGC_AADC_CLK_SEL_HSIOSC_DIV_20 = 1, ///< The AADC input clock source is CLK_HSIOSC/20
    CGC_AADC_CLK_SEL_CLKC_LSB      = 2  ///< The AADC input clock source is CLK_MOSC/8
} cgc_aadc_clk_sel_t;

/** CGC EXTCLK clock source */
typedef enum e_cgc_extclk_clk_sel
{
    CGC_FOUT_CLK_SEL_MOSC          = 0, ///< The FOUT0 input clock source is Main OSC
    CGC_FOUT_CLK_SEL_HSB           = 1, ///< The FOUT0 input clock source is CLK_HSB
    CGC_FOUT_CLK_SEL_LSIOSC        = 3, ///< The FOUT0 input clock source is LSIOSC
    CGC_FOUT_CLK_SEL_HSIOSC_DIV_20 = 4, ///< The FOUT0 input clock source is HSIOSC/20
    CGC_FOUT_CLK_SEL_MOSC_1        = 6, ///< The FOUT0 input clock source is Main OSC
    CGC_FOUT_CLK_SEL_MOSC_2        = 7, ///< The FOUT0 input clock source is Main OSC
} cgc_extclk_clk_sel_t;

#endif

/** CGC CANFD clock source */
typedef enum e_cgc_canfd_clk_sel
{
    CGC_CANFD_CLK_SEL_EMG        = 0,  ///< The CANFD input clock source is EMG
    CGC_CANFD_CLK_SEL_MOSC       = 1,  ///< The CANFD input clock source is Main OSC
    CGC_CANFD_CLK_SEL_MOSC_DIV_2 = 2,  ///< The CANFD input clock source is CLK_MOSC/2
    CGC_CANFD_CLK_SEL_MOSC_DIV_4 = 3   ///< The CANFD input clock source is CLK_MOSC/4
} cgc_canfd_clk_sel_t;

/* CGC private system clock source */
typedef enum e_cgc_prv_sys_clk_src
{
    CGC_PRV_SYS_CLK_SRC_PLLO = 0,      ///< The SYS_CLEAN/SSCG/SSCG1 clock source is PLLO/SSCG/SSCG1
    CGC_PRV_SYS_CLK_SRC_IOSC = 1       ///< The SYS_CLEAN/SSCG/SSCG1 clock source is IOSC
} cgc_prv_sys_clk_src_t;

/* CGC private system clock name */
typedef enum e_cgc_prv_system_clock
{
    CGC_PRV_SYSTEM_CLOCK_SYS       = 0, ///< The CPU System clock is CLK_SYSSYS
    CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN = 0, ///< The CPU System clock is CLK_SYS_CLEAN
    CGC_PRV_SYSTEM_CLOCK_SYS_SSCG  = 1, ///< The CPU System clock is CLK_SYS_SSCG
    CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1 = 2, ///< The CPU System clock is CLK_SYS_SSCG1
} cgc_prv_system_clock_t;

/** CGC private system clock information */
typedef struct st_cgc_sys_clock_info
{
    volatile uint32_t * p_ckd_ctrl;                   ///< Divider control register
    volatile uint32_t * p_ckd_stat;                   ///< Divider status register
    volatile uint32_t * p_cks_ctrl;                   ///< Clock source control register
    volatile uint32_t * p_cks_stat;                   ///< Clock source status register
    cgc_prv_sys_clk_src_t (* get_clock_source)(void); ///< Function to read current clock source
} cgc_sys_clock_info_t;

/** CGC private peripheral input clock source information */
typedef struct st_cgc_peripheral_clock_backup
{
    cgc_rlin_clk_sel_t   clk_rlin;      ///< Backup of RLIN3 clock source
    cgc_rlin_clk_sel_t   clk_rlin_ch23; ///< Backup of RLIN3_CH23 clock source
    cgc_tauj_clk_sel_t   clk_tauj;      ///< Backup of TAUJ clock source
    cgc_rtca_clk_sel_t   clk_rtca;      ///< Backup of RTCA clock source
    cgc_aadc_clk_sel_t   clk_aadc;      ///< Backup of AADC clock source
    cgc_mspi_clk_sel_t   clk_mspi;      ///< Backup of MSPI clock source
    cgc_extclk_clk_sel_t clk_extclk0;   ///< Backup of EXTCLK0 clock source
    cgc_extclk_clk_sel_t clk_extclk1;   ///< Backup of EXTCLK1 clock source
    cgc_canfd_clk_sel_t  clk_canfd;     ///< Backup of CANFD clock source
} cgc_peripheral_clock_backup_t;

/** CGC private control block. DO NOT MODIFY. Initialization occurs when R_CGC_Open() is called. */
typedef struct st_cgc_instance_ctrl
{
    uint32_t             open;                                                ///< Open status of CGC
    cgc_sys_clock_info_t cgc_sys_clocks[BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK]; ///< System clock information
#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)
    cgc_peripheral_clock_backup_t cgc_peripheral_clocks[2];                   ///< Peripheral clock source backup
#else
    cgc_peripheral_clock_backup_t cgc_peripheral_clocks[1];                   ///< Peripheral clock source backup
#endif
} cgc_instance_ctrl_t;

/**********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/** @cond INC_HEADER_DEFS_SEC */
/** Filled in Interface API structure for this Instance. */
extern const cgc_api_t g_cgc_on_cgc;

/** @endcond */

/***********************************************************************************************************************
 * Public APIs
 **********************************************************************************************************************/
fsp_err_t R_CGC_Open(cgc_ctrl_t * const p_ctrl, cgc_cfg_t const * const p_cfg);
fsp_err_t R_CGC_ClocksCfg(cgc_ctrl_t * const p_ctrl, cgc_clocks_cfg_t const * const p_clock_cfg);
fsp_err_t R_CGC_ClockStart(cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source, cgc_pll_cfg_t const * const p_pll_cfg);
fsp_err_t R_CGC_ClockStop(cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source);
fsp_err_t R_CGC_ClockCheck(cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source);
fsp_err_t R_CGC_SystemClockSet(cgc_ctrl_t * const              p_ctrl,
                               cgc_clock_t                     clock_source,
                               cgc_divider_cfg_t const * const p_divider_cfg);
fsp_err_t R_CGC_SystemClockGearSet(cgc_ctrl_t * const p_ctrl, cgc_system_clock_cfg_t const * const p_system_clock_cfg);
fsp_err_t R_CGC_SystemClockGet(cgc_ctrl_t * const        p_ctrl,
                               cgc_clock_t * const       p_clock_source,
                               cgc_divider_cfg_t * const p_divider_cfg);
fsp_err_t R_CGC_OscStopDetectEnable(cgc_ctrl_t * const p_ctrl);
fsp_err_t R_CGC_OscStopDetectDisable(cgc_ctrl_t * const p_ctrl);
fsp_err_t R_CGC_OscStopStatusClear(cgc_ctrl_t * const p_ctrl);
fsp_err_t R_CGC_CallbackSet(cgc_ctrl_t * const          p_api_ctrl,
                            void (                    * p_callback)(cgc_callback_args_t * p_args),
                            void * const                p_context,
                            cgc_callback_args_t * const p_callback_memory);
fsp_err_t R_CGC_Close(cgc_ctrl_t * const p_ctrl);

/*******************************************************************************************************************//**
 * @} (end defgroup CGC)
 **********************************************************************************************************************/

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /*  R_CGC_H */

/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_cgc.h"
#include "r_cgc_cfg.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* "CGC" in ASCII, used to determine if the module is open. */
#define CGC_OPEN                                   (0x00434743U)

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)
 #define CGC_CLOCK_NO_DIVISION                     (0x01U)
 #define CGC_CLOCK_DIVIDED_BY_2                    (0x02U)
#else
 #define CGC_CLOCK_DIVIDED_BY_2_DIV_16             (0x02U)
 #define CGC_CLOCK_DIVIDED_BY_4_DIV_16             (0x04U)
 #define CGC_CLOCK_DIVIDED_BY_5_DIV_16             (0x05U)
 #define CGC_CLOCK_DIVIDED_BY_6_DIV_16             (0x06U)
 #define CGC_CLOCK_DIVIDED_BY_7_DIV_16             (0x07U)
 #define CGC_CLOCK_DIVIDED_BY_8_DIV_16             (0x08U)
 #define CGC_CLOCK_DIVIDED_BY_9_DIV_16             (0x09U)
 #define CGC_CLOCK_DIVIDED_BY_10_DIV_16            (0x0AU)
 #define CGC_CLOCK_DIVIDED_BY_11_DIV_16            (0x0BU)
 #define CGC_CLOCK_DIVIDED_BY_12_DIV_16            (0x0CU)
 #define CGC_CLOCK_DIVIDED_BY_13_DIV_16            (0x0DU)
 #define CGC_CLOCK_DIVIDED_BY_14_DIV_16            (0x0EU)
 #define CGC_CLOCK_DIVIDED_BY_15_DIV_16            (0x0FU)
 #define CGC_CLOCK_NO_DIVISION                     (0x0000U)
#endif

#define CGC_PERIPHERAL_CLK_BEFORE_MAIN_OSC_STOP    (0U)
#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)
 #define CGC_PERIPHERAL_CLK_BEFORE_SUB_OSC_STOP    (1U)
#endif

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
#if (2U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

/* Array of clock division value used for gear up configuration */
static const uint32_t cgc_gear_up_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_NO_DIVISION
};

/* Array of clock division value used for gear down configuration */
static const uint32_t cgc_gear_down_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_6_DIV_16,
};
#endif

#if (5U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

/* Array of clock division value used for gear up configuration */
static const uint32_t cgc_800_gear_up_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_6_DIV_16,
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_NO_DIVISION
};
static const uint32_t cgc_640_gear_up_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_7_DIV_16,
    CGC_CLOCK_DIVIDED_BY_9_DIV_16,
    CGC_CLOCK_DIVIDED_BY_11_DIV_16,
    CGC_CLOCK_DIVIDED_BY_13_DIV_16,
    CGC_CLOCK_DIVIDED_BY_15_DIV_16,
    CGC_CLOCK_NO_DIVISION
};
static const uint32_t cgc_480_gear_up_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_9_DIV_16,
    CGC_CLOCK_DIVIDED_BY_11_DIV_16,
    CGC_CLOCK_DIVIDED_BY_13_DIV_16,
    CGC_CLOCK_DIVIDED_BY_15_DIV_16,
    CGC_CLOCK_NO_DIVISION
};
static const uint32_t cgc_320_gear_up_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_NO_DIVISION
};

/* Array of clock division value used for gear down configuration */
static const uint32_t cgc_800_gear_down_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_6_DIV_16,
    CGC_CLOCK_DIVIDED_BY_4_DIV_16,
};
static const uint32_t cgc_640_gear_down_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_6_DIV_16,
    CGC_CLOCK_DIVIDED_BY_5_DIV_16,
};
static const uint32_t cgc_480_gear_down_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_DIVIDED_BY_12_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
    CGC_CLOCK_DIVIDED_BY_8_DIV_16,
    CGC_CLOCK_DIVIDED_BY_7_DIV_16,
};
static const uint32_t cgc_320_gear_down_repetitions_arr[] =
{
    CGC_CLOCK_DIVIDED_BY_14_DIV_16,
    CGC_CLOCK_DIVIDED_BY_10_DIV_16,
};
#endif

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static fsp_err_t                       r_cgc_clock_check(cgc_clock_t const clock_source);
static cgc_prv_clock_stability_state_t r_cgc_stabilization_check(cgc_clock_t clock);
static cgc_prv_clock_state_t           r_cgc_clock_run_state_get(cgc_clock_t clock);

static void r_cgc_gear_down(cgc_sys_clock_info_t * p_system_clock, uint32_t clock_hz);
static void r_cgc_gear_up(cgc_sys_clock_info_t * p_system_clock, uint32_t clock_hz);

static fsp_err_t r_cgc_sys_clk_shift(cgc_sys_clock_info_t      * p_system_clock,
                                     cgc_prv_system_clock_t      system_clock,
                                     cgc_system_clock_behavior_t shift_behavior);

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)
static void      r_cgc_sub_osc_restore(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);
static fsp_err_t r_cgc_sub_osc_start(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);
static void      r_cgc_sub_osc_pre_stop(void);
static fsp_err_t r_cgc_sub_osc_stop(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);

#endif

static void      r_cgc_main_osc_restore(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);
static fsp_err_t r_cgc_main_osc_start(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);
static void      r_cgc_main_osc_pre_stop(void);
static fsp_err_t r_cgc_main_osc_stop(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);

static void r_cgc_peripheral_clock_backup(cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)
static cgc_prv_sys_clk_src_t r_cgc_sys_clocksource_get(void);

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)
static cgc_prv_sys_clk_src_t r_cgc_sysclean_clocksource_get(void);

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
static cgc_prv_sys_clk_src_t r_cgc_syssscg_clocksource_get(void);

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)
static cgc_prv_sys_clk_src_t r_cgc_syssscg1_clocksource_get(void);

#endif

static fsp_err_t r_cgc_pll_clock_change(cgc_clock_change_t state);

static fsp_err_t r_cgc_clock_change(cgc_clock_t                     clock_source,
                                    cgc_clock_change_t              operation,
                                    cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock);

static void r_cgc_disable_protection(void);
static void r_cgc_enable_protection(void);

#if CGC_CFG_PARAM_CHECKING_ENABLE
static fsp_err_t r_cgc_common_parameter_checking(cgc_instance_ctrl_t * p_instance_ctrl);

#endif

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/

const cgc_api_t g_cgc_on_cgc =
{
    .open                 = R_CGC_Open,
    .clocksCfg            = R_CGC_ClocksCfg,
    .clockStart           = R_CGC_ClockStart,
    .clockStop            = R_CGC_ClockStop,
    .clockCheck           = R_CGC_ClockCheck,
    .systemClockSet       = R_CGC_SystemClockSet,
    .systemClockGearSet   = R_CGC_SystemClockGearSet,
    .systemClockGet       = R_CGC_SystemClockGet,
    .oscStopDetectEnable  = R_CGC_OscStopDetectEnable,
    .oscStopDetectDisable = R_CGC_OscStopDetectDisable,
    .oscStopStatusClear   = R_CGC_OscStopStatusClear,
    .callbackSet          = R_CGC_CallbackSet,
    .close                = R_CGC_Close,
};

/*******************************************************************************************************************//**
 * @addtogroup CGC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Initialize the CGC API.  Implements @ref cgc_api_t::open.
 *
 * @retval FSP_SUCCESS              CGC successfully initialized.
 * @retval FSP_ERR_ASSERTION        Invalid input argument.
 * @retval FSP_ERR_ALREADY_OPEN     Module is already open.
 **********************************************************************************************************************/
fsp_err_t R_CGC_Open (cgc_ctrl_t * const p_ctrl, cgc_cfg_t const * const p_cfg)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;

#if CGC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ASSERT(NULL != p_cfg);

    /* Verify this unit has already been opened   */
    FSP_ERROR_RETURN(CGC_OPEN != p_instance_ctrl->open, FSP_ERR_ALREADY_OPEN);
#else
    FSP_PARAMETER_NOT_USED(p_cfg);
#endif                                 /* CGC_CFG_PARAM_CHECKING_ENABLE */

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)

    /* System clock SYS information */
    p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS] = (cgc_sys_clock_info_t)
    {
        .p_ckd_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_PLLC,
        .p_ckd_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_PLLS,
        .p_cks_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKSC_CPUC,
        .p_cks_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKSC_CPUS,
        .get_clock_source = r_cgc_sys_clocksource_get,
    };
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)

    /* System clock CLEAN information */
    p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN] = (cgc_sys_clock_info_t)
    {
        .p_ckd_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_PLLC,
        .p_ckd_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_PLLS,
        .p_cks_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_CLEANC,
        .p_cks_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_CLEANS,
        .get_clock_source = r_cgc_sysclean_clocksource_get,
    };
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)

    /* System clock SSCG0 information */
    p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG] = (cgc_sys_clock_info_t)
    {
        .p_ckd_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_SSCGC,
        .p_ckd_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_SSCGS,
        .p_cks_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_SSCGC,
        .p_cks_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_SSCGS,
        .get_clock_source = r_cgc_syssscg_clocksource_get,
    };
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)

    /* System clock SSCG1 information */
    p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1] = (cgc_sys_clock_info_t)
    {
        .p_ckd_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_SSCG1C,
        .p_ckd_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKD_SSCG1S,
        .p_cks_ctrl       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_SSCG1C,
        .p_cks_stat       = (volatile uint32_t *) &R_SYS0_CLOCK->CKS_SSCG1S,
        .get_clock_source = r_cgc_syssscg1_clocksource_get,
    };
#endif

    r_cgc_peripheral_clock_backup(&p_instance_ctrl->cgc_peripheral_clocks[CGC_PERIPHERAL_CLK_BEFORE_MAIN_OSC_STOP]);
#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)
    r_cgc_peripheral_clock_backup(&p_instance_ctrl->cgc_peripheral_clocks[CGC_PERIPHERAL_CLK_BEFORE_SUB_OSC_STOP]);
#endif

    /* Mark the module as open so other APIs can be used. */
    p_instance_ctrl->open = CGC_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Reconfigures all main oscillator clocks. This API can be used for start or stop oscillator clocks
 *
 * Configure clock oscillators (Main OSC, Sub OSC, PLL). This function blocks waiting for the stabilization flag of the
 * requested clock source to be set. If the PLL is started while Main OSC is stopped, an error will be returned.
 * Do not disable Main OSC and PLL while CLK_PLL is selected for CLK_SYS.
 *
 * Implements @ref cgc_api_t::clocksCfg.
 *
 * Do not attempt to stop the requested clock source or the source of a PLL if the PLL will be running after this
 * operation completes.
 *
 * @retval FSP_SUCCESS                      Clock configuration applied successfully
 * @retval FSP_ERR_ASSERTION                Invalid input argument
 * @retval FSP_ERR_NOT_OPEN                 Module is not open
 * @retval FSP_ERR_CGC_CLOCK_IN_USE         Clock already running or cannot be stopped
 * @retval FSP_ERR_CGC_CLOCK_INACTIVE       The PLL clock source is not started
 * @retval FSP_ERR_CGC_NOT_STABILIZED       Clock failed to stabilize
 * @retval FSP_ERR_CGC_INVALID_CLOCK_SOURCE Invalid clock source
 **********************************************************************************************************************/
fsp_err_t R_CGC_ClocksCfg (cgc_ctrl_t * const p_ctrl, cgc_clocks_cfg_t const * const p_clock_cfg)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;

#if CGC_CFG_PARAM_CHECKING_ENABLE
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_clock_cfg);
#endif

    /* Set the PLL clock oscillator as configuration */
#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
    err = r_cgc_clock_change(CGC_CLOCK_PLL, p_clock_cfg->pll_sscg_state, p_instance_ctrl->cgc_peripheral_clocks);
#else
    err = r_cgc_clock_change(CGC_CLOCK_PLL, p_clock_cfg->pll_state, p_instance_ctrl->cgc_peripheral_clocks);
#endif

    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Set the Main OSC clock oscillator as configuration */
    err = r_cgc_clock_change(CGC_CLOCK_MAIN_OSC, p_clock_cfg->mainosc_state, p_instance_ctrl->cgc_peripheral_clocks);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)

    /* Set the Sub OSC clock oscillator as configuration */
    err = r_cgc_clock_change(CGC_CLOCK_SUBCLOCK, p_clock_cfg->subosc_state, p_instance_ctrl->cgc_peripheral_clocks);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Start the specified clock if it is not currently active. The PLL configuration cannot be changed while the PLL
 * is running.
 *
 * Implements @ref cgc_api_t::clockStart.
 *
 * The PLL source clock must be operating and stable prior to starting the PLL.
 *
 * @retval FSP_SUCCESS                      Clock initialized successfully.
 * @retval FSP_ERR_ASSERTION                Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN                 Module is not open.
 * @retval FSP_ERR_CGC_NOT_STABILIZED       The clock source is not stabilized
 * @retval FSP_ERR_CGC_INVALID_CLOCK_SOURCE Invalid clock source.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE         The clock source has been active.
 **********************************************************************************************************************/
fsp_err_t R_CGC_ClockStart (cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source, cgc_pll_cfg_t const * const p_pll_cfg)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;

#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Ensure selected clock support start operation */
 #if BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE
    FSP_ERROR_RETURN(((CGC_CLOCK_HOCO != clock_source) && (CGC_CLOCK_LOCO != clock_source) &&
                      (CGC_CLOCK_HVIO != clock_source)),
                     FSP_ERR_CGC_CLOCK_IN_USE);
 #else
    FSP_ERROR_RETURN(((CGC_CLOCK_HOCO != clock_source) && (CGC_CLOCK_LOCO != clock_source)), FSP_ERR_CGC_CLOCK_IN_USE);
 #endif
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
    FSP_PARAMETER_NOT_USED(p_pll_cfg);
#endif                                 /* CGC_CFG_PARAM_CHECKING_ENABLE */

    cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock = p_instance_ctrl->cgc_peripheral_clocks;
    err = r_cgc_clock_change(clock_source, CGC_CLOCK_CHANGE_START, p_cgc_peripheral_clock);

    return err;
}

/*******************************************************************************************************************//**
 * Stop the specified clock if it is active.
 *
 * Implements @ref cgc_api_t::clockStop.
 *
 * The main OSC and PLL cannot be disabled while system clock is selected PLL for input source.
 *
 * @retval FSP_SUCCESS                          Clock stopped successfully.
 * @retval FSP_ERR_ASSERTION                    Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN                     Module is not open.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE             Attempt to stop the HOCO or LOCO
 * @retval FSP_ERR_CGC_INVALID_CLOCK_SOURCE     Invalid clock source
 * @retval FSP_ERR_CGC_NOT_STABILIZED           Clock not stabilized after starting.
 **********************************************************************************************************************/
fsp_err_t R_CGC_ClockStop (cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;
#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    /* Ensure the selected clock support stop operation */
 #if BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE
    FSP_ERROR_RETURN(((CGC_CLOCK_HOCO != clock_source) && (CGC_CLOCK_LOCO != clock_source) &&
                      (CGC_CLOCK_HVIO != clock_source)),
                     FSP_ERR_CGC_CLOCK_IN_USE);
 #else
    FSP_ERROR_RETURN(((CGC_CLOCK_HOCO != clock_source) && (CGC_CLOCK_LOCO != clock_source)), FSP_ERR_CGC_CLOCK_IN_USE);
 #endif
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif                                 /* CGC_CFG_PARAM_CHECKING_ENABLE */

    cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock = p_instance_ctrl->cgc_peripheral_clocks;
    err = r_cgc_clock_change(clock_source, CGC_CLOCK_CHANGE_STOP, p_cgc_peripheral_clock);

    return err;
}

/*******************************************************************************************************************//**
 * @ref cgc_api_t::systemClockSet is not supported on the RH850 U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CGC_SystemClockSet (cgc_ctrl_t * const              p_ctrl,
                                cgc_clock_t                     clock_source,
                                cgc_divider_cfg_t const * const p_divider_cfg)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(clock_source);
    FSP_PARAMETER_NOT_USED(p_divider_cfg);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief This function configures the system clock operation for the applicable system clock domains.
 * Implements @ref cgc_api_t::systemClockGearSet.
 *
 * The system clock gear operation (gear up, gear down, or unchanged) is performed
 * according to the configuration specified for each applicable system clock domain.
 *
 * @retval FSP_SUCCESS                              Operation performed successfully.
 * @retval FSP_ERR_ASSERTION                        Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN                         Module is not open.
 * @retval FSP_ERR_CGC_CLOCK_INACTIVE               The specified clock source is inactive.
 * @retval FSP_ERR_CGC_NOT_STABILIZED               The clock source has not stabilized
 * @retval FSP_ERR_CGC_INVALID_DIVIDER_GEAR_DOWN    Invalid division ratio of clock source
 * @retval FSP_ERR_CGC_INVALID_GEAR_OPERATION       The operation is invalid in the current clock state
 **********************************************************************************************************************/
fsp_err_t R_CGC_SystemClockGearSet (cgc_ctrl_t * const p_ctrl, cgc_system_clock_cfg_t const * const p_system_clock_cfg)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;

#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_system_clock_cfg);
#endif

    /* Disable register protection */
    r_cgc_disable_protection();

    cgc_sys_clock_info_t * p_sysclock_info;

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)

    /* Configure the system clock gear (up/down/unchanged) according to the setting. */
    p_sysclock_info = &p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS];
    err             =
        r_cgc_sys_clk_shift(p_sysclock_info, CGC_PRV_SYSTEM_CLOCK_SYS, p_system_clock_cfg->sys_clock_gear);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)

    /* Configure the system clock CLEAN gear (up/down/unchanged) according to the setting. */
    p_sysclock_info = &p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN];
    err             = r_cgc_sys_clk_shift(p_sysclock_info,
                                          CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN,
                                          p_system_clock_cfg->clean_clock_gear);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)

    /* Configure the system clock SSCG gear (up/down/unchanged) according to the setting. */
    p_sysclock_info = &p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG];
    err             = r_cgc_sys_clk_shift(p_sysclock_info,
                                          CGC_PRV_SYSTEM_CLOCK_SYS_SSCG,
                                          p_system_clock_cfg->sscg_clock_gear);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)

    /* Configure the system clock SSCG1 gear (up/down/unchanged) according to the setting. */
    p_sysclock_info = &p_instance_ctrl->cgc_sys_clocks[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1];
    err             = r_cgc_sys_clk_shift(p_sysclock_info,
                                          CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1,
                                          p_system_clock_cfg->sscg1_clock_gear);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    /* Enable register protection */
    r_cgc_enable_protection();

    return err;
}

/*******************************************************************************************************************//**
 * Return the current system clock source.  Implements @ref cgc_api_t::systemClockGet.
 *
 * @retval FSP_SUCCESS                 Parameters returned successfully.
 * @retval FSP_ERR_ASSERTION           Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN            Module is not open.
 **********************************************************************************************************************/
fsp_err_t R_CGC_SystemClockGet (cgc_ctrl_t * const        p_ctrl,
                                cgc_clock_t * const       p_clock_source,
                                cgc_divider_cfg_t * const p_divider_cfg)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;

#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
    FSP_ASSERT(NULL != p_clock_source);
#endif
    FSP_PARAMETER_NOT_USED(p_divider_cfg);

    cgc_prv_sys_clk_src_t sys_clock_source;
    for (size_t i = 0; i < BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK; i++)
    {
        /* System clock information */
        cgc_sys_clock_info_t * p_sys_clock_infor = &p_instance_ctrl->cgc_sys_clocks[i];

        /* Get the current system clock source */
        sys_clock_source = p_sys_clock_infor->get_clock_source();

        /* The clock source if the input is PLL/SSCG/SSCG1 */
        if (CGC_PRV_SYS_CLK_SRC_PLLO == sys_clock_source)
        {
            p_clock_source[i] = CGC_CLOCK_MAIN_OSC;
        }
        /* The clock source if the input is IOSC, its default source is HOCO. */
        else
        {
            p_clock_source[i] = CGC_CLOCK_HOCO;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * Check the specified clock for stability.  Implements @ref cgc_api_t::clockCheck.
 *
 * @retval FSP_SUCCESS                 Clock is running and stable.
 * @retval FSP_ERR_ASSERTION           Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN            Module is not open.
 * @retval FSP_ERR_CGC_NOT_STABILIZED  Clock not stabilized.
 * @retval FSP_ERR_CGC_CLOCK_INACTIVE  Clock not turned on.
 **********************************************************************************************************************/
fsp_err_t R_CGC_ClockCheck (cgc_ctrl_t * const p_ctrl, cgc_clock_t clock_source)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;
    fsp_err_t             err             = FSP_SUCCESS;
#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#else
    FSP_PARAMETER_NOT_USED(p_instance_ctrl);
#endif

    /* Ensure the clock is running and stable. */
    err = r_cgc_clock_check(clock_source);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref cgc_api_t::oscStopDetectEnable is not supported on the RH850 U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CGC_OscStopDetectEnable (cgc_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref cgc_api_t::oscStopDetectDisable is not supported on the RH850 U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CGC_OscStopDetectDisable (cgc_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref cgc_api_t::oscStopStatusClear is not supported on the RH850 U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CGC_OscStopStatusClear (cgc_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref cgc_api_t::callbackSet is not supported on the RH850 U2x.
 *
 * @retval FSP_ERR_UNSUPPORTED
 **********************************************************************************************************************/
fsp_err_t R_CGC_CallbackSet (cgc_ctrl_t * const          p_api_ctrl,
                             void (                    * p_callback)(cgc_callback_args_t * p_args),
                             void * const                p_context,
                             cgc_callback_args_t * const p_callback_memory)
{
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
    FSP_PARAMETER_NOT_USED(p_callback);
    FSP_PARAMETER_NOT_USED(p_context);
    FSP_PARAMETER_NOT_USED(p_callback_memory);

    return FSP_ERR_UNSUPPORTED;
}

/******************************************************************************************************************//**
 * Closes the CGC module.  Implements @ref cgc_api_t::close.
 *
 * @retval FSP_SUCCESS                 The module is successfully closed.
 * @retval FSP_ERR_ASSERTION           Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN            Module is not open.
 **********************************************************************************************************************/
fsp_err_t R_CGC_Close (cgc_ctrl_t * const p_ctrl)
{
    cgc_instance_ctrl_t * p_instance_ctrl = (cgc_instance_ctrl_t *) p_ctrl;

#if CGC_CFG_PARAM_CHECKING_ENABLE

    /* Verify p_instance_ctrl is not NULL and the module is open. */
    fsp_err_t err = r_cgc_common_parameter_checking(p_instance_ctrl);
    FSP_ERROR_RETURN(FSP_SUCCESS == err, err);
#endif

    p_instance_ctrl->open = 0U;

    /* All done, return success. */
    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup CGC)
 **********************************************************************************************************************/

#if CGC_CFG_PARAM_CHECKING_ENABLE

/*******************************************************************************************************************//**
 * Verifies the control structure is not NULL and the module is open. This reduces code size when the error
 * logger is used.
 *
 * @param[in]  p_instance_ctrl          Pointer to instance control structure.
 *
 * @retval FSP_SUCCESS                  No error detected.
 * @retval FSP_ERR_ASSERTION            Invalid input argument.
 * @retval FSP_ERR_NOT_OPEN             Module is not open.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_common_parameter_checking (cgc_instance_ctrl_t * p_instance_ctrl)
{
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(CGC_OPEN == p_instance_ctrl->open, FSP_ERR_NOT_OPEN);

    return FSP_SUCCESS;
}

#endif

/*******************************************************************************************************************//**
 * @brief Check if the specified clock is running and stable
 *
 * @param[in]   clock_source               Clock to be verified
 *
 * @retval FSP_SUCCESS                     Clock is running and stable.
 * @retval FSP_ERR_CGC_NOT_STABILIZED      Clock not stabilized.
 * @retval FSP_ERR_CGC_CLOCK_INACTIVE      Clock not turned on.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_clock_check (cgc_clock_t const clock_source)
{
    /* Get the current run state of the specified clock source */
    cgc_prv_clock_state_t running_state = r_cgc_clock_run_state_get(clock_source);

    /* Return error if the clock is not running */
    FSP_ERROR_RETURN((CGC_PRV_CLOCK_STATE_RUNNING == running_state), FSP_ERR_CGC_CLOCK_INACTIVE);

    /* Get the current stabilization state of the specified clock source */
    cgc_prv_clock_stability_state_t stable_state = r_cgc_stabilization_check(clock_source);

    /* Return error if the clock is not stabilized */
    FSP_ERROR_RETURN(CGC_PRV_CLOCK_STABILITY_STATE_STABLE == stable_state, FSP_ERR_CGC_NOT_STABILIZED);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function returns the run state of the selected clock
 * After power supply the LS intOSC starts operation. It cannot be stop
 *
 * @param[in]  clock   Clock to check
 *
 * @return  1 if running, 0 if stopped
 **********************************************************************************************************************/
static cgc_prv_clock_state_t r_cgc_clock_run_state_get (cgc_clock_t clock)
{
    /* Get clock run state. */
    uint32_t              reg   = 0U;
    cgc_prv_clock_state_t state = CGC_PRV_CLOCK_STATE_STOPPED;

    switch (clock)
    {
        /* Check if the selected clock is Main OSC */
        case CGC_CLOCK_MAIN_OSC:
        {
            reg = R_SYS0_A_CLOCK->MOSCS;
            if ((reg & R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk) != 0)
            {
                state = CGC_PRV_CLOCK_STATE_RUNNING;
            }

            break;
        }

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)

        /* Check the running state of Sub OSC */
        case CGC_CLOCK_SUBCLOCK:
        {
            reg = R_SYS0_A_CLOCK->SOSCS;
            if ((reg & R_SYS0_A_CLOCK_SOSCS_SOSCEN_Msk) != 0)
            {
                state = CGC_PRV_CLOCK_STATE_RUNNING;
            }

            break;
        }
#endif

        /* The LOCO always running after power-up. It cannot be stopped.
         * The HOCO is only stopped when have a request in chip standby mode.*/

        case CGC_CLOCK_HOCO:
        case CGC_CLOCK_LOCO:
        {
            state = CGC_PRV_CLOCK_STATE_RUNNING;
            break;
        }

#if BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE

        /* The HVIO always running after power-up and only stop when transitioning to DeepSTOP mode*/
        case CGC_CLOCK_HVIO:
        {
            state = CGC_PRV_CLOCK_STATE_RUNNING;
            break;
        }
#endif

        /* Check the running state of PLL/SSCG/SSCG1 */
        case CGC_CLOCK_PLL:
#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
        case CGC_CLOCK_SSCG:
#endif
            {
                reg = R_SYS0_CLOCK->PLLS;
                if ((reg & R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk) != 0)
                {
                    state = CGC_PRV_CLOCK_STATE_RUNNING;
                }

                break;
            }

        default:
        {
            /* Should not run in to this case */
            break;
        }
    }

    return state;
}

/*******************************************************************************************************************//**
 * This function checks the selected clock for stability.
 *
 * @param[in]  clock   Clock to check
 *
 * @return true if stable, false if not stable
 **********************************************************************************************************************/
static cgc_prv_clock_stability_state_t r_cgc_stabilization_check (cgc_clock_t clock)
{
    uint32_t reg = 0U;
    cgc_prv_clock_stability_state_t state = CGC_PRV_CLOCK_STABILITY_STATE_NOT_STABLE;

    switch (clock)
    {
        /* Check stable state for Main OSC */
        case CGC_CLOCK_MAIN_OSC:
        {
            reg = R_SYS0_A_CLOCK->MOSCS;
            if ((reg & R_SYS0_A_CLOCK_MOSCS_MOSCEN_Msk) && (reg & R_SYS0_A_CLOCK_MOSCS_MOSCSTAB_Msk))
            {
                state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;
            }

            break;
        }

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)

        /* Check stable state for Sub OSC */
        case CGC_CLOCK_SUBCLOCK:
        {
            reg = R_SYS0_A_CLOCK->SOSCS;
            if ((reg & R_SYS0_A_CLOCK_SOSCS_SOSCEN_Msk) && (reg & R_SYS0_A_CLOCK_SOSCS_SOSCSTAB_Msk))
            {
                state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;
            }

            break;
        }
#endif

        /* Check stable state for PLL/SSCG/SSCG1 */
        case CGC_CLOCK_PLL:
#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
        case CGC_CLOCK_SSCG:
#endif
            {
                reg = R_SYS0_CLOCK->PLLS;
                if ((reg & R_SYS0_CLOCK_PLLS_PLLCLKEN_Msk) && (reg & R_SYS0_CLOCK_PLLS_PLLCLKSTAB_Msk))
                {
                    state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;
                }

                break;
            }

        /* Check stable state for HOCO */
        case CGC_CLOCK_HOCO:
        {
            reg = R_SYS0_A_CLOCK->HSOSCS;
            if (reg & R_SYS0_A_CLOCK_HSOSCS_HSOSCSTAB_Msk)
            {
                state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;
            }

            break;
        }

        /* The LOCO always running after power-up. It cannot be stopped */
        case CGC_CLOCK_LOCO:
        {
            state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;

            break;
        }

#if BSP_FEATURE_CGC_HVIOSC_IS_AVAILABLE

        /* The HVIO always running after power-up and only stop when transitioning to DeepSTOP mode*/
        case CGC_CLOCK_HVIO:
        {
            state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;

            break;
        }
#endif
        default:
        {
            /* Should not run in to this case */
            break;
        }
    }

    return state;
}

/*******************************************************************************************************************//**
 * @brief This function is called by @ref R_CGC_SystemClockSet to changing the system clock at runtime
 *
 * @param[in]  p_system_clock         Pointer to system clock information
 * @param[in]  system_clock           System clock domain to be shifted
 * @param[in]  shift_behavior         System clock gear operation (gear up, gear down, or unchanged).
 *
 * @retval FSP_SUCCESS                               Change the system clock successfully
 * @retval FSP_ERR_CGC_NOT_STABILIZED                Clock has not stabilized
 * @retval FSP_ERR_CGC_INVALID_DIVIDER_GEAR_DOWN     Invalid division ratio of clock source
 * @retval FSP_ERR_CGC_INVALID_GEAR_OPERATION        The operation is invalid in the current clock state
 **********************************************************************************************************************/
static fsp_err_t r_cgc_sys_clk_shift (cgc_sys_clock_info_t      * p_system_clock,
                                      cgc_prv_system_clock_t      system_clock,
                                      cgc_system_clock_behavior_t shift_behavior)
{
    cgc_prv_sys_clk_src_t clk_src = p_system_clock->get_clock_source();

    if (CGC_SYSTEM_CLOCK_BEHAVIOR_GEAR_DOWN == shift_behavior)
    {
        if (CGC_PRV_SYS_CLK_SRC_PLLO == clk_src)
        {
            /* No division is selected as the division ratio of clock source */
            FSP_ERROR_RETURN((CGC_CLOCK_NO_DIVISION == *p_system_clock->p_ckd_ctrl) &&
                             (0UL != (*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk)),
                             FSP_ERR_CGC_INVALID_DIVIDER_GEAR_DOWN);

            switch (system_clock)
            {
#if (BSP_FEATURE_CGC_HAS_CLK_SYS)
                case CGC_PRV_SYSTEM_CLOCK_SYS:
                {
                    /* Gear down if clock source is PLL */
                    r_cgc_gear_down(p_system_clock, BSP_CFG_CLOCK_CLK_PLL_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)
                case CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN:
                {
                    /* Gear down if clock source is PLL/SSCG/SSCG1 */
                    r_cgc_gear_down(p_system_clock, BSP_CFG_CLOCK_CLK_PLL_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
                case CGC_PRV_SYSTEM_CLOCK_SYS_SSCG:
                {
                    /* Gear down if clock source is PLL/SSCG/SSCG1 */
                    r_cgc_gear_down(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)
                case CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1:
                {
 #if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ

                    /* Gear down if clock source is PLL/SSCG/SSCG1 */
                    r_cgc_gear_down(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG1_HZ);

                    break;
 #elif BSP_FEATURE_DFA_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ

                    /* Gear down if clock source is PLL/SSCG/SSCG1 */
                    r_cgc_gear_down(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG1_HZ);

                    break;
 #else

                    return FSP_ERR_CGC_INVALID_GEAR_OPERATION;
 #endif
                }
#endif
                default:
                {
                    /* Should not run in to this case */
                    break;
                }
            }
        }
        else
        {
            return FSP_ERR_CGC_INVALID_GEAR_OPERATION;
        }
    }
    else if (CGC_SYSTEM_CLOCK_BEHAVIOR_GEAR_UP == shift_behavior)
    {
        if (CGC_PRV_SYS_CLK_SRC_IOSC == clk_src)
        {
            /* Ensure PLL/SSCG/SSCG1 is running and stable before switching to it. */
            FSP_ERROR_RETURN(FSP_SUCCESS == r_cgc_clock_check(CGC_CLOCK_PLL), FSP_ERR_CGC_NOT_STABILIZED);

            switch (system_clock)
            {
#if (BSP_FEATURE_CGC_HAS_CLK_SYS)
                case CGC_PRV_SYSTEM_CLOCK_SYS:
                {
                    /* Gear up if clock source is IOSC */
                    r_cgc_gear_up(p_system_clock, BSP_CFG_CLOCK_CLK_PLL_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)
                case CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN:
                {
                    /* Gear up if clock source is IOSC */
                    r_cgc_gear_up(p_system_clock, BSP_CFG_CLOCK_CLK_PLL_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
                case CGC_PRV_SYSTEM_CLOCK_SYS_SSCG:
                {
                    /* Gear up if clock source is IOSC */
                    r_cgc_gear_up(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG_HZ);

                    break;
                }
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)
                case CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1:
                {
 #if BSP_FEATURE_DFP_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ

                    /* Gear up if clock source is IOSC */
                    r_cgc_gear_up(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG1_HZ);

                    break;
 #elif BSP_FEATURE_DFA_IS_AVAILABLE && BSP_CFG_CLOCK_CLK_SSCG1_HZ

                    /* Gear up if clock source is IOSC */
                    r_cgc_gear_up(p_system_clock, BSP_CFG_CLOCK_CLK_SSCG1_HZ);

                    break;
 #else

                    return FSP_ERR_CGC_INVALID_GEAR_OPERATION;
 #endif
                }
#endif
                default:
                {
                    /* Should not run in to this case */
                    break;
                }
            }
        }
        else
        {
            return FSP_ERR_CGC_INVALID_GEAR_OPERATION;
        }
    }
    else
    {
        /* Do nothing */;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * This function starts or stops the PLL clock.
 *
 * @param[in]  state                       Clock to start or stop
 *
 * @retval     FSP_SUCCESS                 The PLL clock source is start or stop successfully.
 * @retval     FSP_ERR_CGC_NOT_STABILIZED  The PLL clock source is not stabilized.
 * @retval     FSP_ERR_CGC_CLOCK_INACTIVE  The PLL clock source is not started.
 * @retval     FSP_ERR_CGC_CLOCK_IN_USE    The PLL clock source has been started.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_pll_clock_change (cgc_clock_change_t state)
{
    fsp_err_t                       err          = FSP_SUCCESS;
    cgc_prv_clock_state_t           run_state    = CGC_PRV_CLOCK_STATE_STOPPED;
    cgc_prv_clock_stability_state_t stable_state = CGC_PRV_CLOCK_STABILITY_STATE_STABLE;

    /* Start the PLL clock */
    if (CGC_CLOCK_CHANGE_START == state)
    {
        /* Confirm that the Main OSC is stable */
        run_state = r_cgc_clock_run_state_get(CGC_CLOCK_MAIN_OSC);
        FSP_ERROR_RETURN(CGC_PRV_CLOCK_STATE_RUNNING == run_state, FSP_ERR_CGC_CLOCK_INACTIVE);
        stable_state = r_cgc_stabilization_check(CGC_CLOCK_MAIN_OSC);
        FSP_ERROR_RETURN(CGC_PRV_CLOCK_STABILITY_STATE_STABLE == stable_state, FSP_ERR_CGC_NOT_STABILIZED);

        run_state = r_cgc_clock_run_state_get(CGC_CLOCK_PLL);
        FSP_ERROR_RETURN(CGC_PRV_CLOCK_STATE_STOPPED == run_state, FSP_ERR_CGC_CLOCK_IN_USE);

        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Start PLL */
        R_SYS0_CLOCK->PLLE = R_SYS0_CLOCK_PLLE_PLLENTRG_Msk;

        /* Confirm that the PLL has been stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS, CGC_CLOCK_STABLED);

        FSP_CRITICAL_SECTION_EXIT;
    }
    /* Stop the PLL clock */
    else
    {
        cgc_prv_sys_clk_src_t sys_clock_source[BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK];
#if (BSP_FEATURE_CGC_HAS_CLK_SYS)

        /* get the current system CLEAN clock source */
        sys_clock_source[0] = r_cgc_sys_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)

        /* get the current system CLEAN clock source */
        sys_clock_source[0] = r_cgc_sysclean_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)

        /* get the current system SSCG clock source */
        sys_clock_source[1] = r_cgc_syssscg_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)

        /* get the current system SSCG1 clock source */
        sys_clock_source[2] = r_cgc_syssscg1_clocksource_get();
#endif

        for (uint8_t i = 0; i < BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK; i++)
        {
            /* Don't disable PLL when it is input as system clock source */
            if (CGC_PRV_SYS_CLK_SRC_PLLO == sys_clock_source[i])
            {
                return FSP_ERR_CGC_CLOCK_IN_USE;
            }
        }

        stable_state = r_cgc_stabilization_check(CGC_CLOCK_PLL);
        FSP_ERROR_RETURN(CGC_PRV_CLOCK_STABILITY_STATE_STABLE == stable_state, FSP_ERR_CGC_NOT_STABILIZED);

        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Stop the PLL clock */
        R_SYS0_CLOCK->PLLE = R_SYS0_CLOCK_PLLE_PLLDISTRG_Msk;

        /* Confirm that PLL has been stopped */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->PLLS, CGC_CLOCK_STOPPED);

        FSP_CRITICAL_SECTION_EXIT;
    }

    return err;
}

/*******************************************************************************************************************//**
 * Internal function to change oscillator clock state (start or stop)
 *
 * @param[in]  clock_source             Clock to be changed
 * @param[in]  operation                Operation to perform (start or stop)
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 *
 * @retval FSP_SUCCESS                      Clock operation performed successfully.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE         The clock source is already running or cannot be stopped.
 * @retval FSP_ERR_CGC_INVALID_CLOCK_SOURCE Invalid clock source.
 * @retval FSP_ERR_CGC_CLOCK_INACTIVE       The PLL clock source is not started.
 * @retval FSP_ERR_CGC_NOT_STABILIZED       The clock source is not stabilized.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_clock_change (cgc_clock_t                     clock_source,
                                     cgc_clock_change_t              operation,
                                     cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Disable register protection */
    r_cgc_disable_protection();

    switch (clock_source)
    {
        /* Check if the selected clock is HOCO or LOCO
         * Note: The LOCO always running after power-up and it cannot be stopped
         * The HOCO is only stopped when have a request in chip standby mode. */

        /* Start/Stop the PLL/SSCG/SSCG1 clock */
        case CGC_CLOCK_PLL:
#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)
        case CGC_CLOCK_SSCG:
#endif
            {
                if (CGC_CLOCK_CHANGE_START == operation)
                {
                    err = r_cgc_pll_clock_change(CGC_CLOCK_CHANGE_START);
                }
                else if (CGC_CLOCK_CHANGE_STOP == operation)
                {
                    err = r_cgc_pll_clock_change(CGC_CLOCK_CHANGE_STOP);
                }
                else
                {
                    /* Do nothing */
                }

                break;
            }

        /* Start/Stop the Main Oscillator */
        case CGC_CLOCK_MAIN_OSC:
        {
            if (CGC_CLOCK_CHANGE_START == operation)
            {
                err = r_cgc_main_osc_start(&p_cgc_peripheral_clock[CGC_PERIPHERAL_CLK_BEFORE_MAIN_OSC_STOP]);
            }
            else if (CGC_CLOCK_CHANGE_STOP == operation)
            {
                err = r_cgc_main_osc_stop(&p_cgc_peripheral_clock[CGC_PERIPHERAL_CLK_BEFORE_MAIN_OSC_STOP]);
            }
            else
            {
                /* Do nothing */
            }

            break;
        }

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)

        /* Start/Stop the Sub-Clock Oscillator */
        case CGC_CLOCK_SUBCLOCK:
        {
            if (CGC_CLOCK_CHANGE_START == operation)
            {
                err = r_cgc_sub_osc_start(&p_cgc_peripheral_clock[CGC_PERIPHERAL_CLK_BEFORE_SUB_OSC_STOP]);
            }
            else if (CGC_CLOCK_CHANGE_STOP == operation)
            {
                err = r_cgc_sub_osc_stop(&p_cgc_peripheral_clock[CGC_PERIPHERAL_CLK_BEFORE_SUB_OSC_STOP]);
            }
            else
            {
                /* Do nothing */
            }

            break;
        }
#endif

        /* Anything else is invalid */
        default:
        {
            err = FSP_ERR_CGC_INVALID_CLOCK_SOURCE;

            break;
        }
    }

    /* Enable register protection */
    r_cgc_enable_protection();

    return err;
}

/*******************************************************************************************************************//**
 * @brief This function gears down the selected system clock.
 *
 * If CLK_PLLO/SSCGO/SSCG1O is selected as the System Clock source, this function performs the necessary steps
 * to gear down the system clock.
 * The number and type of steps performed depend on the frequency of the selected source clock.
 *
 * @param[in]  p_system_clock          Pointer to system clock information
 * @param[in]  clock_hz                Clock Frequency
 **********************************************************************************************************************/
static void r_cgc_gear_down (cgc_sys_clock_info_t * p_system_clock, uint32_t clock_hz)
{
    /* Ensure the clock switching sequence executes atomically*/
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

#if (1U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)
    *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_2;

    /* Confirm that PLL dividers are synchronized */
    FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                               R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
#else
    uint8_t repetition_count     = 0;
    uint8_t total_of_repetitions = 0;

 #if (2U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Get the total of repetitions */
    total_of_repetitions = (sizeof(cgc_gear_down_repetitions_arr) / sizeof(cgc_gear_down_repetitions_arr[0]));

    /* Start the repetitions 5 repetitions (800/640MHz) */
    if (BSP_CLOCK_SYSTEM_480MHz != clock_hz)
    {
        total_of_repetitions = total_of_repetitions;
    }
    else
    {
        total_of_repetitions = total_of_repetitions - 1;
    }

    for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
    {
        /* Write the divider value to PLL/SSCG/SSCG1 */
        *p_system_clock->p_ckd_ctrl = cgc_gear_down_repetitions_arr[repetition_count];

        /* Confirm that PLL/SSCG/SSCG1 dividers are synchronized */
        FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                   R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
 #elif (5U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)
    switch (clock_hz)
    {
        case BSP_CLOCK_SYSTEM_800MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_800_gear_down_repetitions_arr) / sizeof(cgc_800_gear_down_repetitions_arr[0]));

            /* Start the repetitions 6 repetitions (800MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL */
                *p_system_clock->p_ckd_ctrl = cgc_800_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_640MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_640_gear_down_repetitions_arr) / sizeof(cgc_640_gear_down_repetitions_arr[0]));

            /* Start the repetitions 6 repetitions (640MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_640_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_480MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_480_gear_down_repetitions_arr) / sizeof(cgc_480_gear_down_repetitions_arr[0]));

            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_480_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_320MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_320_gear_down_repetitions_arr) / sizeof(cgc_320_gear_down_repetitions_arr[0]));

            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_320_gear_down_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_240MHz:
        {
            /* Write the divider value to PLL/SSCG */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_12_DIV_16;

            /* Confirm that PLL dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                       R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);

            break;
        }

        default:
        {
            /* Should not run in to this case */
            break;
        }
    }

    /* Delay 20us */
    R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);

    /* Check if No division is not selected as the division ratio of clock source HSIOSC */
    if (CGC_CLOCK_NO_DIVISION != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
    {
        R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0U;

        /* Confirm that CLK_HSIOSCO divider is stable */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
    }
 #endif
#endif

    /* Change the clock source for the system clock to CLK_IOSC */
    *p_system_clock->p_cks_ctrl = CGC_PRV_SYS_CLK_SRC_IOSC;

    /* Confirm that IOSC has been selected */
    FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_cks_stat, CGC_PRV_SYS_CLK_SRC_IOSC);

#if (5U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Delay 20us */
    R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);
#else

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
#endif

    FSP_CRITICAL_SECTION_EXIT;
}

/*******************************************************************************************************************//**
 * This function gears up the selected system clock.
 *
 * If CLK_IOSC is selected as the System Clock source, this function performs the necessary steps
 * to gear up the system clock.
 * The number of steps performed depend on the frequency of the selected source clock.
 *
 * @param[in]  p_system_clock          Pointer to system clock information
 * @param[in]  clock_hz                Clock Frequency
 **********************************************************************************************************************/
static void r_cgc_gear_up (cgc_sys_clock_info_t * p_system_clock, uint32_t clock_hz)
{
    /* Ensure the clock switching sequence executes atomically*/
    FSP_CRITICAL_SECTION_DEFINE;
    FSP_CRITICAL_SECTION_ENTER;

#if (1U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Division ratio of clock source PLL are changed from 1/1 to 1/2 */
    *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_2;

    /* Confirm that PLL dividers are synchronized */
    FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                               R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
#else
    uint8_t repetition_count     = 0;
    uint8_t total_of_repetitions = 0;

 #if (2U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Start the repetitions 5 repetitions (800/640MHz) */
    if (BSP_CLOCK_SYSTEM_480MHz != clock_hz)
    {
        /* Division ratio of clock source PLL/SSCG/SSCG1 are changed from 1/1 to 3/8 */
        *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_6_DIV_16;
    }
    else
    {
        /* Division ratio of clock source PLL/SSCG/SSCG1 are changed from 1/1 to 4/8 */
        *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_8_DIV_16;
    }

 #elif (5U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)
    switch (clock_hz)
    {
        case BSP_CLOCK_SYSTEM_800MHz:
        {
            /* Division ratio of clock source PLL/SSCG are changed from 1/1 to 4/16 */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_4_DIV_16;

            break;
        }

        case BSP_CLOCK_SYSTEM_640MHz:
        {
            /* Division ratio of clock source PLL/SSCG are changed from 1/1 to 5/16 */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_5_DIV_16;

            break;
        }

        case BSP_CLOCK_SYSTEM_480MHz:
        {
            /* Division ratio of clock source PLL/SSCG are changed from 1/1 to 7/16 */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_7_DIV_16;

            break;
        }

        case BSP_CLOCK_SYSTEM_320MHz:
        {
            /* Division ratio of clock source PLL/SSCG are changed from 1/1 to 8/16 */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_10_DIV_16;

            break;
        }

        case BSP_CLOCK_SYSTEM_240MHz:
        {
            /* Division ratio of clock source PLL/SSCG are changed from 1/1 to 12/16 */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_DIVIDED_BY_12_DIV_16;

            /* Confirm that PLL0 divider is synchronized */
            FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                       R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);

            break;
        }

        default:
        {
            break;
        }
    }

    /* Check if No division is not selected as the division ratio of clock source HSIOSC */
    if (CGC_CLOCK_NO_DIVISION != R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
    {
        if (2U == R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID)
        {
            /* Division ratio of clock source HSIOSCO must go through 1/2 to switch between 1/10 and 1/1. */
            R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0x01UL;

            /* Confirm that HSIOSCO divider is synchronized */
            FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
        }

        /* Division ratio of clock source HSIOSCO are changed to 1/1 */
        R_SYS0_CLOCK->CKD_HSIOSCC_b.HSIOSCCLKDCSID = 0x00UL;

        /* Confirm that CLK_HSIOSCO divider is synchronized */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKD_HSIOSCS, R_SYS0_CLOCK_CKD_HSIOSCS_HSIOSCCLKDSYNC_Msk);
    }
 #endif
#endif

    /* Setting PLL/SSCG/SSCG1 for system clock */
    *p_system_clock->p_cks_ctrl = CGC_PRV_SYS_CLK_SRC_PLLO;

    /* Confirm that PLL/SSCG/SSCG1 have been selected */
    FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_cks_stat, CGC_PRV_SYS_CLK_SRC_PLLO);

#if (1U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Write the divider value to PLL */
    *p_system_clock->p_ckd_ctrl = CGC_CLOCK_NO_DIVISION;

    /* Confirm that PLL dividers are synchronized */
    FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                               R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
#elif (2U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)

    /* Get the total of repetitions */
    total_of_repetitions = (sizeof(cgc_gear_up_repetitions_arr) / sizeof(cgc_gear_up_repetitions_arr[0]));

    /* Start the repetitions 5 repetitions (800/640MHz) */
    if (BSP_CLOCK_SYSTEM_480MHz != clock_hz)
    {
        for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the divider value to PLL/SSCG/SSCG1 */
            *p_system_clock->p_ckd_ctrl = cgc_gear_up_repetitions_arr[repetition_count];

            /* Confirm that PLL/SSCG/SSCG1 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                       R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
        }
    }
    /* Start the repetitions 4 repetitions (480MHz) */
    else
    {
        for (repetition_count = 1; repetition_count < total_of_repetitions; repetition_count++)
        {
            /* Write the divider value to PLL/SSCG/SSCG1 */
            *p_system_clock->p_ckd_ctrl = cgc_gear_up_repetitions_arr[repetition_count];

            /* Confirm that PLL/SSCG/SSCG1 dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                       R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
        }
    }

    /* Delay 100us */
    R_BSP_SoftwareDelay(100, BSP_DELAY_UNITS_MICROSECONDS);
#elif (5U == BSP_FEATURE_CGC_NUM_OF_GEAR_SHIFT_REPETITION_CASE)
    switch (clock_hz)
    {
        case BSP_CLOCK_SYSTEM_800MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_800_gear_up_repetitions_arr) / sizeof(cgc_800_gear_up_repetitions_arr[0]));

            /* Start the repetitions 6 repetitions (800MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_800_gear_up_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_640MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_640_gear_up_repetitions_arr) / sizeof(cgc_640_gear_up_repetitions_arr[0]));

            /* Start the repetitions 6 repetitions (640MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_640_gear_up_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_480MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_480_gear_up_repetitions_arr) / sizeof(cgc_480_gear_up_repetitions_arr[0]));

            /* Start the repetitions 5 repetitions (480MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_480_gear_up_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_320MHz:
        {
            /* Get the total of repetitions */
            total_of_repetitions =
                (sizeof(cgc_320_gear_up_repetitions_arr) / sizeof(cgc_320_gear_up_repetitions_arr[0]));

            /* Start the repetitions 2 repetitions (320MHz) */
            for (repetition_count = 0; repetition_count < total_of_repetitions; repetition_count++)
            {
                /* Write the divider value to PLL/SSCG */
                *p_system_clock->p_ckd_ctrl = cgc_320_gear_up_repetitions_arr[repetition_count];

                /* Confirm that PLL/SSCG dividers are synchronized */
                FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                           R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);
            }

            break;
        }

        case BSP_CLOCK_SYSTEM_240MHz:
        {
            /* Write the divider value to PLL/SSCG */
            *p_system_clock->p_ckd_ctrl = CGC_CLOCK_NO_DIVISION;

            /* Confirm that PLL/SSCG dividers are synchronized */
            FSP_HARDWARE_REGISTER_WAIT(*p_system_clock->p_ckd_stat & R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk,
                                       R_SYS0_CLOCK_CKD_PLLS_PLLCLKDSYNC_Msk);

            break;
        }

        default:
        {
            /* Should not run in to this case */
            break;
        }
    }

    /* Delay 20us */
    R_BSP_SoftwareDelay(20, BSP_DELAY_UNITS_MICROSECONDS);
#endif

    FSP_CRITICAL_SECTION_EXIT;
}

/*******************************************************************************************************************//**
 * @brief Starts the main oscillator.
 *
 * This function enables the main oscillator if it is currently stopped and waits for it to stabilize.
 * Peripheral clocks that were temporarily switched to a safe source while the main oscillator was stopped are
 * automatically restored to the main oscillator after it starts.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 *
 * @retval FSP_SUCCESS                Main oscillator started successfully.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE   Main oscillator is already running.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_main_osc_start (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Start the clock */
    cgc_prv_clock_state_t run_state = r_cgc_clock_run_state_get(CGC_CLOCK_MAIN_OSC);

    if (CGC_PRV_CLOCK_STATE_STOPPED == run_state)
    {
        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Enable Main OSC */
        R_SYS0_A_CLOCK->MOSCE = R_SYS0_A_CLOCK_MOSCE_MOSCENTRG_Msk;

        /* Wait until stable */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS, CGC_CLOCK_STABLED);

        /* Restore the peripheral clocks */
        r_cgc_main_osc_restore(p_cgc_peripheral_clock);

        FSP_CRITICAL_SECTION_EXIT;
    }
    else
    {
        err = FSP_ERR_CGC_CLOCK_IN_USE;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief Stops the main oscillator.
 *
 * This function stops the main oscillator after performing necessary checks and configurations.
 * It ensures that the PLL input clock is IOSC1 before proceeding with the stop sequence.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 *
 * @retval FSP_SUCCESS                Main oscillator stopped successfully.
 * @retval FSP_ERR_CGC_NOT_STABILIZED Main oscillator is not stable and cannot be stopped.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE   Stop Main OSC while system clock select PLL/SSCG/SSCG1 outputs.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_main_osc_stop (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    cgc_prv_sys_clk_src_t sys_clock_source[BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK];

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)

    /* get the current system CLEAN clock source */
    sys_clock_source[CGC_PRV_SYSTEM_CLOCK_SYS] = r_cgc_sys_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)

    /* get the current system CLEAN clock source */
    sys_clock_source[CGC_PRV_SYSTEM_CLOCK_SYS_CLEAN] = r_cgc_sysclean_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)

    /* get the current system SSCG clock source */
    sys_clock_source[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG] = r_cgc_syssscg_clocksource_get();
#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)

    /* get the current system SSCG1 clock source */
    sys_clock_source[CGC_PRV_SYSTEM_CLOCK_SYS_SSCG1] = r_cgc_syssscg1_clocksource_get();
#endif

    for (uint8_t i = 0; i < BSP_FEATURE_CGC_NUM_OF_SYSTEM_CLOCK; i++)
    {
        /* Do not disable Main OSC while PLLO/SSCGO/SSCG1O is selected is selected for system clock */
        if (CGC_PRV_SYS_CLK_SRC_PLLO == sys_clock_source[i])
        {
            return FSP_ERR_CGC_CLOCK_IN_USE;
        }
    }

    /* Not allow to stop Main oscillators if the PLL/SSCG/SSCG1 is running with input clock source is Main OSC */
    cgc_prv_clock_state_t run_state = r_cgc_clock_run_state_get(CGC_CLOCK_PLL);
    if (CGC_PRV_CLOCK_STATE_RUNNING == run_state)
    {
        return FSP_ERR_CGC_CLOCK_IN_USE;
    }

    /* Stop the clock */
    cgc_prv_clock_stability_state_t stable_state = r_cgc_stabilization_check(CGC_CLOCK_MAIN_OSC);
    if (CGC_PRV_CLOCK_STABILITY_STATE_STABLE == stable_state)
    {
        /* Backup the the current peripheral clocks before stopping Main oscillator */
        r_cgc_peripheral_clock_backup(p_cgc_peripheral_clock);

        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        r_cgc_main_osc_pre_stop();

        /* Disable Main OSC */
        R_SYS0_A_CLOCK->MOSCE = R_SYS0_A_CLOCK_MOSCE_MOSCDISTRG_Msk;

        /* Wait until stopped */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->MOSCS, CGC_CLOCK_STOPPED);

        FSP_CRITICAL_SECTION_EXIT;
    }
    else
    {
        return FSP_ERR_CGC_NOT_STABILIZED;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Prepares the system for stopping the main oscillator (MOSC).
 *
 * This function switches clock sources for peripherals that might be using the MOSC
 * to ensure continued operation after the MOSC is stopped.
 **********************************************************************************************************************/
static void r_cgc_main_osc_pre_stop (void)
{
#if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)

    /* Change the RLIN3 input clock source */
    if ((CGC_RLIN_CLK_SEL_MOSC == R_SYS0_CLOCK->CKS_RLINS) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_4 == R_SYS0_CLOCK->CKS_RLINS) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_8 == R_SYS0_CLOCK->CKS_RLINS))
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_CLOCK->CKS_RLINC = CGC_RLIN_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, CGC_RLIN_CLK_SEL_EMG);
    }

    /* Change the RLIN3_CH23 input clock source */
    if ((CGC_RLIN_CLK_SEL_MOSC == (R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos)) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_4 == (R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos)) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_8 == (R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos)))
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_CLOCK->CKS_RLINC = (R_SYS0_CLOCK->CKS_RLINC & ~R_SYS0_CLOCK_CKS_RLINC_RLIN23SCSID_Msk) |
                                  ((uint32_t) CGC_RLIN_CLK_SEL_EMG << R_SYS0_CLOCK_CKS_RLINC_RLIN23SCSID_Pos);

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT((R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos),
                                   CGC_RLIN_CLK_SEL_EMG);
    }

    /* Change the TAUJ input clock source */
    if (CGC_TAUJ_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKS_ATAUJS)
    {
        /* Change to another input clock source for TAUJ3 */
        R_SYS0_A_CLOCK->CKS_ATAUJC = CGC_TAUJ_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS, CGC_TAUJ_CLK_SEL_EMG);
    }

    /* Change the CANFD_XIN input clock source */
    if ((CGC_CANFD_CLK_SEL_MOSC == R_SYS0_CLOCK->CKS_RCANS) ||
        (CGC_CANFD_CLK_SEL_MOSC_DIV_2 == R_SYS0_CLOCK->CKS_RCANS) ||
        (CGC_CANFD_CLK_SEL_MOSC_DIV_4 == R_SYS0_CLOCK->CKS_RCANS))
    {
        /* Change to another input clock source for CANFD_XIN */
        R_SYS0_CLOCK->CKS_RCANC = CGC_CANFD_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RCANS, CGC_CANFD_CLK_SEL_EMG);
    }

    /* Change the RTCA input clock source */
    if (CGC_RTCA_CLK_SEL_MOSC_DIV_16 == R_SYS0_A_CLOCK->CKS_ARTCAS)
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKS_ARTCAC = CGC_RTCA_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, CGC_RTCA_CLK_SEL_EMG);
    }

    /* Change the ADC input clock source */
    if (CGC_AADC_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKS_AADCS)
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_A_CLOCK->CKS_AADCC = CGC_AADC_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS, CGC_AADC_CLK_SEL_EMG);
    }

    /* Change the MSPI input clock source */
    if (CGC_MSPI_CLK_SEL_MOSC == R_SYS0_CLOCK->CKS_MSPIS)
    {
        /* Change to another input clock source for MSPI3 */
        R_SYS0_CLOCK->CKS_MSPIC = CGC_MSPI_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, CGC_MSPI_CLK_SEL_EMG);
    }

#else

    /* Change the RLIN3 input clock source */
    if ((CGC_RLIN_CLK_SEL_MOSC == R_SYS0_CLOCK->CKS_RLINS) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_4 == R_SYS0_CLOCK->CKS_RLINS) ||
        (CGC_RLIN_CLK_SEL_MOSC_DIV_8 == R_SYS0_CLOCK->CKS_RLINS))
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_CLOCK->CKS_RLINC = CGC_RLIN_CLK_SEL_HSB;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, CGC_RLIN_CLK_SEL_HSB);
    }

    /* Change the TAUJ input clock source */
    if (CGC_TAUJ_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKSC_ATAUJS)
    {
        /* Change to another input clock source for TAUJ3 */
        R_SYS0_A_CLOCK->CKSC_ATAUJC = CGC_TAUJ_CLK_SEL_HSIOSC_DIV_20;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, CGC_TAUJ_CLK_SEL_HSIOSC_DIV_20);
    }

    /* Change the MSPI input clock source */
    if (CGC_MSPI_CLK_SEL_MOSC == R_SYS0_CLOCK->CKS_MSPIS)
    {
        /* Change to another input clock source for MSPI3 */
        R_SYS0_CLOCK->CKS_MSPIC = CGC_MSPI_CLK_SEL_HSB;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, CGC_MSPI_CLK_SEL_HSB);
    }

    /* Change the RCANOSC input clock source */
    if (CGC_CANFD_CLK_SEL_MOSC != R_SYS0_CLOCK->CKSC_RCANS)
    {
        /* Change to another input clock source for RCANOSC */
        R_SYS0_CLOCK->CKSC_RCANC = CGC_CANFD_CLK_SEL_MOSC;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_RCANS, CGC_CANFD_CLK_SEL_MOSC);
    }

    /* Change the RTCA input clock source */
    if (CGC_RTCA_CLK_SEL_MOSC_DIV_16 == R_SYS0_A_CLOCK->CKSC_ARTCAS)
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKSC_ARTCAC = CGC_RTCA_CLK_SEL_LSIOSC;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS, CGC_RTCA_CLK_SEL_LSIOSC);
    }

    /* Change the ADC input clock source */
    if (CGC_AADC_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKSC_AADCS)
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_A_CLOCK->CKSC_AADCC = CGC_AADC_CLK_SEL_HSIOSC_DIV_20;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS, CGC_AADC_CLK_SEL_HSIOSC_DIV_20);
    }
#endif

#if (BSP_CFG_CLOCK_EXTCLK0O_DIV > 0)

    /* Change the EXTCLK0 input clock source */
 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    if (CGC_EXTCLK_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKS_EXTCLK0S)
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0C, 0x00U);

        /* Change to another input clock source for EXTCLK0 */
        R_SYS0_A_CLOCK->CKS_EXTCLK0C = CGC_EXTCLK_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, CGC_EXTCLK_CLK_SEL_HSB);

        /* Start the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);
    }

 #else
    if ((CGC_FOUT_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKSC_FOUT0S) ||
        (CGC_FOUT_CLK_SEL_MOSC_1 == R_SYS0_A_CLOCK->CKSC_FOUT0S) ||
        (CGC_FOUT_CLK_SEL_MOSC_2 == R_SYS0_A_CLOCK->CKSC_FOUT0S))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, CGC_CLOCK_STABLED);

        /* Stop the FOUT0 */
        R_SYS0_A_CLOCK->CLKD_FOUT0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0C, 0x00U);

        /* Change to another input clock source for FOUT0 */
        R_SYS0_A_CLOCK->CKSC_FOUT0C = CGC_FOUT_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, CGC_FOUT_CLK_SEL_HSB);

        /* Start the FOUT0 */
        R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, CGC_CLOCK_STABLED);
    }
 #endif
#endif

#if (BSP_CFG_CLOCK_EXTCLK1O_DIV > 0)

    /* Change the EXTCLK1 input clock source */
 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    if (CGC_EXTCLK_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKS_EXTCLK1S)
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1C, 0x00U);

        /* Change to another input clock source for EXTCLK1 */
        R_SYS0_A_CLOCK->CKS_EXTCLK1C = CGC_EXTCLK_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, CGC_EXTCLK_CLK_SEL_HSB);

        /* Start the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);
    }

 #else
    if ((CGC_FOUT_CLK_SEL_MOSC == R_SYS0_A_CLOCK->CKSC_FOUT1S) ||
        (CGC_FOUT_CLK_SEL_MOSC_1 == R_SYS0_A_CLOCK->CKSC_FOUT1S) ||
        (CGC_FOUT_CLK_SEL_MOSC_2 == R_SYS0_A_CLOCK->CKSC_FOUT1S))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, CGC_CLOCK_STABLED);

        /* Stop the FOUT1 */
        R_SYS0_A_CLOCK->CLKD_FOUT1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1C, 0x00U);

        /* Change to another input clock source for FOUT1 */
        R_SYS0_A_CLOCK->CKSC_FOUT1C = CGC_FOUT_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT1S, CGC_FOUT_CLK_SEL_HSB);

        /* Start the FOUT1 */
        R_SYS0_A_CLOCK->CLKD_FOUT1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, CGC_CLOCK_STABLED);
    }
 #endif
#endif
}

/*******************************************************************************************************************//**
 * @brief Restores peripheral clock sources after the main oscillator (MOSC) is restarted.
 *
 * This function restores the original clock sources for peripherals that were temporarily
 * switched to an alternative clock when the Main OSC was stopped, ensuring they return to
 * using the Main OSC as intended.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 **********************************************************************************************************************/
static void r_cgc_main_osc_restore (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    /* Change the RLIN3 input clock source */
    cgc_rlin_clk_sel_t pre_rlin     = p_cgc_peripheral_clock->clk_rlin;
    cgc_rlin_clk_sel_t current_rlin = (cgc_rlin_clk_sel_t) R_SYS0_CLOCK->CKS_RLINS;

    if (((CGC_RLIN_CLK_SEL_MOSC == pre_rlin) ||
         (CGC_RLIN_CLK_SEL_MOSC_DIV_4 == pre_rlin) ||
         (CGC_RLIN_CLK_SEL_MOSC_DIV_8 == pre_rlin)) &&
        (pre_rlin != current_rlin))
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_CLOCK->CKS_RLINC = (uint32_t) pre_rlin;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RLINS, (uint32_t) pre_rlin);
    }

    /* Change the MSPI input clock source */
    cgc_mspi_clk_sel_t pre_mspi     = p_cgc_peripheral_clock->clk_mspi;
    cgc_mspi_clk_sel_t current_mspi = (cgc_mspi_clk_sel_t) R_SYS0_CLOCK->CKS_MSPIS;

    if ((CGC_MSPI_CLK_SEL_MOSC == pre_mspi) && (pre_mspi != current_mspi))
    {
        /* Change to another input clock source for MSPI */
        R_SYS0_CLOCK->CKS_MSPIC = (uint32_t) pre_mspi;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_MSPIS, (uint32_t) pre_mspi);
    }

#if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)

    /* Change the RLIN3_CH23 input clock source */
    cgc_rlin_clk_sel_t pre_rlin_ch23 =
        (cgc_rlin_clk_sel_t) (((uint32_t) (p_cgc_peripheral_clock->clk_rlin_ch23)) >>
                              R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos);

    cgc_rlin_clk_sel_t current_rlin_ch23 =
        (cgc_rlin_clk_sel_t) (R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos);

    if (((CGC_RLIN_CLK_SEL_MOSC == pre_rlin_ch23) ||
         (CGC_RLIN_CLK_SEL_MOSC_DIV_4 == pre_rlin_ch23) ||
         (CGC_RLIN_CLK_SEL_MOSC_DIV_8 == pre_rlin_ch23)) &&
        (pre_rlin_ch23 != current_rlin_ch23))
    {
        /* Change to another input clock source for RLIN3_CH23 */
        R_SYS0_CLOCK->CKS_RLINC =
            (R_SYS0_CLOCK->CKS_RLINC & ~R_SYS0_CLOCK_CKS_RLINC_RLIN23SCSID_Msk) |
            ((uint32_t) pre_rlin_ch23 << R_SYS0_CLOCK_CKS_RLINC_RLIN23SCSID_Pos);

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT((R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos),
                                   (uint32_t) pre_rlin_ch23);
    }

    /* Change the TAUJ input clock source */
    cgc_tauj_clk_sel_t pre_tauj     = p_cgc_peripheral_clock->clk_tauj;
    cgc_tauj_clk_sel_t current_tauj = (cgc_tauj_clk_sel_t) R_SYS0_A_CLOCK->CKS_ATAUJS;

    if ((CGC_TAUJ_CLK_SEL_MOSC == pre_tauj) && (pre_tauj != current_tauj))
    {
        /* Change to another input clock source for TAUJ */
        R_SYS0_A_CLOCK->CKS_ATAUJC = (uint32_t) pre_tauj;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ATAUJS, (uint32_t) pre_tauj);
    }

    /* Change the CANFD_XIN input clock source */
    cgc_canfd_clk_sel_t pre_canfd     = p_cgc_peripheral_clock->clk_canfd;
    cgc_canfd_clk_sel_t current_canfd = (cgc_canfd_clk_sel_t) R_SYS0_CLOCK->CKS_RCANS;

    if (((CGC_CANFD_CLK_SEL_MOSC == pre_canfd) ||
         (CGC_CANFD_CLK_SEL_MOSC_DIV_2 == pre_canfd) ||
         (CGC_CANFD_CLK_SEL_MOSC_DIV_4 == pre_canfd)) &&
        (pre_canfd != current_canfd))
    {
        /* Change to another input clock source for CANFD_XIN */
        R_SYS0_CLOCK->CKS_RCANC = (uint32_t) pre_canfd;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKS_RCANS, (uint32_t) pre_canfd);
    }

    /* Change the RTCA input clock source */
    cgc_rtca_clk_sel_t pre_rtca     = p_cgc_peripheral_clock->clk_rtca;
    cgc_rtca_clk_sel_t current_rtca = (cgc_rtca_clk_sel_t) R_SYS0_A_CLOCK->CKS_ARTCAS;

    if ((CGC_RTCA_CLK_SEL_MOSC_DIV_16 == pre_rtca) && (pre_rtca != current_rtca))
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKS_ARTCAC = (uint32_t) pre_rtca;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, (uint32_t) pre_rtca);
    }

    /* Change the ADC input clock source */
    cgc_aadc_clk_sel_t pre_aadc     = p_cgc_peripheral_clock->clk_aadc;
    cgc_aadc_clk_sel_t current_aadc = (cgc_aadc_clk_sel_t) R_SYS0_A_CLOCK->CKS_AADCS;

    if ((CGC_AADC_CLK_SEL_MOSC == pre_aadc) && (pre_aadc != current_aadc))
    {
        /* Change to another input clock source for ADC */
        R_SYS0_A_CLOCK->CKS_AADCC = (uint32_t) pre_aadc;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_AADCS, (uint32_t) pre_aadc);
    }

#else

    /* Change the TAUJ input clock source */
    cgc_tauj_clk_sel_t pre_tauj     = p_cgc_peripheral_clock->clk_tauj;
    cgc_tauj_clk_sel_t current_tauj = (cgc_tauj_clk_sel_t) R_SYS0_A_CLOCK->CKSC_ATAUJS;

    if ((CGC_TAUJ_CLK_SEL_MOSC == pre_tauj) && (pre_tauj != current_tauj))
    {
        /* Change to another input clock source for TAUJ3 */
        R_SYS0_A_CLOCK->CKSC_ATAUJC = (uint32_t) pre_tauj;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ATAUJS, (uint32_t) pre_tauj);
    }

    /* Change the RCANOSC input clock source */
    cgc_canfd_clk_sel_t pre_canfd     = p_cgc_peripheral_clock->clk_canfd;
    cgc_canfd_clk_sel_t current_canfd = (cgc_canfd_clk_sel_t) R_SYS0_CLOCK->CKSC_RCANS;

    if ((CGC_CANFD_CLK_SEL_MOSC != pre_canfd) && (pre_canfd != current_canfd))
    {
        /* Change to another input clock source for RCANOSC */
        R_SYS0_CLOCK->CKSC_RCANC = (uint32_t) pre_canfd;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_CLOCK->CKSC_RCANS, (uint32_t) pre_canfd);
    }

    /* Change the RTCA input clock source */
    cgc_rtca_clk_sel_t pre_rtca     = p_cgc_peripheral_clock->clk_rtca;
    cgc_rtca_clk_sel_t current_rtca = (cgc_rtca_clk_sel_t) R_SYS0_A_CLOCK->CKSC_ARTCAS;

    if ((CGC_RTCA_CLK_SEL_MOSC_DIV_16 == pre_rtca) && (pre_rtca != current_rtca))
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKSC_ARTCAC = (uint32_t) pre_rtca;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_ARTCAS, (uint32_t) pre_rtca);
    }

    /* Change the ADC input clock source */
    cgc_aadc_clk_sel_t pre_aadc     = p_cgc_peripheral_clock->clk_aadc;
    cgc_aadc_clk_sel_t current_aadc = (cgc_aadc_clk_sel_t) R_SYS0_A_CLOCK->CKSC_AADCS;

    if ((CGC_AADC_CLK_SEL_MOSC == pre_aadc) && (pre_aadc != current_aadc))
    {
        /* Change to another input clock source for RLIN3 */
        R_SYS0_A_CLOCK->CKSC_AADCC = pre_aadc;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_AADCS, pre_aadc);
    }
#endif

#if (BSP_CFG_CLOCK_EXTCLK0O_DIV > 0)

    /* Change the EXTCLK0 input clock source */
    cgc_extclk_clk_sel_t pre_extclk0 = p_cgc_peripheral_clock->clk_extclk0;

 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    cgc_extclk_clk_sel_t current_extclk0 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK0S;
    if ((CGC_EXTCLK_CLK_SEL_MOSC == pre_extclk0) && (pre_extclk0 != current_extclk0))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0C, 0x00U);

        /* Change to another input clock source for EXTCLK0 */
        R_SYS0_A_CLOCK->CKS_EXTCLK0C = pre_extclk0;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, pre_extclk0);

        /* Start the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);
    }

 #else
    cgc_extclk_clk_sel_t current_extclk0 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKSC_FOUT0S;
    if (((CGC_FOUT_CLK_SEL_MOSC == pre_extclk0) ||
         (CGC_FOUT_CLK_SEL_MOSC_1 == pre_extclk0) ||
         (CGC_FOUT_CLK_SEL_MOSC_2 == pre_extclk0)) && (pre_extclk0 != current_extclk0))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, CGC_CLOCK_STABLED);

        /* Stop the FOUT0 */
        R_SYS0_A_CLOCK->CLKD_FOUT0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0C, 0x00U);

        /* Change to another input clock source for FOUT0 */
        R_SYS0_A_CLOCK->CKSC_FOUT0C = pre_extclk0;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT0S, pre_extclk0);

        /* Start the FOUT0 */
        R_SYS0_A_CLOCK->CLKD_FOUT0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT0S, CGC_CLOCK_STABLED);
    }
 #endif
#endif

#if (BSP_CFG_CLOCK_EXTCLK1O_DIV > 0)

    /* Change the EXTCLK1 input clock source */
    cgc_extclk_clk_sel_t pre_extclk1 = p_cgc_peripheral_clock->clk_extclk1;

 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    cgc_extclk_clk_sel_t current_extclk1 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK1S;
    if ((CGC_EXTCLK_CLK_SEL_MOSC == pre_extclk1) && (pre_extclk1 != current_extclk1))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1C, 0x00U);

        /* Change to another input clock source for EXTCLK1 */
        R_SYS0_A_CLOCK->CKS_EXTCLK1C = pre_extclk1;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, pre_extclk1);

        /* Start the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);
    }

 #else
    cgc_extclk_clk_sel_t current_extclk1 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKSC_FOUT1S;
    if (((CGC_FOUT_CLK_SEL_MOSC == pre_extclk1) ||
         (CGC_FOUT_CLK_SEL_MOSC_1 == pre_extclk1) ||
         (CGC_FOUT_CLK_SEL_MOSC_2 == pre_extclk1)) && (pre_extclk1 != current_extclk1))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, CGC_CLOCK_STABLED);

        /* Stop the FOUT1 */
        R_SYS0_A_CLOCK->CLKD_FOUT1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1C, 0x00U);

        /* Change to another input clock source for FOUT1 */
        R_SYS0_A_CLOCK->CKSC_FOUT1C = pre_extclk1;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKSC_FOUT1S, pre_extclk1);

        /* Start the FOUT1 */
        R_SYS0_A_CLOCK->CLKD_FOUT1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CLKD_FOUT1S, CGC_CLOCK_STABLED);
    }
 #endif
#endif
}

#if (BSP_FEATURE_CGC_SUB_OSC_IS_AVAILABLE)

/*******************************************************************************************************************//**
 * @brief Starts the sub-oscillator clock.
 *
 * This function enables the sub-oscillator clock if it is currently stopped.
 * It waits for the clock to stabilize before returning.
 * Peripheral clocks that were temporarily switched to a safe source while the sub oscillator was stopped are
 * automatically restored to the sub oscillator after it starts.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 *
 * @retval FSP_SUCCESS                Sub-oscillator started successfully.
 * @retval FSP_ERR_CGC_CLOCK_IN_USE   Sub-oscillator is already running.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_sub_osc_start (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Start the clock */
    cgc_prv_clock_state_t run_state = r_cgc_clock_run_state_get(CGC_CLOCK_SUBCLOCK);

    if (CGC_PRV_CLOCK_STATE_STOPPED == run_state)
    {
        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        /* Enable Sub-Clock */
        R_SYS0_A_CLOCK->SOSCE = R_SYS0_A_CLOCK_SOSCE_SOSCENTRG_Msk;

        /* Wait until stable */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, CGC_CLOCK_STABLED);

        /* Restore the peripheral clocks */
        r_cgc_sub_osc_restore(p_cgc_peripheral_clock);

        FSP_CRITICAL_SECTION_EXIT;
    }
    else
    {
        err = FSP_ERR_CGC_CLOCK_IN_USE;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief Stops the sub-oscillator clock.
 *
 * This function disables the sub-oscillator clock after performing necessary pre-stop operations.
 * It waits for the clock to completely stop before returning.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 *
 * @return FSP_SUCCESS                  Sub-oscillator clock stopped successfully.
 * @return FSP_ERR_CGC_NOT_STABILIZED   Sub-oscillator clock was not stable before attempting to stop.
 **********************************************************************************************************************/
static fsp_err_t r_cgc_sub_osc_stop (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    fsp_err_t err = FSP_SUCCESS;

    /* Stop the clock */
    cgc_prv_clock_stability_state_t stable_state = r_cgc_stabilization_check(CGC_CLOCK_SUBCLOCK);

    if (CGC_PRV_CLOCK_STABILITY_STATE_STABLE == stable_state)
    {
        /* Backup the the current peripheral clocks before stopping Sub oscillator */
        r_cgc_peripheral_clock_backup(p_cgc_peripheral_clock);

        /* Protect oscillator clock status update in a critical section to ensure atomicity */
        FSP_CRITICAL_SECTION_DEFINE;
        FSP_CRITICAL_SECTION_ENTER;

        r_cgc_sub_osc_pre_stop();

        /* Disable Sub-Clock */
        R_SYS0_A_CLOCK->SOSCE = R_SYS0_A_CLOCK_SOSCE_SOSCDISTRG_Msk;

        /* Wait until stopped */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->SOSCS, 0x00);

        FSP_CRITICAL_SECTION_EXIT;
    }
    else
    {
        err = FSP_ERR_CGC_NOT_STABILIZED;
    }

    return err;
}

/*******************************************************************************************************************//**
 * @brief Prepares the system for stopping the Sub-OSC (Sub Oscillator).
 *
 * Before stopping the Sub-OSC, this function switches the clock sources for RTCA (Real-Time Clock) and
 * EXTCLK0 (External Clock 0) from Sub-OSC to alternative sources to ensure continuous operation of these peripherals.
 **********************************************************************************************************************/
static void r_cgc_sub_osc_pre_stop (void)
{
    /* Change the RTCA input clock source */
    if (CGC_RTCA_CLK_SEL_SOSC == R_SYS0_A_CLOCK->CKS_ARTCAS)
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKS_ARTCAC = CGC_RTCA_CLK_SEL_EMG;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, CGC_RTCA_CLK_SEL_EMG);
    }

 #if (BSP_CFG_CLOCK_EXTCLK0O_DIV > 0)

    /* Change the EXTCLK0 input clock source */
    if (CGC_EXTCLK_CLK_SEL_SOSC == R_SYS0_A_CLOCK->CKS_EXTCLK0S)
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0C, 0x00U);

        /* Change to another input clock source for EXTCLK0 */
        R_SYS0_A_CLOCK->CKS_EXTCLK0C = CGC_EXTCLK_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, CGC_EXTCLK_CLK_SEL_HSB);

        /* Start the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);
    }
 #endif

 #if (BSP_CFG_CLOCK_EXTCLK1O_DIV > 0)

    /* Change the EXTCLK1 input clock source */
    if (CGC_EXTCLK_CLK_SEL_SOSC == R_SYS0_A_CLOCK->CKS_EXTCLK1S)
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1C, 0x00U);

        /* Change to another input clock source for EXTCLK1 */
        R_SYS0_A_CLOCK->CKS_EXTCLK1C = CGC_EXTCLK_CLK_SEL_HSB;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, CGC_EXTCLK_CLK_SEL_HSB);

        /* Start the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);
    }
 #endif
}

/*******************************************************************************************************************//**
 * @brief Restores peripheral clock sources after the Sub OSC (Sub Oscillator) is restarted.
 *
 * This function restores the original clock sources for RTCA (Real-Time Clock) and EXTCLK0 (External Clock 0)
 * that were temporarily switched to alternative sources when the Sub OSC was stopped, ensuring they return
 * to using the Sub OSC as intended.
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 **********************************************************************************************************************/
static void r_cgc_sub_osc_restore (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    /* Change the RTCA input clock source */
    cgc_rtca_clk_sel_t pre_rtca     = p_cgc_peripheral_clock->clk_rtca;
    cgc_rtca_clk_sel_t current_rtca = (cgc_rtca_clk_sel_t) R_SYS0_A_CLOCK->CKS_ARTCAS;
    if ((CGC_RTCA_CLK_SEL_SOSC == pre_rtca) && (pre_rtca != current_rtca))
    {
        /* Change to another input clock source for RTCA */
        R_SYS0_A_CLOCK->CKS_ARTCAC = pre_rtca;

        /* Confirm that input clock source has been changed */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_ARTCAS, pre_rtca);
    }

 #if (BSP_CFG_CLOCK_EXTCLK0O_DIV > 0)

    /* Change the EXTCLK0 input clock source */
    cgc_extclk_clk_sel_t pre_extclk0     = p_cgc_peripheral_clock->clk_extclk0;
    cgc_extclk_clk_sel_t current_extclk0 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK0S;
    if ((CGC_EXTCLK_CLK_SEL_SOSC == pre_extclk0) && (pre_extclk0 != current_extclk0))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0C, 0x00U);

        /* Change to another input clock source for EXTCLK0 */
        R_SYS0_A_CLOCK->CKS_EXTCLK0C = pre_extclk0;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK0S, pre_extclk0);

        /* Start the EXTCLK0 */
        R_SYS0_A_CLOCK->CKD_EXTCLK0C = BSP_CFG_CLOCK_EXTCLK0O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK0S, CGC_CLOCK_STABLED);
    }
 #endif

 #if (BSP_CFG_CLOCK_EXTCLK1O_DIV > 0)

    /* Change the EXTCLK1 input clock source */
    cgc_extclk_clk_sel_t pre_extclk1     = p_cgc_peripheral_clock->clk_extclk1;
    cgc_extclk_clk_sel_t current_extclk1 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK1S;
    if ((CGC_EXTCLK_CLK_SEL_SOSC == pre_extclk1) && (pre_extclk1 != current_extclk1))
    {
        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);

        /* Stop the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = 0x00U;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1C, 0x00U);

        /* Change to another input clock source for EXTCLK1 */
        R_SYS0_A_CLOCK->CKS_EXTCLK1C = pre_extclk1;
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKS_EXTCLK1S, pre_extclk1);

        /* Start the EXTCLK1 */
        R_SYS0_A_CLOCK->CKD_EXTCLK1C = BSP_CFG_CLOCK_EXTCLK1O_DIV;

        /* Confirm that clock output is stabled */
        FSP_HARDWARE_REGISTER_WAIT(R_SYS0_A_CLOCK->CKD_EXTCLK1S, CGC_CLOCK_STABLED);
    }
 #endif
}

#endif

/*******************************************************************************************************************//**
 * @brief This function saves the current peripheral input clock source
 *
 * @param[in]  p_cgc_peripheral_clock   Pointer to peripheral clocks backup
 **********************************************************************************************************************/
static void r_cgc_peripheral_clock_backup (cgc_peripheral_clock_backup_t * p_cgc_peripheral_clock)
{
    p_cgc_peripheral_clock->clk_rlin = (cgc_rlin_clk_sel_t) R_SYS0_CLOCK->CKS_RLINS;
    p_cgc_peripheral_clock->clk_mspi = (cgc_mspi_clk_sel_t) R_SYS0_CLOCK->CKS_MSPIS;

#if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    p_cgc_peripheral_clock->clk_rlin_ch23 =
        (cgc_rlin_clk_sel_t) (R_SYS0_CLOCK->CKS_RLINS >> R_SYS0_CLOCK_CKS_RLINS_RLIN23SACT_Pos);
    p_cgc_peripheral_clock->clk_tauj  = (cgc_tauj_clk_sel_t) R_SYS0_A_CLOCK->CKS_ATAUJS;
    p_cgc_peripheral_clock->clk_canfd = (cgc_canfd_clk_sel_t) R_SYS0_CLOCK->CKS_RCANS;
    p_cgc_peripheral_clock->clk_rtca  = (cgc_rtca_clk_sel_t) R_SYS0_A_CLOCK->CKS_ARTCAS;
    p_cgc_peripheral_clock->clk_aadc  = (cgc_aadc_clk_sel_t) R_SYS0_A_CLOCK->CKS_AADCS;
#else
    p_cgc_peripheral_clock->clk_tauj  = (cgc_tauj_clk_sel_t) R_SYS0_A_CLOCK->CKSC_ATAUJS;
    p_cgc_peripheral_clock->clk_canfd = (cgc_canfd_clk_sel_t) R_SYS0_CLOCK->CKSC_RCANS;
    p_cgc_peripheral_clock->clk_rtca  = (cgc_rtca_clk_sel_t) R_SYS0_A_CLOCK->CKSC_ARTCAS;
    p_cgc_peripheral_clock->clk_aadc  = (cgc_aadc_clk_sel_t) R_SYS0_A_CLOCK->CKSC_AADCS;
#endif

#if (BSP_CFG_CLOCK_EXTCLK0O_DIV > 0)
 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    p_cgc_peripheral_clock->clk_extclk0 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK0S;
 #else
    p_cgc_peripheral_clock->clk_extclk0 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKSC_FOUT0S;
 #endif
#endif

#if (BSP_CFG_CLOCK_EXTCLK1O_DIV > 0)
 #if (BSP_FEATURE_CGC_EXTENDED_PERIPHERAL_CLK_SELECTORS)
    p_cgc_peripheral_clock->clk_extclk1 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKS_EXTCLK1S;
 #else
    p_cgc_peripheral_clock->clk_extclk1 = (cgc_extclk_clk_sel_t) R_SYS0_A_CLOCK->CKSC_FOUT1S;
 #endif
#endif
}

#if (BSP_FEATURE_CGC_HAS_CLK_SYS)

/*******************************************************************************************************************//**
 * @brief This function get the current system SYS clock source
 **********************************************************************************************************************/
static cgc_prv_sys_clk_src_t r_cgc_sys_clocksource_get (void)
{
    return (cgc_prv_sys_clk_src_t) (R_SYS0_CLOCK->CKSC_CPUS ==
                                    1U ? CGC_PRV_SYS_CLK_SRC_IOSC : CGC_PRV_SYS_CLK_SRC_PLLO);
}

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_CLEAN)

/*******************************************************************************************************************//**
 * @brief This function get the current system CLEAN clock source
 **********************************************************************************************************************/
static cgc_prv_sys_clk_src_t r_cgc_sysclean_clocksource_get (void)
{
    return (cgc_prv_sys_clk_src_t) (R_SYS0_CLOCK->CKS_CLEANS ==
                                    1U ? CGC_PRV_SYS_CLK_SRC_IOSC : CGC_PRV_SYS_CLK_SRC_PLLO);
}

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG)

/*******************************************************************************************************************//**
 * @brief This function get the current system SSCG clock source
 **********************************************************************************************************************/
static cgc_prv_sys_clk_src_t r_cgc_syssscg_clocksource_get (void)
{
    return (cgc_prv_sys_clk_src_t) (R_SYS0_CLOCK->CKS_SSCGS ==
                                    1U ? CGC_PRV_SYS_CLK_SRC_IOSC : CGC_PRV_SYS_CLK_SRC_PLLO);
}

#endif

#if (BSP_FEATURE_CGC_HAS_CLK_SYS_SSCG1)

/*******************************************************************************************************************//**
 * @brief This function get the current system SSCG1 clock source
 **********************************************************************************************************************/
static cgc_prv_sys_clk_src_t r_cgc_syssscg1_clocksource_get (void)
{
    return (cgc_prv_sys_clk_src_t) (R_SYS0_CLOCK->CKS_SSCG1S ==
                                    1U ? CGC_PRV_SYS_CLK_SRC_IOSC : CGC_PRV_SYS_CLK_SRC_PLLO);
}

#endif

/*******************************************************************************************************************//**
 * @brief Disable the protected CGC registers to allow change of register content
 **********************************************************************************************************************/
static void r_cgc_disable_protection (void)
{
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_ENABLE;
}

/*******************************************************************************************************************//**
 * @brief Enable protected CGC registers and do not allow change of register content
 **********************************************************************************************************************/
static void r_cgc_enable_protection (void)
{
    R_SYS0_CLOCK->CLKKCPROT1 = BSP_PRV_KEYCODE_DISABLE;
}

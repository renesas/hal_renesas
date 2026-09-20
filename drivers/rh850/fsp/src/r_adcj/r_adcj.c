/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"                   /* PRQA S 0380 # Dir-1.1-C99, Dir-1.1-C11 */
#include "r_adcj.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "ADCJ" in ASCII, used to determine if unit is open. */
#define ADCJ_OPEN                                   (0x4144434AULL)

/* The address offset between scan groups */
#define ADCJ_SG_OFFSET                              (0x40U)

/* This macro is used to get scan group register address from the register address of the first scan group
 * and scan group index */
#define ADCJ_SG_REG_ADDR(base, index)    ((uint8_t *) (base) + (ADCJ_SG_OFFSET) *(index)) /* PRQA S 3472 # Dir-4.9 */

#define ADCJ_NUMBER_OF_UNIT                         (BSP_FEATURE_ADCJ_NUM_OF_UNIT)

#define ADCJ_SG3_AD_TIMER_CFG_INDEX                 (0x0U)
#define ADCJ_SG4_AD_TIMER_CFG_INDEX                 (0x1U)

#define ADCJ_VCR_END_INT_MASK                       (0x1U << 7U)

#define ADCJ_SGSTCR_SW_START                        (0x1U)

#define ADCJ_ADTSTCR_AD_TIMER_SW_START              (0x1U)

#define ADCJ_SGVCPR_VCSP_MASK                       (0x3F)
#define ADCJ_SGVCPR_VCEP_MASK                       (0x3F)
#define ADCJ_SGVCPR_VCEP_POS                        (8U)

#define ADCJ_SGCR_HW_TRIGGER_ENABLE                 (0x1U)
#define ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE           (0x1U << 1U)
#define ADCJ_SGCR_SCAN_END_INT_MASK                 (0x1U << 4U)

#define ADCJ_SGSR_SCAN_GROUP_IDLE                   (0U)

#define ADCJ_HALTR_HALT                             (0x1U)

#define ADCJ_THSTPCR_SAMPLING_STOP                  (0x1U)

#define ADCJ_THSMPSTCR_SAMPLING_START               (0x1U)

#define ADCJ_VCLMASCR_ALLMSC                        (0x1U)

#define ADCJ_SGULCCR_CLEAR_ALL                      (0x3FU)

#define ADCJ_SGSTPCR_SG_STOP                        (0x1U)

#define ADCJ_THGSR_TH_UNIT_BIT_OFFSET               (0x2U) /* PRQA S 1534 # Rule-2.5 */

#define ADCJ_THACR_SGS_MASK                         (0x3U)
#define ADCJ_THACR_HOLD_CTRL_ENABLE                 (0x1U)
#define ADCJ_THAHLDSTCR_HOLD_START                  (0x1U)
#define ADCJ_THACR_HW_TRIGGER_ENABLE                (0x1U)

#define ADCJ_THBCR_SGS_MASK                         (0x3U) /* PRQA S 1534 # Rule-2.5 */
#define ADCJ_THBCR_HOLD_CTRL_ENABLE                 (0x1U) /* PRQA S 1534 # Rule-2.5 */
#define ADCJ_THBHLDSTCR_HOLD_START                  (0x1U) /* PRQA S 1534 # Rule-2.5 */
#define ADCJ_THBCR_HW_TRIGGER_ENABLE                (0x1U) /* PRQA S 1534 # Rule-2.5 */

#define ADCJ_PWDSGCR_HW_TRIGGER_ENABLE              (0x1U)

#define ADCJ_SGSTR_SGS_STATUS_MASK                  (0x3FU)
#define ADCJ_SGSTR_AD_TIMER_STATUS_MASK             (0x3000U)

#define ADCJ_ADTENDCR_AD_TIMER_STOP                 (0x1U)

#define ADCJ_TOCER_SG0_MASK                         (0x1U << 1U)

#define ADCJ_ECR_ID_ERROR_STATUS_CLEAR              (0x1U)
#define ADCJ_ECR_PARITY_ERROR_STATUS_CLEAR          (0x1U << 1U) /* PRQA S 1534 # Rule-2.5 */
#define ADCJ_ECR_OVERWRITE_ERROR_STATUS_CLEAR       (0x1U << 2U)
#define ADCJ_ECR_TRIGGER_OVERLAP_ERROR_SG0_CLEAR    (0x1U << 9U)
#define ADCJ_ECR_CLEAR_ALL                          (0x3F07U)

#define ADCJ_VIRTUAL_CHANNEL_UL_STATUS_CLEAR        (0x1U)
#define ADCJ_PWM_DIAG_UL_STATUS_CLEAR               (0x1U)

#define ADCJ_TH_SAMPLING_DELAY_CLKAD_CYCLE          (30U)
#define ADCJ_REG_ACCESS_TIME_CLKAD_CYCLE            (2U)

#define ADCJ_CALLBACK_ARGS_CHANNEL_NOT_USE          (0U)
#define ADCJ_CALLBACK_ARGS_GROUP_MASK_NOT_USE       (0U)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void      r_adcj_scan_groups_force_termination(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_ul_limit_check_initialize(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_mpx_initialize(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_th_initialize(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_ad_timer_initialize(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_interrupts_enable(adcj_instance_ctrl_t * const p_instance_ctrl);
static void      r_adcj_interrupts_disable(adcj_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t r_adcj_scan_group_start(adcj_instance_ctrl_t * const p_instance_ctrl,
                                         adcj_scan_group_t const      scan_group);
__STATIC_INLINE void r_adcj_scan_group_vc_init(R_ADCJ1_Type * const p_reg, adcj_sg_generic_cfg_t const * p_sg_cfg);
static void          r_adcj_scan_group_stop(adcj_instance_ctrl_t * const p_instance_ctrl,
                                            adcj_scan_group_t const      scan_group);
__STATIC_INLINE bool r_adcj_is_th_sw_trigger(adcj_instance_ctrl_t * const p_instance_ctrl,
                                             adcj_scan_group_t const      scan_group);
static void          r_adcj_th_disable(adcj_instance_ctrl_t * const p_instance_ctrl, adcj_th_group_t const th_group);
__STATIC_INLINE void r_adcj_scan_group_vc_deinit(R_ADCJ1_Type * const p_reg, adcj_sg_generic_cfg_t const * p_sg_cfg);

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t r_adcj_group_mask_param_check(adc_group_mask_t const group_mask, adcj_sg_cfg_t const * p_sg_cfg);

#endif                                 /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

/* This function is used to call user callback functions */
__STATIC_INLINE void r_adcj_call_callback(adcj_instance_ctrl_t * const p_ctrl, adc_callback_args_t * const p_args);

/* This function handle common scan end interrupts */
__STATIC_INLINE void r_adcj_scan_end_common_handle(adc_event_t const event);

/* This function handle ID error interrupt for ADCJ */
__STATIC_INLINE void r_adcj_id_error_handle(adcj_instance_ctrl_t * const p_instance_ctrl);

/* This function handle overwrite error interrupt for ADCJ */
__STATIC_INLINE void r_adcj_overwrite_error_handle(adcj_instance_ctrl_t * const p_instance_ctrl);

/* This function handle trigger overlap error interrupt for ADCJ */
__STATIC_INLINE void r_adcj_trigger_overlap_error_handle(adcj_instance_ctrl_t * const p_instance_ctrl);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg0_scan_end_isr(void); /* PRQA S 3449, 3451 # Rule-8.5 */
BSP_INTERRUPT_ATTRIBUTE void adcj_sg1_scan_end_isr(void); /* PRQA S 3449, 3451 # Rule-8.5 */
BSP_INTERRUPT_ATTRIBUTE void adcj_sg2_scan_end_isr(void); /* PRQA S 3449, 3451 # Rule-8.5 */
BSP_INTERRUPT_ATTRIBUTE void adcj_sg3_scan_end_isr(void); /* PRQA S 3449, 3451 # Rule-8.5 */
BSP_INTERRUPT_ATTRIBUTE void adcj_sg4_scan_end_isr(void); /* PRQA S 3449, 3451 # Rule-8.5 */
BSP_INTERRUPT_ATTRIBUTE void adcj_error_isr(void);        /* PRQA S 3449, 3451 # Rule-8.5 */

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const adc_api_t g_adc_on_adcj =              /* PRQA S 1502 # Rule-2.8 */
{
    .open           = R_ADCJ_Open,           /* PRQA S 3635 # Rule-17.12 */
    .scanCfg        = R_ADCJ_ScanCfg,        /* PRQA S 3635 # Rule-17.12 */
    .scanStart      = R_ADCJ_ScanStart,      /* PRQA S 3635 # Rule-17.12 */
    .scanGroupStart = R_ADCJ_ScanGroupStart, /* PRQA S 3635 # Rule-17.12 */
    .scanStop       = R_ADCJ_ScanStop,       /* PRQA S 3635 # Rule-17.12 */
    .scanStatusGet  = R_ADCJ_ScanStatusGet,  /* PRQA S 3635 # Rule-17.12 */
    .read           = R_ADCJ_Read,           /* PRQA S 3635 # Rule-17.12 */
    .read32         = R_ADCJ_Read32,         /* PRQA S 3635 # Rule-17.12 */
    .calibrate      = R_ADCJ_Calibrate,      /* PRQA S 3635 # Rule-17.12 */
    .offsetSet      = R_ADCJ_OffsetSet,      /* PRQA S 3635 # Rule-17.12 */
    .callbackSet    = R_ADCJ_CallbackSet,    /* PRQA S 3635 # Rule-17.12 */
    .close          = R_ADCJ_Close,          /* PRQA S 3635 # Rule-17.12 */
    .infoGet        = R_ADCJ_InfoGet         /* PRQA S 3635 # Rule-17.12 */
};

/* PRQA S 0292 ++ # DP_MSR_D-1.1_UC1 */

/*******************************************************************************************************************//**
 * @addtogroup ADCJ
 * @{
 **********************************************************************************************************************/

/* PRQA S 0292 -- */

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Initialize the ADCJ
 * Implements @ref adc_api_t::open
 * This function configures the ADCJ's registers and sets up the necessary hardware configuration for operation.
 * It must be called before any other API interacting with the ADCJ.
 * This function forces to stop all ADCJ activities, setting up hardware features (suspend method, data format,
 * addition count in A/D addition conversions, error interrupts, clear after read function, upper/lower limit check,
 * sampling time, input buffer amp, pin-level self-diagnostic, Wiring-break detection, MPX function, A/D conversion
 * timing monitor, initialize T&H , A/D timers, wait table, callback function, initialize interrupts).
 *
 * @retval FSP_SUCCESS                     ADCJ instance is initialize successfully
 * @retval FSP_ERR_ASSERTION               The pointer input is NULL
 * @retval FSP_ERR_ALREADY_OPEN            ADCJ instance has already opened
 * @retval FSP_ERR_IP_UNIT_NOT_PRESENT     This unit is not supported
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_Open (adc_ctrl_t * const p_ctrl, adc_cfg_t const * const p_cfg) /* PRQA S 1505 # Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;    /* PRQA S 0316 # Rule-11.5 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_cfg);                                                          /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_cfg->p_extend);                                                /* PRQA S 3432 # Rule-20.7 */

    /* Check if unit input is valid */
    FSP_ERROR_RETURN((ADCJ_NUMBER_OF_UNIT > p_cfg->unit), FSP_ERR_IP_UNIT_NOT_PRESENT); /* PRQA S 3432 # Rule-20.7 */

    /* Verify this unit has already been opened   */
    FSP_ERROR_RETURN(ADCJ_OPEN != p_instance_ctrl->opened, FSP_ERR_ALREADY_OPEN);       /* PRQA S 3432 # Rule-20.7 */
#endif                                                                                  /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    p_instance_ctrl->p_cfg = p_cfg;
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_cfg->p_extend;           /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

#if (ADCJ_NUMBER_OF_UNIT > 2)
    {
        /* Get the ADCJ unit base address and store to p_reg */
        if (0U == p_instance_ctrl->p_cfg->unit)
        {
            p_instance_ctrl->p_reg      = R_ADCJ0; /* PRQA S 0306 # Rule-11.4 */
            p_instance_ctrl->p_selb_reg = NULL;
        }
        else if (1U == p_instance_ctrl->p_cfg->unit)
        {
            p_instance_ctrl->p_reg      = R_ADCJ1; /* PRQA S 0306 # Rule-11.4 */
            p_instance_ctrl->p_selb_reg = NULL;
        }
        else
        {
            p_instance_ctrl->p_reg      = R_ADCJ2;      /* PRQA S 0306 # Rule-11.4 */
            p_instance_ctrl->p_selb_reg = R_ADCJ2_SELB; /* PRQA S 0306 # Rule-11.4 */
        }
    }
#else
    {
        /* Get the ADCJ unit base address and store to p_reg */
        if (0U == p_instance_ctrl->p_cfg->unit)
        {
            p_instance_ctrl->p_reg      = R_ADCJ0;
            p_instance_ctrl->p_selb_reg = NULL;
        }
        else
        {
            p_instance_ctrl->p_reg      = R_ADCJ1;
            p_instance_ctrl->p_selb_reg = NULL;
        }
    }
#endif

    p_instance_ctrl->p_sg_cfg = NULL;

    /* Call termination procedure */
    r_adcj_scan_groups_force_termination(p_instance_ctrl);

    /* Set suspend method */
    p_instance_ctrl->p_reg->ADCJnADCR1 = (uint8_t) p_extend->suspend_method;                   /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Set data format and addition count */
    p_instance_ctrl->p_reg->ADCJnADCR2 = (uint8_t) p_extend->data_format_addition_count.adcr2; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* If overwrite error interrupt is enabled, dummy read all data registers to clear the write flag if any */
    if (ADCJ_OVERWRITE_ERROR_INT_ENABLE == p_extend->safety_cfg.sftcr_b.overwrite)             /* PRQA S 1881 # Rule-10.4 */
    {
        for (uint32_t channel = 0U; channel < ADCJ_NUMBER_OF_VIRTUAL_CHANNEL; channel++)
        {
            (void) p_instance_ctrl->p_reg->ADCJnDR[channel];                                   /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
    }

    /* Clear all id error flag, parity error flag, overwrite error flag, and trigger overlap error flags */
    p_instance_ctrl->p_reg->ADCJnECR = ADCJ_ECR_CLEAR_ALL;                     /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Configure read and clear, id error interrupt enable, parity error interrupt enable,
     * overwrite error interrupt enable, trigger overlap error interrupt enable */
    p_instance_ctrl->p_reg->ADCJnSFTCR = (uint8_t) p_extend->safety_cfg.sftcr; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    r_adcj_ul_limit_check_initialize(p_instance_ctrl);

    /* Setting sampling period, extended sampling period, sampling selection, and enable/disable input buffer */
    p_instance_ctrl->p_reg->ADCJnSMPCR = p_extend->sampling_cfg.smpcr;          /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting pin level self-diagnostic voltage level, enable/disable T&H path self-diagnostic */
    p_instance_ctrl->p_reg->ADCJnTDCR = p_extend->pin_lv_self_diag_cfg.tdcr;    /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting enable/disable addition wiring-break detection mode, setting detection pulse length in wiring-break
     * detection mode 1 */
    p_instance_ctrl->p_reg->ADCJnODCR = p_extend->wiring_break_detect_cfg.odcr; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    r_adcj_mpx_initialize(p_instance_ctrl);

    /* Setting to enable/disable trigger overlap check function */
    p_instance_ctrl->p_reg->ADCJnTOCCR = (uint8_t) p_extend->trigger_overlap_detect; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting target virtual channel for A/D conversion timing monitor v (v = 0 to 4) */
    for (uint32_t timing_monior = 0U; timing_monior < ADCJ_NUMBER_OF_AD_CONVERSION_TIMING_MONITOR; timing_monior++)
    {
        p_instance_ctrl->p_reg->ADCJnADENDP[timing_monior] = /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                                                             (uint8_t) p_extend->ad_conv_timing_monitor.target_vc[
            timing_monior];
    }

    r_adcj_th_initialize(p_instance_ctrl);

    r_adcj_ad_timer_initialize(p_instance_ctrl);

    /* Setting wait time table */
    for (uint32_t wait_setting = 0U; wait_setting < ADCJ_WAIT_SETTING_TABLE_ENTRIES; wait_setting++)
    {
        p_instance_ctrl->p_reg->ADCJnWAITTR[wait_setting] = /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                                                            (uint16_t) p_extend->wait_setting.wait_setting[wait_setting];
    }

    /* Initialize callback related variables */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_callback_memory = NULL;
    p_instance_ctrl->p_context         = p_cfg->p_context;

    /* Setting ADCJ interrupts */
    r_adcj_interrupts_enable(p_instance_ctrl);

    /* Mark this ADCJ instance is opened */
    p_instance_ctrl->opened = ADCJ_OPEN; /* PRQA S 1257 # Rule-10.3 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the ADCJ
 * Implements @ref adc_api_t::close
 * This function halts all ADCJ activities, disables interrupts
 *
 * @retval FSP_SUCCESS                 Module closed.
 * @retval FSP_ERR_ASSERTION           p_ctrl is NULL
 * @retval FSP_ERR_NOT_OPEN            Unit is not open.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_Close (adc_ctrl_t * const p_ctrl)                            /* PRQA S 1505 # Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl; /* PRQA S 0316 # Rule-11.5 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);                                              /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # Rule-20.7 */
#endif

    r_adcj_interrupts_disable(p_instance_ctrl);

    /* Call termination procedure */
    r_adcj_scan_groups_force_termination(p_instance_ctrl);

    p_instance_ctrl->initialized = 0U;
    p_instance_ctrl->opened      = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This function perform scan groups setting for ADCJ after ADCJ is opened with Open API.
 * Implements @ref adc_api_t::scanCfg
 * This function does:
 * - Configure settings for scan group 0 to 4
 *  + Setting start, end virtual channel pointer, scan cycle in multi-cycle scan mode
 *  + Select hardware trigger source for ADCJ0 scan groups
 * - Configure settings for SG-Diag
 *  + Physical channel select
 *  + Conversion type for virtual channels
 *  + End interrupt enable, end pointer select, end interrupt mode select, subtraction type select
 *    upper/lower limit check table register select 0, upper/lower limit check table register select 1, and disable
 *    HW trigger for SG-Diag
 * - Configure settings for PWM-Diag
 *  + Enable/Disable scan group 4 as PWM-Diag
 *  + Disable hardware trigger for PWM-Diag
 * When calling this function, user need to ensure that all scan group of ADCJ is stopped.
 *
 * @retval FSP_SUCCESS                     ADCJ scan groups is initialize successfully
 * @retval FSP_ERR_ASSERTION               The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                Unit is not open
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanCfg (adc_ctrl_t * const p_ctrl, void const * const p_scan_group_cfg) /* PRQA S 1505 # Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;             /* PRQA S 0316 # Rule-11.5 */
    adcj_sg_cfg_t        * p_sgs_cfg       = (adcj_sg_cfg_t *) p_scan_group_cfg;          /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);  /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_scan_group_cfg); /* PRQA S 3432 # Rule-20.7 */

    /* Check if scan group is enable but virtual channel configuration pointer is NULL */
    for (uint8_t scan_group = 0U; scan_group < ADCJ_NUMBER_OF_SG; scan_group++)
    {
        FSP_ASSERT((NULL == p_sgs_cfg->p_sg_cfg[scan_group]) || (NULL != (p_sgs_cfg->p_sg_cfg[scan_group]->p_vc_cfg))); /* PRQA S 3432 # Rule-20.7 */
    }
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                                           /* PRQA S 3432 # Rule-20.7 */
#endif                                                                                                                  /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg = p_instance_ctrl->p_reg;
    p_instance_ctrl->p_sg_cfg = p_sgs_cfg;
    adcj_sg_generic_cfg_t const * p_sgx_cfg = NULL;

    /* Set all virtual channel j (j = 0 to 63) to reset value */
    for (uint32_t channel = 0U; channel < ADCJ_NUMBER_OF_VIRTUAL_CHANNEL; channel++)
    {
        p_reg->ADCJnVCR[channel] = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }

    for (uint32_t scan_group = 0; scan_group < ADCJ_NUMBER_OF_SG; scan_group++)
    {
        p_sgx_cfg = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group];

        /* Check if scan group x (x = 0 to 4) is used */
        if (NULL != p_sgx_cfg)
        {
            /* Setting start and end virtual channel pointers for scan group x */
            *((uint16_t *) ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGVCPR0, scan_group)) =                             /* PRQA S 0310, 0312, 0488, 0662, 3305, 3395, 3469, 4434 # Rule-11.3, Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.3 */
                                                                                 (p_sgx_cfg->start_vc_ptr &  /* PRQA S 1861, 4542 # Rule-10.4, Rule-10.1 */
                                                                                  ADCJ_SGVCPR_VCSP_MASK) |   /* PRQA S 1861, 4532, 4542 # Rule-10.4, Rule-10.1 */
                                                                                 ((p_sgx_cfg->end_vc_ptr &   /* PRQA S 1861, 4542 # Rule-10.4, Rule-10.1 */
                                                                                   ADCJ_SGVCPR_VCEP_MASK) << /* PRQA S 4533 # Rule-10.1 */
                                                                                  ADCJ_SGVCPR_VCEP_POS);     /* PRQA S 1861, 4533, 4542 # Rule-10.4, Rule-10.1 */

            /* Setting scan cycles for scan group in multi-cycle scan mode */
            if (ADCJ_SG_SCAN_MODE_MULTICYLE == p_sgx_cfg->sg_ctrl.sgcr_b.scan_mode)                          /* PRQA S 1881 # Rule-10.4 */
            {
                *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGMCYCR0, scan_group) = p_sgx_cfg->scan_cycle;                /* PRQA S 0312, 0488, 0662, 3395, 3469 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9 */
            }
            else
            {
                /* Clear multi-cycle register for scan group x */
                *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGMCYCR0, scan_group) = 0U; /* PRQA S 0312, 0488, 0662, 3395, 3469 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9 */
            }
        }
        else
        {
            /* Scan group x is not used so reset the scan group control register to reset value */
            *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) = 0U; /* PRQA S 0312, 0488, 0662, 3395, 3469 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9 */
        }

        /* Check if this is ADCJ0, if it is not, NULL is assigned to p_selb_reg in Open API */
        if (NULL != p_instance_ctrl->p_selb_reg)
        {
            /* Setting the scan group hardware trigger source for ADCJ0 */
            p_instance_ctrl->p_selb_reg->ADCJnSGTSEL[scan_group] = p_sgx_cfg->adcj_trigger_src; /* PRQA S 0662, 4424 # Dir-1.1, Rule-1.2, Rule-10.3 */
        }

        /* Set T&H target scan group to scan group x if it is triggered by T&H group A or T&H group B */
        if (ADCJ_SCAN_GROUP_SG1 <= scan_group)                                                  /* PRQA S 1880 # Rule-10.4 */
        {
            /* Check if trigger method for this can group is T&H group A */
            if (ADCJ_SG_TRIGGER_METHOD_THA == p_sgx_cfg->trigger_method)
            {
                /* Set scan group x to be target scan group for T&H group A */
                p_reg->ADCJnTHACR_b.SGS = (scan_group - (uint32_t) ADCJ_SCAN_GROUP_SG1) & ADCJ_THACR_SGS_MASK; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
            }
            else
            {
                /* Because scan group x trigger method is not T&H group A or T&H group B so no need to do anything */
            }
        }
    }

    /* Check if scan group 4 is configure as PWM-Diag */
    if (ADCJ_PWM_DIAG_CFG_ENABLE == p_instance_ctrl->p_sg_cfg->pwm_diag_cfg)
    {
        /* Reset ADCJnSGCR4 to reset value */
        p_reg->ADCJnSGCR4 = 0U;                       /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Reset ADCJnSGVCPR to reset value */
        p_reg->ADCJnSGVCPR4 = 0U;                     /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Set scan cycle count to 1 */
        p_reg->ADCJnSGMCYCR4 = 0U;                    /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Disable PWM-Diag HW trigger */
        p_reg->ADCJnPWDSGCR = 0U;                     /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Enable PWM-Diag */
        p_reg->ADCJnPWDCR = ADCJ_PWM_DIAG_CFG_ENABLE; /* PRQA S 0662, 4424 # Dir-1.1, Rule-1.2, Rule-10.3 */
    }
    else
    {
        /* Disable PWM-Diag */
        p_reg->ADCJnPWDCR = ADCJ_PWM_DIAG_CFG_DISABLE; /* PRQA S 0662, 4424 # Dir-1.1, Rule-1.2, Rule-10.3 */
    }

    /* Set initialized to ADCJ_OPEN to indicate that the scan groups are configured */
    p_instance_ctrl->initialized = ADCJ_OPEN;          /* PRQA S 1257 # Rule-10.3 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::scanStart is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanStart (adc_ctrl_t * const p_ctrl) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::scanStop is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanStop (adc_ctrl_t * const p_ctrl) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief This function starts (in case of a software trigger) or enable hardware trigger the scan group
 * specified by group_mask.
 * Implements @ref adc_api_t::scanGroupStart
 * When calling this function, user need to ensure that the specified scan group is in idle state (user can check
 * the scan group status by @ref R_ADCJ_ScanGroupStatusGet API)
 * Scan groups need to initialized first by @ref adc_api_t::scanCfg before calling this function.
 *
 * @retval FSP_SUCCESS                          Scan group is started successfully.
 * @retval FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                     This ADCJ instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_IN_USE                       Scan group is currently activated or hardware trigger is enabled or
 *                                              T&H hardware trigger is enabled or A/D timer is enabled
 * @retval FSP_ERR_NOT_INITIALIZED              Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval FSP_ERR_ADCJ_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @retval FSP_ERR_TH_STOP                      Scan group use T&H but T&H is disabled by @ref R_ADCJ_ScanGroupStop
 * @return                                      See @ref RENESAS_ERROR_CODES or functions called by this function for
 *                                              other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanGroupStart (adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask) /* PRQA S 1505 # Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;      /* PRQA S 0316 # Rule-11.5 */
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                  /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);             /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED); /* PRQA S 3432 # Rule-20.7 */

    /* Verify group_mask is valid for ADCJ */
    return_val = r_adcj_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);                              /* PRQA S 3432 # Rule-20.7 */
#endif                                                                                    /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg         = p_instance_ctrl->p_reg;
    uint32_t       sg_number     = 0U;
    uint8_t        sg_status     = 0U;
    uint8_t        sg_hw_trigger = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number     = SCH1R(group_mask) - 1;                               /* PRQA S 3469, 4434 # Dir-4.9, Rule-10.3 */
        sg_status     = *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGSR0, sg_number);    /* PRQA S 0311, 0312, 0488, 0662, 3395, 3469 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9 */
        sg_hw_trigger = (*ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, sg_number)) & /* PRQA S 0312, 0488, 0662, 3395, 3469 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9 */
                        (ADCJ_SGCR_HW_TRIGGER_ENABLE | ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE);

        if ((0U == sg_status) && ((0U == sg_hw_trigger) ||
                                  ((ADCJ_SGCR_HW_TRIGGER_ENABLE == sg_hw_trigger) &&
                                   r_adcj_is_th_sw_trigger(p_instance_ctrl, (adcj_scan_group_t) sg_number)))) /* PRQA S 4342 # Rule-10.5 */
        {
            /* Start the scan group */
            return_val = r_adcj_scan_group_start(p_instance_ctrl, (adcj_scan_group_t) sg_number);             /* PRQA S 4342 # Rule-10.5 */
        }
        else
        {
            /* Scan group is active or hardware trigger or A/D timer or track and hold is enabled */
            return_val = FSP_ERR_IN_USE;
        }
    }
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Check if scan group 4 is inactive */
        if ((0U == p_reg->ADCJnSGSR4) && (0U == p_reg->ADCJnPWDSGCR_b.TRGMD)) /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        {
            /* Enable HW trigger for PWM-Diag scan group (SG4) */
            p_reg->ADCJnPWDSGCR = ADCJ_PWDSGCR_HW_TRIGGER_ENABLE;             /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
        else
        {
            return_val = FSP_ERR_IN_USE;
        }
    }
    else
    {
        /* R_ADCJ_ScanGroupStart is called with wrong group_mask */
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function stops the scan group specified by group_mask.
 * Scan groups need to initialized first by @ref adc_api_t::scanCfg before calling this function.
 * If the scan group uses T&H, the T&H units which scan group used are disabled after calling this function.
 * User need to stop all scan groups and re-enable T&H by calling to @ref R_ADCJ_THEnable in order to start
 * this scan group again by @ref R_ADCJ_ScanGroupStart
 *
 * @param[in] p_ctrl                               Pointer to instance control
 * @param[in] group_mask                           Group mask of scan group to stop
 * @retval    FSP_SUCCESS                          Request to stop scan group successfully
 * @retval    FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN                     This ADCJ instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_NOT_INITIALIZED              Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval    FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval    FSP_ERR_ADCJ_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval    FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @return                                         See @ref RENESAS_ERROR_CODES or functions called by this function
 *                                                 for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanGroupStop (adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask) /* PRQA S 1503 # Rule-2.1 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;     /* PRQA S 0316 # Rule-11.5 */
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                  /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);             /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED); /* PRQA S 3432 # Rule-20.7 */

    /* Verify group_mask is valid for ADCJ */
    return_val = r_adcj_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);                              /* PRQA S 3432 # Rule-20.7 */
#endif                                                                                    /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg     = p_instance_ctrl->p_reg;
    uint32_t       sg_number = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number = SCH1R(group_mask) - 1;                                      /* PRQA S 3469, 4434 # Dir-4.9, Rule-10.3 */

        /* Stop the scan group */
        r_adcj_scan_group_stop(p_instance_ctrl, (adcj_scan_group_t) sg_number); /* PRQA S 4342 # Rule-10.5 */
    }
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Disable HW trigger for PWM-Diag */
        p_reg->ADCJnPWDSGCR = 0U;                                                 /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Wait until scan group is stopped */
        FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCJnSGSR4, ADCJ_SGSR_SCAN_GROUP_IDLE); /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }
    else
    {
        /* R_ADCJ_ScanGroupStop is called with wrong group_mask */
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function get the overall status of all scan groups
 * This function return ADC_STATE_SCAN_IN_PROGRESS when one of scan group 0 to 4, SG-Diag is activated.
 * Implements @ref adc_api_t::scanStatusGet
 *
 * @retval FSP_SUCCESS              Scan groups status is get successfully
 * @retval FSP_ERR_ASSERTION        The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN         This ADCJ instance is not open by @ref adc_api_t::open
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanStatusGet (adc_ctrl_t * const p_ctrl, adc_status_t * p_status) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;       /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                      /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_status);                                             /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # Rule-20.7 */
#endif                                                                        /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg = p_instance_ctrl->p_reg;                            /* PRQA S 3678 # Rule-8.13 */

    /* Check if any scan group is active */
    if (0U != (p_reg->ADCJnSGSTR & ADCJ_SGSTR_SGS_STATUS_MASK))               /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        p_status->state = ADC_STATE_SCAN_IN_PROGRESS;
    }
    else
    {
        p_status->state = ADC_STATE_IDLE;
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This function get the status of scan group specified by group_mask.
 * @param[in]  p_ctrl                               Pointer to ADCJ instance control
 * @param[in]  group_mask                           Group mask of the scan group to get status
 * @param[out] p_status                             Pointer to variable to store the scan group status
 * @retval     FSP_SUCCESS                          Scan group status is get successfully
 * @retval     FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval     FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval     FSP_ERR_ADCJ_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval     FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @retval     FSP_ERR_NOT_OPEN                     This ADCJ instance is not open by @ref adc_api_t::open
 * @return                                          See @ref RENESAS_ERROR_CODES or functions called by this function
 *                                                  for other possible return codes
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_ScanGroupStatusGet (adc_ctrl_t * const       p_ctrl,         /* PRQA S 3673, 1503 # Rule-8.13, Rule-2.1 */
                                     adc_group_mask_t const   group_mask,
                                     adcj_sg_status_t * const p_status)
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                      /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_status);                                             /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # Rule-20.7 */

    /* Verify group_mask is valid for ADCJ */
    return_val = r_adcj_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);                  /* PRQA S 3432 # Rule-20.7 */
#endif                                                                        /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg     = p_instance_ctrl->p_reg;                        /* PRQA S 3678 # Rule-8.13 */
    uint32_t       sg_number = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number = SCH1R(group_mask) - 1;                                                                         /* PRQA S 3469, 4434 # Dir-4.9, Rule-10.3 */

        /* Get status of scan group */
        *p_status =
            (adcj_sg_status_t) (*ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGSR0, sg_number) >> R_ADCJ1_ADCJnSGSR0_SGACT_Pos); /* PRQA S 0311, 0312, 0488, 0662, 3395, 3469, 4342, 4394 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.5, Rule-10.8 */
    }
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Scan group 4 and PWM-Diag use the same status register */
        *p_status = (adcj_sg_status_t) p_reg->ADCJnSGSR4_b.SGACT; /* PRQA S 0662, 4342 # Dir-1.1, Rule-1.2, Rule-10.5 */
    }
    else
    {
        /* R_ADCJ_ScanGroupStatusGet is called with wrong group_mask */
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function read the data register of the virtual channel specified by reg_id.
 * Implements @ref adc_api_t::read
 * When the read and clear function is enabled, consider using @ref adc_api_t::read32 instead to read two consecutive
 * virtual channels
 * @retval FSP_SUCCESS                          The read operation is performed successfully
 * @retval FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                     This ADCJ instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_ADCJ_CHANNEL_NOT_SUPPORT     This reg_id is not supported
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_Read (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint16_t * const p_data) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;                          /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                         /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_data);                                                                  /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                    /* PRQA S 3432 # Rule-20.7 */

    /* Check reg_id input valid */
    FSP_ERROR_RETURN(reg_id < ADCJ_NUMBER_OF_VIRTUAL_CHANNEL, FSP_ERR_ADCJ_CHANNEL_NOT_SUPPORT); /* PRQA S 1880, 3432 # Rule-10.4, Rule-20.7 */
#endif                                                                                           /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg = p_instance_ctrl->p_reg;                                               /* PRQA S 3678 # Rule-8.13 */

    /* Write data of channel reg_id into p_data*/
    *p_data = p_reg->ADCJnDR[reg_id];                                                            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Read the data register of the virtual channels specified by reg_id.
 * Implements @ref adc_api_t::read32
 * When reg_id is even, this function read the data of virtual channels specified by reg_id and reg_id + 1 and
 * store it in 32 bits of p_data.
 * When reg_id is odd, this function read the data of reg_id and store it in the lower 16 bits of p_data.
 * @retval FSP_SUCCESS                          The read operation is performed successfully
 * @retval FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                     This ADCJ instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_ADCJ_CHANNEL_NOT_SUPPORT     This reg_id is not supported
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_Read32 (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint32_t * const p_data) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;                            /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                         /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_data);                                                                  /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                    /* PRQA S 3432 # Rule-20.7 */

    /* Check reg_id input valid */
    FSP_ERROR_RETURN(reg_id < ADCJ_NUMBER_OF_VIRTUAL_CHANNEL, FSP_ERR_ADCJ_CHANNEL_NOT_SUPPORT); /* PRQA S 1880, 3432 # Rule-10.4, Rule-20.7 */
#endif                                                                                           /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg = p_instance_ctrl->p_reg;                                               /* PRQA S 3678 # Rule-8.13 */

    /* Check if number of channel is even */
    if (0U == ((uint32_t) reg_id) % 2U)                                                          /* PRQA S 3397 # Rule-12.1 */
    {
        /* Write data of channels reg_id and reg_id + 1 into p_data */
        *p_data = *(uint32_t *) (&p_reg->ADCJnDR[reg_id]);                                       /* PRQA S 0310, 0311, 0312, 0662, 3305 # Rule-11.3, Rule-11.8, Dir-1.1, Rule-1.2 */
    }
    else
    {
        /* Write data of channel reg_id into p_data */
        *p_data = (uint32_t) (p_reg->ADCJnDR[reg_id]); /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read PWM-Diag data register to p_data
 *
 * @param[in] p_ctrl                       Pointer to ADCJ instance control
 * @param[in] p_data                       Pointer to variable to store read data
 * @retval    FSP_SUCCESS                  The read operation is performed successfully.
 * @retval    FSP_ERR_ASSERTION            p_ctrl or p_data is NULL.
 * @retval    FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval    FSP_ERR_NOT_INITIALIZED      Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval    FSP_ERR_PWM_DIAG_DISABLED    PWM-Diag is disabled.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_PWMDiagRead (adc_ctrl_t * const p_ctrl, uint16_t * const p_data) /* PRQA S 3673, 1503 # Rule-8.13, Rule-2.1 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;     /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                                     /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_data);                                                              /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED);    /* PRQA S 3432 # Rule-20.7 */

    /* Check if PWM-Diag is enable */
    adcj_pwm_diag_cfg_t p_pwm_diag_cfg = p_instance_ctrl->p_sg_cfg->pwm_diag_cfg;
    FSP_ERROR_RETURN(ADCJ_PWM_DIAG_CFG_ENABLE == p_pwm_diag_cfg, FSP_ERR_PWM_DIAG_DISABLED); /* PRQA S 3432 # Rule-20.7 */
#endif                                                                                       /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg = p_instance_ctrl->p_reg;                                           /* PRQA S 3678 # Rule-8.13 */

    *p_data = p_reg->ADCJnPWDDR;                                                             /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This function is not supported on this release.
 *
 * @param[in]  p_ctrl                       Pointer to ADCJ instance control
 * @param[out] p_data                       Pointer to variable to store read data
 * @retval     FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_SGDiagRead (adc_ctrl_t * const p_ctrl, uint16_t * const p_data) /* PRQA S 3673, 1503 # Rule-8.13, Rule-2.1 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_data);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief Updates the user callback and has option of providing memory for callback structure.
 * Implements adc_api_t::callbackSet
 *
 * @retval  FSP_SUCCESS                  Callback updated successfully.
 * @retval  FSP_ERR_ASSERTION            p_ctrl or p_callback is NULL.
 * @retval  FSP_ERR_NOT_OPEN             The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_CallbackSet (adc_ctrl_t * const          p_ctrl,                             /* PRQA S 1505 # Rule-8.7 */
                              void (                    * p_callback)(adc_callback_args_t *), /* PRQA S 1336 # Rule-8.2 */
                              void * const                p_context,
                              adc_callback_args_t * const p_callback_memory)
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;                 /* PRQA S 0316 # Rule-11.5 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);                                                              /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(p_callback);                                                                   /* PRQA S 3344, 3432 # Rule-14.4, Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                 /* PRQA S 3432 # Rule-20.7 */
#endif

    /* Update variables related to callback */
    p_instance_ctrl->p_callback        = p_callback;
    p_instance_ctrl->p_context         = p_context;
    p_instance_ctrl->p_callback_memory = p_callback_memory;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Returns the address of the lowest number configured virtual channel and the total number of bytes to be read
 * of each scan group in order for the DTSC/DMAC to read the conversion results of configured channels. If no channels
 * are configured, then a length of 0 is returned.
 * Implemets @ref adc_api_t::infoGet.
 *
 * @retval FSP_SUCCESS                 Information stored in p_adc_info.
 * @retval FSP_ERR_ASSERTION           An input argument is invalid.
 * @retval FSP_ERR_NOT_OPEN            Unit is not open.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_InfoGet (adc_ctrl_t * const p_ctrl, adc_info_t * const p_adc_info)  /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    adcj_instance_ctrl_t        * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl; /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */
    adcj_sg_generic_cfg_t const * p_sgx_cfg       = NULL;
    fsp_err_t err = FSP_SUCCESS;

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);                                      /* PRQA S 3432 # Rule-20.7 */
    FSP_ASSERT(NULL != p_adc_info);                                           /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # Rule-20.7 */
#endif

    /* Loop through each scan group to search for the lowest and number of consecutive virtual channels */
    for (uint32_t scan_group = 0; scan_group < ADCJ_NUMBER_OF_SG; scan_group++)
    {
        /* p_instance_ctrl->p_sg_cfg is set within ScanCfg */
        p_sgx_cfg = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group];
        if (NULL != p_sgx_cfg)
        {
            /* Determine the lowest virtual channel that is configured */
            p_adc_info->p_address[scan_group] = &p_instance_ctrl->p_reg->ADCJnDR[p_sgx_cfg->start_vc_ptr];       /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

            /* Determine the number of virtual channels that is configured consecutively */
            p_adc_info->length[scan_group] = (uint32_t) ((p_sgx_cfg->end_vc_ptr - p_sgx_cfg->start_vc_ptr) + 1); /* PRQA S 1860, 4393 # Rule-10.4, Rule-10.8 */
        }
        else
        {
            p_adc_info->p_address[scan_group] = NULL;
            p_adc_info->length[scan_group]    = 0U;
        }
    }

    return err;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::calibrate is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_Calibrate (adc_ctrl_t * const p_ctrl, void const * p_extend) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(p_extend);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::offsetSet is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_OffsetSet (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, int32_t const offset) /* PRQA S 3673, 1505 # Rule-8.13, Rule-8.7 */
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(reg_id);
    FSP_PARAMETER_NOT_USED(offset);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief Enable the T&H units after it is stopped by @ref R_ADCJ_ScanGroupStop
 * After user use @ref R_ADCJ_ScanGroupStop to stop T&H units after stop a scan group, user need to use this function
 * to re-enable T&H units before calling @ref R_ADCJ_ScanGroupStart to start the scan group again.
 * This function must be called when all scan groups are stopped
 * @param[in] p_ctrl                         Pointer to ADCJ control block
 * @retval    FSP_SUCCESS                    T&H units is enabled successfully
 * @retval    FSP_ERR_ASSERTION              The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN               This ADCJ instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_SG_AD_TIMER_ACTIVE     Any scan group or A/D timer of any scan group is active
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_THEnable (adc_ctrl_t * const p_ctrl)                                               /* PRQA S 1503 # Rule-2.1 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;                       /* PRQA S 0316 # Rule-11.5 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);                                                                    /* PRQA S 3432 # Rule-20.7 */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);                       /* PRQA S 3432 # Rule-20.7 */
#endif

    R_ADCJ1_Type        * p_reg      = p_instance_ctrl->p_reg;
    adcj_extended_cfg_t * p_extend   = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;    /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */
    fsp_err_t             return_val = FSP_SUCCESS;

    /* Check if all scan groups and A/D timer are in idle state */
    if (0U == (p_reg->ADCJnSGSTR & (ADCJ_SGSTR_SGS_STATUS_MASK | ADCJ_SGSTR_AD_TIMER_STATUS_MASK))) /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        /* Disable T&H triggers */
        p_reg->ADCJnTHACR_b.HLDTE = 0U;                                                             /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Stop sampling of all T&H units */
        p_reg->ADCJnTHSTPCR = ADCJ_THSTPCR_SAMPLING_STOP;                                           /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Setting to enable/disable individual T&H unit */
        p_reg->ADCJnTHER = (uint8_t) p_extend->th_cfg.th_enable_cfg.ther;                           /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Start all T&H units sampling */
        p_reg->ADCJnTHSMPSTCR = ADCJ_THSMPSTCR_SAMPLING_START;                                      /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Update status of T&H group A and T&H group B */
        p_instance_ctrl->th_state[ADCJ_TH_GROUP_A] = ADCJ_TH_STATE_SAMPLING;
        p_instance_ctrl->th_state[ADCJ_TH_GROUP_B] = ADCJ_TH_STATE_SAMPLING;

        /*  After start T&H sampling, a wait time (about 30 CLKAD cycles ) is necessary
         *  before start a hold trigger.
         *  CLKAD (A/D conversion clock) and PCLK (Register access clock) clock source are the same (CLK_ADC0).
         *  So by read ADCJnSGSTR 15 times, we get the minimum time delay required for T&H sampling */
        for (uint32_t cnt = 0U; cnt < ADCJ_TH_SAMPLING_DELAY_CLKAD_CYCLE / ADCJ_REG_ACCESS_TIME_CLKAD_CYCLE; cnt++) /* PRQA S 3397 # Rule-12.1 */
        {
            volatile uint16_t sgstr = p_reg->ADCJnSGSTR;                                                            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
            (void) sgstr;
        }
    }
    else
    {
        /* Any scan group or A/D timer is in active state so can not enable T&H group sampling */
        return_val = FSP_ERR_SG_AD_TIMER_ACTIVE;
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief Change the voltage level in pin-level self-diagnosis
 * This function is used only when T&H path self-diagnosis is enabled or all scan groups are stopped.
 *
 * @param[in] p_ctrl                    Pointer to ADCJ instance control
 * @param[in] voltage_level             Pin-level self-diagnostic voltage level to set
 * @retval    FSP_SUCCESS               The voltage level is changed successfully
 * @retval    FSP_ERR_ASSERTION         The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN          This ADCJ instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_IN_USE            T&H path self-diagnosis is disabled and any scan group is on-going.
 **********************************************************************************************************************/
fsp_err_t R_ADCJ_PinLVSelfDiagVoltageChange (adc_ctrl_t * const p_ctrl, const adcj_pin_lv_self_diag_vol_t voltage_level) /* PRQA S 3673, 1503 # Rule-8.13, Rule-2.1 */
{
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) p_ctrl;                                            /* PRQA S 0316, 3678 # Rule-11.5, Rule-8.13 */

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);                                      /* PRQA S 3432 # Rule-20.7 */

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCJ_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN); /* PRQA S 3432 # Rule-20.7 */
#endif                                                                        /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

    R_ADCJ1_Type * p_reg      = p_instance_ctrl->p_reg;
    fsp_err_t      return_val = FSP_SUCCESS;

    if ((ADCJ_TH_PATH_SELF_DIAG_ENABLE == p_reg->ADCJnTDCR_b.THSDE) || /* PRQA S 0662, 1881 # Dir-1.1, Rule-1.2, Rule-10.4 */
        (0U == (p_reg->ADCJnSGSTR & ADCJ_SGSTR_SGS_STATUS_MASK)))      /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        p_reg->ADCJnTDCR_b.TDLV = voltage_level;                       /* PRQA S 0662, 4424 # Dir-1.1, Rule-1.2, Rule-10.3 */
    }
    else
    {
        return_val = FSP_ERR_IN_USE;
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ADCJ)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief This function is used at beginning of Open or Close to stop the ADC activities.
 * This function process the force termination procedure of all scan groups:
 * - Disable hold trigger of T&H group A and B.
 * - Clear all hardware trigger sources of all scan groups.
 * - Force termination of all scan group by writing 1 to ADHALTR.HALT bit.
 * - Stop sampling of all T&H units.
 * - Disable all T&H units.
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control block
 **********************************************************************************************************************/
static void r_adcj_scan_groups_force_termination (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    /* Disable T&H triggers */
    p_instance_ctrl->p_reg->ADCJnTHACR_b.HLDTE = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Disable Hardware trigger of all scan groups */
    p_instance_ctrl->p_reg->ADCJnSGCR0_b.TRGMD = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    p_instance_ctrl->p_reg->ADCJnSGCR1_b.TRGMD = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    p_instance_ctrl->p_reg->ADCJnSGCR2_b.TRGMD = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Clear hardware trigger and A/D timer trigger of scan group 3 and scan group 4 */
    p_instance_ctrl->p_reg->ADCJnSGCR3 &=            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                                          (uint8_t) (~(ADCJ_SGCR_HW_TRIGGER_ENABLE |
                                                       ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE));
    p_instance_ctrl->p_reg->ADCJnSGCR4 &=            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                                          (uint8_t) (~(ADCJ_SGCR_HW_TRIGGER_ENABLE |
                                                       ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE));

    /* Disable Hardware trigger of PWM-Diag */
    p_instance_ctrl->p_reg->ADCJnPWDSGCR = 0U;                         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Kill all scan groups, clear the information of held physical number of SG-Diag */
    p_instance_ctrl->p_reg->ADCJnADHALTR = ADCJ_HALTR_HALT;            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Stop sampling of all T&H units */
    p_instance_ctrl->p_reg->ADCJnTHSTPCR = ADCJ_THSTPCR_SAMPLING_STOP; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Disable all T&H units */
    p_instance_ctrl->p_reg->ADCJnTHER = 0U;                            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Update T&H group state */
    p_instance_ctrl->th_state[ADCJ_TH_GROUP_A] = ADCJ_TH_STATE_STOP;
    p_instance_ctrl->th_state[ADCJ_TH_GROUP_B] = ADCJ_TH_STATE_STOP;
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to initialize the upper/lower limit check
 * This function does:
 * - Setting the upper/lower limit table
 * - Clear all status bits of virtual channels (scan group, PWM-Diag, SG-Diag) upper/lower limit check
 * - Clear all scan groups x (x = 0 to 4), SG-Diag upper/lower limit check status bits
 * - Setting upper/lower limit check enable for individual virtual channel j
 * - Setting upper/lower limit check enable for PWM-Diag
 * - Setting upper/lower limit check enable for SG-Diag
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control block
 **********************************************************************************************************************/
static void r_adcj_ul_limit_check_initialize (adcj_instance_ctrl_t * const p_instance_ctrl)    /* PRQA S 3673 # Rule-8.13 */
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* Setup upper/lower limit check table (from 0 to 7) */
    for (uint32_t limit_entry = 0U; limit_entry < ADCJ_UPPER_LOWER_LIMIT_TABLE_ENTRIES; limit_entry++)
    {
        p_instance_ctrl->p_reg->ADCJnVCULLMTBR[limit_entry] = /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                                                              p_extend->ul_limit_cfg.ul_limit_tb[limit_entry].vcullmtbr;
    }

    /* Clear all status bits of virtual channels (scan group, PWM-Diag, SG-Diag) upper/lower limit check */
    p_instance_ctrl->p_reg->ADCJnVCLMASCR = ADCJ_VCLMASCR_ALLMSC;                               /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Clear all scan groups x (x = 0 to 4), SG-Diag upper/lower limit check status bits */
    p_instance_ctrl->p_reg->ADCJnSGULCCR = ADCJ_SGULCCR_CLEAR_ALL;                              /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Enable upper/lower limit check for individual virtual channel j */
    p_instance_ctrl->p_reg->ADCJnVCLMINTER1 = p_extend->ul_limit_cfg.vc_ul_limit_int_0;         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    p_instance_ctrl->p_reg->ADCJnVCLMINTER2 = p_extend->ul_limit_cfg.vc_ul_limit_int_1;         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Enable upper/lower limit check for PWM-Diag */
    p_instance_ctrl->p_reg->ADCJnPWVCLMINTER = p_extend->ul_limit_cfg.pwm_diag_ul_limit_int;    /* PRQA S 0662, 4424 # Dir-1.1, Rule-1.2, Rule-10.3 */
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup MPX for ADCJ
 * This function does:
 * - Setting mask control format for MPX in I/O port output mode (using DMAC to transfer ADCJnMPXCURR1 to PSRn)
 * - Setting SPI communication command for MPX using SPI
 * - Setting to enable/disable MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control blockss
 **********************************************************************************************************************/
static void r_adcj_mpx_initialize (adcj_instance_ctrl_t * const p_instance_ctrl)                /* PRQA S 3673 # Rule-8.13 */
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;  /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* Setting mask control format for MPX in I/O port output mode */
    p_instance_ctrl->p_reg->ADCJnMPXCURCR = (uint8_t) p_extend->mpx_cfg.mask_control_format;    /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting SPI communication command for MPX using SPI output mode */
    p_instance_ctrl->p_reg->ADCJnMPXCMDR = (uint8_t) p_extend->mpx_cfg.spi_command_information; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting to enable/disable MPX interrupt */
    p_instance_ctrl->p_reg->ADCJnMPXINTER = (uint8_t) p_extend->mpx_cfg.mpx_int;                /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup T&H units for ADCJ
 * After calling to r_adcj_scan_groups_force_termination, all T&H groups are stopped, T&H groups hardware
 * trigger is disabled, and all T&H units is disabled.
 * This function does:
 * - Reset T&H group A, B control register to their reset values
 * - When T&H is used,
 *  + Enable automatic sampling function for T&H units
 *  + Select T&H groups for each T&H units
 *  + Enable or disable T&H units
 *  + Start sampling of all T&H units
 * - When T&H is not used, this function does:
 *  + Clear T&H group select for all T&H units
 * The T&H group A and T&H group B scan group selection is performed by ScanGroupCfg API
 * This function shall only be called when all scan groups are stopped
 * @param [in] p_instance_ctrl Pointer to this ADCJ instance control block
 **********************************************************************************************************************/
static void r_adcj_th_initialize (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* Reset T&H group A, B control register to their reset value */
    p_instance_ctrl->p_reg->ADCJnTHACR = 0U;                                                   /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Check if T&H is used */
    if (0U != p_extend->th_cfg.th_enable_cfg.ther)
    {
        /* PRQA S 0288 ++ # DP_MSR_D-1.1_UC1 */

        /* When T&H is used, it is recommended to set 0 to ADCJnTHCR.ASMPMSK (automatic sampling is effective)
         * Reference cautions 1 of section 1.2.5.10 "ADCJnTHSMPSTCR — T&H Sampling Start Control Register"
         * in the ADCJ IP manual r01uh1122ej0040 Rev .0 .40 */

        /* PRQA S 0288 -- */

        p_instance_ctrl->p_reg->ADCJnTHCR = 0U;                                            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Setting to enable/disable individual T&H unit */
        p_instance_ctrl->p_reg->ADCJnTHER = (uint8_t) p_extend->th_cfg.th_enable_cfg.ther; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Start all T&H units sampling */
        p_instance_ctrl->p_reg->ADCJnTHSMPSTCR = ADCJ_THSMPSTCR_SAMPLING_START;            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Update status of T&H group A and T&H group B */
        p_instance_ctrl->th_state[ADCJ_TH_GROUP_A] = ADCJ_TH_STATE_SAMPLING;
        p_instance_ctrl->th_state[ADCJ_TH_GROUP_B] = ADCJ_TH_STATE_SAMPLING;

        /* After start T&H sampling, a wait time (about 30 CLKAD cycles ) is necessary
         * before start a hold trigger. But to start a hold trigger, user need to call ScanCfg API
         * and the overhead of that function is enough to satisfy the wait time requirement for T&H sampling time */
    }
    else
    {
        /* Do Nothing */
    }
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup AD timer trigger for ADCJ
 * This function does:
 * - Setting AD timer x (x = 3 to 4) initial phase
 * - Setting AD timer x (x = 3 to 4) cycle time
 * After calling to r_adcj_scan_groups_force_termination, the AD timers is stopped and
 * AD timers hardware trigger are disabled so there is no need to disable it again in this function
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control blocks
 **********************************************************************************************************************/
static void r_adcj_ad_timer_initialize (adcj_instance_ctrl_t * const p_instance_ctrl)                         /* PRQA S 3673 # Rule-8.13 */
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;                /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* Setting AD timer x (x = 3 to 4) initial phase */
    p_instance_ctrl->p_reg->ADCJnADTIPR3 = p_extend->ad_timer_cfg.initial_phase[ADCJ_SG3_AD_TIMER_CFG_INDEX]; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    p_instance_ctrl->p_reg->ADCJnADTIPR4 = p_extend->ad_timer_cfg.initial_phase[ADCJ_SG4_AD_TIMER_CFG_INDEX]; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Setting AD timer x (x = 3 to 4) cycle time */
    p_instance_ctrl->p_reg->ADCJnADTPRR3 = p_extend->ad_timer_cfg.cycle[ADCJ_SG3_AD_TIMER_CFG_INDEX];         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    p_instance_ctrl->p_reg->ADCJnADTPRR4 = p_extend->ad_timer_cfg.cycle[ADCJ_SG4_AD_TIMER_CFG_INDEX];         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup ADCJ interrupts
 * This function setting ADCJ interrupts:
 * - Error interrupt (ID error, overwrite error, trigger overlap error, upper/lower limit check error).
 * - Scan group 0 end interrupt
 * - Scan group 1 end interrupt
 * - Scan group 2 end interrupt
 * - Scan group 3 end interrupt
 * - Scan group 4 end interrupt
 * - Scan group SG-Diag end interrupt
 * - MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control block
 **********************************************************************************************************************/
static void r_adcj_interrupts_enable (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* ADCJ error interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.error_irq, p_extend->int_cfg.error_ipl, p_instance_ctrl);

    /* ADCJ scan group 0 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_0_end_irq, p_extend->int_cfg.scan_group_0_end_ipl, p_instance_ctrl);

    /* ADCJ scan group 1 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_1_end_irq, p_extend->int_cfg.scan_group_1_end_ipl, p_instance_ctrl);

    /* ADCJ scan group 2 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_2_end_irq, p_extend->int_cfg.scan_group_2_end_ipl, p_instance_ctrl);

    /* ADCJ scan group 3 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_3_end_irq, p_extend->int_cfg.scan_group_3_end_ipl, p_instance_ctrl);

    /* ADCJ scan group 4 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_4_end_irq, p_extend->int_cfg.scan_group_4_end_ipl, p_instance_ctrl);
}

/*******************************************************************************************************************//**
 * @brief This function is called in Close API to disable ADCJ interrupts
 * This function disable ADCJ interrupts:
 * - Error interrupt (ID error, overwrite error, trigger overlap error, upper/lower limit check error).
 * - Scan group 0 end interrupt
 * - Scan group 1 end interrupt
 * - Scan group 2 end interrupt
 * - Scan group 3 end interrupt
 * - Scan group 4 end interrupt
 * - Scan group SG-Diag end interrupt
 * - MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCJ instance control block
 **********************************************************************************************************************/
static void r_adcj_interrupts_disable (adcj_instance_ctrl_t * const p_instance_ctrl)           /* PRQA S 3673 # Rule-8.13 */
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */

    /* ADCJ error interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.error_irq);

    /* ADCJ scan group 0 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_0_end_irq);

    /* ADCJ scan group 1 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_1_end_irq);

    /* ADCJ scan group 2 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_2_end_irq);

    /* ADCJ scan group 3 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_3_end_irq);

    /* ADCJ scan group 4 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_4_end_irq);
}

/*******************************************************************************************************************//**
 * @brief This function start the scan group specified by scan_group
 *
 * @param[in]   p_instance_ctrl  Pointer to control block of this ADCJ instance
 * @param[in]   scan_group       Scan group need to start
 * @retval      FSP_SUCCESS      Scan group is started successfully
 * @retval      FSP_ERR_TH_STOP  T&H units of scan group is stopped. Use @ref R_ADCJ_THEnable to re-enable it.
 **********************************************************************************************************************/
static fsp_err_t r_adcj_scan_group_start (adcj_instance_ctrl_t * const p_instance_ctrl,          /* PRQA S 3673 # Rule-8.13 */
                                          adcj_scan_group_t const      scan_group)
{
    R_ADCJ1_Type        * p_reg      = p_instance_ctrl->p_reg;
    adcj_extended_cfg_t * p_extend   = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */
    adcj_sg_cfg_t const * p_sg_cfg   = p_instance_ctrl->p_sg_cfg;
    fsp_err_t             return_val = FSP_SUCCESS;

    /* Setup virtual virtual channel for scan group (enable virtual channel end interrupt) */
    r_adcj_scan_group_vc_init(p_reg, p_sg_cfg->p_sg_cfg[scan_group]);

    /* Disable hardware trigger, AD timer trigger, setting scan end interrupt enable and scan mode
     * for scan group */
    *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) = p_sg_cfg->p_sg_cfg[scan_group]->sg_ctrl.sgcr; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */

    /* Get scan group trigger method */
    adcj_sg_trigger_method_t sgx_trigger_method = p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;
    switch (sgx_trigger_method)
    {
        case ADCJ_SG_TRIGGER_METHOD_SW:
        {
            /* Start scan group */
            *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGSTCR0, scan_group) = ADCJ_SGSTCR_SW_START; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */
            break;
        }

        case ADCJ_SG_TRIGGER_METHOD_HW:
        {
            /* Enable hardware trigger for scan group */
            *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) |= ADCJ_SGCR_HW_TRIGGER_ENABLE; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */
            break;
        }

        case ADCJ_SG_TRIGGER_METHOD_AD_TIMER:
        {
            if (ADCJ_SCAN_GROUP_SG3 == scan_group)
            {
                if (ADCJ_AD_TIMER_TRIGGER_METHOD_SW ==
                    p_extend->ad_timer_cfg.trigger_method[ADCJ_SG3_AD_TIMER_CFG_INDEX])
                {
                    /* Enable scan group 3 to be trigger by A/D timer */
                    p_reg->ADCJnSGCR3 |= ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

                    /* Start A/D timer for scan group 3 */
                    p_reg->ADCJnADTSTCR3 = ADCJ_ADTSTCR_AD_TIMER_SW_START;  /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                }
                else
                {
                    /* Enable scan group 4 to be trigger by A/D timer and A/D timer is trigger by HW */
                    p_reg->ADCJnSGCR3 |= ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE | ADCJ_SGCR_HW_TRIGGER_ENABLE; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                }
            }
            else if (ADCJ_SCAN_GROUP_SG4 == scan_group)
            {
                if (ADCJ_AD_TIMER_TRIGGER_METHOD_SW ==
                    p_extend->ad_timer_cfg.trigger_method[ADCJ_SG4_AD_TIMER_CFG_INDEX])
                {
                    /* Enable scan group to be trigger by A/D timer */
                    p_reg->ADCJnSGCR4 |= ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

                    /* Start A/D timer for scan group 4 */
                    p_reg->ADCJnADTSTCR4 = ADCJ_ADTSTCR_AD_TIMER_SW_START;  /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                }
                else
                {
                    /* Enable scan group 4 to be trigger by A/D timer and A/D timer is trigger by HW */
                    p_reg->ADCJnSGCR4 |= ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE | ADCJ_SGCR_HW_TRIGGER_ENABLE; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                }
            }
            else
            {
                /* Should not run in to this case because only scan group 3 and scan group 4 support A/D timer */
            }

            break;
        }

        case ADCJ_SG_TRIGGER_METHOD_THA:
        {
            /* Check if T&H group is in sampling state */
            if (ADCJ_TH_STATE_SAMPLING == p_instance_ctrl->th_state[ADCJ_TH_GROUP_A])
            {
                /* Check if T&H group A target scan group is scan_group */
                if (p_reg->ADCJnTHACR_b.SGS + ADCJ_SCAN_GROUP_SG1 == scan_group) /* PRQA S 0662, 1881, 3397, 4521 # Dir-1.1, Rule-1.2, Rule-10.4, Rule-12.1, Rule-10.1 */
                {
                    if (ADCJ_TH_GROUP_TRIGGER_METHOD_SW == p_extend->th_cfg.trigger_method[ADCJ_TH_GROUP_A])
                    {
                        /* Enable hold control for T&H group A */
                        p_reg->ADCJnTHACR_b.HLDCTE = ADCJ_THACR_HOLD_CTRL_ENABLE;                         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

                        /* Enable hardware trigger for scan group */
                        *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) |= ADCJ_SGCR_HW_TRIGGER_ENABLE; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */

                        /* Start T&H group A by software */
                        p_reg->ADCJnTHAHLDSTCR = ADCJ_THAHLDSTCR_HOLD_START;                              /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                    }
                    else
                    {
                        /* Enable hold control for T&H group A */
                        p_reg->ADCJnTHACR_b.HLDCTE = ADCJ_THACR_HOLD_CTRL_ENABLE;                         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

                        /* Enable hardware trigger for scan group */
                        *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) |= ADCJ_SGCR_HW_TRIGGER_ENABLE; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */

                        /* T&H group A is triggered by HW trigger */
                        p_reg->ADCJnTHACR_b.HLDTE = ADCJ_THACR_HW_TRIGGER_ENABLE;                         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
                    }
                }
            }
            else
            {
                return_val = FSP_ERR_TH_STOP;
            }

            break;
        }

        default:
        {
            /* Should not run in to this case */
            break;
        }
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function is called by @ref R_ADCJ_ScanGroupStart to setup virtual channel settings for scan group
 *
 * @param[in]   p_reg            Pointer to registers of this ADCJ instance
 * @param[in]   p_sg_cfg         Pointer to scan group configuration
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_scan_group_vc_init (R_ADCJ1_Type * const p_reg, adcj_sg_generic_cfg_t const * p_sg_cfg)
{
    /* Setting virtual channels for scan group */
    for (uint32_t channel = p_sg_cfg->start_vc_ptr, cnt = 0U; channel <= p_sg_cfg->end_vc_ptr; channel++, cnt++) /* PRQA S 2462, 2463, 3387, 3418 # Rule-14.2, Rule-13.3, Rule-12.3 */
    {
        p_reg->ADCJnVCR[channel] = p_sg_cfg->p_vc_cfg[cnt].vcr;                                                  /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }
}

/*******************************************************************************************************************//**
 * @brief Stop the scan group specified by scan_group
 *
 * @param[in]  p_instance_ctrl  Pointer to this ADCJ instance control
 * @param[in]  scan_group       Scan group to stop
 **********************************************************************************************************************/
static void r_adcj_scan_group_stop (adcj_instance_ctrl_t * const p_instance_ctrl, adcj_scan_group_t const scan_group)
{
    R_ADCJ1_Type        * p_reg    = p_instance_ctrl->p_reg;
    adcj_sg_cfg_t const * p_sg_cfg = p_instance_ctrl->p_sg_cfg;

    /* Get scan group x trigger method */
    adcj_sg_trigger_method_t sgx_trigger_method = p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;

    /* Disable T&H hold trigger if trigger method of scan_group is T&H */
    if (ADCJ_SG_TRIGGER_METHOD_THA == sgx_trigger_method)
    {
        /* Disable hold trigger for T&H group A */
        p_reg->ADCJnTHACR_b.HLDTE = 0U; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }
    else
    {
        /* scan_group trigger method is not T&H so no need to do anything */
    }

    /* Disable HW trigger and A/D timer trigger (for scan group 3 and 4) and scan end interrupt */
    *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGCR0, scan_group) &= (uint8_t) (~(ADCJ_SGCR_HW_TRIGGER_ENABLE | /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */
                                                                      ADCJ_SGCR_AD_TIMER_TRIGGER_ENABLE |
                                                                      ADCJ_SGCR_SCAN_END_INT_MASK));

    /* Check if AD timer is used for scan group */
    if (ADCJ_SG_TRIGGER_METHOD_AD_TIMER == sgx_trigger_method)
    {
        /* Request to stop AD timer */
        if (ADCJ_SCAN_GROUP_SG3 == scan_group)
        {
            p_reg->ADCJnADTENDCR3 = ADCJ_ADTENDCR_AD_TIMER_STOP; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
        else if (ADCJ_SCAN_GROUP_SG4 == scan_group)
        {
            p_reg->ADCJnADTENDCR4 = ADCJ_ADTENDCR_AD_TIMER_STOP; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
        else
        {
            /* Should not run into this case */
        }
    }

    /* Disable virtual channels end interrupt of scan group x */
    r_adcj_scan_group_vc_deinit(p_reg, p_sg_cfg->p_sg_cfg[scan_group]);

    /* Request to stop scan group */
    *ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGSTPCR0, scan_group) = ADCJ_SGSTPCR_SG_STOP; /* PRQA S 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */

    if (ADCJ_SG_TRIGGER_METHOD_AD_TIMER == sgx_trigger_method)
    {
        if (ADCJ_SCAN_GROUP_SG3 == scan_group)
        {
            FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCJnSGSR3_b.ADTACT, 0U); /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
        else if (ADCJ_SCAN_GROUP_SG4 == scan_group)
        {
            FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCJnSGSR4_b.ADTACT, 0U); /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        }
        else
        {
            /* Should not run into this case */
        }
    }

    /* Wait until scan group is stopped */
    FSP_HARDWARE_REGISTER_WAIT(*ADCJ_SG_REG_ADDR(&p_reg->ADCJnSGSR0, scan_group), ADCJ_SGSR_SCAN_GROUP_IDLE); /* PRQA S 0311, 0312, 0488, 0662, 3395, 3469, 4521 # Rule-11.8, Rule-18.4, Dir-1.1, Rule-1.2, Rule-12.1, Dir-4.9, Rule-10.1 */

    /* Check if scan group use track and hold */
    if (ADCJ_SG_TRIGGER_METHOD_THA == sgx_trigger_method)
    {
        /* Request to disable track and hold units belong to track and hold group A */
        r_adcj_th_disable(p_instance_ctrl, ADCJ_TH_GROUP_A);
    }
    else if (ADCJ_SG_TRIGGER_METHOD_THB == sgx_trigger_method)
    {
        /* Request to disable track and hold units belong to track and hold group B */
        r_adcj_th_disable(p_instance_ctrl, ADCJ_TH_GROUP_B);
    }
    else
    {
        /* Scan group not use track and hold, no need to do anything */
    }
}

/*******************************************************************************************************************//**
 * @brief This function check whether the scan group specified by scan_group is using track and hold software trigger
 *
 * @param[in]  p_instance_ctrl  Pointer to this ADCJ instance control
 * @param[in]  scan_group       Scan group to check
 * @retval     true             Scan group uses track and hold software trigger
 * @retval     false            Scan group is not use track and hold software trigger
 **********************************************************************************************************************/
__STATIC_INLINE bool r_adcj_is_th_sw_trigger (adcj_instance_ctrl_t * const p_instance_ctrl,    /* PRQA S 3673 # Rule-8.13 */
                                              adcj_scan_group_t const      scan_group)
{
    adcj_extended_cfg_t * p_extend = (adcj_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend; /* PRQA S 0311, 0316, 3678 # Rule-11.8, Rule-11.5, Rule-8.13 */
    bool return_val                = true;

    /* Get scan group trigger method */
    adcj_sg_trigger_method_t trigger_method = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;

    if (ADCJ_SG_TRIGGER_METHOD_THA == trigger_method)
    {
        /* Check if track and hold group A is not use software trigger */
        if (ADCJ_TH_GROUP_TRIGGER_METHOD_SW != p_extend->th_cfg.trigger_method[ADCJ_TH_GROUP_A])
        {
            return_val = false;
        }
    }
    else if (ADCJ_SG_TRIGGER_METHOD_THB == trigger_method)
    {
        /* Check if track and hold group B is not use software trigger */
        if (ADCJ_TH_GROUP_TRIGGER_METHOD_SW != p_extend->th_cfg.trigger_method[ADCJ_TH_GROUP_B])
        {
            return_val = false;
        }
    }
    else
    {
        /* Scan group is not use track and hold trigger method */
        return_val = false;
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function is called by @ref R_ADCJ_ScanGroupStop to disable all virtual channel end interrupt
 * of the scan group
 *
 * @param[in]   p_reg            Pointer to registers of this ADCJ instance
 * @param[in]   p_sg_cfg         Pointer to scan group configuration
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_scan_group_vc_deinit (R_ADCJ1_Type * const p_reg, adcj_sg_generic_cfg_t const * p_sg_cfg)
{
    /* Disable all virtual channel end interrupt for scan group */
    for (uint32_t channel = p_sg_cfg->start_vc_ptr, cnt = 0U; channel <= p_sg_cfg->end_vc_ptr; channel++, cnt++) /* PRQA S 2462, 2463, 3387, 3418 # Rule-14.2, Rule-13.3, Rule-12.3 */
    {
        p_reg->ADCJnVCR[channel] = p_sg_cfg->p_vc_cfg[cnt].vcr & (~ADCJ_VCR_END_INT_MASK);                       /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    }
}

/*******************************************************************************************************************//**
 * @brief Disable the T&H units which scan group used after stop the scan group.
 * This function is called by @ref R_ADCJ_ScanGroupStop to disable T&H units that scan group used
 * This function must be called when the target scan group of th_group is in idle state.
 * @param[in] p_instance_ctrl                   Pointer to ADCJ instance control block
 * @param[in] th_group                          T&H group to stop (T&H group A and T&H group B)
 **********************************************************************************************************************/
static void r_adcj_th_disable (adcj_instance_ctrl_t * const p_instance_ctrl, adcj_th_group_t const th_group)
{
    R_ADCJ1_Type * p_reg    = p_instance_ctrl->p_reg;
    uint8_t        ther_tha = 0U;

    for (uint32_t th_unit = 0U; th_unit < ADCJ_NUMBER_OF_TH_UNIT; th_unit++)
    {
        /* Check if T&H unit z is belong to T&H group A or T&H group B */
        ther_tha |= 0x1U << th_unit;
    }

    if (ADCJ_TH_GROUP_A == th_group)
    {
        /* Disable T&H units used by T&H group A */
        p_reg->ADCJnTHER &= ~ther_tha; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Mask this T&H group as stopped so user can not use it */
        p_instance_ctrl->th_state[ADCJ_TH_GROUP_A] = ADCJ_TH_STATE_STOP;
    }
    else
    {
        /* Do Nothing */
    }
}

#if (ADCJ_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief This function check the valid of group_mask parameter
 *
 * @param[in] group_mask    group_mask to check
 * @param[in] p_sg_cfg      Scan groups configuration of ADCJ
 * @retval    FSP_SUCCESS                           The group mask is valid
 * @retval    FSP_ERR_SGX_DISABLED                  The scan group specified by group mask is disabled
 * @retval    FSP_ERR_PWM_DIAG_DISABLED             PWM-Diag is disabled and group_mask is PWM-Diag
 * @retval    FSP_ERR_ADCJ_GROUP_MASK_NOT_SUPPORT   This group_mask is not supported
 **********************************************************************************************************************/
static fsp_err_t r_adcj_group_mask_param_check (adc_group_mask_t const group_mask, adcj_sg_cfg_t const * p_sg_cfg)
{
    fsp_err_t return_val = FSP_SUCCESS;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Check if scan group is enabled in scan groups configuration */
        if (NULL == p_sg_cfg->p_sg_cfg[SCH1R(group_mask) - 1]) /* PRQA S 3469 # Dir-4.9 */
        {
            return_val = FSP_ERR_SGX_DISABLED;
        }
    }
    else if ((ADC_GROUP_MASK_PWM_DIAG == group_mask))
    {
        /*  Check if scan group PWM-Diag is disabled */
        if (ADCJ_PWM_DIAG_CFG_DISABLE == p_sg_cfg->pwm_diag_cfg)
        {
            return_val = FSP_ERR_PWM_DIAG_DISABLED;
        }
    }
    else
    {
        /* Not support cases */
        return_val = FSP_ERR_ADCJ_GROUP_MASK_NOT_SUPPORT;
    }

    return return_val;
}

#endif                                                                                                              /* ADCJ_CFG_PARAM_CHECKING_ENABLE */

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to ADCJ instance control block
 * @param[in]     p_args     Pointer to arguments on stack
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_call_callback (adcj_instance_ctrl_t * const p_ctrl, adc_callback_args_t * const p_args) /* PRQA S 3673 # Rule-8.13 */
{
    adc_callback_args_t args;

    /* Check if the user callback function is available */
    if (NULL == p_ctrl->p_callback)
    {
        return;
    }

    /* Store callback arguments in memory provided by user if available.  This allows callback arguments to be
     * stored in non-secure memory so they can be accessed by a non-secure callback function. */
    adc_callback_args_t * p_args_memory = p_ctrl->p_callback_memory;
    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args_memory;

        /* Copy the stacked args to callback memory */
        *p_args_memory = *p_args;
    }
    else
    {
        /* Use provided args struct on stack */
        p_args_memory = p_args;
    }

    p_ctrl->p_callback(p_args_memory);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

/*******************************************************************************************************************//**
 * This function handle common scan end interrupt for ADCJ.
 *
 * @param[in]  event                   Event that triggered the ISR
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_scan_end_common_handle (adc_event_t const event)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Scan end interrupt is pulse interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Rule-11.5 */
    adc_callback_args_t    args            = {0U};

    args.unit       = p_instance_ctrl->p_cfg->unit;
    args.event      = event;
    args.p_context  = p_instance_ctrl->p_context;
    args.channel    = (adc_channel_t) ADCJ_CALLBACK_ARGS_CHANNEL_NOT_USE;       /* PRQA S 4342 # Rule-10.5 */
    args.group_mask = (adc_group_mask_t) ADCJ_CALLBACK_ARGS_GROUP_MASK_NOT_USE; /* PRQA S 4342 # Rule-10.5 */

    /* Call user callback function */
    r_adcj_call_callback(p_instance_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * This function handle ID error interrupt for ADCJ.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCJ instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_id_error_handle (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCJ1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_ID_ERROR;
    args.p_context = p_instance_ctrl;

    /* Get the channel in which ID error occurred */
    args.channel    = (adc_channel_t) p_reg->ADCJnIDER_b.IDECAP;                /* PRQA S 0662, 4342 # Dir-1.1, Rule-1.2, Rule-10.5 */
    args.group_mask = (adc_group_mask_t) ADCJ_CALLBACK_ARGS_GROUP_MASK_NOT_USE; /* PRQA S 4342 # Rule-10.5 */

    /* Clear the ID error status flag */
    p_reg->ADCJnECR_b.IDEC = ADCJ_ECR_ID_ERROR_STATUS_CLEAR;                    /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Call user callback function to notify an ID error had occurred */
    r_adcj_call_callback(p_instance_ctrl, &args);
}

/*******************************************************************************************************************//**
 * This function handle overwrite error interrupt for ADCJ.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCJ instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_overwrite_error_handle (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCJ1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_OVERWRITE_ERROR;
    args.p_context = p_instance_ctrl;

    /* Get the channel in which overwrite error occurred */
    args.channel    = (adc_channel_t) p_reg->ADCJnOWER_b.OWECAP;                /* PRQA S 0662, 4342 # Dir-1.1, Rule-1.2, Rule-10.5 */
    args.group_mask = (adc_group_mask_t) ADCJ_CALLBACK_ARGS_GROUP_MASK_NOT_USE; /* PRQA S 4342 # Rule-10.5 */

    /* Clear the overwrite error status flag */
    p_reg->ADCJnECR = ADCJ_ECR_OVERWRITE_ERROR_STATUS_CLEAR;                    /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

    /* Call user callback function to notify an overwrite error had occurred */
    r_adcj_call_callback(p_instance_ctrl, &args);
}

/*******************************************************************************************************************//**
 * This function handle trigger overlap error interrupt for ADCJ.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCJ instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_trigger_overlap_error_handle (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCJ1_Type      * p_reg     = p_instance_ctrl->p_reg;
    uint8_t             tocer_reg = p_reg->ADCJnTOCER; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    uint16_t            ecr_reg   = 0U;
    adc_callback_args_t args      = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_TRIGGER_OVERLAP_ERROR;
    args.p_context = p_instance_ctrl->p_context;
    args.channel   = (adc_channel_t) ADCJ_CALLBACK_ARGS_CHANNEL_NOT_USE; /* PRQA S 4342 # Rule-10.5 */

    for (uint32_t scan_group = 0U; scan_group < ADCJ_NUMBER_OF_SG; scan_group++)
    {
        /* Check if trigger overlap had occurred in scan group */
        if (0U != (tocer_reg & (ADCJ_TOCER_SG0_MASK << scan_group)))
        {
            args.group_mask = (adc_group_mask_t) (0x1U << scan_group);         /* PRQA S 4342, 4394 # Rule-10.5, Rule-10.8 */

            /* Clear the trigger overlap interrupt for scan group 0 */
            ecr_reg |= ADCJ_ECR_TRIGGER_OVERLAP_ERROR_SG0_CLEAR << scan_group; /* PRQA S 0499, 1891 # Rule-12.2, Rule-10.7 */

            /* Call user callback function to notify an trigger overlap error had occurred */
            r_adcj_call_callback(p_instance_ctrl, &args);
        }
    }

    /* Request to clear the trigger overlap activated */
    p_reg->ADCJnECR = ecr_reg;         /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
}

/*******************************************************************************************************************//**
 * This function handle Upper/Lower limit check error interrupt for ADCJ.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCJ instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adcj_upper_lower_error_handle (adcj_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCJ1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit       = p_instance_ctrl->p_cfg->unit;
    args.event      = ADC_EVENT_UPPER_LOWER_LIMIT_ERROR;
    args.p_context  = p_instance_ctrl->p_context;
    args.group_mask = (adc_group_mask_t) ADCJ_CALLBACK_ARGS_GROUP_MASK_NOT_USE; /* PRQA S 4342 # Rule-10.5 */

    /* Check if Upper/Lower Limit error had occured in PWM-Diag */
    if (0U != p_reg->ADCJnPWVCLMSR_b.PWVCLMS)                                   /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        args.channel = ADC_CHANNEL_PWM_DIAG;

        /* Clear the PWM-diag upper/lower limit interrupt status */
        p_reg->ADCJnPWVCLMSCR |= ADCJ_PWM_DIAG_UL_STATUS_CLEAR; /* PRQA S 0662 # Dir-1.1, Rule-1.2 */

        /* Call user callback function to notify a PWM-diag upper/lower limit check error had occurred */
        r_adcj_call_callback(p_instance_ctrl, &args);
    }

    /* Check if Upper/Lower Limit error had occured in Virtual Channel and get the channel number */
    if (0U != p_reg->ADCJnVCLMSR1)                                                 /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        /* Get the lower channel (0..31) that interrupt had occurred */
        uint32_t channel = SCH1R(p_reg->ADCJnVCLMSR1) - 1;                         /* PRQA S 0662, 3469, 4434 # Dir-1.1, Rule-1.2, Dir-4.9, Rule-10.3 */
        args.channel = (adc_channel_t) channel;                                    /* PRQA S 4342 # Rule-10.5 */

        /* Clear the Upper/Lower limit interrupt status for the current channel */
        p_reg->ADCJnVCLMSCR1 |= (ADCJ_VIRTUAL_CHANNEL_UL_STATUS_CLEAR << channel); /* PRQA S 0662, 1891 # Dir-1.1, Rule-1.2, Rule-10.7 */

        /* Call user callback function to notify a Virtual Channel upper/lower limit check error had occurred */
        r_adcj_call_callback(p_instance_ctrl, &args);
    }
    else if (0U != p_reg->ADCJnVCLMSR2)                                            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        /* Get the higher channel (32..63) that interrupt had occurred */
        uint32_t channel = SCH1R(p_reg->ADCJnVCLMSR2) - 1;                         /* PRQA S 0662, 3469, 4434 # Dir-1.1, Rule-1.2, Dir-4.9, Rule-10.3 */
        args.channel = (adc_channel_t) (channel + 32);                             /* PRQA S 1840, 4342, 4394 # Rule-10.4, Rule-10.5, Rule-10.8 */

        /* Clear the Upper/Lower limit interrupt status for the current channel */
        p_reg->ADCJnVCLMSCR2 |= (ADCJ_VIRTUAL_CHANNEL_UL_STATUS_CLEAR << channel); /* PRQA S 0662, 1891 # Dir-1.1, Rule-1.2, Rule-10.7 */

        /* Call user callback function to notify a Virtual Channel upper/lower limit check error had occurred */
        r_adcj_call_callback(p_instance_ctrl, &args);
    }
    else
    {
        /* Should not enter here */
    }
}

/***********************************************************************************************************************
 * Interrupt service routines
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Scan group 0 scan end interrupt handle. The adcj_sg0_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 0 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_0 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg0_scan_end_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    r_adcj_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_0);
}

/*******************************************************************************************************************//**
 * Scan group 1 scan end interrupt handle. The adcj_sg1_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 0 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_1 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg1_scan_end_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    r_adcj_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_1);
}

/*******************************************************************************************************************//**
 * Scan group 2 scan end interrupt handle. The adcj_sg2_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 2 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_2 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg2_scan_end_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    r_adcj_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_2);
}

/*******************************************************************************************************************//**
 * Scan group 3 scan end interrupt handle. The adcj_sg3_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 3 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_3 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg3_scan_end_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    r_adcj_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_3);
}

/*******************************************************************************************************************//**
 * Scan group 4 scan end interrupt handle. The adcj_sg4_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 4 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_4 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_sg4_scan_end_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    r_adcj_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_4);
}

/*******************************************************************************************************************//**
 * ADCJ error interrupt service routine. The adcj_error_isr interrupt fires when trigger overlap, overwrite error,
 * ID error, upper/lower limit check interrupt is activated.
 * The user callback function is called with the corresponding even code if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adcj_error_isr (void) /* PRQA S 1503 # Rule-2.1 */
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* ADCJ error interrupt is pulse interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    adcj_instance_ctrl_t * p_instance_ctrl = (adcj_instance_ctrl_t *) R_FSP_IsrContextGet(irq); /* PRQA S 0316 # Rule-11.5 */
    R_ADCJ1_Type         * p_reg           = p_instance_ctrl->p_reg;                            /* PRQA S 3678 # Rule-8.13 */

    /* Check if ID error interrupt had happened and handle it */
    if ((ADCJ_ID_ERROR_INT_ENABLE == p_reg->ADCJnSFTCR_b.IDEIE) && (0U != p_reg->ADCJnIDER))    /* PRQA S 0662, 1881 # Dir-1.1, Rule-1.2, Rule-10.4 */
    {
        r_adcj_id_error_handle(p_instance_ctrl);
    }

    /* Check if overwrite error interrupt had happened and handle it */
    if ((ADCJ_OVERWRITE_ERROR_INT_ENABLE == p_reg->ADCJnSFTCR_b.OWEIE) && (0U != p_reg->ADCJnOWER)) /* PRQA S 0662, 1881 # Dir-1.1, Rule-1.2, Rule-10.4 */
    {
        r_adcj_overwrite_error_handle(p_instance_ctrl);
    }

    /* Check if trigger overlap error interrupt had happened and handle it */
    if ((ADCJ_TRIGGER_OVERLAP_INT_ENABLE == p_reg->ADCJnSFTCR_b.TOCEIE) && (0U != p_reg->ADCJnTOCER)) /* PRQA S 0662, 1881 # Dir-1.1, Rule-1.2, Rule-10.4 */
    {
        r_adcj_trigger_overlap_error_handle(p_instance_ctrl);
    }

    /* Check if upper/lower limit error interrupt had happened and handle it */
    if (((0U != p_reg->ADCJnVCLMINTER1) && (0U != p_reg->ADCJnVCLMSR1)) ||            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        ((0U != p_reg->ADCJnVCLMINTER2) && (0U != p_reg->ADCJnVCLMSR2)) ||            /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
        ((ADCJ_PWM_DIAG_UL_LIMIT_INT_ENABLE == p_reg->ADCJnPWVCLMINTER_b.PWADULIE) && /* PRQA S 0662, 1881 # Dir-1.1, Rule-1.2, Rule-10.4 */
         (0U != p_reg->ADCJnPWVCLMSR_b.PWVCLMS)))                                     /* PRQA S 0662 # Dir-1.1, Rule-1.2 */
    {
        r_adcj_upper_lower_error_handle(p_instance_ctrl);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

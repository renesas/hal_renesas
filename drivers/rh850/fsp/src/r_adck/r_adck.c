/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_adck.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** "ADCK" in ASCII, used to determine if unit is open. */
#define ADCK_OPEN                                       (0x4144434BULL)

/* The address offset between scan groups */
#define ADCK_SG_OFFSET                                  (0x40U)

/* This macro is used to get scan group register address from the register address of the first scan group
 * and scan group index */
#define ADCK_SG_REG_ADDR(base, index)    ((uint8_t *) (base) + (ADCK_SG_OFFSET) *(index))

#define ADCK_NUMBER_OF_UNIT                             (BSP_FEATURE_ADCK_NUM_OF_UNIT)

#define ADCK_SG3_AD_TIMER_CFG_INDEX                     (0x0U)
#define ADCK_SG4_AD_TIMER_CFG_INDEX                     (0x1U)

#define ADCK_VCR_END_INT_MASK                           (0x1U << 7U)

#define ADCK_SGSTCR_SW_START                            (0x1U)

#define ADCK_ADTSTCR_AD_TIMER_SW_START                  (0x1U)

#define ADCK_SGVCPR_VCSP_MASK                           (0x3F)
#define ADCK_SGVCPR_VCEP_MASK                           (0x3F)
#define ADCK_SGVCPR_VCEP_POS                            (8U)

#define ADCK_SGCR_HW_TRIGGER_ENABLE                     (0x1U)
#define ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE               (0x1U << 1U)
#define ADCK_SGCR_SCAN_END_INT_MASK                     (0x1U << 4U)

#define ADCK_SGSR_SCAN_GROUP_IDLE                       (0U)

#define ADCK_SGDIAGSR_SCAN_GROUP_IDLE                   (0U)

#define ADCK_HALTR_HALT                                 (0x1U)

#define ADCK_THSTPCR_SAMPLING_STOP                      (0x1U)

#define ADCK_THSMPSTCR_SAMPLING_START                   (0x1U)

#define ADCK_VCLMASCR_ALLMSC                            (0x1U)

#define ADCK_SGULCCR_CLEAR_ALL                          (0x3FU)

#define ADCK_SGSTPCR_SG_STOP                            (0x1U)

#define ADCK_THGSR_TH_UNIT_BIT_OFFSET                   (0x2U)

#define ADCK_THACR_SGS_MASK                             (0x3U)
#define ADCK_THACR_HOLD_CTRL_ENABLE                     (0x1U)
#define ADCK_THAHLDSTCR_HOLD_START                      (0x1U)
#define ADCK_THACR_HW_TRIGGER_ENABLE                    (0x1U)

#define ADCK_THBCR_SGS_MASK                             (0x3U)
#define ADCK_THBCR_HOLD_CTRL_ENABLE                     (0x1U)
#define ADCK_THBHLDSTCR_HOLD_START                      (0x1U)
#define ADCK_THBCR_HW_TRIGGER_ENABLE                    (0x1U)

#define ADCK_PWDSGCR_HW_TRIGGER_ENABLE                  (0x1U)

#define ADCK_SGDIAGSTCR_SW_START                        (0x1U)

#define ADCK_SGDIAGCR_HW_TRIGGER_ENABLE                 (0x1U)

#define ADCK_SGDIAGSTPCR_SG_STOP                        (0x1U)

#define ADCK_SGSTR_SGS_STATUS_MASK                      (0x3FU)
#define ADCK_SGSTR_AD_TIMER_STATUS_MASK                 (0x3000U)

#define ADCK_ADTENDCR_AD_TIMER_STOP                     (0x1U)

#define ADCK_TOCER_SG_DIAG_MASK                         (0x1U << 0U)
#define ADCK_TOCER_SG0_MASK                             (0x1U << 1U)

#define ADCK_ECR_ID_ERROR_STATUS_CLEAR                  (0x1U)
#define ADCK_ECR_PARITY_ERROR_STATUS_CLEAR              (0x1U << 1U)
#define ADCK_ECR_OVERWRITE_ERROR_STATUS_CLEAR           (0x1U << 2U)
#define ADCK_ECR_TRIGGER_OVERLAP_ERROR_SG_DIAG_CLEAR    (0x1U << 8U)
#define ADCK_ECR_TRIGGER_OVERLAP_ERROR_SG0_CLEAR        (0x1U << 9U)
#define ADCK_ECR_CLEAR_ALL                              (0x3F07U)

#define ADCK_VIRTUAL_CHANNEL_UL_STATUS_CLEAR            (0x1U)
#define ADCK_PWM_DIAG_UL_STATUS_CLEAR                   (0x1U)
#define ADCK_SG_DIAG_UL_STATUS_CLEAR                    (0x1U)

#define ADCK_TH_SAMPLING_DELAY_CLKAD_CYCLE              (30U)
#define ADCK_REG_ACCESS_TIME_CLKAD_CYCLE                (2U)

#define ADCK_CALLBACK_ARGS_CHANNEL_NOT_USE              (0U)
#define ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE           (0U)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static void      r_adck_scan_groups_force_termination(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adck_ul_limit_check_initialize(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adc_mpx_initialize(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adc_th_initialize(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adc_ad_timer_initialize(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adck_interrupts_enable(adck_instance_ctrl_t * const p_instance_ctrl);
static void      r_adck_interrupts_disable(adck_instance_ctrl_t * const p_instance_ctrl);
static fsp_err_t r_adck_scan_group_start(adck_instance_ctrl_t * const p_instance_ctrl,
                                         adck_scan_group_t const      scan_group);
__STATIC_INLINE void r_adck_scan_group_vc_init(R_ADCK1_Type * const p_reg, adck_sg_generic_cfg_t const * p_sg_cfg);
static void          r_adck_scan_group_stop(adck_instance_ctrl_t * const p_instance_ctrl,
                                            adck_scan_group_t const      scan_group);
__STATIC_INLINE bool r_adck_is_th_sw_trigger(adck_instance_ctrl_t * const p_instance_ctrl,
                                             adck_scan_group_t const      scan_group);
static void          r_adck_th_disable(adck_instance_ctrl_t * const p_instance_ctrl, adck_th_group_t const th_group);
__STATIC_INLINE void r_adck_scan_group_vc_deinit(R_ADCK1_Type * const p_reg, adck_sg_generic_cfg_t const * p_sg_cfg);

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)
static fsp_err_t r_adck_group_mask_param_check(adc_group_mask_t const group_mask, adck_sg_cfg_t const * p_sg_cfg);

#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

/* This function is used to call user callback functions */
__STATIC_INLINE void r_adck_call_callback(adck_instance_ctrl_t * const p_ctrl, adc_callback_args_t * const p_args);

/* This function handle common scan end interrupts */
__STATIC_INLINE void r_adck_scan_end_common_handle(adc_event_t const event);

/* This function handle ID error interrupt for ADCK */
__STATIC_INLINE void r_adck_id_error_handle(adck_instance_ctrl_t * const p_instance_ctrl);

/* This function handle overwrite error interrupt for ADCK */
__STATIC_INLINE void r_adck_overwrite_error_handle(adck_instance_ctrl_t * const p_instance_ctrl);

/* This function handle trigger overlap error interrupt for ADCK */
__STATIC_INLINE void r_adck_trigger_overlap_error_handle(adck_instance_ctrl_t * const p_instance_ctrl);

/***********************************************************************************************************************
 * ISR prototypes
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg0_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_sg1_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_sg2_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_sg3_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_sg4_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_sg_diag_scan_end_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_error_isr(void);
BSP_INTERRUPT_ATTRIBUTE void adck_mpx_isr(void);

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/
const adc_api_t g_adc_on_adck =
{
    .open           = R_ADCK_Open,
    .scanCfg        = R_ADCK_ScanCfg,
    .scanStart      = R_ADCK_ScanStart,
    .scanGroupStart = R_ADCK_ScanGroupStart,
    .scanStop       = R_ADCK_ScanStop,
    .scanStatusGet  = R_ADCK_ScanStatusGet,
    .read           = R_ADCK_Read,
    .read32         = R_ADCK_Read32,
    .calibrate      = R_ADCK_Calibrate,
    .offsetSet      = R_ADCK_OffsetSet,
    .callbackSet    = R_ADCK_CallbackSet,
    .close          = R_ADCK_Close,
    .infoGet        = R_ADCK_InfoGet
};

/*******************************************************************************************************************//**
 * @addtogroup ADCK
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @brief Initialize the ADCK
 * Implements @ref adc_api_t::open
 * This function configures the ADCK's registers and sets up the necessary hardware configuration for operation.
 * It must be called before any other API interacting with the ADCK.
 * This function forces to stop all ADCK activities, setting up hardware features (suspend method, data format,
 * addition count in A/D addition conversions, error interrupts, clear after read function, upper/lower limit check,
 * sampling time, input buffer amp, pin-level self-diagnostic, Wiring-break detection, MPX function, A/D conversion
 * timing monitor, initialize T&H , A/D timers, wait table, callback function, initialize interrupts).
 *
 * @retval FSP_SUCCESS                     ADCK instance is initialize successfully
 * @retval FSP_ERR_ASSERTION               The pointer input is NULL
 * @retval FSP_ERR_ALREADY_OPEN            ADCK instance has already opened
 * @retval FSP_ERR_IP_UNIT_NOT_PRESENT     This unit is not supported
 * @retval FSP_ERR_INVALID_ARGUMENT        Invalid unit
 **********************************************************************************************************************/
fsp_err_t R_ADCK_Open (adc_ctrl_t * const p_ctrl, adc_cfg_t const * const p_cfg)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);

    /* Check if unit input is valid */
    FSP_ERROR_RETURN((ADCK_NUMBER_OF_UNIT > p_cfg->unit), FSP_ERR_IP_UNIT_NOT_PRESENT);

    /* Verify this unit has already been opened   */
    FSP_ERROR_RETURN(ADCK_OPEN != p_instance_ctrl->opened, FSP_ERR_ALREADY_OPEN);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    p_instance_ctrl->p_cfg = p_cfg;
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_cfg->p_extend;

    /* Get the ADCK unit base address and store to p_reg */
#if (BSP_MCU_GROUP_RH850U2Bx)
    switch (p_instance_ctrl->p_cfg->unit)
    {
        case 0:
        {
            p_instance_ctrl->p_reg = R_ADCK0;
            break;
        }

        case 1:
        {
            p_instance_ctrl->p_reg = R_ADCK1;
            break;
        }

        case 2:
        {
            p_instance_ctrl->p_reg = R_ADCK2;
            break;
        }

        case 3:
        {
            p_instance_ctrl->p_reg = R_ADCK3;
            break;
        }

        default:

            /* Invalid unit */
            return FSP_ERR_INVALID_ARGUMENT;
    }

#elif (BSP_MCU_GROUP_RH850U2Cx)
    if (0U == p_instance_ctrl->p_cfg->unit)
    {
        p_instance_ctrl->p_reg      = R_ADCKA;
        p_instance_ctrl->p_selb_reg = R_ADCKA_SELB;
    }
    else if (1U == p_instance_ctrl->p_cfg->unit)
    {
        p_instance_ctrl->p_reg = R_ADCK1;
    }
    else if (2U == p_instance_ctrl->p_cfg->unit)
    {
        p_instance_ctrl->p_reg = R_ADCK2;
    }
    else
    {

        /* Invalid unit */
        return FSP_ERR_INVALID_ARGUMENT;
    }
#endif

    p_instance_ctrl->p_sg_cfg = NULL;

    /* Call termination procedure */
    r_adck_scan_groups_force_termination(p_instance_ctrl);

    /* Set suspend method */
    p_instance_ctrl->p_reg->ADCKnADCR1 = (uint8_t) p_extend->suspend_method;

    /* Set data format and addition count */
    p_instance_ctrl->p_reg->ADCKnADCR2 = (uint8_t) p_extend->data_format_addition_count.adcr2;

    /* If overwrite error interrupt is enabled, dummy read all data registers to clear the write flag if any */
    if (ADCK_OVERWRITE_ERROR_INT_ENABLE == p_extend->safety_cfg.sftcr_b.overwrite)
    {
        for (uint32_t channel = 0U; channel < ADCK_NUMBER_OF_VIRTUAL_CHANNEL; channel++)
        {
            (void) p_instance_ctrl->p_reg->ADCKnDR[channel];
        }
    }

    /* Clear all id error flag, parity error flag, overwrite error flag, and trigger overlap error flags */
    p_instance_ctrl->p_reg->ADCKnECR = ADCK_ECR_CLEAR_ALL;

    /* Configure read and clear, id error interrupt enable, parity error interrupt enable,
     * overwrite error interrupt enable, trigger overlap error interrupt enable */
    p_instance_ctrl->p_reg->ADCKnSFTCR = (uint8_t) p_extend->safety_cfg.sftcr;

    r_adck_ul_limit_check_initialize(p_instance_ctrl);

    /* Setting sampling period, extended sampling period, sampling selection, and enable/disable input buffer */
    p_instance_ctrl->p_reg->ADCKnSMPCR = p_extend->sampling_cfg.smpcr;

    /* Setting pin level self-diagnostic voltage level, enable/disable T&H path self-diagnostic */
    p_instance_ctrl->p_reg->ADCKnTDCR = p_extend->pin_lv_self_diag_cfg.tdcr;

    /* Setting enable/disable addition wiring-break detection mode, setting detection pulse length in wiring-break
     * detection mode 1 */
    p_instance_ctrl->p_reg->ADCKnODCR = p_extend->wiring_break_detect_cfg.odcr;

    r_adc_mpx_initialize(p_instance_ctrl);

    /* Setting to enable/disable trigger overlap check function */
    p_instance_ctrl->p_reg->ADCKnTOCCR = (uint8_t) p_extend->trigger_overlap_detect;

    /* Setting target virtual channel for A/D conversion timing monitor v (v = 0 to 4) */
    for (uint32_t timing_monior = 0U; timing_monior < ADCK_NUMBER_OF_AD_CONVERSION_TIMING_MONITOR; timing_monior++)
    {
        p_instance_ctrl->p_reg->ADCKnADENDP[timing_monior] =
            (uint8_t) p_extend->ad_conv_timing_monitor.target_vc[timing_monior];
    }

    r_adc_th_initialize(p_instance_ctrl);

    r_adc_ad_timer_initialize(p_instance_ctrl);

    /* Setting wait time table */
    for (uint32_t wait_setting = 0U; wait_setting < ADCK_WAIT_SETTING_TABLE_ENTRIES; wait_setting++)
    {
        p_instance_ctrl->p_reg->ADCKnWAITTR[wait_setting] =
            (uint16_t) p_extend->wait_setting.wait_setting[wait_setting];
    }

    /* Initialize callback related variables */
    p_instance_ctrl->p_callback        = p_cfg->p_callback;
    p_instance_ctrl->p_callback_memory = NULL;
    p_instance_ctrl->p_context         = p_cfg->p_context;

    /* Setting ADCK interrupts */
    r_adck_interrupts_enable(p_instance_ctrl);

    /* Mark this ADCK instance is opened */
    p_instance_ctrl->opened = ADCK_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief Close the ADCK
 * Implements @ref adc_api_t::close
 * This function halts all ADCK activities, disables interrupts
 *
 * @retval FSP_SUCCESS                 Module closed.
 * @retval FSP_ERR_ASSERTION           p_ctrl is NULL
 * @retval FSP_ERR_NOT_OPEN            Unit is not open.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_Close (adc_ctrl_t * const p_ctrl)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif

    r_adck_interrupts_disable(p_instance_ctrl);

    /* Call termination procedure */
    r_adck_scan_groups_force_termination(p_instance_ctrl);

    p_instance_ctrl->initialized = 0U;
    p_instance_ctrl->opened      = 0U;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This function perform scan groups setting for ADCK after ADCK is opened with Open API.
 * Implements @ref adc_api_t::scanCfg
 * This function does:
 * - Configure settings for scan group 0 to 4
 *  + Setting start, end virtual channel pointer, scan cycle in multi-cycle scan mode
 *  + Select hardware trigger source for ADCK0 scan groups
 * - Configure settings for SG-Diag
 *  + Physical channel select
 *  + Conversion type for virtual channels
 *  + End interrupt enable, end pointer select, end interrupt mode select, subtraction type select
 *    upper/lower limit check table register select 0, upper/lower limit check table register select 1, and disable
 *    HW trigger for SG-Diag
 * - Configure settings for PWM-Diag
 *  + Enable/Disable scan group 4 as PWM-Diag
 *  + Disable hardware trigger for PWM-Diag
 * When calling this function, user need to ensure that all scan group of ADCK is stopped.
 *
 * @retval FSP_SUCCESS                     ADCK scan groups is initialize successfully
 * @retval FSP_ERR_ASSERTION               The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                Unit is not open
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanCfg (adc_ctrl_t * const p_ctrl, void const * const p_scan_group_cfg)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;
    adck_sg_cfg_t        * p_sgs_cfg       = (adck_sg_cfg_t *) p_scan_group_cfg;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_scan_group_cfg);

    /* Check if scan group is enable but virtual channel configuration pointer is NULL */
    for (uint8_t scan_group = 0U; scan_group < ADCK_NUMBER_OF_SG; scan_group++)
    {
        FSP_ASSERT((NULL == p_sgs_cfg->p_sg_cfg[scan_group]) || (NULL != (p_sgs_cfg->p_sg_cfg[scan_group]->p_vc_cfg)));
    }
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg = p_instance_ctrl->p_reg;
    p_instance_ctrl->p_sg_cfg = p_sgs_cfg;
    adck_sg_generic_cfg_t const * p_sgx_cfg     = NULL;
    adck_sg_diag_cfg_t const    * p_sg_diag_cfg = p_instance_ctrl->p_sg_cfg->p_sg_diag_cfg;

    /* Set all virtual channel j (j = 0 to 63) to reset value */
    for (uint32_t channel = 0U; channel < ADCK_NUMBER_OF_VIRTUAL_CHANNEL; channel++)
    {
        p_reg->ADCKnVCR[channel] = 0U;
    }

    for (uint32_t scan_group = 0; scan_group < ADCK_NUMBER_OF_SG; scan_group++)
    {
        p_sgx_cfg = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group];

        /* Check if scan group x (x = 0 to 4) is used */
        if (NULL != p_sgx_cfg)
        {
            /* Setting start and end virtual channel pointers for scan group x */
            *((uint16_t *) ADCK_SG_REG_ADDR(&p_reg->ADCKnSGVCPR0, scan_group)) =
                (p_sgx_cfg->start_vc_ptr & ADCK_SGVCPR_VCSP_MASK) |
                ((p_sgx_cfg->end_vc_ptr & ADCK_SGVCPR_VCEP_MASK) << ADCK_SGVCPR_VCEP_POS);

            /* Setting scan cycles for scan group in multi-cycle scan mode */
            if (ADCK_SG_SCAN_MODE_MULTICYLE == p_sgx_cfg->sg_ctrl.sgcr_b.scan_mode)
            {
                *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGMCYCR0, scan_group) = p_sgx_cfg->scan_cycle;
            }
            else
            {
                /* Clear multi-cycle register for scan group x */
                *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGMCYCR0, scan_group) = 0U;
            }
        }
        else
        {
            /* Scan group x is not used so reset the scan group control register to reset value */
            *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) = 0U;
        }

#if (BSP_MCU_GROUP_RH850U2Cx)

        /* Check if this is ADCK0, if it is not, NULL is assigned to p_selb_reg in Open API */
        if (NULL != p_instance_ctrl->p_selb_reg)
        {
            /* Setting the scan group hardware trigger source for ADCK0 */
            p_instance_ctrl->p_selb_reg->ADCKnSGTSEL[scan_group] = p_sgx_cfg->adck_trigger_src;
        }
#endif

        /* Set T&H target scan group to scan group x if it is triggered by T&H group A or T&H group B */
        if (ADCK_SCAN_GROUP_SG1 <= scan_group)
        {
            /* Check if trigger method for this can group is T&H group A */
            if (ADCK_SG_TRIGGER_METHOD_THA == p_sgx_cfg->trigger_method)
            {
                /* Set scan group x to be target scan group for T&H group A */
                p_reg->ADCKnTHACR_b.SGS = (scan_group - (uint32_t) ADCK_SCAN_GROUP_SG1) & ADCK_THACR_SGS_MASK;
            }
            else if (ADCK_SG_TRIGGER_METHOD_THB == p_sgx_cfg->trigger_method)
            {
                /* Set scan group x to be target scan group for T&H group B */
                p_reg->ADCKnTHBCR_b.SGS = (scan_group - (uint32_t) ADCK_SCAN_GROUP_SG1) & ADCK_THBCR_SGS_MASK;
            }
            else
            {
                /* Because scan group x trigger method is not T&H group A or T&H group B so no need to do anything */
            }
        }
    }

    /* Check if SG-Diag is enable */
    if (NULL != p_sg_diag_cfg)
    {
        /* Setting target physical channels for SG-Diag */
        p_reg->ADCKnSGDIAGPCCR0 = p_sg_diag_cfg->physical_channel_cfg_0;
        p_reg->ADCKnSGDIAGPCCR1 = p_sg_diag_cfg->physical_channel_cfg_1;

        /* Setting conversion type for SG-Diag virtual channels k (k = 0 to 2) */
        for (uint32_t channel = 0U; channel < ADCK_NUMBER_OF_SG_DIAG_VIRTUAL_CHANNEL; channel++)
        {
            p_reg->ADCKnSGDIAGVCR[channel] = p_sg_diag_cfg->sg_diag_vc_cfg[channel].conv_type;
        }

        /* Setting SG-Diag end interrupt enable, end pointer select, end interrupt mode select, subtraction type select
         * upper/lower limit check table register select 0, upper/lower limit check table register select 1, and disable
         * HW trigger for SG-Diag */
        p_reg->ADCKnSGDIAGCR = p_sg_diag_cfg->sg_ctrl.sgdiagcr;
    }
    else
    {
        /* SG-Diag is not used so reset entire SGDIAGCR to its reset value */
        p_reg->ADCKnSGDIAGCR = 0U;
    }

#if (BSP_MCU_GROUP_RH850U2Cx)

    /* Check if scan group 4 is configure as PWM-Diag */
    if (ADCK_PWM_DIAG_CFG_ENABLE == p_instance_ctrl->p_sg_cfg->pwm_diag_cfg)
    {
        /* Reset ADCKnSGCR4 to reset value */
        p_reg->ADCKnSGCR4 = 0U;

        /* Reset ADCKnSGVCPR to reset value */
        p_reg->ADCKnSGVCPR4 = 0U;

        /* Set scan cycle count to 1 */
        p_reg->ADCKnSGMCYCR4 = 0U;

        /* Disable PWM-Diag HW trigger */
        p_reg->ADCKnPWDSGCR = 0U;

        /* Enable PWM-Diag */
        p_reg->ADCKnPWDCR = ADCK_PWM_DIAG_CFG_ENABLE;
    }
    else
    {
        /* Disable PWM-Diag */
        p_reg->ADCKnPWDCR = ADCK_PWM_DIAG_CFG_DISABLE;
    }
#endif

    /* Set initialized to ADCK_OPEN to indicate that the scan groups are configured */
    p_instance_ctrl->initialized = ADCK_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::scanStart is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanStart (adc_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @ref adc_api_t::scanStop is not supported on the RH850 MCUs.
 *
 * @retval FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanStop (adc_ctrl_t * const p_ctrl)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief This function starts (in case of a software trigger) or enable hardware trigger the scan group
 * specified by group_mask.
 * Implements @ref adc_api_t::scanGroupStart
 * When calling this function, user need to ensure that the specified scan group is in idle state (user can check
 * the scan group status by @ref R_ADCK_ScanGroupStatusGet API)
 * Scan groups need to initialized first by @ref adc_api_t::scanCfg before calling this function.
 *
 * @retval FSP_SUCCESS                          Scan group is started successfully.
 * @retval FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval FSP_ERR_NOT_OPEN                     This ADCK instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_IN_USE                       Scan group is currently activated or hardware trigger is enabled or
 *                                              T&H hardware trigger is enabled or A/D timer is enabled
 * @retval FSP_ERR_NOT_INITIALIZED              Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval FSP_ERR_ADCK_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @retval FSP_ERR_SG_DIAG_DISABLED             group_mask is SG-Diag but it is disabled
 * @retval FSP_ERR_TH_STOP                      Scan group use T&H but T&H is disabled by @ref R_ADCK_ScanGroupStop
 * @return                                      See @ref RENESAS_ERROR_CODES or functions called by this function for
 *                                              other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanGroupStart (adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED);

    /* Verify group_mask is valid for ADCK */
    return_val = r_adck_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type        * p_reg         = p_instance_ctrl->p_reg;
    adck_sg_cfg_t const * p_sg_cfg      = p_instance_ctrl->p_sg_cfg;
    uint32_t              sg_number     = 0U;
    uint8_t               sg_status     = 0U;
    uint8_t               sg_hw_trigger = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number     = SCH1R(group_mask) - 1;
        sg_status     = *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGSR0, sg_number);
        sg_hw_trigger = (*ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, sg_number)) &
                        (ADCK_SGCR_HW_TRIGGER_ENABLE | ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE);

        if ((0U == sg_status) && ((0U == sg_hw_trigger) ||
                                  ((ADCK_SGCR_HW_TRIGGER_ENABLE == sg_hw_trigger) &&
                                   r_adck_is_th_sw_trigger(p_instance_ctrl, (adck_scan_group_t) sg_number))))
        {
            /* Start the scan group */
            return_val = r_adck_scan_group_start(p_instance_ctrl, (adck_scan_group_t) sg_number);
        }
        else
        {
            /* Scan group is active or hardware trigger or A/D timer or track and hold is enabled */
            return_val = FSP_ERR_IN_USE;
        }
    }

#if (BSP_MCU_GROUP_RH850U2Cx)
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Check if scan group 4 is inactive */
        if ((0U == p_reg->ADCKnSGSR4) && (0U == p_reg->ADCKnPWDSGCR_b.TRGMD))
        {
            /* Enable HW trigger for PWM-Diag scan group (SG4) */
            p_reg->ADCKnPWDSGCR = ADCK_PWDSGCR_HW_TRIGGER_ENABLE;
        }
        else
        {
            return_val = FSP_ERR_IN_USE;
        }
    }
#endif
    else if (ADC_GROUP_MASK_SG_DIAG == group_mask)
    {
        /* Check if SG-Diag scan group is inactive */
        if ((0U == p_reg->ADCKnSGDIAGSR) && (0U == p_reg->ADCKnSGDIAGCR_b.TRGMD))
        {
            if (ADCK_SG_DIAG_TRIGGER_METHOD_SW == p_sg_cfg->p_sg_diag_cfg->trigger_method)
            {
                /* Start SG-Diag by software */
                p_reg->ADCKnSGDIAGSTCR = ADCK_SGDIAGSTCR_SW_START;
            }
            else
            {
                /* Enable HW trigger for SG-Diag */
                p_reg->ADCKnSGDIAGCR_b.TRGMD = ADCK_SGDIAGCR_HW_TRIGGER_ENABLE;
            }
        }
        else
        {
            return_val = FSP_ERR_IN_USE;
        }
    }
    else
    {
        /* R_ADCK_ScanGroupStart is called with wrong group_mask */
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @brief This function stops the scan group specified by group_mask.
 * Scan groups need to initialized first by @ref adc_api_t::scanCfg before calling this function.
 * If the scan group uses T&H, the T&H units which scan group used are disabled after calling this function.
 * User need to stop all scan groups and re-enable T&H by calling to @ref R_ADCK_THEnable in order to start
 * this scan group again by @ref R_ADCK_ScanGroupStart
 *
 * @param[in] p_ctrl                               Pointer to instance control
 * @param[in] group_mask                           Group mask of scan group to stop
 * @retval    FSP_SUCCESS                          Request to stop scan group successfully
 * @retval    FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN                     This ADCK instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_NOT_INITIALIZED              Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval    FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval    FSP_ERR_ADCK_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval    FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @retval    FSP_ERR_SG_DIAG_DISABLED             group_mask is SG-Diag but it is disabled
 * @return                                         See @ref RENESAS_ERROR_CODES or functions called by this function
 *                                                 for other possible return codes.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanGroupStop (adc_ctrl_t * p_ctrl, adc_group_mask_t group_mask)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED);

    /* Verify group_mask is valid for ADCK */
    return_val = r_adck_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg     = p_instance_ctrl->p_reg;
    uint32_t       sg_number = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number = SCH1R(group_mask) - 1;

        /* Stop the scan group */
        r_adck_scan_group_stop(p_instance_ctrl, (adck_scan_group_t) sg_number);
    }

#if (BSP_MCU_GROUP_RH850U2Cx)
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Disable HW trigger for PWM-Diag */
        p_reg->ADCKnPWDSGCR = 0U;

        /* Wait until scan group is stopped */
        FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCKnSGSR4, ADCK_SGSR_SCAN_GROUP_IDLE);
    }
#endif
    else if (ADC_GROUP_MASK_SG_DIAG == group_mask)
    {
        /* Disable HW trigger for SG-Diag scan group */
        p_reg->ADCKnSGDIAGCR_b.TRGMD = 0U;

        /* Request to stop SG-Diag */
        p_reg->ADCKnSGDIAGSTPCR = ADCK_SGDIAGSTPCR_SG_STOP;

        /* Wait until scan group is stopped */
        FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCKnSGDIAGSR, ADCK_SGDIAGSR_SCAN_GROUP_IDLE);
    }
    else
    {
        /* R_ADCK_ScanGroupStop is called with wrong group_mask */
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
 * @retval FSP_ERR_NOT_OPEN         This ADCK instance is not open by @ref adc_api_t::open
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanStatusGet (adc_ctrl_t * const p_ctrl, adc_status_t * p_status)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg = p_instance_ctrl->p_reg;

    /* Check if any scan group is active */
    if (0U != (p_reg->ADCKnSGSTR & ADCK_SGSTR_SGS_STATUS_MASK))
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
 * This function return ADCK_SG_STATUS_ACTIVE when the specified scan group is activated.
 * @param[in]  p_ctrl                               Pointer to ADCK instance control
 * @param[in]  group_mask                           Group mask of the scan group to get status
 * @param[out] p_status                             Pointer to variable to store the scan group status
 * @retval     FSP_SUCCESS                          Scan group status is get successfully
 * @retval     FSP_ERR_ASSERTION                    The pointer input is NULL
 * @retval     FSP_ERR_SGX_DISABLED                 Scan group specified by group_mask is disabled
 * @retval     FSP_ERR_ADCK_GROUP_MASK_NOT_SUPPORT  group_mask input is not supported
 * @retval     FSP_ERR_PWM_DIAG_DISABLED            group_mask is PWM-Diag but it is disabled
 * @retval     FSP_ERR_SG_DIAG_DISABLED             group_mask is SG-Diag but it is disabled
 * @retval     FSP_ERR_NOT_OPEN                     This ADCK instance is not open by @ref adc_api_t::open
 * @return                                          See @ref RENESAS_ERROR_CODES or functions called by this function
 *                                                  for other possible return codes
 **********************************************************************************************************************/
fsp_err_t R_ADCK_ScanGroupStatusGet (adc_ctrl_t * const       p_ctrl,
                                     adc_group_mask_t const   group_mask,
                                     adck_sg_status_t * const p_status)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;
    fsp_err_t              return_val      = FSP_SUCCESS;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_status);

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);

    /* Verify group_mask is valid for ADCK */
    return_val = r_adck_group_mask_param_check(group_mask, p_instance_ctrl->p_sg_cfg);
    FSP_ERROR_RETURN(FSP_SUCCESS == return_val, return_val);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg     = p_instance_ctrl->p_reg;
    uint32_t       sg_number = 0U;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Get the scan group number */
        sg_number = SCH1R(group_mask) - 1;

        /* Get status of scan group */
        *p_status =
            (adck_sg_status_t) (*ADCK_SG_REG_ADDR(&p_reg->ADCKnSGSR0, sg_number) >> R_ADCK1_ADCKnSGSR0_SGACT_Pos);
    }

#if (BSP_MCU_GROUP_RH850U2Cx)
    else if (ADC_GROUP_MASK_PWM_DIAG == group_mask)
    {
        /* Scan group 4 and PWM-Diag use the same status register */
        *p_status = (adck_sg_status_t) p_reg->ADCKnSGSR4_b.SGACT;
    }
#endif
    else if (ADC_GROUP_MASK_SG_DIAG == group_mask)
    {
        /* Get the status of SG-Diag */
        *p_status = (adck_sg_status_t) p_reg->ADCKnSGDIAGSR_b.SDACT;
    }
    else
    {
        /* R_ADCK_ScanGroupStatusGet is called with wrong group_mask */
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
 * @retval FSP_ERR_NOT_OPEN                     This ADCK instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_ADCK_CHANNEL_NOT_SUPPORT     This reg_id is not supported
 **********************************************************************************************************************/
fsp_err_t R_ADCK_Read (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint16_t * const p_data)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_data);

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);

    /* Check reg_id input valid */
    FSP_ERROR_RETURN(reg_id < ADCK_NUMBER_OF_VIRTUAL_CHANNEL, FSP_ERR_ADCK_CHANNEL_NOT_SUPPORT);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg = p_instance_ctrl->p_reg;

    /* Write data of channel reg_id into p_data*/
    *p_data = p_reg->ADCKnDR[reg_id];

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
 * @retval FSP_ERR_NOT_OPEN                     This ADCK instance is not open by @ref adc_api_t::open
 * @retval FSP_ERR_ADCK_CHANNEL_NOT_SUPPORT     This reg_id is not supported
 **********************************************************************************************************************/
fsp_err_t R_ADCK_Read32 (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, uint32_t * const p_data)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_data);

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);

    /* Check reg_id input valid */
    FSP_ERROR_RETURN(reg_id < ADCK_NUMBER_OF_VIRTUAL_CHANNEL, FSP_ERR_ADCK_CHANNEL_NOT_SUPPORT);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg = p_instance_ctrl->p_reg;

    /* Check if number of channel is even */
    if (0U == ((uint32_t) reg_id) % 2U)
    {
        /* Write data of channels reg_id and reg_id + 1 into p_data */
        *p_data = *(uint32_t *) (&p_reg->ADCKnDR[reg_id]);
    }
    else
    {
        /* Write data of channel reg_id into p_data */
        *p_data = (uint32_t) (p_reg->ADCKnDR[reg_id]);
    }

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Read PWM-Diag data register to p_data
 *
 * @param[in] p_ctrl                       Pointer to ADCK instance control
 * @param[in] p_data                       Pointer to variable to store read data
 * @retval    FSP_SUCCESS                  The read operation is performed successfully.
 * @retval    FSP_ERR_ASSERTION            p_ctrl or p_data is NULL.
 * @retval    FSP_ERR_NOT_OPEN             The control block has not been opened.
 * @retval    FSP_ERR_NOT_INITIALIZED      Scan groups is not initialized by @ref adc_api_t::scanCfg
 * @retval    FSP_ERR_PWM_DIAG_DISABLED    PWM-Diag is disabled.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_PWMDiagRead (adc_ctrl_t * const p_ctrl, uint16_t * const p_data)
{
#if (BSP_MCU_GROUP_RH850U2Cx)
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

 #if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_data);

    /* Verify if unit and scan groups are opened and initialized */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->initialized, FSP_ERR_NOT_INITIALIZED);

    /* Check if PWM-Diag is enable */
    adck_pwm_diag_cfg_t p_pwm_diag_cfg = p_instance_ctrl->p_sg_cfg->pwm_diag_cfg;
    FSP_ERROR_RETURN(ADCK_PWM_DIAG_CFG_ENABLE == p_pwm_diag_cfg, FSP_ERR_PWM_DIAG_DISABLED);
 #endif                                /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg = p_instance_ctrl->p_reg;

    *p_data = p_reg->ADCKnPWDDR;
#endif

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @brief This function is not supported on this release.
 *
 * @param[in]  p_ctrl                       Pointer to ADCK instance control
 * @param[out] p_data                       Pointer to variable to store read data
 * @retval     FSP_ERR_UNSUPPORTED            Function not supported in this implemention.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_SGDiagRead (adc_ctrl_t * const p_ctrl, uint16_t * const p_data)
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
fsp_err_t R_ADCK_CallbackSet (adc_ctrl_t * const          p_ctrl,
                              void (                    * p_callback)(adc_callback_args_t *),
                              void * const                p_context,
                              adc_callback_args_t * const p_callback_memory)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ASSERT(p_callback);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
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
fsp_err_t R_ADCK_InfoGet (adc_ctrl_t * const p_ctrl, adc_info_t * const p_adc_info)
{
    adck_instance_ctrl_t        * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;
    adck_sg_generic_cfg_t const * p_sgx_cfg       = NULL;
    fsp_err_t err = FSP_SUCCESS;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(NULL != p_instance_ctrl);
    FSP_ASSERT(NULL != p_adc_info);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif

    /* Loop through each scan group to search for the lowest and number of consecutive virtual channels */
    for (uint32_t scan_group = 0; scan_group < ADCK_NUMBER_OF_SG; scan_group++)
    {
        /* p_instance_ctrl->p_sg_cfg is set within ScanCfg */
        p_sgx_cfg = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group];
        if (NULL != p_sgx_cfg)
        {
            /* Determine the lowest virtual channel that is configured */
            p_adc_info->p_address[scan_group] = &p_instance_ctrl->p_reg->ADCKnDR[p_sgx_cfg->start_vc_ptr];

            /* Determine the number of virtual channels that is configured consecutively */
            p_adc_info->length[scan_group] = (uint32_t) ((p_sgx_cfg->end_vc_ptr - p_sgx_cfg->start_vc_ptr) + 1);
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
fsp_err_t R_ADCK_Calibrate (adc_ctrl_t * const p_ctrl, void const * p_extend)
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
fsp_err_t R_ADCK_OffsetSet (adc_ctrl_t * const p_ctrl, adc_channel_t const reg_id, int32_t const offset)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    FSP_PARAMETER_NOT_USED(reg_id);
    FSP_PARAMETER_NOT_USED(offset);

    return FSP_ERR_UNSUPPORTED;
}

/*******************************************************************************************************************//**
 * @brief Enable the T&H units after it is stopped by @ref R_ADCK_ScanGroupStop
 * After user use @ref R_ADCK_ScanGroupStop to stop T&H units after stop a scan group, user need to use this function
 * to re-enable T&H units before calling @ref R_ADCK_ScanGroupStart to start the scan group again.
 * This function must be called when all scan groups are stopped
 * @param[in] p_ctrl                         Pointer to ADCK control block
 * @retval    FSP_SUCCESS                    T&H units is enabled successfully
 * @retval    FSP_ERR_ASSERTION              The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN               This ADCK instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_SG_AD_TIMER_ACTIVE     Any scan group or A/D timer of any scan group is active
 **********************************************************************************************************************/
fsp_err_t R_ADCK_THEnable (adc_ctrl_t * const p_ctrl)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)
    FSP_ASSERT(p_instance_ctrl);
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif

    R_ADCK1_Type        * p_reg      = p_instance_ctrl->p_reg;
    adck_extended_cfg_t * p_extend   = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    fsp_err_t             return_val = FSP_SUCCESS;

    /* Check if all scan groups and A/D timer are in idle state */
    if (0U == (p_reg->ADCKnSGSTR & (ADCK_SGSTR_SGS_STATUS_MASK | ADCK_SGSTR_AD_TIMER_STATUS_MASK)))
    {
        /* Disable T&H triggers */
        p_reg->ADCKnTHACR_b.HLDTE = 0U;
        p_reg->ADCKnTHBCR_b.HLDTE = 0U;

        /* Stop sampling of all T&H units */
        p_reg->ADCKnTHSTPCR = ADCK_THSTPCR_SAMPLING_STOP;

        /* Setting to enable/disable individual T&H unit */
        p_reg->ADCKnTHER = (uint8_t) p_extend->th_cfg.th_enable_cfg.ther;

        /* Start all T&H units sampling */
        p_reg->ADCKnTHSMPSTCR = ADCK_THSMPSTCR_SAMPLING_START;

        /* Update status of T&H group A and T&H group B */
        p_instance_ctrl->th_state[ADCK_TH_GROUP_A] = ADCK_TH_STATE_SAMPLING;
        p_instance_ctrl->th_state[ADCK_TH_GROUP_B] = ADCK_TH_STATE_SAMPLING;

        /*  After start T&H sampling, a wait time (about 30 CLKAD cycles ) is necessary
         *  before start a hold trigger.
         *  CLKAD (A/D conversion clock) and PCLK (Register access clock) clock source are the same (CLK_ADC0).
         *  So by read ADCKnSGSTR 15 times, we get the minimum time delay required for T&H sampling */
        for (uint32_t cnt = 0U; cnt < ADCK_TH_SAMPLING_DELAY_CLKAD_CYCLE / ADCK_REG_ACCESS_TIME_CLKAD_CYCLE; cnt++)
        {
            volatile uint16_t sgstr = p_reg->ADCKnSGSTR;
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
 * @param[in] p_ctrl                    Pointer to ADCK instance control
 * @param[in] voltage_level             Pin-level self-diagnostic voltage level to set
 * @retval    FSP_SUCCESS               The voltage level is changed successfully
 * @retval    FSP_ERR_ASSERTION         The pointer input is NULL
 * @retval    FSP_ERR_NOT_OPEN          This ADCK instance is not open by @ref adc_api_t::open
 * @retval    FSP_ERR_IN_USE            T&H path self-diagnosis is disabled and any scan group is on-going.
 **********************************************************************************************************************/
fsp_err_t R_ADCK_PinLVSelfDiagVoltageChange (adc_ctrl_t * const p_ctrl, const adck_pin_lv_self_diag_vol_t voltage_level)
{
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) p_ctrl;

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

    /* Verify the pointers are valid */
    FSP_ASSERT(NULL != p_instance_ctrl);

    /* Verify if unit and scan groups are opened */
    FSP_ERROR_RETURN(ADCK_OPEN == p_instance_ctrl->opened, FSP_ERR_NOT_OPEN);
#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

    R_ADCK1_Type * p_reg      = p_instance_ctrl->p_reg;
    fsp_err_t      return_val = FSP_SUCCESS;

    if ((ADCK_TH_PATH_SELF_DIAG_ENABLE == p_reg->ADCKnTDCR_b.THSDE) ||
        (0U == (p_reg->ADCKnSGSTR & ADCK_SGSTR_SGS_STATUS_MASK)))
    {
        p_reg->ADCKnTDCR_b.TDLV = voltage_level;
    }
    else
    {
        return_val = FSP_ERR_IN_USE;
    }

    return return_val;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup ADCK)
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
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control block
 **********************************************************************************************************************/
static void r_adck_scan_groups_force_termination (adck_instance_ctrl_t * const p_instance_ctrl)
{
    /* Disable T&H triggers */
    p_instance_ctrl->p_reg->ADCKnTHACR_b.HLDTE = 0U;
    p_instance_ctrl->p_reg->ADCKnTHBCR_b.HLDTE = 0U;

    /* Disable Hardware trigger of all scan groups */
    p_instance_ctrl->p_reg->ADCKnSGCR0_b.TRGMD = 0U;
    p_instance_ctrl->p_reg->ADCKnSGCR1_b.TRGMD = 0U;
    p_instance_ctrl->p_reg->ADCKnSGCR2_b.TRGMD = 0U;

    /* Clear hardware trigger and A/D timer trigger of scan group 3 and scan group 4 */
    p_instance_ctrl->p_reg->ADCKnSGCR3 &=
        (uint8_t) (~(ADCK_SGCR_HW_TRIGGER_ENABLE | ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE));
    p_instance_ctrl->p_reg->ADCKnSGCR4 &=
        (uint8_t) (~(ADCK_SGCR_HW_TRIGGER_ENABLE | ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE));

#if (BSP_MCU_GROUP_RH850U2Cx)

    /* Disable Hardware trigger of PWM-Diag */
    p_instance_ctrl->p_reg->ADCKnPWDSGCR = 0U;
#endif

    /* Disable Hardware trigger of SG-Diag */
    p_instance_ctrl->p_reg->ADCKnSGDIAGCR_b.TRGMD = 0U;

    /* Kill all scan groups, clear the information of held physical number of SG-Diag */
    p_instance_ctrl->p_reg->ADCKnADHALTR = ADCK_HALTR_HALT;

    /* Stop sampling of all T&H units */
    p_instance_ctrl->p_reg->ADCKnTHSTPCR = ADCK_THSTPCR_SAMPLING_STOP;

    /* Disable all T&H units */
    p_instance_ctrl->p_reg->ADCKnTHER = 0U;

    /* Update T&H group state */
    p_instance_ctrl->th_state[ADCK_TH_GROUP_A] = ADCK_TH_STATE_STOP;
    p_instance_ctrl->th_state[ADCK_TH_GROUP_B] = ADCK_TH_STATE_STOP;
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
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control block
 **********************************************************************************************************************/
static void r_adck_ul_limit_check_initialize (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Setup upper/lower limit check table (from 0 to 7) */
    for (uint32_t limit_entry = 0U; limit_entry < ADCK_UPPER_LOWER_LIMIT_TABLE_ENTRIES; limit_entry++)
    {
        p_instance_ctrl->p_reg->ADCKnVCULLMTBR[limit_entry] =
            p_extend->ul_limit_cfg.ul_limit_tb[limit_entry].vcullmtbr;
    }

    /* Clear all status bits of virtual channels (scan group, PWM-Diag, SG-Diag) upper/lower limit check */
    p_instance_ctrl->p_reg->ADCKnVCLMASCR = ADCK_VCLMASCR_ALLMSC;

    /* Clear all scan groups x (x = 0 to 4), SG-Diag upper/lower limit check status bits */
    p_instance_ctrl->p_reg->ADCKnSGULCCR = ADCK_SGULCCR_CLEAR_ALL;

    /* Enable upper/lower limit check for individual virtual channel j */
    p_instance_ctrl->p_reg->ADCKnVCLMINTER1 = p_extend->ul_limit_cfg.vc_ul_limit_int_0;
    p_instance_ctrl->p_reg->ADCKnVCLMINTER2 = p_extend->ul_limit_cfg.vc_ul_limit_int_1;

#if (BSP_MCU_GROUP_RH850U2Cx)

    /* Enable upper/lower limit check for PWM-Diag */
    p_instance_ctrl->p_reg->ADCKnPWVCLMINTER = p_extend->ul_limit_cfg.pwm_diag_ul_limit_int;
#endif

    /* Enable upper/lower limit check for SG-Diag */
    p_instance_ctrl->p_reg->ADCKnSDVCLMINTER = p_extend->ul_limit_cfg.sg_diag_ul_limit_int;
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup MPX for ADCK
 * This function does:
 * - Setting mask control format for MPX in I/O port output mode (using DMAC to transfer ADCKnMPXCURR1 to PSRn)
 * - Setting SPI communication command for MPX using SPI
 * - Setting to enable/disable MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control blockss
 **********************************************************************************************************************/
static void r_adc_mpx_initialize (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Setting mask control format for MPX in I/O port output mode */
    p_instance_ctrl->p_reg->ADCKnMPXCURCR = (uint8_t) p_extend->mpx_cfg.mask_control_format;

    /* Setting SPI communication command for MPX using SPI output mode */
    p_instance_ctrl->p_reg->ADCKnMPXCMDR = (uint8_t) p_extend->mpx_cfg.spi_command_information;

    /* Setting to enable/disable MPX interrupt */
    p_instance_ctrl->p_reg->ADCKnMPXINTER = (uint8_t) p_extend->mpx_cfg.mpx_int;
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup T&H units for ADCK
 * After calling to r_adck_scan_groups_force_termination, all T&H groups are stopped, T&H groups hardware
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
 * @param [in] p_instance_ctrl Pointer to this ADCK instance control block
 **********************************************************************************************************************/
static void r_adc_th_initialize (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Reset T&H group A, B control register to their reset value */
    p_instance_ctrl->p_reg->ADCKnTHACR = 0U;
    p_instance_ctrl->p_reg->ADCKnTHBCR = 0U;

    /* Check if T&H is used */
    if (0U != p_extend->th_cfg.th_enable_cfg.ther)
    {
        /* When T&H is used, it is recommended to set 0 to ADCKnTHCR.ASMPMSK (automatic sampling is effective)
         * Reference cautions 1 of section 1.2.5.10 "ADCKnTHSMPSTCR — T&H Sampling Start Control Register"
         * in the ADCK IP manual r01uh1122ej0040 Rev.0.40 */
        p_instance_ctrl->p_reg->ADCKnTHCR = 0U;

        /* Select T&H group for for each T&H unit */
        p_instance_ctrl->p_reg->ADCKnTHGSR = (uint16_t) p_extend->th_cfg.group_select_cfg.thgsr;

        /* Setting to enable/disable individual T&H unit */
        p_instance_ctrl->p_reg->ADCKnTHER = (uint8_t) p_extend->th_cfg.th_enable_cfg.ther;

        /* Start all T&H units sampling */
        p_instance_ctrl->p_reg->ADCKnTHSMPSTCR = ADCK_THSMPSTCR_SAMPLING_START;

        /* Update status of T&H group A and T&H group B */
        p_instance_ctrl->th_state[ADCK_TH_GROUP_A] = ADCK_TH_STATE_SAMPLING;
        p_instance_ctrl->th_state[ADCK_TH_GROUP_B] = ADCK_TH_STATE_SAMPLING;

        /* After start T&H sampling, a wait time (about 30 CLKAD cycles ) is necessary
         * before start a hold trigger. But to start a hold trigger, user need to call ScanCfg API
         * and the overhead of that function is enough to satisfy the wait time requirement for T&H sampling time */
    }
    else
    {
        /* Because all T&H units are disabled by r_adck_scan_groups_force_termination there is not need to do it again */

        /* Clear T&H group select for each T&H unit */
        p_instance_ctrl->p_reg->ADCKnTHGSR = 0U;
    }
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup AD timer trigger for ADCK
 * This function does:
 * - Setting AD timer x (x = 3 to 4) initial phase
 * - Setting AD timer x (x = 3 to 4) cycle time
 * After calling to r_adck_scan_groups_force_termination, the AD timers is stopped and
 * AD timers hardware trigger are disabled so there is no need to disable it again in this function
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control blocks
 **********************************************************************************************************************/
static void r_adc_ad_timer_initialize (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* Setting AD timer x (x = 3 to 4) initial phase */
    p_instance_ctrl->p_reg->ADCKnADTIPR3 = p_extend->ad_timer_cfg.initial_phase[ADCK_SG3_AD_TIMER_CFG_INDEX];
    p_instance_ctrl->p_reg->ADCKnADTIPR4 = p_extend->ad_timer_cfg.initial_phase[ADCK_SG4_AD_TIMER_CFG_INDEX];

    /* Setting AD timer x (x = 3 to 4) cycle time */
    p_instance_ctrl->p_reg->ADCKnADTPRR3 = p_extend->ad_timer_cfg.cycle[ADCK_SG3_AD_TIMER_CFG_INDEX];
    p_instance_ctrl->p_reg->ADCKnADTPRR4 = p_extend->ad_timer_cfg.cycle[ADCK_SG4_AD_TIMER_CFG_INDEX];
}

/*******************************************************************************************************************//**
 * @brief This function is called in Open API to setup ADCK interrupts
 * This function setting ADCK interrupts:
 * - Error interrupt (ID error, overwrite error, trigger overlap error, upper/lower limit check error).
 * - Scan group 0 end interrupt
 * - Scan group 1 end interrupt
 * - Scan group 2 end interrupt
 * - Scan group 3 end interrupt
 * - Scan group 4 end interrupt
 * - Scan group SG-Diag end interrupt
 * - MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control block
 **********************************************************************************************************************/
static void r_adck_interrupts_enable (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

#if (BSP_MCU_GROUP_RH850U2Bx)
    p_instance_ctrl->p_air_reg              = R_AIR;
    p_instance_ctrl->p_air_reg->AIRISELR[0] = 0U;
    p_instance_ctrl->p_air_reg->AIRISELR[1] = 0U;
#endif

    /* ADCK error interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.error_irq, p_extend->int_cfg.error_ipl, p_instance_ctrl);

    /* ADCK scan group 0 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_0_end_irq, p_extend->int_cfg.scan_group_0_end_ipl, p_instance_ctrl);

    /* ADCK scan group 1 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_1_end_irq, p_extend->int_cfg.scan_group_1_end_ipl, p_instance_ctrl);

    /* ADCK scan group 2 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_2_end_irq, p_extend->int_cfg.scan_group_2_end_ipl, p_instance_ctrl);

    /* ADCK scan group 3 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_3_end_irq, p_extend->int_cfg.scan_group_3_end_ipl, p_instance_ctrl);

    /* ADCK scan group 4 end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.scan_group_4_end_irq, p_extend->int_cfg.scan_group_4_end_ipl, p_instance_ctrl);

    /* ADCK SG-Diag end interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.sg_diag_end_irq, p_extend->int_cfg.sg_diag_end_ipl, p_instance_ctrl);

    /* ADCK MPX interrupt enable */
    R_BSP_IrqCfgEnable(p_extend->int_cfg.mpx_irq, p_extend->int_cfg.mpx_ipl, p_instance_ctrl);
}

/*******************************************************************************************************************//**
 * @brief This function is called in Close API to disable ADCK interrupts
 * This function disable ADCK interrupts:
 * - Error interrupt (ID error, overwrite error, trigger overlap error, upper/lower limit check error).
 * - Scan group 0 end interrupt
 * - Scan group 1 end interrupt
 * - Scan group 2 end interrupt
 * - Scan group 3 end interrupt
 * - Scan group 4 end interrupt
 * - Scan group SG-Diag end interrupt
 * - MPX interrupt
 * @param[in] p_instance_ctrl Pointer to this ADCK instance control block
 **********************************************************************************************************************/
static void r_adck_interrupts_disable (adck_instance_ctrl_t * const p_instance_ctrl)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;

    /* ADCK error interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.error_irq);

    /* ADCK scan group 0 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_0_end_irq);

    /* ADCK scan group 1 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_1_end_irq);

    /* ADCK scan group 2 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_2_end_irq);

    /* ADCK scan group 3 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_3_end_irq);

    /* ADCK scan group 4 end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.scan_group_4_end_irq);

    /* ADCK SG-Diag end interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.sg_diag_end_irq);

    /* ADCK MPX interrupt disable */
    R_BSP_IrqDisable(p_extend->int_cfg.mpx_irq);
}

/*******************************************************************************************************************//**
 * @brief This function start the scan group specified by scan_group
 *
 * @param[in]   p_instance_ctrl  Pointer to control block of this ADCK instance
 * @param[in]   scan_group       Scan group need to start
 * @retval      FSP_SUCCESS      Scan group is started successfully
 * @retval      FSP_ERR_TH_STOP  T&H units of scan group is stopped. Use @ref R_ADCK_THEnable to re-enable it.
 **********************************************************************************************************************/
static fsp_err_t r_adck_scan_group_start (adck_instance_ctrl_t * const p_instance_ctrl,
                                          adck_scan_group_t const      scan_group)
{
    R_ADCK1_Type        * p_reg      = p_instance_ctrl->p_reg;
    adck_extended_cfg_t * p_extend   = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    adck_sg_cfg_t const * p_sg_cfg   = p_instance_ctrl->p_sg_cfg;
    fsp_err_t             return_val = FSP_SUCCESS;

    /* Setup virtual virtual channel for scan group (enable virtual channel end interrupt) */
    r_adck_scan_group_vc_init(p_reg, p_sg_cfg->p_sg_cfg[scan_group]);

    /* Disable hardware trigger, AD timer trigger, setting scan end interrupt enable and scan mode
     * for scan group */
    *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) = p_sg_cfg->p_sg_cfg[scan_group]->sg_ctrl.sgcr;

    /* Get scan group trigger method */
    adck_sg_trigger_method_t sgx_trigger_method = p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;
    switch (sgx_trigger_method)
    {
        case ADCK_SG_TRIGGER_METHOD_SW:
        {
            /* Start scan group */
            *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGSTCR0, scan_group) = ADCK_SGSTCR_SW_START;
            break;
        }

        case ADCK_SG_TRIGGER_METHOD_HW:
        {
            /* Enable hardware trigger for scan group */
            *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) |= ADCK_SGCR_HW_TRIGGER_ENABLE;
            break;
        }

        case ADCK_SG_TRIGGER_METHOD_AD_TIMER:
        {
            if (ADCK_SCAN_GROUP_SG3 == scan_group)
            {
                if (ADCK_AD_TIMER_TRIGGER_METHOD_SW ==
                    p_extend->ad_timer_cfg.trigger_method[ADCK_SG3_AD_TIMER_CFG_INDEX])
                {
                    /* Enable scan group 3 to be trigger by A/D timer */
                    p_reg->ADCKnSGCR3 |= ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE;

                    /* Start A/D timer for scan group 3 */
                    p_reg->ADCKnADTSTCR3 = ADCK_ADTSTCR_AD_TIMER_SW_START;
                }
                else
                {
                    /* Enable scan group 4 to be trigger by A/D timer and A/D timer is trigger by HW */
                    p_reg->ADCKnSGCR3 |= ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE | ADCK_SGCR_HW_TRIGGER_ENABLE;
                }
            }
            else if (ADCK_SCAN_GROUP_SG4 == scan_group)
            {
                if (ADCK_AD_TIMER_TRIGGER_METHOD_SW ==
                    p_extend->ad_timer_cfg.trigger_method[ADCK_SG4_AD_TIMER_CFG_INDEX])
                {
                    /* Enable scan group to be trigger by A/D timer */
                    p_reg->ADCKnSGCR4 |= ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE;

                    /* Start A/D timer for scan group 4 */
                    p_reg->ADCKnADTSTCR4 = ADCK_ADTSTCR_AD_TIMER_SW_START;
                }
                else
                {
                    /* Enable scan group 4 to be trigger by A/D timer and A/D timer is trigger by HW */
                    p_reg->ADCKnSGCR4 |= ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE | ADCK_SGCR_HW_TRIGGER_ENABLE;
                }
            }
            else
            {
                /* Should not run in to this case because only scan group 3 and scan group 4 support A/D timer */
            }

            break;
        }

        case ADCK_SG_TRIGGER_METHOD_THA:
        {
            /* Check if T&H group is in sampling state */
            if (ADCK_TH_STATE_SAMPLING == p_instance_ctrl->th_state[ADCK_TH_GROUP_A])
            {
                /* Check if T&H group A target scan group is scan_group */
                if (p_reg->ADCKnTHACR_b.SGS + ADCK_SCAN_GROUP_SG1 == scan_group)
                {
                    if (ADCK_TH_GROUP_TRIGGER_METHOD_SW == p_extend->th_cfg.trigger_method[ADCK_TH_GROUP_A])
                    {
                        /* Enable hold control for T&H group A */
                        p_reg->ADCKnTHACR_b.HLDCTE = ADCK_THACR_HOLD_CTRL_ENABLE;

                        /* Enable hardware trigger for scan group */
                        *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) |= ADCK_SGCR_HW_TRIGGER_ENABLE;

                        /* Start T&H group A by software */
                        p_reg->ADCKnTHAHLDSTCR = ADCK_THAHLDSTCR_HOLD_START;
                    }
                    else
                    {
                        /* Enable hold control for T&H group A */
                        p_reg->ADCKnTHACR_b.HLDCTE = ADCK_THACR_HOLD_CTRL_ENABLE;

                        /* Enable hardware trigger for scan group */
                        *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) |= ADCK_SGCR_HW_TRIGGER_ENABLE;

                        /* T&H group A is triggered by HW trigger */
                        p_reg->ADCKnTHACR_b.HLDTE = ADCK_THACR_HW_TRIGGER_ENABLE;
                    }
                }
            }
            else
            {
                return_val = FSP_ERR_TH_STOP;
            }

            break;
        }

        case ADCK_SG_TRIGGER_METHOD_THB:
        {
            /* Check if T&H group is in sampling state */
            if (ADCK_TH_STATE_SAMPLING == p_instance_ctrl->th_state[ADCK_TH_GROUP_B])
            {
                /* Check if T&H group B target scan group is scan_group */
                if (p_reg->ADCKnTHBCR_b.SGS + ADCK_SCAN_GROUP_SG1 == scan_group)
                {
                    if (ADCK_TH_GROUP_TRIGGER_METHOD_SW == p_extend->th_cfg.trigger_method[ADCK_TH_GROUP_B])
                    {
                        /* Enable hold control for T&H group B */
                        p_reg->ADCKnTHBCR_b.HLDCTE = ADCK_THBCR_HOLD_CTRL_ENABLE;

                        /* Enable hardware trigger for scan group */
                        *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) |= ADCK_SGCR_HW_TRIGGER_ENABLE;

                        /* Start T&H group B by software */
                        p_reg->ADCKnTHBHLDSTCR = ADCK_THBHLDSTCR_HOLD_START;
                    }
                    else
                    {
                        /* Enable hold control for T&H group B */
                        p_reg->ADCKnTHBCR_b.HLDCTE = ADCK_THBCR_HOLD_CTRL_ENABLE;

                        /* Enable hardware trigger for scan group */
                        *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) |= ADCK_SGCR_HW_TRIGGER_ENABLE;

                        /* T&H group B is triggered by HW trigger */
                        p_reg->ADCKnTHBCR_b.HLDTE = ADCK_THBCR_HW_TRIGGER_ENABLE;
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
 * @brief This function is called by @ref R_ADCK_ScanGroupStart to setup virtual channel settings for scan group
 *
 * @param[in]   p_reg            Pointer to registers of this ADCK instance
 * @param[in]   p_sg_cfg         Pointer to scan group configuration
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_scan_group_vc_init (R_ADCK1_Type * const p_reg, adck_sg_generic_cfg_t const * p_sg_cfg)
{
    /* Setting virtual channels for scan group */
    for (uint32_t channel = p_sg_cfg->start_vc_ptr, cnt = 0U; channel <= p_sg_cfg->end_vc_ptr; channel++, cnt++)
    {
        p_reg->ADCKnVCR[channel] = p_sg_cfg->p_vc_cfg[cnt].vcr;
    }
}

/*******************************************************************************************************************//**
 * @brief Stop the scan group specified by scan_group
 *
 * @param[in]  p_instance_ctrl  Pointer to this ADCK instance control
 * @param[in]  scan_group       Scan group to stop
 **********************************************************************************************************************/
static void r_adck_scan_group_stop (adck_instance_ctrl_t * const p_instance_ctrl, adck_scan_group_t const scan_group)
{
    R_ADCK1_Type        * p_reg    = p_instance_ctrl->p_reg;
    adck_sg_cfg_t const * p_sg_cfg = p_instance_ctrl->p_sg_cfg;

    /* Get scan group x trigger method */
    adck_sg_trigger_method_t sgx_trigger_method = p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;

    /* Disable T&H hold trigger if trigger method of scan_group is T&H */
    if (ADCK_SG_TRIGGER_METHOD_THA == sgx_trigger_method)
    {
        /* Disable hold trigger for T&H group A */
        p_reg->ADCKnTHACR_b.HLDTE = 0U;
    }
    else if (ADCK_SG_TRIGGER_METHOD_THB == sgx_trigger_method)
    {
        /* Disable hold trigger for T&H group B */
        p_reg->ADCKnTHBCR_b.HLDTE = 0U;
    }
    else
    {
        /* scan_group trigger method is not T&H so no need to do anything */
    }

    /* Disable HW trigger and A/D timer trigger (for scan group 3 and 4) and scan end interrupt */
    *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGCR0, scan_group) &= (uint8_t) (~(ADCK_SGCR_HW_TRIGGER_ENABLE |
                                                                      ADCK_SGCR_AD_TIMER_TRIGGER_ENABLE |
                                                                      ADCK_SGCR_SCAN_END_INT_MASK));

    /* Check if AD timer is used for scan group */
    if (ADCK_SG_TRIGGER_METHOD_AD_TIMER == sgx_trigger_method)
    {
        /* Request to stop AD timer */
        if (ADCK_SCAN_GROUP_SG3 == scan_group)
        {
            p_reg->ADCKnADTENDCR3 = ADCK_ADTENDCR_AD_TIMER_STOP;
        }
        else if (ADCK_SCAN_GROUP_SG4 == scan_group)
        {
            p_reg->ADCKnADTENDCR4 = ADCK_ADTENDCR_AD_TIMER_STOP;
        }
        else
        {
            /* Should not run into this case */
        }
    }

    /* Disable virtual channels end interrupt of scan group x */
    r_adck_scan_group_vc_deinit(p_reg, p_sg_cfg->p_sg_cfg[scan_group]);

    /* Request to stop scan group */
    *ADCK_SG_REG_ADDR(&p_reg->ADCKnSGSTPCR0, scan_group) = ADCK_SGSTPCR_SG_STOP;

    if (ADCK_SG_TRIGGER_METHOD_AD_TIMER == sgx_trigger_method)
    {
        if (ADCK_SCAN_GROUP_SG3 == scan_group)
        {
            FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCKnSGSR3_b.ADTACT, 0U);
        }
        else if (ADCK_SCAN_GROUP_SG4 == scan_group)
        {
            FSP_HARDWARE_REGISTER_WAIT(p_reg->ADCKnSGSR4_b.ADTACT, 0U);
        }
        else
        {
            /* Should not run into this case */
        }
    }

    /* Wait until scan group is stopped */
    FSP_HARDWARE_REGISTER_WAIT(*ADCK_SG_REG_ADDR(&p_reg->ADCKnSGSR0, scan_group), ADCK_SGSR_SCAN_GROUP_IDLE);

    /* Check if scan group use track and hold */
    if (ADCK_SG_TRIGGER_METHOD_THA == sgx_trigger_method)
    {
        /* Request to disable track and hold units belong to track and hold group A */
        r_adck_th_disable(p_instance_ctrl, ADCK_TH_GROUP_A);
    }
    else if (ADCK_SG_TRIGGER_METHOD_THB == sgx_trigger_method)
    {
        /* Request to disable track and hold units belong to track and hold group B */
        r_adck_th_disable(p_instance_ctrl, ADCK_TH_GROUP_B);
    }
    else
    {
        /* Scan group not use track and hold, no need to do anything */
    }
}

/*******************************************************************************************************************//**
 * @brief This function check whether the scan group specified by scan_group is using track and hold software trigger
 *
 * @param[in]  p_instance_ctrl  Pointer to this ADCK instance control
 * @param[in]  scan_group       Scan group to check
 * @retval     true             Scan group uses track and hold software trigger
 * @retval     false            Scan group is not use track and hold software trigger
 **********************************************************************************************************************/
__STATIC_INLINE bool r_adck_is_th_sw_trigger (adck_instance_ctrl_t * const p_instance_ctrl,
                                              adck_scan_group_t const      scan_group)
{
    adck_extended_cfg_t * p_extend = (adck_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend;
    bool return_val                = true;

    /* Get scan group trigger method */
    adck_sg_trigger_method_t trigger_method = p_instance_ctrl->p_sg_cfg->p_sg_cfg[scan_group]->trigger_method;

    if (ADCK_SG_TRIGGER_METHOD_THA == trigger_method)
    {
        /* Check if track and hold group A is not use software trigger */
        if (ADCK_TH_GROUP_TRIGGER_METHOD_SW != p_extend->th_cfg.trigger_method[ADCK_TH_GROUP_A])
        {
            return_val = false;
        }
    }
    else if (ADCK_SG_TRIGGER_METHOD_THB == trigger_method)
    {
        /* Check if track and hold group B is not use software trigger */
        if (ADCK_TH_GROUP_TRIGGER_METHOD_SW != p_extend->th_cfg.trigger_method[ADCK_TH_GROUP_B])
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
 * @brief This function is called by @ref R_ADCK_ScanGroupStop to disable all virtual channel end interrupt
 * of the scan group
 *
 * @param[in]   p_reg            Pointer to registers of this ADCK instance
 * @param[in]   p_sg_cfg         Pointer to scan group configuration
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_scan_group_vc_deinit (R_ADCK1_Type * const p_reg, adck_sg_generic_cfg_t const * p_sg_cfg)
{
    /* Disable all virtual channel end interrupt for scan group */
    for (uint32_t channel = p_sg_cfg->start_vc_ptr, cnt = 0U; channel <= p_sg_cfg->end_vc_ptr; channel++, cnt++)
    {
        p_reg->ADCKnVCR[channel] = p_sg_cfg->p_vc_cfg[cnt].vcr & (~ADCK_VCR_END_INT_MASK);
    }
}

/*******************************************************************************************************************//**
 * @brief Disable the T&H units which scan group used after stop the scan group.
 * This function is called by @ref R_ADCK_ScanGroupStop to disable T&H units that scan group used
 * This function must be called when the target scan group of th_group is in idle state.
 * @param[in] p_instance_ctrl                   Pointer to ADCK instance control block
 * @param[in] th_group                          T&H group to stop (T&H group A and T&H group B)
 **********************************************************************************************************************/
static void r_adck_th_disable (adck_instance_ctrl_t * const p_instance_ctrl, adck_th_group_t const th_group)
{
    R_ADCK1_Type * p_reg    = p_instance_ctrl->p_reg;
    uint8_t        ther_tha = 0U;
    uint8_t        ther_thb = 0U;
    uint16_t       thgsr    = p_reg->ADCKnTHGSR;

    for (uint32_t th_unit = 0U; th_unit < ADCK_NUMBER_OF_TH_UNIT; th_unit++)
    {
        /* Check if T&H unit z is belong to T&H group A or T&H group B */
        if (ADCK_TH_GROUP_A == (thgsr & 0x1U))
        {
            ther_tha |= 0x1U << th_unit;
        }
        else
        {
            ther_thb |= 0x1U << th_unit;
        }

        thgsr = thgsr >> ADCK_THGSR_TH_UNIT_BIT_OFFSET;
    }

    if (ADCK_TH_GROUP_A == th_group)
    {
        /* Disable T&H units used by T&H group A */
        p_reg->ADCKnTHER &= ~ther_tha;

        /* Mask this T&H group as stopped so user can not use it */
        p_instance_ctrl->th_state[ADCK_TH_GROUP_A] = ADCK_TH_STATE_STOP;
    }
    else
    {
        /* Disable T&H units used by T&H group B */
        p_reg->ADCKnTHER &= ~ther_thb;

        /* Mask this T&H group as stopped so user can not use it */
        p_instance_ctrl->th_state[ADCK_TH_GROUP_B] = ADCK_TH_STATE_STOP;
    }
}

#if (ADCK_CFG_PARAM_CHECKING_ENABLE)

/*******************************************************************************************************************//**
 * @brief This function check the valid of group_mask parameter
 *
 * @param[in] group_mask    group_mask to check
 * @param[in] p_sg_cfg      Scan groups configuration of ADCK
 * @retval    FSP_SUCCESS                           The group mask is valid
 * @retval    FSP_ERR_SGX_DISABLED                  The scan group specified by group mask is disabled
 * @retval    FSP_ERR_PWM_DIAG_DISABLED             PWM-Diag is disabled and group_mask is PWM-Diag
 * @retval    FSP_ERR_SG_DIAG_DISABLED              SG-Diag is disabled and group_mask is SG-Diag
 * @retval    FSP_ERR_ADCK_GROUP_MASK_NOT_SUPPORT   This group_mask is not supported
 **********************************************************************************************************************/
static fsp_err_t r_adck_group_mask_param_check (adc_group_mask_t const group_mask, adck_sg_cfg_t const * p_sg_cfg)
{
    fsp_err_t return_val = FSP_SUCCESS;

    if ((group_mask >= ADC_GROUP_MASK_0) && (group_mask <= ADC_GROUP_MASK_4))
    {
        /* Check if scan group is enabled in scan groups configuration */
        if (NULL == p_sg_cfg->p_sg_cfg[SCH1R(group_mask) - 1])
        {
            return_val = FSP_ERR_SGX_DISABLED;
        }
    }
    else if ((ADC_GROUP_MASK_PWM_DIAG == group_mask))
    {
        /*  Check if scan group PWM-Diag is disabled */
        if (ADCK_PWM_DIAG_CFG_DISABLE == p_sg_cfg->pwm_diag_cfg)
        {
            return_val = FSP_ERR_PWM_DIAG_DISABLED;
        }
    }
    else if ((ADC_GROUP_MASK_SG_DIAG == group_mask))
    {
        /* Check if scan group SG-Diag is disabled */
        if (NULL == p_sg_cfg->p_sg_diag_cfg)
        {
            return_val = FSP_ERR_SG_DIAG_DISABLED;
        }
    }
    else
    {
        /* Not support cases */
        return_val = FSP_ERR_ADCK_GROUP_MASK_NOT_SUPPORT;
    }

    return return_val;
}

#endif                                 /* ADCK_CFG_PARAM_CHECKING_ENABLE */

/*******************************************************************************************************************//**
 * Calls user callback.
 *
 * @param[in]     p_ctrl     Pointer to ADCK instance control block
 * @param[in]     p_args     Pointer to arguments on stack
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_call_callback (adck_instance_ctrl_t * const p_ctrl, adc_callback_args_t * const p_args)
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
 * This function handle common scan end interrupt for ADCK.
 *
 * @param[in]  event                   Event that triggered the ISR
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_scan_end_common_handle (adc_event_t const event)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* Scan end interrupt is pulse interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    adc_callback_args_t    args            = {0U};

    args.unit       = p_instance_ctrl->p_cfg->unit;
    args.event      = event;
    args.p_context  = p_instance_ctrl->p_context;
    args.channel    = (adc_channel_t) ADCK_CALLBACK_ARGS_CHANNEL_NOT_USE;
    args.group_mask = (adc_group_mask_t) ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE;

    /* Call user callback function */
    r_adck_call_callback(p_instance_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * This function handle ID error interrupt for ADCK.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCK instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_id_error_handle (adck_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCK1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_ID_ERROR;
    args.p_context = p_instance_ctrl;

    /* Get the channel in which ID error occurred */
    args.channel    = (adc_channel_t) p_reg->ADCKnIDER_b.IDECAP;
    args.group_mask = (adc_group_mask_t) ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE;

    /* Clear the ID error status flag */
    p_reg->ADCKnECR_b.IDEC = ADCK_ECR_ID_ERROR_STATUS_CLEAR;

    /* Call user callback function to notify an ID error had occurred */
    r_adck_call_callback(p_instance_ctrl, &args);
}

/*******************************************************************************************************************//**
 * This function handle overwrite error interrupt for ADCK.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCK instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_overwrite_error_handle (adck_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCK1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_OVERWRITE_ERROR;
    args.p_context = p_instance_ctrl;

    /* Get the channel in which overwrite error occurred */
    args.channel    = (adc_channel_t) p_reg->ADCKnOWER_b.OWECAP;
    args.group_mask = (adc_group_mask_t) ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE;

    /* Clear the overwrite error status flag */
    p_reg->ADCKnECR = ADCK_ECR_OVERWRITE_ERROR_STATUS_CLEAR;

    /* Call user callback function to notify an overwrite error had occurred */
    r_adck_call_callback(p_instance_ctrl, &args);
}

/*******************************************************************************************************************//**
 * This function handle trigger overlap error interrupt for ADCK.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCK instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_trigger_overlap_error_handle (adck_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCK1_Type      * p_reg     = p_instance_ctrl->p_reg;
    uint8_t             tocer_reg = p_reg->ADCKnTOCER;
    uint16_t            ecr_reg   = 0U;
    adc_callback_args_t args      = {0U};

    args.unit      = p_instance_ctrl->p_cfg->unit;
    args.event     = ADC_EVENT_TRIGGER_OVERLAP_ERROR;
    args.p_context = p_instance_ctrl->p_context;
    args.channel   = (adc_channel_t) ADCK_CALLBACK_ARGS_CHANNEL_NOT_USE;

    for (uint32_t scan_group = 0U; scan_group < ADCK_NUMBER_OF_SG; scan_group++)
    {
        /* Check if trigger overlap had occurred in scan group */
        if (0U != (tocer_reg & (ADCK_TOCER_SG0_MASK << scan_group)))
        {
            args.group_mask = (adc_group_mask_t) (0x1U << scan_group);

            /* Clear the trigger overlap interrupt for scan group 0 */
            ecr_reg |= ADCK_ECR_TRIGGER_OVERLAP_ERROR_SG0_CLEAR << scan_group;

            /* Call user callback function to notify an trigger overlap error had occurred */
            r_adck_call_callback(p_instance_ctrl, &args);
        }
    }

    /* Check if trigger overlap had occurred in SG-Diag */
    if (0U != (tocer_reg & ADCK_TOCER_SG_DIAG_MASK))
    {
        args.group_mask = ADC_GROUP_MASK_SG_DIAG;

        /* Clear the trigger overlap interrupt for SG-Diag */
        ecr_reg |= ADCK_ECR_TRIGGER_OVERLAP_ERROR_SG_DIAG_CLEAR;

        /* Call user callback function to notify an trigger overlap error had occurred */
        r_adck_call_callback(p_instance_ctrl, &args);
    }

    /* Request to clear the trigger overlap activated */
    p_reg->ADCKnECR = ecr_reg;
}

/*******************************************************************************************************************//**
 * This function handle Upper/Lower limit check error interrupt for ADCK.
 *
 * @param[in]     p_instance_ctrl     Pointer to ADCK instance control block.
 **********************************************************************************************************************/
__STATIC_INLINE void r_adck_upper_lower_error_handle (adck_instance_ctrl_t * const p_instance_ctrl)
{
    R_ADCK1_Type      * p_reg = p_instance_ctrl->p_reg;
    adc_callback_args_t args  = {0U};

    args.unit       = p_instance_ctrl->p_cfg->unit;
    args.event      = ADC_EVENT_UPPER_LOWER_LIMIT_ERROR;
    args.p_context  = p_instance_ctrl->p_context;
    args.group_mask = (adc_group_mask_t) ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE;

#if (BSP_MCU_GROUP_RH850U2Cx)

    /* Check if Upper/Lower Limit error had occured in PWM-Diag */
    if (0U != p_reg->ADCKnPWVCLMSR_b.PWVCLMS)
    {
        args.channel = ADC_CHANNEL_PWM_DIAG;

        /* Clear the PWM-diag upper/lower limit interrupt status */
        p_reg->ADCKnPWVCLMSCR |= ADCK_PWM_DIAG_UL_STATUS_CLEAR;

        /* Call user callback function to notify a PWM-diag upper/lower limit check error had occurred */
        r_adck_call_callback(p_instance_ctrl, &args);
    }
#endif                                 /* BSP_MCU_GROUP_RH850U2Cx */

    /* Check if Upper/Lower Limit error had occured in SG-Diag */
    if (0U != p_reg->ADCKnSDVCLMSR)
    {
        uint32_t channel = SCH1R(p_reg->ADCKnSDVCLMSR) - 1;
        args.channel = (adc_channel_t) (ADC_CHANNEL_SG_DIAG_CHANNEL_0 + channel);

        /* Clear the SG-diag upper/lower limit interrupt status */
        p_reg->ADCKnSDVCLMSCR |= (ADCK_SG_DIAG_UL_STATUS_CLEAR << channel);

        /* Call user callback function to notify a SG-diag upper/lower limit check error had occurred */
        r_adck_call_callback(p_instance_ctrl, &args);
    }

    /* Check if Upper/Lower Limit error had occured in Virtual Channel and get the channel number */
    if (0U != p_reg->ADCKnVCLMSR1)
    {
        /* Get the lower channel (0..31) that interrupt had occurred */
        uint32_t channel = SCH1R(p_reg->ADCKnVCLMSR1) - 1;
        args.channel = (adc_channel_t) channel;

        /* Clear the Upper/Lower limit interrupt status for the current channel */
        p_reg->ADCKnVCLMSCR1 |= (ADCK_VIRTUAL_CHANNEL_UL_STATUS_CLEAR << channel);

        /* Call user callback function to notify a Virtual Channel upper/lower limit check error had occurred */
        r_adck_call_callback(p_instance_ctrl, &args);
    }
    else if (0U != p_reg->ADCKnVCLMSR2)
    {
        /* Get the higher channel (32..63) that interrupt had occurred */
        uint32_t channel = SCH1R(p_reg->ADCKnVCLMSR2) - 1;
        args.channel = (adc_channel_t) (channel + 32);

        /* Clear the Upper/Lower limit interrupt status for the current channel */
        p_reg->ADCKnVCLMSCR2 |= (ADCK_VIRTUAL_CHANNEL_UL_STATUS_CLEAR << channel);

        /* Call user callback function to notify a Virtual Channel upper/lower limit check error had occurred */
        r_adck_call_callback(p_instance_ctrl, &args);
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
 * Scan group 0 scan end interrupt handle. The adck_sg0_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 0 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_0 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg0_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_0);
}

/*******************************************************************************************************************//**
 * Scan group 1 scan end interrupt handle. The adck_sg1_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 0 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_1 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg1_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_1);
}

/*******************************************************************************************************************//**
 * Scan group 2 scan end interrupt handle. The adck_sg2_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 2 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_2 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg2_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_2);
}

/*******************************************************************************************************************//**
 * Scan group 3 scan end interrupt handle. The adck_sg3_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 3 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_3 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg3_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_3);
}

/*******************************************************************************************************************//**
 * Scan group 4 scan end interrupt handle. The adck_sg4_scan_end_isr interrupt fires when scan end interrupt
 * of scan group 4 is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_GROUP_4 if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg4_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_GROUP_4);
}

/*******************************************************************************************************************//**
 * Scan group SG-Diag scan end interrupt handle. The adck_sg_diag_scan_end_isr interrupt fires when scan end interrupt
 * of SG-Diag is activated.
 * The user callback function is called with the event ADC_EVENT_SCAN_COMPLETE_SG_DIAG if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_sg_diag_scan_end_isr (void)
{
    r_adck_scan_end_common_handle(ADC_EVENT_SCAN_COMPLETE_SG_DIAG);
}

/*******************************************************************************************************************//**
 * ADCK error interrupt service routine. The adck_error_isr interrupt fires when trigger overlap, overwrite error,
 * ID error, upper/lower limit check interrupt is activated.
 * The user callback function is called with the corresponding even code if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_error_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* ADCK error interrupt is pulse interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    R_ADCK1_Type         * p_reg           = p_instance_ctrl->p_reg;

    /* Check if ID error interrupt had happened and handle it */
    if ((ADCK_ID_ERROR_INT_ENABLE == p_reg->ADCKnSFTCR_b.IDEIE) && (0U != p_reg->ADCKnIDER))
    {
        r_adck_id_error_handle(p_instance_ctrl);
    }

    /* Check if overwrite error interrupt had happened and handle it */
    if ((ADCK_OVERWRITE_ERROR_INT_ENABLE == p_reg->ADCKnSFTCR_b.OWEIE) && (0U != p_reg->ADCKnOWER))
    {
        r_adck_overwrite_error_handle(p_instance_ctrl);
    }

    /* Check if trigger overlap error interrupt had happened and handle it */
    if ((ADCK_TRIGGER_OVERLAP_ENABLE == p_reg->ADCKnSFTCR_b.TOCEIE) && (0U != p_reg->ADCKnTOCER))
    {
        r_adck_trigger_overlap_error_handle(p_instance_ctrl);
    }

    /* Check if upper/lower limit error interrupt had happened and handle it */
    if (((0U != p_reg->ADCKnVCLMINTER1) && (0U != p_reg->ADCKnVCLMSR1)) ||
        ((0U != p_reg->ADCKnVCLMINTER2) && (0U != p_reg->ADCKnVCLMSR2)) ||
#if (BSP_MCU_GROUP_RH850U2Cx)
        ((ADCK_PWM_DIAG_UL_LIMIT_INT_ENABLE == p_reg->ADCKnPWVCLMINTER_b.PWADULIE) &&
         (0U != p_reg->ADCKnPWVCLMSR_b.PWVCLMS)) ||
#endif                                 /* BSP_MCU_GROUP_RH850U2Cx */
        ((ADCK_SG_DIAG_UL_LIMIT_INT_ENABLE == p_reg->ADCKnSDVCLMINTER_b.SDADULIE) && (0U != p_reg->ADCKnSDVCLMSR)))
    {
        r_adck_upper_lower_error_handle(p_instance_ctrl);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

/*******************************************************************************************************************//**
 * ADCK MPX interrupt service routine. The adck_mpx_isr interrupt fires at beginning of MPX normal A/D conversion
 * or MPX addition mode A/D conversion.
 * The user callback function is called with event code ADC_EVENT_MPX_CONVERSION if it is available.
 **********************************************************************************************************************/
BSP_INTERRUPT_ATTRIBUTE void adck_mpx_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    /* MPX interrupt is pulse interrupt so clear the flag at beginning of isr */
    R_BSP_IrqStatusClear(irq);

    /* Recover ISR context saved in open. */
    adck_instance_ctrl_t * p_instance_ctrl = (adck_instance_ctrl_t *) R_FSP_IsrContextGet(irq);
    adc_callback_args_t    args            = {0U};

    args.unit       = p_instance_ctrl->p_cfg->unit;
    args.event      = ADC_EVENT_MPX_CONVERSION;
    args.p_context  = p_instance_ctrl;
    args.channel    = (adc_channel_t) ADCK_CALLBACK_ARGS_CHANNEL_NOT_USE;
    args.group_mask = (adc_group_mask_t) ADCK_CALLBACK_ARGS_GROUP_MASK_NOT_USE;

    /* Call user callback function to notify an MPX interrupt had occurred */
    r_adck_call_callback(p_instance_ctrl, &args);

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

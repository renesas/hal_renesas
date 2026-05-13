/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_pic.h"
#include <stdint.h>

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define PIC_OPEN                                                 (0x504943ULL)

/* Three Phase Pulse Input Control Function */
#define PIC_TSG0HALLSEL_THREE_PHASE_PULSE                        ((uint8_t) (0x01))
#define PIC_REG50_THREE_PHASE_PULSE_TSG30_SEL_ENCA               ((uint8_t) (0x00))
#define PIC_REG50_THREE_PHASE_PULSE_TSG30OPCI0_SEL_INTTAUD0I5    ((uint8_t) (0x03U))
#define PIC_REG50_THREE_PHASE_PULSE_TSG30OPCI1_SEL_INTTAUD0I7    ((uint8_t) (0x00U))
#define PIC_REG200_THREE_PHASE_PULSE_TAUD0TTIN_SEL_TSG30         ((uint32_t) (0x02))
#define PIC_REG200_THREE_PHASE_PULSE_TAUD0TTIN_INPUT_SEL         ((uint8_t) (0x0F))
#define PIC_TAUD0SEL_THREE_PHASE_PULSE_INPUT_SEL                 ((uint32_t) (0xFFFF00FF))
#define PIC_TSG1HALLSEL_THREE_PHASE_PULSE                        ((uint8_t) (0x02))
#define PIC_REG51_THREE_PHASE_PULSE_TSG31_SEL_ENCA               ((uint8_t) (0x01))
#define PIC_REG51_THREE_PHASE_PULSE_TSG31OPCI0_SEL_INTTAUD1I5    ((uint8_t) (0x03U))
#define PIC_REG51_THREE_PHASE_PULSE_TSG31OPCI1_SEL_INTTAUD1I7    ((uint8_t) (0x00U))
#define PIC_REG210_THREE_PHASE_PULSE_TAUD1TTIN_SEL_TSG31         ((uint32_t) (0x02))
#define PIC_REG210_THREE_PHASE_PULSE_TAUD1TTIN_INPUT_SEL         ((uint8_t) (0x0F))
#define PIC_TAUD1SEL_THREE_PHASE_PULSE_INPUT_SEL                 ((uint32_t) (0xFFFF00FF))

/* Two Phase Encoder Control Function */
#define PIC_REG50_TWO_PHASE_ENCODER_INTENCAI_SEL                 ((uint8_t) (0x00U))
#define PIC_REG51_TWO_PHASE_ENCODER_INTENCAI_SEL                 ((uint8_t) (0x00U))
#define PIC_REG30_TW0_PHASE_ENCODER_METHOD12                     ((uint32_t) (0x005B0FC3UL))
#define PIC_REG30_TWO_PHASE_ENCODER_METHOD3                      ((uint32_t) (0x00430FC3UL))
#define PIC_REG30_TWO_PHASE_ENCODER_METHOD3_ENCA1_SEL_ENCA0      ((uint32_t) (0x0D40UL))

/* Three Phase Encoder Control Function */
#define PIC_TSG0HALLSEL_THREE_PHASE_ENCODER                      ((uint8_t) (0x01U))
#define PIC_REG50_THREE_PHASE_ENCODER                            ((uint8_t) (0x00U))
#define PIC_REG30_THREE_PHASE_ENCODER_ENCA0_SEL_TSG30            ((uint8_t) (0x03U))
#define PIC_REG30_THREE_PHASE_ENCODER_ENCA0_INPUT_SEL            ((uint8_t) 0x00U)
#define PIC_TSG1HALLSEL_THREE_PHASE_ENCODER                      ((uint8_t) (0x01U))
#define PIC_REG51_THREE_PHASE_ENCODER                            ((uint8_t) (0x01U))
#define PIC_REG30_THREE_PHASE_ENCODER_ENCA1_SEL_TSG31            ((uint8_t) (0x02U))

/* Encoder Capture Trigger Select Function */
#define PIC_ENCA0_CAPTURE_TRIGGER_GET_REG30(SIGNAL)                ((uint8_t) (SIGNAL))
#define PIC_ENCA1_CAPTURE_TRIGGER_GET_REG30(SIGNAL)                ((uint8_t) ((SIGNAL < \
                                                                                0x02) ? (SIGNAL ^ 0x01) : SIGNAL))
#define PIC_ENCA_CAPTURE_TRIGGER_ENCAI_INTTAUDI_GET_MSK(SIGNAL)    ((uint8_t) ((SIGNAL >> 0x04) & 0x0F))

/* Trigger Pulse Width Measurement Function */
#define PIC_REG30_TRIGGER_PULSE_WIDTH_MEASURE          ((uint32_t) (0x005B03C3UL))
#define PIC_TAUDSEL_TRIGGER_PULSE_WIDTH_MEASURE        ((uint32_t) (0x001FUL))

/*  High Accuracy Triangle Wave PWM Output Function with Dead Time */
#define PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0          ((uint32_t) (0x1U))
#define PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2              ((uint32_t) (0x2U))
#define PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL             ((uint32_t) (0x00U))
#define PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H           ((uint32_t) (0x2U))
#define PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H           ((uint32_t) (0x4U))
#define PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L           ((uint32_t) (0x3U))
#define PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L           ((uint32_t) (0x5U))

/* PWM Output Function with Dead Time */
#define PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2            ((uint32_t) (0x2U))

/* Delay Pulse Output Function with Dead Time */
#define PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2    ((uint32_t) (0x2U))

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/

static void r_pic_reset_value_register(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_simultaneous_start_trigger_func(pic_instance_ctrl_t * const p_instance_ctrl);

#if (!BSP_FEATURE_PIC_PIC1_ONLY_PIC12_IS_AVAILABLE)
 #if (BSP_FEATURE_PIC_SWITCH_TSG_OUT)
static void r_pic_switch_tsg_output_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #endif
 #if (BSP_FEATURE_PIC_TSG_START_TRIG)
static void r_pic_tsg_simultaneous_start_trigger_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #endif
static void r_pic_tsg_sync_start_clear_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_three_phase_pulse_input_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_encoder_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_two_phase_encoder_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_three_phase_encoder_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_hiz_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_encoder_capture_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_int_signal_output_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_trigger_pulse_with_measurement_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #if (BSP_FEATURE_PIC_ENCA_INPUT_SEL)
static void r_pic_enca_input_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #endif
static void r_pic_triangle_wave_pwm_output_with_deadtime(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_timer_input_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_pwm_output_with_deadtime(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_delay_pulse_output_with_deadtime(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_timer_output_monitor_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #if (BSP_FEATURE_PIC_TIMER_INPUT_MONITOR)
static void r_pic_timer_input_monitor_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #endif
 #if (BSP_FEATURE_PIC_TAUD_INPUT_SEL)
static void r_pic_taud_input_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

 #endif
#endif
#if (BSP_FEATURE_PIC_ADCK_TRIG_SEL)
static void r_pic_adck_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_ADCJ_TRIG_SEL)
static void r_pic_adcj_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_DSADC_TRIG_SEL)
static void r_pic_dsadc_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_CADC_TRIG_SEL)
static void r_pic_cadc_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_EMU_FEATURE)
static void r_pic_emu_peak_trough_intr_adc_trigger_select_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_FCMP_FEATURE)
static void r_pic_fcmp_output_control_func(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_fcmp_input_sel(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_HRPWM_FEATURE)
static void r_pic_hrpwm_control_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_DFE_TRIG_SEL)
static void r_pic_dfe_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_PSI5_TRIG_SEL)
static void r_pic_psi5_timestamp_sync_pulse_sig_select_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL)
static void r_pic_psi5s_timestamp_sync_pulse_sig_select_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_ENCA_TRIG_SEL)
static void r_pic_enca_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
static void r_pic_gtm_tim_input_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)
static void r_pic_gtm_tio_input_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);
#endif
#if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL)
static void r_pic_enca_enc_input_sel(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
static void r_pic_hiz_ctrl_gtm_out(pic_instance_ctrl_t * const p_instance_ctrl);

#if (BSP_FEATURE_PIC_RDC_FEATURE)
static void r_pic_rdc_latch_trigger(pic_instance_ctrl_t * const p_instance_ctrl);
static void r_pic_rdc_excit_signal_input(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE)
static void r_pic_can_gtm_io_sharing(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_MSPI_TRIG_SEL)
static void r_pic_mspi_trigger_sel_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL)
static void r_pic_gtm_dtm_input_sel_abfg_func(pic_instance_ctrl_t * const p_instance_ctrl);

#endif
#if (BSP_FEATURE_PIC_TSG3_INPUT_GTM)
static void r_pic_tsg3_input_gtm(pic_instance_ctrl_t * const p_instance_ctrl);

#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Global Variables
 **********************************************************************************************************************/
const pic_api_t g_pic_on_pic =
{
    .open  = R_PIC_Open,
    .close = R_PIC_Close
};

/*******************************************************************************************************************//**
 * @addtogroup PIC
 * @{
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * This functions initializes for PIC module. Implements @ref pic_api_t::open.
 *
 * This function performs the following tasks:
 * - Performs parameter checking and processes error conditions.
 * - Configures the peripheral registers according to the configuration.
 * - Initialize the control structure for use in other @ref PIC_API functions.
 *
 * @retval  FSP_SUCCESS                    Unit opened successfully.
 * @retval  FSP_ERR_ASSERTION              Pointer to PIC control block or configuration structure is NULL.
 * @retval  FSP_ERR_ALREADY_OPEN           Control block has already been opened. Call close() then open() to
 *                                         reconfigure.
 **********************************************************************************************************************/
fsp_err_t R_PIC_Open (pic_ctrl_t * const p_api_ctrl,pic_cfg_t const * const p_cfg)
{
    pic_instance_ctrl_t * p_ctrl = (pic_instance_ctrl_t *) p_api_ctrl;

#if PIC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PIC_OPEN != p_ctrl->open,FSP_ERR_ALREADY_OPEN);
    FSP_ASSERT(NULL != p_cfg);
    FSP_ASSERT(NULL != p_cfg->p_extend);
#endif

#if (BSP_FEATURE_PIC_FEATURE_U2BX)
    p_ctrl->p_regs_pic10 = R_PIC10;         ///< Base register for PIC10.
    p_ctrl->p_regs_pic11 = R_PIC11;         ///< Base register for PIC11.
    p_ctrl->p_regs_pic12 = R_PIC12;         ///< Base register for PIC12.
    p_ctrl->p_regs_pic13 = R_PIC13;         ///< Base register for PIC13.
    p_ctrl->p_regs_pic20 = R_PIC20;         ///< Base register for PIC20.
    p_ctrl->p_regs_pic21 = R_PIC21;         ///< Base register for PIC21.
    p_ctrl->p_regs_pic22 = R_PIC22;         ///< Base register for PIC22.
 #if (BSP_FEATURE_PIC_FEATURE_U2BX)
    p_ctrl->p_regs_pic23 = R_PIC23;         ///< Base register for PIC23.
 #endif
    p_ctrl->p_regs_pic24 = R_PIC24;         ///< Base register for PIC24.
#elif (BSP_FEATURE_PIC_FEATURE_U2CX)
    p_ctrl->p_regs_pic10     = R_PIC10;     ///< Base register for PIC10.
    p_ctrl->p_regs_pic1_selb = R_PIC1_SELB; ///< Base register for PIC1_SELB.
    p_ctrl->p_regs_pic20     = R_PIC20;     ///< Base register for PIC20.
    p_ctrl->p_regs_pic21     = R_PIC21;     ///< Base register for PIC21.
    p_ctrl->p_regs_pic22     = R_PIC22;     ///< Base register for PIC22.
    p_ctrl->p_regs_pic2_selb = R_PIC2_SELB; ///< Base register for PIC2_SELB.
#elif (BSP_FEATURE_PIC_FEATURE_U2AX)
    p_ctrl->p_regs_pic10     = R_PIC1;      ///< Base register for PIC1.
    p_ctrl->p_regs_pic1_selb = R_PIC1_SELB; ///< Base register for PIC1_SELB.
    p_ctrl->p_regs_pic20     = R_PIC20;     ///< Base register for PIC20.
    p_ctrl->p_regs_pic21     = R_PIC21;     ///< Base register for PIC21.
    p_ctrl->p_regs_pic22     = R_PIC22;     ///< Base register for PIC22.
#endif

    p_ctrl->p_cfg = p_cfg;
#if (BSP_FEATURE_PIC_TSG_START_TRIG)

    /* Configure hardware for TSG Simultaneous start trigger function. */
    r_pic_tsg_simultaneous_start_trigger_func(p_ctrl);
#endif

    /* Configure hardware for Simultaneous start trigger function. */
    r_pic_simultaneous_start_trigger_func(p_ctrl);
#if (!BSP_FEATURE_PIC_PIC1_ONLY_PIC12_IS_AVAILABLE)

    /* Configure hardware for TSG3 synchronous start and clear function. */
    r_pic_tsg_sync_start_clear_func(p_ctrl);

    /* Configure for Encoder Control function. */
    r_pic_encoder_control_func(p_ctrl);

    /* Configuration for Hi-Z Control Function. */
    r_pic_hiz_control_func(p_ctrl);

    /* Configuration for Encoder Capture Trigger Select Function. */
    r_pic_encoder_capture_trigger_sel_func(p_ctrl);
 #if (BSP_FEATURE_PIC_ENCA_INPUT_SEL)

    /* Configuration for ENCA Input Select Function. */
    r_pic_enca_input_sel_func(p_ctrl);
 #endif
 #if (BSP_FEATURE_PIC_SWITCH_TSG_OUT)

    /* Configure hardware for Switch Function between TSG Output and Low/High Level Output. */
    r_pic_switch_tsg_output_func(p_ctrl);
 #endif

    if (PIC10_FUNC_TIMER_INPUT_SEL == p_cfg->pic10_function_mode)
    {
        /* Configure hardware for Timer Input Selection function. */
        r_pic_timer_input_sel_func(p_ctrl);
    }
    else if (PIC10_FUNC_HIGH_ACCURACY_TRIANGLE_WAVE_PWM == p_cfg->pic10_function_mode)
    {
        /* Configure hardware for High Accuracy Triangle Wave PWM Output Function with Dead Time. */
        r_pic_triangle_wave_pwm_output_with_deadtime(p_ctrl);
    }
    else if (PIC10_FUNC_PWM_OUTPUT == p_cfg->pic10_function_mode)
    {
        /* Configure hardware for PWM Output with Deadtime Function. */
        r_pic_pwm_output_with_deadtime(p_ctrl);
    }
    else if (PIC10_FUNC_DELAY_PULSE_OUTPUT == p_cfg->pic10_function_mode)
    {
        /* Configure hardware for Delay Pulse Output Function with Dead Time. */
        r_pic_delay_pulse_output_with_deadtime(p_ctrl);
    }
    else if (PIC10_FUNC_TRIGGER_PULSE_WIDTH_MEASUREMENT == p_cfg->pic10_function_mode)
    {
        /* Configuration for Trigger Pulse Width Measurement Function.*/
        r_pic_trigger_pulse_with_measurement_func(p_ctrl);
    }
    else
    {
        /* Do nothing. */
    }

    /* Configuration for Timer Output Monitor Function. */
    r_pic_timer_output_monitor_func(p_ctrl);
 #if (BSP_FEATURE_PIC_TIMER_INPUT_MONITOR)

    /* Configuration for Timer Input Monitor Function. */
    r_pic_timer_input_monitor_func(p_ctrl);
 #endif

    /* Configuration for INT signal output selection function.*/
    r_pic_int_signal_output_sel_func(p_ctrl);

 #if (BSP_FEATURE_PIC_TAUD_INPUT_SEL)

    /* Configuration for TAUD Input Select Function.*/
    r_pic_taud_input_sel_func(p_ctrl);
 #endif
#endif
#if (BSP_FEATURE_PIC_ADCK_TRIG_SEL)

    /* Configure hardware registers for ADCK trigger selection function. */
    r_pic_adck_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_ADCJ_TRIG_SEL)

    /* Configure hardware registers for ADCJ trigger selection function. */
    r_pic_adcj_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_DSADC_TRIG_SEL)
    /* Configure hardware registers for DSADC trigger selection function. */
    r_pic_dsadc_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_CADC_TRIG_SEL)
    /* Configure hardware registers for CADC trigger selection function. */
    r_pic_cadc_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_EMU_FEATURE)

    /* Configure hardware for EMU Peak/Trough Interrupt and A/D Conversion Trigger Selection Function. */
    r_pic_emu_peak_trough_intr_adc_trigger_select_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_FCMP_FEATURE)

    /* Configuration for FCMP Output Control. */
    r_pic_fcmp_output_control_func(p_ctrl);

    /* Configuration for FCMP Input Selection Control. */
    r_pic_fcmp_input_sel(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_HRPWM_FEATURE)

    /* Configuration for HRPWM Control Function. */
    r_pic_hrpwm_control_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_DFE_TRIG_SEL)

    /* Configuration for PSI5 Trigger Selection Function.*/
    r_pic_dfe_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_PSI5_TRIG_SEL)

    /* Configuration for PSI5 Trigger Selection Function.*/
    r_pic_psi5_timestamp_sync_pulse_sig_select_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL)

    /* Configuration for PSI5S Trigger Selection Function.*/
    r_pic_psi5s_timestamp_sync_pulse_sig_select_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_ENCA_TRIG_SEL)

    /* Configuration for ENCA Trigger Selection Function.*/
    r_pic_enca_trigger_sel_func(p_ctrl);
#endif

    /* Configuration for GTM Timer Input (TIM) Selection Function.*/
    r_pic_gtm_tim_input_sel_func(p_ctrl);

#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)
    /* Configuration for GTM Timer Input (TIO) Selection Function.*/
    r_pic_gtm_tio_input_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL)

    /* Configuration for INT signal output selection function.*/
    r_pic_enca_enc_input_sel(p_ctrl);
#endif

    /* Configuration for Hi-Z Control Function Over External Pin for GTM Output.*/
    r_pic_hiz_ctrl_gtm_out(p_ctrl);

#if (BSP_FEATURE_PIC_RDC_FEATURE)

    /* Configuration for RDC Angle/Angular Velocity Latch Trigger.*/
    r_pic_rdc_latch_trigger(p_ctrl);

    /* Configuration for RDC Excitation Signal Input. */
    r_pic_rdc_excit_signal_input(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE)

    /* Configuration for IO sharing CAN / GTM- Internal arbitration. */
    r_pic_can_gtm_io_sharing(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_MSPI_TRIG_SEL)

    /* Configuration for MSPI Trigger Selection Function. */
    r_pic_mspi_trigger_sel_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL)

    /* Configuration for GTM DTM Input Selection Function. */
    r_pic_gtm_dtm_input_sel_abfg_func(p_ctrl);
#endif
#if (BSP_FEATURE_PIC_TSG3_INPUT_GTM)

    /* Configuration for TSG3 inputs from GTM. */
    r_pic_tsg3_input_gtm(p_ctrl);
#endif
    p_ctrl->open = PIC_OPEN;

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * Resets PIC registers. Implements @ref pic_api_t::close
 *
 * @retval  FSP_SUCCESS              Unit successfully closed.
 * @retval  FSP_ERR_ASSERTION        Pointer to PIC control block is NULL.
 * @retval  FSP_ERR_NOT_OPEN         The control block has not been opened.
 **********************************************************************************************************************/
fsp_err_t R_PIC_Close (pic_ctrl_t * const p_api_ctrl)
{
    pic_instance_ctrl_t * p_ctrl = (pic_instance_ctrl_t *) p_api_ctrl;

#if PIC_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(NULL != p_ctrl);
    FSP_ERROR_RETURN(PIC_OPEN == p_ctrl->open,FSP_ERR_NOT_OPEN);
#endif
#if (BSP_FEATURE_PIC_HRPWM_FEATURE)

    /* Stops HRPWM high low fix control for TSG31. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHLFIXCTT_b.PIC2HRPWMHLFIXCTT0 = 1U;

    /* Stops HRPWM high low fix control for TSG32. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHLFIXCTT_b.PIC2HRPWMHLFIXCTT1 = 1U;

    /*  Stops HRPWM Hi-Z control for ATOM2_IN1_HRPWM_HIZ. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHIZCTT_b.PIC2HRPWMHIZCTT3 = 1U;

    /*  Stops HRPWM Hi-Z control for ATOM2_IN0_HRPWM_HIZ. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHIZCTT_b.PIC2HRPWMHIZCTT2 = 1U;

    /*  Stops HRPWM Hi-Z control for ATOM1_IN1_HRPWM_HIZ. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHIZCTT_b.PIC2HRPWMHIZCTT1 = 1U;

    /*  Stops HRPWM Hi-Z control for ATOM1_IN0_HRPWM_HIZ. */
    p_ctrl->p_regs_pic24->PIC2HRPWMHIZCTT_b.PIC2HRPWMHIZCTT0 = 1U;
#endif

    /* Set state to closed. */
    p_ctrl->open = 0;

    /* Reset value register. */
    r_pic_reset_value_register(p_ctrl);

    return FSP_SUCCESS;
}

/*******************************************************************************************************************//**
 * @} (end addtogroup PIC)
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * Reset value register.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_reset_value_register (pic_instance_ctrl_t * const p_instance_ctrl)
{
    p_instance_ctrl->p_regs_pic10->PIC1TSGHALLSEL = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG50      = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG51      = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG202     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG201     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG203     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG212     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG211     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG213     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG200     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG210     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG202     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG212     = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1REG30      = 0UL;
    p_instance_ctrl->p_regs_pic10->PIC1REG31      = 0UL;
    p_instance_ctrl->p_regs_pic10->PIC1INI01      = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1INI11      = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL400  = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL410  = 0U;
#if (BSP_FEATURE_PIC_ADCJ_TRIG_SEL)
    for (uint8_t adcj_unit = 0; adcj_unit < BSP_FEATURE_PIC_ADC_NUMBER_UNIT; adcj_unit++)
    {
        for (uint8_t adcj_sg = 0; adcj_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adcj_sg++)
        {
            p_instance_ctrl->p_regs_pic20->PIC2ADCJ[adcj_unit].TSEL[adcj_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN5[adcj_unit].SG[adcj_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN6[adcj_unit].SG[adcj_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN7[adcj_unit].SG[adcj_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN8[adcj_unit].SG[adcj_sg] = 0U;
        }

        p_instance_ctrl->p_regs_pic20->PIC2ADCJ[adcj_unit].EDGSEL = 0U;
    }

    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN4[taud_unit].SG[adck_sg].SG = 0U;
        }
    }

    for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
    {
        p_instance_ctrl->p_regs_pic20->PIC2ADTEN42[adck_sg].SG = 0U;
    }
#endif
#if (BSP_FEATURE_PIC_ADCK_TRIG_SEL)
    for (uint8_t adck_unit = 0; adck_unit < BSP_FEATURE_PIC_ADC_NUMBER_UNIT; adck_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            p_instance_ctrl->p_regs_pic20->PIC20ADCK[adck_unit].TSEL[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21ADCK[adck_unit].TSEL[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN5[adck_unit].SG[adck_sg] = 0U;
 #if (BSP_FEATURE_PIC_ADCK_TRIG_SEL_ATU)
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN7[adck_unit].SG[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN8[adck_unit].SG[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN5[adck_unit].SG[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN6[adck_unit].SG[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN7[adck_unit].SG[adck_sg] = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN8[adck_unit].SG[adck_sg] = 0U;
 #endif
        }

        p_instance_ctrl->p_regs_pic20->PIC20ADCK[adck_unit].EDGSEL = 0U;

        p_instance_ctrl->p_regs_pic21->PIC21ADCK[adck_unit].EDGSEL = 0U;
    }

    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN4[taud_unit].SG[adck_sg].SG = 0U;
        }
    }

    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC21_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN4[taud_unit].SG[adck_sg].SG = 0U;
        }
    }

    for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
    {
        p_instance_ctrl->p_regs_pic20->PIC20ADTEN42[adck_sg].SG = 0U;
    }

    for (uint8_t pic2adtcfg_idx = 0; pic2adtcfg_idx < BSP_FEATURE_PIC_ADTCFG_NUMBER; pic2adtcfg_idx++)
    {
 #if (BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC24)
        p_instance_ctrl->p_regs_pic24->PIC2ADTCFG[pic2adtcfg_idx] = 0U;
 #else
        p_instance_ctrl->p_regs_pic2_selb->PIC2ADTCFG[pic2adtcfg_idx] = 0U;
 #endif
    }
#endif
#if (BSP_FEATURE_PIC_DSADC_TRIG_SEL)
    for (uint8_t dsadc_unit = 0; dsadc_unit < BSP_FEATURE_PIC_DSADTCFG_NUMBER; dsadc_unit++)
    {
        for (uint8_t dsadc_reg = 0; dsadc_reg < BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG; dsadc_reg++)
        {
            p_instance_ctrl->p_regs_pic20->PIC20DSADCATSEL0 = 0U;
            p_instance_ctrl->p_regs_pic20->PIC20DSADCATSEL1 = 0U;

 #if (BSP_FEATURE_PIC_FEATURE_U2BX)
            p_instance_ctrl->p_regs_pic21->PIC21DSADCATSEL0 = 0U;
            p_instance_ctrl->p_regs_pic21->PIC21DSADCATSEL1 = 0U;
 #endif 
#if (BSP_FEATURE_PIC_FEATURE_U2BX)
 #if (BSP_FEATURE_PIC_DSADC_TRIG_SEL_ATU)
        
        if(2 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }
        else if (3 == dsadc_reg) {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }

        if(2 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        else if (3 == dsadc_reg)   {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }

        if(0 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }
        else if (1 == dsadc_reg )
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }
        else if (2 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }
        else
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }

        if(0 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        else if (1 == dsadc_reg )
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        else if (2 == dsadc_reg )
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        else
        {
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
 #endif
#endif
        p_instance_ctrl->p_regs_pic24->PIC2DSADTCFG[dsadc_unit] = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2DSADTCFG[dsadc_unit] = 0U;

        if(0 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }
        else if (1 == dsadc_reg){
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] = 0U;
        }

        if(0 == dsadc_reg)
        {
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        else if (1 == dsadc_reg) {
        p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] = 0U;
        }
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN0 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN1 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN4 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN5 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN6 = 0U;
    }
}
#endif
#if (BSP_FEATURE_PIC_CADC_TRIG_SEL)
    p_instance_ctrl->p_regs_pic22->PIC22CADCATSEL0 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADCATSEL1 = 0U;
 #if (BSP_FEATURE_PIC_FEATURE_U2BX)
    p_instance_ctrl->p_regs_pic23->PIC23CADCATSEL0 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADCATSEL1 = 0U;   
 #endif
#if (BSP_FEATURE_PIC_FEATURE_U2BX)
 #if (BSP_FEATURE_PIC_CADC_TRIG_SEL_ATU)
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN002 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN003 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN102 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN103 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN000 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN001 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN002 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN003 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN100 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN101 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN102 = 0U;
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN103 = 0U;
 #endif
#endif
    p_instance_ctrl->p_regs_pic24->PIC2CADTCFG0   = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN000 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN100 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN001 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN101 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2CADTSEN0   = 0U;
#endif
#if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_CANIOSEN_NUMBER; idx++)
    {
 #if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC24)
        p_instance_ctrl->p_regs_pic24->PIC2CANIOSEN[idx] = 0U;
 #elif (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC2_SELB)
        p_instance_ctrl->p_regs_pic2_selb->PIC2CANIOSEN[idx] = 0U;
 #endif
    }
#endif

#if (2 < BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER)
    p_instance_ctrl->p_regs_pic11->PIC1REG222   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG221   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG223   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG232   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG231   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG233   = 0U;
#endif

#if (BSP_FEATURE_PIC_EMU_FEATURE)
    p_instance_ctrl->p_regs_pic24->PIC2EMUISEN0 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2EMUISEL0 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2EMUISEL1 = 0U;
#endif

#if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL)
 #if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC24)
    p_instance_ctrl->p_regs_pic24->PIC2ENCAISEN0 = 0U;
 #elif (BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC22)
    p_instance_ctrl->p_regs_pic22->PIC2ENCAISEN = 0U;
 #endif
#endif

#if (BSP_FEATURE_PIC_ENCA_TRIG_SEL)
 #if (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22)
    p_instance_ctrl->p_regs_pic22->PIC2ENCA0TSEL0 = 0U;

  #if (1 < BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT)
    p_instance_ctrl->p_regs_pic22->PIC2ENCA1TSEL0 = 0U;
  #endif
 #else
    p_instance_ctrl->p_regs_pic21->PIC2ENCA0TSEL = 0U;
    p_instance_ctrl->p_regs_pic21->PIC2ENCA1TSEL = 0U;
 #endif
 #if (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24)
    p_instance_ctrl->p_regs_pic24->PIC2ENCATCFG0 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2ENCATCFG1 = 0U;
 #elif (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC2_SELB)
    p_instance_ctrl->p_regs_pic2_selb->PIC2ENCATCFG0 = 0U;
    p_instance_ctrl->p_regs_pic2_selb->PIC2ENCATCFG1 = 0U;
 #elif (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22)
    p_instance_ctrl->p_regs_pic22->PIC2ENCATCFG0 = 0U;
    p_instance_ctrl->p_regs_pic22->PIC2ENCATCFG1 = 0U;
 #endif
#endif
#if (BSP_FEATURE_PIC_DFE_TRIG_SEL)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_DFETSEN_NUMBER; idx++)
    {
    p_instance_ctrl->p_regs_pic24->PIC2DFETSEN[idx] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL0 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL1 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL2 = 0U;
    }
#endif
#if (BSP_FEATURE_PIC_PSI5_TRIG_SEL)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_PSI5EN_NUMBER; idx++) 
    {
 #if (BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC24)
        p_instance_ctrl->p_regs_pic24->PIC2PSI5EN[idx] = 0U;

 #elif (BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC2_SELB)
        p_instance_ctrl->p_regs_pic2_selb->PIC2PSI5EN[idx] = 0U;
 #endif
    }
#endif
#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL)
    for (uint8_t psi5s_idx = 0; psi5s_idx < BSP_FEATURE_PIC_PSI5SEN_NUMBER; psi5s_idx++)
    {
 #if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24)
        p_instance_ctrl->p_regs_pic24->PIC2PSI5S0EN[psi5s_idx] = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2PSI5S1EN[psi5s_idx] = 0U;

 #elif (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB)
        p_instance_ctrl->p_regs_pic2_selb->PIC2PSI5S0EN[psi5s_idx] = 0U;

 #elif (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22)

        p_instance_ctrl->p_regs_pic22->PIC2PSI5S0EN[psi5s_idx] = 0U;
        p_instance_ctrl->p_regs_pic22->PIC2PSI5S1EN[psi5s_idx] = 0U;
 #endif
    }
#endif
#if (BSP_FEATURE_PIC_FCMP_FEATURE)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_FCMPISEN_NUMBER_REG; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2FCMPISEN[idx] = 0U;
    }

    p_instance_ctrl->p_regs_pic24->PIC2FCMPOCTL = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2FCMPOLSEN = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2FCMPOHTSGSEN = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2FCMPOLTSGSEN = 0U;

    for (uint8_t idx = 0; idx < PIC_GTMSEN_REG_NUMBER; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2GTMSEN[idx] = 0U;
    }

    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_ABFGODSEN_NUMBER; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2ABFGODSEN[idx] = 0U;
    }
#endif

#if (BSP_FEATURE_PIC_GTM_TIM_IN_PIC24)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN01 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN23 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN45 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN67 = 0U;
    }

#elif (BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN01 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN23 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN45 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN67 = 0U;
    }

#elif (BSP_FEATURE_PIC_GTM_TIM_IN_PIC22)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        p_instance_ctrl->p_regs_pic22->PIC2GTMINEN[idx].GTM_TIM_IN03 = 0U;
        p_instance_ctrl->p_regs_pic22->PIC2GTMINEN[idx].GTM_TIM_IN47 = 0U;
    }
#endif
#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)
#if (BSP_FEATURE_PIC_GTM_TIO_IN_PIC24)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN01 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN23 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN45 = 0U;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN67 = 0U;
    }

#elif (BSP_FEATURE_PIC_GTM_TIO_IN_PIC2_SELB)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP; idx++)
    {
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN01 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN23 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN45 = 0U;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN67 = 0U;
    }
#endif
#endif
#if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN0)
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN00 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN01 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN02 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN03 = 0U;
#endif
#if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN1)
    p_instance_ctrl->p_regs_pic11->PIC1HIZCEN10 = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1HIZCEN12 = 0U;
#endif
#if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN)
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN0 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN1 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN2 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN3 = 0U;
#endif

#if (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24)
    p_instance_ctrl->p_regs_pic24->PIC2ENHIZDTM = 0U;
#elif (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB)
    p_instance_ctrl->p_regs_pic2_selb->PIC2ENHIZDTM = 0U;
#elif (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC22)
    p_instance_ctrl->p_regs_pic20->PIC2ENHIZDTM = 0U;
#endif

#if (BSP_FEATURE_PIC_HRPWM_FEATURE)
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHLFIXCTS = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZ      = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZCTS   = 0U;
#endif

#if (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24)
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG0 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_0] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG1 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_1] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG2 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_2] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG3 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_3] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG4 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_4] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG5 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_5] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG6 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_6] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG7 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_7] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG8 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_8] = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG9 = 0U;
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_9] = 0U;
#endif

#if (BSP_FEATURE_PIC_RDC_FEATURE)
    p_instance_ctrl->p_regs_pic24->PIC2RDCEISEN0 = 0U;

 #if (BSP_FEATURE_PIC_RDC_FEATURE_RDC3AL && BSP_FEATURE_PIC_RDC_FEATURE_RDC3AS)
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN[PIC_RDC3A_FEATURE_UNIT_1] = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN[PIC_RDC3A_FEATURE_UNIT_1] = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN[PIC_RDC3A_FEATURE_UNIT_0] = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN[PIC_RDC3A_FEATURE_UNIT_0] = 0U;
 #else
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1 = 0U;

    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1 = 0U;
 #endif
#endif

#if (BSP_FEATURE_PIC_SST_PIC1_MULTI_UNIT)
    p_instance_ctrl->p_regs_pic10->PIC1SST0      = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1SST1      = 0U;
    p_instance_ctrl->p_regs_pic12->PIC1SST2      = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSTOUTEN0 = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1SSTOUTEN1 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER00    = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER01    = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER02    = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1SSER10    = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1SSER11    = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1SSER12    = 0U;
    p_instance_ctrl->p_regs_pic12->PIC1SSER20    = 0U;
#elif (BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST0)
    p_instance_ctrl->p_regs_pic10->PIC1SST0   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER00 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER01 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER02 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER03 = 0U;
#elif (BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST)
    p_instance_ctrl->p_regs_pic10->PIC1SST   = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER0 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER1 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER2 = 0U;
    p_instance_ctrl->p_regs_pic10->PIC1SSER3 = 0U;
#endif

#if (BSP_FEATURE_PIC_SWITCH_TSG_OUT)
    p_instance_ctrl->p_regs_pic10->PIC1TSGOUTCTR0 = 0U;

    p_instance_ctrl->p_regs_pic10->PIC1LHSEL0 = 0U;

    p_instance_ctrl->p_regs_pic10->PIC1TSGOUTCTR1 = 0U;

    p_instance_ctrl->p_regs_pic10->PIC1LHSEL1 = 0U;

 #if (BSP_FEATURE_PIC_SWITCH_TSG_OUT_TSG32)
    p_instance_ctrl->p_regs_pic11->PIC1TSGOUTCTR2 = 0U;

    p_instance_ctrl->p_regs_pic11->PIC1LHSEL2 = 0U;
 #endif
#endif

#if (BSP_FEATURE_PIC_TAUD_INPUT_SEL)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_TAUDTINSEL_NUMBER; idx++)
    {
 #if (BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC13)
        p_instance_ctrl->p_regs_pic13->PIC1TAUD[idx].TINSEL0 = 0U;
        p_instance_ctrl->p_regs_pic13->PIC1TAUD[idx].TINSEL1 = 0U;
 #elif (BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC1_SELB)
        p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD[idx].TINSEL0 = 0U;
        p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD[idx].TINSEL1 = 0U;
 #endif
    }
#endif

#if (BSP_FEATURE_PIC_TIMER_INPUT_MONITOR)
    p_instance_ctrl->p_regs_pic1_selb->PIC1PIMONSEL = 0U;
#endif

#if (2 < BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER)
    p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG220   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG230   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG222   = 0U;
    p_instance_ctrl->p_regs_pic11->PIC1REG232   = 0U;
#endif
#if (BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2)
    p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD2SEL = 0U;
#endif
#if (BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ)
    p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUD2I = 0U;

    p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUJ2I = 0U;

    p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUJ3I = 0U;
#endif
#if (BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13)
    p_instance_ctrl->p_regs_pic13->PIC1POMONSEL = 0U;
#elif (BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB)
    p_instance_ctrl->p_regs_pic1_selb->PIC1POMONSEL = 0U;
#endif

#if (BSP_FEATURE_PIC_TSG3_INPUT_GTM)
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG0 = 0U;

    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG1 = 0U;

    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG2 = 0U;

    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG0 = 0U;

    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG1 = 0U;

    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG2 = 0U;
#endif

#if (BSP_FEATURE_PIC_TSG_START_TRIG)
    p_instance_ctrl->p_regs_pic10->PIC1SSTSGSEL0 = 0U;
 #if (BSP_FEATURE_PIC_TSG_START_TRIG_TSG32)
    p_instance_ctrl->p_regs_pic11->PIC1SSTSGSEL1 = 0U;
 #endif
#endif

#if (BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB)
    p_instance_ctrl->p_regs_pic1_selb->PIC1SELBSSER = 0U;
#else
    p_instance_ctrl->p_regs_pic13->PIC1SELBSSER = 0U;
#endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Simultaneous start trigger function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_simultaneous_start_trigger_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    if (PIC_MODE_ENABLE == p_instance_ctrl->p_cfg->pic_simultaneous)
    {
        pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

#if (BSP_FEATURE_PIC_PIC1_ONLY_PIC12_IS_AVAILABLE)

        /* Configurate PIC1SSER20 register to enable/disable start trigger. */
        p_instance_ctrl->p_regs_pic12->PIC1SSER20 = (uint16_t) p_extend->pic1_simultaneous_start_control_20;

        /* Configurate PIC1SST2 register to enable/disable simultaneous start trigger for PIC12.*/
        p_instance_ctrl->p_regs_pic12->PIC1SST2 = (uint8_t) p_extend->pic12_simultaneous_start_trigger;
#else
 #if (BSP_FEATURE_PIC_SST_PIC1_MULTI_UNIT)

        /* Configurate PIC1SSER register to enable/disable start trigger. */
        p_instance_ctrl->p_regs_pic10->PIC1SSER00 = (uint16_t) p_extend->pic1_simultaneous_start_control_00;
        p_instance_ctrl->p_regs_pic10->PIC1SSER01 = (uint16_t) p_extend->pic1_simultaneous_start_control_01;
        p_instance_ctrl->p_regs_pic10->PIC1SSER02 = (uint16_t) p_extend->pic1_simultaneous_start_control_02;
        p_instance_ctrl->p_regs_pic11->PIC1SSER10 = (uint16_t) p_extend->pic1_simultaneous_start_control_10;
        p_instance_ctrl->p_regs_pic11->PIC1SSER11 = (uint16_t) p_extend->pic1_simultaneous_start_control_11;
        p_instance_ctrl->p_regs_pic11->PIC1SSER12 = (uint16_t) p_extend->pic1_simultaneous_start_control_12;
        p_instance_ctrl->p_regs_pic12->PIC1SSER20 = (uint16_t) p_extend->pic1_simultaneous_start_control_20;

        /* Configurate PIC1SSTOUTENk register to enable/disable the output of simultaneous start triggers from one PIC1 unit to the other. */
        p_instance_ctrl->p_regs_pic10->PIC1SSTOUTEN0 =
            (uint8_t) p_extend->pic10_simultaneous_start_trigger_to_pic11_pic12;
        p_instance_ctrl->p_regs_pic11->PIC1SSTOUTEN1 =
            (uint8_t) p_extend->pic11_simultaneous_start_trigger_to_pic10_pic12;

        /* Configurate PIC1SST2 register to enable/disable simultaneous start trigger.*/
        p_instance_ctrl->p_regs_pic10->PIC1SST0 = (uint8_t) p_extend->pic10_simultaneous_start_trigger;
        p_instance_ctrl->p_regs_pic11->PIC1SST1 = (uint8_t) p_extend->pic11_simultaneous_start_trigger;
        p_instance_ctrl->p_regs_pic12->PIC1SST2 = (uint8_t) p_extend->pic12_simultaneous_start_trigger;
 #elif (BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST0)

        /* Configurate PIC1SSER register to enable/disable start trigger. */
        p_instance_ctrl->p_regs_pic10->PIC1SSER00 = (uint16_t) p_extend->pic1_simultaneous_start_control_00;
        p_instance_ctrl->p_regs_pic10->PIC1SSER01 = (uint16_t) p_extend->pic1_simultaneous_start_control_01;
        p_instance_ctrl->p_regs_pic10->PIC1SSER02 = (uint16_t) p_extend->pic1_simultaneous_start_control_02;
        p_instance_ctrl->p_regs_pic10->PIC1SSER03 = (uint16_t) p_extend->pic1_simultaneous_start_control_03;

        /* Configurate PIC1SST register to enable/disable Simultaneous start trigger */
        p_instance_ctrl->p_regs_pic10->PIC1SST0 = (uint8_t) p_extend->pic10_simultaneous_start_trigger;
 #elif (BSP_FEATURE_PIC_SST_PIC1_SINGLE_SST)

        /* Configurate PIC1SSER register to enable/disable start trigger. */
        p_instance_ctrl->p_regs_pic10->PIC1SSER0 = (uint16_t) p_extend->pic1_simultaneous_start_control_00;
        p_instance_ctrl->p_regs_pic10->PIC1SSER1 = (uint16_t) p_extend->pic1_simultaneous_start_control_01;
        p_instance_ctrl->p_regs_pic10->PIC1SSER2 = (uint16_t) p_extend->pic1_simultaneous_start_control_02;
        p_instance_ctrl->p_regs_pic10->PIC1SSER3 = (uint16_t) p_extend->pic1_simultaneous_start_control_03;

        /* Configurate PIC1SST register to enable/disable Simultaneous start trigger */
        p_instance_ctrl->p_regs_pic10->PIC1SST = (uint8_t) p_extend->pic10_simultaneous_start_trigger;
 #endif
#endif
    }
}

#if (!BSP_FEATURE_PIC_PIC1_ONLY_PIC12_IS_AVAILABLE)
 #if (BSP_FEATURE_PIC_TSG_START_TRIG)

/*******************************************************************************************************************//**
 * Hardware configuration for TSG Simultaneous start trigger function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_tsg_simultaneous_start_trigger_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Selects the simultaneous start trigger for TSG30. */
    p_instance_ctrl->p_regs_pic10->PIC1SSTSGSEL0 =
        ((uint8_t) (p_extend->pic_tsg30_start_trigger_select << R_PIC10_PIC1SSTSGSEL0_PIC1SSTSGSEL00_Pos) |
         (p_extend->pic_tsg31_start_trigger_select << R_PIC10_PIC1SSTSGSEL0_PIC1SSTSGSEL01_Pos));
  #if (BSP_FEATURE_PIC_TSG_START_TRIG_TSG32)

    /* Selects the simultaneous start trigger for TSG32. */
    p_instance_ctrl->p_regs_pic11->PIC1SSTSGSEL1 =
        ((uint8_t) (p_extend->pic_tsg32_start_trigger_select << R_PIC11_PIC1SSTSGSEL1_PIC1SSTSGSEL10_Pos));
  #endif
}

 #endif

/*******************************************************************************************************************//**
 * Hardware configuration for TSG3 synchronous start and clear function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_tsg_sync_start_clear_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Configurate PIC1SELBSSER register to selects TAUD0 for synchronous starting or clearing of TSG30 and TSG31.*/
 #if (BSP_FEATURE_PIC_TSG_SYNC_START_CLEAR_PIC1_SELB)
    p_instance_ctrl->p_regs_pic1_selb->PIC1SELBSSER = ((uint8_t) (p_extend->pic_tsg_sync_start_clear_select));
 #else
    p_instance_ctrl->p_regs_pic13->PIC1SELBSSER = ((uint8_t) (p_extend->pic_tsg_sync_start_clear_select));
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Three-Phase Pulse Input Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_three_phase_pulse_input_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    if (PIC_MODE_ENABLE == p_extend->pic_tsg30_three_phase_pulse_input_control)
    {
        /* Select encoder group ENCA0 as input signal for TSG30PTSIx */
        p_instance_ctrl->p_regs_pic10->PIC1TSGHALLSEL         |= PIC_TSG0HALLSEL_THREE_PHASE_PULSE;
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG5000 = PIC_REG50_THREE_PHASE_PULSE_TSG30_SEL_ENCA;

        /* Select INTTAUD0I5 and INTTAUD0I7 as input signal for TSG30OPC0 and TSG30OPC1 */
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG5007 =
            PIC_REG50_THREE_PHASE_PULSE_TSG30OPCI1_SEL_INTTAUD0I7;
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG500605 =
            PIC_REG50_THREE_PHASE_PULSE_TSG30OPCI0_SEL_INTTAUD0I5;

        /* Select TSG30PTE as input signal for TAUD0TTIN4 to 5  */
        p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG2001110 = PIC_REG200_THREE_PHASE_PULSE_TAUD0TTIN_SEL_TSG30;
        p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG2000908 = PIC_REG200_THREE_PHASE_PULSE_TAUD0TTIN_SEL_TSG30;
        p_instance_ctrl->p_regs_pic10->PIC1REG200   |= PIC_REG200_THREE_PHASE_PULSE_TAUD0TTIN_INPUT_SEL;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL &= PIC_TAUD0SEL_THREE_PHASE_PULSE_INPUT_SEL;
    }
    else
    {
        /* Do nothing */
    }

    if (PIC_MODE_ENABLE == p_extend->pic_tsg31_three_phase_pulse_input_control)
    {
        /* Select encoder group ENCA1 as input signal for TSG31PTSIx */
        p_instance_ctrl->p_regs_pic10->PIC1TSGHALLSEL         |= PIC_TSG1HALLSEL_THREE_PHASE_PULSE;
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG5100 = PIC_REG51_THREE_PHASE_PULSE_TSG31_SEL_ENCA;

        /* Select INTTAUD1I5 and INTTAUD1I7 as input signal for TSG31OPC0 and TSG31OPC1 */
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG5107 =
            PIC_REG51_THREE_PHASE_PULSE_TSG31OPCI1_SEL_INTTAUD1I7;
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG510605 =
            PIC_REG51_THREE_PHASE_PULSE_TSG31OPCI0_SEL_INTTAUD1I5;

        /* Select TSG31PTE as input signal for TAUD1TTIN4 to 5  */
        p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG2101110 = PIC_REG210_THREE_PHASE_PULSE_TAUD1TTIN_SEL_TSG31;
        p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG2100908 = PIC_REG210_THREE_PHASE_PULSE_TAUD1TTIN_SEL_TSG31;
        p_instance_ctrl->p_regs_pic10->PIC1REG210   |= PIC_REG210_THREE_PHASE_PULSE_TAUD1TTIN_INPUT_SEL;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL &= PIC_TAUD1SEL_THREE_PHASE_PULSE_INPUT_SEL;
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Configuration for Two Phase Encoder Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_two_phase_encoder_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    if (PIC_ENCODER_CTRL_FUNC_TWO_PHASE_METHOD_1_2 == p_extend->pic_encoder_control_function)
    {
        /* Select ENCA pin as input for ENCA. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30 &= ~PIC_REG30_TW0_PHASE_ENCODER_METHOD12;
    }
    else if (PIC_ENCODER_CTRL_FUNC_TW0_PHASE_METHOD_3 == p_extend->pic_encoder_control_function)
    {
        /* Select ENCAn pin as input for ENCAn. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30 &= ~PIC_REG30_TWO_PHASE_ENCODER_METHOD3;

        /* Select ENCA0EQ1 as input for ENCA1. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30 |= PIC_REG30_TWO_PHASE_ENCODER_METHOD3_ENCA1_SEL_ENCA0;
    }
    else
    {
        /* Do nothing */
    }

    if (PIC_TSG3OPCI0_INPUT_SIGNAL_NONE != p_extend->pic_tsg30opci0_input_signal)
    {
        /* Selects the signal to be input as the TSG30OPCI0 signal of the TSG30 timer. */
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG500605 = ((uint8_t) (p_extend->pic_tsg30opci0_input_signal));
 #if (BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3)
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG5010 = PIC_REG50_TWO_PHASE_ENCODER_INTENCAI_SEL;
 #endif
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG5008 = PIC_REG50_TWO_PHASE_ENCODER_INTENCAI_SEL;
    }

    if (PIC_TSG3OPCI0_INPUT_SIGNAL_NONE != p_extend->pic_tsg31opci0_input_signal)
    {
        /* Selects the signal to be input as the TSG31OPCI0 signal of the TSG31 timer. */
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG510605 = ((uint16_t) (p_extend->pic_tsg31opci0_input_signal));
 #if (BSP_FEATURE_PIC_2PHASE_SEL_INTENCA1_TO_TSG3)
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG5110 = PIC_REG51_TWO_PHASE_ENCODER_INTENCAI_SEL;
 #endif
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG5108 = PIC_REG51_TWO_PHASE_ENCODER_INTENCAI_SEL;
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Configuration for Three Phase Encoder Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_three_phase_encoder_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    if (PIC_MODE_ENABLE == p_extend->pic_tsg30_three_phase_encoder_control)
    {
        /* Select ENCA0E0, ENCA0E1, and ENCA0EC as TSG30PTSI0 to TSG30PTSI2.*/
        p_instance_ctrl->p_regs_pic10->PIC1TSGHALLSEL_b.TSG0HALLSEL = PIC_TSG0HALLSEL_THREE_PHASE_ENCODER;
        p_instance_ctrl->p_regs_pic10->PIC1REG50_b.PIC1REG5000      = PIC_REG50_THREE_PHASE_ENCODER;

        /* Select the TSG30PEC and TSG30PUD as ENCA0E0 and ENCA0E1. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3022 = PIC_REG30_THREE_PHASE_ENCODER_ENCA0_INPUT_SEL;
        p_instance_ctrl->p_regs_pic10->PIC1REG30              |= PIC_REG30_THREE_PHASE_ENCODER_ENCA0_SEL_TSG30;
    }
    else
    {
        /* Do nothing */
    }

 #if (BSP_FEATURE_PIC_3PHASE_ENCODER_TSG31)
    if (PIC_MODE_ENABLE == p_extend->pic_tsg31_three_phase_encoder_control)
    {
        /* Select ENCA1E0, ENCA1E1, and ENCA1EC as TSG31PTSI0 to TSG31PTSI2.*/
        p_instance_ctrl->p_regs_pic10->PIC1TSGHALLSEL_b.TSG1HALLSEL = PIC_TSG1HALLSEL_THREE_PHASE_ENCODER;
        p_instance_ctrl->p_regs_pic10->PIC1REG51_b.PIC1REG5100      = PIC_REG51_THREE_PHASE_ENCODER;

        /* Select the TSG31PEC and TSG31PUD as ENCA1E0 and ENCA1E1. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300908 = PIC_REG30_THREE_PHASE_ENCODER_ENCA1_SEL_TSG31;
        p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300706 = PIC_REG30_THREE_PHASE_ENCODER_ENCA1_SEL_TSG31;
    }
    else
    {
        /* Do nothing */
    }
 #endif
}

/*******************************************************************************************************************//**
 * Configuration for Encoder Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_encoder_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    if ((PIC_ENCODER_CTRL_FUNC_TWO_PHASE_METHOD_1_2 == p_extend->pic_encoder_control_function) ||
        (PIC_ENCODER_CTRL_FUNC_TW0_PHASE_METHOD_3 == p_extend->pic_encoder_control_function))
    {
        r_pic_two_phase_encoder_control_func(p_instance_ctrl);
    }
    else if (PIC_ENCODER_CTRL_FUNC_THREE_PHASE_ENCODER == p_extend->pic_encoder_control_function)
    {
        r_pic_three_phase_encoder_control_func(p_instance_ctrl);
    }
    else if (PIC_ENCODER_CTRL_FUNC_THREE_PHASE_PULSE == p_extend->pic_encoder_control_function)
    {
        r_pic_three_phase_pulse_input_control_func(p_instance_ctrl);
    }
    else
    {
        /* Do nothing */
    }
}

/*******************************************************************************************************************//**
 * Configuration for Hi-Z Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_hiz_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

 #if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN0)

    /* Configurate Hi-Z control input signal for TAUD0, TAUD1, TSG30, TSG31 (U2Bx, U2Cx)*/
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN00 = ((uint8_t) p_extend->pic_hiz_control_00);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN01 = ((uint8_t) p_extend->pic_hiz_control_01);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN02 = ((uint8_t) p_extend->pic_hiz_control_02);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN03 = ((uint8_t) p_extend->pic_hiz_control_03);
 #endif
 #if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN1)

    /* Configurate Hi-Z control input signal for TAUD2, TSG32 (U2Bx)*/
    p_instance_ctrl->p_regs_pic11->PIC1HIZCEN10 = ((uint8_t) p_extend->pic_hiz_control_10);
    p_instance_ctrl->p_regs_pic11->PIC1HIZCEN12 = ((uint8_t) p_extend->pic_hiz_control_12);
 #endif

 #if (BSP_FEATURE_PIC_HIZ_CTRL_HIZCEN)

    /* Configurate Hi-Z control input signal for TAUD0, TAUD1, TSG30, TSG31 (U2Ax)*/
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN0 = ((uint8_t) p_extend->pic_hiz_control_00);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN1 = ((uint8_t) p_extend->pic_hiz_control_01);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN2 = ((uint8_t) p_extend->pic_hiz_control_02);
    p_instance_ctrl->p_regs_pic10->PIC1HIZCEN3 = ((uint8_t) p_extend->pic_hiz_control_03);
 #endif
}

/*******************************************************************************************************************//**
 * Configuration for Encoder Capture Trigger Select Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_encoder_capture_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    uint8_t              enca_input_enca0i_inttaud0i_sel = ((uint8_t) (p_extend->pic_enc_cap_trig_enca0_inttaud0i));
    uint8_t              enca_input_enca1i_inttaud1i_sel = ((uint8_t) (p_extend->pic_enc_cap_trig_enca1_inttaud1i));

    /* Selects the signal to be input as the ENCA0TTIN1 signal. */
    p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300502 = PIC_ENCA0_CAPTURE_TRIGGER_GET_REG30(
        (uint8_t) p_extend->pic_enc_cap_trig_enca0);
 #if (BSP_FEATURE_PIC_ENC_CAPT_TRIG_ENCA1)

    /* Selects the signal to be input as the ENCA1TTIN1 signal. */
    p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG301512 = PIC_ENCA1_CAPTURE_TRIGGER_GET_REG30(
        (uint8_t) p_extend->pic_enc_cap_trig_enca1);
 #endif

    /* Selects the signal(ENCA0I1 or selected by the PIC1ENCSEL4007 bit) to be supplied to PIC1REG3002 to 05. */
    p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3018 = PIC_ENCA_CAPTURE_TRIGGER_ENCAI_INTTAUDI_GET_MSK(
        enca_input_enca0i_inttaud0i_sel);

    /* Enables or disables output of INTTAUD0Im signal selected by PIC1ENCSEL400[3:0].*/
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL400_b.PIC1ENCSEL4007 = PIC_ENCA_CAPTURE_TRIGGER_ENCAI_INTTAUDI_GET_MSK(
        enca_input_enca0i_inttaud0i_sel);

    /* Selects INTTAUD0Im to be used as a capture trigger signal. */
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL400_b.PIC1ENCSEL40030 = ((uint8_t) enca_input_enca0i_inttaud0i_sel);

    /* Selects the signal(ENCA1I1 or selected by the PIC1ENCSEL4107 bit) to be supplied to PIC1REG3012 to 15. */
    p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3021 = PIC_ENCA_CAPTURE_TRIGGER_ENCAI_INTTAUDI_GET_MSK(
        enca_input_enca1i_inttaud1i_sel);

    /* Enables or disables output of INTTAUD1Im signal selected by PIC1ENCSEL410[3:0].*/
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL410_b.PIC1ENCSEL4107 = PIC_ENCA_CAPTURE_TRIGGER_ENCAI_INTTAUDI_GET_MSK(
        enca_input_enca1i_inttaud1i_sel);

    /* Selects INTTAUD1Im to be used as a capture trigger signal. */
    p_instance_ctrl->p_regs_pic10->PIC1ENCSEL410_b.PIC1ENCSEL41030 = ((uint8_t) enca_input_enca1i_inttaud1i_sel);
}

/*******************************************************************************************************************//**
 * Hardware configuration for INT signal output selection function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_int_signal_output_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Select the TAUD0 channel used by TAPA0TSIM0 and TAPA0TUDCM0. */
    p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG2002524 =
        ((uint8_t) p_extend->pic_taud_int_signal_output[PIC_TAUD_FEATURE_UNIT_0]);
 #if (BSP_FEATURE_PIC_INT_SIG_OUT_TAUD1)

    /* Select the TAUD1 channel used by TAPA1TSIM0 and TAPA1TUDCM0. */
    p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG2102524 =
        ((uint8_t) p_extend->pic_taud_int_signal_output[PIC_TAUD_FEATURE_UNIT_1]);
 #endif
 #if (BSP_FEATURE_PIC_INT_SIG_OUT_TAUD2)

    /* Select the TAUD2 channel used by TAPA2TSIM0 and TAPA2TUDCM0. */
    p_instance_ctrl->p_regs_pic11->PIC1REG220_b.PIC1REG2202524 =
        ((uint8_t) p_extend->pic_taud_int_signal_output[PIC_TAUD_FEATURE_UNIT_2]);
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Trigger Pulse Width Measurement Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_trigger_pulse_with_measurement_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    if (PIC_MODE_ENABLE == p_instance_ctrl->p_cfg->pic10_trigger_pulse_width)
    {
        pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

        /* Configurate for input signal for trigger pulse width measurement. */
        p_instance_ctrl->p_regs_pic10->PIC1REG30    &= ~PIC_REG30_TRIGGER_PULSE_WIDTH_MEASURE;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL &= ~PIC_TAUDSEL_TRIGGER_PULSE_WIDTH_MEASURE;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL &= ~PIC_TAUDSEL_TRIGGER_PULSE_WIDTH_MEASURE;
        p_instance_ctrl->p_regs_pic10->PIC1REG31     = ((uint32_t) p_extend->pic_trigger_pulse_width_measure_sel);

        /* Enable initialization of DT02. */
        p_instance_ctrl->p_regs_pic10->PIC1INI01_b.PIC1INIn12 = 1U;

        /* Enable initialization of DT01. */
        p_instance_ctrl->p_regs_pic10->PIC1INI01_b.PIC1INIn11 = 1U;

        /* Enable initialization of DT11. */
        p_instance_ctrl->p_regs_pic10->PIC1INI11_b.PIC1INIn11 = 1U;

 #if (BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT12)

        /* Enable initialization of DT12. */
        p_instance_ctrl->p_regs_pic10->PIC1INI11_b.PIC1INIn12 = 1U;
 #endif

 #if (BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT00)

        /* Enable initialization of DT00. */
        p_instance_ctrl->p_regs_pic10->PIC1INI01_b.PIC1INIn10 = 1U;
 #endif
 #if (BSP_FEATURE_PIC_TRIG_PULSE_WIDTH_MEASURE_DT10)

        /* Enable initialization of DT10. */
        p_instance_ctrl->p_regs_pic10->PIC1INI11_b.PIC1INIn10 = 1U;
 #endif
    }
}

 #if (BSP_FEATURE_PIC_ENCA_INPUT_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for ENCA Input Select Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_enca_input_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    if (PIC_MODE_ENABLE == p_extend->pic_enca_input_sel)
    {
        if (PIC_ENCA_INPUT_SEL_UNIT_1 == p_extend->pic_enca0_input_sel_unit)
        {
            /* Select the signal to input to ENCAT0E0, ENCAT0E1, and ENCAT0EC in unit (3) via unit (1) */
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3022 = 0U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3001 = 0U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3000 = 0U;
        }
        else if (PIC_ENCA_INPUT_SEL_UNIT_2 == p_extend->pic_enca0_input_sel_unit)
        {
            /* Select the signal to input to ENCAT0E0, ENCAT0E1, and ENCAT0EC in unit (3) via unit (2) */
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG3022 = 1U;
        }

        if (PIC_ENCA_INPUT_SEL_UNIT_1 == p_extend->pic_enca1_input_sel_unit)
        {
            /* Select the signal to input to ENCAT1E0, ENCAT1E1, and ENCAT1EC in unit (3) via unit (1) */
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG301110 = 0x02U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300908 = 0x01U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300706 = 0x01U;
        }
        else if (PIC_ENCA_INPUT_SEL_UNIT_2 == p_extend->pic_enca1_input_sel_unit)
        {
            /* Select the signal to input to ENCAT1E0, ENCAT1E1, and ENCAT1EC in unit (3) via unit (2) */
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG301110 = 0x00U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300908 = 0x00U;
            p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG300706 = 0x00U;
        }

        /* Select the signal to input to unit (1) */
        p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG301716 =
            ((uint8_t) p_extend->pic_enca_unit1_input_signal_sel);

        /* Select the signal to input to unit (2) */
        p_instance_ctrl->p_regs_pic10->PIC1REG30_b.PIC1REG302019 =
            ((uint8_t) p_extend->pic_enca_unit2_input_signal_sel);
    }
}

 #endif
 #if (BSP_FEATURE_PIC_SWITCH_TSG_OUT)

/*******************************************************************************************************************//**
 * Hardware configuration for Switch Function between TSG Output and Low/High Level Output.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_switch_tsg_output_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Selects the output type of TSG30 output signal. */
    p_instance_ctrl->p_regs_pic10->PIC1TSGOUTCTR0 =
        (uint8_t) p_extend->pic_switch_tsg_output_control[PIC_TSG3_FEATURE_UNIT_0];

    /* Selects low/high level output of the TSG30 output. */
    p_instance_ctrl->p_regs_pic10->PIC1LHSEL0 =
        (uint8_t) p_extend->pic_switch_tsg_output_low_high_level_select[PIC_TSG3_FEATURE_UNIT_0];

    /* Selects the output type of TSG31 output signal. */
    p_instance_ctrl->p_regs_pic10->PIC1TSGOUTCTR1 =
        (uint8_t) p_extend->pic_switch_tsg_output_control[PIC_TSG3_FEATURE_UNIT_1];

    /* Selects low/high level output of the TSG31 output. */
    p_instance_ctrl->p_regs_pic10->PIC1LHSEL1 =
        (uint8_t) p_extend->pic_switch_tsg_output_low_high_level_select[PIC_TSG3_FEATURE_UNIT_1];

  #if (BSP_FEATURE_PIC_SWITCH_TSG_OUT_TSG32)

    /* Selects the output type of TSG32 output signal. */
    p_instance_ctrl->p_regs_pic11->PIC1TSGOUTCTR2 =
        (uint8_t) p_extend->pic_switch_tsg_output_control[PIC_TSG3_FEATURE_UNIT_2];

    /* Selects low/high level output of the TSG32 output. */
    p_instance_ctrl->p_regs_pic11->PIC1LHSEL2 =
        (uint8_t) p_extend->pic_switch_tsg_output_low_high_level_select[PIC_TSG3_FEATURE_UNIT_2];
  #endif
}

 #endif

/*******************************************************************************************************************//**
 * Hardware configuration for High Accuracy Triangle Wave PWM output function with dead time.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_triangle_wave_pwm_output_with_deadtime (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Set up hardware for TAUD0. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].u_phase.mode)
    {
        /* The values to output TOUT02 as TIN10. */
        p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG20018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n21716    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1010 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].v_phase.mode)
    {
        /* The values to output TOUT02 as TIN12. */
        p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG20018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22120    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1210 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].w_phase.mode)
    {
        /* The values to output TOUT02 as TIN14. */
        p_instance_ctrl->p_regs_pic10->PIC1REG200_b.PIC1REG20018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22524    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1410 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_0].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Set up hardware for TAUD1. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].u_phase.mode)
    {
        /* The values to output TOUT02 as TIN10. */
        p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG21018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n21716    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1010 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].v_phase.mode)
    {
        /* The values to output TOUT02 as TIN12. */
        p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG21018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22120    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1210 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].w_phase.mode)
    {
        /* The values to output TOUT02 as TIN14. */
        p_instance_ctrl->p_regs_pic10->PIC1REG210_b.PIC1REG21018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22524    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1410 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_1].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

 #if (2 < BSP_FEATURE_PIC_TAUD_TRIANGLE_WAVE_NUMBER)

    /* Set up hardware for TAUD2. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].u_phase.mode)
    {
        /* The values to output TOUT02 as TIN10. */
        p_instance_ctrl->p_regs_pic11->PIC1REG220_b.PIC1REG22018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n21716    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1010 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].v_phase.mode)
    {
        /* The values to output TOUT02 as TIN12. */
        p_instance_ctrl->p_regs_pic11->PIC1REG220_b.PIC1REG22018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22120    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1210 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].w_phase.mode)
    {
        /* The values to output TOUT02 as TIN14. */
        p_instance_ctrl->p_regs_pic11->PIC1REG220_b.PIC1REG22018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22524    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1410 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_2].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Set up hardware for TAUD3. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].u_phase.mode)
    {
        /* The values to output TOUT02 as TIN10. */
        p_instance_ctrl->p_regs_pic11->PIC1REG230_b.PIC1REG23018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n21716    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1010 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].u_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO10 and TAUDnO11. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11918 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11716 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30604 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30200 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].v_phase.mode)
    {
        /* The values to output TOUT02 as TIN12. */
        p_instance_ctrl->p_regs_pic11->PIC1REG230_b.PIC1REG23018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22120    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1210 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].v_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO12 and TAUDnO13. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12322 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12120 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31412 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31008 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].w_phase.mode)
    {
        /* The values to output TOUT02 as TIN14. */
        p_instance_ctrl->p_regs_pic11->PIC1REG230_b.PIC1REG23018      = PIC_TRIANGLE_PWM_INPUT_SEL_UVW_REG2N0;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22524    = PIC_TRIANGLE_PWM_INPUT_SEL_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1410 = PIC_TRIANGLE_PWM_INPUT_SEL_TAUDSEL;

        if (PIC_ACTIVE_LEVEL_HIGH == p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active high) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_H;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_H;
        }
        else if (PIC_ACTIVE_LEVEL_LOW ==
                 p_extend->pic_taud_triangle_wave_pwm[PIC_TAUD_FEATURE_UNIT_3].w_phase.active_level)
        {
            /* The values to output one-phase PWM (active low) from TAUDnO14 and TAUDnO15. */
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12726 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12524 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N1_L;

            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n32220 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
            p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31816 = PIC_TRIANGLE_PWM_ACTIVE_SIG_REG2N3_L;
        }
    }
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Timer Input Select Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_timer_input_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    if (PIC_MODE_ENABLE == p_instance_ctrl->p_cfg->pic_timer_input_sel)
    {
        pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

        /* Selects TAUD0 TTIN input signal. */
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL = (uint32_t) p_extend->pic_taud0_ttinm.taud0sel;

        /* Selects TAUD1 TTIN input signal. */
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL = (uint32_t) p_extend->pic_taud1_ttinm.taud1sel;

        /* Select TIN pin input as TAUDnTTIN input. */
        p_instance_ctrl->p_regs_pic10->PIC1REG200 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG210 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG202 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG212 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG31  = 0U;
 #if (!BSP_FEATURE_PIC_TIMER_IN_SEL_B)

        /* Selects TAUD2 TTIN input signal. */
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL = (uint32_t) p_extend->pic_taud2_ttinm.taud2sel;

        /* Selects TAUD3 TTIN input signal. */
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL = (uint32_t) p_extend->pic_taud3_ttinm.taud3sel;

        /* Select TIN pin input as TAUDnTTIN input. */
        p_instance_ctrl->p_regs_pic11->PIC1REG220 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG230 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG222 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG232 = 0U;
 #elif (BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD2)

        /* Selects TAUD2 TTIN input signal. */
        p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD2SEL = (uint16_t) p_extend->pic_taud2_ttinm.taud2sel;
 #elif (BSP_FEATURE_PIC_TIMER_IN_SEL_B_TAUD_TAUJ)

        /* Selects TAUD2 TTIN input signal. */
        p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUD2I = (uint16_t) p_extend->pic_taud2_ttinm.taud2sel;

        /* Selects TAUJ2TTIN2, TAUJ2TTIN3 input signal. */
        p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUJ2I = (uint8_t) p_extend->pic_tauj2_ttinm.tauj2sel;

        /* Selects TAUJ3TTIN2, TAUJ3TTIN3 input signal. */
        p_instance_ctrl->p_regs_pic1_selb->PIC1SELB_TAUJ3I = (uint8_t) p_extend->pic_tauj3_ttinm;
 #endif
    }
}

/*******************************************************************************************************************//**
 * Hardware configuration for PWM output function with dead time.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_pwm_output_with_deadtime (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Set up hardware for TAUD0. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_0].u_phase)
    {
        /* Output the signal Q from RS02 as TIN11. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n21918    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n202      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP0U and TOP0UB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30604 = 0U;

        /* Enable initialization of RS02. */
        p_instance_ctrl->p_regs_pic10->PIC1INI00_b.PIC1INIn02 = 1U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_0].v_phase)
    {
        /* Output the signal Q from RS03 as TIN13. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22322    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n203      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP0V and TOP0VB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31412 = 0U;

        /* Enable initialization of RS03. */
        p_instance_ctrl->p_regs_pic10->PIC1INI00_b.PIC1INIn03 = 1U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_0].w_phase)
    {
        /* Output the signal Q from RS04 as TIN15. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22726    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n204      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP0W and TOP0WB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n32220 = 0U;

        /* Enable initialization of RS04. */
        p_instance_ctrl->p_regs_pic10->PIC1INI00_b.PIC1INIn04 = 1U;
    }

    /* Set up hardware for TAUD1. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_1].u_phase)
    {
        /* Output the signal Q from RS12 as TIN11. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n21918    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n202      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP1U and TOP1UB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30604 = 0U;

        /* Enable initialization of RS12. */
        p_instance_ctrl->p_regs_pic10->PIC1INI10_b.PIC1INIn02 = 1U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_1].v_phase)
    {
        /* Output the signal Q from RS13 as TIN13. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22322    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n203      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP1V and TOP1VB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31412 = 0U;

        /* Enable initialization of RS13. */
        p_instance_ctrl->p_regs_pic10->PIC1INI10_b.PIC1INIn03 = 1U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_1].w_phase)
    {
        /* Output the signal Q from RS14 as TIN15. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22726    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n204      = 1U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP1W and TOP1WB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n32220 = 0U;

        /* Enable initialization of RS14. */
        p_instance_ctrl->p_regs_pic10->PIC1INI10_b.PIC1INIn04 = 1U;
    }

 #if (2 < BSP_FEATURE_PIC_TAUD_PWM_OUTPUT_NUMBER)

    /* Set up hardware for TAUD2. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_2].u_phase)
    {
        /* Output the signal Q from RS22 as TIN11. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n21918    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n202      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP2U and TOP2UB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30604 = 0U;

        /* Enable initialization of RS22. */
        p_instance_ctrl->p_regs_pic11->PIC1INI20_b.PIC1INIn02 = 1U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_2].v_phase)
    {
        /* Output the signal Q from RS23 as TIN13. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22322    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n203      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP2V and TOP2VB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31412 = 0U;

        /* Enable initialization of RS23. */
        p_instance_ctrl->p_regs_pic11->PIC1INI20_b.PIC1INIn03 = 1U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_2].w_phase)
    {
        /* Output the signal Q from RS24 as TIN15. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22726    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n204      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP2W and TOP2WB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n32220 = 0U;

        /* Enable initialization of RS24. */
        p_instance_ctrl->p_regs_pic11->PIC1INI20_b.PIC1INIn04 = 1U;
    }

    /* Set up hardware for TAUD3. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_3].u_phase)
    {
        /* Output the signal Q from RS32 as TIN11. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n21918    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n202      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP3U and TOP3UB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30604 = 0U;

        /* Enable initialization of RS32. */
        p_instance_ctrl->p_regs_pic11->PIC1INI30_b.PIC1INIn02 = 1U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_3].v_phase)
    {
        /* Output the signal Q from RS33 as TIN13. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22322    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n203      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP3V and TOP3VB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31412 = 0U;

        /* Enable initialization of RS33. */
        p_instance_ctrl->p_regs_pic11->PIC1INI30_b.PIC1INIn03 = 1U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_pwm_output[PIC_TAUD_FEATURE_UNIT_3].w_phase)
    {
        /* Output the signal Q from RS34 as TIN15. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22726    = PIC_PWM_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n204      = 1U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP3W and TOP3WB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n32220 = 0U;

        /* Enable initialization of RS34. */
        p_instance_ctrl->p_regs_pic11->PIC1INI30_b.PIC1INIn04 = 1U;
    }
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Delay pulse output function with dead time.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_delay_pulse_output_with_deadtime (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Set up hardware for TAUD0. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_0].u_phase)
    {
        /* Output TOUT05 as TIN11. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n21918    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n202      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP0U and TOP0UB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n30604 = 0U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_0].v_phase)
    {
        /* Output TOUT07 as TIN13. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22322    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n203      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP0V and TOP0VB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31412 = 0U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_0].w_phase)
    {
        /* Output TOUT09 as TIN15. */
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n22726    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG202_b.PIC1REG2n204      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD0SEL_b.PIC1TAUD0IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP0W and TOP0WB */
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG201_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG203_b.PIC1REG2n32220 = 0U;
    }

    /* Set up hardware for TAUD1. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_1].u_phase)
    {
        /* Output TOUT05 as TIN11. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n21918    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n202      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP1U and TOP1UB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n30604 = 0U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_1].v_phase)
    {
        /* Output TOUT07 as TIN13. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22322    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n203      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP1V and TOP1VB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31412 = 0U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_1].w_phase)
    {
        /* Output TOUT09 as TIN15. */
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n22726    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic10->PIC1REG212_b.PIC1REG2n204      = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1TAUD1SEL_b.PIC1TAUD1IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP1W and TOP1WB */
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG211_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic10->PIC1REG213_b.PIC1REG2n32220 = 0U;
    }

 #if (2 < BSP_FEATURE_PIC_TAUD_DELAY_PULSE_NUMBER)

    /* Set up hardware for TAUD2. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_2].u_phase)
    {
        /* Output TOUT05 as TIN11. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n21918    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n202      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP2U and TOP2UB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n30604 = 0U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_2].v_phase)
    {
        /* Output TOUT07 as TIN13. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22322    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n203      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP2V and TOP2VB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31412 = 0U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_2].w_phase)
    {
        /* Output TOUT09 as TIN15. */
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n22726    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG222_b.PIC1REG2n204      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD2SEL_b.PIC1TAUD2IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP2W and TOP2WB */
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG221_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG223_b.PIC1REG2n32220 = 0U;
    }

    /* Set up hardware for TAUD3. */
    /* Configurate for U phase/UB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_3].u_phase)
    {
        /* Output TOUT05 as TIN11. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n21918    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n202      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1032 = 0U;

        /* The values to output TOUT10 and TOUT11 as TOP3U and TOP3UB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11918 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n11716 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30200 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n30604 = 0U;
    }

    /* Configurate for V phase/VB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_3].v_phase)
    {
        /* Output TOUT07 as TIN13. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22322    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n203      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1232 = 0U;

        /* The values to output TOUT12 and TOUT13 as TOP3V and TOP3VB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12322 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12120 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31008 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31412 = 0U;
    }

    /* Configurate for W phase/WB phase. */
    if (PIC_MODE_ENABLE == p_extend->pic_taud_delay_pulse_output[PIC_TAUD_FEATURE_UNIT_3].w_phase)
    {
        /* Output TOUT09 as TIN15. */
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n22726    = PIC_DELAY_PULSE_OUTPUT_INPUT_SEL_UVW_REG2N2;
        p_instance_ctrl->p_regs_pic11->PIC1REG232_b.PIC1REG2n204      = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1TAUD3SEL_b.PIC1TAUD3IN1432 = 0U;

        /* The values to output TOUT14 and TOUT15 as TOP3W and TOP3WB */
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12726 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG231_b.PIC1REG2n12524 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n31816 = 0U;
        p_instance_ctrl->p_regs_pic11->PIC1REG233_b.PIC1REG2n32220 = 0U;
    }
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for Timer Output Monitor Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_timer_output_monitor_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_TIMER_OUT_MON_PIC13)

    /* Configure PIC1POMONSEL register to select the input pin to monitor. */
    p_instance_ctrl->p_regs_pic13->PIC1POMONSEL_b.POMONSEL20 = (uint8_t) p_extend->pic_taud2_monitor_sel;
 #elif (BSP_FEATURE_PIC_TIMER_OUT_MON_PIC1_SELB)

    /* Configure PIC1POMONSEL register to select the input pin to monitor. */
    p_instance_ctrl->p_regs_pic1_selb->PIC1POMONSEL_b.POMONSEL20 = (uint8_t) p_extend->pic_taud2_monitor_sel;
 #endif
}

 #if (BSP_FEATURE_PIC_TIMER_INPUT_MONITOR)

/*******************************************************************************************************************//**
 * Hardware configuration for Timer Input Monitor Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_timer_input_monitor_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Configure PIC1PIMONSEL register to select the input pin to monitor. */
    p_instance_ctrl->p_regs_pic1_selb->PIC1PIMONSEL_b.PIMONSEL10 = (uint8_t) p_extend->pic_tauj2_in_monitor_sel;
}

 #endif

 #if (BSP_FEATURE_PIC_TAUD_INPUT_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for TAUD Input Select Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_taud_input_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    if (PIC_MODE_ENABLE == p_instance_ctrl->p_cfg->pic_taud_input_select)
    {
        pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

        for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_TAUDTINSEL_NUMBER; idx++)
        {
  #if (BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC13)

            /** Selects sources to TAUDnTTINm input signals */
            p_instance_ctrl->p_regs_pic13->PIC1TAUD[idx].TINSEL0 =
                (uint32_t) p_extend->pic_taudtinsel_ctrl[idx].taudtinsel0_ctrl.taudtinsel;
            p_instance_ctrl->p_regs_pic13->PIC1TAUD[idx].TINSEL1 =
                (uint32_t) p_extend->pic_taudtinsel_ctrl[idx].taudtinsel1_ctrl.taudtinsel;
  #elif (BSP_FEATURE_PIC_TAUD_INPUT_SEL_PIC1_SELB)

            /** Selects sources to TAUDnTTINm input signals */
            p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD[idx].TINSEL0 =
                (uint32_t) p_extend->pic_taudtinsel_ctrl[idx].taudtinsel0_ctrl.taudtinsel;
            p_instance_ctrl->p_regs_pic1_selb->PIC1TAUD[idx].TINSEL1 =
                (uint32_t) p_extend->pic_taudtinsel_ctrl[idx].taudtinsel1_ctrl.taudtinsel;
  #endif
        }
    }
}

 #endif
#endif

#if (BSP_FEATURE_PIC_ADCK_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for ADCK trigger selection function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_adck_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    for (uint8_t adck_unit = 0; adck_unit < BSP_FEATURE_PIC_ADC_NUMBER_UNIT; adck_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            /* Selects triggers in PIC20ADCKnTSELj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC20ADCK[adck_unit].TSEL[adck_sg] =
                p_extend->pic20_adc_sg_trigger_sel[adck_unit][adck_sg];

            /* Selects triggers in PIC21ADCKnTSELj registers. */
            p_instance_ctrl->p_regs_pic21->PIC21ADCK[adck_unit].TSEL[adck_sg] =
                p_extend->pic21_adc_sg_trigger_sel[adck_unit][adck_sg];

            /* Selects trigger from GTM in PIC20ADTEN5nj registers. */
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN5[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_gtm_sel[adck_unit][adck_sg];
 #if (BSP_FEATURE_PIC_ADCK_TRIG_SEL_ATU)

            /* Selects trigger from ATU timer D 1shot pulse ON occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN7[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_1shot_pulse_on_sel[adck_unit][adck_sg];

            /* Selects trigger from ATU timer D 1shot pulse OFF occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN8[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_1shot_pulse_off_sel[adck_unit][adck_sg];

            /* Selects trigger from ATU timer C compare match, ATU timer G compare match, and ATU DMA request switch function. */
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN5[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_c_g_dma_sel[adck_unit][adck_sg];

            /* Selects trigger from ATU timer D compare match A and ATU timer G compare match. */
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN6[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_d_g_compare_match_sel[adck_unit][adck_sg];

            /* Selects trigger from ATU timer D compare match B. */
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN7[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_d_compare_match_b_sel[adck_unit][adck_sg];

            /* Selects trigger from ATU timer D down-counter underflow. */
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN8[adck_unit].SG[adck_sg] =
                p_extend->pic_adc_convert_trigger_atu_d_downcounter_underflow_sel[adck_unit][adck_sg];
 #endif
        }

        /* Selects an effective edge for the one-shot pulse generation circuit
         * which generates an ADCK trigger from PIC20ADCKnTSEL.*/
        p_instance_ctrl->p_regs_pic20->PIC20ADCK[adck_unit].EDGSEL =
            p_extend->pic20_adc_sg_trigger_edge[adck_unit];

        /* Selects an effective edge for the one-shot pulse generation circuit
         * which generates an ADCK trigger from PIC21ADCKnTSEL.*/
        p_instance_ctrl->p_regs_pic21->PIC21ADCK[adck_unit].EDGSEL =
            p_extend->pic21_adc_sg_trigger_edge[adck_unit];
    }

    /* Selects trigger from TAUDn channel m (n = 0, 1; m = 0 to 15). */
    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            /* Set the trigger selection for each scan group. */
            p_instance_ctrl->p_regs_pic20->PIC20ADTEN4[taud_unit].SG[adck_sg].SG =
                p_extend->pic20_adc_convert_trigger_taud_sel[taud_unit][adck_sg];
        }
    }

    /* Selects trigger from TAUDn channel m (n = 2, 3; m = 0 to 15). */
    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC21_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            /* Set the trigger selection for each scan group. */
            p_instance_ctrl->p_regs_pic21->PIC21ADTEN4[taud_unit].SG[adck_sg].SG =
                p_extend->pic21_adc_convert_trigger_taud_sel[taud_unit][adck_sg];
        }
    }

    /* Selects trigger from ENCAn interrupt . */
    for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
    {
        /* Selects triggers from ENCAn interrupt for each scan group.*/
        p_instance_ctrl->p_regs_pic20->PIC20ADTEN42[adck_sg].SG = p_extend->pic_adc_convert_trigger_enca_sel[adck_sg];
    }

    /* Assigns timer output of GTM (TOM, ATOM, TIO, MCS, TIM) to PIC20ADTEN5nj*/
    for (uint8_t pic2adtcfg_idx = 0; pic2adtcfg_idx < BSP_FEATURE_PIC_ADTCFG_NUMBER; pic2adtcfg_idx++)
    {
 #if (BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC24)
        p_instance_ctrl->p_regs_pic24->PIC2ADTCFG[pic2adtcfg_idx] =
            (p_extend->pic_adc_convert_trigger_gtm_output_sel[pic2adtcfg_idx].adtcfg);
 #elif (BSP_FEATURE_PIC_ADCK_TRIG_SEL_GTM_PIC2_SELB)
        p_instance_ctrl->p_regs_pic2_selb->PIC2ADTCFG[pic2adtcfg_idx] =
            (p_extend->pic_adc_convert_trigger_gtm_output_sel[pic2adtcfg_idx].adtcfg);
 #endif
    }
}

#endif

#if (BSP_FEATURE_PIC_ADCJ_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for ADCJ trigger selection function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_adcj_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    for (uint8_t adcj_unit = 0; adcj_unit < BSP_FEATURE_PIC_ADC_NUMBER_UNIT; adcj_unit++)
    {
        for (uint8_t adcj_sg = 0; adcj_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adcj_sg++)
        {
            /* Selects triggers in PIC2ADCJnTSELj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC2ADCJ[adcj_unit].TSEL[adcj_sg] =
                p_extend->pic20_adc_sg_trigger_sel[adcj_unit][adcj_sg];

            /* Selects triggers in PIC2ADTEN5nj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN5[adcj_unit].SG[adcj_sg] =
                p_extend->pic_adc_convert_trigger_gtm_intgtma0int_sel[adcj_unit][adcj_sg];

            /* Selects triggers in PIC2ADTEN6nj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN6[adcj_unit].SG[adcj_sg] =
                p_extend->pic_adc_convert_trigger_gtm_atom_01_sel[adcj_unit][adcj_sg];

            /* Selects triggers in PIC2ADTEN7nj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN7[adcj_unit].SG[adcj_sg] =
                p_extend->pic_adc_convert_trigger_gtm_atom_23_sel[adcj_unit][adcj_sg];

            /* Selects triggers in PIC2ADTEN8nj registers.*/
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN8[adcj_unit].SG[adcj_sg] =
                p_extend->pic_adc_convert_trigger_gtm_mcs_sel[adcj_unit][adcj_sg];
        }

        /* Selects an effective edge for the one-shot pulse generation circuit
         * which generates an ADCJ trigger from PIC2ADCJnTSELj.*/
        p_instance_ctrl->p_regs_pic20->PIC2ADCJ[adcj_unit].EDGSEL =
            p_extend->pic20_adc_sg_trigger_edge[adcj_unit];
    }

    /* Selects trigger from TAUDn channel m (n = 0, 1; m = 0 to 15). */
    for (uint8_t taud_unit = 0; taud_unit < BSP_FEATURE_PIC20_TAUD_NUMBER_UNIT; taud_unit++)
    {
        for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
        {
            /* Set the trigger selection for each scan group. */
            p_instance_ctrl->p_regs_pic20->PIC2ADTEN4[taud_unit].SG[adck_sg].SG =
                p_extend->pic20_adc_convert_trigger_taud_sel[taud_unit][adck_sg];
        }
    }

    /* Selects trigger from ENCAn interrupt . */
    for (uint8_t adck_sg = 0; adck_sg < BSP_FEATURE_PIC_ADC_NUMBER_SG; adck_sg++)
    {
        /* Selects triggers from ENCAn interrupt for each scan group.*/
        p_instance_ctrl->p_regs_pic20->PIC2ADTEN42[adck_sg].SG = p_extend->pic_adc_convert_trigger_enca_sel[adck_sg];
    }
}

#endif

#if (BSP_FEATURE_PIC_DSADC_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for DSADC trigger selection function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_dsadc_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    for (uint8_t dsadc_unit = 0; dsadc_unit < BSP_FEATURE_PIC_DSADTCFG_NUMBER; dsadc_unit++)
    {
        for (uint8_t dsadc_reg = 0; dsadc_reg < BSP_FEATURE_PIC_DSADTCFG_NUMBER_REG; dsadc_reg++)
        {
            /* Selects triggers in PIC20DSADCATSEL0 registers.*/
            p_instance_ctrl->p_regs_pic20->PIC20DSADCATSEL0 = (uint32_t) p_extend->pic_dsadc_trigger_00_sel;

            /* Selects triggers in PIC20DSADCATSEL1 registers. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADCATSEL1 = (uint32_t) p_extend->pic_dsadc_trigger_01_sel;

 #if (BSP_FEATURE_PIC_FEATURE_U2BX)
            /* Selects triggers in PIC21DSADCATSEL0 registers.*/
            p_instance_ctrl->p_regs_pic21->PIC21DSADCATSEL0 = (uint32_t) p_extend->pic_dsadc_trigger_10_sel;

            /* Selects triggers in PIC21DSADCATSEL1 registers. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADCATSEL1 = (uint32_t) p_extend->pic_dsadc_trigger_11_sel;   
 #endif 
#if (BSP_FEATURE_PIC_FEATURE_U2BX)
 #if (BSP_FEATURE_PIC_DSADC_TRIG_SEL_ATU)
        
        if(2 == dsadc_reg)
        {
            /* Selects start trigger from ATU timer D 1shot pulse ON occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_atu_1shot_pulse_on_0n2_sel[dsadc_unit][dsadc_reg];
        }
        else if (3 == dsadc_reg) {
            /* Selects start trigger from ATU timer D 1shot pulse OFF occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_atu_1shot_pulse_off_0n3_sel[dsadc_unit][dsadc_reg];
        }

        if(2 == dsadc_reg)
        {
            /* Selects stop trigger from ATU timer D 1shot pulse ON occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_atu_1shot_pulse_on_1n2_sel[dsadc_unit][dsadc_reg];
        }
        else if (3 == dsadc_reg)   {
            /* Selects stop trigger from ATU timer D 1shot pulse OFF occurrence. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_atu_1shot_pulse_off_1n3_sel[dsadc_unit][dsadc_reg];
        }

        if(0 == dsadc_reg)
        {
            /* Selects start trigger source from ATU timer C compare match and ATU timer G compare match occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_out_ctrl_atu_c_g_compare_match_sel[dsadc_unit][dsadc_reg];
        }
        else if (1 == dsadc_reg )
        {
            /* Selects start trigger source from ATU timer D compare match A and ATU timer G compare occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_out_ctrl_atu_d_g_compare_match_a_sel[dsadc_unit][dsadc_reg];
        }
        else if (2 == dsadc_reg)
        {
            /* Selects start trigger source from ATU timer D compare match B occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_out_ctrl_atu_d_compare_match_b_sel[dsadc_unit][dsadc_reg];
        }
        else
        {
            /* Selects start trigger source from ATU timer D downcounter underflow occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_atu_d_downcounter_underflow_sel[dsadc_unit][dsadc_reg];
        }

        if(0 == dsadc_reg)
        {
            /* Selects stop trigger from ATU timer C compare match and ATU timer G compare match occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_out_ctrl_atu_c_g_compare_match_sel[dsadc_unit][dsadc_reg];
        }
        else if (1 == dsadc_reg )
        {
            /* Selects stop trigger source from ATU timer D compare match A and ATU timer G compare occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_out_ctrl_atu_d_g_compare_match_a_sel[dsadc_unit][dsadc_reg];
        }
        else if (2 == dsadc_reg )
        {
            /* Selects stop trigger source from ATU timer D compare match B occurrence. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_out_ctrl_atu_d_compare_match_b_sel[dsadc_unit][dsadc_reg];
        }
        else
        {
            /* Selects stop trigger source from ATU timer D down-counter underflow. */
            p_instance_ctrl->p_regs_pic21->PIC21DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_stop_trigger_atu_d_downcounter_underflow_sel[dsadc_unit][dsadc_reg];
        }
 #endif
#endif

        /* Start / Stop Trigger Output Configuration */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTCFG[dsadc_unit] = p_extend->dsadc_sta_sto_trigger_out_cfg[PIC_DSADTCFG_REG_0][dsadc_unit].dsadcstasto;

        p_instance_ctrl->p_regs_pic24->PIC2DSADTCFG[dsadc_unit] = p_extend->dsadc_sta_sto_trigger_out_cfg[PIC_DSADTCFG_REG_1][dsadc_unit].dsadcstasto;

        if(0 == dsadc_reg)
        {
            /* Selects start trigger from trigger source from GTM. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_atu_1shot_pulse_on_0n0_sel[dsadc_unit][dsadc_reg];
        }
        else if (1 == dsadc_reg){

            /* Selects start trigger source from ENCA. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN0[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_enca_0n1_sel[dsadc_unit][dsadc_reg];
        }

        if(0 == dsadc_reg)
        {
            /* Selects stop trigger trigger source from GTM. */
            p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
                p_extend->pic_dsadc_start_trigger_atu_1shot_pulse_on_1n0_sel[dsadc_unit][dsadc_reg];
        }
        else if (1 == dsadc_reg) {
            /* Selects stop trigger source from ENCA. */
        p_instance_ctrl->p_regs_pic20->PIC20DSADTEN[dsadc_unit].DSADTEN1[dsadc_reg] =
            p_extend->pic_dsadc_stop_trigger_enca_1n1_sel[dsadc_unit][dsadc_reg];
        }


        /* The PIC2DSADTSENk register selects a trigger for DSADC read gate. k= 0 */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN0 = p_extend->dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_0].dsadcrega;

        /* The PIC2DSADTSENk register selects a trigger for DSADC read gate. k= 1 */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN1 = p_extend->dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_1].dsadcrega;

        /* The PIC2DSADTSENk register selects a trigger for DSADC read gate. k= 4 */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN4 = p_extend->dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_4].dsadcrega;

        /* The PIC2DSADTSENk register selects a trigger for DSADC read gate. k= 5 */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN5 = p_extend->dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_5].dsadcrega;

        /* The PIC2DSADTSENk register selects a trigger for DSADC read gate. k= 6 */
        p_instance_ctrl->p_regs_pic24->PIC2DSADTSEN6 = p_extend->dsadc_read_gate_trigger_sel[PIC_DSADC_READ_GATE_6].dsadcrega;
        
    }
}

}
#endif

#if (BSP_FEATURE_PIC_CADC_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for CADC trigger selection function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_cadc_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Selects triggers in PIC22CADCATSEL0 registers.*/
    p_instance_ctrl->p_regs_pic22->PIC22CADCATSEL0 = (uint32_t) p_extend->pic_cadc_trigger_00_sel;

    /* Selects triggers in PIC22CADCATSEL1 registers. */
    p_instance_ctrl->p_regs_pic22->PIC22CADCATSEL1 = (uint32_t) p_extend->pic_cadc_trigger_01_sel;

#if (BSP_FEATURE_PIC_FEATURE_U2BX)

    /* Selects triggers in PIC23CADCATSEL0 registers.*/
    p_instance_ctrl->p_regs_pic23->PIC23CADCATSEL0 = (uint32_t) p_extend->pic_cadc_trigger_10_sel;

    /* Selects triggers in PIC23CADCATSEL0 registers. */
    p_instance_ctrl->p_regs_pic23->PIC23CADCATSEL1 = (uint32_t) p_extend->pic_cadc_trigger_11_sel;    
#endif

#if (BSP_FEATURE_PIC_FEATURE_U2BX)
 #if (BSP_FEATURE_PIC_CADC_TRIG_SEL_ATU)

    /* Selects start trigger source from ATU timer D 1shot pulse ON occurrence. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN002 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_on_002_sel;

    /* Selects start trigger source from ATU timer D 1shot pulse OFF occurrence. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN003 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_d_1shot_pulse_off_003_sel;

    /* Selects stop trigger source from ATU timer D 1shot pulse ON occurrence. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN102 = p_extend->pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_on_002_sel;

    /* Selects stop trigger source from ATU timer D 1shot pulse OFF occurrence. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN103 = p_extend->pic_cadc_stop_trigger_out_ctrl_atu_d_1shot_pulse_off_003_sel;

    /* Selects start trigger source from ATU timer C compare match and ATU timer G compare match occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN000 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_c_g_compare_match_sel;

    /* Selects start trigger source from ATU timer D compare match A and ATU timer G compare occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN001 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_d_g_compare_match_a_sel;

    /* Selects start trigger source from ATU timer D compare match B occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN002 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_d_compare_match_b_sel;

    /* Selects start trigger source from ATU timer D downcounter underflow occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN003 = p_extend->pic_cadc_start_trigger_out_ctrl_atu_d_downcounter_underflow_sel;

    /* Selects stop trigger from ATU timer C compare match and ATU timer G compare match occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN100 = p_extend->pic_cadc_stop_trigger_atu_c_g_compare_match_sel;

    /* Selects stop trigger source from ATU timer D compare match A and ATU timer G compare occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN101 = p_extend->pic_cadc_stop_trigger_atu_d_g_compare_match_a_sel;

    /* Selects stop trigger source from ATU timer D compare match B occurrence. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN102 = p_extend->pic_cadc_stop_trigger_atu_d_compare_match_b_sel;

    /* Selects stop trigger source from ATU timer D down-counter underflow. */
    p_instance_ctrl->p_regs_pic23->PIC23CADTEN103 = p_extend->pic_cadc_stop_trigger_atu_d_downcounter_underflow_sel;
 #endif
#endif

    /* Assigns timer output of GTM (TOM, ATOM, TIO, MCS, TIM) to PIC22CADTENj00 (j= 0,1) */
    /* Start / Stop Trigger Output Configuration */
    p_instance_ctrl->p_regs_pic24->PIC2CADTCFG0  = p_extend->cadc_sta_sto_trigger_out_cfg[PIC_CADTTCFG_REG_0].cadcstasto;

    p_instance_ctrl->p_regs_pic24->PIC2CADTCFG0  = p_extend->cadc_sta_sto_trigger_out_cfg[PIC_CADTTCFG_REG_1].cadcstasto;

    /* Selects start trigger source from GTM. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN000 = p_extend->pic_cadc_start_trigger_out_ctrl_gtm_000_sel;

    /* Selects stop trigger source from GTM. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN100 = p_extend->pic_cadc_stop_trigger_out_ctrl_gtm_100_sel;
    
    /* Enables a trigger source from ENCA */
    /* Selects start trigger source from ENCA. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN001 = p_extend->pic_cadc_start_trigger_out_ctrl_enca_100_sel;

    /* Selects stop trigger source from ENCA. */
    p_instance_ctrl->p_regs_pic22->PIC22CADTEN101 = p_extend->pic_cadc_stop_trigger_out_ctrl_enca_101_sel;

    /* The PIC2CADTSEN0 register selects a trigger for CADC00 read gate. */
    p_instance_ctrl->p_regs_pic24->PIC2CADTSEN0 = p_extend->cadc_read_gate_trigger_sel.cadcrega;
    
}
#endif

#if (BSP_FEATURE_PIC_EMU_FEATURE)

/*******************************************************************************************************************//**
 * Hardware configuration for EMU Peak/Trough Interrupt and A/D Conversion Trigger Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_emu_peak_trough_intr_adc_trigger_select_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
    if (PIC_MODE_ENABLE == p_extend->pic_emu_peak_trough_interrupt_adc_trigger_sel_enable)
    {
        /* Selects the GTM output as a signal source for EMU3. */
        p_instance_ctrl->p_regs_pic24->PIC2EMUISEN0 = ((uint32_t) p_extend->pic_emu_input_source_select.emuisen0);

        /* Selects inputs for EMU3S0. */
        p_instance_ctrl->p_regs_pic24->PIC2EMUISEL0 = p_extend->pic_emu_input_selection_control[PIC_EMU3S_UNIT_0];

        /* Selects inputs for EMU3S0. */
        p_instance_ctrl->p_regs_pic24->PIC2EMUISEL1 = p_extend->pic_emu_input_selection_control[PIC_EMU3S_UNIT_1];
    }
    else
    {
        /* Do nothing */
    }
}

#endif

#if (BSP_FEATURE_PIC_FCMP_FEATURE)

/*******************************************************************************************************************//**
 * Hardware configuration for FCMP Output Control.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_fcmp_output_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Selects the FCMP comparator output data signal. */
    p_instance_ctrl->p_regs_pic24->PIC2FCMPOCTL = ((uint32_t) p_extend->pic_fcmp_output_signal_sel);

    /* Selects low level of FCMP comparator output data as the sources input of HRPWM Hi-Z Control Function. */
    p_instance_ctrl->p_regs_pic24->PIC2FCMPOLSEN = ((uint32_t) p_extend->pic_fcmp_output_low_level_ctrl.fcmpolsen);

    /* Selects high level of FCMP comparator output data as the sources input of HRPWM High Low Fix Control Function and Hi-Z Control Function of PIC1.*/
    p_instance_ctrl->p_regs_pic24->PIC2FCMPOHTSGSEN = ((uint32_t) p_extend->pic_fcmp_output_high_level_tsg_ctrl.tsgsen);

    /* Selects low level of FCMP comparator output data as the sources input of HRPWM High Low Fix Control Function and Hi-Z Control Function of PIC1.*/
    p_instance_ctrl->p_regs_pic24->PIC2FCMPOLTSGSEN = ((uint32_t) p_extend->pic_fcmp_output_low_level_tsg_ctrl.tsgsen);

    /* Selects the FCMP comparator output data as a signal source for GTM. */
    for (uint8_t idx = 0; idx < PIC_GTMSEN_REG_NUMBER; idx++)
    {
        p_instance_ctrl->p_regs_pic24->PIC2GTMSEN[idx] = ((uint32_t) p_extend->pic_fcmp_output_gtm_ctrl[idx].gtmsen);
    }
}

#endif

#if (BSP_FEATURE_PIC_HRPWM_FEATURE)

/*******************************************************************************************************************//**
 * Hardware configuration for HRPWM Control Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_hrpwm_control_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /* Configure HRPWM High Low Fix Control Start Trigger for TSG31. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHLFIXCTS_b.PIC2HRPWMHLFIXCTS0 =
        ((uint8_t) p_extend->pic_hrpwm_highlow_fix_start_tsg31);

    /* Configure HRPWM High Low Fix Control Start Trigger for TSG32. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHLFIXCTS_b.PIC2HRPWMHLFIXCTS1 =
        ((uint8_t) p_extend->pic_hrpwm_highlow_fix_start_tsg32);

    /* Configure Hi-Z control input signals of FCMP comparator output data for HRPWM.*/
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZ = ((uint32_t) p_extend->pic_hrpwm_hiz_fcmp_ctrl);

    /* Configure HRPWM Hi-Z Control Start Trigger for ATOM1_IN0_HRPWM_HIZ. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZCTS_b.PIC2HRPWMHIZCTS0 =
        ((uint8_t) p_extend->pic_hrpwm_hiz_start_atom10);

    /* Configure HRPWM Hi-Z Control Start Trigger for ATOM1_IN1_HRPWM_HIZ. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZCTS_b.PIC2HRPWMHIZCTS1 =
        ((uint8_t) p_extend->pic_hrpwm_hiz_start_atom11);

    /* Configure HRPWM Hi-Z Control Start Trigger for ATOM2_IN0_HRPWM_HIZ. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZCTS_b.PIC2HRPWMHIZCTS2 =
        ((uint8_t) p_extend->pic_hrpwm_hiz_start_atom20);

    /* Configure HRPWM Hi-Z Control Start Trigger for ATOM2_IN1_HRPWM_HIZ. */
    p_instance_ctrl->p_regs_pic24->PIC2HRPWMHIZCTS_b.PIC2HRPWMHIZCTS3 =
        ((uint8_t) p_extend->pic_hrpwm_hiz_start_atom21);
}

#endif

#if (BSP_FEATURE_PIC_DFE_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for DFE Trigger Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_dfe_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_DFETSEN_NUMBER; idx++)
    {

    /** Selects hardware triggers for signal PIC2DFETSENk(k= 0 to 21).*/
    p_instance_ctrl->p_regs_pic24->PIC2DFETSEN[idx] =
        ((uint32_t) p_extend->pic_dfe_tim_trigger_sel[idx].dfetsen);

    /** Selects effective edge of DFE HW trigger signal PIC2DFETEDGSEL0.*/
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL0 =
        ((uint32_t) p_extend->pic_dfe_trigger_sel.dfe_tim_trigger_edge_sel_com_a.dfetedga);
    
    /** Selects effective edge of DFE HW trigger signal PIC2DFETEDGSEL1.*/
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL1 =
    ((uint32_t) p_extend->pic_dfe_trigger_sel.dfe_tim_trigger_edge_sel_com_b.dfetedgb);

    /** Selects effective edge of DFE HW trigger signal PIC2DFETEDGSEL2.*/
    p_instance_ctrl->p_regs_pic24->PIC2DFETEDGSEL2 =
    ((uint32_t) p_extend->pic_dfe_trigger_sel.dfe_tim_trigger_edge_sel_com.dfetedg);

    }
}

#endif

#if (BSP_FEATURE_PIC_PSI5_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for PSI5 Timestamp and the Sync Pulse Signal Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_psi5_timestamp_sync_pulse_sig_select_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t psi5_unit = 0; psi5_unit < BSP_FEATURE_PIC_PSI5EN_NUMBER_UNIT; psi5_unit++) 
    {
        for (uint8_t psi5_idx = 0; psi5_idx < BSP_FEATURE_PIC_PSI5EN_NUMBER; psi5_idx++)
        {
 #if (BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC24)

        /** Selects sources to PIC2PSI5EN input signals */
        p_instance_ctrl->p_regs_pic24->PIC2PSI5EN[psi5_idx] =
            ((uint32_t) p_extend->pic_psi5ts_sync_pul_sig_sel[psi5_unit][psi5_idx].psi5en);

 #elif (BSP_FEATURE_PIC_PSI5_TRIG_SEL_GTM_PIC2_SELB)
        /** Selects sources to PIC2PSI5EN input signals */
        p_instance_ctrl->p_regs_pic2_selb->PIC2PSI5EN[psi5_idx] =
            ((uint32_t) p_extend->pic_psi5ts_sync_pul_sig_sel[psi5_unit][psi5_idx].psi5en);
 #endif
       }
    }
}

#endif

#if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for PSI5-S Timestamp and the Sync Pulse Signal Selection Function
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_psi5s_timestamp_sync_pulse_sig_select_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t psi5s_unit = 0; psi5s_unit < BSP_FEATURE_PIC_PSI5S_NUMBER_UNIT; psi5s_unit++) 
    {
        for (uint8_t psi5s_idx = 0; psi5s_idx < BSP_FEATURE_PIC_PSI5SEN_NUMBER; psi5s_idx++)
        {
 #if (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC24)
            /** Selects sources to PIC2PSI5S0EN input signals */
            p_instance_ctrl->p_regs_pic24->PIC2PSI5S0EN[psi5s_idx] =
                ((uint32_t) p_extend->pic_psi5sts_sync_pul_sig_sel[psi5s_unit][psi5s_idx].psi5sen);

            /** Selects sources to PIC2PSI5S1EN input signals */
            p_instance_ctrl->p_regs_pic24->PIC2PSI5S1EN[psi5s_idx] =
                ((uint32_t) p_extend->pic_psi5sts_sync_pul_sig_sel[psi5s_unit][psi5s_idx].psi5sen);
 #elif (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC2_SELB)
            /** Selects sources to PIC2PSI5S0EN input signals */
            p_instance_ctrl->p_regs_pic2_selb->PIC2PSI5S0EN[psi5s_idx] =
                ((uint32_t) p_extend->pic_psi5sts_sync_pul_sig_sel[psi5s_unit][psi5s_idx].psi5sen);

 #elif (BSP_FEATURE_PIC_PSI5S_TRIG_SEL_GTM_PIC22)
            /** Selects sources to PIC2PSI5S0EN input signals */
            p_instance_ctrl->p_regs_pic22->PIC2PSI5S0EN[psi5s_idx] =
                ((uint32_t) p_extend->pic_psi5sts_sync_pul_sig_sel[psi5s_unit][psi5s_idx].psi5sen);

            /** Selects sources to PIC2PSI5S1EN input signals */
            p_instance_ctrl->p_regs_pic22->PIC2PSI5S1EN[psi5s_idx] =
                ((uint32_t) p_extend->pic_psi5sts_sync_pul_sig_sel[psi5s_unit][psi5s_idx].psi5sen);
 #endif
        }
    }
}

#endif

#if (BSP_FEATURE_PIC_ENCA_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for ENCA Trigger Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_enca_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    if (PIC_MODE_ENABLE == p_instance_ctrl->p_cfg->pic_enca_trigger)
    {
        pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC22)

        /* Selects a trigger for ENCA0TTIN0 . */
        p_instance_ctrl->p_regs_pic22->PIC2ENCA0TSEL0 =
            (uint32_t) p_extend->pic_enca_trigger_sel[PIC_ENCA_FEATURE_UNIT_0];

  #if (1 < BSP_FEATURE_PIC_ENCA_FEATURE_NUMBER_UNIT)

        /* Selects a trigger for ENCA1TTIN0. */
        p_instance_ctrl->p_regs_pic22->PIC2ENCA1TSEL0 =
            (uint32_t) p_extend->pic_enca_trigger_sel[PIC_ENCA_FEATURE_UNIT_1];
  #endif
 #elif (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATSEL_PIC21)

        /* Selects a trigger for ENCA0TTIN0 . */
        p_instance_ctrl->p_regs_pic21->PIC2ENCA0TSEL =
            (uint32_t) p_extend->pic_enca_trigger_sel[PIC_ENCA_FEATURE_UNIT_0];

        /* Selects a trigger for ENCA1TTIN0. */
        p_instance_ctrl->p_regs_pic21->PIC2ENCA1TSEL =
            (uint32_t) p_extend->pic_enca_trigger_sel[PIC_ENCA_FEATURE_UNIT_1];
 #endif

 #if (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC24)

        /* Assigns timer output of GTM (TOM, ATOM, TIO, TIM) to PIC2ENCAnTSEL0. */
        p_instance_ctrl->p_regs_pic24->PIC2ENCATCFG0 =
            (uint32_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_0].encatcfg;
        p_instance_ctrl->p_regs_pic24->PIC2ENCATCFG1 =
            (uint32_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_1].encatcfg;
 #elif (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC2_SELB)

        /* Assigns timer output of GTM (TOM, ATOM, TIO, TIM) to PIC2ENCAnTSEL0. */
        p_instance_ctrl->p_regs_pic2_selb->PIC2ENCATCFG0 =
            (uint32_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_0].encatcfg;
        p_instance_ctrl->p_regs_pic2_selb->PIC2ENCATCFG1 =
            (uint32_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_1].encatcfg;
 #elif (BSP_FEATURE_PIC_ENCA_TRIG_SEL_ENCATCFG_PIC22)

        /* Assigns timer output of GTM (TOM, ATOM, TIO, TIM) to PIC2ENCAnTSEL0. */
        p_instance_ctrl->p_regs_pic22->PIC2ENCATCFG0 =
            (uint16_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_0].encatcfg;
        p_instance_ctrl->p_regs_pic22->PIC2ENCATCFG1 =
            (uint16_t) p_extend->pic_enca_trigger_config[PIC_ENCATCFG_REG_1].encatcfg;
 #endif
    }
    else
    {
        /* Do nothing */
    }
}

#endif

/*******************************************************************************************************************//**
 * Hardware configuration for GTM Timer Input (TIM) Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_gtm_tim_input_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
#if (BSP_FEATURE_PIC_GTM_TIM_IN_PIC24)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        /* Selects source of timer input of GTM (TIM). */
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN01 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in01.gtminen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN23 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in23.gtminen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN45 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in45.gtminen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINEN[idx].GTM_TIM_IN67 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in67.gtminen;
    }

#elif (BSP_FEATURE_PIC_GTM_TIM_IN_PIC2_SELB)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        /* Selects source of timer input of GTM (TIM). */
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN01 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in01.gtminen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN23 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in23.gtminen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN45 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in45.gtminen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINEN[idx].GTM_TIM_IN67 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in67.gtminen;
    }

#elif (BSP_FEATURE_PIC_GTM_TIM_IN_PIC22)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINEN_NUMBER_GROUP; idx++)
    {
        /* Selects source of timer input of GTM (TIM). */
        p_instance_ctrl->p_regs_pic22->PIC2GTMINEN[idx].GTM_TIM_IN03 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in03.gtminen;
        p_instance_ctrl->p_regs_pic22->PIC2GTMINEN[idx].GTM_TIM_IN47 =
            p_extend->pic_gtm_tim_src_sel[idx].gtm_tim_in47.gtminen;
    }
#endif
}

#if (BSP_FEATURE_PIC_GTM_TIO_INPUT_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for GTM Timer Input (TIM) Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_gtm_tio_input_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_GTM_TIO_IN_PIC24)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP; idx++)
    {
        /* Selects source of timer input of GTM (TIO). */
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN01 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in01.gtmintioen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN23 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in23.gtmintioen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN45 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in45.gtmintioen;
        p_instance_ctrl->p_regs_pic24->PIC2GTMINTIOEN[idx].GTM_TIO_IN67 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in67.gtmintioen;
    }
#endif
 #if (BSP_FEATURE_PIC_GTM_TIO_IN_PIC2_SELB)
    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_GTMINTIOEN_NUMBER_GROUP; idx++)
    {
        /* Selects source of timer input of GTM (TIO). */
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN01 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in01.gtmintioen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN23 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in23.gtmintioen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN45 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in45.gtmintioen;
        p_instance_ctrl->p_regs_pic2_selb->PIC2GTMINTIOEN[idx].GTM_TIO_IN67 =
            p_extend->pic_gtm_tio_src_sel[idx].gtm_tio_in67.gtmintioen;
    }
 #endif
}

#endif

#if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for ENCA Encoder Input Selection.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_enca_enc_input_sel (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC24)

    /** Selects ENCAn input signals */
    p_instance_ctrl->p_regs_pic24->PIC2ENCAISEN0 = ((uint8_t) (p_extend->pic_enca_input_connect_sel));
 #elif (BSP_FEATURE_PIC_ENCA_ENC_INPUT_PIC22)

    /** Selects ENCAn input signals */
    p_instance_ctrl->p_regs_pic22->PIC2ENCAISEN = ((uint8_t) (p_extend->pic_enca_input_connect_sel));
 #endif
}

#endif

/*******************************************************************************************************************//**
 * Hardware configuration for Hi-Z Control Function Over External Pin for GTM Output.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_hiz_ctrl_gtm_out (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
#if (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC24)

    /** Configure Hi-Z control by ECM/ESOZ for GTM ATOM output */
    p_instance_ctrl->p_regs_pic24->PIC2ENHIZDTM = ((uint8_t) p_extend->pic_hiz_gtm_out);
#elif (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC2_SELB)

    /** Configure Hi-Z control by ECM/ESOZ for GTM ATOM output */
    p_instance_ctrl->p_regs_pic2_selb->PIC2ENHIZDTM = ((uint8_t) p_extend->pic_hiz_gtm_out);
#elif (BSP_FEATURE_PIC_HIZ_CTRL_GTM_PIC20)

    /** Configure Hi-Z control by ECM/ESOZ for GTM ATOM output */
    p_instance_ctrl->p_regs_pic20->PIC2ENHIZDTM = ((uint8_t) p_extend->pic_hiz_gtm_out);
#endif
}

#if (BSP_FEATURE_PIC_RDC_FEATURE)

/*******************************************************************************************************************//**
 * Hardware configuration for RDC Angle/Angular Velocity Latch Trigger.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_rdc_latch_trigger (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_RDC_FEATURE_RDC3AL && BSP_FEATURE_PIC_RDC_FEATURE_RDC3AS)

    /** Selects trigger source of RDC3AS0 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AS].PIC2RDCAVTSENk0700 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AS].PIC2RDCAVTSENk1508 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].omega_latch);

    /** Selects trigger source of RDC3AS1 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AS].PIC2RDCAVTSENk2316 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AS].PIC2RDCAVTSENk3124 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].omega_latch);

    /** Selects trigger source of RDC3AL0 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AL].PIC2RDCAVTSENk0700 =
        ((uint8_t) p_extend->pic_rdc3al_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AL].PIC2RDCAVTSENk1508 =
        ((uint8_t) p_extend->pic_rdc3al_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].omega_latch);

    /** Selects trigger source of RDC3AL1 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AL].PIC2RDCAVTSENk2316 =
        ((uint8_t) p_extend->pic_rdc3al_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN_b[PIC_RDC_FEATURE_RDC3AL].PIC2RDCAVTSENk3124 =
        ((uint8_t) p_extend->pic_rdc3al_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].omega_latch);
 #else

    /** Selects trigger source of RDC3AS0 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1_b.PIC2RDCAVTSENk0700 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1_b.PIC2RDCAVTSENk1508 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_0].omega_latch);

    /** Selects trigger source of RDC3AS1 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1_b.PIC2RDCAVTSENk2316 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].phi_latch);
    p_instance_ctrl->p_regs_pic24->PIC2RDCAVTSEN1_b.PIC2RDCAVTSENk3124 =
        ((uint8_t) p_extend->pic_rdc3as_latch_trigger_sel_ctrl[PIC_RDC3A_FEATURE_UNIT_1].omega_latch);
 #endif
}

/*******************************************************************************************************************//**
 * Hardware configuration for RDC Excitation Signal Input.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_rdc_excit_signal_input (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /** Selects the TPBA and GTM output as a signal source for RDC3AS0 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCEISEN0_b.PIC2RDCEISEN00400 =
        ((uint8_t) p_extend->pic_rdc3as_excit_input_sel[PIC_RDC3A_FEATURE_UNIT_0]);

    /** Selects the TPBA and GTM output as a signal source for RDC3AS1 */
    p_instance_ctrl->p_regs_pic24->PIC2RDCEISEN0_b.PIC2RDCEISEN01208 =
        ((uint8_t) p_extend->pic_rdc3as_excit_input_sel[PIC_RDC3A_FEATURE_UNIT_1]);
}

#endif

#if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE)

/*******************************************************************************************************************//**
 * Hardware configuration for IO sharing CAN / GTM- Internal arbitration.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_can_gtm_io_sharing (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_CANIOSEN_NUMBER; idx++)
    {
 #if (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC24)

        /** Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSEN register. */
        p_instance_ctrl->p_regs_pic24->PIC2CANIOSEN[idx] =
            ((uint32_t) p_extend->pic_can_gtm_io_sharing_ctrl[idx].caniosen);
 #elif (BSP_FEATURE_PIC_CAN_GTM_IO_SHARE_PIC2_SELB)

        /** Selects the GTM output as a signal source for IO sharing CAN/GTM- Internal arbitration function in PIC2CANIOSEN register. */
        p_instance_ctrl->p_regs_pic2_selb->PIC2CANIOSEN[idx] =
            ((uint32_t) p_extend->pic_can_gtm_io_sharing_ctrl[idx].caniosen);
 #endif
    }
}

#endif

#if (BSP_FEATURE_PIC_FCMP_FEATURE)

/*******************************************************************************************************************//**
 * Hardware configuration for FCMP Input Selection Control.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_fcmp_input_sel (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_FCMPISEN_NUMBER_REG; idx++)
    {
        /** Selects the GTM output as a signal source for FCMP in PIC2FCMPISEN register. */
        p_instance_ctrl->p_regs_pic24->PIC2FCMPISEN[idx] = ((uint32_t) p_extend->pic_fcmp_input_sel_ctrl[idx].fcmpisen);
    }
}

#endif

#if (BSP_FEATURE_PIC_MSPI_TRIG_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for MSPI Trigger Selection Function.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_mspi_trigger_sel_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);
 #if (BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC2_SELB)

    /** Selects hardware triggers for signal GTMMSPICFG0m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPICFG0 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_0].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG0m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_0] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_0].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG1m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPICFG1 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_1].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG1m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_1] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_1].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG2m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPICFG2 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_2].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG2m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic2_selb->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_2] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_2].edge_sel.gtmmspicfg);
 #elif (BSP_FEATURE_PIC_MSPI_TRIG_SEL_PIC24)

    /** Selects hardware triggers for signal GTMMSPICFG0m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG0 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_0].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG0m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_0] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_0].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG1m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG1 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_1].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG1m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_1] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_1].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG2m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG2 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_2].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG2m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_2] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_2].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG3m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG3 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_3].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG3m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_3] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_3].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG4m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG4 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_4].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG4m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_4] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_4].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG5m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG5 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_5].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG5m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_5] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_5].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG6m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG6 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_6].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG6m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_6] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_6].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG7m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG7 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_7].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG7m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_7] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_7].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG8m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG8 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_8].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG8m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_8] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_8].edge_sel.gtmmspicfg);

    /** Selects hardware triggers for signal GTMMSPICFG9m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPICFG9 =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_9].trigger_sel);

    /** Selects effective edge of MSPI HW trigger signa GTMMSPICFG9m (m = 0 to 2).  */
    p_instance_ctrl->p_regs_pic24->PIC2GTMMSPITS[PIC_GTM_MSPI_SIGNAL_9] =
        ((uint32_t) p_extend->pic_mspi_trigger_sel[PIC_GTM_MSPI_SIGNAL_9].edge_sel.gtmmspicfg);
 #endif
}

#endif

#if (BSP_FEATURE_PIC_GTM_DTM_INPUT_SEL)

/*******************************************************************************************************************//**
 * Hardware configuration for GTM DTM Input Selection Function
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_gtm_dtm_input_sel_abfg_func (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    for (uint8_t idx = 0; idx < BSP_FEATURE_PIC_ABFGODSEN_NUMBER; idx++)
    {
        /** Select the ABFG trigger source for GTM DTM in PIC2ABFGODSEN register. */
        p_instance_ctrl->p_regs_pic24->PIC2ABFGODSEN[idx] =
            ((uint32_t) p_extend->pic_gtm_abfg_out_dtm_sel_ctrl[idx].abfgodsen);
    }
}

#endif

#if (BSP_FEATURE_PIC_TSG3_INPUT_GTM)

/*******************************************************************************************************************//**
 * Hardware configuration for TSG3 inputs from GTM.
 *
 * @param[in]  p_instance_ctrl          pointer to control structure.
 **********************************************************************************************************************/
static void r_pic_tsg3_input_gtm (pic_instance_ctrl_t * const p_instance_ctrl)
{
    pic_extended_cfg_t * p_extend = ((pic_extended_cfg_t *) p_instance_ctrl->p_cfg->p_extend);

    /** Selects input for TSG30IVS, TSG30IVC, TSG30IUS, TSG30IUC. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG0 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_0].pic_tsg3_input0_sel_ctrl.tsg3trg0);

    /** Selects input for TSG30IUP, TSG30CLKI, TSG30IWS, TSG30IWC. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG1 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_0].pic_tsg3_input1_sel_ctrl.tsg3trg1);

    /** Selects input for TSG30IWP, TSG30IVP. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG30TRG2 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_0].pic_tsg3_input2_sel_ctrl.tsg3trg2);

    /** Selects input for TSG31IVS, TSG31IVC, TSG31IUS, TSG31IUC. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG0 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_1].pic_tsg3_input0_sel_ctrl.tsg3trg0);

    /** Selects input for TSG31IUP, TSG31CLKI, TSG31IWS, TSG31IWC. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG1 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_1].pic_tsg3_input1_sel_ctrl.tsg3trg1);

    /** Selects input for TSG31IWP, TSG31IVP. */
    p_instance_ctrl->p_regs_pic2_selb->PIC2TSG31TRG2 =
        ((uint32_t) p_extend->pic_tsg3_input_gtm_sel_ctrl[PIC_TSG3_FEATURE_UNIT_1].pic_tsg3_input2_sel_ctrl.tsg3trg2);
}

#endif

/* End of file R_PIC. */

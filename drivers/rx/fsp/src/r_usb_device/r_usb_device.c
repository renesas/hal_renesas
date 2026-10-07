/*
 * Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "bsp_api.h"
#include "r_usb_device.h"
#include "r_usb_device_define.h"

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
#define LITTLE_ENDIAN         0
#define BIG_ENDIAN            1
#if defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
 #define BYTE_ORDER           LITTLE_ENDIAN
#elif defined(__BYTE_ORDER__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
 #define BYTE_ORDER           BIG_ENDIAN
#else
 #error "Cannot determine endianness"
#endif

#define USBD_OPEN             (0x55534244) /* USBD in ASCII */

#define USB_EP_DIR_Msk        (0x80UL)
#define USB_EP_IN_DIR         (0x80UL)
#define USB_EP_OUT_DIR        (0UL)

#define USB_EP_IDX_Msk        (0x7FUL)
#define USB_EP_CONTROL_IN     (0x80UL)
#define USB_EP_CONTROL_OUT    (0UL)

#define USB_GET_EP_DIR(ep_addr)      (ep_addr & USB_EP_DIR_Msk)
#define USB_GET_EP_IDX(ep_addr)      (ep_addr & USB_EP_IDX_Msk)
#define USB_GET_EP_ADDR(idx, dir)    ((idx & USB_EP_IDX_Msk) | (dir & USB_EP_DIR_Msk))

#define USB_PIPECFG_DIR_IDX(dir)     ((dir == USB_EP_IN_DIR) ? 1 : 0)

#define USB_HIGHSPEED_MPS_Msk           (0x7ffUL)
#define USB_FULLSPEED_MPS_Msk           (0x1ffUL)

#define USB_XFER_TYPE_CONTROL           (0)
#define USB_XFER_TYPE_ISO               (1)
#define USB_XFER_TYPE_BULK              (2)
#define USB_XFER_TYPE_INT               (3)

#define USB_REQUEST_TYPE_SET_ADDRESS    (0x0500)

#define USB_MINIMUM_ACCESS_CYCLE_TIME   (41) /* nanoseconds */
#define USB_BUS_WAIT_OFFSET             (2) /* set bwait 0 -> wait for 2ns */
#define SECOND_TO_NANOSECONDS           (1000000000U)

/* TODO: BUFNMB should be changed depending on the allocation scheme */
#define R_USB_PIPEBUF_FIXED             (0x7C08) /* Fixed Pipe Buffer configurations */
#define R_USB_PIPEBUF_FIRST_HALF_START  (0x08)
#define R_USB_PIPEBUF_SECOND_HALF_START (0x48)

/***********************************************************************************************************************
 * Private constants
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/
typedef struct st_pipe_state
{
    void   * buf;                      /* the start address of a transfer data buffer */
    uint16_t length;                   /* the number of bytes in the buffer */
    uint16_t remaining;                /* the number of bytes remaining in the buffer */
    uint8_t  ep;                       /* an assigned endpoint address */
} pipe_state_t;

typedef struct st_usb_pipe_cfg
{
    pipe_state_t pipe[10];
    uint8_t      ep[2][16];            /* a lookup table for a pipe index from an endpoint address */
} usb_pipe_cfg_t;

typedef enum e_usb_control_stage
{
    USB_CONTROL_STAGE_IDLE   = 0,
    USB_CONTROL_STAGE_SETUP  = 0,
    USB_CONTROL_STAGE_DATA   = 1,
    USB_CONTROL_STAGE_STATUS = 2,
    USB_CONTROL_STAGE_ERROR  = 3,
} usb_control_stage_t;

/***********************************************************************************************************************
 * Private function prototypes
 **********************************************************************************************************************/
static inline uint16_t min16 (uint16_t x, uint16_t y) {
    return (x < y) ? x : y;
}

static inline uint32_t get_first_bit1_offset (uint32_t s) {
    return __builtin_ctz(s);
}

static inline uint16_t get_edpt_packet_size(usbd_instance_ctrl_t * const p_ctrl,
                                            usbd_desc_endpoint_t const * p_desc_ep);
static uint32_t                   find_pipe(usbd_instance_ctrl_t * const p_ctrl, uint32_t xfer_type);
static inline void                enable_pipe_information_change (usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline volatile uint16_t * get_pipectr(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static volatile void *            get_pipetre(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline uint16_t            edpt0_max_packet_size(usbd_instance_ctrl_t * const p_ctrl);
static inline uint16_t            edpt_max_packet_size(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline void                pipe_wait_for_ready(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline void                pipe_write_packet(usbd_instance_ctrl_t * const p_ctrl,
                                                    void                       * buf,
                                                    volatile void              * fifo,
                                                    unsigned int                 len);
static inline void pipe_read_packet(usbd_instance_ctrl_t * const p_ctrl,
                                    void                       * buf,
                                    volatile void              * fifo,
                                    unsigned int                 len);
static inline bool      pipe0_xfer_in(usbd_instance_ctrl_t * const p_ctrl);
static inline bool      pipe0_xfer_out(usbd_instance_ctrl_t * const p_ctrl);
static inline bool      pipe_xfer_in(usbd_instance_ctrl_t * const p_ctrl, uint8_t num);
static inline bool      pipe_xfer_out(usbd_instance_ctrl_t * const p_ctrl, uint8_t num);
static void             r_usb_device_call_callback(usbd_instance_ctrl_t * const p_ctrl, usbd_event_t * event);
static inline void      process_setup_packet(usbd_instance_ctrl_t * const p_ctrl);
static inline void      process_status_completion(usbd_instance_ctrl_t * const p_ctrl);
static inline fsp_err_t process_pipe0_xfer(usbd_instance_ctrl_t * const p_ctrl,
                                           uint8_t                      ep_addr,
                                           void                       * buffer,
                                           uint16_t                     total_bytes);
static inline fsp_err_t process_pipe_xfer(usbd_instance_ctrl_t * const p_ctrl,
                                          uint8_t                      ep_addr,
                                          void                       * buffer,
                                          uint16_t                     total_bytes);
static inline void process_pipe_forced_termination(usbd_instance_ctrl_t * const p_ctrl, uint8_t ep);
static inline void process_pipe0_bemp(usbd_instance_ctrl_t * const p_ctrl);
static inline void process_pipe_brdy(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline bool check_vbus_stable (usbd_instance_ctrl_t * p_ctrl);
static inline void process_bus_reset(usbd_instance_ctrl_t * const p_ctrl);
static inline void process_set_address(usbd_instance_ctrl_t * const p_ctrl);
static usb_control_stage_t dcp_get_current_stage(usbd_instance_ctrl_t * p_ctrl);
static inline void physical_init(usbd_instance_ctrl_t * const p_ctrl, usbd_cfg_t const * const p_cfg);
static inline void control_pipe_setup(usbd_instance_ctrl_t * const p_ctrl);
static inline void usb_interrupt_configure(usbd_instance_ctrl_t * p_ctrl, usbd_cfg_t const * p_cfg);
static inline void usb_module_start(usbd_instance_ctrl_t * p_ctrl);
static inline void usb_module_stop(usbd_instance_ctrl_t * p_ctrl);
static inline void usb_disable_interrupt(usbd_instance_ctrl_t * p_ctrl);
static inline void usb_enable_interrupt(usbd_instance_ctrl_t * p_ctrl);

#ifdef USB_HIGH_SPEED_MODULE
static inline fsp_err_t pipebuffer_allocate(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
static inline fsp_err_t pipebuffer_deallocate(usbd_instance_ctrl_t * const p_ctrl, uint32_t num);
#endif

/***********************************************************************************************************************
 * Private global variables
 **********************************************************************************************************************/
static usb_pipe_cfg_t g_pipe_cfg[USB_NUM_USBIP];

#ifdef USB_HIGH_SPEED_MODULE
static uint8_t g_pipe_allocation_status[USBHS_MAX_NUM_OF_ALLOCATABLE_PIPE] = {0};
static uint8_t g_fifo_buffer_usage = 0; /* This variable store status of buffer allocated */
#endif

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

/**
 * @brief Start USB module, configure correct operate mode and default pipe0
 *
 * @param p_api_ctrl
 * @param p_cfg
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_Open (usbd_ctrl_t * const p_api_ctrl, usbd_cfg_t const * const p_cfg)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_cfg);
    FSP_ERROR_RETURN(0 == p_ctrl->open, FSP_ERR_ALREADY_OPEN);

    /* Make sure this channel exists. */
    FSP_ERROR_RETURN(USB_NUM_USBIP > p_cfg->module_number, FSP_ERR_IP_CHANNEL_NOT_PRESENT);

    /* USBFS module only support FS operation */
    if (USB_IP0_MODULE == p_cfg->module_number)
    {
        FSP_ERROR_RETURN((USBD_SPEED_FS == p_cfg->usb_speed), FSP_ERR_INVALID_ARGUMENT);
    }

    /* USBHS module only support FS/HS operation */
    if (USB_IP1_MODULE == p_cfg->module_number)
    {
        FSP_ERROR_RETURN((USBD_SPEED_FS == p_cfg->usb_speed) || (USBD_SPEED_HS == p_cfg->usb_speed),
                         FSP_ERR_INVALID_ARGUMENT);
    }
#endif

#ifdef USB_HIGH_SPEED_MODULE
    p_ctrl->p_reg = (void *) (USB_IS_USBHS(p_cfg->module_number) ? R_USB_HS0_BASE : R_USB_FS0_BASE);
#else
    p_ctrl->p_reg = (void *) R_USB_FS0_BASE;
#endif
    p_ctrl->p_cfg             = p_cfg;
    p_ctrl->p_callback        = p_cfg->p_callback;
    p_ctrl->p_context         = p_cfg->p_context;
    p_ctrl->p_callback_memory = NULL;

    /* clear pipe config */
    memset(&g_pipe_cfg[p_cfg->module_number], 0, sizeof(usb_pipe_cfg_t));

    /* module start */
    usb_module_start(p_ctrl);

    /* physical layer init */
    physical_init(p_ctrl, p_cfg);

    /* setup default control pipe */
    control_pipe_setup(p_ctrl);

    /* enable interrupt */
    usb_interrupt_configure(p_ctrl, p_cfg);

    p_ctrl->open = USBD_OPEN;

    return FSP_SUCCESS;
}

/**
 * @brief Shutdown USB device
 *
 * @param p_api_ctrl
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_Close (usbd_ctrl_t * const p_api_ctrl)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* disable interrupt */
    usb_disable_interrupt(p_ctrl);

    /* module stop */
    usb_module_stop(p_ctrl);

    p_ctrl->open = 0;

    return FSP_SUCCESS;
}

/**
 * @brief Connect USB D+/D- pin to the USB data bus
 *
 * @param p_api_ctrl
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_Connect (usbd_ctrl_t * const p_api_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
#endif

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    /* Set the D+ pull up for the host controller to recognize and enumerate this device */
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->SYSCFG_b.DPRPU = 1;
    }
    else
#endif
    {
        R_USB_FS0->SYSCFG_b.DPRPU = 1;
    }

    return FSP_SUCCESS;
}

/**
 * @brief Disconnect pin D+/D- to USB data bus
 *
 * @param p_api_ctrl
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_Disconnect (usbd_ctrl_t * const p_api_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
#endif

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->SYSCFG_b.DPRPU = 0;
    }
    else
#endif
    {
        R_USB_FS0->SYSCFG_b.DPRPU = 0;
    }

    return FSP_SUCCESS;
}

/**
 * @brief Configure an endpoint to make it ready to transfer
 *
 * @param p_api_ctrl
 * @param p_ep_desc
 *
 * @retval FSP_SUCCESS          on success
 * @retval FSP_ERR_USB_BUSY     if these is no available pipe can be used with this endpoint
 */
fsp_err_t R_USBD_EdptOpen (usbd_ctrl_t * const p_api_ctrl, usbd_desc_endpoint_t const * p_ep_desc)
{
    usbd_instance_ctrl_t * const p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ASSERT(p_ep_desc);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    const uint8_t  ep_addr = p_ep_desc->bEndpointAddress;
    const uint8_t  epn     = USB_GET_EP_IDX(ep_addr);
    const uint8_t  dir     = USB_GET_EP_DIR(ep_addr);
    const uint8_t  xfer    = p_ep_desc->Attributes.xfer;
    const uint32_t num     = find_pipe(p_ctrl, xfer);
#ifdef USB_HIGH_SPEED_MODULE
    volatile uint16_t * d0fifosel = USB_IS_USBHS(p_ctrl->p_cfg->module_number) ?
                                    &R_USB_HS0->D0FIFOSEL :
                                    &R_USB_FS0->D0FIFOSEL;
    volatile uint16_t * d1fifosel = USB_IS_USBHS(p_ctrl->p_cfg->module_number) ?
                                    &R_USB_HS0->D1FIFOSEL :
                                    &R_USB_FS0->D1FIFOSEL;

#else
    volatile uint16_t * d0fifosel = &R_USB_FS0->D0FIFOSEL;
    volatile uint16_t * d1fifosel = &R_USB_FS0->D1FIFOSEL;
#endif
    uint16_t d0fifosel_save = *d0fifosel;
    uint16_t d1fifosel_save = *d1fifosel;

    /* There are no available pipes that can be configured for this endpoint. */
    FSP_ERROR_RETURN(0 != num, FSP_ERR_USB_BUSY);

    g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num].ep = ep_addr;
    g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn] = num;

    /* setup pipe */
    usb_disable_interrupt(p_ctrl);
    enable_pipe_information_change(p_ctrl, num);

    /* Clear the D0FIFOSEL and D1FIFOSEL to ensure the pipe can be change setting */

    *d0fifosel = 0;
    *d1fifosel = 0;

    /* When modified CURPIPE[3:0] bits should wait that value to be applied */
    FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk), 0);
    FSP_HARDWARE_REGISTER_WAIT((*d1fifosel & R_USB_D1FIFOSEL_CURPIPE_Msk), 0);

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->PIPESEL  = num;
        /* Set pipe buffer depends on num */
        pipebuffer_allocate(p_ctrl, num);
        R_USB_HS0->PIPEMAXP = get_edpt_packet_size(p_ctrl, p_ep_desc);
    }
    else
#endif
    {
        R_USB_FS0->PIPESEL  = num;
        R_USB_FS0->PIPEMAXP = get_edpt_packet_size(p_ctrl, p_ep_desc);
    }

    /**
     * Initialize pipe
     *   Set the SQCLR => Clear the sequence toggle to choose next transfer is DATA0 in this pipe
     *   Set ACLRM to 1 and then 0 to completely clear the information in the FIFO buffer
     *  allocated to this pipe.
     *   According to HWUM need at least 100ns between modify ACLRM from 0 to 1.
     */
    volatile uint16_t * ctr = get_pipectr(p_api_ctrl, num);
    *ctr = R_USB_PIPE_CTR_ACLRM_Msk | R_USB_PIPE_CTR_SQCLR_Msk;
    /* The minimum unit of software delay is 1 us */
    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    *ctr = 0;

    uint16_t cfg = (USB_PIPECFG_DIR_IDX(dir) << R_USB_PIPECFG_DIR_Pos) | epn;

    if (xfer == USB_XFER_TYPE_BULK)
    {
        if (USB_EP_IN_DIR == dir)
        {
            /* IN: DBLB lets the CPU fill one plane while the SIE transmits from the other. */
            cfg |= (R_USB_PIPECFG_TYPE_BULK | R_USB_PIPECFG_DBLB_Msk);
        }
        else
        {
            /* OUT: SHTNAK NAKs the pipe after a short/ZLP packet.  Do NOT use DBLB —
             * HWM §BCLR: with DBLB, BCLR clears only the CPU-side plane; if the SIE-side
             * plane already holds the next transaction's data (possible before SHTNAK takes
             * effect), the following pipe_xfer_out() sees DTLN=1 instead of 0 for the ZLP,
             * reporting one extra byte and causing CDC-NCM "block len" mismatch. */
            cfg |= (R_USB_PIPECFG_TYPE_BULK | R_USB_PIPECFG_SHTNAK_Msk);
        }
    }
    else if (xfer == USB_XFER_TYPE_INT)
    {
        cfg |= R_USB_PIPECFG_TYPE_INT;
    }
    else
    {
        cfg |= (R_USB_PIPECFG_TYPE_ISO | R_USB_PIPECFG_DBLB_Msk);
    }

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->PIPECFG  = cfg;
        R_USB_HS0->BRDYSTS  = R_USB_BRDYSTS_PIPEBRDY_Msk ^ (1 << num);
        R_USB_HS0->BRDYENB |= (1 << num);
    }
    else
#endif
    {
        R_USB_FS0->PIPECFG  = cfg;
        R_USB_FS0->BRDYSTS  = R_USB_BRDYSTS_PIPEBRDY_Msk ^ (1 << num);
        R_USB_FS0->BRDYENB |= (1 << num);
    }

    /**
     * Change PID response to BUF response (depending on buffer state) to enable pipe operation
     */
    if ((USB_EP_IN_DIR == dir) || (USB_XFER_TYPE_BULK != xfer))
    {
        *ctr = R_USB_PIPE_CTR_PID_BUF;
    }

    /* Restore the FIFOSELs */

    *d0fifosel = d0fifosel_save;
    *d1fifosel = d1fifosel_save;
    FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk),
                                (d0fifosel_save & R_USB_D0FIFOSEL_CURPIPE_Msk));
    FSP_HARDWARE_REGISTER_WAIT((*d1fifosel & R_USB_D1FIFOSEL_CURPIPE_Msk),
                                (d1fifosel_save & R_USB_D1FIFOSEL_CURPIPE_Msk));

    usb_enable_interrupt(p_ctrl);

    return FSP_SUCCESS;
}

/**
 * @brief Remove configuration for an endpoint
 *
 * @param p_api_ctrl
 * @param ep_addr
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_EdptClose (usbd_ctrl_t * const p_api_ctrl, uint8_t ep_addr)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    const uint8_t epn = USB_GET_EP_IDX(ep_addr);
    const uint8_t dir = USB_GET_EP_DIR(ep_addr);
    const uint8_t num = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn];

    enable_pipe_information_change(p_ctrl, num);
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        volatile uint16_t * ctr = get_pipectr(p_ctrl, num);

        R_USB_HS0->BRDYENB &= ~(1 << num);
        *ctr                = 0;
        R_USB_HS0->PIPESEL  = num;
        pipebuffer_deallocate(p_ctrl, num);
        R_USB_HS0->PIPECFG  = 0;
    }
    else
#endif
    {
        volatile uint16_t * ctr = get_pipectr(p_ctrl, num);

        R_USB_FS0->BRDYENB &= ~(1 << num);
        *ctr                = 0;
        R_USB_FS0->PIPESEL  = num;
        R_USB_FS0->PIPECFG  = 0;
    }

    g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num].ep = 0;
    g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn] = 0;

    return FSP_SUCCESS;
}

/**
 * @brief Wake up host
 *
 * @param p_api_ctrl
 *
 * @retval FSP_SUCCESS on success
 * @retval FSP_ERR_USB_NOT_SUSPEND if this function is called when device not in suspended state
 */
fsp_err_t R_USBD_RemoteWakeup (usbd_ctrl_t * const p_api_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;
#else
    FSP_PARAMETER_NOT_USED(p_api_ctrl);
#endif

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        /* Check if the current state is suspended state */
        if ((R_USB_HS0->INTSTS0 & R_USB_INTSTS0_DVSQ_STATE_SUSP0) == 0) {
            /* Bit indicate suspended state is not set */
            return FSP_ERR_USB_NOT_SUSPEND;
        }

        /* As mentioned in HWUM, do not stop the PHY clock when set WKUP to 1 */
        R_USB_HS0->LPSTS_b.SUSPENDM = 1; /* Keep supply PHY clock */
        R_USB_HS0->DVSTCTR0_b.WKUP = 1;
    }
    else
#endif
    {
        R_USB_FS0->DVSTCTR0_b.WKUP = 1;
    }

    return FSP_SUCCESS;
}

/**
 * @brief Trigger an endpoint transfer
 *
 * @param p_api_ctrl
 * @param ep_addr
 * @param buffer
 * @param total_bytes
 *
 * @retval FSP_SUCCESS              on success
 * @retval FSP_ERR_USB_NOT_OPEN     if input endpoint has not opened yet
 */
fsp_err_t R_USBD_XferStart (usbd_ctrl_t * const p_api_ctrl, uint8_t ep_addr, uint8_t * buffer, uint16_t total_bytes)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;
    fsp_err_t              err    = FSP_SUCCESS;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    usb_disable_interrupt(p_ctrl);

    if (USB_GET_EP_IDX(ep_addr) > 0)
    {
        err = process_pipe_xfer(p_api_ctrl, ep_addr, buffer, total_bytes);
    }
    else
    {
        err = process_pipe0_xfer(p_api_ctrl, ep_addr, buffer, total_bytes);
    }

    usb_enable_interrupt(p_ctrl);

    return err;
}

/**
 * @brief Abort on-going transfer
 *
 * @param p_api_ctrl
 * @param ep_addr
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_XferAbort (usbd_ctrl_t * const p_api_ctrl, uint8_t ep_addr)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    usb_disable_interrupt(p_ctrl);

    process_pipe_forced_termination(p_ctrl, ep_addr);

    usb_enable_interrupt(p_ctrl);

    return FSP_SUCCESS;
}

/**
 * @brief Set a endpoint to halt and send a stall packet
 *
 * @param p_api_ctrl
 * @param ep_addr
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_EdptStall (usbd_ctrl_t * const p_api_ctrl, uint8_t ep_addr)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    usb_disable_interrupt(p_ctrl);

    const uint8_t       epn     = USB_GET_EP_IDX(ep_addr);
    const uint8_t       dir     = USB_GET_EP_DIR(ep_addr);
    const uint8_t       num     = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn];
    volatile uint16_t * pipectr = get_pipectr(p_api_ctrl, num);
    const uint16_t      pid     = *pipectr & R_USB_PIPE_CTR_PID_Msk;

    /**
     * If current PID is NAK (00b) set to 10b (STALL)
     * If current PID is BUF (01b) set to 11b (STALL)
     */
    *pipectr = pid | R_USB_PIPE_CTR_PID_STALL;

    /**
     * When change 11b to 00b, 11b -> 10b -> 00b
     * When change 11b to 01b, 11b -> 10b -> 00b -> 01b
     * So set the PID to 10b first to prepare for the clear stall
     */
    *pipectr = R_USB_PIPE_CTR_PID_STALL;

    usb_enable_interrupt(p_ctrl);

    return FSP_SUCCESS;
}

/**
 * @brief Reset halt state of an endpoint
 *
 * @param p_api_ctrl
 * @param ep_addr
 *
 * @retval FSP_SUCCESS on success
 */
fsp_err_t R_USBD_EdptClearStall (usbd_ctrl_t * const p_api_ctrl, uint8_t ep_addr)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif

    usb_disable_interrupt(p_ctrl);

    const uint8_t       epn     = USB_GET_EP_IDX(ep_addr);
    const uint8_t       dir     = USB_GET_EP_DIR(ep_addr);
    const uint8_t       num     = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn];
    volatile uint16_t * pipectr = get_pipectr(p_api_ctrl, num);

    *pipectr = R_USB_PIPE_CTR_SQCLR_Msk;
    if (USB_EP_IN_DIR == dir)
    {
        /**
         * According to HWUM,
         * If change PID from STALL to BUF
         * Change PID to NAK before change it to BUF
         */
        *pipectr = R_USB_PIPE_CTR_PID_NAK;
        *pipectr = R_USB_PIPE_CTR_PID_BUF;
    }
    else
    {
#ifdef USB_HIGH_SPEED_MODULE
        if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
        {
            R_USB_HS0->PIPESEL = num;
            if (R_USB_HS0->PIPECFG_b.TYPE != 1)
            {
                *pipectr = R_USB_PIPE_CTR_PID_NAK;
                *pipectr = R_USB_PIPE_CTR_PID_BUF;
            }
        }
        else
#endif
        {
            R_USB_FS0->PIPESEL = num;
            if (R_USB_FS0->PIPECFG_b.TYPE != 1)
            {
                *pipectr = R_USB_PIPE_CTR_PID_NAK;
                *pipectr = R_USB_PIPE_CTR_PID_BUF;
            }
        }
    }

    usb_enable_interrupt(p_ctrl);

    return FSP_SUCCESS;
}

/**
 * @brief Reset halt state of an endpoint
 *
 * @param p_api_ctrl
 * @param mode
 *
 * @retval FSP_SUCCESS on success
 * @retval FSP_ERR_UNSUPPORTED if test mode is not supported
 * @retval FSP_ERR_NOT_OPEN if the USB device is not opened
 * @retval FSP_ERR_INVALID_ARGUMENT if the mode is not valid
 */
fsp_err_t R_USBD_Testmode(usbd_ctrl_t * const p_api_ctrl, const usbd_test_mode_t mode)
{
    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) p_api_ctrl;

#if USBD_CFG_PARAM_CHECKING_ENABLE
    FSP_ASSERT(p_ctrl);
    FSP_ERROR_RETURN(0 != p_ctrl->open, FSP_ERR_NOT_OPEN);
#endif
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        if (mode >= 5) {
            return FSP_ERR_INVALID_ARGUMENT; /* Invalid test mode */
        }
        R_USB_HS0->TESTMODE_b.UTST = mode;
    }
    else
#endif
    {
        /* Test mode is supported only in high-speed */
        return FSP_ERR_UNSUPPORTED;
    }

    return FSP_SUCCESS;
}

/**********************************************************************************************************************
 * Private functions
 **********************************************************************************************************************/

/* get the mps from an EP descriptor */
static inline uint16_t get_edpt_packet_size (usbd_instance_ctrl_t * const p_ctrl,
                                             usbd_desc_endpoint_t const * p_desc_ep)
{
    uint16_t mps = 0;

#ifdef USB_HIGH_SPEED_MODULE
    if (p_ctrl->p_cfg->usb_speed == USBD_SPEED_HS)
    {
        mps = p_desc_ep->wMaxPacketSize & USB_HIGHSPEED_MPS_Msk;
    }
    else
#endif
    {
        mps = p_desc_ep->wMaxPacketSize & USB_FULLSPEED_MPS_Msk;
    }

    return mps;
}

/* get the available pipe can be used to configure for new endpoint */
static uint32_t find_pipe (usbd_instance_ctrl_t * const p_ctrl, uint32_t xfer_type)
{
    const uint8_t pipe_idx_arr[4][2] =
    {
        {0, 0},                        // Control
        {1, 2},                        // Isochronous
        {1, 5},                        // Bulk
        {6, 9},                        // Interrupt
    };

    /* find backward since only pipe 1, 2 support ISO */
    const uint8_t idx_first = pipe_idx_arr[xfer_type][0];
    const uint8_t idx_last  = pipe_idx_arr[xfer_type][1];

    for (int i = idx_last; i >= idx_first; i--)
    {
        if (0 == g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[i].ep)
        {
            return i;
        }
    }

    return 0;
}

/* get the correct PIPE_CTR register by the pipe number */
static inline volatile uint16_t * get_pipectr (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    volatile uint16_t * pipectr = NULL;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        if (0 != num)
        {
            pipectr = &R_USB_HS0->PIPE_CTR[num - 1];
        }
        else
        {
            pipectr = &R_USB_HS0->DCPCTR;
        }
    }
    else
#endif
    {
        if (0 != num)
        {
            pipectr = &R_USB_FS0->PIPE_CTR[num - 1];
        }
        else
        {
            pipectr = &R_USB_FS0->DCPCTR;
        }
    }

    return pipectr;
}

/* get the correct PIPE_TR by the pipe number */
static volatile void * get_pipetre (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    volatile void * pipetre = NULL;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        pipetre = (1 <= num) && (num <= 5) ? &(R_USB_HS0->PIPE_TR[num - 1].E) : NULL;
    }
    else
#endif
    {
        pipetre = (1 <= num) && (num <= 5) ? &(R_USB_FS0->PIPE_TR[num - 1].E) : NULL;
    }

    return pipetre;
}

/* get the mps of pipe0 */
static inline uint16_t edpt0_max_packet_size (usbd_instance_ctrl_t * const p_ctrl)
{
    uint16_t mps = 0;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        mps = R_USB_HS0->DCPMAXP_b.MXPS;
    }
    else
#endif
    {
        mps = R_USB_FS0->DCPMAXP_b.MXPS;
    }

    return mps;
}

/* get the mps of common endpoint */
static inline uint16_t edpt_max_packet_size (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    uint16_t mps = 0;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->PIPESEL = num;

        mps = R_USB_HS0->PIPEMAXP;
    }
    else
#endif
    {
        R_USB_FS0->PIPESEL = num;

        mps = R_USB_FS0->PIPEMAXP;
    }

    return mps;
}

/* wait for pipe to be ready */
static inline void pipe_wait_for_ready (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->D0FIFOSEL_b.CURPIPE, num);
        FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->D0FIFOCTR_b.FRDY, 1);
    }
    else
#endif
    {
        FSP_HARDWARE_REGISTER_WAIT(R_USB_FS0->D0FIFOSEL_b.CURPIPE, num);
        FSP_HARDWARE_REGISTER_WAIT(R_USB_FS0->D0FIFOCTR_b.FRDY, 1);
    }
}

/* write data buffer --> hw fifo */
static inline void pipe_write_packet (usbd_instance_ctrl_t * const p_ctrl,
                                      void                       * buf,
                                      volatile void              * fifo,
                                      unsigned int                 len)
{
    volatile uint16_t * ff16;
    volatile uint8_t  * ff8;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        /* These offsets are only for the LITTLE ENDIAN */
        /* Highspeed FIFO is 32-bit */
        ff16 = (volatile uint16_t *) ((uintptr_t) fifo + 2);
        ff8  = (volatile uint8_t *) ((uintptr_t) fifo + 3);
    }
    else
#endif
    {
        ff16 = (volatile uint16_t *) fifo;
        ff8  = ((volatile uint8_t *) fifo);
    }

    uint8_t const * buf8 = (uint8_t const *) buf;

    while (len >= 2)
    {
        *ff16 = *(uint16_t *) (buf8);
        buf8 += 2;
        len  -= 2;
    }

    if (len > 0)
    {
        *ff8 = *buf8;
        ++buf8;
    }
}

/* read data buffer <-- hw fifo */
static inline void pipe_read_packet (usbd_instance_ctrl_t * const p_ctrl,
                                     void                       * buf,
                                     volatile void              * fifo,
                                     unsigned int                 len)
{
    FSP_PARAMETER_NOT_USED(p_ctrl);
    uint8_t          * p_buf = (uint8_t *) buf;
    volatile uint8_t * reg   = (volatile uint8_t *) fifo; /* byte access is always at base register address */

    while (len--)
    {
        *p_buf++ = *reg;
    }
}

/* process IN direction transfer through pipe0 */
static inline bool pipe0_xfer_in (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile void     * cfifo     = hs_module ? &R_USB_HS0->CFIFO : &R_USB_FS0->CFIFO;
    volatile uint16_t * cfifoctr  = hs_module ? &R_USB_HS0->CFIFOCTR : &R_USB_FS0->CFIFOCTR;
#else
    volatile void     * cfifo    = &R_USB_FS0->CFIFO;
    volatile uint16_t * cfifoctr = &R_USB_FS0->CFIFOCTR;
#endif

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[0];
    const uint16_t rem  = pipe->remaining;

    if (0 == rem)
    {
        pipe->buf = NULL;

        return true;
    }

    const uint16_t mps = edpt0_max_packet_size(p_ctrl);
    const uint16_t len = min16(mps, rem);
    void         * buf = pipe->buf;

    if (len)
    {
        pipe_write_packet(p_ctrl, buf, cfifo, len);
        pipe->buf = (uint8_t *) buf + len;
    }

    if (len < mps)
    {
        /** Enable transfer manually when data not fullfill the buffer */
        *cfifoctr = R_USB_CFIFOCTR_BVAL_Msk;
    }

    pipe->remaining = rem - len;

    return false;
}

/* process OUT direction transfer through pipe0 */
static inline bool pipe0_xfer_out (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile void     * cfifo     = hs_module ? &R_USB_HS0->CFIFO : &R_USB_FS0->CFIFO;
    volatile uint16_t * cfifoctr  = hs_module ? &R_USB_HS0->CFIFOCTR : &R_USB_FS0->CFIFOCTR;
#else
    volatile void     * cfifo    = &R_USB_FS0->CFIFO;
    volatile uint16_t * cfifoctr = &R_USB_FS0->CFIFOCTR;
#endif

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[0];
    const uint16_t rem  = pipe->remaining;
    const uint16_t mps  = edpt0_max_packet_size(p_ctrl);
#ifdef USB_HIGH_SPEED_MODULE
    const uint16_t vld = hs_module ? R_USB_HS0->CFIFOCTR_b.DTLN : R_USB_FS0->CFIFOCTR_b.DTLN;
#else
    const uint16_t vld = R_USB_FS0->CFIFOCTR_b.DTLN;
#endif
    const uint16_t len = min16(min16(rem, mps), vld);
    void         * buf = pipe->buf;

    if (len)
    {
        pipe_read_packet(p_ctrl, buf, cfifo, len);
        pipe->buf = (uint8_t *) buf + len;
    }

    if (len < mps)
    {
        /* All data are read, clear buffer */
        *cfifoctr = R_USB_CFIFOCTR_BCLR_Msk;
    }

    pipe->remaining = rem - len;
    if ((len < mps) || (rem == len))
    {
        pipe->buf = NULL;

        return true;
    }

    return false;
}

/* process IN direction transfer for common pipe */
static inline bool pipe_xfer_in (usbd_instance_ctrl_t * const p_ctrl, uint8_t num)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile uint16_t * d0fifosel = hs_module ? &R_USB_HS0->D0FIFOSEL : &R_USB_FS0->D0FIFOSEL;
    volatile void     * d0fifo    = hs_module ? &R_USB_HS0->D0FIFO : &R_USB_FS0->D0FIFO;
    volatile uint16_t * d0fifoctr = hs_module ? &R_USB_HS0->D0FIFOCTR : &R_USB_FS0->D0FIFOCTR;
#else
    volatile uint16_t * d0fifosel = &R_USB_FS0->D0FIFOSEL;
    volatile void     * d0fifo    = &R_USB_FS0->D0FIFO;
    volatile uint16_t * d0fifoctr = &R_USB_FS0->D0FIFOCTR;
#endif

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num];
    const uint16_t rem  = pipe->remaining;

    if (!rem)
    {
        pipe->buf = NULL;

        return true;
    }

    *d0fifosel = num | R_USB_FIFOSEL_MBW_16BIT | (BYTE_ORDER == BIG_ENDIAN ? R_USB_FIFOSEL_BIGEND : 0);
    pipe_wait_for_ready(p_ctrl, num);

    const uint16_t mps = edpt_max_packet_size(p_ctrl, num);
    const uint16_t len = min16(rem, mps);
    void         * buf = pipe->buf;

    if (len)
    {
        pipe_write_packet(p_ctrl, buf, d0fifo, len);
        pipe->buf = (uint8_t *) buf + len;
    }

    if (len < mps)
    {
        /**
         * The data to be written cannot fullfill the FIFO buffer
         * Set the BVAL to enable the data transmission
         */
        *d0fifoctr = R_USB_D0FIFOCTR_BVAL_Msk;
    }

    *d0fifosel = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk), 0);

    pipe->remaining = rem - len;

    return false;
}

/* process OUT direction transfer for common pipe */
static inline bool pipe_xfer_out (usbd_instance_ctrl_t * const p_ctrl, uint8_t num)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile uint16_t * d0fifosel = hs_module ? &R_USB_HS0->D0FIFOSEL : &R_USB_FS0->D0FIFOSEL;
    volatile uint32_t * d0fifo    = hs_module ? &R_USB_HS0->D0FIFO : &R_USB_FS0->D0FIFO;
    volatile uint16_t * d0fifoctr = hs_module ? &R_USB_HS0->D0FIFOCTR : &R_USB_FS0->D0FIFOCTR;
#else
    volatile uint16_t * d0fifosel = &R_USB_FS0->D0FIFOSEL;
    volatile uint32_t * d0fifo    = &R_USB_FS0->D0FIFO;
    volatile uint16_t * d0fifoctr = &R_USB_FS0->D0FIFOCTR;
#endif

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num];
    const uint16_t rem  = pipe->remaining;

    *d0fifosel = num | R_USB_FIFOSEL_MBW_8BIT;

    const uint16_t mps = edpt_max_packet_size(p_ctrl, num);
    pipe_wait_for_ready(p_ctrl, num);

#ifdef USB_HIGH_SPEED_MODULE
    const uint16_t vld = hs_module ? R_USB_HS0->D0FIFOCTR_b.DTLN : R_USB_FS0->D0FIFOCTR_b.DTLN;
#else
    const uint16_t vld = R_USB_FS0->D0FIFOCTR_b.DTLN;
#endif
    const uint16_t len = min16(min16(rem, mps), vld);
    void         * buf = pipe->buf;

    if (len > 0)
    {
        pipe_read_packet(p_ctrl, buf, d0fifo, len);
        pipe->buf = (uint8_t *) buf + len;
    }

    if (len < mps)
    {
        /* All data are read, clear buffer */
        *d0fifoctr = R_USB_D0FIFOCTR_BCLR_Msk;
    }

    *d0fifosel = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk), 0);

    pipe->remaining = rem - len;
    if ((len < mps) || (rem == len))
    {
        pipe->buf = NULL;

        return NULL != buf;
    }

    return false;
}

/* setup packet received event handler */
static inline void process_setup_packet (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile uint16_t * intsts0   = hs_module ? &R_USB_HS0->INTSTS0 : &R_USB_FS0->INTSTS0;
    volatile uint16_t * cfifoctr  = hs_module ? &R_USB_HS0->CFIFOCTR : &R_USB_FS0->CFIFOCTR;
#else
    volatile uint16_t * intsts0  = &R_USB_FS0->INTSTS0;
    volatile uint16_t * cfifoctr = &R_USB_FS0->CFIFOCTR;
#endif

    if (0 == (*intsts0 & R_USB_INTSTS0_VALID_Msk))
    {
        return;
    }

    *cfifoctr = R_USB_CFIFOCTR_BCLR_Msk;

#ifdef USB_HIGH_SPEED_MODULE
    usbd_event_t event =
    {
        .event_id       = USBD_EVENT_SETUP_RECEIVED,
        .setup_received =
        {
            hs_module ? R_USB_HS0->USBREQ : R_USB_FS0->USBREQ,
            hs_module ? R_USB_HS0->USBVAL : R_USB_FS0->USBVAL,
            hs_module ? R_USB_HS0->USBINDX : R_USB_FS0->USBINDX,
            hs_module ? R_USB_HS0->USBLENG : R_USB_FS0->USBLENG
        }
    };
#else
    usbd_event_t event =
    {
        .event_id       = USBD_EVENT_SETUP_RECEIVED,
        .setup_received =
        {
            R_USB_FS0->USBREQ,
            R_USB_FS0->USBVAL,
            R_USB_FS0->USBINDX,
            R_USB_FS0->USBLENG
        }
    };
#endif

    r_usb_device_call_callback(p_ctrl, &event);

    *intsts0 = ~((uint16_t) R_USB_INTSTS0_VALID_Msk);
}

/* status packet transfer complete event handler */
static inline void process_status_completion (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    volatile uint16_t * cfifosel =
        USB_IS_USBHS(p_ctrl->p_cfg->module_number) ? &R_USB_HS0->CFIFOSEL : &R_USB_FS0->CFIFOSEL;
#else
    volatile uint16_t * cfifosel = &R_USB_FS0->CFIFOSEL;
#endif
    uint8_t ep_addr;

    /* Check the data stage direction */
    if (*cfifosel & R_USB_CFIFOSEL_ISEL_WRITE)
    {
        /* IN transfer. */
        ep_addr = USB_EP_CONTROL_IN;
    }
    else
    {
        /* OUT transfer. */
        ep_addr = USB_EP_CONTROL_OUT;
    }

    usbd_event_t event =
    {
        .event_id      = USBD_EVENT_XFER_COMPLETE,
        .xfer_complete =
        {
            .result  = USBD_XFER_RESULT_SUCCESS,
            .ep_addr = ep_addr,
            .len     = 0,
        },
    };

    r_usb_device_call_callback(p_ctrl, &event);
}

/* read CTSQ from INTSTS0 without clearing any interrupt status bits */
static usb_control_stage_t dcp_get_current_stage (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    uint16_t ctsq = (USB_IS_USBHS(p_ctrl->p_cfg->module_number) ?
                     R_USB_HS0->INTSTS0 : R_USB_FS0->INTSTS0) & R_USB_INTSTS0_CTSQ_Msk;
#else
    uint16_t ctsq = R_USB_FS0->INTSTS0 & R_USB_INTSTS0_CTSQ_Msk;
#endif

    switch (ctsq)
    {
        case R_USB_INTSTS0_CTSQ_CTRL_RDATA:
        case R_USB_INTSTS0_CTSQ_CTRL_WDATA:
        {
            return USB_CONTROL_STAGE_DATA;
        }

        case R_USB_INTSTS0_CTSQ_CTRL_RSTATUS:
        case R_USB_INTSTS0_CTSQ_CTRL_WSTATUS:
        case R_USB_INTSTS0_CTSQ_CTRL_WSTATUS_NODATA:
        {
            return USB_CONTROL_STAGE_STATUS;
        }

        case R_USB_INTSTS0_CTSQ_CTRL_ERROR:
        {
            return USB_CONTROL_STAGE_ERROR;
        }

        default:
        {
            return USB_CONTROL_STAGE_IDLE;
        }
    }
}

/* pipe0 transfer */
static inline fsp_err_t process_pipe0_xfer (usbd_instance_ctrl_t * const p_ctrl,
                                            uint8_t                      ep_addr,
                                            void                       * buffer,
                                            uint16_t                     total_bytes)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile uint16_t * cfifosel  = hs_module ? &R_USB_HS0->CFIFOSEL : &R_USB_FS0->CFIFOSEL;
    volatile uint16_t * dcpctr    = hs_module ? &R_USB_HS0->DCPCTR : &R_USB_FS0->DCPCTR;
    volatile uint16_t * cfifoctr  = hs_module ? &R_USB_HS0->CFIFOCTR : &R_USB_FS0->CFIFOCTR;
#else
    volatile uint16_t * cfifosel = &R_USB_FS0->CFIFOSEL;
    volatile uint16_t * dcpctr   = &R_USB_FS0->DCPCTR;
    volatile uint16_t * cfifoctr = &R_USB_FS0->CFIFOCTR;
#endif

    const uint8_t dir = USB_GET_EP_DIR(ep_addr);
    const uint8_t num = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][0];

    /* configure fifo direction and access unit settings */
    if (USB_EP_CONTROL_IN == ep_addr)
    {
        /* IN, 2 bytes */
        *cfifosel = R_USB_CFIFOSEL_ISEL_WRITE | R_USB_FIFOSEL_MBW_16BIT |
                    (BYTE_ORDER == BIG_ENDIAN ? R_USB_FIFOSEL_BIGEND : 0);
        FSP_HARDWARE_REGISTER_WAIT((*cfifosel & R_USB_CFIFOSEL_CURPIPE_Msk), 0);
    }
    else
    {
        /* OUT, a byte */
        *cfifosel = R_USB_FIFOSEL_MBW_8BIT;
        FSP_HARDWARE_REGISTER_WAIT((*cfifosel & R_USB_CFIFOSEL_CURPIPE_Msk), 0);
    }

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num];
    pipe->length    = total_bytes;
    pipe->remaining = total_bytes;

    if (total_bytes > 0)
    {
        pipe->buf = buffer;
        if (USB_EP_CONTROL_IN == ep_addr)
        {
            pipe0_xfer_in(p_ctrl);
        }

        *dcpctr = R_USB_PIPE_CTR_PID_BUF;
    }
    else
    {
        /* ZLP */
        pipe->buf = NULL;

        /*
         * A 0-byte OUT transfer is only started to arm the Status stage ZLP, so it does not
         * depend on CTSQ, which may not have reached the status stage yet.
         */
        if ((USB_EP_CONTROL_OUT == ep_addr) || (USB_CONTROL_STAGE_STATUS == dcp_get_current_stage(p_ctrl)))
        {
            /* status stage: hardware will exchange the ZLP handshake automatically */
            *dcpctr = R_USB_DCPCTR_CCPL_Msk | R_USB_PIPE_CTR_PID_BUF;
        }
        else
        {
            /* data stage ended on a packet boundary: send ZLP to signal end of data */
            *cfifoctr |= R_USB_CFIFOCTR_BVAL_Msk;
        }
    }

    return FSP_SUCCESS;
}

/* common pipe transfer */
static inline fsp_err_t process_pipe_xfer (usbd_instance_ctrl_t * const p_ctrl,
                                           uint8_t                      ep_addr,
                                           void                       * buffer,
                                           uint16_t                     total_bytes)
{
#ifdef USB_HIGH_SPEED_MODULE
    bool                hs_module = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    volatile uint16_t * d0fifosel = hs_module ? &R_USB_HS0->D0FIFOSEL : &R_USB_FS0->D0FIFOSEL;
    volatile uint16_t * d0fifoctr = hs_module ? &R_USB_HS0->D0FIFOCTR : &R_USB_FS0->D0FIFOCTR;
#else
    volatile uint16_t * d0fifosel = &R_USB_FS0->D0FIFOSEL;
    volatile uint16_t * d0fifoctr = &R_USB_FS0->D0FIFOCTR;
#endif
    const uint8_t epn = USB_GET_EP_IDX(ep_addr);
    const uint8_t dir = USB_GET_EP_DIR(ep_addr);
    const uint8_t num = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn];

    if (0 == num)
    {
        return FSP_ERR_USB_NOT_OPEN;
    }

    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num];
    pipe->buf       = buffer;
    pipe->length    = total_bytes;
    pipe->remaining = total_bytes;

    if (USB_EP_IN_DIR == dir)
    {
        /* IN */
        if (total_bytes > 0)
        {
            pipe_xfer_in(p_ctrl, num);
        }
        else
        {
            /* ZLP */
            *d0fifosel = num;
            FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk), num);

            /* Trigger send zlp */
            if(*d0fifoctr & R_USB_D0FIFOCTR_FRDY_Msk) {
                *d0fifoctr = R_USB_D0FIFOCTR_BCLR_Msk | R_USB_D0FIFOCTR_BVAL_Msk;
            }

            *d0fifosel = 0;
            FSP_HARDWARE_REGISTER_WAIT((*d0fifosel & R_USB_D0FIFOSEL_CURPIPE_Msk), 0);
        }
    }
    else
    {
        /* OUT */
        volatile R_USB_PIPE_TR_t * pt = get_pipetre(p_ctrl, num);

        if (NULL != pt)
        {
            const uint16_t      mps     = edpt_max_packet_size(p_ctrl, num);
            volatile uint16_t * pipectr = get_pipectr(p_ctrl, num);

            if (*pipectr & R_USB_PIPE_CTR_PID_Msk)
            {
                *pipectr = R_USB_PIPE_CTR_PID_NAK;
            }

            pt->E_b.TRCLR = 1;
            pt->N         = (total_bytes + mps - 1) / mps;
            pt->E_b.TRENB = 1;
            *pipectr      = R_USB_PIPE_CTR_PID_BUF;
        }
    }

    return FSP_SUCCESS;
}

/* pipe0 BEMP event handler */
static inline void process_pipe0_bemp (usbd_instance_ctrl_t * const p_ctrl)
{
    bool completed = pipe0_xfer_in(p_ctrl);

    if (!completed)
    {
        return;
    }

    pipe_state_t * pipe  = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[0];
    usbd_event_t   event =
    {
        .event_id      = USBD_EVENT_XFER_COMPLETE,
        .xfer_complete =
        {
            .result  = USBD_XFER_RESULT_SUCCESS,
            .ep_addr = USB_EP_CONTROL_IN,
            .len     = pipe->length,
        },
    };
    r_usb_device_call_callback(p_ctrl, &event);
}

/* BRDY event hanlder  */
static inline void process_pipe_brdy (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    pipe_state_t * pipe = &g_pipe_cfg[p_ctrl->p_cfg->module_number].pipe[num];
    const uint32_t dir  = USB_GET_EP_DIR(pipe->ep);
    bool           completed;

    if (USB_EP_IN_DIR == dir)
    {
        completed = pipe_xfer_in(p_ctrl, num);
    }
    else
    {
        /* OUT */
        if (num)
        {
            completed = pipe_xfer_out(p_ctrl, num);
        }
        else
        {
            completed = pipe0_xfer_out(p_ctrl);
        }
    }

    if (completed)
    {
        usbd_event_t event =
        {
            .event_id      = USBD_EVENT_XFER_COMPLETE,
            .xfer_complete =
            {
                .result  = USBD_XFER_RESULT_SUCCESS,
                .ep_addr = pipe->ep,
                .len     = pipe->length - pipe->remaining,
            },
        };
        r_usb_device_call_callback(p_ctrl, &event);
    }
}

static inline usbd_speed_t usbfs_module_bus_reset (void)
{
    usbd_speed_t speed;

    R_USB_FS0->BEMPENB  = 1;
    R_USB_FS0->BRDYENB  = 1;
    R_USB_FS0->CFIFOCTR = R_USB_CFIFOCTR_BCLR_Msk;

    R_USB_FS0->D0FIFOSEL = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT(R_USB_FS0->D0FIFOSEL_b.CURPIPE, 0);

    R_USB_FS0->D1FIFOSEL = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT(R_USB_FS0->D0FIFOSEL_b.CURPIPE, 0);

    volatile uint16_t * pipectr = (volatile uint16_t *) &R_USB_FS0->PIPE_CTR[0];
    volatile uint16_t * pipetre = (volatile uint16_t *) &R_USB_FS0->PIPE_TR[0].E;

    for (int i = 1; i <= 5; ++i)
    {
        R_USB_FS0->PIPESEL = i;
        R_USB_FS0->PIPECFG = 0;
        *pipectr           = R_USB_PIPE_CTR_ACLRM_Msk;
        *pipectr           = 0;
        ++pipectr;
        *pipetre = (1 << 8);
        pipetre += 2;
    }

    for (int i = 6; i <= 9; ++i)
    {
        R_USB_FS0->PIPESEL = i;
        R_USB_FS0->PIPECFG = 0;
        *pipectr           = R_USB_PIPE_CTR_ACLRM_Msk;
        *pipectr           = 0;
        ++pipectr;
    }

    switch (R_USB_FS0->DVSTCTR0 & R_USB_DVSTCTR0_RHST_Msk)
    {
        case (1 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_LS;
            break;
        }

        case (2 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_FS;
            break;
        }

        case (3 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_HS;
            break;
        }

        default:
        {
            speed = USBD_SPEED_INVALID;
            break;
        }
    }

    return speed;
}

#ifdef USB_HIGH_SPEED_MODULE
static inline usbd_speed_t usbhs_module_bus_reset (void)
{
    usbd_speed_t speed;

    R_USB_HS0->BEMPENB  = 1;
    R_USB_HS0->BRDYENB  = 1;
    R_USB_HS0->CFIFOCTR = R_USB_CFIFOCTR_BCLR_Msk;

    R_USB_HS0->D0FIFOSEL = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->D0FIFOSEL_b.CURPIPE, 0);

    R_USB_HS0->D1FIFOSEL = 0;

    /* if CURPIPE bits changes, check written value */
    FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->D0FIFOSEL_b.CURPIPE, 0);

    volatile uint16_t * pipectr = (volatile uint16_t *) ((uintptr_t) (&R_USB_HS0->PIPE_CTR[0]));
    volatile uint16_t * pipetre = (volatile uint16_t *) ((uintptr_t) (&R_USB_HS0->PIPE_TR[0].E));

    for (int i = 1; i <= 5; ++i)
    {
        R_USB_HS0->PIPESEL = i;
        R_USB_HS0->PIPECFG = 0;
        *pipectr           = R_USB_PIPE_CTR_ACLRM_Msk;
        *pipectr           = 0;
        ++pipectr;
        *pipetre = (1 << 8);
        pipetre += 2;
    }

    for (int i = 6; i <= 9; ++i)
    {
        R_USB_HS0->PIPESEL = i;
        R_USB_HS0->PIPECFG = 0;
        *pipectr           = R_USB_PIPE_CTR_ACLRM_Msk;
        *pipectr           = 0;
        ++pipectr;
    }

    switch (R_USB_HS0->DVSTCTR0 & R_USB_DVSTCTR0_RHST_Msk)
    {
        case (1 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_LS;
            break;
        }

        case (2 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_FS;
            break;
        }

        case (3 << R_USB_DVSTCTR0_RHST_Pos):
        {
            speed = USBD_SPEED_HS;
            break;
        }

        default:
        {
            speed = USBD_SPEED_INVALID;
            break;
        }
    }

    return speed;
}

#endif

/* BUS_RESET event handler */
static inline void process_bus_reset (usbd_instance_ctrl_t * const p_ctrl)
{
    usbd_speed_t speed = USBD_SPEED_INVALID;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        speed = usbhs_module_bus_reset();
    }
    else
#endif
    {
        speed = usbfs_module_bus_reset();
    }

    usbd_event_t event =
    {
        .event_id        = USBD_EVENT_BUS_RESET,
        .bus_reset.speed = speed,
    };

    r_usb_device_call_callback(p_ctrl, &event);
}

/* SET_ADDRESS request packet received event handler */
static inline void process_set_address (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    const bool     is_usbhs = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    const uint16_t addr     = (is_usbhs ? R_USB_HS0->USBADDR : R_USB_FS0->USBADDR) & 0x7fUL;
#else
    const uint16_t addr = R_USB_FS0->USBADDR & 0x7fUL;
#endif

    if (0 == addr)
    {
        return;
    }

    usbd_event_t event =
    {
        .event_id       = USBD_EVENT_SETUP_RECEIVED,
        .setup_received =
        {
            .request_type   = USB_REQUEST_TYPE_SET_ADDRESS,
            .request_value  = addr,
            .request_index  = 0,
            .request_length = 0,
        },
    };

    r_usb_device_call_callback(p_ctrl, &event);
}

/* configure for usb physical layer */
static inline void physical_init (usbd_instance_ctrl_t * const p_ctrl, usbd_cfg_t const * const p_cfg)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_cfg->module_number))
    {
        /* Disable D+/D- lines pulldown when in function controller mode */
        R_USB_HS0->SYSCFG_b.DRPD = 0;

        /* Set function controller operation (device) */
        R_USB_HS0->SYSCFG_b.DCFM = 0;

        /* High-speed selection */
        R_USB_HS0->SYSCFG_b.HSE = (p_cfg->usb_speed == USBD_SPEED_HS) ? 1 : 0;

        /* PLLRESET bit is 1 after a reset,
         * changing the setting after release from the reset state is not required */
        /* R_USB_HS0->PHYSET_b.PLLRESET = 1; */

        /* If clock source of PHYCLK is not XTAL, enable Classic-only mode */
        R_USB_HS0->PHYSET_b.HSEB = (USBHS_PHY_CLOCK_SOURCE_IS_XTAL == 0) ? 1 : 0;

#if USBHS_PHY_CLOCK_SOURCE_IS_XTAL
        /* Setting the PHY clock */
        R_USB_HS0->PHYSET_b.CLKSEL = USBD_CFG_PHYSET_CLKSEL;
#endif
        /* After set those bit, wait 1 microsecond */
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);

        /* Power on PHY by set DIRPD bit to 0 */
        R_USB_HS0->PHYSET_b.DIRPD = 0;

        /* Wait for PHY power on */
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MILLISECONDS);

        /* Enable the automatic adjustment function for the terminating resistance */
        R_USB_HS0->PHYSET_b.REPSEL = 1;

        /* Start the PLL */
        R_USB_HS0->PHYSET_b.PLLRESET = 0;

        /* Supply the PHY clock to operate function controller */
        R_USB_HS0->LPSTS_b.SUSPENDM = 1;

        /* Wait for the PLL to lock */
        FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->PLLSTA_b.PLLLOCK, 1);

        /* Set the Level output for Interrupt Signal Output Method */
        R_USB_HS0->SOFCFG_b.INTL = 1;

        /* Enable USB operation */
        R_USB_HS0->SYSCFG_b.USBE = 1;

        uint32_t pclka_freq = R_FSP_SystemClockHzGet(FSP_PRIV_CLOCK_PCLKA);

        /* Calculate the minimum access cycles and make sure it satisfy the time >= 41ns */
        uint8_t access_cycles = ((USB_MINIMUM_ACCESS_CYCLE_TIME * pclka_freq)
                                / SECOND_TO_NANOSECONDS) + 1;

        /* Set CPU bus wait time */
        R_USB_HS0->BUSWAIT_b.BWAIT = access_cycles - USB_BUS_WAIT_OFFSET;

        /* Set CNEN bit depend on status of VBUS */
        if (R_USB_HS0->INTSTS0_b.VBSTS == 1) {
            R_USB_HS0->SYSCFG_b.CNEN = 1;
        }
    }
    else
#endif
    {
        /* Enable clock supply to USBFS */
        R_USB_FS0->SYSCFG_b.SCKE = 1;
        FSP_HARDWARE_REGISTER_WAIT(R_USB_FS0->SYSCFG_b.SCKE, 1);

        /* Disable line pull-down */
        R_USB_FS0->SYSCFG_b.DRPD = 0;

        /* Disable D+ line pull-up */
        R_USB_FS0->SYSCFG_b.DPRPU = 0;

        /* Select controller as device controller */
        R_USB_FS0->SYSCFG_b.DCFM = 0;

        /* Enable module operation */
        R_USB_FS0->SYSCFG_b.USBE = 1;
    }
}

/* setting default pipe0 to ready for enumerations */
static inline void control_pipe_setup (usbd_instance_ctrl_t * const p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->DCPMAXP_b.MXPS = 64;
        R_USB_HS0->BEMPENB        = 1;
        R_USB_HS0->BRDYENB        = 1;
    }
    else
#endif
    {
        R_USB_FS0->DCPMAXP_b.MXPS = 64;
        R_USB_FS0->BEMPENB        = 1;
        R_USB_FS0->BRDYENB        = 1;
    }
}

static inline void usb_interrupt_configure (usbd_instance_ctrl_t * p_ctrl, usbd_cfg_t const * p_cfg)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_cfg->module_number))
    {
        /* clear active bits except VALID (don't write 0 to already cleared bits according to the HW manual) */
        R_USB_HS0->INTSTS0 = ~((R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_SOFR_Msk |
                                R_USB_INTSTS0_RESM_Msk | R_USB_INTSTS0_VBINT_Msk) & R_USB_HS0->INTSTS0);
        R_USB_HS0->INTENB0 = R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                             R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_RESM_Msk;

        R_BSP_IrqCfgEnable(p_cfg->hs_irq, p_cfg->hsipl, p_ctrl);
    }
    else
#endif
    {
        R_USB_FS0->INTSTS0 = ~((R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_SOFR_Msk |
                                R_USB_INTSTS0_RESM_Msk | R_USB_INTSTS0_VBINT_Msk) & R_USB_FS0->INTSTS0);
        R_USB_FS0->INTENB0 = R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                             R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_RESM_Msk;

        R_BSP_IrqCfgEnable(p_cfg->irq, p_cfg->ipl, p_ctrl);
        R_BSP_IrqCfgEnable(p_cfg->irq_r, p_cfg->ipl_r, p_ctrl);
    }
}

static void r_usb_device_call_callback (usbd_instance_ctrl_t * p_ctrl, usbd_event_t * event)
{
    usbd_callback_arg_t   args;
    usbd_callback_arg_t * p_args = p_ctrl->p_callback_memory;

    if (NULL == p_ctrl->p_callback)
    {
        return;
    }

    if (NULL == p_args)
    {
        /* Store on stack */
        p_args = &args;
    }
    else
    {
        /* Save current arguments on the stack in case this is a nested interrupt. */
        args = *p_args;
    }

    p_args->module_number = p_ctrl->p_cfg->module_number;
    p_args->p_context     = p_ctrl->p_context;
    memcpy(&p_args->event, event, sizeof(usbd_event_t));

    p_ctrl->p_callback(p_args);

    if (NULL != p_ctrl->p_callback_memory)
    {
        /* Restore callback memory in case this is a nested interrupt. */
        *p_ctrl->p_callback_memory = args;
    }
}

static inline void usb_module_register_clear (uint8_t usb_ip)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(usb_ip))
    {
        R_USB_HS0->DVSTCTR0    = 0;
        R_USB_HS0->DCPCTR      = R_USB_PIPE_CTR_SQSET_Msk;
        R_USB_HS0->PIPE_CTR[0] = 0;
        R_USB_HS0->PIPE_CTR[1] = 0;
        R_USB_HS0->PIPE_CTR[2] = 0;
        R_USB_HS0->PIPE_CTR[3] = 0;
        R_USB_HS0->PIPE_CTR[4] = 0;
        R_USB_HS0->PIPE_CTR[5] = 0;
        R_USB_HS0->PIPE_CTR[6] = 0;
        R_USB_HS0->PIPE_CTR[7] = 0;
        R_USB_HS0->PIPE_CTR[8] = 0;
        R_USB_HS0->BRDYENB     = 0;
        R_USB_HS0->NRDYENB     = 0;
        R_USB_HS0->BEMPENB     = 0;
        R_USB_HS0->INTENB0     = 0;
        R_USB_HS0->INTENB1     = 0;
        R_USB_HS0->BRDYSTS     = 0;
        R_USB_HS0->NRDYSTS     = 0;
        R_USB_HS0->BEMPSTS     = 0;
        R_USB_HS0->SYSCFG     &= (~R_USB_SYSCFG_DPRPU_Msk);
        R_USB_HS0->SYSCFG     &= (~R_USB_SYSCFG_USBE_Msk);
        R_USB_HS0->SYSCFG     &= (~R_USB_SYSCFG_DRPD_Msk);
        R_USB_HS0->SYSCFG     &= (~R_USB_SYSCFG_DCFM_Msk);
        R_USB_HS0->LPSTS       = 0;
    }
    else
#endif
    {
        R_USB_FS0->DVSTCTR0    = 0;
        R_USB_FS0->DCPCTR      = R_USB_PIPE_CTR_SQSET_Msk;
        R_USB_FS0->PIPE_CTR[0] = 0;
        R_USB_FS0->PIPE_CTR[1] = 0;
        R_USB_FS0->PIPE_CTR[2] = 0;
        R_USB_FS0->PIPE_CTR[3] = 0;
        R_USB_FS0->PIPE_CTR[4] = 0;
        R_USB_FS0->PIPE_CTR[5] = 0;
        R_USB_FS0->PIPE_CTR[6] = 0;
        R_USB_FS0->PIPE_CTR[7] = 0;
        R_USB_FS0->PIPE_CTR[8] = 0;
        R_USB_FS0->BRDYENB     = 0;
        R_USB_FS0->NRDYENB     = 0;
        R_USB_FS0->BEMPENB     = 0;
        R_USB_FS0->INTENB0     = 0;
        R_USB_FS0->INTENB1     = 0;
        R_USB_FS0->BRDYSTS     = 0;
        R_USB_FS0->NRDYSTS     = 0;
        R_USB_FS0->BEMPSTS     = 0;
        R_USB_FS0->SYSCFG     &= (~R_USB_SYSCFG_DPRPU_Msk);
        R_USB_FS0->SYSCFG     &= (~R_USB_SYSCFG_DRPD_Msk);
        R_USB_FS0->SYSCFG     &= (~R_USB_SYSCFG_DCFM_Msk);
        R_USB_FS0->SYSCFG     &= (~R_USB_SYSCFG_USBE_Msk);
    }

    R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
}

static inline void usb_module_start (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_BSP_MODULE_START(FSP_IP_USBHS, 0);
    }
    else
#endif
    {
        R_BSP_MODULE_START(FSP_IP_USBFS, 0);
    }
}

static inline void usb_module_stop (usbd_instance_ctrl_t * p_ctrl)
{
    usb_module_register_clear(p_ctrl->p_cfg->module_number);

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_BSP_MODULE_STOP(FSP_IP_USBHS, 0);
    }
    else
#endif
    {
        R_BSP_MODULE_STOP(FSP_IP_USBFS, 0);
    }
}

static inline void usb_disable_interrupt (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->INTENB0 &= ~(R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                                R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk |
                                R_USB_INTSTS0_RESM_Msk);

        R_BSP_IrqDisable(p_ctrl->p_cfg->hs_irq);
    }
    else
#endif
    {
        R_USB_FS0->INTENB0 &= ~(R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                                R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk |
                                R_USB_INTSTS0_RESM_Msk);

        R_BSP_IrqDisable(p_ctrl->p_cfg->irq);
        R_BSP_IrqDisable(p_ctrl->p_cfg->irq_r);
    }
}

static inline void usb_enable_interrupt (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_BSP_IrqEnable(p_ctrl->p_cfg->hs_irq);

        R_USB_HS0->INTENB0 = R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                             R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_RESM_Msk;
    }
    else
#endif
    {
        R_BSP_IrqEnable(p_ctrl->p_cfg->irq);
        R_BSP_IrqEnable(p_ctrl->p_cfg->irq_r);

        R_USB_FS0->INTENB0 = R_USB_INTSTS0_VBINT_Msk | R_USB_INTSTS0_BRDY_Msk | R_USB_INTSTS0_BEMP_Msk |
                             R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_RESM_Msk;
    }
}

static inline void process_pipe_forced_termination (usbd_instance_ctrl_t * const p_ctrl, uint8_t ep)
{
    const uint8_t epn = USB_GET_EP_IDX(ep);
    const uint8_t dir = USB_GET_EP_DIR(ep);
    const uint8_t num = g_pipe_cfg[p_ctrl->p_cfg->module_number].ep[USB_PIPECFG_DIR_IDX(dir)][epn];

    if (0 == num)
    {
        /* This endpoint is not yet opened */
        return;
    }

    volatile uint16_t * pt      = get_pipetre(p_ctrl, num);
    volatile uint16_t * pipectr = get_pipectr(p_ctrl, num);

    /* Set NAK */
    if (0 != (*pipectr & R_USB_PIPE_CTR_PID_Msk))
    {
        *pipectr = R_USB_PIPE_CTR_PID_NAK;
    }

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        /* Disable pipe interrupt */
        R_USB_HS0->BRDYENB &= (uint16_t) ~(1 << num);
        R_USB_HS0->NRDYENB &= (uint16_t) ~(1 << num);
        R_USB_HS0->BEMPENB &= (uint16_t) ~(1 << num);

        /* Clear transaction counter */
        *pt &= ~R_USB_PIPE_TR_E_TRENB_Msk;
        *pt |= R_USB_PIPE_TR_E_TRCLR_Msk;

        /* Clear FIFO port */
        if (num == R_USB_HS0->D0FIFOSEL_b.CURPIPE)
        {
            R_USB_HS0->D0FIFOSEL_b.CURPIPE = 0;

            /* if CURPIPE bits changes, check written value */
            FSP_HARDWARE_REGISTER_WAIT(R_USB_HS0->D0FIFOSEL_b.CURPIPE, 0);
        }
    }
    else
#endif
    {
        R_USB_FS0->BRDYENB &= (uint16_t) ~(1 << num);
        R_USB_FS0->NRDYENB &= (uint16_t) ~(1 << num);
        R_USB_FS0->BEMPENB &= (uint16_t) ~(1 << num);

        /* Clear transaction counter */
        *pt &= ~R_USB_PIPE_TR_E_TRENB_Msk;
        *pt |= R_USB_PIPE_TR_E_TRCLR_Msk;

        /* Clear FIFO port */
        if (num == R_USB_FS0->D0FIFOSEL_b.CURPIPE)
        {
            R_USB_FS0->D0FIFOSEL_b.CURPIPE = 0;
        }
    }

    /* Buffer clear */
    *pipectr |= R_USB_PIPE_CTR_ACLRM_Msk;
    *pipectr &= ~R_USB_PIPE_CTR_ACLRM_Msk;

    /* SPLIT Buffer clear */
    *pipectr &= R_USB_PIPE_CTR_CSCLR_Msk;
}

static inline void enable_pipe_information_change (usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    volatile uint16_t * ctr = get_pipectr(p_ctrl, num);

    /* Set PID to 0b00: NAK response */
    *ctr &= (~R_USB_PIPE_CTR_PID_Msk);

    /* Wait for pipe to be ready */
    FSP_HARDWARE_REGISTER_WAIT((*ctr & R_USB_PIPE_CTR_PBUSY_Msk), 0);
}

#ifdef USB_HIGH_SPEED_MODULE
/**
 * Only USBHS module need this function
 * This function allocates buffer 1-5 with size 2KB
 * Buffer 0 and buffer 6-9 has fixed size and location when in used so they are not allocated here
 * Because of the limit in the size, only 2 of them can be allocated at the same time
 * TODO: The size of buffer should be configurable in order to allocate more buffers
 */
static inline fsp_err_t pipebuffer_allocate(usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    if (!USB_IS_USBHS(p_ctrl->p_cfg->module_number)) {
        return FSP_ERR_UNSUPPORTED;
    }

    if (num > 9) {
        /* Only buffers 0-9 are available */
        return FSP_ERR_INVALID_ARGUMENT;
    }

    if (num > 5 || num == 0) {
        /* Only buffers 1-5 need to be allocated */
        return FSP_SUCCESS;
    }

    if (g_pipe_allocation_status[num - 1] != 0) {
        /* Pipebuff is already allocated */
        return FSP_SUCCESS;
    }

    if (g_fifo_buffer_usage > 3) {
        /* Only 2 bits in use */
        return FSP_ERR_INVALID_STATE;
    }

    if (g_fifo_buffer_usage == 3) {
        /* 2 buffers are already allocated (no more space) */
        return FSP_ERR_INSUFFICIENT_SPACE;
    } else if (g_fifo_buffer_usage == 1) {
        /* The first half is taken, so allocated to second half */
        /* 0x48: index of the start block of second half */
        /* 0x1F: set buff size to 2KB */
        R_USB_HS0->PIPEBUF = (R_USB_PIPEBUF_SECOND_HALF_START << R_USB_PIPEBUF_BUFNMB_Pos) |
                            (0x1F << R_USB_PIPEBUF_BUFSIZE_Pos);

        /* Set second bit to indicate that second half is used */
        g_fifo_buffer_usage |= 0x02;
        g_pipe_allocation_status[num - 1] = 2;

    } else {
        /* Allocate to first half */
        /* 0x08: index of the start block of first half */
        /* 0x1F: set buff size to 2KB */
        R_USB_HS0->PIPEBUF = (R_USB_PIPEBUF_FIRST_HALF_START << R_USB_PIPEBUF_BUFNMB_Pos) |
                            (0x1F << R_USB_PIPEBUF_BUFSIZE_Pos);

        /* Set first bit to indicate that first half is used */
        g_fifo_buffer_usage |= 0x01;
        g_pipe_allocation_status[num - 1] = 1;
    }

    return FSP_SUCCESS;
}

/* Dealloc allocated pipe buffers */
static inline fsp_err_t pipebuffer_deallocate(usbd_instance_ctrl_t * const p_ctrl, uint32_t num)
{
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number)) {
        return FSP_ERR_UNSUPPORTED;
    }

    if (num > 9) {
        /* Only buffers 0-9 are available */
        return FSP_ERR_INVALID_ARGUMENT;
    }

    if (num > 5 || num == 0) {
        /* Buffer other than 1 - 5 have fixed size and location */
        return FSP_SUCCESS;
    }

    if (g_pipe_allocation_status[num - 1] == 0) {
        /* Pipebuff is not allocated */
        return FSP_SUCCESS;
    }

    if (R_USB_HS0->PIPEBUF_b.BUFNMB == R_USB_PIPEBUF_FIRST_HALF_START) {
        /* Clear first bit to indicate that first half is free */
        g_fifo_buffer_usage &= ~0x01;
    } else if (R_USB_HS0->PIPEBUF_b.BUFNMB == R_USB_PIPEBUF_SECOND_HALF_START) {
        /* Clear second bit to indicate that second half is free */
        g_fifo_buffer_usage &= ~0x02;
    } else {
        /* Buffer not allocated */
    }

    g_pipe_allocation_status[num - 1] = 0;
    return FSP_SUCCESS;
}
#endif

static inline uint16_t get_active_bit_intsts0 (usbd_instance_ctrl_t * p_ctrl)
{
    uint16_t intsts0 = 0;

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        intsts0 = R_USB_HS0->INTSTS0;

        /* clear active bits except VALID (don't write 0 to already cleared bits according to the HW manual) */
        R_USB_HS0->INTSTS0 = ~((R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_SOFR_Msk |
                                R_USB_INTSTS0_RESM_Msk | R_USB_INTSTS0_VBINT_Msk) & intsts0);
    }
    else
#endif
    {
        intsts0 = R_USB_FS0->INTSTS0;

        /* clear active bits except VALID (don't write 0 to already cleared bits according to the HW manual) */
        R_USB_FS0->INTSTS0 = ~((R_USB_INTSTS0_CTRT_Msk | R_USB_INTSTS0_DVST_Msk | R_USB_INTSTS0_SOFR_Msk |
                                R_USB_INTSTS0_RESM_Msk | R_USB_INTSTS0_VBINT_Msk) & intsts0);
    }

    return intsts0;
}

/* This function is to avoid flickering */
static bool check_vbus_stable (usbd_instance_ctrl_t * p_ctrl)
{
    /* Read the VBSTS flag at least 3 times and check if the values are the same */
#ifdef USB_HIGH_SPEED_MODULE
    volatile uint16_t *intsts0 = USB_IS_USBHS(p_ctrl->p_cfg->module_number) ?
                                &R_USB_HS0->INTSTS0 : &R_USB_FS0->INTSTS0;
#else
    volatile uint16_t *intsts0 = &R_USB_FS0->INTSTS0;
#endif

    uint16_t vbus1 = *intsts0 & R_USB_INTSTS0_VBSTS_Msk;
    uint16_t vbus2 = *intsts0 & R_USB_INTSTS0_VBSTS_Msk;
    uint16_t vbus3 = *intsts0 & R_USB_INTSTS0_VBSTS_Msk;

    return (vbus1 == vbus2) && (vbus2 == vbus3);
}

static inline void process_vbus_changed (usbd_instance_ctrl_t * p_ctrl, uint16_t intsts0)
{
    usbd_event_t event;

    if (!check_vbus_stable(p_ctrl))
    {
        /* VBUS state is not stable, do not process the event */
        return;
    }

    if (intsts0 & R_USB_INTSTS0_VBSTS_Msk)
    {
#ifdef USB_HIGH_SPEED_MODULE
        if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
        {
            /* According to HWUM set this bit to 1 whenever VBUS is detected */
            R_USB_HS0->SYSCFG_b.CNEN = 1;
        }
#endif
        event.event_id = USBD_EVENT_VBUS_RDY;
    }
    else
    {
#ifdef USB_HIGH_SPEED_MODULE
        if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
        {
            /* According to HWUM set this bit to 0 whenever VBUS is removed */
            R_USB_HS0->SYSCFG_b.CNEN = 0;
        }
#endif
        event.event_id = USBD_EVENT_VBUS_REMOVED;
    }

    r_usb_device_call_callback(p_ctrl, &event);
}

static inline void process_resumed_event (usbd_instance_ctrl_t * p_ctrl)
{
    usbd_event_t event =
    {
        .event_id = USBD_EVENT_RESUME,
    };

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->INTENB0_b.SOFE = 0;
    }
    else
#endif
    {
        R_USB_FS0->INTENB0_b.SOFE = 0;
    }

    r_usb_device_call_callback(p_ctrl, &event);
}

static inline void process_sof_event (usbd_instance_ctrl_t * p_ctrl)
{
    usbd_event_t event =
    {
        .event_id = USBD_EVENT_SOF,
    };

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        if (R_USB_HS0->INTENB0_b.SOFE)
        {
            R_USB_HS0->INTENB0_b.SOFE = 0;
        }
        else
        {
            return;
        }
    }
    else
#endif
    {
        if (R_USB_FS0->INTENB0_b.SOFE)
        {
            R_USB_FS0->INTENB0_b.SOFE = 0;
        }
        else
        {
            return;
        }
    }

    r_usb_device_call_callback(p_ctrl, &event);
}

static inline void process_suppend_event (usbd_instance_ctrl_t * p_ctrl)
{
    usbd_event_t event =
    {
        .event_id = USBD_EVENT_SUSPEND,
    };

#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->INTENB0_b.SOFE = 1;
    }
    else
#endif
    {
        R_USB_FS0->INTENB0_b.SOFE = 1;
    }

    r_usb_device_call_callback(p_ctrl, &event);
}

static inline void process_nrdy_event (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    if (USB_IS_USBHS(p_ctrl->p_cfg->module_number))
    {
        R_USB_HS0->NRDYSTS = 0;
    }
    else
#endif
    {
        R_USB_FS0->NRDYSTS = 0;
    }
}

static inline void process_bemp_event (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    const bool     is_usbhs = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    const uint16_t bempsts  = is_usbhs ? R_USB_HS0->BEMPSTS : R_USB_FS0->BEMPSTS;
#else
    const uint16_t bempsts = R_USB_FS0->BEMPSTS;
#endif

#ifdef USB_HIGH_SPEED_MODULE
    if (is_usbhs)
    {
        R_USB_HS0->BEMPSTS = 0;
    }
    else
#endif
    {
        R_USB_FS0->BEMPSTS = 0;
    }

    if ((bempsts & 0x1U) != 0)
    {
        process_pipe0_bemp(p_ctrl);
    }
}

static inline void process_brdy_event (usbd_instance_ctrl_t * p_ctrl)
{
#ifdef USB_HIGH_SPEED_MODULE
    const bool     is_usbhs = USB_IS_USBHS(p_ctrl->p_cfg->module_number);
    const uint16_t brdyenb  = is_usbhs ? R_USB_HS0->BRDYENB : R_USB_FS0->BRDYENB;
    uint16_t       brdysts  = (is_usbhs ? R_USB_HS0->BRDYSTS : R_USB_FS0->BRDYSTS) & brdyenb;
#else
    const uint16_t brdyenb = R_USB_FS0->BRDYENB;
    uint16_t       brdysts = R_USB_FS0->BRDYSTS & brdyenb;
#endif

    /* clear active bits (don't write 0 to already cleared bits according to the HW manual) */
#ifdef USB_HIGH_SPEED_MODULE
    if (is_usbhs)
    {
        R_USB_HS0->BRDYSTS = ~brdysts;
    }
    else
#endif
    {
        R_USB_FS0->BRDYSTS = ~brdysts;
    }

    while (brdysts)
    {
        const uint32_t num = get_first_bit1_offset(brdysts);
        process_pipe_brdy(p_ctrl, num);
        brdysts &= ~(1 << num);
    }
}

/***********************************************************************************************************************
 * USB device isr
 **********************************************************************************************************************/
void usb_device_isr (void)
{
    /* Save context if RTOS is used */
    FSP_CONTEXT_SAVE

    IRQn_Type irq = R_FSP_CurrentIrqGet();

    usbd_instance_ctrl_t * p_ctrl = (usbd_instance_ctrl_t *) R_FSP_IsrContextGet(irq);

    uint16_t intsts0 = get_active_bit_intsts0(p_ctrl);

    /* Clear pending IRQ immediately after reading status so that any new interrupt
     * arriving during this ISR's execution re-pends at the ICU level and is not lost. */
    R_BSP_IrqStatusClear(irq);

    /* VBUS changes */
    if (intsts0 & R_USB_INTSTS0_VBINT_Msk)
    {
        process_vbus_changed(p_ctrl, intsts0);
    }

    /* Resumed */
    if (intsts0 & R_USB_INTSTS0_RESM_Msk)
    {
        process_resumed_event(p_ctrl);
    }

    /* SOF received */
    if (intsts0 & R_USB_INTSTS0_SOFR_Msk)
    {
        process_sof_event(p_ctrl);
    }

    /* Device state changes */
    if (intsts0 & R_USB_INTSTS0_DVST_Msk)
    {
        switch (intsts0 & R_USB_INTSTS0_DVSQ_Msk)
        {
            case R_USB_INTSTS0_DVSQ_STATE_DEF:
            {
                process_bus_reset(p_ctrl);
                break;
            }

            case R_USB_INTSTS0_DVSQ_STATE_ADDR:
            {
                process_set_address(p_ctrl);
                break;
            }

            case R_USB_INTSTS0_DVSQ_STATE_SUSP0:
            case R_USB_INTSTS0_DVSQ_STATE_SUSP1:
            case R_USB_INTSTS0_DVSQ_STATE_SUSP2:
            case R_USB_INTSTS0_DVSQ_STATE_SUSP3:
            {
                process_suppend_event(p_ctrl);
                break;
            }

            default:
            {
                break;
            }
        }
    }

    if (intsts0 & R_USB_INTSTS0_NRDY_Msk)
    {
        process_nrdy_event(p_ctrl);
    }

    /* Control transfer stage changes */
    if (intsts0 & R_USB_INTSTS0_CTRT_Msk)
    {
        switch (intsts0 & R_USB_INTSTS0_CTSQ_Msk)
        {
            case R_USB_INTSTS0_CTSQ_CTRL_RDATA:
            case R_USB_INTSTS0_CTSQ_CTRL_WDATA:
            case R_USB_INTSTS0_CTSQ_CTRL_WSTATUS_NODATA:
            {
                /* A setup packet has been received. */
                process_setup_packet(p_ctrl);
                break;
            }

            case R_USB_INTSTS0_CTSQ_CTRL_IDLE:
            {
                /* A ZLP has been sent/received — status stage complete. */
                process_status_completion(p_ctrl);
                break;
            }

            case R_USB_INTSTS0_CTSQ_CTRL_RSTATUS:
            case R_USB_INTSTS0_CTSQ_CTRL_WSTATUS:
            {
                /* Complete the status stage here so it cannot race with the software ZLP arm. */
#ifdef USB_HIGH_SPEED_MODULE
                volatile uint16_t * dcpctr =
                    USB_IS_USBHS(p_ctrl->p_cfg->module_number) ? &R_USB_HS0->DCPCTR : &R_USB_FS0->DCPCTR;
#else
                volatile uint16_t * dcpctr = &R_USB_FS0->DCPCTR;
#endif
                *dcpctr = R_USB_DCPCTR_CCPL_Msk | R_USB_PIPE_CTR_PID_BUF;
                break;
            }

            default:
            {
                break;
            }
        }
    }

    /* Buffer empty */
    if (intsts0 & R_USB_INTSTS0_BEMP_Msk)
    {
        process_bemp_event(p_ctrl);
    }

    /* Buffer ready */
    if (intsts0 & R_USB_INTSTS0_BRDY_Msk)
    {
        process_brdy_event(p_ctrl);
    }

    /* Restore context if RTOS is used */
    FSP_CONTEXT_RESTORE
}

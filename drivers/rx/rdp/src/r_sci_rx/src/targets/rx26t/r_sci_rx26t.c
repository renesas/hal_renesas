/***********************************************************************************************************************
* Copyright (c) 2023 - 2025 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
***********************************************************************************************************************/
/**********************************************************************************************************************
* File Name    : r_sci_rx26t.c
* Description  : Functions for using SCI on the RX26T device.
***********************************************************************************************************************
* History : DD.MM.YYYY Version Description
*           31.03.2023 1.00    Initial Release.
*           28.06.2024 5.30    Corrected the typecasting formula in sci_init_bit_rate().
*           01.11.2024 5.40    Fixed the issue that data cannot be sent when using the SCI_CMD_TX_Q_FLUSH command
*                              with the R_SCI_Control() function before executing the R_SCI_Send() function.
*           15.03.2025 5.41    Updated disclaimer
***********************************************************************************************************************/

/*****************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "platform.h"

#include "r_sci_rx26t_private.h"

/*****************************************************************************
Typedef definitions
******************************************************************************/

/*****************************************************************************
Macro definitions
******************************************************************************/

/*****************************************************************************
Private global variables and functions
******************************************************************************/

/*****************************************************************************
* Function Name: sci_mcu_param_check
* Description  : This function parameters check on MCU.
*                 (channel range, interrupt priority, etc...)
* Arguments    : chan -
*                    channel to check
* Return Value : SCI_SUCCESS -
*                    parameter check all successfully
*                SCI_ERR_BAD_CHAN -
*                    channel number invalid for part
*                SCI_ERR_INVALID_ARG -
*                    interrupt priority out of range
******************************************************************************/
sci_err_t sci_mcu_param_check(uint8_t const chan)
{
    /* channel range parameter check */
    if ((SCI_CH1 != chan) && (SCI_CH5 != chan) && (SCI_CH6 != chan) && (SCI_CH12 != chan))
    {
        return SCI_ERR_BAD_CHAN;
    }

    /* interrupt priority configuration parameter check */
    if ((1 > SCI_CFG_ERI_TEI_PRIORITY) || (15 < SCI_CFG_ERI_TEI_PRIORITY))
    {
        return SCI_ERR_INVALID_ARG;
    }

    return SCI_SUCCESS;
} /* End of function sci_mcu_param_check() */

/*****************************************************************************
* Function Name: sci_init_register
* Description  : This function initializes the register for SCI.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void sci_init_register(sci_hdl_t const hdl)
{
    /* SCI transmit enable bit and receive enable bit check & disable */
    /* WAIT_LOOP */
    while ((0 != hdl->rom->regs->SCR.BIT.TE) || (0 != hdl->rom->regs->SCR.BIT.RE))
    {
        if (0 != hdl->rom->regs->SCR.BIT.TE)
        {
            hdl->rom->regs->SCR.BIT.TE = 0;  // transmit disable
        }

        if (0 != hdl->rom->regs->SCR.BIT.RE)
        {
            hdl->rom->regs->SCR.BIT.RE = 0;  // receive disable
        }
    }

    /* SMR register initialize */
    hdl->rom->regs->SMR.BYTE = 0x00;

    /* SCR register initialize */
    hdl->rom->regs->SCR.BYTE = 0x00;

    /* SSR register initialize */
    if (1 == SCI_SSR_ORER)
    {
        SCI_SSR_ORER = 0;
    }

    if (1 == SCI_SSR_PER)
    {
        SCI_SSR_PER = 0;
    }

    if (1 == SCI_SSR_FER)
    {
        SCI_SSR_FER = 0;
    }

    /* SCMR register initialize */
    hdl->rom->regs->SCMR.BIT.SMIF = 0;
    hdl->rom->regs->SCMR.BIT.SINV = 0;
    hdl->rom->regs->SCMR.BIT.SDIR = 0;

    /* SPTR register initialize */
    hdl->rom->regs->SPTR.BIT.SPB2DT = 0;
    hdl->rom->regs->SPTR.BIT.SPB2IO = 0;
    hdl->rom->regs->SPTR.BIT.RTADJ  = 0;
    hdl->rom->regs->SPTR.BIT.TTADJ  = 0;
    hdl->rom->regs->SPTR.BIT.RINV   = 0;
    hdl->rom->regs->SPTR.BIT.TINV   = 0;

    /* TMGR register initialize */
    hdl->rom->regs->TMGR.BIT.RTMG = 0;
    hdl->rom->regs->TMGR.BIT.TTMG = 0;

    /* BRR register initialize */
    hdl->rom->regs->BRR = 0xFF;

    /* SEMR register initialize */
    hdl->rom->regs->SEMR.BIT.BRME    = 0;
    hdl->rom->regs->SEMR.BIT.ABCS    = 0;
    hdl->rom->regs->SEMR.BIT.ABCSE   = 0;
    hdl->rom->regs->SEMR.BIT.NFEN    = 0;
    hdl->rom->regs->SEMR.BIT.BGDM    = 0;
    hdl->rom->regs->SEMR.BIT.RXDESEL = 0;

    /* SNFR register initialize */
    hdl->rom->regs->SNFR.BYTE = 0;

    /* SPMR register initialize */
    hdl->rom->regs->SPMR.BIT.CTSE  = 0;
    hdl->rom->regs->SPMR.BIT.CKPOL = 0;
    hdl->rom->regs->SPMR.BIT.CKPH  = 0;

    #if SCI_CFG_DATA_MATCH_INCLUDED
    /* DCCR register initialize */
    hdl->rom->regs->DCCR.BIT.DCME  = 0;
    hdl->rom->regs->DCCR.BIT.DCMF  = 0;
    hdl->rom->regs->DCCR.BIT.DFER  = 0;
    hdl->rom->regs->DCCR.BIT.DPER  = 0;
    hdl->rom->regs->DCCR.BIT.IDSEL = 0;

    /* CDR register initialize */
    hdl->rom->regs->CDR.BYTE.L = 0;

    /* Set initial value of receive in 8-bit data length */
    hdl->rom->regs->SMR.BIT.CHR = 0;
    hdl->rom->regs->SCMR.BIT.CHR1 = 1;
    #endif /* End of SCI_CFG_DATA_MATCH_INCLUDED */

    return;
} /* End of function sci_init_register() */

/*****************************************************************************
* Function Name: sci_init_bit_rate
* Description  : This function determines the best possible settings for the
*                baud rate registers for the specified peripheral clock speed
*                and baud rate. Note that this does not guarantee a low bit
*                error rate, just the best possible one. The bit rate error is
*                returned in .1% increments. If the hardware cannot support
*                the specified combination, a value of 1000 (100% error) is
*                returned.
*
* NOTE: The transmitter and receiver (TE and RE bits in SCR) must be disabled
*       prior to calling this function.
*
*       The application must pause for 1 bit time after the BRR register
*       is loaded before transmitting/receiving to allow time for the clock
*       to settle.
*
* Arguments    : hdl -
*                    Handle for channel (ptr to chan control block)
*                    NOTE: mode element must be already set
*                pclk -
*                    Peripheral clock speed; e.g. 24000000 for 24MHz
*                baud -
*                    Baud rate; 19200, 57600, 115200, etc.
* Return Value : bit error in .1% increments; e.g. 16 = 1.6% bit rate error
*                a value of 1000 denotes 100% error; no registers set
******************************************************************************/
int32_t sci_init_bit_rate(sci_hdl_t const  hdl,
                          uint32_t const   pclk,
                          uint32_t const   baud)
{
    uint32_t i;
    uint32_t num_divisors = 0;
    uint32_t ratio;
    uint32_t tmp;
    baud_divisor_t const *p_baud_info = NULL;

    uint32_t divisor;
    uint32_t int_M;
    float    float_M;
    float    error;
    float    abs_error;

    #if SCI_CFG_PARAM_CHECKING_ENABLE
    if ((0 == pclk) || (0 == baud))
    {
        return 1000;
    }
    #endif /* End of SCI_CFG_PARAM_CHECKING_ENABLE */

    /* SELECT PROPER TABLE BASED UPON MODE */
    if (SCI_MODE_ASYNC == hdl->mode)
    {
    #if (SCI_CFG_ASYNC_INCLUDED)
        p_baud_info = async_baud;
        num_divisors = NUM_DIVISORS_ASYNC;
    #endif /* End of SCI_CFG_ASYNC_INCLUDED */
    }
    else
    {
        /* SYNC or SSPI */
    #if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
        p_baud_info = sync_baud;
        num_divisors = NUM_DIVISORS_SYNC;
    #endif /* End of SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED */
    }

    /* FIND DIVISOR; table has associated ABCS, BGDM, ABCSE and CKS values */
    /* BRR must be 255 or less */
    /* the "- 1" is ignored in some steps for approximations */
    /* BRR = (PCLK/(divisor * baud)) - 1 */
    /* BRR = (ratio / divisor) - 1 */
    ratio = pclk / baud;

    /* WAIT_LOOP */
    for(i = 0; i < num_divisors; i++)
    {
        if (SCI_MODE_ASYNC == hdl->mode)
        {
#if (SCI_CFG_ASYNC_INCLUDED)
   /* ABCSE bit is not available on CH12 */
   /* Skip divisor result have ABCSE bit*/
            if (SCI_CH12 == hdl->rom->chan)
            {
                if(1 == p_baud_info[i].abcse)
                {
                    continue;
                }
            }
#endif
        }
        /* Casting int16_t to uint32_t is valid. Because clock divisor is positive integer */
        if (ratio < (uint32_t)(p_baud_info[i].divisor * 256))
        {
             break;
        }
    }

    /* RETURN IF BRR WILL BE >255 OR LESS THAN 0 */
    if (i == num_divisors)
    {
        return(1000);           // impossible baud rate requested; return 100% error
    }

    /* Casting int16_t to uint32_t is valid. Because clock divisor is a positive integer */
    divisor = (uint32_t)p_baud_info[i].divisor;
    tmp = ratio / (divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1
    if (0 == tmp)
    {
        return (1000);            // illegal value; return 100% error
    }

    /* SET BRR, ABCS, BDGM, and CKS */
    tmp = ratio / (divisor / 2);  // divide by half the divisor

    /* if odd, "round up" by ignoring -1; divide by 2 again for rest of divisor */
    hdl->rom->regs->BRR = (uint8_t)((tmp & 0x01) ? (tmp / 2) : ((tmp / 2) - 1));
    hdl->rom->regs->SEMR.BIT.ABCS = p_baud_info[i].abcs;
    hdl->rom->regs->SEMR.BIT.BGDM = p_baud_info[i].bgdm;
    hdl->rom->regs->SMR.BIT.CKS = p_baud_info[i].cks;
    hdl->rom->regs->SEMR.BIT.ABCSE = p_baud_info[i].abcse;

    /* CALCULATE BIT RATE ERROR.
     * RETURN IF ERROR LESS THAN 1% OR IF IN SYNCHRONOUS/SSPI MODE.
     */
    tmp = ratio / (divisor);      // tmp = PCLK/(baud * divisor) = BRR+1 = N+1

    /* Casting uint32_t to float is valid */
    error     = (((float)pclk / ((baud * divisor) * tmp)) - 1) * 100;
    abs_error = (error < 0) ? (-error) : error;

    if ((abs_error <= 1.0f) || (SCI_MODE_ASYNC != hdl->mode))
    {
        hdl->rom->regs->SEMR.BIT.BRME = 0;          // disable MDDR

        /* Casting float to uint32_t */
        return (uint32_t)(error*10);
    }

    /* CALCULATE M ASSUMING A 0% ERROR then WRITE REGISTER */
    hdl->rom->regs->BRR = (uint8_t)(tmp - 1);

    /* Casting uint32_t to float is valid  */
    float_M = ((((float)baud * divisor) * 256) * tmp) / pclk;
    float_M *= 2;

    /* Casting float to uint32_t */
    int_M = (uint32_t)float_M;
    int_M = (int_M & 0x01) ? ((int_M / 2) + 1) : (int_M / 2);

    /* Casting uint32_t type to uint8_t type in this case is valid. Range value of m is not exceed uint8_t */
    hdl->rom->regs->MDDR = (uint8_t)int_M;      // write M
    hdl->rom->regs->SEMR.BIT.BRME = 1;          // enable MDDR

    /* Casting uint32_t to float is valid*/
    error = (((float)(pclk) / (((divisor * tmp) * baud) * ((float)(256)/int_M))) - 1) * 100;

    /* Casting float to int32_t */
    return (int32_t)(error*10);
} /* End of function sci_init_bit_rate() */

/*****************************************************************************
* Function Name: sci_initialize_ints
* Description  : This function sets priority, clears flags, and sets
*                interrupts in both the ICU and SCI peripheral. These include
*                RXI, TXI, TEI, and ERI/GROUP12 interrupts.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                priority -
*                    priority for interrupts
* Return Value : none
******************************************************************************/
void sci_initialize_ints(sci_hdl_t const hdl,
                         uint8_t const   priority)
{
    volatile bsp_int_ctrl_t group_priority;

    /* SET PRIORITY FOR INTERRUPTS */
    *hdl->rom->ipr_rxi = priority;          // can set separately using Control()
    *hdl->rom->ipr_txi = priority;

    group_priority.ipl = 0x00000000;

    /* Check interrupt priority */
    if (SCI_CFG_ERI_TEI_PRIORITY > IPR(ICU, GROUPBL0))
    {
        /* Casting a positive integer to uint32_t is valid */
        group_priority.ipl = (uint32_t)SCI_CFG_ERI_TEI_PRIORITY;
    }

    /* DISABLE ERI INTERRUPT */
    DISABLE_ERI_INT;

    /* DISABLE RXI INTERRUPT */
    DISABLE_RXI_INT;

    /* DISABLE TXI INTERRUPT */
    DISABLE_TXI_INT;

    /* DISABLE TEI INTERRUPT */
    DISABLE_TEI_INT;

    /* CLEAR INTERRUPT FLAGS */
    *hdl->rom->ir_rxi = 0;
    *hdl->rom->ir_txi = 0;
    DISABLE_TEI_INT;
    DISABLE_ERI_INT;

    /* REGISTER GROUP INTERRUPTS WITH BSP */
    #if SCI_CFG_TEI_INCLUDED
    R_BSP_InterruptWrite(hdl->rom->tei_vector, hdl->rom->tei_isr);
    #endif
    R_BSP_InterruptWrite(hdl->rom->eri_vector, hdl->rom->eri_isr);

    /* ENABLE GROUP INTERRUPTS */
    R_BSP_InterruptControl(hdl->rom->eri_vector, BSP_INT_CMD_GROUP_INTERRUPT_ENABLE, (void *)&group_priority);

    /* ENABLE ERI AND RXI INTERRUPTS REQUESTS */
    ENABLE_ERI_INT;
    ENABLE_RXI_INT;

    /* ENABLE INTERRUPTS IN SCI PERIPHERAL */
    /* Note: Enable interrupts after xcvr or will get "extra" interrupt */
    hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;    // enable TE, RE, TXI, and RXI/ERI

    return;
} /* End of function sci_initialize_ints() */

/*****************************************************************************
* Function Name: sci_disable_ints
* Description  : This function disable interrupts in both the ICU and SCI
*                peripheral. These include RXI, TXI, TEI, ERI, and group
*                interrupts.
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
* Return Value : none
******************************************************************************/
void sci_disable_ints(sci_hdl_t const hdl)
{
    volatile bsp_int_ctrl_t group_priority;

    /* Disable ICU RXI interrupt */
    DISABLE_RXI_INT;

    /* Disable ICU TXI interrupt */
    DISABLE_TXI_INT;

    /* Disable ICU ERI interrupt */
    DISABLE_ERI_INT;

    /* Disable ICU TEI interrupt */
    DISABLE_TEI_INT;

    /* disable peripheral interrupts and xcvr (TE and RE) */
    hdl->rom->regs->SCR.BYTE = 0;

    /* disable group interrupts */
    group_priority.ipl = 0x00000000;

    /* Casting pointer to void* is valid */
    R_BSP_InterruptControl(hdl->rom->eri_vector, BSP_INT_CMD_GROUP_INTERRUPT_DISABLE, (void *)&group_priority);

    return;
} /* End of function sci_disable_ints() */


#if (SCI_CFG_ASYNC_INCLUDED)
/*****************************************************************************
* Function Name: sci_async_cmds
* Description  : This function configures non-standard UART hardware and
*                performs special software operations.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCI_SUCCESS -
*                    Command completed successfully.
*                SCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
******************************************************************************/
sci_err_t sci_async_cmds(sci_hdl_t const hdl,
                         sci_cmd_t const cmd,
                         void            *p_args)
{
    sci_err_t   err=SCI_SUCCESS;
    int32_t     bit_err;
    uint32_t    slow_baud;

    #if SCI_CFG_PARAM_CHECKING_ENABLE
    /* Check parameters */
    if (((NULL == p_args) || (FIT_NO_PTR == p_args))
    && ((SCI_CMD_TX_Q_BYTES_FREE == cmd) || (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ == cmd) || (SCI_CMD_COMPARE_RECEIVED_DATA == cmd)))
    {
        return SCI_ERR_NULL_PTR;
    }
    #endif /* End of SCI_CFG_PARAM_CHECKING_ENABLE */

    switch (cmd)
    {
        case (SCI_CMD_EN_NOISE_CANCEL):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SEMR.BIT.NFEN = 1;      /* enable noise filter */
            hdl->rom->regs->SNFR.BYTE = 0;          /* clock divided by 1 (default) */
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        case (SCI_CMD_OUTPUT_BAUD_CLK):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SCR.BIT.CKE = 0x01;     /* output baud clock on SCK pin */
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        case (SCI_CMD_START_BIT_EDGE):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SEMR.BIT.RXDESEL = 1;   /* detect start bit on falling edge */
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        #if SCI_CFG_TEI_INCLUDED
        case (SCI_CMD_EN_TEI):  /* SCI_CMD_EN_TEI is obsolete command, but it exists only for compatibility with older version. */
        {
            break;
        }
        #endif /* End of SCI_CFG_TEI_INCLUDED */

        #if TX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_TX_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
            {
                if (false == hdl->tx_idle)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
        #endif /* End of TX_DTC_DMACA_ENABLE */

        #if RX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_RX_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                if (0 != hdl->queue[0].rx_cnt)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
        #endif /* End of RX_DTC_DMACA_ENABLE */

        case (SCI_CMD_TX_Q_FLUSH):
        {
            #if (SCI_CFG_USE_CIRCULAR_BUFFER == 1)
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            #else
            /* Disable TXI interrupt */
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);
            ENABLE_TXI_INT;

            /* Re-enable interrupts */
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            #endif /*End of SCI_CFG_USE_CIRCULAR_BUFFER == 1 */
            break;
        }

        case (SCI_CMD_RX_Q_FLUSH):
        {
            #if (SCI_CFG_USE_CIRCULAR_BUFFER == 1)
            R_BYTEQ_Flush(hdl->u_rx_data.que);
            #else
            /* Disable RXI interrupt */
            DISABLE_RXI_INT;
            R_BYTEQ_Flush(hdl->u_rx_data.que);
            ENABLE_RXI_INT;
            #endif /*End of SCI_CFG_USE_CIRCULAR_BUFFER == 1 */
            break;
        }

        case (SCI_CMD_TX_Q_BYTES_FREE):
        {
            /* Casting pointer void* to uint16_t* type is valid */
            R_BYTEQ_Unused(hdl->u_tx_data.que, (uint16_t *) p_args);
        break;
        }

        case (SCI_CMD_RX_Q_BYTES_AVAIL_TO_READ):
        {
            /* Casting pointer void* type to uint16_t* type is valid  */
            R_BYTEQ_Used(hdl->u_rx_data.que, (uint16_t *) p_args);
        break;
        }

        case (SCI_CMD_GENERATE_BREAK):
        {
            /* flush transmit queue */
            DISABLE_TXI_INT;
            R_BYTEQ_Flush(hdl->u_tx_data.que);

            #if(TX_DTC_DMACA_ENABLE)
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
            {
                sci_fifo_ctrl_t *p_tctrl = &hdl->queue[hdl->qindex_app_rx];
                p_tctrl->tx_cnt = 0;
                p_tctrl->tx_fraction = 0;

                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                if(SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable)
                {
                    dtc_cmd_arg_t args_dtc;
                    args_dtc.act_src = hdl->rom->dtc_tx_act_src;
                    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);
                }
                #endif /* End of (TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01)*/

                #if ((TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02))
                if(SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable)
                {
                    dmaca_stat_t stat_dmaca;
                    R_DMACA_Control(hdl->rom->dmaca_tx_channel, DMACA_CMD_DISABLE, &stat_dmaca);
                }
                #endif /* End of (TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02)*/
            }
            #endif /* End of TX_DTC_DMACA_ENABLE*/

            ENABLE_TXI_INT;

            /* NOTE: the following steps will abort anything being sent */

            /* set baud rate 1.5x slower */
            slow_baud = (hdl->baud_rate << 1) / 3;
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            bit_err = sci_init_bit_rate(hdl, hdl->pclk_speed, slow_baud);
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            if (1000 == bit_err)
            {
                err = SCI_ERR_INVALID_ARG;
            }
            else
            {
                /* transmit "0" and wait for completion */
                SCI_TDR(0);

                /* WAIT_LOOP */
                while (0 == hdl->rom->regs->SSR.BIT.TEND)
                {
                    R_BSP_NOP();
                }

                /* restore original baud rate */
                hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
                SCI_SCR_DUMMY_READ;
                sci_init_bit_rate(hdl, hdl->pclk_speed, hdl->baud_rate);
                SCI_IR_TXI_CLEAR;
                hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            }
            break;
        }

        #if SCI_CFG_DATA_MATCH_INCLUDED
        case SCI_CMD_COMPARE_RECEIVED_DATA:
        {
            hdl->rom->regs->DCCR.BIT.DFER = 0; /* Clear Match Data Framing Error Flag */
            hdl->rom->regs->DCCR.BIT.DPER = 0; /* Clear Match Data Parity Error Flag */
            hdl->rom->regs->DCCR.BIT.DCME = 1; /* Enable Data match function */
            hdl->rom->regs->CDR.BYTE.L = *((unsigned char *)p_args); /* Comparison data */
            break;
        }
        #endif /* End of SCI_CFG_DATA_MATCH_INCLUDED */

        /* Enable receive data sampling timing adjust feature*/
        case SCI_CMD_RX_SAMPLING_ENABLE:
        {
            hdl->rom->regs->SPTR.BIT.RTADJ = 1;
            break;
        }

        /* Disable receive data sampling timing adjust feature*/
        case SCI_CMD_RX_SAMPLING_DISABLE:
        {
            hdl->rom->regs->SPTR.BIT.RTADJ = 0;
            break;
        }

        /* Enable transmit signal transition timing adjust feature*/
        case SCI_CMD_TX_TRANSITION_TIMING_ENABLE:
        {
            hdl->rom->regs->SPTR.BIT.TTADJ = 1;
            break;
        }

        /* Disable transmit signal transition timing adjust feature*/
        case SCI_CMD_TX_TRANSITION_TIMING_DISABLE:
        {
            hdl->rom->regs->SPTR.BIT.TTADJ = 0;
            break;
        }

        /* Set value for receive data sampling timing adjust feature*/
        case SCI_CMD_SAMPLING_TIMING_ADJUST:
        {
            if ((0 == hdl->rom->regs->SEMR.BIT.ABCSE) && (0 == hdl->rom->regs->SEMR.BIT.ABCS))
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if ((*(uint8_t *)p_args) <= 15)
                {
                    if (1 == hdl->rom->regs->SPTR.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->TMGR.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = SCI_ERR_INVALID_ARG;
                }
            }
            else if ((0 == hdl->rom->regs->SEMR.BIT.ABCSE) && (1 == hdl->rom->regs->SEMR.BIT.ABCS))
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if (((*(uint8_t *)p_args) <= 3) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 11)))
                {
                    if (1 == hdl->rom->regs->SPTR.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->TMGR.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = SCI_ERR_INVALID_ARG;
                }
            }
            else
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if (((*(uint8_t *)p_args) <= 2) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 10)))
                {
                    if (1 == hdl->rom->regs->SPTR.BIT.RTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->TMGR.BIT.RTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = SCI_ERR_INVALID_ARG;
                }
            }
            break;
        }

        /*Set va transmit signal transition timing adjust feature*/
        case SCI_CMD_TRANSITION_TIMING_ADJUST:
        {
            if (0 == hdl->rom->regs->SEMR.BIT.ABCSE)
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if ((*(uint8_t *)p_args) <= 15)
                {
                    if (1 == hdl->rom->regs->SPTR.BIT.TTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->TMGR.BIT.TTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = SCI_ERR_INVALID_ARG;
                }
            }
            else
            {
                /* Casting pointer void* to uint8_t* type is valid */
                if (((*(uint8_t *)p_args) <= 5) || ((8 <= (*(uint8_t *)p_args)) && ((*(uint8_t *)p_args) <= 13)))
                {
                    if (1 == hdl->rom->regs->SPTR.BIT.TTADJ)
                    {
                        /* Casting pointer void* to uint8_t* type is valid */
                        hdl->rom->regs->TMGR.BIT.TTMG = *(uint8_t *)p_args;
                    }
                }
                else
                {
                    err = SCI_ERR_INVALID_ARG;
                }
            }
            break;
        }

        default:
        {
            err = SCI_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function sci_async_cmds() */
#endif /* End of SCI_CFG_ASYNC_INCLUDED */

#if (SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED)
/*****************************************************************************
* Function Name: sci_sync_cmds
* Description  : This function performs special software operations specific
*                to the SSPI and SYNC protocols.
*
* Arguments    : hdl -
*                    handle for channel (ptr to chan control block)
*                cmd -
*                    command to run
*                p_args -
*                    pointer argument(s) specific to command
* Return Value : SCI_SUCCESS -
*                    Command completed successfully.
*                SCI_ERR_NULL_PTR -
*                    p_args is NULL when required for cmd
*                SCI_ERR_INVALID_ARG -
*                    The cmd value or p_args contains an invalid value.
*                    May be due to mode channel is operating in.
******************************************************************************/
sci_err_t sci_sync_cmds(sci_hdl_t const hdl,
                        sci_cmd_t const cmd,
                        void            *p_args)
{
    sci_spi_mode_t  spi_mode;
    sci_cb_args_t   args;
    sci_err_t       err = SCI_SUCCESS;

    switch (cmd)
    {
        case (SCI_CMD_CHECK_XFER_DONE):
        {
            if (false == hdl->tx_idle)
            {
                err = SCI_ERR_XFER_NOT_DONE;
            }
            break;
        }

        case (SCI_CMD_XFER_LSB_FIRST):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SCMR.BIT.SDIR = 0;
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        case (SCI_CMD_XFER_MSB_FIRST):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SCMR.BIT.SDIR = 1;
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        case (SCI_CMD_INVERT_DATA):
        {
            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SCMR.BIT.SINV ^= 1;
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        case (SCI_CMD_ABORT_XFER):
        {
            /* Disable receive interrupts in ICU and peripheral */
            DISABLE_RXI_INT;
            DISABLE_ERI_INT;

            #if(TX_DTC_DMACA_ENABLE)
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable))
            {
                sci_fifo_ctrl_t        *p_tctrl = &hdl->queue[hdl->qindex_app_rx];
                p_tctrl->tx_cnt = 0;
                p_tctrl->tx_fraction = 0;

                #if ((TX_DTC_DMACA_ENABLE & 0x01) || (RX_DTC_DMACA_ENABLE & 0x01))
                if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_tx_enable) && (SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
                {
                    // Set condition for reset TDFR to generate interrupt in next time
                    hdl->qindex_int_tx = 1;
                    dtc_cmd_arg_t           args_dtc;
                    args_dtc.act_src               = hdl->rom->dtc_tx_act_src;
                    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);

                    args_dtc.act_src               = hdl->rom->dtc_rx_act_src;
                    R_DTC_Control(DTC_CMD_ACT_SRC_DISABLE, NULL, &args_dtc);
                }
                #endif

                #if ((TX_DTC_DMACA_ENABLE & 0x02) || (RX_DTC_DMACA_ENABLE & 0x02))
                if((SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_tx_enable) && (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
                {
                    R_DMACA_Close(hdl->rom->dmaca_tx_channel);
                    R_DMACA_Close(hdl->rom->dmaca_rx_channel);
                }
                #endif
            }
            #endif

            hdl->rom->regs->SCR.BYTE &= (~(SCI_SCR_REI_MASK | SCI_SCR_RE_MASK | SCI_SCR_TE_MASK));

            hdl->tx_cnt = 0;
            hdl->tx_dummy = false;
            hdl->tx_idle = true;

            /* Do callback if available */
            if ((NULL != hdl->callback) && (FIT_NO_FUNC != hdl->callback))
            {
                args.hdl = hdl;
                args.event = SCI_EVT_XFER_ABORTED;

                /* Casting pointer to void* is valid */
                hdl->callback((void *)&args);
            }

            *hdl->rom->ir_rxi = 0;                  /* clear rxi interrupt flag */

            DISABLE_ERI_INT;                        /* clear eri interrupt flag */
            ENABLE_ERI_INT;                         /* enable rx err interrupts in ICU */
            ENABLE_RXI_INT;                         /* enable receive interrupts in ICU */

            /* Enable receive interrupt in peripheral after rcvr or will get "extra" interrupt */
            hdl->rom->regs->SCR.BYTE |= (SCI_SCR_RE_MASK | SCI_SCR_TE_MASK);
            hdl->rom->regs->SCR.BYTE |= SCI_SCR_REI_MASK;
            break;
        }

        #if RX_DTC_DMACA_ENABLE
        case (SCI_CMD_CHECK_RX_SYNC_DONE):
        {
            if((SCI_DTC_ENABLE == hdl->rom->dtc_dmaca_rx_enable) || (SCI_DMACA_ENABLE == hdl->rom->dtc_dmaca_rx_enable))
            {
                if (0 != hdl->queue[0].rx_cnt)
                {
                    err = SCI_ERR_XCVR_BUSY;
                }
            }
            break;
        }
        #endif

        case (SCI_CMD_CHANGE_SPI_MODE):
        {
            #if SCI_CFG_PARAM_CHECKING_ENABLE
            if (SCI_MODE_SSPI != hdl->mode)
            {
                return SCI_ERR_INVALID_ARG;
            }

            /* Check parameters */
            if ((NULL == p_args ) || (FIT_NO_PTR == p_args))
            {
                return SCI_ERR_NULL_PTR;
            }

            /* Casting pointer void* type is valid */
            spi_mode = *((sci_spi_mode_t *)p_args);

            if ((SCI_SPI_MODE_0 != spi_mode) && (SCI_SPI_MODE_1 != spi_mode)
              && (SCI_SPI_MODE_2 != spi_mode) && (SCI_SPI_MODE_3 != spi_mode))
            {
                return SCI_ERR_INVALID_ARG;
            }
            #endif

            hdl->rom->regs->SCR.BYTE &= (~SCI_EN_XCVR_MASK);
            SCI_SCR_DUMMY_READ;
            hdl->rom->regs->SPMR.BYTE &= 0x3F;      /* clear previous mode */
            hdl->rom->regs->SPMR.BYTE |= (*((uint8_t *)p_args));
            SCI_IR_TXI_CLEAR;
            hdl->rom->regs->SCR.BYTE |= SCI_EN_XCVR_MASK;
            break;
        }

        default:
        {
            err = SCI_ERR_INVALID_ARG;
            break;
        }
    }

    return err;
} /* End of function sci_sync_cmds() */
#endif /* End of SCI_CFG_SSPI_INCLUDED || SCI_CFG_SYNC_INCLUDED */

/*****************************************************************************
ISRs
******************************************************************************/


#if ((SCI_CFG_ASYNC_INCLUDED) || (TX_DTC_DMACA_ENABLE | RX_DTC_DMACA_ENABLE))

/*****************************************************************************
* sciN_txiN_isr
* Description  : TXI interrupt routines for every SCI channel
******************************************************************************/

#if SCI_CFG_CH1_INCLUDED
/*******************************************************************************
 * Function Name: sci1_txi1_isr
 * Description  : TXI interrupt routines for SCI1 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_txi1_isr, VECT(SCI1,TXI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_txi1_isr(void)
{
#if SCI_CFG_CH1_EN_TXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    txi_handler(&ch1_ctrl);
} /* End of function sci1_txi1_isr() */
#endif /* End of SCI_CFG_CH1_INCLUDED  */

#if SCI_CFG_CH5_INCLUDED
/*******************************************************************************
 * Function Name: sci5_txi5_isr
 * Description  : TXI interrupt routines for SCI5 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_txi5_isr, VECT(SCI5,TXI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_txi5_isr(void)
{
#if SCI_CFG_CH5_EN_TXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    txi_handler(&ch5_ctrl);
} /* End of function sci5_txi5_isr() */
#endif /* End of SCI_CFG_CH5_INCLUDED  */

#if SCI_CFG_CH6_INCLUDED
/*******************************************************************************
 * Function Name: sci6_txi6_isr
 * Description  : TXI interrupt routines for SCI6 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_txi6_isr, VECT(SCI6,TXI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_txi6_isr(void)
{
#if SCI_CFG_CH6_EN_TXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    txi_handler(&ch6_ctrl);
} /* End of function sci6_txi6_isr() */
#endif /* End of SCI_CFG_CH6_INCLUDED  */

#if SCI_CFG_CH12_INCLUDED
/*******************************************************************************
 * Function Name: sci12_txi12_isr
 * Description  : TXI interrupt routines for SCI12 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_txi12_isr, VECT(SCI12,TXI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_txi12_isr(void)
{
#if SCI_CFG_CH12_EN_TXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    txi_handler(&ch12_ctrl);
} /* End of function sci12_txi12_isr() */
#endif /* End of SCI_CFG_CH12_INCLUDED  */

#endif /* End of SCI_CFG_ASYNC_INCLUDED */

#if SCI_CFG_TEI_INCLUDED
/*****************************************************************************
* sciN_teiN_isr
*
* Description  : TEI interrupt routines for every SCI channel.
*                BSP gets main group interrupt, then vectors to/calls these
*                "interrupts"/callbacks.
******************************************************************************/

#if SCI_CFG_CH1_INCLUDED
/*******************************************************************************
 * Function Name: sci1_tei1_isr
 * Description  : TEI interrupt routines for SCI1 channel.
 ******************************************************************************/
void sci1_tei1_isr(void *cb_args)
{
#if SCI_CFG_CH1_EN_TEI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    tei_handler(&ch1_ctrl);
} /* End of function sci1_tei1_isr() */
#endif /* End of SCI_CFG_CH1_INCLUDED */

#if SCI_CFG_CH5_INCLUDED
/*******************************************************************************
 * Function Name: sci5_tei5_isr
 * Description  : TEI interrupt routines for SCI5 channel.
 ******************************************************************************/
void sci5_tei5_isr(void *cb_args)
{
#if SCI_CFG_CH5_EN_TEI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    tei_handler(&ch5_ctrl);
} /* End of function sci5_tei5_isr() */
#endif /* End of SCI_CFG_CH5_INCLUDED */

#if SCI_CFG_CH6_INCLUDED
/*******************************************************************************
 * Function Name: sci6_tei6_isr
 * Description  : TEI interrupt routines for SCI6 channel.
 ******************************************************************************/
void sci6_tei6_isr(void *cb_args)
{
#if SCI_CFG_CH6_EN_TEI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    tei_handler(&ch6_ctrl);
} /* End of function sci6_tei6_isr() */
#endif /* End of SCI_CFG_CH6_INCLUDED */

#if SCI_CFG_CH12_INCLUDED
/*****************************************************************************
* Function Name: sci12_tei12_isr
* Description  : TEI interrupt routines for SCI12 channel.
******************************************************************************/
void sci12_tei12_isr(void *cb_args)
{
#if SCI_CFG_CH12_EN_TEI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    tei_handler(&ch12_ctrl);
} /* End of function sci12_tei12_isr() */
#endif /* End of SCI_CFG_CH12_INCLUDED */

#endif /* SCI_CFG_TEI_INCLUDED */

/*****************************************************************************
* sciN_rxiN_isr
* Description  : RXI interrupt routines for every SCI channel
******************************************************************************/

#if SCI_CFG_CH1_INCLUDED
/*******************************************************************************
 * Function Name: sci1_rxi1_isr
 * Description  : RXI interrupt routines for SCI1 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci1_rxi1_isr, VECT(SCI1,RXI1))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci1_rxi1_isr(void)
{
#if SCI_CFG_CH1_EN_RXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    rxi_handler(&ch1_ctrl);
} /* End of function sci1_rxi1_isr() */
#endif /* End of SCI_CFG_CH1_INCLUDED */

#if SCI_CFG_CH5_INCLUDED
/*******************************************************************************
 * Function Name: sci5_rxi5_isr
 * Description  : RXI interrupt routines for SCI5 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci5_rxi5_isr, VECT(SCI5,RXI5))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci5_rxi5_isr(void)
{
#if SCI_CFG_CH5_EN_RXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    rxi_handler(&ch5_ctrl);
} /* End of function sci5_rxi5_isr() */
#endif /* End of SCI_CFG_CH5_INCLUDED */

#if SCI_CFG_CH6_INCLUDED
/*******************************************************************************
 * Function Name: sci6_rxi6_isr
 * Description  : RXI interrupt routines for SCI6 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci6_rxi6_isr, VECT(SCI6,RXI6))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci6_rxi6_isr(void)
{
#if SCI_CFG_CH6_EN_RXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    rxi_handler(&ch6_ctrl);
} /* End of function sci6_rxi6_isr() */
#endif /* End of SCI_CFG_CH6_INCLUDED */

#if SCI_CFG_CH12_INCLUDED
/*******************************************************************************
 * Function Name: sci12_rxi12_isr
 * Description  : RXI interrupt routines for SCI12 channel
 ******************************************************************************/
R_BSP_PRAGMA_STATIC_INTERRUPT(sci12_rxi12_isr, VECT(SCI12,RXI12))
R_BSP_ATTRIB_STATIC_INTERRUPT void sci12_rxi12_isr(void)
{
#if SCI_CFG_CH12_EN_RXI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    rxi_handler(&ch12_ctrl);
} /* End of function sci12_rxi12_isr() */
#endif /* End of SCI_CFG_CH12_INCLUDED */

/*****************************************************************************
* sciN_eriN_isr
*
* Description  : ERI interrupt routines for every SCI channel.
*                BSP gets main group interrupt, then vectors to/calls these
*                "interrupts"/callbacks.
******************************************************************************/

#if SCI_CFG_CH1_INCLUDED
/*****************************************************************************
* Function name: sci1_eri1_isr
* Description  : ERI interrupt routines for SCI1 channel.
******************************************************************************/
void sci1_eri1_isr(void *cb_args)
{
#if SCI_CFG_CH1_EN_ERI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    eri_handler(&ch1_ctrl);
} /* End of function sci1_eri1_isr() */
#endif /* End of SCI_CFG_CH1_INCLUDED */

#if SCI_CFG_CH5_INCLUDED
/*****************************************************************************
* Function name: sci5_eri5_isr
* Description  : ERI interrupt routines for SCI5 channel.
******************************************************************************/
void sci5_eri5_isr(void *cb_args)
{
#if SCI_CFG_CH5_EN_ERI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    eri_handler(&ch5_ctrl);
} /* End of function sci5_eri5_isr() */
#endif /* End of SCI_CFG_CH5_INCLUDED */

#if SCI_CFG_CH6_INCLUDED
/*****************************************************************************
* Function name: sci6_eri6_isr
* Description  : ERI interrupt routines for SCI6 channel.
******************************************************************************/
void sci6_eri6_isr(void *cb_args)
{
#if SCI_CFG_CH6_EN_ERI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    eri_handler(&ch6_ctrl);
} /* End of function sci6_eri6_isr() */
#endif /* End of SCI_CFG_CH6_INCLUDED */

#if SCI_CFG_CH12_INCLUDED
/*****************************************************************************
* Function name: sci12_eri12_isr
* Description  : ERI interrupt routines for SCI12 channel.
******************************************************************************/
void sci12_eri12_isr(void *cb_args)
{
#if SCI_CFG_CH12_EN_ERI_NESTED_INT == 1
    /* set bit PSW.I = 1 to allow nested interrupt */
    R_BSP_SETPSW_I();
#endif

    eri_handler(&ch12_ctrl);
} /* End of function sci12_eri12_isr() */
#endif /* End of SCI_CFG_CH12_INCLUDED */

/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifdef CONFIG_USE_RCARSOC_DRV_MFIS
#include "mfis/mfis.h"
#include "mfis_internal.h"
#include "interrupts.h"
#else
#include "mfis/mfis.h"
#include "mfis/mfis_lock.h"
#include "mfis_internal.h"
#include "interrupts.h"
#include "rcar_utils.h"
#endif /* CONFIG_USE_RCARSOC_DRV_MFIS */
/*--------------------------- MFIS Driver ---------------------------------*/

/* Get interrupt source number of a channel */
uint16_t mfis_get_int_source_num(struct mfis_channel *ch)
{
    uint16_t result = 0;
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        result = (uint16_t)(*(volatile uint32_t *) EICR(ch->ch)) >> 1;
    }
    else
    {
        result = (uint16_t)(*(volatile uint32_t *) IICR(ch->ch)) >> 1;
    }

    result = (result == 0) ? 0xff : result;
    return result;
}

/* Get message of a channel */
uint32_t mfis_get_message(struct mfis_channel *ch)
{
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        return *(volatile uint32_t *) EMBR(ch->ch);
    }
    else
    {
        return *(volatile uint32_t *) IMBR(ch->ch);
    }

}


/* Interrupt callback sample */
void mfis_interrupt_cb(void* data)
{
    struct mfis_channel *ch = (struct mfis_channel*) data;

    ch->int_source = mfis_get_int_source_num(ch);
    ch->recv_message = mfis_get_message(ch);

    if(ch->cb_function != (void*)0) {
        ch->cb_function(ch->arg);
    }

    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;

    /* Clear interrupt flag */
    uint32_t value;
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        value = *(volatile uint32_t *)EICR(ch->ch);
        *(volatile uint32_t *)EICR(ch->ch) = value & 0xFFFE;
    }
    else
    {
        value = *(volatile uint32_t *)EICR(ch->ch);
        *(volatile uint32_t *)IICR(ch->ch) = value & 0xFFFE;
    }

}

/* Initialize MFIS */
int mfis_init(struct mfis_channel *ch)
{
    //* Initialize */
    ch->int_source = 0;
    ch->recv_message = 0;
    unsigned int intid = MFIS_INTID(ch->ch, ch->type);
    /* Set callback function */
    Irq_SetupEntry(intid, (IrqHandlerFn)mfis_interrupt_cb, (void*) ch);
    /* Enable interrupt from Receiver to Sender */
    Irq_SetPriority(intid, IPRIORITY(2));
    Irq_Enable(intid);

    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;

    return 0;
}

int mfis_deinit(struct mfis_channel *ch)
{
    Irq_Disable(INTID_R_S(ch->ch));
    Irq_RemoveEntry(INTID_R_S(ch->ch));
    return 0;
}

/* Trigger interrupt to Receiver channel ch
int_number is 15-bit integer for interrupt source
*/
int mfis_trigger_interrupt(struct mfis_channel *ch, uint16_t int_number)
{
    if(ch->ch > 63 || int_number > 0x7FFF)
    {
        return -1;
    }

    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        *(volatile uint32_t *)IICR(ch->ch) = (int_number << 1) | 0x01;
    }
    else
    {
        *(volatile uint32_t *)EICR(ch->ch) = (int_number << 1) | 0x01;
    }
    return 0;
}

/* Send a 32-bit int to Receiver via message register */
int mfis_send_message(struct mfis_channel *ch, uint32_t value)
{
    if(ch->ch > 63)
    {
        return -1;
    }

#ifdef CONFIG_USE_RCARSOC_DRV_MFIS
    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;
#endif /* CONFIG_USE_RCARSOC_DRV_MFIS */

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        *(volatile uint32_t *)IMBR(ch->ch) = value;
    }
    else
    {
        *(volatile uint32_t *)EMBR(ch->ch) = value;
    }
    return 0;
}

#ifdef CONFIG_USE_RCARSOC_DRV_MFIS
/* Unlock write protection */
void mfis_unlock_write(void)
{
    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;
}

/* Clear incoming interrupt flag  */
int mfis_clear_interrupt(struct mfis_channel *ch)
{
    if(ch->ch > 63)
    {
        return -1;
    }

    /* Unlock MFIS register write protection */
    *(volatile uint32_t *)(MFIS_UNLOCK_WRITE) = 0xACC00001U;

    /* Clear interrupt flag */
    uint32_t value;
    if(ch->type == MFIS_TYPE_RECEVER)
    {
        value = *(volatile uint32_t *)EICR(ch->ch);
        *(volatile uint32_t *)EICR(ch->ch) = value & 0xFFFE;
    }
    else
    {
        value = *(volatile uint32_t *)IICR(ch->ch);
        *(volatile uint32_t *)IICR(ch->ch) = value & 0xFFFE;
    }

    return 0;
}

/* Check whether TX interrupt request bit is still pending */
int mfis_check_tx_pending(struct mfis_channel *ch, bool *pending)
{
    uint32_t reg;

    if ((ch == NULL) || (pending == NULL) || (ch->ch > 63))
    {
        return -1;
    }

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        reg = *(volatile uint32_t *)IICR(ch->ch);
    }
    else
    {
        reg = *(volatile uint32_t *)EICR(ch->ch);
    }

    *pending = (reg & 0x1U) != 0U;

    return 0;
}

/* Check whether RX interrupt request bit is still pending */
int mfis_check_rx_pending(struct mfis_channel *ch, bool *pending)
{
    uint32_t reg;

    if ((ch == NULL) || (pending == NULL) || (ch->ch > 63))
    {
        return -1;
    }

    if (ch->type == MFIS_TYPE_RECEVER)
    {
        reg = *(volatile uint32_t *)EICR(ch->ch);
    }
    else
    {
        reg = *(volatile uint32_t *)IICR(ch->ch);
    }

    *pending = (reg & 0x1U) != 0U;

    return 0;
}
#else
e_mfis_lock_status_t R_MFIS_LockAcquire(e_mfis_lock_id_t mfis_id, uint32_t timeout)
{
    e_mfis_lock_status_t ret;
    uint32_t timer_feq = R_UTILS_GetTimerFrequency();
    uint64_t start;
    uintptr_t mfis_lock_reg;

    if (mfis_id < MFIS_LOCK_ID_0 || mfis_id >= MFIS_LOCK_ID_MAX_NUM) {
        ret = MFIS_LOCK_ID_UNSUPPORTED;
        return ret;
    }

    if (mfis_id < MFIS_LOCK_ID_8) {
        mfis_lock_reg = MFIS_LOCK_0_7_BASE + mfis_id*4;
    }
    else {
        mfis_lock_reg = MFIS_LOCK_8_63_BASE + mfis_id*4;
    }

    start = R_UTILS_GetTimerCounter();
    while(1) {
        if ( *(volatile uint32_t*)mfis_lock_reg == MFIS_LOCK_IS_ACQUIRED ) {
            if ( (timeout == 0) ||
                ((R_UTILS_GetTimerCounter() - start)*1000/timer_feq < timeout) )
            {
                continue;
            }
            else
            {
                ret = MFIS_LOCK_TIMEOUT;
                break;
            }
        }
        else {
            ret = MFIS_LOCK_SUCCESS;
            break;
        }
    }

    return ret;
}

e_mfis_lock_status_t R_MFIS_LockRelease(e_mfis_lock_id_t mfis_id)
{
    e_mfis_lock_status_t ret;
    uintptr_t mfis_lock_reg;

    if (mfis_id < MFIS_LOCK_ID_0 || mfis_id >= MFIS_LOCK_ID_MAX_NUM) {
        ret = MFIS_LOCK_ID_UNSUPPORTED;
        return ret;
    }

    if (mfis_id < MFIS_LOCK_ID_8) {
        mfis_lock_reg = MFIS_LOCK_0_7_BASE + mfis_id*4;
    }
    else {
        mfis_lock_reg = MFIS_LOCK_8_63_BASE + mfis_id*4;
    }

    *(volatile uint32_t*)MFIS_UNLOCK_WRITE = 0xACCE0001;
    *(volatile uint32_t*)mfis_lock_reg = MFIS_LOCK_RELEASE;
    *(volatile uint32_t*)MFIS_UNLOCK_WRITE = 0xACC00000;

    return MFIS_LOCK_SUCCESS;
}

#endif /* CONFIG_USE_RCARSOC_DRV_MFIS */

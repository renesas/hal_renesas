/* generated configuration header file - do not edit */
#include <zephyr/devicetree.h>

#ifndef R_CANFD_CFG_H_
#define R_CANFD_CFG_H_
#ifdef __cplusplus
            extern "C" {
            #endif

#define CANFD_CFG_PARAM_CHECKING_ENABLE   ((BSP_CFG_PARAM_CHECKING_ENABLE))

#define CANFD_CFG_AFL_CH0_RULE_NUM   (32)
#define CANFD_CFG_AFL_CH1_RULE_NUM   (0)

#define CANFD_CFG_GLOBAL_ERROR_CH    ((0U))

#define CANFD_CFG_FD_PROTOCOL_EXCEPTION ((0))
#define CANFD_CFG_GLOBAL_ERR_IPL        ((2))
#define CANFD_CFG_RX_FIFO_IPL           ((2))
#define CANFD_CFG_GLOBAL_ERR_SOURCES    ( 0x3)
#define CANFD_CFG_TX_PRIORITY           ((R_CANFD_GCFG_TPRI_Msk))
#define CANFD_CFG_DLC_CHECK             ((0))
#define CANFD_CFG_FD_OVERFLOW           ((0))
#define CANFD_CFG_RXMB_NUMBER           (0)
#define CANFD_CFG_RXMB_SIZE             ((0))
#define CANFD_CFG_RXFIFO0_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO0_DEPTH         ((3))
#define CANFD_CFG_RXFIFO0_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO0_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO0_ENABLE        ((1))

#define CANFD_CFG_RXFIFO1_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO1_DEPTH         ((3))
#define CANFD_CFG_RXFIFO1_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO1_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO1_ENABLE        ((0))

#define CANFD_CFG_RXFIFO2_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO2_DEPTH         ((3))
#define CANFD_CFG_RXFIFO2_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO2_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO2_ENABLE        ((0))

#define CANFD_CFG_RXFIFO3_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO3_DEPTH         ((3))
#define CANFD_CFG_RXFIFO3_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO3_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO3_ENABLE        ((0))

#define CANFD_CFG_RXFIFO4_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO4_DEPTH         ((3))
#define CANFD_CFG_RXFIFO4_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO4_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO4_ENABLE        ((0))

#define CANFD_CFG_RXFIFO5_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO5_DEPTH         ((3))
#define CANFD_CFG_RXFIFO5_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO5_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO5_ENABLE        ((0))

#define CANFD_CFG_RXFIFO6_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO6_DEPTH         ((3))
#define CANFD_CFG_RXFIFO6_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO6_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO6_ENABLE        ((0))

#define CANFD_CFG_RXFIFO7_INT_THRESHOLD ((3U))
#define CANFD_CFG_RXFIFO7_DEPTH         ((3))
#define CANFD_CFG_RXFIFO7_PAYLOAD       ((7))
#define CANFD_CFG_RXFIFO7_INT_MODE      ((R_CANFD_RFCR_RFIE_Msk  | R_CANFD_RFCR_RFIM_Msk))
#define CANFD_CFG_RXFIFO7_ENABLE        ((0))

#define CANFD_CFG_COMMONFIFO0 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_CFG_COMMONFIFO1 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_CFG_COMMONFIFO2 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_CFG_COMMONFIFO3 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_CFG_COMMONFIFO4 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_CFG_COMMONFIFO5 (((0) << R_CANFD_CFCR_CFE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFRIE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_CFTIE_Pos) | \
                                                    ((7) << R_CANFD_CFCR_PLS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_MODE_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITCS_Pos) | \
                                                    ((0) << R_CANFD_CFCR_ITR_Pos) | \
                                                    ((R_CANFD_RFCR_RFIE_Msk) << R_CANFD_CFCR_CFIM_Pos) | \
                                                    ((3U) << R_CANFD_CFCR_CFITH_Pos) | \
                                                    (0 << R_CANFD_CFCR_LTM_Pos) | \
                                                    ((3) << R_CANFD_CFCR_FDS_Pos) | \
                                                    (0 << R_CANFD_CFCR_TINT_Pos))

#define CANFD_GROUP_IRQ DT_IRQ(DT_NODELABEL(group_irq_al2), irq)

/* Global interrupt */
#define CANFDG0_RFRI DT_IRQ_BY_NAME(DT_NODELABEL(canfd_global), rfri, irq)
#define CANFDG0_GLEI DT_IRQ_BY_NAME(DT_NODELABEL(canfd_global), glei, irq)

#define VECTOR_NUMBER_CANFDG0_RFRI ((IRQn_Type)(CANFDG0_RFRI << 8U | CANFD_GROUP_IRQ))
#define VECTOR_NUMBER_CANFDG0_GLEI ((IRQn_Type)(CANFDG0_GLEI << 8U | CANFD_GROUP_IRQ))

#ifdef __cplusplus
            }
            #endif
#endif /* R_CANFD_CFG_H_ */

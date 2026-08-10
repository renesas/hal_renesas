/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef RDMAC_COMMON_H
#define RDMAC_COMMON_H

/**
*  @defgroup      RDMAC_Module Configuration Module of RDMAC Module
*  @{
*  @brief         This module provides functions to configure and control RDMAC communication.
*
*  The RDMAC module allows for the configuration and control of RDMAC communication.
*  It provides struct,enum to configure RDMAC.
*/

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

/**
 *  @brief         Enum representing RT_DMAC instances
 *  @details       R-Car Gen5 will have 4 instances: 0, 1, 2, 3
 *  @note          Each instance corresponds to a specific DMA controller in the system.
 */
typedef enum {
    RT_DMAC0, /*!< RT-DMA Controller Instance 0 */
    RT_DMAC1, /*!< RT-DMA Controller Instance 1 */
    RT_DMAC2, /*!< RT-DMA Controller Instance 2 */
    RT_DMAC3, /*!< RT-DMA Controller Instance 3 */
    SYS_DMAC0 = 8, /*!< SYS-DMA Controller Instance 0 */
    SYS_DMAC1 = 9, /*!< SYS-DMA Controller Instance 1 */
    SYS_DMAC2 = 10, /*!< SYS-DMA Controller Instance 2 */
    SYS_DMAC3 = 11, /*!< SYS-DMA Controller Instance 3 */
    DMAC_MAX /*!< Maximum number of DMA Controller Instances */
} DMAC_t;

/**
 *  @brief         Enum representing RT_DMAC channels
 *  @details       R-Car Gen5 each instance will have 16 channels from 0 to 15.
 *  @note          Each channel corresponds to a specific DMA channel within a DMA controller instance.
 */
typedef enum {
    DMAC_CH0,  /*!< DMA Channel 0 */
    DMAC_CH1,  /*!< DMA Channel 1 */
    DMAC_CH2,  /*!< DMA Channel 2 */
    DMAC_CH3,  /*!< DMA Channel 3 */
    DMAC_CH4,  /*!< DMA Channel 4 */
    DMAC_CH5,  /*!< DMA Channel 5 */
    DMAC_CH6,  /*!< DMA Channel 6 */
    DMAC_CH7,  /*!< DMA Channel 7 */
    DMAC_CH8,  /*!< DMA Channel 8 */
    DMAC_CH9,  /*!< DMA Channel 9 */
    DMAC_CH10, /*!< DMA Channel 10 */
    DMAC_CH11, /*!< DMA Channel 11 */
    DMAC_CH12, /*!< DMA Channel 12 */
    DMAC_CH13, /*!< DMA Channel 13 */
    DMAC_CH14, /*!< DMA Channel 14 */
    DMAC_CH15, /*!< DMA Channel 15 */
    DMAC_CH_MAX /*!< Maximum number of DMA Channels */
} DMAC_ch_t;

/***********************************************************
Macro definitions
***********************************************************/
/**
 *  @def DRV_RTDMAC_EXT_NUM_DESC
 *  @brief Number of external descriptor memory entries.
 *  @details Specifies the number of entries in the external descriptor memory.
 */
#define DRV_RTDMAC_EXT_NUM_DESC    128   /* The number of external descriptor memory */

/**
 *  @def DRV_RTDMAC_INT1_NUM_DESC
 *  @brief Number of internal descriptor memory entries (Type 1).
 *  @details Specifies the number of entries in the first type of internal descriptor memory.
 */
#define DRV_RTDMAC_INT1_NUM_DESC   128   /* The number of internal descriptor memory */

/**
 *  @def DRV_RTDMAC_INT2_NUM_DESC
 *  @brief Number of internal descriptor memory entries (Type 2).
 *  @details Specifies the number of entries in the second type of internal descriptor memory.
 */
#define DRV_RTDMAC_INT2_NUM_DESC   128   /* The number of internal descriptor memory */

/**
 *  @def rDmacCH_VALID
 *  @brief Valid channel mask.
 *  @details Specifies the bitmask used to validate DMA channels.
 */
#define rDmacCH_VALID   (0x000000FF00FF0000U)


/***********************************************************
Typedef definitions
***********************************************************/

/**
 *  @brief         Enum representing DMA operation priority mode
 *  @details       Specifies the priority mode for DMA operations. There are two modes:
 *                 - Fixed mode: The priority is fixed and does not change.
 *                 - Round-robin mode: The priority rotates among the channels.
 */
typedef enum e_rDmacPriorityMode {
    DRV_RTDMAC_PRIO_FIX, /*!< Fixed priority mode */
    DRV_RTDMAC_PRIO_RR,  /*!< Round-robin priority mode */
    DRV_RTDMAC_PRIO_MAX  /*!< Maximum number of priority modes */
} rDmacPriorityMode_t;

/**
 *  @brief         Enum representing Descriptor operating mode
 *  @details       Specifies whether Descriptor is enabled or disabled, and its
 *                 operational mode.
 */
typedef enum e_rDmacDmaMode {
    DRV_DMAC_DMA_NO_DESCRIPTOR, /*!< No descriptor mode */
    DRV_DMAC_DMA_DESC_NORMAL,   /*!< Normal descriptor mode */
    DRV_DMAC_DMA_DESC_REPEAT,   /*!< Repeat descriptor mode */
    DRV_DMAC_DMA_DESC_READOUT,  /*!< Readout descriptor mode */
    DRV_DMAC_DMA_DESC_INFINITE, /*!< Infinite descriptor mode */
    DRV_DMAC_DMA_MODE_MAX       /*!< Maximum number of descriptor modes */
} rDmacDmaMode_t;

/**
 *  @brief         Enum representing Source/Destination address mode
 *  @details       Specifies whether the DMA source/destination address is incremented,
 *                 fixed, or decremented, and the amount of increment.
 */
typedef enum e_rDmacAddrMode {
    DRV_RTDMAC_ADDR_FIXED,        /*!< Fixed address mode */
    DRV_RTDMAC_ADDR_INCREMENTED,  /*!< Incremented address mode */
    DRV_RTDMAC_ADDR_DECREMENTED,  /*!< Decremented address mode */
    DRV_RTDMAC_ADDR_MAX           /*!< Maximum number of address modes */
} rDmacAddrMode_t;

/**
 *  @brief         Enum representing Source/Destination transfer unit
 *  @details       Specifies the amount of increment or decrement to the
 *                 source/destination of the DMA.
 */
typedef enum e_rDmacTransferUnit {
    DRV_RTDMAC_TRANS_UNIT_1BYTE,   /*!< Transfer unit of 1 byte */
    DRV_RTDMAC_TRANS_UNIT_2BYTE,   /*!< Transfer unit of 2 bytes */
    DRV_RTDMAC_TRANS_UNIT_4BYTE,   /*!< Transfer unit of 4 bytes */
    DRV_RTDMAC_TRANS_UNIT_8BYTE,   /*!< Transfer unit of 8 bytes */
    DRV_RTDMAC_TRANS_UNIT_16BYTE,  /*!< Transfer unit of 16 bytes */
    DRV_RTDMAC_TRANS_UNIT_32BYTE,  /*!< Transfer unit of 32 bytes */
    DRV_RTDMAC_TRANS_UNIT_64BYTE,  /*!< Transfer unit of 64 bytes */
    DRV_RTDMAC_TRANS_UNIT_MAX      /*!< Maximum number of transfer units */
} rDmacTransferUnit_t;

/**
 *  @brief         Enum representing Resource selection
 *  @details       Specifies resource selection as memory or maximum resource.
 */
typedef enum e_rDmacResource {
    DRV_RTDMAC_MEMORY,        /*!< Memory resource */
    DRV_RTDMAC_RESOUCE_MAX    /*!< Maximum number of resources */
} rDmacResource_t;

/**
 *  @brief         Enum representing DMA Transfer Slow Speed mode
 *  @details       Specifies the number of times by clock (ZS) cycle that DMA
 *                 transfer is kept waiting after single DMA transfer. DMA transfer is
 *                 operated once in multiple clock cycle specified by SLM.
*/
typedef enum e_rDmacLowSpeedMode {
    DRV_RTDMAC_SPEED_NORMAL,    /*!< Normal speed mode */
    DRV_RTDMAC_SPEED_MAX        /*!< Maximum number of speed modes */
} rDmacLowSpeedMode_t;


/**
 *  @brief         DMA Transfer Request ID
 *  @details       Specify DMA transfer request ID for each channel.
 *
 */
typedef enum e_rDmacTransferRequest {
    MID_RID_I2C1_MST_RX = 0x92,   /*!< I2C1 DMARS ID Master RX */
    MID_RID_I2C1_MST_TX = 0x93,   /*!< I2C1 DMARS ID Master TX*/
    MID_RID_I2C2_MST_RX = 0x94,   /*!< I2C2 DMARS ID Master RX */
    MID_RID_I2C2_MST_TX = 0x95,   /*!< I2C2 DMARS ID Master TX*/
    MID_RID_I2C3_MST_RX = 0x96,   /*!< I2C3 DMARS ID Master RX */
    MID_RID_I2C3_MST_TX = 0x97,   /*!< I2C3 DMARS ID Master TX*/
    MID_RID_I2C4_MST_RX = 0x98,   /*!< I2C4 DMARS ID Master RX */
    MID_RID_I2C4_MST_TX = 0x99,   /*!< I2C4 DMARS ID Master TX*/
    MID_RID_I2C5_MST_RX = 0x9A,   /*!< I2C5 DMARS ID Master RX */
    MID_RID_I2C5_MST_TX = 0x9B,   /*!< I2C5 DMARS ID Master TX*/
    MID_RID_I2C6_MST_RX = 0x9C,   /*!< I2C6 DMARS ID Master RX */
    MID_RID_I2C6_MST_TX = 0x9D,   /*!< I2C6 DMARS ID Master TX*/
    MID_RID_I2C7_MST_RX = 0x9E,   /*!< I2C7 DMARS ID Master RX */
    MID_RID_I2C7_MST_TX = 0x9F,   /*!< I2C7 DMARS ID Master TX*/
    MID_RID_I2C8_MST_RX = 0xB0,   /*!< I2C8 DMARS ID Master RX */
    MID_RID_I2C8_MST_TX = 0xB1,   /*!< I2C8 DMARS ID Master TX*/
} rDmacTransferRequest_t;

/**
 *  @struct st_rDmacCfg
 *  @brief         Configuration structure for the DMA controller.
 *  @details       This structure is used to configure the DMA controller.
*/
typedef struct st_rDmacCfg {
    uintptr_t mSrcAddr;                /*!< Source address */
    uintptr_t mDestAddr;               /*!< Destination address */
    uint32_t mTransferCount;           /*!< Transfer count */
    rDmacDmaMode_t mDMAMode;           /*!< DMA mode */
    rDmacAddrMode_t mSrcAddrMode;      /*!< Source address mode */
    rDmacAddrMode_t mDestAddrMode;     /*!< Destination address mode */
    rDmacTransferUnit_t mTransferUnit; /*!< Transfer unit */
    rDmacResource_t mResource;         /*!< Resource */
    rDmacLowSpeedMode_t mLowSpeed;     /*!< Low speed mode */
    rDmacTransferRequest_t mSourceRequest;	/*!< Source request */
    uint8_t mPrioLevel;                /*!< Priority level */
} rDmacCfg_t;

/**
 *  @struct st_rDmacDescUpdate
 *  @brief         Update DMA descriptor
 *  @details       This structure is used to update the DMA descriptor.
 */
typedef struct st_rDmacDescUpdate {
    bool mSrcAddrUpdate;    /*!< Update source address */
    bool mDestAddrUpdate;   /*!< Update destination address */
    bool mTransCountUpdate; /*!< Update transfer count */
    bool mCHCRUpdate;       /*!< Update CHCR register */
} rDmacDescUpdate_t;

/**
 * @brief Control structure for Dmac operations.
 */
typedef void dmac_ctrl_t;

/**
 *  @struct st_rDmacDescCfg
 *  @brief         Config DMA descriptor
 *  @details    This structure is used to configure the DMA descriptor.
 */
typedef struct st_rDmacDescCfg {
    uintptr_t mDescBaseAddr;    /*!< Descriptor base address */
    rDmacDescUpdate_t mDescUpdate;  /*!< Update descriptor */
    bool mDescRead1st;        /*!< Descriptor read 1st */
    bool mStateEndEnable;   /*!< State end enable */
    uint8_t mDescCount;     /*!< Descriptor count */
    uint8_t mDescIndex;    /*!< Descriptor index */
} rDmacDescCfg_t;

/**
 *  @struct st_DmacIrqHandler
 *  @brief  Handle structure for DMAC.
 *  @details This structure is used to manage the DMAC Irq handle.
 */
typedef struct st_rDmacIrqCfg {
    void                (* p_callback)(void *);    /*!<  Pointer to the callback function */
    void                *p_context;                /*!<  Pointer to context to be passed into callback */
    uint8_t             Unit;      /*!< Unit number */
    uint8_t             SubCh;     /*!< Sub-channel number */
    uint16_t            irq_channel; /*!< IRQ channel */
    rDmacCfg_t          *p_dma_cfg;
} rDmacIrqCfg_t;

/**
 *  @struct     st_rDmacDescMemCfg
 *  @brief      DMA Descriptor Memory Configuration:
 *  @details    This struct is used to allocate and define memory for DMA descriptors.
 *              The structure is 16-byte aligned to meet the hardware requirements
 *              of the DMA descriptor.
 */
typedef struct st_rDmacDescMemCfg {
        uint32_t SAR;   /**< Source Address */
        uint32_t DAR;   /**< Destination Address */
        uint32_t TCR;   /**< Transfer Count */
        uint32_t CHCR;  /**< Channel Control Register */
} __attribute__((aligned(16))) rDmacDescMemCfg_t;

#ifdef __cplusplus
}
#endif

/** @} */ // end of RTDMAC_Module

#endif  /* RTDMAC_COMMON_H */

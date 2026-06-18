/*
 * Copyright (c) 2025 Renesas Electronics Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#ifndef RDMAC_REG_H
#define RDMAC_REG_H

/* Definition of RTDMAC address */
#define DRV_RTDMAC_REG_ADDR_DMAC0           (0x18900000U)      /* RTDMAC0 */
#define DRV_RTDMAC_REG_ADDR_DMAC1           (0x18910000U)      /* RTDMAC1 */
#define DRV_RTDMAC_REG_ADDR_DMAC2           (0x18920000U)      /* RTDMAC2 */
#define DRV_RTDMAC_REG_ADDR_DMAC3           (0x18930000U)      /* RTDMAC3 */

#define DRV_RTDMAC_REG_ADDR_DMACP0          (0x19438000U)      /* RTDMACP0 */
#define DRV_RTDMAC_REG_ADDR_DMACP1          (0x19439000U)      /* RTDMACP1 */
#define DRV_RTDMAC_REG_ADDR_DMACP2          (0x1943A000U)      /* RTDMACP2 */
#define DRV_RTDMAC_REG_ADDR_DMACP3          (0x1943B000U)      /* RTDMACP3 */

#ifdef CONFIG_USE_RCARSOC_DRV_DMAC
#define DRV_ADMAC_REG_ADDR_DMAC0 (0xEC700000U) /* ADMAC0 */
#define DRV_ADMAC_REG_ADDR_DMAC1 (0xEC710000U) /* ADMAC1 */
#define DRV_ADMAC_REG_ADDR_DMAC2 (0xED700000U) /* ADMAC2 */
#define DRV_ADMAC_REG_ADDR_DMAC3 (0xED710000U) /* ADMAC3 */

#define DRV_ADMAC_REG_ADDR_DMACP0 (0xEC720000U) /* ADMACP0 */
#define DRV_ADMAC_REG_ADDR_DMACP1 (0xEC721000U) /* ADMACP1 */
#define DRV_ADMAC_REG_ADDR_DMACP2 (0xED720000U) /* ADMACP2 */
#define DRV_ADMAC_REG_ADDR_DMACP3 (0xED721000U) /* ADMACP3 */

#define DRV_SCPDMAC_REG_ADDR_DMAC0 (0xC11A0000U) /* SCPDMAC0 */
#define DRV_SCPDMAC_REG_ADDR_DMAC1 (0xC11B0000U) /* SCPDMAC1 */

#define DRV_SCPDMAC_REG_ADDR_DMACP0 (0xC11E8000U) /* SCPDMACP0 */
#define DRV_SCPDMAC_REG_ADDR_DMACP1 (0xC11E9000U) /* SCPDMACP1 */
#endif /* CONFIG_USE_RCARSOC_DRV_DMAC */


/* Definition of SYSDMAC address */
#define DRV_SYSDMAC_REG_ADDR_DMAC0			(0xC0900000U)      /* SYSDMAC0 */
#define DRV_SYSDMAC_REG_ADDR_DMAC1			(0xC0480000U)      /* SYSDMAC1 */
#define DRV_SYSDMAC_REG_ADDR_DMAC2			(0xC0490000U)      /* SYSDMAC2 */
#define DRV_SYSDMAC_REG_ADDR_DMAC3			(0xC04A0000U)      /* SYSDMAC3 */

#define DRV_SYSDMAC_REG_ADDR_DMACP0			(0xC0940000U)      /* SYSDMACP0 */
#define DRV_SYSDMAC_REG_ADDR_DMACP1			(0xC05C4000U)      /* SYSDMACP1 */
#define DRV_SYSDMAC_REG_ADDR_DMACP2			(0xC05C5000U)      /* SYSDMACP2 */
#define DRV_SYSDMAC_REG_ADDR_DMACP3			(0xC05C6000U)      /* SYSDMACP3 */

#define DRV_RTDMAC_REG_MASK_CH0             (0x00000001U)      /* CH0 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH1             (0x00000002U)      /* CH1 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH2             (0x00000004U)      /* CH2 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH3             (0x00000008U)      /* CH3 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH4             (0x00000010U)      /* CH4 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH5             (0x00000020U)      /* CH5 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH6             (0x00000040U)      /* CH6 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH7             (0x00000080U)      /* CH7 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH8             (0x00000100U)      /* CH8 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH9             (0x00000200U)      /* CH9 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH10            (0x00000400U)      /* CH10 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH11            (0x00000800U)      /* CH11 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH12            (0x00001000U)      /* CH12 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH13            (0x00002000U)      /* CH13 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH14            (0x00004000U)      /* CH14 use bit mask */
#define DRV_RTDMAC_REG_MASK_CH15            (0x00008000U)      /* CH15 use bit mask */

#define DRV_RTDMAC_CH_REG_ADDR              (0x00001000U)      /* CH_REG */
#define DRV_RTDMAC_CH_REG_SIZE              (0x00000080U)      /* CH_REG */

#define DRV_RTDMAC_REG_MASK_LOWER_RDMSAR    (0xFFFFFFFFU)      /* RDMSAR use bit lower mask */
#define DRV_RTDMAC_REG_MASK_UPPER_RDMSAR    (0x000007FFU)      /* RDMSAR use bit upper mask */
#define DRV_RTDMAC_REG_MASK_LOWER_RDMDAR    (0xFFFFFFFFU)      /* RDMDAR use bit lower mask */
#define DRV_RTDMAC_REG_MASK_UPPER_RDMDAR    (0x000007FFU)      /* RDMDAR use bit upper mask */
#define DRV_RTDMAC_REG_MASK_LOWER_RDMDPBASE (0xFFFFFFF0U)      /* RDMDPBASE use bit lower mask */
#define DRV_RTDMAC_REG_MASK_UPPER_RDMDPBASE (0x000007FFU)      /* RDMDPBASE use bit upper mask */
#define DRV_RTDMAC_REG_MASK_RDMCHCR_TS      (0x00000003U)      /* RDMCHCR use bit TS */
#define DRV_RTDMAC_REG_MASK_ADDR            (0x000000FFFFFFFFFFU)      /* ADDRESS(40Bit) mask */
#define DRV_RTDMAC_REG_MASK_CHECK_ADDR      (0x000000FFFFFFF000U)      /* Check ADDRESS(40Bit) mask */
#define DRV_RTDMAC_REG_MASK_COUNT           (0x00FFFFFFU)      /* COUNT(24Bit) mask */

#define DRV_RTDMAC_REG_RDMOR_DME            (0x00000001U)
#define DRV_RTDMAC_REG_RDMOR_AE             (0x00000004U)
#define DRV_RTDMAC_REG_RDMCHCR_CAE          (0x80000000U)
#define DRV_RTDMAC_REG_RDMCHCR_DSE          (0x00080000U)
#define DRV_RTDMAC_REG_RDMCHCR_DSIE         (0x00040000U)
#define DRV_RTDMAC_REG_RDMCHCR_TE           (0x00000002U)
#define DRV_RTDMAC_REG_RDMCHCR_DE           (0x00000001U)
#define DRV_RTDMAC_REG_RDMCHCR_DPM          (0x30000000U)
#define DRV_RTDMAC_REG_RDMCHCR_DPM_REP      (0x20000000U)
#define DRV_RTDMAC_REG_RDMCHCR_DPM_READ     (0x30000000U)

#define DRV_RTDMAC_REG_PRIO_FIX             0U
#define DRV_RTDMAC_REG_PRIO_RR              3U

#define DRV_RTDMAC_REG_TRANS_UNIT_1BYTE     0U
#define DRV_RTDMAC_REG_TRANS_UNIT_2BYTE     1U
#define DRV_RTDMAC_REG_TRANS_UNIT_4BYTE     2U
#define DRV_RTDMAC_REG_TRANS_UNIT_8BYTE     7U
#define DRV_RTDMAC_REG_TRANS_UNIT_16BYTE    3U
#define DRV_RTDMAC_REG_TRANS_UNIT_32BYTE    4U
#define DRV_RTDMAC_REG_TRANS_UNIT_64BYTE    5U

#define DRV_RTDMAC_RDMOR_DME_ENABLE         1U
#define DRV_RTDMAC_RDMOR_DME_DISABLE        0U
#define DRV_RTDMAC_RDMOR_AE_CLEAR           0U
#define DRV_RTDMAC_RDMCHCR_RS_AUTO_REQEST       4U
#define DRV_RTDMAC_RDMCHCR_RS_EXTENDED_RESOUSE  8U
#define DRV_RTDMAC_RDMCHCR_CAIE_ENABLE      1U
#define DRV_RTDMAC_RDMCHCR_CAIE_DISABLE     0U
#define DRV_RTDMAC_RDMCHCR_IE_ENABLE        1U
#define DRV_RTDMAC_RDMCHCR_IE_DISABLE       0U
#define DRV_RTDMAC_RDMCHCR_DE_ENABLE        1U
#define DRV_RTDMAC_RDMCHCR_DE_DISABLE       0U
#define DRV_RTDMAC_RDMCHCR_CAE_CLEAR        0U
#define DRV_RTDMAC_RDMCHCR_DSE_CLEAR        0U
#define DRV_RTDMAC_RDMCHCR_TE_CLEAR         0U

#define DRV_RTDMAC_PRIORITY_MAX             16U

#define DRV_RTDMAC_MAX_CH                   8U

#define DRV_RTDMAC_CLR_DE_RETRY             5U

enum REGISTER {
    DRV_RTDMAC_REG0,
    DRV_RTDMAC_REG1,
    DRV_RTDMAC_UNIT_NUM = 32
};

/* Structure of register access definitions */
/* Structure of RDMISTA */
typedef union u_regRDMISTA {
    struct {
        uint32_t    I:          1;     /* R Interrupt state of channel */
        uint32_t    reserve_1:  15;    /* R Reserved */
        uint32_t    OV:         1;     /* R Event counter overflow status of channel */
        uint32_t    reserve_2:  15;    /* R Reserved */
    } bit;
    uint32_t INT;
} regRDMISTA_t;

/* Structure of RDMOR    */
typedef union u_regRDMOR {
    struct {
        uint32_t    DME:        1;  /* R/W    DMA Master Enable */
        uint32_t    reserve_1:  1;  /* R      Reserved */
        uint32_t    AE:         1;  /* R/(W)* Address Error Flag */
        uint32_t    reserve_2:  5;  /* R      Reserved */
        uint32_t    PR:         2;  /* R/W    Priority Mode */
        uint32_t    reserve_3:  6;  /* R      Reserved */
    } bit;
    uint16_t INT;
} regRDMOR_t;

/* Structure of RDMCHCLR    */
typedef union u_regRDMCHCLR {
    struct {
        uint32_t    CLR:        1;   /* W Writing to a bit leads to clearing of all registers for the corresponding */
        uint32_t    reserve_1:  31;  /* R Reserved */
    } bit;
    uint32_t INT;
} regRDMCHCLR_t;


/* Structure of DMDPSEC    */
typedef union u_regRDMDPSEC {
    struct {
        uint32_t    SM:         9;   /* R/W Security Attribute Setting for Base Address Mask of Descriptor */
        uint32_t    reserve_1:  7;   /* R   Reserved */
        uint32_t    SA:         9;   /* R/W Security Attribute Setting for Base Address of Descriptor Memory */
        uint32_t    reserve_2:  6;   /* R   Reserved */
        uint32_t    SEC:        1;   /* R/W Security Attribute Setting for Descriptor Memory */
    } bit;
    uint32_t INT;
} regRDMDPSEC_t;

/* Structure of RDMSAR_0 to RDMSAR_32    */
typedef union u_regRDMSAR {
    struct {
        uint32_t    SAR:      32;    /* R/W DMA Source Address */
    } bit;
    uint32_t INT;
} regRDMSAR_t;

/* Structure of RDMDAR_0 to RDMDAR_32    */
typedef union u_regRDMDAR {
    struct {
        uint32_t    DAR:      32;    /* R/W DMA Destination Address */
    } bit;
    uint32_t INT;
} regRDMDAR_t;

/* Structure of RDMTCR_0 to RDMTCR_32    */
typedef union u_regRDMTCR {
    struct {
        uint32_t    TCR:        24;   /* R/W DMA Transfer Size */
        uint32_t    reserve_1:  8;    /* R Reserved */
    } bit;
    uint32_t INT;
} regRDMTCR_t;

/* Structure of RDMCHCR_0 to RDMCHCR_32    */
typedef union u_regRDMCHCR {
    struct {
        uint32_t    DE:         1;   /* R/W    DMA Enable */
        uint32_t    TE:         1;   /* R/(W)* Transfer End Flag */
        uint32_t    IE:         1;   /* R/W    Interrupt Enable */
        uint32_t    TS01:       2;   /* R/W    DMA Transfer Size */
        uint32_t    reserve_1:  3;   /* R      Reserved */
        uint32_t    RS:         4;   /* R/W    Resource Selection */
        uint32_t    SM:         2;   /* R/W    Source Address Mode */
        uint32_t    DM:         2;   /* R/W    Destination Address Mode */
        uint32_t    reserve_2:  2;   /* R      Reserved */
        uint32_t    DSIE:       1;   /* R/W    Descriptor Stage End Interrupt Enable */
        uint32_t    DSE:        1;   /* R/(W)* Descriptor Stage End */
        uint32_t    TS23:       2;   /* R/W    DMA Transfer Size */
        uint32_t    DPB:        1;   /* R/W    Descriptor Start */
        uint32_t    reserve_3:  1;   /* R      Reserved */
        uint32_t    RPT0:       1;   /* R/W    Descriptor Setting Update(RPT[0]) */
        uint32_t    RPT1:       1;   /* R/W    Descriptor Setting Update(RPT[1]) */
        uint32_t    RPT2:       1;   /* R/W    Descriptor Setting Update(RPT[2]) */
        uint32_t    RPT3:       1;   /* R/W    Descriptor Setting Update(RPT[2]) */
        uint32_t    DPM:        2;   /* R/W    Operating Mode of Descriptor Memory */
        uint32_t    CAIE:       1;   /* R/W    Channel Address Error Interrupt Enable */
        uint32_t    CAE:        1;   /* R/(W)* Channel Address Error Flag */
    } bit;
    uint32_t INT;
} regRDMCHCR_t;

/* Structure of RDMTCRB_0 to RDMTCRB_32    */
typedef union u_regRDMTCRB {
    struct {
        uint32_t    TCR:          24; /* R/W DMA Transfer Count B */
        uint32_t    reserve_1:    8;  /* R   Reserved */
    } bit;
    uint32_t INT;
} regRDMTCRB_t;

/* Structure of RDMTSRB_0 to RDMTSRB_32    */
typedef union u_regRDMTSRB {
    struct {
        uint32_t    TSR:      32;     /* R/W DMA Transfer Size */
    } bit;
    uint32_t INT;
} regRDMTSRB_t;

/* Structure of RDMCHCRB_0 to RDMCHCRB_32    */
typedef union u_regRDMCHCRB {
    struct {
        uint32_t    PRI:        4;    /* R/W Channel Request Priority Setting */
        uint32_t    SLM:        4;    /* R/W DMA Transfer Low-Speed Mode */
        uint32_t    DTS:        1;    /* R/W Total Size Transmission under Descriptor Control */
        uint32_t    reserve_1:  1;    /* R   Reserved */
        uint32_t    DSIEEN:     1;    /* R/W Descriptor DSIE Enable */
        uint32_t    reserve_2:  3;    /* R   Reserved */
        uint32_t    DREQOSEN:   1;    /* R/W DREQ Outstanding Enable */
        uint32_t    DRST:       1;    /* W   Descriptor Reset */
        uint32_t    DPTR:       8;    /* R   Descriptor Pointer */
        uint32_t    DCNT:       8;    /* R/W Number of Stages of Descriptor Memory */
    } bit;
    uint32_t INT;
} regRDMCHCRB_t;

/* Structure of RDMRS_0 to RDMRS_32    */
typedef union u_regRDMRS {
    struct {
        uint32_t    RID:        2;    /* R/W DMA Request Source Adoption RID[1] and RID[0] (RID) */
        uint32_t    MID:        6;    /* R/W DMA Request Source Adoption MID[5] to MID[0] (MID) */
        uint32_t    reserve_1:  8;    /* R   Reserved */
    } bit;
    uint16_t INT;
} regRDMRS_t;

/* Structure of RDMBUFCR_0 to RDMBUFCR_32    */
typedef union u_regRDMBUFCR {
    struct {
        uint32_t    ULB:        10;   /* R/W Upper Limit on Buffer Size */
        uint32_t    reserve_1:  6;    /* R   Reserved */
        uint32_t    MBU:        9;    /* R/W Maximum Burst dev for SDRAM */
        uint32_t    reserve_2:  7;    /* R   Reserved */
    } bit;
    uint32_t INT;
} regRDMBUFCR_t;

/* Structure of RDMDPBASE_0 to RDMDPBASE_32    */
typedef union u_regRDMDPBASE {
    struct {
        uint32_t    SEL:        1;    /* R/W */
        uint32_t    reserve_1:  3;    /* R   Reserved */
        uint32_t    DPBASE:     28;   /* R/W Base Address of Descriptor Memory */
    } bit;
    uint32_t INT;
} regRDMDPBASE_t;

/* Structure of RDMDPCR_0 to RDMDPCR_32    */
typedef union u_regRDMDPCR {
    struct {
        uint32_t    DPTR_11_8:  4;    /* R DMA Descriptor Control */
        uint32_t    reserve_1:  4;    /* R   Reserved */
        uint32_t    DNCT_11_8:  4;    /* R/W DMA Descriptor Control */
        uint32_t    reserve_2:  4;    /* R   Reserved */
        uint32_t    DIPT_11_8:  4;    /* R/W DMA Descriptor Control */
        uint32_t    reserve_3:  4;    /* R   Reserved */
        uint32_t    DIPT_7_0:   8;    /* R/W DMA Descriptor Control */
    } bit;
    uint32_t INT;
} regRDMDPCR_t;

/* Structure of RDMFIXSAR_0 to RDMFIXSAR_32    */
typedef union u_regRDMFIXSAR {
    struct {
        uint32_t    SAR:        11;    /* R/W DMA Fixed Source Address */
        uint32_t    reserve_1:  21;    /* R   Reserved */
    } bit;
    uint32_t INT;
} regRDMFIXSAR_t;

/* Structure of RDMFIXDAR_0 to RDMFIXDAR_32    */
typedef union u_regRDMFIXDAR {
    struct {
        uint32_t    DAR:        11;    /* R/W DMA Fixed Destination Address */
        uint32_t    reserve_1:  21;    /* R   Reserved */
    } bit;
    uint32_t INT;
} regRDMFIXDAR_t;

/* Structure of RDMFIXDPBASE_0 to RDMFIXDPBASE_32    */
typedef union u_regRDMFIXDPBASE {
    struct {
        uint32_t    DPBASE:     11;     /* R/W DMA Fixed Descriptor Base Address */
        uint32_t    reserve_1:  21;     /* R   Reserved */
    } bit;
    uint32_t INT;
} regRDMFIXDPBASE_t;

/* Structure of RDMSES    */
typedef union u_regRDMSES {
    struct {
        uint32_t    Error_1:      1;    /* R/W* Error status of channel 31 to 16 */
        uint32_t    reserve_1:    31;   /* R    Reserved */
    } bit;
    uint32_t INT;
} regRDMSES_t;

/* Structure of RDMSEMID    */
typedef union u_regRDMSEMID {
    struct {
        uint32_t    EID:      32;        /* R First error ID when access to channel 16 to 31 was denied. */
    } bit;
    uint32_t INT;
} regRDMSEMID_t;

/* Structure of FDSDM_CONTROL_0 to FDSDM_CONTROL_2    */
typedef union u_rDmacFDSDM_CONTROL {
    struct {
        uint32_t    D_EN:       1;       /* R/W */
        uint32_t    A_EN:       1;       /* R/W */
        uint32_t    reserve_1:  22;      /* R   Reserved */
        uint32_t    DMA_CH1:    4;       /* R/W */
        uint32_t    DMA_CH2:    4;       /* R/W */
    } bit;
    uint32_t INT;
} rDmacFDSDM_CONTROL_t;

/* Structure of FDSDM_STATUS    */
typedef union u_rDmacFDSDM_STATUS {
    struct {
        uint32_t    ST0:        1;       /* R/W */
        uint32_t    ST1:        1;       /* R/W */
        uint32_t    ST2:        1;       /* R/W */
        uint32_t    reserve_1:  29;      /* R   Reserved */
    } bit;
    uint32_t INT;
} rDmacFDSDM_STATUS_t;

#define RDMISTA            0x00000110U
#define RDMSEC             0x000000B0U
#define RDMOR              0x00000060U
#define RDMCHCLR           0x00000100U
#define FDSDM_CONTROL_0    0x000000D0U
#define FDSDM_CONTROL_1    0x000000D4U
#define FDSDM_CONTROL_2    0x000000D8U
#define FDSDM_STATUS       0x000000E0U

#define RDMSAR             0x00000000U
#define RDMDAR             0x00000004U
#define RDMTCR             0x00000008U
#define RDMCHCR            0x0000000CU
#define RDMFIXSAR          0x00000010U
#define RDMFIXDAR          0x00000014U
#define RDMTCRB            0x00000018U
#define RDMCHCRB           0x0000001CU
#define RDMSAR2            0x00000020U
#define RDMDAR2            0x00000024U
#define RDMTSR             0x00000028U
#define RDMCHCR2           0x0000002CU
#define RDMTSRB            0x00000038U
#define RDMRS              0x00000040U
#define RDMBUFCR           0x00000048U
#define RDMDPBASE          0x00000050U
#define RDMDPCR            0x00000054U
#define RDMFIXDPBASE       0x00000060U

extern const uint32_t rDmacCH_MaskTable[];

extern uint32_t R_RTDMAC_Get_RDMOR(uint8_t dev);
extern uint32_t R_RTDMAC_Get_RDMCHCR(uint8_t dev,uint8_t ch);
extern uint32_t R_RTDMAC_Get_RDMDPCR(uint8_t dev,uint8_t ch);

extern void R_RTDMAC_Update_RDMOR(uint8_t dev, rDmacPriorityMode_t mode);
extern void R_RTDMAC_Update_RDMCHCLR(uint8_t dev,uint8_t ch);
extern void R_RTDMAC_Update_RDMSAR(uint8_t dev,uint8_t ch, uint64_t Value);
extern void R_RTDMAC_Update_RDMDAR(uint8_t dev,uint8_t ch, uint64_t Value);
extern void R_RTDMAC_Update_RDMTCR(uint8_t dev,uint8_t ch, uint32_t Value);
extern void R_RTDMAC_Update_RDMCHCR(uint8_t dev,uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);
extern uint16_t R_RTDMAC_Clear_RDMCHCR_DE(uint8_t dev,uint8_t ch);
extern void R_RTDMAC_Clear_RDMCHCR_CAE(uint8_t dev,uint8_t ch);
extern void R_RTDMAC_Clear_RDMCHCR_DSE(uint8_t dev,uint8_t ch);
extern void R_RTDMAC_Clear_RDMCHCR_TE(uint8_t dev,uint8_t ch);
extern void R_RTDMAC_Update_RDMCHCRB(uint8_t dev,uint8_t ch, rDmacCfg_t *cfg, rDmacDescCfg_t *descCfg);
extern void R_RTDMAC_Update_RDMCHCRB_RST(uint8_t dev, uint8_t ch);
extern void R_RTDMAC_Update_RDMRS_ID(uint8_t dev,uint8_t ch, uint8_t Value);
extern void R_RTDMAC_Update_RDMDPBASE(uint8_t dev,uint8_t ch, uint64_t Value);
extern void R_RTDMAC_Update_RDMDPCR(uint8_t dev,uint8_t ch, uint8_t Value);

#endif  /* RDMAC_REG_H */

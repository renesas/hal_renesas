#ifndef R_PSCU_REG_H
#define R_PSCU_REG_H

/* =========================================================================================================================== */
/* ================                                          R_PSCU                                           ================ */
/* =========================================================================================================================== */

/**
 * @brief Peripheral Security Control Unit (R_PSCU)
 */

typedef struct                         /*!< (@ 0x8780_4000) R_PSCU Structure                                           */
{
    __IM uint32_t RESERVED[14];        /*!< To update by BSP                                                            */
    union
    {
        __IM uint32_t DLMMON;          /*!< (@ 0x00000038) Device Lifecycle Management State Monitor Register          */

        struct
        {
            __IM uint32_t DLMMON : 4;  /*!< [3..0] Device Lifecycle Management State Monitor                          */
            uint32_t             : 28;
        } DLMMON_b;
    };
} R_PSCU_Type;

#endif                                 /* R_PSCU_REG_H */

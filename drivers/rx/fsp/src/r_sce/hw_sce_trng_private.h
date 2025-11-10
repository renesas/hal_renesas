/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef HW_SCE_TRNG_PRIVATE_H
#define HW_SCE_TRNG_PRIVATE_H

/**********************************************************************************************************************
 * Includes
 ***********************************************************************************************************************/
#include "bsp_api.h"
#include "hw_sce_common.h"

// TRNG

/**
 * @brief Generate 128-bit random number using SCE HW TRNG
 * @param[out] OutData_Text 128-bit random number will be stored in this buffer
 */
fsp_err_t HW_SCE_RNG_Read(uint32_t * OutData_Text);

#endif                                 /* HW_SCE_TRNG_PRIVATE_H */

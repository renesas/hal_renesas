/* ${REA_DISCLAIMER_PLACEHOLDER} */

#include "hw_sce_rx_private.h"

uint32_t change_endian_long (uint32_t data)
{
    return __REV(data);
}

/* ${REA_DISCLAIMER_PLACEHOLDER} */

#include "hw_sce_rx_private.h"

void HW_SCE_SoftwareResetSub (void)
{
    WR1_PROG(REG_140CH, 0x00000000U);
    WR1_PROG(REG_1458H, 0x00000000U);
    WR1_PROG(REG_1458H, 0x00000000U);
    WR1_PROG(REG_1458H, 0x00000000U);
}

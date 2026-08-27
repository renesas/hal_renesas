/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

#include <stdint.h>
#include <stddef.h>

#if defined(CONFIG_RENESAS_RCAR_MP_PHY)
const uint8_t mpphy_firmware[] = {
	#include <rcar_gen5_mp_phy.bin.inc>
};

const size_t mpphy_firmware_size = sizeof(mpphy_firmware);
#endif

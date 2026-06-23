# SPDX-License-Identifier: Apache-2.0

add_subdirectory(rh850_cfg)
add_subdirectory(rh850-core)
zephyr_include_directories(portable)
zephyr_include_directories(portable/inc)


zephyr_library_sources_ifdef(CONFIG_USE_RH850_FSP_CRC
	portable/src/rp_crc/rp_crc.c)


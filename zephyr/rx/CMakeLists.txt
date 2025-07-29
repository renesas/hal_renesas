# SPDX-License-Identifier: Apache-2.0

add_subdirectory(rdp_cfg)
zephyr_include_directories(portable)
add_subdirectory(fsp_cfg)

zephyr_include_directories(
    portable
    portable/inc)

zephyr_include_directories(
    fsp_cfg)
zephyr_include_directories(
    portable)

# Optional build base on feature configuration
zephyr_library_sources_ifdef(CONFIG_USE_RX_FSP_RMAC
    portable/src/rp_rmac/rp_rmac.c)

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

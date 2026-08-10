# SPDX-License-Identifier: Apache-2.0

if(CONFIG_SOC_R8A7800_R52)
    zephyr_library_compile_definitions(BOARD=X5H_IRONHIDE)
endif()

zephyr_include_directories(
    portable/inc
    portable/inc/x5h)
zephyr_library_compile_options(-w)

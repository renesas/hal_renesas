# SPDX-License-Identifier: Apache-2.0

zephyr_library_property(ALLOW_EMPTY TRUE)

if(CONFIG_SOC_R8A78000_R52)
    zephyr_library_compile_definitions(BOARD=X5H_IRONHIDE)
endif()

zephyr_include_directories(
    portable/inc
    portable/inc/x5h
    portable/inc/mfis)
zephyr_library_compile_options(-w)

zephyr_library_sources_ifdef(CONFIG_USE_RCARSOC_DRV_MFIS
    portable/src/rp_mfis_mbox/rp_mfis_mbox.c
    portable/src/rp_mfis_mbox/rp_mfis_scp_mbox.c)

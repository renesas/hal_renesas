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

# ---------------------------------------------------------------------------
# RP PHY Q32XX private library
# ---------------------------------------------------------------------------
if(CONFIG_PHY_RENESAS_Q32XX)

set(RP_PHY_Q32XX_DIR
    ${CMAKE_CURRENT_LIST_DIR}/portable/src/rp_phy_q32xx
)

set(RP_PHY_Q32XX_SRC
    ${RP_PHY_Q32XX_DIR}/rp_phy_q32xx.c
)

set(RP_PHY_Q32XX_PREBUILT_LIB
    ${RP_PHY_Q32XX_DIR}/librp_phy_q32xx.a
)

# Choose the private archive library when it is provided in the same directory as the
# source file. If it is not present, build the PHY implementation from source.
if(EXISTS "${RP_PHY_Q32XX_PREBUILT_LIB}")

    message(STATUS "Using prebuilt RP PHY Q32XX library: ${RP_PHY_Q32XX_PREBUILT_LIB}")

    add_library(rp_phy_q32xx STATIC IMPORTED GLOBAL)
    set_target_properties(rp_phy_q32xx PROPERTIES
        IMPORTED_LOCATION "${RP_PHY_Q32XX_PREBUILT_LIB}"
    )

    target_link_libraries(app PRIVATE rp_phy_q32xx)

elseif(EXISTS "${RP_PHY_Q32XX_SRC}")

    message(STATUS "Building RP PHY Q32XX library from source: ${RP_PHY_Q32XX_SRC}")

    add_library(rp_phy_q32xx STATIC
        ${RP_PHY_Q32XX_SRC}
    )
    add_dependencies(rp_phy_q32xx zephyr_generated_headers)

    target_include_directories(rp_phy_q32xx PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/portable
        ${CMAKE_CURRENT_LIST_DIR}/portable/inc
        ${CMAKE_CURRENT_LIST_DIR}/portable/inc/CMSIS_5
    )

    target_link_libraries(rp_phy_q32xx PRIVATE
        zephyr_interface
    )

    target_compile_options(rp_phy_q32xx PRIVATE
        -w
    )

    target_link_libraries(app PRIVATE rp_phy_q32xx)

else()

    message(FATAL_ERROR
        "RP PHY Q32XX library not found. Provide ${RP_PHY_Q32XX_PREBUILT_LIB} "
        "or ${RP_PHY_Q32XX_SRC}."
    )

endif()

endif() # CONFIG_PHY_RENESAS_Q32XX

if(CONFIG_RENESAS_RCAR_MP_PHY)
	generate_inc_file_for_target(
		${ZEPHYR_CURRENT_LIBRARY}
		portable/assets/gen5/rcar_gen5_mp_phy.bin
		${ZEPHYR_BINARY_DIR}/include/generated/rcar_gen5_mp_phy.bin.inc
	)
	zephyr_library_sources(${CMAKE_CURRENT_LIST_DIR}/portable/src/mpphy_firmware.c)
endif()

/* ${REA_DISCLAIMER_PLACEHOLDER} */

#ifndef BSP_CLOCK_CFG_H
#define BSP_CLOCK_CFG_H

/***********************************************************************************************************************
 * Choose pin configuration file for current build
 **********************************************************************************************************************/
#if defined(RX26T_MCB_BUILD)
 #include "bsp_clock_cfg_rx26t.h"
#elif defined(RX72N_RSK_BUILD) || defined(RX72N_EK_BUILD)
 #include "bsp_clock_cfg_rx72n.h"
#elif defined(RX74M_EK_BUILD)
 #include "bsp_clock_cfg_rx74m.h"
#endif

/** The HOCO can operate at several different frequencies. Choose which one using the macro below. The frequency
 * used out of reset depends upon the OFS1.HOCOFRQ0 bits.
 *
 * <PRE>
 * Available frequency settings:
 * 0 = 16MHz
 * 1 = 18MHz
 * 2 = 20MHz
 * </PRE>
 *
 * @note Setting a value of 3 is prohibited
 */
#ifndef BSP_CFG_HOCO_FREQUENCY
 #define BSP_CFG_HOCO_FREQUENCY       (2)
#endif

/** PLL clock source (PLLSRCSEL). Choose which clock source to input to the PLL circuit.
 *
 * <PRE>
 * Available clock sources:     -   Enumeration to use for macro
 * 0 = Main clock (default)     -   BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC
 * 1 = HOCO                     -   BSP_CLOCKS_SOURCE_CLOCK_HOCO
 * </PRE>
 */
#ifndef BSP_CFG_PLL_SOURCE
 #define BSP_CFG_PLL_SOURCE           (BSP_CLOCKS_SOURCE_CLOCK_HOCO)
#endif

/** PLL Input Frequency Division Ratio Select (PLIDIV).
 *
 * Available divisors = - /1 (no division), - /2, - /3
 *
 * @note Set macro definition to 'BSP_CLOCKS_PLL_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PLL_DIV
 #define BSP_CFG_PLL_DIV              (BSP_CLOCKS_PLL_DIV_1)
#endif

/** PLL Frequency Multiplication Factor Select (PLLSTC).
 *
 * Available multipliers = x10.0 to x30.0 in 0.5 increments (e.g. 10.0, 10.5, 11.0, 11.5, ..., 29.0, 29.5, 30.0)
 */
#ifndef BSP_CFG_PLL_STC
 #define BSP_CFG_PLL_STC              (BSP_CLOCKS_PLL_STC_12_0)
#endif

/** PPLL Input Pulse Frequency Division Ratio Select (PPLIDIV).
 *
 * Available divisors = - /1 (no division), - /2, - /3
 *
 * @note Set macro definition to 'BSP_CLOCKS_INPUT_PPLL_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PPLL_INPUT_DIV
 #define BSP_CFG_PPLL_INPUT_DIV       (BSP_CLOCKS_PPLL_INPUT_DIV_1)
#endif

/** PPLL Frequency Multiplication Factor Select (PPLLSTC).
 *
 * Available multipliers = x10.0 to x30.0 in 0.5 increments (e.g. 10.0, 10.5, 11.0, 11.5, ..., 29.0, 29.5, 30.0)
 */
#ifndef BSP_CFG_PPLL_STC
 #define BSP_CFG_PPLL_STC             (BSP_CLOCKS_PPLL_STC_10_0)
#endif

/** PPLL Frequency-Divided Clock Select (PPLCK).
 *
 * Available divisors = - /2 , - /3 , - /4 , - /5 ,
 *
 * @note Set macro definition to 'BSP_CLOCKS_PPLL_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PPLL_DIV
 #define BSP_CFG_PPLL_DIV             (BSP_CLOCKS_PPLL_DIV_4)
#endif

/** Clock source select (CKSEL). The chosen clock will be the base clock provided for the system clock and all
 * peripheral clocks. It is also used for the flash clock and the external bus clocks.
 *
 * Set the macro to one of the enumerations shown.
 *
 * <PRE>
 * Clock                                        -   Enumeration to use for macro
 * High Speed On-Chip Oscillator   (HOCO)       -   BSP_CLOCKS_SOURCE_CLOCK_HOCO
 * Low Speed On-Chip Oscillator    (LOCO)       -   BSP_CLOCKS_SOURCE_CLOCK_LOCO
 * Main Clock Oscillator                        -   BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC
 * Sub-Clock Oscillator                         -   BSP_CLOCKS_SOURCE_CLOCK_SUBCLOCK
 * PLL Clock Oscillator                         -   BSP_CLOCKS_SOURCE_CLOCK_PLL
 * </PRE>
 */
#ifndef BSP_CFG_CLOCK_SOURCE
 #define BSP_CFG_CLOCK_SOURCE         (BSP_CLOCKS_SOURCE_CLOCK_PLL)
#endif

/** CLKOUT Output Source Select (CKOSEL).
 *
 * Set the macro to one of the enumerations shown.
 *
 * <PRE>
 * Clock                                        -   Enumeration to use for macro
 * High Speed On-Chip Oscillator   (HOCO)       -   BSP_CLOCKS_SOURCE_CLOCK_HOCO
 * Low Speed On-Chip Oscillator    (LOCO)       -   BSP_CLOCKS_SOURCE_CLOCK_LOCO
 * Main Clock Oscillator                        -   BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC
 * Sub-Clock Oscillator                         -   BSP_CLOCKS_SOURCE_CLOCK_SUBCLOCK
 * PLL Clock Oscillator                         -   BSP_CLOCKS_SOURCE_CLOCK_PLL
 * PPLL Clock Oscillator                        -   BSP_CLOCKS_SOURCE_CLOCK_PPLL
 * </PRE>
 */
#ifndef BSP_CFG_CLKOUT_SOURCE
 #define BSP_CFG_CLKOUT_SOURCE        (BSP_CLOCKS_SOURCE_CLOCK_MAIN_OSC)
#endif

/** CLKOUT Output Divisor Select (CKODIV).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_CLKOUT_DIV
 #define BSP_CFG_CLKOUT_DIV           (BSP_CLOCKS_SYS_CLOCK_DIV_1)
#endif

/** Flash IF Clock Divider (FCK).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_FCLK_DIV
 #define BSP_CFG_FCLK_DIV             (BSP_CLOCKS_SYS_CLOCK_DIV_4)
#endif

/** System Clock Divider (ICK).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_ICLK_DIV
 #define BSP_CFG_ICLK_DIV             (BSP_CLOCKS_SYS_CLOCK_DIV_1)
#endif

/** Peripheral Module Clock A Divider (PCKA).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PCLKA_DIV
 #define BSP_CFG_PCLKA_DIV            (BSP_CLOCKS_SYS_CLOCK_DIV_2)
#endif

/** Peripheral Module Clock B Divider (PCKB).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PCLKB_DIV
 #define BSP_CFG_PCLKB_DIV            (BSP_CLOCKS_SYS_CLOCK_DIV_4)
#endif

/** Peripheral Module Clock C Divider (PCKC).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PCLKC_DIV
 #define BSP_CFG_PCLKC_DIV            (BSP_CLOCKS_SYS_CLOCK_DIV_4)
#endif

/** Peripheral Module Clock D Divider (PCKD).
 *
 * Available divisors = /1 (no division), /2, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_PCLKD_DIV
 #define BSP_CFG_PCLKD_DIV            (BSP_CLOCKS_SYS_CLOCK_DIV_4)
#endif

/** External Bus Clock Divider (BCLK).
 *
 * Available divisors = /1 (no division), /2, /3, /4, /8, /16, /32, /64
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_BCLK_DIV
 #define BSP_CFG_BCLK_DIV             (BSP_CLOCKS_SYS_CLOCK_DIV_8)
#endif

/** External Bus Clock Divider (BCLKDIV).
 *
 * Available options:
 * - 0 = BCK/1 frequency
 * - 1 = BCK/2 frequency
 *
 * @note Set macro definition to 'BSP_CLOCKS_SYS_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_BCLKDIV_DIV
 #define BSP_CFG_BCLKDIV_DIV          (BSP_CLOCKS_BCLK_DIV_1)
#endif

/**
 * The USB Clock source that is configured during startup.
 * Available clock sources:
 *   - BSP_USBCLK_SOURCE_CLOCK_USB
 *   - BSP_USBCLK_SOURCE_CLOCK_PPLL
 */
#ifndef BSP_CFG_UCK_SOURCE
 #define BSP_CFG_UCK_SOURCE           (BSP_USBCLK_SOURCE_CLOCK_PPLL)
#endif

/** USB Clock Divider Select.
 *
 * Available divisors = /2, /3, /4, /5
 *
 * @note Set macro definition to 'BSP_CLOCKS_USB_CLOCK_DIV_' + your divider selection.
 */
#ifndef BSP_CFG_UCK_DIV
 #define BSP_CFG_UCK_DIV              (BSP_CLOCKS_USB_CLOCK_DIV_4)
#endif

/**
 * The Ethernet-PHY External Clock (CLKOUT25M) Source that is configured during startup.
 * Available clock sources:
 *   - BSP_CLOCKOUT25_SOURCE_CLOCK_PLL
 *   - BSP_CLOCKOUT25_SOURCE_CLOCK_PPLL
 */
#ifndef BSP_CFG_CLOCKOUT25_SOURCE
 #define BSP_CFG_CLOCKOUT25_SOURCE    (BSP_CLOCKOUT25_SOURCE_CLOCK_PPLL)
#endif

#endif

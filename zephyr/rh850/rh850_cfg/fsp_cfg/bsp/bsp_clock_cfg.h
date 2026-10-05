/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef BSP_CLOCK_CFG_H_
#define BSP_CLOCK_CFG_H_

#include <zephyr/dt-bindings/clock/renesas_rh850_clock_common.h>
#include <zephyr/devicetree.h>

/* Constant expressions shared by early BSP configuration and runtime CGC.
 * These are register selectors, not logical RH850_CLOCK_* identifiers.
 * -1 denotes an unsupported parent. Keep expressions usable in #if.
 */
#define RP_BSP_CLOCK_LEGACY_BUS(s) \
	((s) == RH850_CLOCK_SYS || (s) == RH850_CLOCK_SYS_CLEAN || \
	 (s) == RH850_CLOCK_CPU || (s) == RH850_CLOCK_CLKC_CPU)

#if defined(CONFIG_SOC_SERIES_RH850U2A)
#define RP_BSP_CLOCK_HSB RH850_CLOCK_HSB
#define RP_BSP_CLOCK_LSB RH850_CLOCK_LSB
#define RP_BSP_CLOCK_HSI_WDT RH850_CLOCK_WDT
#define RP_BSP_CLOCK_LSI_WDT RH850_CLOCK_CLKA_WDT
#define RP_BSP_SELECT_MSPI(s) \
	(((s) == RH850_CLOCK_MOSC) ? 0 : \
	 ((s) == RH850_CLOCK_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 1 : -1)
#define RP_BSP_SELECT_TAUJ(s) \
	(((s) == RH850_CLOCK_LSIOSC) ? 0 : ((s) == RH850_CLOCK_HSIOSC) ? 1 : \
	 ((s) == RH850_CLOCK_MOSC) ? 2 : ((s) == RH850_CLOCK_HSB) ? 3 : -1)
#define RP_BSP_SELECT_EXTCLK(s) \
	(((s) == RH850_CLOCK_MOSC) ? 0 : \
	 ((s) == RH850_CLOCK_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 1 : \
	 ((s) == RH850_CLOCK_LSIOSC) ? 3 : ((s) == RH850_CLOCK_HSIOSC) ? 4 : -1)
#define RP_BSP_WDT_MAX_DIV 1
#else
#define RP_BSP_CLOCK_HSB RH850_CLOCK_CLKC_HSB
#define RP_BSP_CLOCK_LSB RH850_CLOCK_CLKC_LSB
#define RP_BSP_CLOCK_HSI_WDT RH850_CLOCK_WDTB
#define RP_BSP_CLOCK_LSI_WDT RH850_CLOCK_CLKA_WDTBA
#define RP_BSP_SELECT_MSPI(s) \
	(((s) == RH850_CLOCK_EMG) ? 0 : ((s) == RH850_CLOCK_MOSC) ? 1 : \
	 ((s) == RH850_CLOCK_CLKC_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 2 : -1)
#define RP_BSP_SELECT_TAUJ(s) \
	(((s) == RH850_CLOCK_EMG) ? 0 : ((s) == RH850_CLOCK_LSIOSC) ? 1 : \
	 ((s) == RH850_CLOCK_HSIOSC) ? 2 : ((s) == RH850_CLOCK_MOSC) ? 3 : \
	 ((s) == RH850_CLOCK_CLKC_HSB) ? 4 : -1)
#if defined(CONFIG_SOC_SERIES_RH850U2C)
#define RP_BSP_SELECT_EXTCLK_SOSC(s) (((s) == RH850_CLOCK_SOSC) ? 3 : -1)
#else
#define RP_BSP_SELECT_EXTCLK_SOSC(s) (-1)
#endif /* defined(CONFIG_SOC_SERIES_RH850U2C) */
#define RP_BSP_SELECT_EXTCLK(s) \
	(((s) == RH850_CLOCK_MOSC) ? 0 : ((s) == RH850_CLOCK_LSIOSC) ? 1 : \
	 ((s) == RH850_CLOCK_HSIOSC) ? 2 : \
	 ((s) == RH850_CLOCK_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 4 : \
	 ((s) == RH850_CLOCK_CLKC_HSB) ? 5 : RP_BSP_SELECT_EXTCLK_SOSC(s))
#define RP_BSP_WDT_MAX_DIV 2
#endif /* defined(CONFIG_SOC_SERIES_RH850U2A) */

#define RP_BSP_SELECT_IOSC(s) (((s) == RH850_CLOCK_HSIOSC) ? 1 : -1)
#define RP_BSP_SELECT_EMG(s) RP_BSP_SELECT_IOSC(s)
#define RP_BSP_SELECT_SYS(s) \
	(((s) == RH850_CLOCK_PLL || (s) == RH850_CLOCK_PLLO) ? 0 : \
	 ((s) == RH850_CLOCK_IOSC) ? 1 : -1)
#if defined(CONFIG_SOC_SERIES_RH850U2C)
#define RP_BSP_SELECT_BACKUP(s) (((s) == RH850_CLOCK_SYS_IOSC) ? 1 : -1)
#else
#define RP_BSP_SELECT_BACKUP(s) (((s) == RH850_CLOCK_IOSC) ? 1 : -1)
#endif /* defined(CONFIG_SOC_SERIES_RH850U2C) */
#define RP_BSP_SELECT_CLEAN(s) (((s) == RH850_CLOCK_PLL) ? 0 : RP_BSP_SELECT_BACKUP(s))
#define RP_BSP_SELECT_SSCG(s) (((s) == RH850_CLOCK_SSCG) ? 0 : RP_BSP_SELECT_BACKUP(s))
#define RP_BSP_SELECT_SSCG1(s) (((s) == RH850_CLOCK_SSCG1) ? 0 : RP_BSP_SELECT_BACKUP(s))
/* RLIN division is encoded in the source selector, not a separate divider. */
#if defined(CONFIG_SOC_SERIES_RH850U2A)
#define RP_BSP_SELECT_RLIN_MOSC(d) (((d) == 4U) ? 2 : ((d) == 8U) ? 3 : -1)
#define RP_BSP_SELECT_RLIN_EMG(s, d) (-1)
#else
#define RP_BSP_SELECT_RLIN_MOSC(d) \
	(((d) == 0U || (d) == 1U) ? 2 : ((d) == 4U) ? 3 : ((d) == 8U) ? 4 : -1)
#define RP_BSP_SELECT_RLIN_EMG(s, d) \
	(((s) == RH850_CLOCK_EMG && ((d) == 0U || (d) == 1U)) ? 0 : -1)
#endif /* defined(CONFIG_SOC_SERIES_RH850U2A) */
#define RP_BSP_SELECT_RLIN_DIV(s, d, bus) \
	(((s) == RH850_CLOCK_MOSC) ? RP_BSP_SELECT_RLIN_MOSC(d) : \
	 (((s) == (bus) || RP_BSP_CLOCK_LEGACY_BUS(s)) && ((d) == 0U || (d) == 1U)) \
	 ? 1 : RP_BSP_SELECT_RLIN_EMG(s, d))
#define RP_BSP_SELECT_RLIN3_DIV(s, d) RP_BSP_SELECT_RLIN_DIV(s, d, RP_BSP_CLOCK_HSB)
#define RP_BSP_SELECT_RLIN323_DIV(s, d) RP_BSP_SELECT_RLIN_DIV(s, d, RH850_CLOCK_CLKC_HBUS)
#define RP_BSP_SELECT_RLIN3(s) RP_BSP_SELECT_RLIN3_DIV(s, 0U)
#define RP_BSP_SELECT_RLIN323(s) RP_BSP_SELECT_RLIN323_DIV(s, 0U)
#define RP_BSP_SELECT_CANFD(s) \
	(((s) == RH850_CLOCK_CLKC_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 1 : -1)
#define RP_BSP_SELECT_CAN_XIN(s) (((s) == RH850_CLOCK_MOSC) ? 1 : -1)
#define RP_BSP_SELECT_CANXL(s) \
	(((s) == RH850_CLOCK_CLKC_UHSB) ? 4 : \
	 ((s) == RH850_CLOCK_CLKC_HSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 1 : -1)
#define RP_BSP_SELECT_AWDT(s) (((s) == RH850_CLOCK_LSIOSC) ? 1 : -1)
#define RP_BSP_SELECT_RTCA(s) (((s) == RH850_CLOCK_LSIOSC) ? 1 : -1)
#if defined(CONFIG_SOC_SERIES_RH850U2C)
#define RP_BSP_SELECT_AADC(s) \
	(((s) == RH850_CLOCK_EMG) ? 0 : ((s) == RH850_CLOCK_HSIOSC) ? 1 : \
	 ((s) == RH850_CLOCK_MOSC) ? 2 : ((s) == RP_BSP_CLOCK_LSB) ? 3 : -1)
#else
#define RP_BSP_SELECT_AADC(s) \
	(((s) == RH850_CLOCK_MOSC) ? 0 : ((s) == RH850_CLOCK_HSIOSC) ? 1 : \
	 ((s) == RP_BSP_CLOCK_LSB) ? 2 : -1)
#endif /* defined(CONFIG_SOC_SERIES_RH850U2C) */
#define RP_BSP_SELECT_OSPI(s) \
	(((s) == RH850_CLOCK_EMG) ? 0 : ((s) == RH850_CLOCK_CLKC_HBUS) ? 1 : \
	 ((s) == RH850_CLOCK_CLKC_SHSB) ? 2 : -1)
#define RP_BSP_SELECT_ADC(s) \
	(((s) == RH850_CLOCK_LSB || RP_BSP_CLOCK_LEGACY_BUS(s)) ? 1 : -1)
#define RP_BSP_SELECT_GTM(s) \
	(((s) == RH850_CLOCK_CLKC_SBUS) ? 0 : ((s) == RH850_CLOCK_CLKC_UHSB) ? 1 : -1)
#define RP_BSP_SELECT_SWDT(s) (((s) == RH850_CLOCK_HSIOSC) ? 0 : -1)

/* DT div values are physical factors, including fixed prescalers. */
#define BSP_CLOCK_DT_NORMAL_PARENT(node) DT_PHANDLE(node, clock_source)
#define BSP_CLOCK_DT_CYCLIC_PARENT(node) \
	COND_CODE_1(DT_NODE_HAS_PROP(node, renesas_cyclicrun_clock_source), \
		(DT_PHANDLE(node, renesas_cyclicrun_clock_source)), \
		(DT_PHANDLE(node, clock_source)))
#define BSP_CLOCK_DT_NORMAL_FACTOR(node) DT_PROP(node, div)
#define BSP_CLOCK_DT_CYCLIC_FACTOR(node) \
	DT_PROP_OR(node, renesas_cyclicrun_div, DT_PROP(node, div))
#define BSP_CLOCK_DT_PARENT(node, mode) BSP_CLOCK_DT_##mode##_PARENT(node)
#define BSP_CLOCK_DT_FACTOR(node, mode) BSP_CLOCK_DT_##mode##_FACTOR(node)

/* EXTCLK div = 0 stops the output. Keep the denominator nonzero in C and #if. */
#define BSP_CLOCK_DT_RATE_DIV(hz, div) \
	(((div) == 0U) ? 0U : ((hz) / (((div) == 0U) ? 1U : (div))))

/* Unroll up to eight derived edges; the final node must provide a root rate.
 * Separate macro levels allow the preprocessor to follow DT phandles without
 * recursive macro expansion. Both modes round down at each clock-tree edge.
 */
#define BSP_CLOCK_DT_RATE_0(node, mode) DT_PROP(node, clock_frequency)
#define BSP_CLOCK_DT_RATE_1(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_0(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_2(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_1(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_3(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_2(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_4(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_3(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_5(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_4(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_6(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_5(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_7(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_6(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RATE_8(node, mode) \
	DT_PROP_OR(node, clock_frequency, \
		BSP_CLOCK_DT_RATE_DIV(BSP_CLOCK_DT_RATE_7(BSP_CLOCK_DT_PARENT(node, mode), mode), \
			BSP_CLOCK_DT_FACTOR(node, mode)))

#define BSP_CLOCK_DT_HZ(node) BSP_CLOCK_DT_RATE_8(node, NORMAL)
#define BSP_CLOCK_DT_CYCLIC_HZ(node) BSP_CLOCK_DT_RATE_8(node, CYCLIC)

/* Translate clock-node identities to the IDs used by BSP selectors and the HAL.
 * DT_NODE_EXISTS also permits optional source labels on other U2 variants.
 */
#define BSP_CLOCK_DT_MATCH(node, label) \
	(DT_NODE_EXISTS(DT_NODELABEL(label)) && DT_SAME_NODE(node, DT_NODELABEL(label)))
#define BSP_CLOCK_DT_ID(node) \
	(BSP_CLOCK_DT_MATCH(node, mosc) ? RH850_CLOCK_MOSC : \
	 BSP_CLOCK_DT_MATCH(node, hsiosc) ? RH850_CLOCK_HSIOSC : \
	 BSP_CLOCK_DT_MATCH(node, lsiosc) ? RH850_CLOCK_LSIOSC : \
	 BSP_CLOCK_DT_MATCH(node, hviosc) ? RH850_CLOCK_HVIOSC : \
	 BSP_CLOCK_DT_MATCH(node, sosc) ? RH850_CLOCK_SOSC : \
	 BSP_CLOCK_DT_MATCH(node, pll) ? RH850_CLOCK_PLL : \
	 BSP_CLOCK_DT_MATCH(node, pllo) ? RH850_CLOCK_PLLO : \
	 BSP_CLOCK_DT_MATCH(node, sscg0) ? RH850_CLOCK_SSCG : \
	 BSP_CLOCK_DT_MATCH(node, sscg1) ? RH850_CLOCK_SSCG1 : \
	 BSP_CLOCK_DT_MATCH(node, iosc) ? RH850_CLOCK_IOSC : \
	 BSP_CLOCK_DT_MATCH(node, clk_sys) ? RH850_CLOCK_SYS : \
	 BSP_CLOCK_DT_MATCH(node, clk_sys_iosc) ? RH850_CLOCK_SYS_IOSC : \
	 BSP_CLOCK_DT_MATCH(node, clk_sys_clean) ? RH850_CLOCK_SYS_CLEAN : \
	 BSP_CLOCK_DT_MATCH(node, clk_cpu) ? RH850_CLOCK_CPU : \
	 BSP_CLOCK_DT_MATCH(node, clk_sbus) ? RH850_CLOCK_SBUS : \
	 BSP_CLOCK_DT_MATCH(node, clk_hbus) ? RH850_CLOCK_HBUS : \
	 BSP_CLOCK_DT_MATCH(node, clk_hsb) ? RH850_CLOCK_HSB : \
	 BSP_CLOCK_DT_MATCH(node, clk_lsb) ? RH850_CLOCK_LSB : \
	 (BSP_CLOCK_DT_MATCH(node, clk_emg) || BSP_CLOCK_DT_MATCH(node, emg)) ? RH850_CLOCK_EMG : \
	 BSP_CLOCK_DT_MATCH(node, clkc_cpu) ? RH850_CLOCK_CLKC_CPU : \
	 BSP_CLOCK_DT_MATCH(node, clkc_sbus) ? RH850_CLOCK_CLKC_SBUS : \
	 BSP_CLOCK_DT_MATCH(node, clkc_hbus) ? RH850_CLOCK_CLKC_HBUS : \
	 BSP_CLOCK_DT_MATCH(node, clkc_uhsb) ? RH850_CLOCK_CLKC_UHSB : \
	 BSP_CLOCK_DT_MATCH(node, clkc_shsb) ? RH850_CLOCK_CLKC_SHSB : \
	 BSP_CLOCK_DT_MATCH(node, clkc_hsb) ? RH850_CLOCK_CLKC_HSB : \
	 BSP_CLOCK_DT_MATCH(node, clkc_lsb) ? RH850_CLOCK_CLKC_LSB : \
	 BSP_CLOCK_DT_MATCH(node, clk_sys_sscg0) ? RH850_CLOCK_SYS_SSCG : \
	 BSP_CLOCK_DT_MATCH(node, clk_sys_sscg1) ? RH850_CLOCK_SYS_SSCG1 : \
	 BSP_CLOCK_DT_MATCH(node, clk_uhsb) ? RH850_CLOCK_UHSB : \
	 BSP_CLOCK_DT_MATCH(node, clk_mspi) ? RH850_CLOCK_MSPI : \
	 BSP_CLOCK_DT_MATCH(node, clk_rlin3) ? RH850_CLOCK_RLIN3 : \
	 BSP_CLOCK_DT_MATCH(node, clk_rlin3_ch23) ? RH850_CLOCK_CH23_RLIN3 : \
	 BSP_CLOCK_DT_MATCH(node, clk_rcanosc) ? RH850_CLOCK_RCANOSC : \
	 BSP_CLOCK_DT_MATCH(node, clk_canfd_c) ? RH850_CLOCK_CANFD_C : \
	 BSP_CLOCK_DT_MATCH(node, clk_canfd_xin) ? RH850_CLOCK_CANFD_XIN : \
	 BSP_CLOCK_DT_MATCH(node, clk_adc) ? RH850_CLOCK_ADC : \
	 BSP_CLOCK_DT_MATCH(node, clka_adc) ? RH850_CLOCK_CLKA_ADC : \
	 BSP_CLOCK_DT_MATCH(node, clk_wdt) ? RH850_CLOCK_WDT : \
	 BSP_CLOCK_DT_MATCH(node, clka_wdtb) ? RH850_CLOCK_WDTB : \
	 BSP_CLOCK_DT_MATCH(node, clk_wdtb) ? RH850_CLOCK_WDTB : \
	 BSP_CLOCK_DT_MATCH(node, clka_wdt) ? RH850_CLOCK_CLKA_WDT : \
	 BSP_CLOCK_DT_MATCH(node, clka_wdtba) ? RH850_CLOCK_CLKA_WDTBA : \
	 BSP_CLOCK_DT_MATCH(node, clka_tauj) ? RH850_CLOCK_CLKA_TAUJ : \
	 BSP_CLOCK_DT_MATCH(node, clka_rtca) ? RH850_CLOCK_CLKA_RTCA : \
	 BSP_CLOCK_DT_MATCH(node, clk_swdt) ? RH850_CLOCK_SWDT : \
	 BSP_CLOCK_DT_MATCH(node, clkc_canxl_ch0) ? RH850_CLOCK_CANXL_CH0 : \
	 BSP_CLOCK_DT_MATCH(node, clkc_canxl_ch1) ? RH850_CLOCK_CANXL_CH1 : \
	 BSP_CLOCK_DT_MATCH(node, extclk0) ? RH850_CLOCK_EXTCLK0O : \
	 BSP_CLOCK_DT_MATCH(node, extclk1) ? RH850_CLOCK_EXTCLK1O : \
	 BSP_CLOCK_DT_MATCH(node, clk_ecmcnt) ? RH850_CLOCK_ECMCNT : \
	 BSP_CLOCK_DT_MATCH(node, clka_lps) ? RH850_CLOCK_CLKA_LPS : \
	 BSP_CLOCK_DT_MATCH(node, clk_fli) ? RH850_CLOCK_FLI : \
	 BSP_CLOCK_DT_MATCH(node, clk_faci) ? RH850_CLOCK_FACI : \
	 BSP_CLOCK_DT_MATCH(node, clk_gtm) ? RH850_CLOCK_GTM : \
	 BSP_CLOCK_DT_MATCH(node, clk_dfp) ? RH850_CLOCK_DFP : \
	 BSP_CLOCK_DT_MATCH(node, clk_dfp_div_2) ? RH850_CLOCK_DFP_DIV_2 : \
	 BSP_CLOCK_DT_MATCH(node, clk_dfa) ? RH850_CLOCK_DFA : \
	 BSP_CLOCK_DT_MATCH(node, clk_ospi) ? RH850_CLOCK_OSPI : \
	 BSP_CLOCK_DT_MATCH(node, clk_ospix2) ? RH850_CLOCK_OSPIX2 : -1)

#define BSP_CLOCK_DT_SOURCE(node, prop, selector) \
	RP_BSP_SELECT_##selector(BSP_CLOCK_DT_ID( \
		COND_CODE_1(DT_NODE_HAS_PROP(node, prop), (DT_PHANDLE(node, prop)), \
			(DT_PHANDLE(node, clock_source)))))

/* AADC and EXTCLK have a fixed /20 stage when HSIOSC is selected. */
#define BSP_CLOCK_DT_PRESCALER(node, mode) \
	((BSP_CLOCK_DT_MATCH(BSP_CLOCK_DT_PARENT(node, mode), hsiosc) && \
	  (BSP_CLOCK_DT_MATCH(node, clka_adc) || BSP_CLOCK_DT_MATCH(node, extclk0) || \
	   BSP_CLOCK_DT_MATCH(node, extclk1))) ? 20U : 1U)
#define BSP_CLOCK_DT_REGISTER_DIV(node, mode) \
	(BSP_CLOCK_DT_MATCH(node, clka_adc) ? \
	 ((BSP_CLOCK_DT_FACTOR(node, mode) == BSP_CLOCK_DT_PRESCALER(node, mode)) ? 1 : \
	  (BSP_CLOCK_DT_FACTOR(node, mode) == 2U * BSP_CLOCK_DT_PRESCALER(node, mode)) ? 0 : -1) : \
	 ((BSP_CLOCK_DT_FACTOR(node, mode) % BSP_CLOCK_DT_PRESCALER(node, mode)) != 0U) ? \
	 -1 : (BSP_CLOCK_DT_FACTOR(node, mode) / BSP_CLOCK_DT_PRESCALER(node, mode)))
#define BSP_CLOCK_DT_DIV(node) BSP_CLOCK_DT_REGISTER_DIV(node, NORMAL)
#define BSP_CLOCK_DT_CYCLIC_DIV(node) BSP_CLOCK_DT_REGISTER_DIV(node, CYCLIC)

/* SYS_IOSC and WDT encode fixed divider choices in selector fields. */
#define BSP_CLOCK_DT_SYS_IOSC(node) \
	((BSP_CLOCK_DT_MATCH(BSP_CLOCK_DT_NORMAL_PARENT(node), hsiosc)) ? \
	 ((DT_PROP(node, div) == 1U) ? 0 : (DT_PROP(node, div) == 2U) ? 1 : \
	  (DT_PROP(node, div) == 10U) ? 2 : -1) : -1)
#define BSP_CLOCK_DT_WDT_MODE(node, mode) \
	((BSP_CLOCK_DT_MATCH(BSP_CLOCK_DT_PARENT(node, mode), hsiosc)) ? \
	 ((BSP_CLOCK_DT_FACTOR(node, mode) == 20U) ? 0 : \
	  (BSP_CLOCK_DT_FACTOR(node, mode) == 640U) ? 1 : \
	  (BSP_CLOCK_DT_FACTOR(node, mode) == 800U && RP_BSP_WDT_MAX_DIV == 2) ? 2 : -1) : -1)
#define BSP_CLOCK_DT_WDT(node, cyclic) \
	COND_CODE_1(cyclic, (BSP_CLOCK_DT_WDT_MODE(node, CYCLIC)), \
		(BSP_CLOCK_DT_WDT_MODE(node, NORMAL)))
#define BSP_CLOCK_DT_RLIN_MODE(node, mode) \
	((BSP_CLOCK_DT_FACTOR(node, mode) == 0U) ? -1 : \
	 BSP_CLOCK_DT_MATCH(node, clk_rlin3_ch23) ? \
	 RP_BSP_SELECT_RLIN323_DIV(BSP_CLOCK_DT_ID(BSP_CLOCK_DT_PARENT(node, mode)), \
				 BSP_CLOCK_DT_FACTOR(node, mode)) : \
	 RP_BSP_SELECT_RLIN3_DIV(BSP_CLOCK_DT_ID(BSP_CLOCK_DT_PARENT(node, mode)), \
			       BSP_CLOCK_DT_FACTOR(node, mode)))
#define BSP_CLOCK_DT_RLIN(node, cyclic) \
	COND_CODE_1(cyclic, (BSP_CLOCK_DT_RLIN_MODE(node, CYCLIC)), \
		(BSP_CLOCK_DT_RLIN_MODE(node, NORMAL)))


#if defined(CONFIG_SOC_R7F702300AEBBC_C) || defined(CONFIG_SOC_R7F702300BEBBC_C) || defined(CONFIG_SOC_R7F702300EBBG_C) \
    || defined(CONFIG_SOC_R7F702300AEBBB_C)  || defined(CONFIG_SOC_R7F702300BEBBB_C) || defined(CONFIG_SOC_R7F702300EBBB_C) \
    || defined(CONFIG_SOC_R7F702300AFABA_C)  || defined(CONFIG_SOC_R7F702300BFABA_C) || defined(CONFIG_SOC_R7F702300EABA_C)
 #include "bsp_clock_cfg_u2a16.h"
#elif defined(CONFIG_SOC_R7F702301AEBBA_C) || defined(CONFIG_SOC_R7F702301BEBBA_C) || defined(CONFIG_SOC_R7F702301EBBA_C) \
    || defined(CONFIG_SOC_R7F702301AFABG_C)  || defined(CONFIG_SOC_R7F702301BFABG_C) || defined(CONFIG_SOC_R7F702301EABG_C)
 #include "bsp_clock_cfg_u2a8.h"
#elif defined(CONFIG_SOC_R7F702302FABB_C) || defined(CONFIG_SOC_R7F702302FAFK_C) || defined(CONFIG_SOC_R7F702302FABD_C) \
    || defined(CONFIG_SOC_R7F702302FAFM_C)
 #include "bsp_clock_cfg_u2a6.h"
#elif defined(CONFIG_SOC_R7F70255XFABB_C) || defined(CONFIG_SOC_R7F70255XAFABB_C) || defined(CONFIG_SOC_R7F70255XBFABB_C)
 #include "bsp_clock_cfg_u2b6.h"
#elif defined(CONFIG_SOC_R7F70254XFABG_C) || defined(CONFIG_SOC_R7F70254XFABA_C) || defined(CONFIG_SOC_R7F70254XFABB_C) \
    || defined(CONFIG_SOC_R7F70254XAFABG_C)  || defined(CONFIG_SOC_R7F70254XAFABA_C) || defined(CONFIG_SOC_R7F70254XAFABB_C)
 #include "bsp_clock_cfg_u2b10.h"
#elif defined(CONFIG_SOC_R7F7025BXFABC_C) || defined(CONFIG_SOC_R7F7025BXFABB_C)
 #include "bsp_clock_cfg_u2b24e.h"
#elif defined(CONFIG_SOC_R7F7025FXFABC_C) || defined(CONFIG_SOC_R7F7025FXFABB_C)
 #include "bsp_clock_cfg_u2b16e.h"
#elif defined(CONFIG_SOC_R7F7025CXFABC_C) || defined(CONFIG_SOC_R7F7025CXFABB_C)
 #include "bsp_clock_cfg_u2b12e.h"
#elif defined(CONFIG_SOC_R7F7025DXFABB_C)
 #include "bsp_clock_cfg_u2b6e.h"
#elif defined(CONFIG_SOC_R7F702600AFABB_C)
 #include "bsp_clock_cfg_u2c8.h"
#elif defined(CONFIG_SOC_R7F702606AFABB_C) || defined(CONFIG_SOC_R7F702616AFABB_C) || defined(CONFIG_SOC_R7F702613AFABB_C) \
    || defined(CONFIG_SOC_R7F702606AFAFB_C)  || defined(CONFIG_SOC_R7F702606AFAFM_C)  || defined(CONFIG_SOC_R7F702616AFAFM_C) \
    || defined(CONFIG_SOC_R7F702616AFAFB_C) || defined(CONFIG_SOC_R7F702614AFAFM_C) || defined(CONFIG_SOC_R7F702614AFAFB_C) \
    || defined(CONFIG_SOC_R7F702606AFAFD_C)  || defined(CONFIG_SOC_R7F702616AFAFD_C)  || defined(CONFIG_SOC_R7F702613AFAFD_C)
 #include "bsp_clock_cfg_u2c4.h"
#elif defined(CONFIG_SOC_R7F702615AFAFM_C) || defined(CONFIG_SOC_R7F702615AFAFB_C) \
    || defined(CONFIG_SOC_R7F702608AFAFD_C)
 #include "bsp_clock_cfg_u2c2.h"
#endif

#endif                                 /* BSP_CLOCK_CFG_H_ */

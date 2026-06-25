/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/
#ifndef BSP_CLOCK_CFG_U2A16_H_
#define BSP_CLOCK_CFG_U2A16_H_
#ifndef BSP_CFG_CLOCKS_SECURE
 #define BSP_CFG_CLOCKS_SECURE                    (0)
#endif

#ifndef BSP_CFG_CLOCKS_OVERRIDE
 #define BSP_CFG_CLOCKS_OVERRIDE                  (0)
#endif

#ifndef BSP_CFG_CLK_MOSC_VALUE
 #define BSP_CFG_CLK_MOSC_VALUE                   (BSP_CLKCTL_CLK_MOSC_20MHZ) /* MainOSC: 20MHz */
#endif

#ifndef BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE
 #if defined(CONFIG_RH850_CLOCK_MOSC_CONTINUE_IN_STANDBY)
  #define BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE    (1)                         /* MainOSC-STPM: Continue */
 #else
  #define BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE    (0)                         /* MainOSC-STPM: Stop */
 #endif
#endif

#ifndef BSP_CFG_CLOCK_CLK_PLL_HZ
 #define BSP_CFG_CLOCK_CLK_PLL_HZ                 (800000000)                 /* PLL: 800000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_PLL0_DIV
 #define BSP_CFG_CLOCK_CLK_PLL0_DIV               (0)                         /* PLL0: No division */
#endif

#ifndef BSP_CFG_CLOCK_CLK_PLLO_HZ
 #define BSP_CFG_CLOCK_CLK_PLLO_HZ                (800000000)                 /* PLL0: 800000000Hz */
#endif

#ifndef BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE
 #if defined(CONFIG_RH850_CLOCK_PLL_CONTINUE_IN_STANDBY)
  #define BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE     (1)                         /* PLL-STPM: Continue */
 #else
  #define BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE     (0)                         /* PLL-STPM: Stop */
 #endif
#endif

#ifndef BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE
 #if defined(CONFIG_RH850_CLOCK_HSOSC_CONTINUE_IN_STANDBY)
  #define BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE   (1)                         /* HSOSC-STPM: Continue */
 #else
  #define BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE   (0)                         /* HSOSC-STPM: Stop */
 #endif
#endif

#ifndef BSP_CFG_CLOCK_CLK_CPU_VALUE
 #define BSP_CFG_CLOCK_CLK_CPU_VALUE              (400000000)                 /* CPU: 400000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HVIOSC_HZ
 #define BSP_CFG_CLOCK_CLK_HVIOSC_HZ              (16000000)                  /* CLK_HVIOSC: 16000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_LSIOSC_HZ
 #define BSP_CFG_CLOCK_CLK_LSIOSC_HZ              (240000)                    /* CLK_LSIOSC: 240000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HSIOSC_HZ
 #define BSP_CFG_CLOCK_CLK_HSIOSC_HZ              (200000000)                 /* CLK_HSIOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_LPS_HZ
 #define BSP_CFG_CLOCK_CLKA_LPS_HZ                (10000000)                  /* CLKA_LPS: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_ECMCNT_HZ
 #define BSP_CFG_CLOCK_CLK_ECMCNT_HZ              (10000000)                  /* CLK_ECMCNT: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MOSC_HZ
 #define BSP_CFG_CLOCK_CLK_MOSC_HZ                (20000000)                  /* CLK_MOSC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_IOSC_SRC
 #define BSP_CFG_CLOCK_CLK_IOSC_SRC               (1)                         /* CLK_IOSC: CLK_HSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLK_IOSC_HZ
 #define BSP_CFG_CLOCK_CLK_IOSC_HZ                (200000000)                 /* CLK_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MSPI_SRC
 #define BSP_CFG_CLOCK_CLK_MSPI_SRC               (1)                         /* CLK_MSPI: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MSPI_HZ
 #define BSP_CFG_CLOCK_CLK_MSPI_HZ                (80000000)                  /* CLK_MSPI: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN3_SRC
 #define BSP_CFG_CLOCK_CLK_RLIN3_SRC              (1)                         /* CLK_RLIN3: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN3_HZ
 #define BSP_CFG_CLOCK_CLK_RLIN3_HZ               (80000000)                  /* CLK_RLIN3: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_WDT_SRC
 #define BSP_CFG_CLOCK_CLK_WDT_SRC                (1)                         /* CLK_WDT: CLK_HSIOSC_DIV_640 */
#endif

#ifndef BSP_CFG_CLOCK_CLK_WDT_HZ
 #define BSP_CFG_CLOCK_CLK_WDT_HZ                 (312500)                    /* CLK_WDT: 312500Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RCANOSC_SRC
 #define BSP_CFG_CLOCK_CLK_RCANOSC_SRC            (1)                         /* CLK_RCANOSC: CLK_MOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RCANOSC_HZ
 #define BSP_CFG_CLOCK_CLK_RCANOSC_HZ             (20000000)                  /* CLK_RCANOSC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CKSC_CPUC_SRC
 #define BSP_CFG_CLOCK_CKSC_CPUC_SRC              (0)                         /* CLK_SYS: CLK_PLLO */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SYS_HZ
 #define BSP_CFG_CLOCK_CLK_SYS_HZ                 (800000000)                 /* CLK_SYS: 800000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CPU_HZ
 #define BSP_CFG_CLOCK_CLK_CPU_HZ                 (400000000)                 /* CLK_CPU: 400000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SBUS_HZ
 #define BSP_CFG_CLOCK_CLK_SBUS_HZ                (200000000)                 /* CLK_SBUS: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HBUS_HZ
 #define BSP_CFG_CLOCK_CLK_HBUS_HZ                (100000000)                 /* CLK_HBUS: 100000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_UHSB_HZ
 #define BSP_CFG_CLOCK_CLK_UHSB_HZ                (160000000)                 /* CLK_UHSB: 160000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HSB_HZ
 #define BSP_CFG_CLOCK_CLK_HSB_HZ                 (80000000)                  /* CLK_HSB: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_LSB_HZ
 #define BSP_CFG_CLOCK_CLK_LSB_HZ                 (40000000)                  /* CLK_LSB: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_WDT_SRC
 #define BSP_CFG_CLOCK_CLKA_WDT_SRC               (1)                         /* CLKA_WDT: CLK_LSIOSC/128 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_WDT_HZ
 #define BSP_CFG_CLOCK_CLKA_WDT_HZ                (1875)                      /* CLKA_WDT: 1875Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_TAUJ_SRC
 #define BSP_CFG_CLOCK_CLKA_TAUJ_SRC              (1)                         /* CLKA_TAUJ: CLK_HSIOSC / 20 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_TAUJ_HZ
 #define BSP_CFG_CLOCK_CLKA_TAUJ_HZ               (10000000)                  /* CLKA_TAUJ: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_RTCA_SRC
 #define BSP_CFG_CLOCK_CLKA_RTCA_SRC              (1)                         /* CLKA_RTCA: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_RTCA_HZ
 #define BSP_CFG_CLOCK_CLKA_RTCA_HZ               (240000)                    /* CLKA_RTCA: 240000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_ADC_SRC
 #define BSP_CFG_CLOCK_CLKA_ADC_SRC               (1)                         /* CLKA_ADC: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_ADC_DIV
 #define BSP_CFG_CLOCK_CLKA_ADC_DIV               (1)                         /* CKSC_AADCC selection/1 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_ADC_HZ
 #define BSP_CFG_CLOCK_CLKA_ADC_HZ                (10000000)                  /* CLKA_ADC: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_ADC_SRC
 #define BSP_CFG_CLOCK_CLK_ADC_SRC                (1)                         /* CLK_ADC: CLK_LSB/2 */
#endif

#ifndef BSP_CFG_CLOCK_CLK_ADC_HZ
 #define BSP_CFG_CLOCK_CLK_ADC_HZ                 (20000000)                  /* CLK_ADC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_SRC
 #define BSP_CFG_CLOCK_EXTCLK0O_SRC               (1)                         /* EXTCLK0O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_DIV
 #define BSP_CFG_CLOCK_EXTCLK0O_DIV               0                           /* EXTCLK0 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_HZ
 #define BSP_CFG_CLOCK_EXTCLK0O_HZ                (0)                         /* EXTCLK0O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_SRC
 #define BSP_CFG_CLOCK_EXTCLK1O_SRC               (1)                         /* EXTCLK1O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_DIV
 #define BSP_CFG_CLOCK_EXTCLK1O_DIV               0                           /* EXTCLK1 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_HZ
 #define BSP_CFG_CLOCK_EXTCLK1O_HZ                (0)                         /* EXTCLK1O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HVIOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HVIOSC_HZ    (16000000)                  /* CYCLIC_CLK_HVIOSC: 16000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ    (240000)                    /* CYCLIC_CLK_LSIOSC: 240000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ    (200000000)                 /* CYCLIC_CLK_HSIOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ      (10000000)                  /* CYCLIC_CLKA_LPS: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ    (10000000)                  /* CYCLIC_CLK_ECMCNT: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ      (20000000)                  /* CYCLIC_CLK_MOSC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_SRC     (1)                         /* CYCLIC_CLK_IOSC: CLK_HSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ      (200000000)                 /* CYCLIC_CLK_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC     (1)                         /* CYCLIC_CLK_MSPI: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ      (20000000)                  /* CYCLIC_CLK_MSPI: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC    (1)                         /* CYCLIC_CLK_RLIN3: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ     (20000000)                  /* CYCLIC_CLK_RLIN3: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CKSC_CPUC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CKSC_CPUC_SRC    (1)                         /* CYCLIC_CLK_SYS: CLK_IOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_HZ       (200000000)                 /* CYCLIC_CLK_SYS: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ       (100000000)                 /* CYCLIC_CLK_CPU: 100000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ      (50000000)                  /* CYCLIC_CLK_SBUS: 50000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ      (25000000)                  /* CYCLIC_CLK_HBUS: 25000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ      (40000000)                  /* CYCLIC_CLK_UHSB: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ       (20000000)                  /* CYCLIC_CLK_HSB: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ       (10000000)                  /* CYCLIC_CLK_LSB: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_SRC     (1)                         /* CYCLIC_CLKA_WDT: CLK_LSIOSC/128 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDT_HZ      (1875)                      /* CYCLIC_CLKA_WDT: 1875Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC    (1)                         /* CYCLIC_CLKA_TAUJ: CLK_HSIOSC / 20 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ     (10000000)                  /* CYCLIC_CLKA_TAUJ: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC    (1)                         /* CYCLIC_CLKA_RTCA: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ     (240000)                    /* CYCLIC_CLKA_RTCA: 240000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC     (1)                         /* CYCLIC_CLKA_ADC: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ      (10000000)                  /* CYCLIC_CLKA_ADC: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_SRC      (1)                         /* CYCLIC_CLK_WDT: CLK_HSIOSC_DIV_640 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_WDT_HZ       (312500)                    /* CYCLIC_CLK_WDT: 312500Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC     (1)                         /* CYCLIC_EXTCLK0O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV     0                           /* CYCLIC_EXTCLK0 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ      (0)                         /* CYCLIC_EXTCLK0O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC     (1)                         /* CYCLIC_EXTCLK1O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV     0                           /* CYCLIC_EXTCLK1 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ      (0)                         /* CYCLIC_EXTCLK1O: 0Hz */
#endif

#endif /* BSP_CLOCK_CFG_U2A16_H_ */

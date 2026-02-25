/* generated configuration header file - do not edit */
#ifndef BSP_CLOCK_CFG_U2C8_H_
#define BSP_CLOCK_CFG_U2C8_H_
#ifndef BSP_CFG_CLOCKS_SECURE
 #define BSP_CFG_CLOCKS_SECURE                         (0)
#endif

#ifndef BSP_CFG_CLOCKS_OVERRIDE
 #define BSP_CFG_CLOCKS_OVERRIDE                       (0)
#endif

#ifndef BSP_CFG_CLOCK_CLK_LSIOSC_HZ
 #define BSP_CFG_CLOCK_CLK_LSIOSC_HZ                   (32768)                     /* CLK_LSIOSC: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HSIOSC_HZ
 #define BSP_CFG_CLOCK_CLK_HSIOSC_HZ                   (200000000)                 /* CLK_HSIOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_IOSC_SRC
 #define BSP_CFG_CLOCK_CLK_IOSC_SRC                    (1)                         /* CLK_IOSC: CLK_HSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLK_IOSC_HZ
 #define BSP_CFG_CLOCK_CLK_IOSC_HZ                     (200000000)                 /* CLK_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SYS_IOSC_SRC
 #define BSP_CFG_CLOCK_CLK_SYS_IOSC_SRC                (0)                         /* CLK_HSIOSCO: No division */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SYS_IOSC_HZ
 #define BSP_CFG_CLOCK_CLK_SYS_IOSC_HZ                 (200000000)                 /* CLK_SYS_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_EMG_SRC
 #define BSP_CFG_CLOCK_CLK_EMG_SRC                     (1)                         /* CLK_EMG: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CLK_EMG_HZ
 #define BSP_CFG_CLOCK_CLK_EMG_HZ                      (10000000)                  /* CLK_EMG: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_ECMCNT_HZ
 #define BSP_CFG_CLOCK_CLK_ECMCNT_HZ                   (10000000)                  /* CLK_ECMCNT: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_FLI_HZ
 #define BSP_CFG_CLOCK_CLK_FLI_HZ                      (320000000)                 /* CLK_FLI: 320000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_FACI_HZ
 #define BSP_CFG_CLOCK_CLK_FACI_HZ                     (40000000)                  /* CLK_FACI: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_LPS_HZ
 #define BSP_CFG_CLOCK_CLKA_LPS_HZ                     (10000000)                  /* CLKA_LPS: 10000000Hz */
#endif

#ifndef BSP_CFG_CLK_MOSC_VALUE
 #define BSP_CFG_CLK_MOSC_VALUE                        (BSP_CLKCTL_CLK_MOSC_20MHZ) /* MainOSC: 20MHz */
#endif

#ifndef BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE
 #define BSP_CFG_CLK_MOSC_SELECT_STANDBY_MODE          (0)                         /* MainOSC-STPM: Stop */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MOSC_HZ
 #define BSP_CFG_CLOCK_CLK_MOSC_HZ                     (20000000)                  /* CLK_MOSC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SOSC_SRC
 #define BSP_CFG_CLOCK_CLK_SOSC_SRC                    (0)                         /* Sub OSC: Disable */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SOSC_HZ
 #define BSP_CFG_CLOCK_CLK_SOSC_HZ                     (0)                         /* CLK_SOSC: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_PLL_HZ
 #define BSP_CFG_CLOCK_CLK_PLL_HZ                      (800000000)                 /* PLLO: 800MHz */
#endif

#ifndef BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE
 #define BSP_CFG_CLK_PLL_SELECT_STANDBY_MODE           (0)                         /* PLL-STPM: Stop */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SSCG_HZ
 #define BSP_CFG_CLOCK_CLK_SSCG_HZ                     (640000000)                 /* SSCG: 640000000Hz */
#endif

#ifndef BSP_CFG_CLK_SSCG_MODULATION_MODE
 #define BSP_CFG_CLK_SSCG_MODULATION_MODE              (0)                         /* SSCG modulation: Disable */
#endif

#ifndef BSP_CFG_CLK_SSCG_MODULATION_VALUE
 #define BSP_CFG_CLK_SSCG_MODULATION_VALUE             (30)                        /* SSCG0 modulation (20MHz): 20.0 KHz */
#endif

#ifndef BSP_CFG_CLK_SSCG_MODULATION_DITHERED_RANGE
 #define BSP_CFG_CLK_SSCG_MODULATION_DITHERED_RANGE    (0)                         /* SSCG modulation dithered range: -1% */
#endif

#ifndef BSP_CFG_CLOCK_CKS_CLEANC_SRC
 #define BSP_CFG_CLOCK_CKS_CLEANC_SRC                  (0)                         /* CLK_SYS_CLEAN: CLK_PLLO */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SYS_CLEAN_HZ
 #define BSP_CFG_CLOCK_CLK_SYS_CLEAN_HZ                (800000000)                 /* CLK_SYS_CLEAN: 800000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CKS_SSCGC_SRC
 #define BSP_CFG_CLOCK_CKS_SSCGC_SRC                   (0)                         /* CLK_SYS_SSCG: CLK_SSCGO */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ
 #define BSP_CFG_CLOCK_CLK_SYS_SSCG_HZ                 (640000000)                 /* CLK_SYS_SSCG: 640000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CPU_HZ
 #define BSP_CFG_CLOCK_CLK_CPU_HZ                      (320000000)                 /* CLK_CPU: 320000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SBUS_HZ
 #define BSP_CFG_CLOCK_CLK_SBUS_HZ                     (160000000)                 /* CLK_SBUS: 160000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HBUS_HZ
 #define BSP_CFG_CLOCK_CLK_HBUS_HZ                     (80000000)                  /* CLK_HBUS: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_UHSB_HZ
 #define BSP_CFG_CLOCK_CLK_UHSB_HZ                     (160000000)                 /* CLK_UHSB: 160000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_HSB_HZ
 #define BSP_CFG_CLOCK_CLK_HSB_HZ                      (80000000)                  /* CLK_HSB: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_LSB_HZ
 #define BSP_CFG_CLOCK_CLK_LSB_HZ                      (40000000)                  /* CLK_LSB: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_SBUS_HZ
 #define BSP_CFG_CLOCK_CLKC_SBUS_HZ                    (200000000)                 /* CLKC_SBUS: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_HBUS_HZ
 #define BSP_CFG_CLOCK_CLKC_HBUS_HZ                    (100000000)                 /* CLKC_HBUS: 100000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_UHSB_HZ
 #define BSP_CFG_CLOCK_CLKC_UHSB_HZ                    (160000000)                 /* CLKC_UHSB: 160000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_HSB_HZ
 #define BSP_CFG_CLOCK_CLKC_HSB_HZ                     (80000000)                  /* CLKC_HSB: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_LSB_HZ
 #define BSP_CFG_CLOCK_CLKC_LSB_HZ                     (40000000)                  /* CLKC_LSB: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_WDTBA_SRC
 #define BSP_CFG_CLOCK_CLKA_WDTBA_SRC                  (1)                         /* CLKA_WDTBA: CLK_LSIOSC/128 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_WDTBA_HZ
 #define BSP_CFG_CLOCK_CLKA_WDTBA_HZ                   (256)                       /* CLKA_WDTBA: 256Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_TAUJ_SRC
 #define BSP_CFG_CLOCK_CLKA_TAUJ_SRC                   (1)                         /* CLKA_TAUJ: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_TAUJ_HZ
 #define BSP_CFG_CLOCK_CLKA_TAUJ_HZ                    (32768)                     /* CLKA_TAUJ: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_RTCA_SRC
 #define BSP_CFG_CLOCK_CLKA_RTCA_SRC                   (1)                         /* CLKA_RTCA: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_RTCA_HZ
 #define BSP_CFG_CLOCK_CLKA_RTCA_HZ                    (32768)                     /* CLKA_RTCA: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_ADC_SRC
 #define BSP_CFG_CLOCK_CLKA_ADC_SRC                    (1)                         /* CLKA_ADC: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CLKA_ADC_HZ
 #define BSP_CFG_CLOCK_CLKA_ADC_HZ                     (10000000)                  /* CLKA_ADC: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MSPI_SRC
 #define BSP_CFG_CLOCK_CLK_MSPI_SRC                    (2)                         /* CLK_MSPI: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLK_MSPI_HZ
 #define BSP_CFG_CLOCK_CLK_MSPI_HZ                     (80000000)                  /* CLK_MSPI: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN3_SRC
 #define BSP_CFG_CLOCK_CLK_RLIN3_SRC                   (1)                         /* CLK_RLIN3: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN3_HZ
 #define BSP_CFG_CLOCK_CLK_RLIN3_HZ                    (80000000)                  /* CLK_RLIN3: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN323_SRC
 #define BSP_CFG_CLOCK_CLK_RLIN323_SRC                 (2)                         /* CLK_RLIN323: CLK_MOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLK_RLIN323_HZ
 #define BSP_CFG_CLOCK_CLK_RLIN323_HZ                  (20000000)                  /* CLK_RLIN323: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CANFD_C_SRC
 #define BSP_CFG_CLOCK_CLK_CANFD_C_SRC                 (1)                         /* CLK_CANFD_C: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CANFD_C_HZ
 #define BSP_CFG_CLOCK_CLK_CANFD_C_HZ                  (80000000)                  /* CLK_CANFD_C: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC
 #define BSP_CFG_CLOCK_CLK_CANFD_XIN_SRC               (1)                         /* CLK_CANFD_XIN: CLK_MOSC */
#endif

#ifndef BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ
 #define BSP_CFG_CLOCK_CLK_CANFD_XIN_HZ                (20000000)                  /* CLK_CANFD_XIN: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_CANXL_CH0_SRC
 #define BSP_CFG_CLOCK_CLKC_CANXL_CH0_SRC              (1)                         /* CLKC_CANXL_CH0: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ
 #define BSP_CFG_CLOCK_CLKC_CANXL_CH0_HZ               (80000000)                  /* CLKC_CANXL_CH0: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_CANXL_CH1_SRC
 #define BSP_CFG_CLOCK_CLKC_CANXL_CH1_SRC              (1)                         /* CLKC_CANXL_CH1: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ
 #define BSP_CFG_CLOCK_CLKC_CANXL_CH1_HZ               (80000000)                  /* CLKC_CANXL_CH1: 80000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_WDTB_SRC
 #define BSP_CFG_CLOCK_CLK_WDTB_SRC                    (1)                         /* CLK_WDTB: CLK_HSIOSC_DIV_640 */
#endif

#ifndef BSP_CFG_CLOCK_CLK_WDTB_HZ
 #define BSP_CFG_CLOCK_CLK_WDTB_HZ                     (312500)                    /* CLK_WDTB: 312500Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_GTM_SRC
 #define BSP_CFG_CLOCK_CLK_GTM_SRC                     (0)                         /* CLK_GTM: CLKC_SBUS */
#endif

#ifndef BSP_CFG_CLOCK_CLK_GTM_HZ
 #define BSP_CFG_CLOCK_CLK_GTM_HZ                      (200000000)                 /* CLK_GTM: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SWDT_SRC
 #define BSP_CFG_CLOCK_CLK_SWDT_SRC                    (0)                         /* CLK_SWDT: CLK_HSIOSC / 20 */
#endif

#ifndef BSP_CFG_CLOCK_CLK_SWDT_HZ
 #define BSP_CFG_CLOCK_CLK_SWDT_HZ                     (10000000)                  /* CLK_SWDT: 10000000Hz */
#endif

#ifndef BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE
 #define BSP_CFG_CLK_HSOSC_SELECT_STANDBY_MODE         (0)                         /* HSOSC-STPM: Stop */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_SRC
 #define BSP_CFG_CLOCK_EXTCLK0O_SRC                    (4)                         /* EXTCLK0O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_DIV
 #define BSP_CFG_CLOCK_EXTCLK0O_DIV                    0                           /* EXTCLK0 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK0O_HZ
 #define BSP_CFG_CLOCK_EXTCLK0O_HZ                     (0)                         /* EXTCLK0O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_SRC
 #define BSP_CFG_CLOCK_EXTCLK1O_SRC                    (4)                         /* EXTCLK1O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_DIV
 #define BSP_CFG_CLOCK_EXTCLK1O_DIV                    0                           /* EXTCLK1 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_EXTCLK1O_HZ
 #define BSP_CFG_CLOCK_EXTCLK1O_HZ                     (0)                         /* EXTCLK1O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_LSIOSC_HZ         (32768)                     /* CYCLIC_CLK_LSIOSC: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HSIOSC_HZ         (200000000)                 /* CYCLIC_CLK_HSIOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_IOSC_HZ           (200000000)                 /* CYCLIC_CLK_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_IOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_IOSC_HZ       (200000000)                 /* CYCLIC_CLK_SYS_IOSC: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_SRC           (1)                         /* CYCLIC_CLK_EMG: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_EMG_HZ            (10000000)                  /* CYCLIC_CLK_EMG: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_ECMCNT_HZ         (10000000)                  /* CYCLIC_CLK_ECMCNT: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_LPS_HZ           (10000000)                  /* CYCLIC_CLKA_LPS: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MOSC_HZ           (20000000)                  /* CYCLIC_CLK_MOSC: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SOSC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SOSC_HZ           (0)                         /* CYCLIC_CLK_SOSC: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CKS_CLEANC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CKS_CLEANC_SRC        (1)                         /* CYCLIC_CLK_SYS_CLEAN: CLK_SYS_IOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_CLEAN_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_CLEAN_HZ      (200000000)                 /* CYCLIC_CLK_SYS_CLEAN: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CKS_SSCGC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CKS_SSCGC_SRC         (1)                         /* CYCLIC_CLK_SYS_SSCG: CLK_SYS_IOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_SSCG_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SYS_SSCG_HZ       (200000000)                 /* CYCLIC_CLK_SYS_SSCG: 200000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_CPU_HZ            (100000000)                 /* CYCLIC_CLK_CPU: 100000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_SBUS_HZ           (50000000)                  /* CYCLIC_CLK_SBUS: 50000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HBUS_HZ           (25000000)                  /* CYCLIC_CLK_HBUS: 25000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_UHSB_HZ           (50000000)                  /* CYCLIC_CLK_UHSB: 50000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_HSB_HZ            (25000000)                  /* CYCLIC_CLK_HSB: 25000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_LSB_HZ            (12500000)                  /* CYCLIC_CLK_LSB: 12500000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKC_SBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKC_SBUS_HZ          (50000000)                  /* CYCLIC_CLKC_SBUS: 50000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKC_HBUS_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKC_HBUS_HZ          (25000000)                  /* CYCLIC_CLKC_HBUS: 25000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKC_UHSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKC_UHSB_HZ          (40000000)                  /* CYCLIC_CLKC_UHSB: 40000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKC_HSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKC_HSB_HZ           (20000000)                  /* CYCLIC_CLKC_HSB: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKC_LSB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKC_LSB_HZ           (10000000)                  /* CYCLIC_CLKC_LSB: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_SRC        (1)                         /* CYCLIC_CLKA_WDTBA: CLK_LSIOSC/128 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_WDTBA_HZ         (256)                       /* CYCLIC_CLKA_WDTBA: 256Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_SRC         (1)                         /* CYCLIC_CLKA_TAUJ: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_TAUJ_HZ          (32768)                     /* CYCLIC_CLKA_TAUJ: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_SRC         (1)                         /* CYCLIC_CLKA_RTCA: CLK_LSIOSC */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_RTCA_HZ          (32768)                     /* CYCLIC_CLKA_RTCA: 32768Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_SRC          (1)                         /* CYCLIC_CLKA_ADC: CLK_HSIOSC/20 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLKA_ADC_HZ           (10000000)                  /* CYCLIC_CLKA_ADC: 10000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_SRC          (2)                         /* CYCLIC_CLK_MSPI: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_MSPI_HZ           (20000000)                  /* CYCLIC_CLK_MSPI: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_SRC         (1)                         /* CYCLIC_CLK_RLIN3: CLKC_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN3_HZ          (20000000)                  /* CYCLIC_CLK_RLIN3: 20000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_SRC       (1)                         /* CYCLIC_CLK_RLIN323: CLKC_HBUS */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_RLIN323_HZ        (25000000)                  /* CYCLIC_CLK_RLIN323: 25000000Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_SRC          (1)                         /* CYCLIC_CLK_WDTB: CLK_HSIOSC_DIV_640 */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_CLK_WDTB_HZ           (312500)                    /* CYCLIC_CLK_WDTB: 312500Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_SRC          (4)                         /* CYCLIC_EXTCLK0O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_DIV          0                           /* CYCLIC_EXTCLK0 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK0O_HZ           (0)                         /* CYCLIC_EXTCLK0O: 0Hz */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_SRC          (4)                         /* CYCLIC_EXTCLK1O: CLK_HSB */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_DIV          0                           /* CYCLIC_EXTCLK1 Div: Disabled */
#endif

#ifndef BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ
 #define BSP_CFG_CLOCK_CYCLICRUN_EXTCLK1O_HZ           (0)                         /* CYCLIC_EXTCLK1O: 0Hz */
#endif

#endif                                                                             /* BSP_CLOCK_CFG_U2C8_H_ */

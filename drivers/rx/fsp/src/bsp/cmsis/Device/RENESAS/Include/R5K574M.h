/* ${REA_DISCLAIMER_PLACEHOLDER} */

/** @addtogroup Renesas Electronics Corporation
 * @{
 */

/** @addtogroup R5K574M
 * @{
 */

#ifndef R5K574M_H
 #define R5K574M_H

 #ifdef __cplusplus
extern "C" {
 #endif

 #include "system.h"                   /*!< R5K574M System                                                          */

 #ifndef __IM                          /*!< Fallback for older CMSIS versions                                         */
  #define __IM     __I
 #endif
 #ifndef __OM                          /*!< Fallback for older CMSIS versions                                         */
  #define __OM     __O
 #endif
 #ifndef __IOM                         /*!< Fallback for older CMSIS versions                                         */
  #define __IOM    __IO
 #endif

/** @addtogroup Device_Peripheral_clusters
 * @{
 */
 #include "R5K574M/r_canfd_reg.h"
 #include "R5K574M/r_icu_reg.h"
 #include "R5K574M/r_mram_reg.h"
 #include "R5K574M/r_port_reg.h"
 #include "R5K574M/r_rtc_reg.h"
 #include "R5K574M/r_sram_reg.h"
 #include "R5K574M/r_system_reg.h"
 #include "R5K574M/r_sci_b_reg.h"
 #include "R5K574M/r_usb_basic_reg.h"

/** @} */ /* End of group Device_Peripheral_clusters */

/** @addtogroup Device_Peripheral_peripheralAddr
 * @{
 */
 #define R_CANFD_BASE      0x87480000UL
 #define R_ICU_BASE        0x87006000UL
 #define R_MRAM_BASE       0x8709C000UL
 #define R_MSTP_BASE       0x87803000UL
 #define R_RTC_BASE        0x87802000UL
 #define R_PORT0_BASE      0x87C00000UL
 #define R_PORT1_BASE      0x87C00020UL
 #define R_PORT2_BASE      0x87C00040UL
 #define R_PORT3_BASE      0x87C00060UL
 #define R_PORT4_BASE      0x87C00080UL
 #define R_PORT5_BASE      0x87C000A0UL
 #define R_PORT6_BASE      0x87C000C0UL
 #define R_PORT7_BASE      0x87C000E0UL
 #define R_PORT8_BASE      0x87C00100UL
 #define R_PORT9_BASE      0x87C00120UL
 #define R_PORT10_BASE     0x87C00140UL
 #define R_PORT11_BASE     0x87C00160UL
 #define R_PORT12_BASE     0x87C00180UL
 #define R_PORT13_BASE     0x87C001A0UL
 #define R_PORT14_BASE     0x87C001C0UL
 #define R_PORT15_BASE     0x87C001E0UL
 #define R_PORT16_BASE     0x87C00200UL
 #define R_PFS_BASE        0x87C00800UL
 #define R_PMISC_BASE      0x87C00D00UL
 #define R_SCI_B0_BASE     0x87458000UL
 #define R_SCI_B1_BASE     0x87458100UL
 #define R_SCI_B2_BASE     0x87458200UL
 #define R_SCI_B3_BASE     0x87458300UL
 #define R_SCI_B4_BASE     0x87458400UL
 #define R_SCI_B5_BASE     0x87458500UL
 #define R_SCI_B6_BASE     0x87458600UL
 #define R_SCI_B7_BASE     0x87458700UL
 #define R_SCI_B8_BASE     0x87458800UL
 #define R_SCI_B9_BASE     0x87458900UL
 #define R_SCI_B10_BASE    0x87458A00UL
 #define R_SCI_B11_BASE    0x87458B00UL
 #define R_SCI_B12_BASE    0x87458C00UL
 #define R_SRAM_BASE       0x8701C000UL
 #define R_SRAM_NS_BASE    0x0701C000UL
 #define R_SYSTEM_BASE     0x87019000UL
 #define R_USB_FS0_BASE    0x87850000UL

/** @} */ /* End of group Device_Peripheral_peripheralAddr */

/* =========================================================================================================================== */
/* ================                                  Peripheral declaration                                   ================ */
/* =========================================================================================================================== */

/** @addtogroup Device_Peripheral_declaration
 * @{
 */

 #define R_CANFD      ((R_CANFD_Type *) R_CANFD_BASE)
 #define R_ICU        ((R_ICU_Type *) R_ICU_BASE)
 #define R_MRAM       ((R_MRAM_Type *) R_MRAM_BASE)
 #define R_MSTP       ((R_MSTP_Type *) R_MSTP_BASE)
 #define R_PORT0      ((R_PORT0_Type *) R_PORT0_BASE)
 #define R_PORT1      ((R_PORT0_Type *) R_PORT1_BASE)
 #define R_PORT2      ((R_PORT0_Type *) R_PORT2_BASE)
 #define R_PORT3      ((R_PORT0_Type *) R_PORT3_BASE)
 #define R_PORT4      ((R_PORT0_Type *) R_PORT4_BASE)
 #define R_PORT5      ((R_PORT0_Type *) R_PORT5_BASE)
 #define R_PORT6      ((R_PORT0_Type *) R_PORT6_BASE)
 #define R_PORT7      ((R_PORT0_Type *) R_PORT7_BASE)
 #define R_PORT8      ((R_PORT0_Type *) R_PORT8_BASE)
 #define R_PORT9      ((R_PORT0_Type *) R_PORT9_BASE)
 #define R_PORT10     ((R_PORT0_Type *) R_PORT10_BASE)
 #define R_PORT11     ((R_PORT0_Type *) R_PORT11_BASE)
 #define R_PORT12     ((R_PORT0_Type *) R_PORT12_BASE)
 #define R_PORT13     ((R_PORT0_Type *) R_PORT13_BASE)
 #define R_PORT14     ((R_PORT0_Type *) R_PORT14_BASE)
 #define R_PORT15     ((R_PORT0_Type *) R_PORT15_BASE)
 #define R_PORT16     ((R_PORT0_Type *) R_PORT16_BASE)
 #define R_PFS        ((R_PFS_Type *) R_PFS_BASE)
 #define R_PMISC      ((R_PMISC_Type *) R_PMISC_BASE)
 #define R_RTC        ((R_RTC_Type *) R_RTC_BASE)
 #define R_SCI_B0     ((R_SCI_B0_Type *) R_SCI_B0_BASE)
 #define R_SCI_B1     ((R_SCI_B0_Type *) R_SCI_B1_BASE)
 #define R_SCI_B2     ((R_SCI_B0_Type *) R_SCI_B2_BASE)
 #define R_SCI_B3     ((R_SCI_B0_Type *) R_SCI_B3_BASE)
 #define R_SCI_B4     ((R_SCI_B0_Type *) R_SCI_B4_BASE)
 #define R_SCI_B5     ((R_SCI_B0_Type *) R_SCI_B5_BASE)
 #define R_SCI_B6     ((R_SCI_B0_Type *) R_SCI_B6_BASE)
 #define R_SCI_B7     ((R_SCI_B0_Type *) R_SCI_B7_BASE)
 #define R_SCI_B8     ((R_SCI_B0_Type *) R_SCI_B8_BASE)
 #define R_SCI_B9     ((R_SCI_B0_Type *) R_SCI_B9_BASE)
 #define R_SCI_B10    ((R_SCI_B0_Type *) R_SCI_B10_BASE)
 #define R_SCI_B11    ((R_SCI_B0_Type *) R_SCI_B11_BASE)
 #define R_SCI_B12    ((R_SCI_B0_Type *) R_SCI_B12_BASE)
 #define R_SRAM       ((R_SRAM_Type *) R_SRAM_BASE)
 #define R_SRAM_NS    ((R_SRAM_NS_Type *) R_SRAM_NS_BASE)
 #define R_SYSTEM     ((R_SYSTEM_Type *) R_SYSTEM_BASE)
 #define R_USB_FS0    ((R_USB_FS0_Type *) R_USB_FS0_BASE)

/** @} */ /* End of group Device_Peripheral_declaration */

/* =========================================================================================================================== */
/* ================                                 Pos/Mask Cluster Section                                  ================ */
/* =========================================================================================================================== */

/** @addtogroup PosMask_clusters
 * @{
 */
 #include "R5K574M/r_canfd_posmsk.h"
 #include "R5K574M/r_icu_posmsk.h"
 #include "R5K574M/r_mram_posmsk.h"
 #include "R5K574M/r_port_posmsk.h"
 #include "R5K574M/r_rtc_posmsk.h"
 #include "R5K574M/r_sram_posmsk.h"
 #include "R5K574M/r_system_posmsk.h"
 #include "R5K574M/r_sci_b_posmsk.h"
 #include "R5K574M/r_usb_basic_posmsk.h"

/** @} */ /* End of group PosMask_peripherals */

 #ifdef __cplusplus
}
 #endif

#endif                                 /* R5K574M_H */

/** @} */ /* End of group R5K574M */

/** @} */ /* End of group Renesas Electronics Corporation */

/* ${REA_DISCLAIMER_PLACEHOLDER} */

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU_RX74M
 * @{
 **********************************************************************************************************************/

/** @} (end addtogroup BSP_MCU_RX74M) */

#ifndef BSP_OVERRIDE_H
#define BSP_OVERRIDE_H

/***********************************************************************************************************************
 * Includes   <System Includes> , "Project Includes"
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/* Private definition to set enumeration values. */
#define IOPORT_PRV_PFS_PSEL_OFFSET    (24)

/* Define overrides required for this MCU. */
#define BSP_OVERRIDE_IOPORT_CFG_OPTIONS_T
#define BSP_OVERRIDE_IOPORT_PERIPHERAL_T

/***********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

/** Superset of all peripheral functions.  */
typedef enum e_ioport_peripheral
{
    /** Pin will functions as an IO pin */
    IOPORT_PERIPHERAL_IO = 0x00,

    /** Pin will function as a DEBUG pin */
    IOPORT_PERIPHERAL_DEBUG = (0x00UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a RTC(RTCIC) pin */
    IOPORT_PERIPHERAL_RTCIC = (0x00UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a GPT peripheral pin */
    IOPORT_PERIPHERAL_GPT0 = (0x01UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a GPT peripheral pin */
    IOPORT_PERIPHERAL_GPT1 = (0x02UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a GPT peripheral pin */
    IOPORT_PERIPHERAL_GPT2 = (0x03UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a GPT peripheral pin */
    IOPORT_PERIPHERAL_GPT3 = (0x04UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a TMR peripheral pin */
    IOPORT_PERIPHERAL_TMR = (0x05UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a RTC(RTCOUT) peripheral pin */
    IOPORT_PERIPHERAL_RTC = (0x06UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a CAC peripheral pin */
    IOPORT_PERIPHERAL_CAC = (0x07UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a POEG peripheral pin */
    IOPORT_PERIPHERAL_POEG = (0x08UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a ADC (Digital input) peripheral pin */
    IOPORT_PERIPHERAL_ADC = (0x09UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Even RSCI peripheral pin */
    IOPORT_PERIPHERAL_SCI0_2_4_6_8_10_12_0A = (0x0AUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Even RSCI(CTS,DE) peripheral pin */
    IOPORT_PERIPHERAL_SCI0_2_4_6_8_10_12_0B = (0x0BUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Even RSCI HBS and others peripheral pin */
    IOPORT_PERIPHERAL_SCI0_2_4_6_8_10_12_0C = (0x0CUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Odd RSCI peripheral pin */
    IOPORT_PERIPHERAL_SCI1_3_5_7_9_11_0D = (0x0DUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Odd RSCI(CTS) peripheral pin */
    IOPORT_PERIPHERAL_SCI1_3_5_7_9_11_0E = (0x0EUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Odd RSCI HBS and others peripheral pin */
    IOPORT_PERIPHERAL_SCI1_3_5_7_9_11_0F = (0x0FUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a RSPI peripheral pin */
    IOPORT_PERIPHERAL_SPI = (0x10UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a RI3C peripheral pin */
    IOPORT_PERIPHERAL_I3C = (0x11UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a CANFD peripheral pin */
    IOPORT_PERIPHERAL_CANFD = (0x12UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a USB full speed peripheral pin */
    IOPORT_PERIPHERAL_USB0 = (0x13UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a USB high speed peripheral pin */
    IOPORT_PERIPHERAL_USBHS = (0x14UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a DSMIF peripheral pin */
    IOPORT_PERIPHERAL_DSMIF = (0x15UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a PDMIF peripheral pin */
    IOPORT_PERIPHERAL_PDMIF = (0x16UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an SSIE peripheral pin */
    IOPORT_PERIPHERAL_SSIE = (0x17UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an PCIF peripheral pin */
    IOPORT_PERIPHERAL_PCIF = (0x18UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an SDHI peripheral pin */
    IOPORT_PERIPHERAL_SDHI = (0x19UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a XSPI first peripheral pin */
    IOPORT_PERIPHERAL_XSPI0_1A = (0x1AUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a XSPI first peripheral pin */
    IOPORT_PERIPHERAL_XSPI0_1B = (0x1BUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a  XSPI second peripheral pin */
    IOPORT_PERIPHERAL_XSPI1_1A = (0x1AUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a  XSPI second peripheral pin */
    IOPORT_PERIPHERAL_XSPI1_1B = (0x1BUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a PDC peripheral pin */
    IOPORT_PERIPHERAL_PDC = (0x1CUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a CMTW peripheral pin */
    IOPORT_PERIPHERAL_CMTW = (0x1DUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as a clock peripheral pin */
    IOPORT_PERIPHERAL_CLKOUT = (0x1FUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet GMII/MII peripheral pin */
    IOPORT_PERIPHERAL_ETHER_MII_GMII = (0x20UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet RMMI peripheral pin */
    IOPORT_PERIPHERAL_ETHER_RMII = (0x21UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet RGMMI peripheral pin */
    IOPORT_PERIPHERAL_ETHER_RGMII = (0x22UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet PTP peripheral pin */
    IOPORT_PERIPHERAL_ETHER_PTP = (0x23UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet EtherCAT first peripheral pin */
    IOPORT_PERIPHERAL_ESC0 = (0x24UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an Ethernet EtherCAT second peripheral pin */
    IOPORT_PERIPHERAL_ESC1 = (0x25UL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an EXBUS/SDRAM first peripheral pin */
    IOPORT_PERIPHERAL_BUS0 = (0x3DUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an EXBUS/SDRAM second peripheral pin */
    IOPORT_PERIPHERAL_BUS1 = (0x3EUL << IOPORT_PRV_PFS_PSEL_OFFSET),

    /** Pin will function as an 32-bit board function peripheral pin */
    IOPORT_PERIPHERAL_PIO = (0x40UL << IOPORT_PRV_PFS_PSEL_OFFSET),
} ioport_peripheral_t;

/** Options to configure pin functions  */
typedef enum e_ioport_cfg_options
{
    IOPORT_CFG_PORT_DIRECTION_INPUT  = 0x00000000, ///< Sets the pin direction to input (default)
    IOPORT_CFG_PORT_DIRECTION_OUTPUT = 0x00000004, ///< Sets the pin direction to output
    IOPORT_CFG_PORT_OUTPUT_LOW       = 0x00000000, ///< Sets the pin level to low
    IOPORT_CFG_PORT_OUTPUT_HIGH      = 0x00000001, ///< Sets the pin level to high
    IOPORT_CFG_PULLUP_ENABLE         = 0x00000010, ///< Enables the pin's internal pull-up
    IOPORT_CFG_NMOS_ENABLE           = 0x00000040, ///< Enables the pin's NMOS open-drain output
    IOPORT_CFG_DRIVE_1               = 0x00000000, ///< Sets pin drive output to DRV1
    IOPORT_CFG_DRIVE_2               = 0x00000100, ///< Sets pin drive output to DRV2
    IOPORT_CFG_DRIVE_3               = 0x00000200, ///< Sets pin drive output to DRV3
    IOPORT_CFG_DRIVE_4               = 0x00000300, ///< Sets pin drive output to DRV4
    IOPORT_CFG_DRIVE_5               = 0x00000400, ///< Sets pin drive output to DRV5
    IOPORT_CFG_DRIVE_6               = 0x00000500, ///< Sets pin drive output to DRV6
    IOPORT_CFG_DRIVE_7               = 0x00000600, ///< Sets pin drive output to DRV7
    IOPORT_CFG_DRIVE_8               = 0x00000700, ///< Sets pin drive output to DRV8
    IOPORT_CFG_EVENT_RISING_EDGE     = 0x00001000, ///< Sets pin event trigger to rising edge
    IOPORT_CFG_EVENT_FALLING_EDGE    = 0x00002000, ///< Sets pin event trigger to falling edge
    IOPORT_CFG_EVENT_BOTH_EDGES      = 0x00003000, ///< Sets pin event trigger to both edges
    IOPORT_CFG_IRQ_ENABLE            = 0x00004000, ///< Sets pin as an IRQ pin
    IOPORT_CFG_ANALOG_ENABLE         = 0x00008000, ///< Enables pin to operate as an analog pin
    IOPORT_CFG_PERIPHERAL_PIN        = 0x00010000  ///< Enables pin to operate as a peripheral pin
} ioport_cfg_options_t;

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#endif

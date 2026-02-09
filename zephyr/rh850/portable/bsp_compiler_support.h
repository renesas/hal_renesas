/*
* Copyright (c) 2025 - 2026 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/*******************************************************************************************************************//**
 * @addtogroup BSP_COMMON
 * @{
 **********************************************************************************************************************/
#ifndef BSP_COMPILER_SUPPORT_H_
#define BSP_COMPILER_SUPPORT_H_

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/** Stacks (and heap) must be sized and aligned to an integer multiple of this number. */
#define BSP_STACK_ALIGNMENT    (8)           ///< Required alignment boundary (in bytes) for stack and heap
#define BSP_DONT_REMOVE                      ///< Marker macro to prevent removal during optimization / link-time elimination

#define EIPC                   0             ///< Register ID: EIPC
#define EIPSW                  1             ///< Register ID: EIPSW
#define FEPC                   2             ///< Register ID: FEPC
#define FEPSW                  3             ///< Register ID: FEPSW
#define PSW                    5             ///< Register ID: PSW
#define FPSR                   6             ///< Register ID: FPSR
#define FPEPC                  7             ///< Register ID: FPEPC
#define FPST                   8             ///< Register ID: FPST
#define FPCC                   9             ///< Register ID: FPCC
#define FPCFG                  10            ///< Register ID: FPCFG
#define FPEC                   11            ///< Register ID: FPEC
#define EIIC                   13            ///< Register ID: EIIC
#define FEIC                   14            ///< Register ID: FEIC
#define CTPC                   16            ///< Register ID: CTPC
#define CTPSW                  17            ///< Register ID: CTPSW
#define CTBP                   20            ///< Register ID: CTBP
#define EIWR                   28            ///< Register ID: EIWR
#define FEWR                   29            ///< Register ID: FEWR

/** Convert token into a string and pass it to _Pragma */
#ifndef   BSP_PRAGMA_HELPER
 #define BSP_PRAGMA_HELPER(x)    _Pragma(#x) ///< Helper macro: stringize token and pass to _Pragma
#endif

#if defined(__CCRH__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           .OPBT_top                        ///< Section name: option bytes
 #define BSP_PRAGMA_SECTION_DEFAULT               default                          ///< Default section name
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        .CRUN_CODE                       ///< CRUN text section tag (compiler-specific)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        .CRUN_CODE                       ///< CRUN data section tag (compiler-specific)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    .CRUN_CODE                       ///< CRUN ROM-data section tag (compiler-specific)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         .CRUN_CODE                       ///< CRUN BSS section tag (compiler-specific)
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      .CRUN_inttable_PE0               ///< CRUN interrupt table (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           .inttable_PE0                    ///< Interrupt table section (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           .inttable_PE1                    ///< Interrupt table section (PE1)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           .inttable_PE2                    ///< Interrupt table section (PE2)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           .inttable_PE3                    ///< Interrupt table section (PE3)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           .inttable_PE4                    ///< Interrupt table section (PE4)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           .inttable_PE5                    ///< Interrupt table section (PE5)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           .inttable_PE6                    ///< Interrupt table section (PE6)

 #define BSP_PRAGMA_SECTION_CRUN_DATA                                              ///< Placeholder: CRUN data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA                                          ///< Placeholder: CRUN ROM-data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_BSS                                               ///< Placeholder: CRUN BSS section (empty on CCRH)

 #define BSP_PRAGMA_SECTION_DATA                                                   ///< Placeholder: data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_ROM_DATA                                               ///< Placeholder: ROM data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_BSS                                                    ///< Placeholder: BSS section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_TEXT                                                   ///< Placeholder: text/code section (empty on CCRH)
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(section name) ///< Apply section placement via pragma

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                                                   ///< Placeholder: section attribute not applied on CCRH
 #define BSP_FORCE_INLINE                         __attribute__((always_inline))   ///< Force function to inline (CCRH-compatible attribute)
 #define BSP_NO_INLINE                            __attribute__((noinline))        ///< Prevent inlining (CCRH-compatible attribute)

 #define BSP_SECTION_STACK_PE0_BSS                                                 ///< Placeholder: stack section tag for PE0 (empty on CCRH)
 #define BSP_SECTION_STACK_PE1_BSS                                                 ///< Placeholder: stack section tag for PE1 (empty on CCRH)
 #define BSP_SECTION_STACK_PE2_BSS                                                 ///< Placeholder: stack section tag for PE2 (empty on CCRH)
 #define BSP_SECTION_STACK_PE3_BSS                                                 ///< Placeholder: stack section tag for PE3 (empty on CCRH)
 #define BSP_SECTION_STACK_PE4_BSS                                                 ///< Placeholder: stack section tag for PE4 (empty on CCRH)
 #define BSP_SECTION_STACK_PE5_BSS                                                 ///< Placeholder: stack section tag for PE5 (empty on CCRH)
 #define BSP_SECTION_STACK_PE6_BSS                                                 ///< Placeholder: stack section tag for PE6 (empty on CCRH)

 #define BSP_SECTION_INTERRUPT_TABLE_PE0                                           ///< Placeholder: interrupt table tag for PE0 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE1                                           ///< Placeholder: interrupt table tag for PE1 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE2                                           ///< Placeholder: interrupt table tag for PE2 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE3                                           ///< Placeholder: interrupt table tag for PE3 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE4                                           ///< Placeholder: interrupt table tag for PE4 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE5                                           ///< Placeholder: interrupt table tag for PE5 (empty on CCRH)
 #define BSP_SECTION_INTERRUPT_TABLE_PE6                                           ///< Placeholder: interrupt table tag for PE6 (empty on CCRH)

/* Heap section */
 #ifndef BSP_SECTION_HEAP
  #define BSP_SECTION_HEAP                                                         ///< Placeholder: heap section tag (empty on CCRH)
 #endif

/* Stack section */
 #ifndef BSP_SECTION_STACK
  #define BSP_SECTION_STACK            ///< Placeholder: stack section tag (empty on CCRH)
 #endif

#elif defined(__ghs__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           ".OPBT_top"                                ///< Section name: option bytes
 #define BSP_PRAGMA_SECTION_DEFAULT               default                                    ///< Default section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        ".CRUN_CODE.text"                          ///< CRUN text section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        ".CRUN_CODE.sdata"                         ///< CRUN small data section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    ".CRUN_CODE.rosdata"                       ///< CRUN ROM small data section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         ".CRUN_CODE.sbss"                          ///< CRUN small BSS section
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      ".CRUN_inttable_PE0"                       ///< CRUN interrupt table (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           ".inttable_PE0"                            ///< Interrupt table section (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           ".inttable_PE1"                            ///< Interrupt table section (PE1)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           ".inttable_PE2"                            ///< Interrupt table section (PE2)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           ".inttable_PE3"                            ///< Interrupt table section (PE3)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           ".inttable_PE4"                            ///< Interrupt table section (PE4)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           ".inttable_PE5"                            ///< Interrupt table section (PE5)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           ".inttable_PE6"                            ///< Interrupt table section (PE6)

 #define BSP_PRAGMA_SECTION_CRUN_DATA             data                                       ///< Section tag: data
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA         rodata                                     ///< Section tag: read-only data
 #define BSP_PRAGMA_SECTION_CRUN_BSS              bss                                        ///< Section tag: BSS

 #define BSP_PRAGMA_SECTION_DATA                  sdata                                      ///< Section tag: small data
 #define BSP_PRAGMA_SECTION_ROM_DATA              rodata                                     ///< Section tag: small read-only data
 #define BSP_PRAGMA_SECTION_BSS                   sbss                                       ///< Section tag: small BSS
 #define BSP_PRAGMA_SECTION_TEXT                  text                                       ///< Section tag: text/code
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(ghs section sec = name) ///< Place symbol in section

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                                                             ///< Placeholder: attribute-free placement (handled via pragma in GHS)
 #define BSP_FORCE_INLINE                                                                    ///< Placeholder: inlining behavior per compiler settings
 #define BSP_NO_INLINE                                                                       ///< Placeholder: noinline not explicitly specified

 #define BSP_SECTION_STACK_PE0_BSS                                                           ///< Placeholder: stack section tag for PE0
 #define BSP_SECTION_STACK_PE1_BSS                                                           ///< Placeholder: stack section tag for PE1
 #define BSP_SECTION_STACK_PE2_BSS                                                           ///< Placeholder: stack section tag for PE2
 #define BSP_SECTION_STACK_PE3_BSS                                                           ///< Placeholder: stack section tag for PE3
 #define BSP_SECTION_STACK_PE4_BSS                                                           ///< Placeholder: stack section tag for PE4
 #define BSP_SECTION_STACK_PE5_BSS                                                           ///< Placeholder: stack section tag for PE5
 #define BSP_SECTION_STACK_PE6_BSS                                                           ///< Placeholder: stack section tag for PE6

 #define BSP_SECTION_INTERRUPT_TABLE_PE0                                                     ///< Placeholder: interrupt table tag for PE0
 #define BSP_SECTION_INTERRUPT_TABLE_PE1                                                     ///< Placeholder: interrupt table tag for PE1
 #define BSP_SECTION_INTERRUPT_TABLE_PE2                                                     ///< Placeholder: interrupt table tag for PE2
 #define BSP_SECTION_INTERRUPT_TABLE_PE3                                                     ///< Placeholder: interrupt table tag for PE3
 #define BSP_SECTION_INTERRUPT_TABLE_PE4                                                     ///< Placeholder: interrupt table tag for PE4
 #define BSP_SECTION_INTERRUPT_TABLE_PE5                                                     ///< Placeholder: interrupt table tag for PE5
 #define BSP_SECTION_INTERRUPT_TABLE_PE6                                                     ///< Placeholder: interrupt table tag for PE6

/* Heap section */
 #ifndef BSP_SECTION_HEAP
  #define BSP_SECTION_HEAP                                                                   ///< Placeholder: heap section tag
 #endif

/* Stack section */
 #ifndef BSP_SECTION_STACK
  #define BSP_SECTION_STACK            ///< Placeholder: stack section tag
 #endif

#elif defined(__GNUC__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           ".OPBT_top"                                            ///< Section name: option bytes
 #define BSP_PRAGMA_SECTION_DEFAULT               "default"                                              ///< Default section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        ".CRUN_CODE.text"                                      ///< CRUN text section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        ".CRUN_CODE.data"                                      ///< CRUN data section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    ".CRUN_CODE.rodata"                                    ///< CRUN read-only data section
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         ".CRUN_CODE.bss"                                       ///< CRUN BSS section
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           ".inttable_PE0.data"                                   ///< Interrupt table section (PE0)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           ".inttable_PE1.data"                                   ///< Interrupt table section (PE1)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           ".inttable_PE2.data"                                   ///< Interrupt table section (PE2)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           ".inttable_PE3.data"                                   ///< Interrupt table section (PE3)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           ".inttable_PE4.data"                                   ///< Interrupt table section (PE4)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           ".inttable_PE5.data"                                   ///< Interrupt table section (PE5)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           ".inttable_PE6.data"                                   ///< Interrupt table section (PE6)

 #define BSP_PRAGMA_SECTION_CRUN_DATA             data                                                   ///< Generic data section tag
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA         rodata                                                 ///< Generic read-only data section tag
 #define BSP_PRAGMA_SECTION_CRUN_BSS              bss                                                    ///< Generic BSS section tag

 #define BSP_PRAGMA_SECTION_DATA                  data                                                   ///< Data section
 #define BSP_PRAGMA_SECTION_ROM_DATA              rodata                                                 ///< Read-only data section
 #define BSP_PRAGMA_SECTION_BSS                   bss                                                    ///< BSS section
 #define BSP_PRAGMA_SECTION_TEXT                  text                                                   ///< Text/code section
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(clang section sec = name)           ///< Place symbol in section

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                   __attribute__((section(x))) __attribute__((__used__)) ///< Apply section attribute and keep symbol
 #define BSP_FORCE_INLINE                         __attribute__((always_inline))                         ///< Force inline for functions
 #define BSP_NO_INLINE                            __attribute__((noinline))                              ///< Prevent function inlining

 #define BSP_SECTION_STACK_PE0_BSS                ".pe0_stack.bss"                                       ///< Section name for stack (PE0)
 #define BSP_SECTION_STACK_PE1_BSS                ".pe1_stack.bss"                                       ///< Section name for stack (PE1)
 #define BSP_SECTION_STACK_PE2_BSS                ".pe2_stack.bss"                                       ///< Section name for stack (PE2)
 #define BSP_SECTION_STACK_PE3_BSS                ".pe3_stack.bss"                                       ///< Section name for stack (PE3)
 #define BSP_SECTION_STACK_PE4_BSS                ".pe4_stack.bss"                                       ///< Section name for stack (PE4)
 #define BSP_SECTION_STACK_PE5_BSS                ".pe5_stack.bss"                                       ///< Section name for stack (PE5)
 #define BSP_SECTION_STACK_PE6_BSS                ".pe6_stack.bss"                                       ///< Section name for stack (PE6)

 #define BSP_SECTION_INTERRUPT_TABLE_PE0          ".inttable_PE0.data"                                   ///< Section name for interrupt table (PE0)
 #define BSP_SECTION_INTERRUPT_TABLE_PE1          ".inttable_PE1.data"                                   ///< Section name for interrupt table (PE1)
 #define BSP_SECTION_INTERRUPT_TABLE_PE2          ".inttable_PE2.data"                                   ///< Section name for interrupt table (PE2)
 #define BSP_SECTION_INTERRUPT_TABLE_PE3          ".inttable_PE3.data"                                   ///< Section name for interrupt table (PE3)
 #define BSP_SECTION_INTERRUPT_TABLE_PE4          ".inttable_PE4.data"                                   ///< Section name for interrupt table (PE4)
 #define BSP_SECTION_INTERRUPT_TABLE_PE5          ".inttable_PE5.data"                                   ///< Section name for interrupt table (PE5)
 #define BSP_SECTION_INTERRUPT_TABLE_PE6          ".inttable_PE6.data"                                   ///< Section name for interrupt table (PE6)

/* Heap section */
 #ifndef BSP_SECTION_HEAP
  #define BSP_SECTION_HEAP                        ".heap"                                                ///< Section name for heap
 #endif

/* Stack section */
 #ifndef BSP_SECTION_STACK
  #define BSP_SECTION_STACK                       ".stack" ///< Section name for stack
 #endif
#endif

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

#if defined(__CCRH__)
 #define DI()                         __DI()                      ///< Disable interrupts
 #define EI()                         __EI()                      ///< Enable interrupts
 #define HALT()                       __halt()                    ///< Halt CPU execution
 #define NOP()                        __nop()                     ///< No-operation instruction
 #define SYNCP()                      __syncp()                   ///< Synchronize pipeline
 #define SCH1R(x)                     __sch1r((x))                ///< SCH1R instruction wrapper
 #define LDSR(regid, ret)             __ldsr((regid), (ret))      ///< Load system register
 #define STSR(regid)                  __stsr((regid))             ///< Store system register
 #define STSR_REGSEL(regID, selID)    __stsr_rh((regID), (selID)) ///< Store system register with selector

#elif defined(__ghs__)
unsigned int __SCH1R(unsigned int val);                           ///< GHS: extern wrapper for SCH1R
unsigned int __STSR(int regID, int selID);                        ///< GHS: extern wrapper for STSR
void         __LDSR(int regID, int selID, unsigned int val);      ///< GHS: extern wrapper for LDSR

 #define DI()                         __asm("di")                 ///< Disable interrupts
 #define EI()                         __asm("ei")                 ///< Enable interrupts
 #define HALT()                       __asm("halt")               ///< Halt CPU execution
 #define NOP()                        __asm("nop")                ///< No-operation
 #define SYNCP()                      __asm("syncp")              ///< Synchronize pipeline
 #define SCH1R(x)                     __SCH1R(x)                  ///< SCH1R intrinsic function on GHS
 #define LDSR(regid, ret)             __LDSR(regid, 0, ret)       ///< Load system register (selID=0)
 #define STSR(regID)                  __STSR(regID, 0)            ///< Store system register (selID=0)
 #define STSR_REGSEL(regID, selID)    __STSR(regID, selID)        ///< Store system register with selector

#elif defined(__GNUC__)
 #define DI()                         __asm("di")                 ///< Disable interrupts
 #define EI()                         __asm("ei")                 ///< Enable interrupts
 #define HALT()                       __asm("halt")               ///< Halt CPU execution
 #define NOP()                        __asm("nop")                ///< No-operation
 #define SYNCP()                      __asm("syncp")              ///< Synchronize pipeline

/** Execute SCH1R and return result */
 #define SCH1R(x)                     ({uint32_t __result; \
                                        __asm__ volatile ("sch1r %1, %0" : "=r"(__result) : "r"(x) : "cc");                 \
                                        __result;})

/** Load system register */
 #define LDSR(regid, ret)             __asm__ volatile ("ldsr %0, %1" : : "r" (ret), "i" (regid) : "cc", "memory")

/** Read system register into GPR and return value */
 #define STSR(regID)                  ({uint32_t __result; \
                                        __asm__ volatile ("stsr %1, %0" : "=r" (__result) : "i" (regID));                 \
                                        __result;})

/** Read system register with selector and return value */
 #define STSR_REGSEL(regID,                                \
                     selID)           ({uint32_t __result; \
                                        __asm__ volatile ("stsr %1, %0, %2" : "=r" (__result) : "i" (regID), "i" (selID));                 \
                                        __result;})

#elif defined(__ICCRH850__)
 #define DI()                         __disable_interrupt()        ///< Disable interrupts
 #define EI()                         __enable_interrupt()         ///< Enable interrupts
 #define HALT()                       __halt()                     ///< Halt CPU execution
 #define NOP()                        __no_operation()             ///< No-operation
 #define SYNCP()                      __SYNCP()                    ///< Synchronize pipeline
 #define SCH1R(x)                     __SCH1R((x))                 ///< SCH1R instruction wrapper
 #define LDSR(regid, ret)             __LDSR((regid), (ret))       ///< Load system register
 #define STSR(regid)                  __STSR((regid), 0)           ///< Store system register
 #define STSR_REGSEL(regID, selID)    __STSR((regID), (selID))     ///< Store system register with selector
#endif

/**
 * @brief   Get Interrupt Mask
 * @details Returns the current state of the interrupt mask bit from the Interrupt Mask Register.
 * @return  Interrupt Mask value
 */
#if defined(__CCRH__)
 #pragma inline_asm __get_INTMASK
static uint32_t __get_INTMASK (void)
{
    stsr  5, r10, 0; r10 < -PSW
    shl  26, r10
    shr  31, r10
}

#elif defined(__ghs__) || defined(__GNUC__) || defined(__ICCRH850__)
__STATIC_INLINE uint32_t __get_INTMASK (void)
{
    uint32_t isrmask;

    __asm__ volatile (
        "stsr  5, %0, 0       \n"      /* Read the PSW register */
        "shl   26, %0         \n"      /* Get the bit 5 */
        "shr   31, %0         \n"
        : "=&r"(isrmask)
        :
        : "memory"
    );

    return isrmask;
}

#endif

/**
 * @brief   Set Interrupt Mask
 * @details Assigns the given value to the Interrupt Mask Register.
 * @param [in]    priMask  Interrupt Mask
 */
__STATIC_INLINE void __set_INTMASK (uint32_t priMask)
{
    if (priMask)
    {
        DI();
    }
    else
    {
        EI();
    }
}

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_COMMON)
 **********************************************************************************************************************/

#endif                                 /* BSP_COMPILER_SUPPORT_H_ */

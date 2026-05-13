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
#define BSP_STACK_ALIGNMENT    (8)
#define BSP_DONT_REMOVE

/** Convert token into a string and pass it to _Pragma */
#ifndef   BSP_PRAGMA_HELPER
 #define BSP_PRAGMA_HELPER(x)        _Pragma(#x)                 ///< Helper macro: stringize token and pass to _Pragma
#endif

#define DI()                         __DI()                      ///< Disable interrupts
#define EI()                         __EI()                      ///< Enable interrupts
#define HALT()                       __HALT()                    ///< Halt CPU execution
#define NOP()                        __NOP()                     ///< No-operation instruction
#define SYNCP()                      __SYNCP()                   ///< Synchronize pipeline
#define SCH1R(x)                     __SCH1R((x))                ///< Bit (1) search from LSB side
#define LDSR(regid, ret)             __LDSR((regid), (0), (ret)) ///< Load system register
#define STSR(regid)                  __STSR((regid), (0))        ///< Store system register
#define STSR_REGSEL(regID, selID)    __STSR((regID), (selID))    ///< Store system register with selector

#if defined(__CCRH__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           .OPBT_top                        ///< Section name: option bytes (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_DEFAULT               default                          ///< Default section name (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        .CRUN_CODE                       ///< CRUN text section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        .CRUN_CODE                       ///< CRUN data section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    .CRUN_CODE                       ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         .CRUN_CODE                       ///< CRUN BSS section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      .CRUN_inttable_PE0               ///< CRUN interrupt table (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           .inttable_PE0                    ///< Interrupt table section (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           .inttable_PE1                    ///< Interrupt table section (PE1) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           .inttable_PE2                    ///< Interrupt table section (PE2) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           .inttable_PE3                    ///< Interrupt table section (PE3) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           .inttable_PE4                    ///< Interrupt table section (PE4) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           .inttable_PE5                    ///< Interrupt table section (PE5) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           .inttable_PE6                    ///< Interrupt table section (PE6) (used for BSP_PRAGMA_PLACE_IN_SECTION)

 #define BSP_PRAGMA_SECTION_CRUN_DATA                                              ///< Placeholder: CRUN data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA                                          ///< Placeholder: CRUN ROM-data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_BSS                                               ///< Placeholder: CRUN BSS section (empty on CCRH)

 #define BSP_PRAGMA_SECTION_DATA                                                   ///< Placeholder: data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_ROM_DATA                                               ///< Placeholder: ROM data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_BSS                                                    ///< Placeholder: BSS section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_TEXT                                                   ///< Placeholder: text/code section (empty on CCRH)
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(section name) ///< Apply section placement via pragma

 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_SMALL                                         ///< Small  memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_MEDIUM                                        ///< Medium memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE                                         ///< Large  memory attribute
 #define BSP_PRAGMA_TYPE_ATTRIBUTE(name)                                           ///< Apply type attribute via pragma

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                   __attribute__((section(x)))     ///< Apply section attribute and keep symbol
 #define BSP_FORCE_INLINE                         __attribute__((always_inline))   ///< Force function to inline (CCRH-compatible attribute)
 #define BSP_NO_INLINE                            __attribute__((noinline))        ///< Prevent inlining (CCRH-compatible attribute)
 #define BSP_ALIGN_VARIABLE(x)                     __attribute__((aligned(x)))     ///< Force variable alignment to x-byte boundary (CCRH-compatible attribute)

 #define BSP_SECTION_STACK_PE0_BSS                ".pe0_stack.bss"                 ///< Section name for stack (PE0)
 #define BSP_SECTION_STACK_PE1_BSS                ".pe1_stack.bss"                 ///< Section name for stack (PE1)
 #define BSP_SECTION_STACK_PE2_BSS                ".pe2_stack.bss"                 ///< Section name for stack (PE2)
 #define BSP_SECTION_STACK_PE3_BSS                ".pe3_stack.bss"                 ///< Section name for stack (PE3)
 #define BSP_SECTION_STACK_PE4_BSS                ".pe4_stack.bss"                 ///< Section name for stack (PE4)
 #define BSP_SECTION_STACK_PE5_BSS                ".pe5_stack.bss"                 ///< Section name for stack (PE5)
 #define BSP_SECTION_STACK_PE6_BSS                ".pe6_stack.bss"                 ///< Section name for stack (PE6)

 #define BSP_SECTION_INTERRUPT_TABLE_PE0          ".inttable_PE0.const"            ///< Section name for interrupt table (PE0)
 #define BSP_SECTION_INTERRUPT_TABLE_PE1          ".inttable_PE1.const"            ///< Section name for interrupt table (PE1)
 #define BSP_SECTION_INTERRUPT_TABLE_PE2          ".inttable_PE2.const"            ///< Section name for interrupt table (PE2)
 #define BSP_SECTION_INTERRUPT_TABLE_PE3          ".inttable_PE3.const"            ///< Section name for interrupt table (PE3)
 #define BSP_SECTION_INTERRUPT_TABLE_PE4          ".inttable_PE4.const"            ///< Section name for interrupt table (PE4)
 #define BSP_SECTION_INTERRUPT_TABLE_PE5          ".inttable_PE5.const"            ///< Section name for interrupt table (PE5)
 #define BSP_SECTION_INTERRUPT_TABLE_PE6          ".inttable_PE6.const"            ///< Section name for interrupt table (PE6)

 #define BSP_SECTION_OPTION_BYTE                  ".OPBT_top"                      ///< Section name: option bytes (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_TEXT               ".CRUN_CODE.text"                ///< CRUN text section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_DATA               ".CRUN_CODE.data"                ///< CRUN data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_ROM_DATA           ".CRUN_CODE.const"               ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_BSS                ".CRUN_CODE.bss"                 ///< CRUN BSS section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_INTVECT_PE0             ".CRUN_inttable_PE0.const"       ///< CRUN interrupt table (PE0) (used for BSP_PLACE_IN_SECTION)

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
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_DEFAULT               default                                                ///< Default section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        ".CRUN_CODE.text"                                      ///< CRUN text section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        ".CRUN_CODE.sdata"                                     ///< CRUN small data section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    ".CRUN_CODE.rosdata"                                   ///< CRUN ROM small data section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         ".CRUN_CODE.sbss"                                      ///< CRUN small BSS section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      ".CRUN_inttable_PE0"                                   ///< CRUN interrupt table (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           ".inttable_PE0"                                        ///< Interrupt table section (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           ".inttable_PE1"                                        ///< Interrupt table section (PE1) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           ".inttable_PE2"                                        ///< Interrupt table section (PE2) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           ".inttable_PE3"                                        ///< Interrupt table section (PE3) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           ".inttable_PE4"                                        ///< Interrupt table section (PE4) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           ".inttable_PE5"                                        ///< Interrupt table section (PE5) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           ".inttable_PE6"                                        ///< Interrupt table section (PE6) (used for BSP_PRAGMA_PLACE_IN_SECTION)

 #define BSP_PRAGMA_SECTION_CRUN_DATA             data                                                   ///< Section tag: data
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA         rosdata                                                ///< Section tag: read-only data
 #define BSP_PRAGMA_SECTION_CRUN_BSS              bss                                                    ///< Section tag: BSS

 #define BSP_PRAGMA_SECTION_DATA                  sdata                                                  ///< Section tag: small data
 #define BSP_PRAGMA_SECTION_ROM_DATA              rosdata                                                ///< Section tag: small read-only data
 #define BSP_PRAGMA_SECTION_BSS                   sbss                                                   ///< Section tag: small BSS
 #define BSP_PRAGMA_SECTION_TEXT                  text                                                   ///< Section tag: text/code
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(ghs section sec = name)             ///< Place symbol in section

 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_SMALL                                                               ///< Small  memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_MEDIUM                                                              ///< Medium memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE                                                               ///< Large  memory attribute
 #define BSP_PRAGMA_TYPE_ATTRIBUTE(name)                                                                 ///< Apply type attribute via pragma

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                   __attribute__((section(x))) __attribute__((__used__)) ///< Apply section attribute and keep symbol
 #define BSP_FORCE_INLINE                         __attribute__((always_inline))                         ///< Force inline for functions
 #define BSP_NO_INLINE                            __attribute__((noinline))                              ///< Prevent function inlining
 #define BSP_ALIGN_VARIABLE(x)                     __attribute__((aligned(x)))                           ///< Force variable alignment to x-byte boundary

 #define BSP_SECTION_STACK_PE0_BSS                ".pe0_stack.bss"                                       ///< Section name for stack (PE0)
 #define BSP_SECTION_STACK_PE1_BSS                ".pe1_stack.bss"                                       ///< Section name for stack (PE1)
 #define BSP_SECTION_STACK_PE2_BSS                ".pe2_stack.bss"                                       ///< Section name for stack (PE2)
 #define BSP_SECTION_STACK_PE3_BSS                ".pe3_stack.bss"                                       ///< Section name for stack (PE3)
 #define BSP_SECTION_STACK_PE4_BSS                ".pe4_stack.bss"                                       ///< Section name for stack (PE4)
 #define BSP_SECTION_STACK_PE5_BSS                ".pe5_stack.bss"                                       ///< Section name for stack (PE5)
 #define BSP_SECTION_STACK_PE6_BSS                ".pe6_stack.bss"                                       ///< Section name for stack (PE6)

 #define BSP_SECTION_INTERRUPT_TABLE_PE0          ".inttable_PE0"                                        ///< Section name for interrupt table (PE0)
 #define BSP_SECTION_INTERRUPT_TABLE_PE1          ".inttable_PE1"                                        ///< Section name for interrupt table (PE1)
 #define BSP_SECTION_INTERRUPT_TABLE_PE2          ".inttable_PE2"                                        ///< Section name for interrupt table (PE2)
 #define BSP_SECTION_INTERRUPT_TABLE_PE3          ".inttable_PE3"                                        ///< Section name for interrupt table (PE3)
 #define BSP_SECTION_INTERRUPT_TABLE_PE4          ".inttable_PE4"                                        ///< Section name for interrupt table (PE4)
 #define BSP_SECTION_INTERRUPT_TABLE_PE5          ".inttable_PE5"                                        ///< Section name for interrupt table (PE5)
 #define BSP_SECTION_INTERRUPT_TABLE_PE6          ".inttable_PE6"                                        ///< Section name for interrupt table (PE6)

 #define BSP_SECTION_OPTION_BYTE                  ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_TEXT               ".CRUN_CODE.text"                                      ///< CRUN text section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_DATA               ".CRUN_CODE.sdata"                                     ///< CRUN data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_ROM_DATA           ".CRUN_CODE.rosdata"                                   ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_BSS                ".CRUN_CODE.sbss"                                      ///< CRUN BSS section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_INTVECT_PE0             ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0) (used for BSP_PLACE_IN_SECTION)

/* Heap section */
 #ifndef BSP_SECTION_HEAP
  #define BSP_SECTION_HEAP                                                                               ///< Placeholder: heap section tag
 #endif

/* Stack section */
 #ifndef BSP_SECTION_STACK
  #define BSP_SECTION_STACK            ///< Placeholder: stack section tag
 #endif

#elif defined(__GNUC__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_DEFAULT               "default"                                              ///< Default section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        ".CRUN_CODE.text"                                      ///< CRUN text section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        ".CRUN_CODE.data"                                      ///< CRUN data section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    ".CRUN_CODE.const"                                     ///< CRUN read-only data section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         ".CRUN_CODE.bss"                                       ///< CRUN BSS section (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           ".inttable_PE0.data"                                   ///< Interrupt table section (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           ".inttable_PE1.data"                                   ///< Interrupt table section (PE1) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           ".inttable_PE2.data"                                   ///< Interrupt table section (PE2) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           ".inttable_PE3.data"                                   ///< Interrupt table section (PE3) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           ".inttable_PE4.data"                                   ///< Interrupt table section (PE4) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           ".inttable_PE5.data"                                   ///< Interrupt table section (PE5) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           ".inttable_PE6.data"                                   ///< Interrupt table section (PE6) (used for BSP_PRAGMA_PLACE_IN_SECTION)

 #define BSP_PRAGMA_SECTION_CRUN_DATA             data                                                   ///< Generic data section tag
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA         rodata                                                 ///< Generic read-only data section tag
 #define BSP_PRAGMA_SECTION_CRUN_BSS              bss                                                    ///< Generic BSS section tag

 #define BSP_PRAGMA_SECTION_DATA                  data                                                   ///< Data section
 #define BSP_PRAGMA_SECTION_ROM_DATA              rodata                                                 ///< Read-only data section
 #define BSP_PRAGMA_SECTION_BSS                   bss                                                    ///< BSS section
 #define BSP_PRAGMA_SECTION_TEXT                  text                                                   ///< Text/code section
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(clang section sec = name)           ///< Place symbol in section

 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_SMALL                                                               ///< Small  memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_MEDIUM                                                              ///< Medium memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE                                                               ///< Large  memory attribute
 #define BSP_PRAGMA_TYPE_ATTRIBUTE(name)                                                                 ///< Apply type attribute via pragma

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                   __attribute__((section(x))) __attribute__((__used__)) ///< Apply section attribute and keep symbol
 #define BSP_FORCE_INLINE                         __attribute__((always_inline))                         ///< Force inline for functions
 #define BSP_NO_INLINE                            __attribute__((noinline))                              ///< Prevent function inlining
 #define BSP_ALIGN_VARIABLE(x)                     __attribute__((aligned(x)))                           ///< Force variable alignment to x-byte boundary

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

 #define BSP_SECTION_OPTION_BYTE                  ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_TEXT               ".CRUN_CODE.text"                                      ///< CRUN text section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_DATA               ".CRUN_CODE.data"                                      ///< CRUN data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_ROM_DATA           ".CRUN_CODE.const"                                     ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_BSS                ".CRUN_CODE.bss"                                       ///< CRUN BSS section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_INTVECT_PE0             ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0) (used for BSP_PLACE_IN_SECTION)

/* Heap section */
 #ifndef BSP_SECTION_HEAP
  #define BSP_SECTION_HEAP                        ".heap"                                                ///< Section name for heap
 #endif

/* Stack section */
 #ifndef BSP_SECTION_STACK
  #define BSP_SECTION_STACK                       ".stack" ///< Section name for stack
 #endif

#elif defined(__ICCRH850__)

/* Create the pragma inline statement with the function name as argument */
 #define BSP_PRAGMA_SECTION_OPTION_BYTE           ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_DEFAULT               "default"                                              ///< Default section name (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_TEXT        ".CRUN_CODE.text"                                      ///< CRUN text section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_DATA        ".CRUN_CODE.data"                                      ///< CRUN data section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_ROM_DATA    ".CRUN_CODE.rodata"                                    ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_CODE_BSS         ".CRUN_CODE.bss"                                       ///< CRUN BSS section tag (compiler-specific) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_CRUN_INTVECT_PE0      ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE0           ".inttable_PE0.data"                                   ///< Interrupt table section (PE0) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE1           ".inttable_PE1.data"                                   ///< Interrupt table section (PE1) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE2           ".inttable_PE2.data"                                   ///< Interrupt table section (PE2) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE3           ".inttable_PE3.data"                                   ///< Interrupt table section (PE3) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE4           ".inttable_PE4.data"                                   ///< Interrupt table section (PE4) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE5           ".inttable_PE5.data"                                   ///< Interrupt table section (PE5) (used for BSP_PRAGMA_PLACE_IN_SECTION)
 #define BSP_PRAGMA_SECTION_INTVECT_PE6           ".inttable_PE6.data"                                   ///< Interrupt table section (PE6) (used for BSP_PRAGMA_PLACE_IN_SECTION)

 #define BSP_PRAGMA_SECTION_CRUN_DATA             default_variable_attributes                            ///< Placeholder: CRUN data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_ROM_DATA         default_variable_attributes                            ///< Placeholder: CRUN ROM-data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_CRUN_BSS              default_variable_attributes                            ///< Placeholder: CRUN BSS section (empty on CCRH)

 #define BSP_PRAGMA_SECTION_DATA                  default_variable_attributes                            ///< Placeholder: data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_ROM_DATA              default_variable_attributes                            ///< Placeholder: ROM data section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_BSS                   default_variable_attributes                            ///< Placeholder: BSS section (empty on CCRH)
 #define BSP_PRAGMA_SECTION_TEXT                  default_function_attributes                            ///< Placeholder: text/code section (empty on CCRH)
 #define BSP_PRAGMA_PLACE_IN_SECTION(sec, name)    BSP_PRAGMA_HELPER(sec = @ name)                       ///< Apply section placement via pragma

 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_SMALL        __brel                                                 ///< Small  memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_MEDIUM       __brel23                                               ///< Medium memory attribute
 #define BSP_PRAGMA_MEMORY_ATTRIBUTE_LARGE        __huge                                                 ///< Large  memory attribute
 #define BSP_PRAGMA_TYPE_ATTRIBUTE(name)           BSP_PRAGMA_HELPER(type_attribute = name)              ///< Apply type attribute via pragma

/* Compiler neutral macros. */
 #define BSP_PLACE_IN_SECTION(x)                   __attribute__((section(x))) __attribute__((__used__)) ///< Apply section attribute and keep symbol
 #define BSP_FORCE_INLINE                         __attribute__((always_inline)) static __inline         ///< Force inline for functions
 #define BSP_NO_INLINE                            __attribute__((noinline))                              ///< Prevent function inlining
 #define BSP_ALIGN_VARIABLE(x)                     __attribute__((aligned(x)))                           ///< Force variable alignment to x-byte boundary

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

 #define BSP_SECTION_OPTION_BYTE                  ".OPBT_top"                                            ///< Section name: option bytes (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_TEXT               ".CRUN_CODE.text"                                      ///< CRUN text section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_DATA               ".CRUN_CODE.data"                                      ///< CRUN data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_ROM_DATA           ".CRUN_CODE.rodata"                                    ///< CRUN ROM-data section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_CODE_BSS                ".CRUN_CODE.bss"                                       ///< CRUN BSS section tag (compiler-specific) (used for BSP_PLACE_IN_SECTION)
 #define BSP_SECTION_CRUN_INTVECT_PE0             ".CRUN_inttable_PE0.data"                              ///< CRUN interrupt table (PE0) (used for BSP_PLACE_IN_SECTION)

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

/*******************************************************************************************************************//**
 * @} (end addtogroup BSP_COMMON)
 **********************************************************************************************************************/

#endif                                 /* BSP_COMPILER_SUPPORT_H_ */

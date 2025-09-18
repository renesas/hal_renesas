/***********************************************************************************************************************
 * Copyright [2020-2025] Renesas Electronics Corporation and/or its affiliates. All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics Corporation and/or its affiliates. and may only
 * be used with products of Renesas Electronics Corp. and its affiliates ("Renesas"). No other uses are authorized.
 * Renesas products are sold pursuant to Renesas terms and conditions of sale. Purchasers are solely responsible for
 * the selection and use of Renesas products and Renesas assumes no liability. No license, express or implied, to any
 * intellectual property right is granted by Renesas. This software is protected under all applicable laws, including
 * copyright laws. Renesas reserves the right to change or discontinue this software and/or this documentation.
 * THE SOFTWARE AND DOCUMENTATION IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND
 * TO THE FULLEST EXTENT PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY,
 * INCLUDING WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE
 * SOFTWARE OR DOCUMENTATION. RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.
 * TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR
 * DOCUMENTATION (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER,
 * INCLUDING, WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY
 * LOST PROFITS, OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE
 * POSSIBILITY OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @addtogroup BSP_MCU
 * @{
 **********************************************************************************************************************/

#ifndef BSP_COMPILER_SUPPORT_H
 #define BSP_COMPILER_SUPPORT_H

 #ifdef __cplusplus
extern "C" {
 #endif

/***********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/
 #if defined(__CCRX__)                 /* CCRX compiler */
  #define BSP_SECTION_HEAP                   "__period__heap"
  #define BSP_DONT_REMOVE
  #define BSP_ATTRIBUTE_STACKLESS            __attribute__((naked))
  #define BSP_FORCE_INLINE                   __attribute__((always_inline))
 #elif defined(__GNUC__)               /* GCC compiler */
  #define BSP_SECTION_HEAP                   ".heap"
  #define BSP_DONT_REMOVE
  #define BSP_ATTRIBUTE_STACKLESS            __attribute__((naked))
  #define BSP_FORCE_INLINE                   __attribute__((always_inline))
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define BSP_SECTION_HEAP                   "HEAP"
  #define BSP_DONT_REMOVE                    __root
  #define BSP_ATTRIBUTE_STACKLESS            __attribute__((naked))
  #define BSP_FORCE_INLINE                   _Pragma("inline=forced")
 #endif
 #define BSP_UNINIT_SECTION_PREFIX

 #if defined(__CCRX__)                 /* CCRX compiler */
  #define BSP_SECTION_STACK                  BSP_UNINIT_SECTION_PREFIX "__period__stack"
  #define BSP_SECTION_NOINIT                 BSP_UNINIT_SECTION_PREFIX "__period__noinit"
  #define BSP_SECTION_FIXED_VECTORS          "__period__fixed_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      "__period__interrupt_vectors"
  #define BSP_SECTION_APPLICATION_VECTORS    "__period__application_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      "__period__interrupt_vectors"
  #define BSP_SECTION_RESET_VECTOR           "__period__reset_vector"
  #define BSP_SECTION_ROM_REGISTERS          "__period__rom_registers"
  #define BSP_SECTION_ID_CODE                "__period__id_code"
 #elif defined(__GNUC__)               /* GCC compiler */
  #define BSP_SECTION_STACK                  BSP_UNINIT_SECTION_PREFIX ".stack"
  #define BSP_SECTION_NOINIT                 BSP_UNINIT_SECTION_PREFIX ".noinit"
  #define BSP_SECTION_FIXED_VECTORS          ".fixed_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      ".interrupt_vectors"
  #define BSP_SECTION_APPLICATION_VECTORS    ".application_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      ".interrupt_vectors"
  #define BSP_SECTION_RESET_VECTOR           ".reset_vector"
  #define BSP_SECTION_ROM_REGISTERS          ".rom_registers"
  #define BSP_SECTION_ID_CODE                ".id_code"
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define BSP_SECTION_STACK                  BSP_UNINIT_SECTION_PREFIX ".stack"
  #define BSP_SECTION_NOINIT                 BSP_UNINIT_SECTION_PREFIX ".noinit"
  #define BSP_SECTION_FIXED_VECTORS          ".fixed_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      ".interrupt_vectors"
  #define BSP_SECTION_APPLICATION_VECTORS    ".application_vectors"
  #define BSP_SECTION_INTERRUPT_VECTORS      ".interrupt_vectors"
  #define BSP_SECTION_RESET_VECTOR           ".reset_vector"
  #define BSP_SECTION_ROM_REGISTERS          ".rom_registers"
  #define BSP_SECTION_ID_CODE                ".id_code"
 #endif

/* Compiler neutral macros. */
 #if defined(__CCRX__)                 /* CCRX compiler */
  #define BSP_PLACE_IN_SECTION(x)    __attribute__((section(x)))
  #define __PACKED_STRUCT    struct
 #elif defined(__GNUC__)               /* GCC compiler */
  #define BSP_PLACE_IN_SECTION(x)    __attribute__((section(x))) __attribute__((__used__))
  #define __PACKED_STRUCT    struct __attribute__((__packed__))
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define BSP_PLACE_IN_SECTION(x)    __attribute__((section(x))) __attribute__((__used__))
  #define __PACKED_STRUCT    __packed struct
 #endif

 #define BSP_ALIGN_VARIABLE(x)       __attribute__((aligned(x)))

 #define BSP_PACKED             __attribute__((aligned(1))) // DEPRECATED

 #define BSP_WEAK_REFERENCE     __attribute__((weak))

 #if defined(__CCRX__)                                      /* CCRX compiler */
  #define __STATIC_INLINE       static inline
 #elif defined(__GNUC__)                                    /* GCC compiler */
  #define __STATIC_INLINE       static __inline
 #elif defined(__ICCRX__)                                   /* IAR compiler */
  #define __STATIC_INLINE       static __inline
 #endif

/** Stacks (and heap) must be sized and aligned to an integer multiple of this number. */
 #define BSP_STACK_ALIGNMENT    (8)

/* Inline assembler macros. */
 #if defined(__CCRX__)                 /* CCRX compiler */
  #define __BKPT(x)        __brk()
  #define __SET_ISP(x)     __set_isp((void *) x)
  #define __SET_EXTB(x)    __set_extb(x)
  #define __SET_INTB(x)    __set_intb(x)
  #define __SETPSW_I()     __setpsw_i()
  #define __CLRPSW_I()     __clrpsw_i()
  #define __GET_PSW(x)     x = __get_psw()
  #define __SET_PSW(x)     __set_psw(x)
  #define __NOP()          __nop()
  #define __WAIT()         __wait()
 #elif defined(__GNUC__)               /* GCC scompiler */
  #define __BKPT(x)        __asm volatile ("brk" : : :)
  #define __SET_ISP(x)     __asm volatile ("mvtc %0, isp" : : "r" (x) :)
  #define __SET_EXTB(x)    __asm volatile ("mvtc %0, extb" : : "r" ((uint32_t) x) :)
  #define __SET_INTB(x)    __asm volatile ("mvtc %0, intb" : : "r" ((uint32_t) x) :)
  #define __SETPSW_I()     __asm volatile ("setpsw i" : : :)
  #define __CLRPSW_I()     __asm volatile ("clrpsw i" : : :)
  #define __GET_PSW(x)     __asm volatile ("mvfc psw,%0" : "=r" (x))
  #define __SET_PSW(x)     __asm volatile ("mvtc %0,psw" ::"r" (x))
  #define __NOP()          __asm volatile ("nop")
  #define __WAIT()         __asm volatile ("wait")
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define __BKPT(x)        __asm volatile ("brk" : : :)
  #define __SET_ISP(x)     __asm volatile ("mvtc %0, isp" : : "r" (x) :)
  #define __SET_EXTB(x)    __asm volatile ("mvtc %0, extb" : : "r" ((uint32_t) x) :)
  #define __SET_INTB(x)    __asm volatile ("mvtc %0, intb" : : "r" ((uint32_t) x) :)
  #define __SETPSW_I()     __asm volatile ("setpsw i" : : :)
  #define __CLRPSW_I()     __asm volatile ("clrpsw i" : : :)
  #define __GET_PSW(x)     __asm volatile ("mvfc psw,%0" : "=r" (x))
  #define __SET_PSW(x)     __asm volatile ("mvtc %0,psw" ::"r" (x))
  #define __NOP()          __asm volatile ("nop")
  #define __WAIT()         __asm volatile ("wait")
 #endif

 #if defined(__CCRX__)                 /* CCRX compiler */
  #define BSP_ATTRIBUTE_ISR
  #define BSP_ATTRIBUTE_KEEP
 #elif defined(__GNUC__)               /* GCC compiler */
  #define BSP_ATTRIBUTE_ISR     __attribute__((naked, noinline))
  #define BSP_ATTRIBUTE_KEEP    static
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define BSP_ATTRIBUTE_ISR     __attribute__((naked, noinline))
  #define BSP_ATTRIBUTE_KEEP    static
 #endif

/**
 * Renesas v3 ISA does not support hardware-specific barrier instruction.
 * Using GCC "__sync_synchronize" built-in function to issues a full memory barrier.
 */
 #if defined(__CCRX__)                 /* CCRX compiler */
  #define __DMB(x)
 #elif defined(__GNUC__)               /* GCC compiler */
  #define __DMB(x)    __sync_synchronize()
 #elif defined(__ICCRX__)              /* IAR compiler */
  #define __DMB(x)
 #endif

/***********************************************************************************************************************
 * Exported global variables
 **********************************************************************************************************************/

/***********************************************************************************************************************
 * Exported global functions (to be accessed by other files)
 **********************************************************************************************************************/

/** @} (end of addtogroup BSP_MCU) */

 #ifdef __cplusplus
}
 #endif

#endif

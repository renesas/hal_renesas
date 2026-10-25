/*
 * Copyright (c) 2026 Renesas Electronics Corporation
 * SPDX-License-Identifier: Apache-2.0
 */

/*
 * memcpy() for the Armv8-R AArch32 cores (Cortex-R52), A32 code.
 *
 * Unaligned LDR/STR/LDRH/STRH are used on Normal memory (SCTLR.A == 0).
 * Copies of 64 bytes or more align the destination to 8 bytes and then move
 * 32 bytes per iteration: LDM/STM of eight registers, or VLD1/VST1 of four
 * D registers with CONFIG_RCAR_GEN5_R52_FAST_MEMCPY_NEON. The NEON loop is
 * only built with CONFIG_FPU_SHARING, since memcpy() runs in every thread
 * and ISR and their FP registers must be saved on first use.
 */

#if !defined(__ARM_FEATURE_UNALIGNED)
#error "memcpy_armv8r.S needs unaligned access support"
#endif

#if defined(CONFIG_RCAR_GEN5_R52_FAST_MEMCPY_NEON) && !defined(CONFIG_FPU_SHARING)
#error "CONFIG_RCAR_GEN5_R52_FAST_MEMCPY_NEON needs CONFIG_FPU_SHARING"
#endif

	.syntax	unified
	.arch	armv8-r
#if defined(CONFIG_RCAR_GEN5_R52_FAST_MEMCPY_NEON)
	.fpu	neon-fp-armv8
#endif
	.arm

	.section .text.memcpy, "ax", %progbits
	.global	memcpy
	.global	__aeabi_memcpy
	.global	__aeabi_memcpy4
	.global	__aeabi_memcpy8
	.type	memcpy, %function
	.type	__aeabi_memcpy, %function
	.type	__aeabi_memcpy4, %function
	.type	__aeabi_memcpy8, %function
	.p2align 5

/* r0: destination (returned), r1: source, r2: length; ip: destination cursor */
memcpy:
__aeabi_memcpy:
__aeabi_memcpy4:
__aeabi_memcpy8:
	mov	ip, r0
	cmp	r2, #64
	blo	.Lsmall

	/* Align the destination to 8 bytes: 1, 2 and 4 byte steps */
	ands	r3, ip, #7
	beq	.Lbulk
	rsb	r3, r3, #8
	sub	r2, r2, r3
	tst	r3, #1
	ldrbne	r0, [r1], #1
	strbne	r0, [ip], #1
	tst	r3, #2
	ldrhne	r0, [r1], #2
	strhne	r0, [ip], #2
	tst	r3, #4
	ldrne	r0, [r1], #4
	strne	r0, [ip], #4
	sub	r0, ip, r3

.Lbulk:
	/* At least 57 bytes left, destination 8-byte aligned */
	sub	r2, r2, #32
#if defined(CONFIG_RCAR_GEN5_R52_FAST_MEMCPY_NEON)
1:	vld1.8	{d0-d3}, [r1]!
	pld	[r1, #64]
	subs	r2, r2, #32
	vst1.64	{d0-d3}, [ip :64]!
	bhs	1b
#else
	push	{r4-r10, lr}
	tst	r1, #3
	bne	2f
1:	ldm	r1!, {r3-r10}
	pld	[r1, #64]
	subs	r2, r2, #32
	stm	ip!, {r3-r10}
	bhs	1b
	b	3f
2:	ldr	r3, [r1], #4
	ldr	r4, [r1], #4
	ldr	r5, [r1], #4
	ldr	r6, [r1], #4
	ldr	r7, [r1], #4
	ldr	r8, [r1], #4
	ldr	r9, [r1], #4
	ldr	r10, [r1], #4
	pld	[r1, #64]
	subs	r2, r2, #32
	stm	ip!, {r3-r10}
	bhs	2b
3:	pop	{r4-r10, lr}
#endif
	add	r2, r2, #32

.Lsmall:
	/* Fewer than 64 bytes: words, then the last 0 to 3 bytes */
	subs	r2, r2, #4
	blo	.Ltail
1:	ldr	r3, [r1], #4
	subs	r2, r2, #4
	str	r3, [ip], #4
	bhs	1b

.Ltail:
	/* r2 + 4 bytes left: bit 0 selects a byte, bit 1 a halfword */
	lsls	r2, r2, #31
	ldrbne	r3, [r1], #1
	strbne	r3, [ip], #1
	ldrhcs	r3, [r1]
	strhcs	r3, [ip]
	bx	lr

	.size	memcpy, . - memcpy
	.size	__aeabi_memcpy, . - __aeabi_memcpy
	.size	__aeabi_memcpy4, . - __aeabi_memcpy4
	.size	__aeabi_memcpy8, . - __aeabi_memcpy8

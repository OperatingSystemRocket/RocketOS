	.file	"kmath.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/kmath.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/kmath.o -Og -Wall -Wextra
/  -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes
/  -Wcast-qual -Wconversion -Wwrite-strings -Wredundant-decls
/  -Wnested-externs -Winline -Wno-long-long -Wpedantic -std=gnu17
/  -ffreestanding -fverbose-asm
/  options enabled:  -faggressive-loop-optimizations -fallocation-dce
/  -fauto-inc-dec -fcombine-stack-adjustments -fcompare-elim
/  -fcprop-registers -fdefer-pop -fdelete-null-pointer-checks
/  -fdwarf2-cfi-asm -fearly-inlining -feliminate-unused-debug-symbols
/  -feliminate-unused-debug-types -fforward-propagate
/  -ffp-int-builtin-inexact -ffunction-cse -fgcse-lm -fgnu-unique
/  -fguess-branch-probability -fident -finline -finline-atomics
/  -fipa-profile -fipa-pure-const -fipa-reference
/  -fipa-reference-addressable -fipa-stack-alignment -fira-hoist-pressure
/  -fira-share-save-slots -fira-share-spill-slots -fivopts
/  -fkeep-static-consts -fleading-underscore -flifetime-dse -fmath-errno
/  -fmerge-constants -fmerge-debug-strings -fpcc-struct-return -fpeephole
/  -fplt -fprefetch-loop-arrays -freorder-blocks
/  -fsched-critical-path-heuristic -fsched-dep-count-heuristic
/  -fsched-group-heuristic -fsched-interblock -fsched-last-insn-heuristic
/  -fsched-rank-heuristic -fsched-spec -fsched-spec-insn-heuristic
/  -fsched-stalled-insns-dep -fschedule-fusion -fsemantic-interposition
/  -fshow-column -fshrink-wrap -fshrink-wrap-separate -fsigned-zeros
/  -fsplit-ivs-in-unroller -fsplit-wide-types -fssa-backprop -fstdarg-opt
/  -fstrict-volatile-bitfields -fsync-libcalls -ftoplevel-reorder
/  -ftrapping-math -ftree-builtin-call-dce -ftree-ccp -ftree-ch
/  -ftree-coalesce-vars -ftree-copy-prop -ftree-cselim -ftree-dce
/  -ftree-dominator-opts -ftree-forwprop -ftree-fre -ftree-loop-if-convert
/  -ftree-loop-im -ftree-loop-ivcanon -ftree-loop-optimize
/  -ftree-parallelize-loops= -ftree-phiprop -ftree-reassoc
/  -ftree-scev-cprop -ftree-sink -ftree-slsr -ftree-ter -funit-at-a-time
/  -fverbose-asm -fzero-initialized-in-bss -m32 -m80387 -m96bit-long-double
/  -malign-stringops -mavx256-split-unaligned-load
/  -mavx256-split-unaligned-store -mfancy-math-387 -mfp-ret-in-387
/  -mieee-fp -mlong-double-80 -mno-red-zone -mno-sse4 -mpush-args -msahf
/  -mstv -mvzeroupper

	.text
	.globl	ksignbit
	.type	ksignbit, @function
ksignbit:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
/  src/kmath.c:6:     bit = bit & 0b00000001;
	movzbl	8(%ebp), %eax	/  val, <retval>
	shrb	$7, %al	/ , <retval>
/  src/kmath.c:9: }
	popl	%ebp	/ 
	ret	
	.size	ksignbit, .-ksignbit
	.globl	max
	.type	max, @function
max:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$12, %esp	/ ,
	movl	8(%ebp), %edi	/  x, x
	movl	12(%ebp), %esi	/  x, x
	movl	16(%ebp), %ecx	/  y, y
	movl	20(%ebp), %ebx	/  y, y
/  src/kmath.c:13:     return x ^ ((x ^ y) & -(x < y));
	movl	%edi, %eax	/  x, _1
	xorl	%ecx, %eax	/  y, _1
	movl	%esi, %edx	/  x, _1
	xorl	%ebx, %edx	/  y, _1
	movl	%edx, -20(%ebp)	/  _1, %sfp
/  src/kmath.c:13:     return x ^ ((x ^ y) & -(x < y));
	movl	$1, %edx	/ , tmp91
	cmpl	%ecx, %edi	/  y, x
	movl	%esi, %ecx	/  x, tmp106
	sbbl	%ebx, %ecx	/  y, tmp106
	jl	.L4	/ ,
	movl	$0, %edx	/ , tmp91
.L4:
/  src/kmath.c:13:     return x ^ ((x ^ y) & -(x < y));
	negl	%edx	/  tmp92
	movl	%edx, %ebx	/  tmp92, _5
	sarl	$31, %ebx	/ , _5
/  src/kmath.c:13:     return x ^ ((x ^ y) & -(x < y));
	andl	%edx, %eax	/  tmp92, _6
	movl	-20(%ebp), %edx	/  %sfp, _1
	andl	%ebx, %edx	/  _5, _1
/  src/kmath.c:13:     return x ^ ((x ^ y) & -(x < y));
	xorl	%edi, %eax	/  x, tmp104
	xorl	%esi, %edx	/  x, tmp105
/  src/kmath.c:14: }
	addl	$12, %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	max, .-max
	.globl	__divdi3
	.globl	kabs
	.type	kabs, @function
kabs:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$36, %esp	/ ,
	movl	8(%ebp), %ecx	/  val, val
	movl	12(%ebp), %ebx	/  val, val
/  src/kmath.c:33:     int8_t bit = val & 0b10000000;
	movl	%ecx, %eax	/  val, _1
/  src/kmath.c:35:     bit = bit & 0b00000001;
	shrb	$7, %al	/ , bit
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	movsbl	%al, %eax	/  bit, bit
	addl	%eax, %eax	/  _3
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	movl	$1, %esi	/ , tmp100
	subl	%eax, %esi	/  _3, tmp99
	movl	%esi, %eax	/  tmp99, _5
	cltd
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	imull	%ecx, %edx	/  val, tmp101
	movl	%ebx, %eax	/  val, tmp102
	imull	%esi, %eax	/  tmp99, tmp102
	addl	%edx, %eax	/  tmp101, tmp103
	movl	%eax, -28(%ebp)	/  tmp103, %sfp
	movl	%esi, %eax	/  tmp99, _6
	mull	%ecx	/  val
	movl	%eax, %esi	/  _6, _6
	movl	%edx, %edi	/  _6, _6
	addl	-28(%ebp), %edi	/  %sfp, _6
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	pushl	%ebx	/  val
	pushl	%ecx	/  val
	call	kabs	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	movl	$0, %edx	/ , _7
	pushl	%edx	/  _7
	pushl	%eax	/  _7
	pushl	$0	/ 
	pushl	$1	/ 
	call	max	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:36:     bit = -1*(bit * 2 - 1) * val / max(1, kabs(val));
	pushl	%edx	/ 
	pushl	%eax	/  tmp113
	pushl	%edi	/  _6
	pushl	%esi	/  _6
	call	__divdi3	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:38:     return bit;
	movsbl	%al, %eax	/  _10, bit
/  src/kmath.c:39: }
	leal	-12(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	kabs, .-kabs
	.globl	algebraic_sign
	.type	algebraic_sign, @function
algebraic_sign:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
/  src/kmath.c:42:     return kabs(val) / max(1,kabs(val)); // I really didn't want to define algbraic sign in terms of abs, but it makes it a lot easier
	subl	$8, %esp	/ ,
	movsbl	8(%ebp), %eax	/  val, val
	cltd
	pushl	%edx	/  val
	pushl	%eax	/  val
	call	kabs	/ 
	addl	$16, %esp	/ ,
	movl	%eax, %esi	/  tmp99, _3
	movl	$0, %edi	/ , _3
/  src/kmath.c:42:     return kabs(val) / max(1,kabs(val)); // I really didn't want to define algbraic sign in terms of abs, but it makes it a lot easier
	pushl	%edi	/  _3
	pushl	%eax	/  _3
	pushl	$0	/ 
	pushl	$1	/ 
	call	max	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:42:     return kabs(val) / max(1,kabs(val)); // I really didn't want to define algbraic sign in terms of abs, but it makes it a lot easier
	pushl	%edx	/ 
	pushl	%eax	/  tmp100
	pushl	%edi	/  _3
	pushl	%esi	/  _3
	call	__divdi3	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:43: }
	leal	-8(%ebp), %esp	/ ,
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	algebraic_sign, .-algebraic_sign
	.ident	"GCC: (GNU) 10.2.0"

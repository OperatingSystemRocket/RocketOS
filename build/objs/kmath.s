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
/  src/kmath.c:4:     return (val >= 0) ? 0 : 1;
	movzbl	8(%ebp), %eax	/  val, tmp86
	shrb	$7, %al	/ , tmp86
/  src/kmath.c:5: }
	popl	%ebp	/ 
	ret	
	.size	ksignbit, .-ksignbit
	.globl	kmax
	.type	kmax, @function
kmax:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	16(%ebp), %ebx	/  y, y
	movl	20(%ebp), %ecx	/  y, y
/  src/kmath.c:8:     return (x >= y) ? x : y;
	movl	8(%ebp), %eax	/  x, x
	movl	12(%ebp), %edx	/  x, tmp91
	cmpl	%ebx, %eax	/  y, x
	movl	%edx, %esi	/  tmp91, tmp94
	sbbl	%ecx, %esi	/  y, tmp94
	jge	.L4	/ ,
	movl	%ebx, %eax	/  y, x
	movl	%ecx, %edx	/  y, tmp91
.L4:
/  src/kmath.c:9: }
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
	.size	kmax, .-kmax
	.globl	kabs
	.type	kabs, @function
kabs:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	12(%ebp), %esi	/  val, val
/  src/kmath.c:12:     return (val >= 0) ? val : -val;
	movl	%esi, %eax	/  val, tmp91
	sarl	$31, %eax	/ , tmp91
	movl	%eax, %ecx	/  tmp91, tmp86
	movl	%eax, %edi	/  tmp86, tmp92
	xorl	8(%ebp), %edi	/  val, tmp92
	movl	%edi, %eax	/  tmp92, tmp87
	subl	%ecx, %eax	/  tmp86, tmp88
/  src/kmath.c:13: }
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	kabs, .-kabs
	.globl	ksign
	.type	ksign, @function
ksign:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$8, %esp	/ ,
	movl	8(%ebp), %eax	/  val, val
/  src/kmath.c:16:     if(val == 0) return 0;
	testb	%al, %al	/  val
	je	.L8	/ ,
/  src/kmath.c:17:     return (kabs(val) > 0) ? 1 : -1;
	subl	$8, %esp	/ ,
	movsbl	%al, %eax	/  val, val
	cltd
	pushl	%edx	/  val
	pushl	%eax	/  val
	call	kabs	/ 
	addl	$16, %esp	/ ,
/  src/kmath.c:17:     return (kabs(val) > 0) ? 1 : -1;
	testl	%eax, %eax	/  tmp90
	je	.L11	/ ,
	movl	$1, %eax	/ , <retval>
.L8:
/  src/kmath.c:18: }
	leave	
	ret	
.L11:
/  src/kmath.c:17:     return (kabs(val) > 0) ? 1 : -1;
	movl	$-1, %eax	/ , <retval>
	jmp	.L8	/ 
	.size	ksign, .-ksign
	.globl	min
	.type	min, @function
min:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	8(%ebp), %ebx	/  x, x
	movl	12(%ebp), %ecx	/  x, x
/  src/kmath.c:21:     return (x < y) ? x : y;
	movl	16(%ebp), %eax	/  y, y
	movl	20(%ebp), %edx	/  y, tmp91
	cmpl	%eax, %ebx	/  y, x
	movl	%ecx, %esi	/  x, tmp94
	sbbl	%edx, %esi	/  tmp91, tmp94
	jge	.L14	/ ,
	movl	%ebx, %eax	/  x, y
	movl	%ecx, %edx	/  x, tmp91
.L14:
/  src/kmath.c:22: }
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
	.size	min, .-min
	.globl	max
	.type	max, @function
max:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	8(%ebp), %ebx	/  x, x
	movl	12(%ebp), %ecx	/  x, x
/  src/kmath.c:25:     return (x > y) ? x : y;
	movl	16(%ebp), %eax	/  y, y
	movl	20(%ebp), %edx	/  y, tmp91
	cmpl	%ebx, %eax	/  x, y
	movl	%edx, %esi	/  tmp91, tmp94
	sbbl	%ecx, %esi	/  x, tmp94
	jge	.L17	/ ,
	movl	%ebx, %eax	/  x, y
	movl	%ecx, %edx	/  x, tmp91
.L17:
/  src/kmath.c:26: }
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
	.size	max, .-max
	.ident	"GCC: (GNU) 10.2.0"

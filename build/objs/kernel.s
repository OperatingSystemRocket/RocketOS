	.file	"kernel.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/kernel.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/kernel.o -Og -Wall -Wextra
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
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"Hello String"
.LC1:
	.string	"Hello %c %i %s\n"
.LC2:
	.string	"this is a test of scrolling\n"
.LC3:
	.string	"%i\n"
.LC4:
	.string	"src/kernel.c"
.LC5:
	.string	"%4[file: %s :: line: %i]: "
.LC6:
	.string	"Condition Failed: "
.LC7:
	.string	"1==2"
	.text
	.globl	kernel_main
	.type	kernel_main, @function
kernel_main:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$8, %esp	/ ,
/  src/kernel.c:7:     terminal_initialize();
	call	terminal_initialize	/ 
/  src/kernel.c:9:     kprintf("Hello %c %i %s\n", 'c', 5, "Hello String");
	pushl	$.LC0	/ 
	pushl	$5	/ 
	pushl	$99	/ 
	pushl	$.LC1	/ 
	call	kprintf	/ 
/  src/kernel.c:10:     terminal_writestring("this is a test of scrolling\n");
	movl	$.LC2, (%esp)	/ ,
	call	terminal_writestring	/ 
/  src/kernel.c:14:     terminal_putchar_color('c', VGA_COLOR_CYAN);
	addl	$8, %esp	/ ,
	pushl	$3	/ 
	pushl	$99	/ 
	call	terminal_putchar_color	/ 
/  src/kernel.c:15:     terminal_putchar('t');
	movl	$116, (%esp)	/ ,
	call	terminal_putchar	/ 
/  src/kernel.c:16:     terminal_putchar('\n');
	movl	$10, (%esp)	/ ,
	call	terminal_putchar	/ 
/  src/kernel.c:19:     kprintf("%i\n", ksign(0));
	movl	$0, (%esp)	/ ,
	call	ksign	/ 
/  src/kernel.c:19:     kprintf("%i\n", ksign(0));
	addl	$8, %esp	/ ,
	movsbl	%al, %eax	/  tmp99, _1
	pushl	%eax	/  _1
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:21:     kprintf("%i\n", ksign(1));
	movl	$1, (%esp)	/ ,
	call	ksign	/ 
/  src/kernel.c:21:     kprintf("%i\n", ksign(1));
	addl	$8, %esp	/ ,
	movsbl	%al, %eax	/  tmp100, _3
	pushl	%eax	/  _3
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:22:     kprintf("%i\n", ksign(2));
	movl	$2, (%esp)	/ ,
	call	ksign	/ 
/  src/kernel.c:22:     kprintf("%i\n", ksign(2));
	addl	$8, %esp	/ ,
	movsbl	%al, %eax	/  tmp101, _5
	pushl	%eax	/  _5
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:23:     kprintf("%i\n", ksign(-3));
	movl	$-3, (%esp)	/ ,
	call	ksign	/ 
/  src/kernel.c:23:     kprintf("%i\n", ksign(-3));
	addl	$8, %esp	/ ,
	movsbl	%al, %eax	/  tmp102, _7
	pushl	%eax	/  _7
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:25:     kprintf("%i\n", ksign(3));
	movl	$3, (%esp)	/ ,
	call	ksign	/ 
/  src/kernel.c:25:     kprintf("%i\n", ksign(3));
	addl	$8, %esp	/ ,
	movsbl	%al, %eax	/  tmp103, _9
	pushl	%eax	/  _9
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:27:     kprintf("%i\n", kmax(13,15));
	pushl	$0	/ 
	pushl	$15	/ 
	pushl	$0	/ 
	pushl	$13	/ 
	call	kmax	/ 
	addl	$28, %esp	/ ,
	pushl	%edx	/ 
	pushl	%eax	/  tmp104
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:28:     kprintf("%i\n", kmin(13,15));
	pushl	$0	/ 
	pushl	$15	/ 
	pushl	$0	/ 
	pushl	$13	/ 
	call	kmin	/ 
	addl	$28, %esp	/ ,
	pushl	%edx	/ 
	pushl	%eax	/  tmp105
	pushl	$.LC3	/ 
	call	kprintf	/ 
/  src/kernel.c:34:     kassert_void(1==2);
	addl	$12, %esp	/ ,
	pushl	$34	/ 
	pushl	$.LC4	/ 
	pushl	$.LC5	/ 
	call	kprintf	/ 
/  src/kernel.c:34:     kassert_void(1==2);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC6	/ 
	call	terminal_writestring_color	/ 
/  src/kernel.c:34:     kassert_void(1==2);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC7	/ 
	call	terminal_writestring_color	/ 
/  src/kernel.c:34:     kassert_void(1==2);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$10	/ 
	call	terminal_putchar_color	/ 
	addl	$16, %esp	/ ,
/  src/kernel.c:36: }
	leave	
	ret	
	.size	kernel_main, .-kernel_main
	.ident	"GCC: (GNU) 10.2.0"

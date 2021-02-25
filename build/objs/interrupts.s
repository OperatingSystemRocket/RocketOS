	.file	"interrupts.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/interrupts.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/interrupts.o -Og -Wall
/  -Wextra -Wundef -Wshadow -Wpointer-arith -Wcast-align
/  -Wstrict-prototypes -Wcast-qual -Wconversion -Wwrite-strings
/  -Wredundant-decls -Wnested-externs -Winline -Wno-long-long -Wpedantic
/  -std=gnu17 -ffreestanding -fverbose-asm
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
	.globl	irq0_handler
	.type	irq0_handler, @function
irq0_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:18: }
/NO_APP
	ret	
	.size	irq0_handler, .-irq0_handler
	.globl	irq1_handler
	.type	irq1_handler, @function
irq1_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:22: }
/NO_APP
	ret	
	.size	irq1_handler, .-irq1_handler
	.globl	irq2_handler
	.type	irq2_handler, @function
irq2_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:26: }
/NO_APP
	ret	
	.size	irq2_handler, .-irq2_handler
	.globl	irq3_handler
	.type	irq3_handler, @function
irq3_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:30: }
/NO_APP
	ret	
	.size	irq3_handler, .-irq3_handler
	.globl	irq4_handler
	.type	irq4_handler, @function
irq4_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:34: }
/NO_APP
	ret	
	.size	irq4_handler, .-irq4_handler
	.globl	irq5_handler
	.type	irq5_handler, @function
irq5_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:38: }
/NO_APP
	ret	
	.size	irq5_handler, .-irq5_handler
	.globl	irq6_handler
	.type	irq6_handler, @function
irq6_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:42: }
/NO_APP
	ret	
	.size	irq6_handler, .-irq6_handler
	.globl	irq7_handler
	.type	irq7_handler, @function
irq7_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:46: }
/NO_APP
	ret	
	.size	irq7_handler, .-irq7_handler
	.globl	irq8_handler
	.type	irq8_handler, @function
irq8_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:51: }
/NO_APP
	ret	
	.size	irq8_handler, .-irq8_handler
	.globl	irq9_handler
	.type	irq9_handler, @function
irq9_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:56: }
/NO_APP
	ret	
	.size	irq9_handler, .-irq9_handler
	.globl	irq10_handler
	.type	irq10_handler, @function
irq10_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:61: }
/NO_APP
	ret	
	.size	irq10_handler, .-irq10_handler
	.globl	irq11_handler
	.type	irq11_handler, @function
irq11_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:66: }
/NO_APP
	ret	
	.size	irq11_handler, .-irq11_handler
	.globl	irq12_handler
	.type	irq12_handler, @function
irq12_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:71: }
/NO_APP
	ret	
	.size	irq12_handler, .-irq12_handler
	.globl	irq13_handler
	.type	irq13_handler, @function
irq13_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:76: }
/NO_APP
	ret	
	.size	irq13_handler, .-irq13_handler
	.globl	irq14_handler
	.type	irq14_handler, @function
irq14_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:81: }
/NO_APP
	ret	
	.size	irq14_handler, .-irq14_handler
	.globl	irq15_handler
	.type	irq15_handler, @function
irq15_handler:
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$32, %eax	/ , tmp82
/APP
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp82,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp82,
/  0 "" 2
/  src/interrupts.c:86: }
/NO_APP
	ret	
	.size	irq15_handler, .-irq15_handler
	.globl	idt_init
	.type	idt_init, @function
idt_init:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$36, %esp	/ ,
/  src/interrupts.c:90:     asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
	movl	$17, %eax	/ , tmp150
/APP
/  90 "src/interrupts.c" 1
	outb %al, $32	/  tmp150,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $160	/  tmp150,
/  0 "" 2
/NO_APP
	movl	$32, %eax	/ , tmp152
/APP
/  90 "src/interrupts.c" 1
	outb %al, $33	/  tmp152,
/  0 "" 2
/NO_APP
	movl	$40, %eax	/ , tmp153
/APP
/  90 "src/interrupts.c" 1
	outb %al, $161	/  tmp153,
/  0 "" 2
/NO_APP
	movl	$4, %eax	/ , tmp154
/APP
/  90 "src/interrupts.c" 1
	outb %al, $33	/  tmp154,
/  0 "" 2
/NO_APP
	movl	$2, %eax	/ , tmp155
/APP
/  90 "src/interrupts.c" 1
	outb %al, $161	/  tmp155,
/  0 "" 2
/NO_APP
	movl	$1, %eax	/ , tmp156
/APP
/  90 "src/interrupts.c" 1
	outb %al, $33	/  tmp156,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $161	/  tmp156,
/  0 "" 2
/NO_APP
	movl	$0, %eax	/ , tmp158
/APP
/  90 "src/interrupts.c" 1
	outb %al, $33	/  tmp158,
/  0 "" 2
/  90 "src/interrupts.c" 1
	outb %al, $161	/  tmp158,
/  0 "" 2
/  src/interrupts.c:147:     irq0_address = (unsigned long)irq0;
/NO_APP
	movl	$irq0, %edx	/ , irq0_address
/  src/interrupts.c:148:     IDT[32].offset_lowerbits = irq0_address & 0xffff;
	movl	$IDT, %eax	/ , tmp160
	movw	%dx, IDT+256	/  irq0_address, IDT[32].offset_lowerbits
/  src/interrupts.c:149:     IDT[32].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+258	/ , IDT[32].selector
/  src/interrupts.c:150:     IDT[32].zero = 0;
	movb	$0, IDT+260	/ , IDT[32].zero
/  src/interrupts.c:151:     IDT[32].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+261	/ , IDT[32].type_attr
/  src/interrupts.c:152:     IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _2
/  src/interrupts.c:152:     IDT[32].offset_higherbits = (irq0_address & 0xffff0000) >> 16;
	movw	%dx, IDT+262	/  _2, IDT[32].offset_higherbits
/  src/interrupts.c:154:     irq1_address = (unsigned long)irq1;
	movl	$irq1, %edx	/ , irq1_address
/  src/interrupts.c:155:     IDT[33].offset_lowerbits = irq1_address & 0xffff;
	movw	%dx, IDT+264	/  irq1_address, IDT[33].offset_lowerbits
/  src/interrupts.c:156:     IDT[33].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+266	/ , IDT[33].selector
/  src/interrupts.c:157:     IDT[33].zero = 0;
	movb	$0, IDT+268	/ , IDT[33].zero
/  src/interrupts.c:158:     IDT[33].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+269	/ , IDT[33].type_attr
/  src/interrupts.c:159:     IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _5
/  src/interrupts.c:159:     IDT[33].offset_higherbits = (irq1_address & 0xffff0000) >> 16;
	movw	%dx, IDT+270	/  _5, IDT[33].offset_higherbits
/  src/interrupts.c:161:     irq2_address = (unsigned long)irq2;
	movl	$irq2, %edx	/ , irq2_address
/  src/interrupts.c:162:     IDT[34].offset_lowerbits = irq2_address & 0xffff;
	movw	%dx, IDT+272	/  irq2_address, IDT[34].offset_lowerbits
/  src/interrupts.c:163:     IDT[34].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+274	/ , IDT[34].selector
/  src/interrupts.c:164:     IDT[34].zero = 0;
	movb	$0, IDT+276	/ , IDT[34].zero
/  src/interrupts.c:165:     IDT[34].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+277	/ , IDT[34].type_attr
/  src/interrupts.c:166:     IDT[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _8
/  src/interrupts.c:166:     IDT[34].offset_higherbits = (irq2_address & 0xffff0000) >> 16;
	movw	%dx, IDT+278	/  _8, IDT[34].offset_higherbits
/  src/interrupts.c:168:     irq3_address = (unsigned long)irq3;
	movl	$irq3, %edx	/ , irq3_address
/  src/interrupts.c:169:     IDT[35].offset_lowerbits = irq3_address & 0xffff;
	movw	%dx, IDT+280	/  irq3_address, IDT[35].offset_lowerbits
/  src/interrupts.c:170:     IDT[35].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+282	/ , IDT[35].selector
/  src/interrupts.c:171:     IDT[35].zero = 0;
	movb	$0, IDT+284	/ , IDT[35].zero
/  src/interrupts.c:172:     IDT[35].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+285	/ , IDT[35].type_attr
/  src/interrupts.c:173:     IDT[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _11
/  src/interrupts.c:173:     IDT[35].offset_higherbits = (irq3_address & 0xffff0000) >> 16;
	movw	%dx, IDT+286	/  _11, IDT[35].offset_higherbits
/  src/interrupts.c:175:     irq4_address = (unsigned long)irq4;
	movl	$irq4, %edx	/ , irq4_address
/  src/interrupts.c:176:     IDT[36].offset_lowerbits = irq4_address & 0xffff;
	movw	%dx, IDT+288	/  irq4_address, IDT[36].offset_lowerbits
/  src/interrupts.c:177:     IDT[36].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+290	/ , IDT[36].selector
/  src/interrupts.c:178:     IDT[36].zero = 0;
	movb	$0, IDT+292	/ , IDT[36].zero
/  src/interrupts.c:179:     IDT[36].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+293	/ , IDT[36].type_attr
/  src/interrupts.c:180:     IDT[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _14
/  src/interrupts.c:180:     IDT[36].offset_higherbits = (irq4_address & 0xffff0000) >> 16;
	movw	%dx, IDT+294	/  _14, IDT[36].offset_higherbits
/  src/interrupts.c:182:     irq5_address = (unsigned long)irq5;
	movl	$irq5, %edx	/ , irq5_address
/  src/interrupts.c:183:     IDT[37].offset_lowerbits = irq5_address & 0xffff;
	movw	%dx, IDT+296	/  irq5_address, IDT[37].offset_lowerbits
/  src/interrupts.c:184:     IDT[37].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+298	/ , IDT[37].selector
/  src/interrupts.c:185:     IDT[37].zero = 0;
	movb	$0, IDT+300	/ , IDT[37].zero
/  src/interrupts.c:186:     IDT[37].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+301	/ , IDT[37].type_attr
/  src/interrupts.c:187:     IDT[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _17
/  src/interrupts.c:187:     IDT[37].offset_higherbits = (irq5_address & 0xffff0000) >> 16;
	movw	%dx, IDT+302	/  _17, IDT[37].offset_higherbits
/  src/interrupts.c:189:     irq6_address = (unsigned long)irq6;
	movl	$irq6, %edx	/ , irq6_address
/  src/interrupts.c:190:     IDT[38].offset_lowerbits = irq6_address & 0xffff;
	movw	%dx, IDT+304	/  irq6_address, IDT[38].offset_lowerbits
/  src/interrupts.c:191:     IDT[38].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+306	/ , IDT[38].selector
/  src/interrupts.c:192:     IDT[38].zero = 0;
	movb	$0, IDT+308	/ , IDT[38].zero
/  src/interrupts.c:193:     IDT[38].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+309	/ , IDT[38].type_attr
/  src/interrupts.c:194:     IDT[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _20
/  src/interrupts.c:194:     IDT[38].offset_higherbits = (irq6_address & 0xffff0000) >> 16;
	movw	%dx, IDT+310	/  _20, IDT[38].offset_higherbits
/  src/interrupts.c:196:     irq7_address = (unsigned long)irq7;
	movl	$irq7, %edx	/ , irq7_address
/  src/interrupts.c:197:     IDT[39].offset_lowerbits = irq7_address & 0xffff;
	movw	%dx, IDT+312	/  irq7_address, IDT[39].offset_lowerbits
/  src/interrupts.c:198:     IDT[39].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+314	/ , IDT[39].selector
/  src/interrupts.c:199:     IDT[39].zero = 0;
	movb	$0, IDT+316	/ , IDT[39].zero
/  src/interrupts.c:200:     IDT[39].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+317	/ , IDT[39].type_attr
/  src/interrupts.c:201:     IDT[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _23
/  src/interrupts.c:201:     IDT[39].offset_higherbits = (irq7_address & 0xffff0000) >> 16;
	movw	%dx, IDT+318	/  _23, IDT[39].offset_higherbits
/  src/interrupts.c:203:     irq8_address = (unsigned long)irq8;
	movl	$irq8, %edx	/ , irq8_address
/  src/interrupts.c:204:     IDT[40].offset_lowerbits = irq8_address & 0xffff;
	movw	%dx, IDT+320	/  irq8_address, IDT[40].offset_lowerbits
/  src/interrupts.c:205:     IDT[40].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+322	/ , IDT[40].selector
/  src/interrupts.c:206:     IDT[40].zero = 0;
	movb	$0, IDT+324	/ , IDT[40].zero
/  src/interrupts.c:207:     IDT[40].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+325	/ , IDT[40].type_attr
/  src/interrupts.c:208:     IDT[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _26
/  src/interrupts.c:208:     IDT[40].offset_higherbits = (irq8_address & 0xffff0000) >> 16;
	movw	%dx, IDT+326	/  _26, IDT[40].offset_higherbits
/  src/interrupts.c:210:     irq9_address = (unsigned long)irq9;
	movl	$irq9, %edx	/ , irq9_address
/  src/interrupts.c:211:     IDT[41].offset_lowerbits = irq9_address & 0xffff;
	movw	%dx, IDT+328	/  irq9_address, IDT[41].offset_lowerbits
/  src/interrupts.c:212:     IDT[41].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+330	/ , IDT[41].selector
/  src/interrupts.c:213:     IDT[41].zero = 0;
	movb	$0, IDT+332	/ , IDT[41].zero
/  src/interrupts.c:214:     IDT[41].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+333	/ , IDT[41].type_attr
/  src/interrupts.c:215:     IDT[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _29
/  src/interrupts.c:215:     IDT[41].offset_higherbits = (irq9_address & 0xffff0000) >> 16;
	movw	%dx, IDT+334	/  _29, IDT[41].offset_higherbits
/  src/interrupts.c:217:     irq10_address = (unsigned long)irq10;
	movl	$irq10, %edx	/ , irq10_address
/  src/interrupts.c:218:     IDT[42].offset_lowerbits = irq10_address & 0xffff;
	movw	%dx, IDT+336	/  irq10_address, IDT[42].offset_lowerbits
/  src/interrupts.c:219:     IDT[42].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+338	/ , IDT[42].selector
/  src/interrupts.c:220:     IDT[42].zero = 0;
	movb	$0, IDT+340	/ , IDT[42].zero
/  src/interrupts.c:221:     IDT[42].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+341	/ , IDT[42].type_attr
/  src/interrupts.c:222:     IDT[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _32
/  src/interrupts.c:222:     IDT[42].offset_higherbits = (irq10_address & 0xffff0000) >> 16;
	movw	%dx, IDT+342	/  _32, IDT[42].offset_higherbits
/  src/interrupts.c:224:     irq11_address = (unsigned long)irq11;
	movl	$irq11, %edx	/ , irq11_address
/  src/interrupts.c:225:     IDT[43].offset_lowerbits = irq11_address & 0xffff;
	movw	%dx, IDT+344	/  irq11_address, IDT[43].offset_lowerbits
/  src/interrupts.c:226:     IDT[43].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+346	/ , IDT[43].selector
/  src/interrupts.c:227:     IDT[43].zero = 0;
	movb	$0, IDT+348	/ , IDT[43].zero
/  src/interrupts.c:228:     IDT[43].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+349	/ , IDT[43].type_attr
/  src/interrupts.c:229:     IDT[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _35
/  src/interrupts.c:229:     IDT[43].offset_higherbits = (irq11_address & 0xffff0000) >> 16;
	movw	%dx, IDT+350	/  _35, IDT[43].offset_higherbits
/  src/interrupts.c:231:     irq12_address = (unsigned long)irq12;
	movl	$irq12, %edx	/ , irq12_address
/  src/interrupts.c:232:     IDT[44].offset_lowerbits = irq12_address & 0xffff;
	movw	%dx, IDT+352	/  irq12_address, IDT[44].offset_lowerbits
/  src/interrupts.c:233:     IDT[44].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+354	/ , IDT[44].selector
/  src/interrupts.c:234:     IDT[44].zero = 0;
	movb	$0, IDT+356	/ , IDT[44].zero
/  src/interrupts.c:235:     IDT[44].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+357	/ , IDT[44].type_attr
/  src/interrupts.c:236:     IDT[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _38
/  src/interrupts.c:236:     IDT[44].offset_higherbits = (irq12_address & 0xffff0000) >> 16;
	movw	%dx, IDT+358	/  _38, IDT[44].offset_higherbits
/  src/interrupts.c:238:     irq13_address = (unsigned long)irq13;
	movl	$irq13, %edx	/ , irq13_address
/  src/interrupts.c:239:     IDT[45].offset_lowerbits = irq13_address & 0xffff;
	movw	%dx, IDT+360	/  irq13_address, IDT[45].offset_lowerbits
/  src/interrupts.c:240:     IDT[45].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+362	/ , IDT[45].selector
/  src/interrupts.c:241:     IDT[45].zero = 0;
	movb	$0, IDT+364	/ , IDT[45].zero
/  src/interrupts.c:242:     IDT[45].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+365	/ , IDT[45].type_attr
/  src/interrupts.c:243:     IDT[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _41
/  src/interrupts.c:243:     IDT[45].offset_higherbits = (irq13_address & 0xffff0000) >> 16;
	movw	%dx, IDT+366	/  _41, IDT[45].offset_higherbits
/  src/interrupts.c:245:     irq14_address = (unsigned long)irq14;
	movl	$irq14, %edx	/ , irq14_address
/  src/interrupts.c:246:     IDT[46].offset_lowerbits = irq14_address & 0xffff;
	movw	%dx, IDT+368	/  irq14_address, IDT[46].offset_lowerbits
/  src/interrupts.c:247:     IDT[46].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+370	/ , IDT[46].selector
/  src/interrupts.c:248:     IDT[46].zero = 0;
	movb	$0, IDT+372	/ , IDT[46].zero
/  src/interrupts.c:249:     IDT[46].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+373	/ , IDT[46].type_attr
/  src/interrupts.c:250:     IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _44
/  src/interrupts.c:250:     IDT[46].offset_higherbits = (irq14_address & 0xffff0000) >> 16;
	movw	%dx, IDT+374	/  _44, IDT[46].offset_higherbits
/  src/interrupts.c:252:     irq15_address = (unsigned long)irq15;
	movl	$irq15, %edx	/ , irq15_address
/  src/interrupts.c:253:     IDT[47].offset_lowerbits = irq15_address & 0xffff;
	movw	%dx, IDT+376	/  irq15_address, IDT[47].offset_lowerbits
/  src/interrupts.c:254:     IDT[47].selector = 0x08; // KERNEL_CODE_SEGMENT_OFFSET  
	movw	$8, IDT+378	/ , IDT[47].selector
/  src/interrupts.c:255:     IDT[47].zero = 0;
	movb	$0, IDT+380	/ , IDT[47].zero
/  src/interrupts.c:256:     IDT[47].type_attr = 0x8e; // INTERRUPT_GATE  
	movb	$-114, IDT+381	/ , IDT[47].type_attr
/  src/interrupts.c:257:     IDT[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;
	shrl	$16, %edx	/ , _47
/  src/interrupts.c:257:     IDT[47].offset_higherbits = (irq15_address & 0xffff0000) >> 16;
	movw	%dx, IDT+382	/  _47, IDT[47].offset_higherbits
/  src/interrupts.c:261:     idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
	movl	%eax, %edx	/  tmp160, _49
	sall	$16, %edx	/ , _49
/  src/interrupts.c:261:     idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
	addl	$2048, %edx	/ , _50
/  src/interrupts.c:261:     idt_ptr[0] = (sizeof (struct IDT_entry) * 256) + ((idt_address & 0xffff) << 16);
	movl	%edx, -16(%ebp)	/  _50, idt_ptr[0]
/  src/interrupts.c:262:     idt_ptr[1] = idt_address >> 16 ;
	shrl	$16, %eax	/ , _51
/  src/interrupts.c:262:     idt_ptr[1] = idt_address >> 16 ;
	movl	%eax, -12(%ebp)	/  _51, idt_ptr[1]
/  src/interrupts.c:266:     load_idt(idt_ptr);
	leal	-16(%ebp), %eax	/ , tmp240
	pushl	%eax	/  tmp240
	call	load_idt	/ 
/  src/interrupts.c:268: }
	addl	$16, %esp	/ ,
	leave	
	ret	
	.size	idt_init, .-idt_init
	.globl	IDT
	.section	.bss
	.align 32
	.type	IDT, @object
	.size	IDT, 2048
IDT:
	.zero	2048
	.ident	"GCC: (GNU) 10.2.0"

	.file	"vga_driver.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/vga_driver.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/vga_driver.o -Og -Wall
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
	.globl	terminal_clear
	.type	terminal_clear, @function
terminal_clear:
/  src/vga_driver.c:40:     terminal_row = 0u;
	movl	$0, terminal_row	/ , terminal_row
/  src/vga_driver.c:41:     terminal_column = 0u;
	movl	$0, terminal_column	/ , terminal_column
/  src/vga_driver.c:43:     for (size_t y = 0u; y < VGA_HEIGHT; y++) {
	movl	$0, %ecx	/ , y
.L7:
/  src/vga_driver.c:43:     for (size_t y = 0u; y < VGA_HEIGHT; y++) {
	cmpl	$24, %ecx	/ , y
	ja	.L12	/ ,
/  src/vga_driver.c:44:         for (size_t x = 0u; x < VGA_WIDTH; x++) {
	movl	$0, %edx	/ , x
/  src/vga_driver.c:44:         for (size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %edx	/ , x
	ja	.L13	/ ,
/  src/vga_driver.c:39: void terminal_clear(void) {
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%ebx	/ 
.L3:
/  src/vga_driver.c:45:             const size_t index = y * VGA_WIDTH + x;
	leal	(%ecx,%ecx,4), %ebx	/ , tmp98
	movl	%ebx, %eax	/  tmp98, tmp98
	sall	$4, %eax	/ , tmp98
/  src/vga_driver.c:45:             const size_t index = y * VGA_WIDTH + x;
	addl	%edx, %eax	/  x, index
/  src/vga_driver.c:46:             terminal_buffer[index] = vga_entry(' ', terminal_color);
	movl	terminal_buffer, %ebx	/  terminal_buffer, terminal_buffer
	leal	(%ebx,%eax,2), %ebx	/ , _5
/  src/vga_driver.c:9:     return (uint16_t) uc | (uint16_t) color << 8u;
	movl	terminal_color, %eax	/  terminal_color, tmp102
	sall	$8, %eax	/ , tmp102
/  src/vga_driver.c:9:     return (uint16_t) uc | (uint16_t) color << 8u;
	orl	$32, %eax	/ , _22
/  src/vga_driver.c:46:             terminal_buffer[index] = vga_entry(' ', terminal_color);
	movw	%ax, (%ebx)	/  _22, *_5
/  src/vga_driver.c:44:         for (size_t x = 0u; x < VGA_WIDTH; x++) {
	addl	$1, %edx	/ , x
.L4:
/  src/vga_driver.c:44:         for (size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %edx	/ , x
	jbe	.L3	/ ,
/  src/vga_driver.c:43:     for (size_t y = 0u; y < VGA_HEIGHT; y++) {
	addl	$1, %ecx	/ , y
/  src/vga_driver.c:43:     for (size_t y = 0u; y < VGA_HEIGHT; y++) {
	cmpl	$24, %ecx	/ , y
	ja	.L14	/ ,
/  src/vga_driver.c:44:         for (size_t x = 0u; x < VGA_WIDTH; x++) {
	movl	$0, %edx	/ , x
	jmp	.L4	/ 
.L14:
/  src/vga_driver.c:49: }
	movl	-4(%ebp), %ebx	/ ,
	leave	
	ret	
.L13:
/  src/vga_driver.c:43:     for (size_t y = 0u; y < VGA_HEIGHT; y++) {
	addl	$1, %ecx	/ , y
	jmp	.L7	/ 
.L12:
	ret	
	.size	terminal_clear, .-terminal_clear
	.globl	terminal_setcolor
	.type	terminal_setcolor, @function
terminal_setcolor:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
/  src/vga_driver.c:52:     terminal_color = color;
	movl	8(%ebp), %eax	/  color, color
	movl	%eax, terminal_color	/  color, terminal_color
/  src/vga_driver.c:53: }
	popl	%ebp	/ 
	ret	
	.size	terminal_setcolor, .-terminal_setcolor
	.globl	terminal_resetcolor
	.type	terminal_resetcolor, @function
terminal_resetcolor:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$20, %esp	/ ,
/  src/vga_driver.c:56:     terminal_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
	pushl	$7	/ 
	call	terminal_setcolor	/ 
/  src/vga_driver.c:57: }
	addl	$16, %esp	/ ,
	leave	
	ret	
	.size	terminal_resetcolor, .-terminal_resetcolor
	.globl	terminal_initialize_test
	.type	terminal_initialize_test, @function
terminal_initialize_test:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$8, %esp	/ ,
/  src/vga_driver.c:32:     terminal_resetcolor();
	call	terminal_resetcolor	/ 
/  src/vga_driver.c:33:     terminal_buffer = terminal_buffer_address;
	movl	8(%ebp), %eax	/  terminal_buffer_address, terminal_buffer_address
	movl	%eax, terminal_buffer	/  terminal_buffer_address, terminal_buffer
/  src/vga_driver.c:34:     terminal_upward_history_size = 0;
	movl	$0, terminal_upward_history_size	/ , terminal_upward_history_size
/  src/vga_driver.c:35:     terminal_downward_history_size = 0;
	movl	$0, terminal_downward_history_size	/ , terminal_downward_history_size
/  src/vga_driver.c:36:     terminal_clear();
	call	terminal_clear	/ 
/  src/vga_driver.c:37: }
	leave	
	ret	
	.size	terminal_initialize_test, .-terminal_initialize_test
	.globl	terminal_initialize
	.type	terminal_initialize, @function
terminal_initialize:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$20, %esp	/ ,
/  src/vga_driver.c:28:     terminal_initialize_test((uint16_t*)0xB8000u);
	pushl	$753664	/ 
	call	terminal_initialize_test	/ 
/  src/vga_driver.c:29: }
	addl	$16, %esp	/ ,
	leave	
	ret	
	.size	terminal_initialize, .-terminal_initialize
	.globl	terminal_putentryat
	.type	terminal_putentryat, @function
terminal_putentryat:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	movl	20(%ebp), %eax	/  y, y
/  src/vga_driver.c:60:     const size_t index = y * VGA_WIDTH + x;
	leal	(%eax,%eax,4), %edx	/ , tmp101
	movl	%edx, %eax	/  tmp101, tmp101
	sall	$4, %eax	/ , tmp101
/  src/vga_driver.c:60:     const size_t index = y * VGA_WIDTH + x;
	addl	16(%ebp), %eax	/  x, index
/  src/vga_driver.c:61:     terminal_buffer[index] = vga_entry(c, color);
	movl	terminal_buffer, %edx	/  terminal_buffer, terminal_buffer
	leal	(%edx,%eax,2), %ecx	/ , _4
/  src/vga_driver.c:9:     return (uint16_t) uc | (uint16_t) color << 8u;
	movsbw	8(%ebp), %ax	/  c, _12
/  src/vga_driver.c:9:     return (uint16_t) uc | (uint16_t) color << 8u;
	movl	12(%ebp), %edx	/  color, tmp105
	sall	$8, %edx	/ , tmp105
/  src/vga_driver.c:9:     return (uint16_t) uc | (uint16_t) color << 8u;
	orl	%edx, %eax	/  tmp105, _18
/  src/vga_driver.c:61:     terminal_buffer[index] = vga_entry(c, color);
	movw	%ax, (%ecx)	/  _18, *_4
/  src/vga_driver.c:62: }
	popl	%ebp	/ 
	ret	
	.size	terminal_putentryat, .-terminal_putentryat
	.globl	terminal_scroll_down
	.type	terminal_scroll_down, @function
terminal_scroll_down:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$12, %esp	/ ,
/  src/vga_driver.c:110:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	movl	$0, %eax	/ , x
/  src/vga_driver.c:110:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	jmp	.L26	/ 
.L27:
/  src/vga_driver.c:111:         terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
	movl	terminal_buffer, %edx	/  terminal_buffer, terminal_buffer
	leal	(%edx,%eax,2), %ecx	/ , _3
/  src/vga_driver.c:111:         terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
	movl	terminal_upward_history_size, %edx	/  terminal_upward_history_size, terminal_upward_history_size.13_4
	leal	1(%edx), %ebx	/ , tmp114
	movl	%ebx, terminal_upward_history_size	/  tmp114, terminal_upward_history_size
/  src/vga_driver.c:111:         terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
	movzwl	(%ecx), %ecx	/  *_3, _6
/  src/vga_driver.c:111:         terminal_upward_history[terminal_upward_history_size++] = terminal_buffer[x];
	movw	%cx, terminal_upward_history(%edx,%edx)	/  _6, terminal_upward_history[terminal_upward_history_size.13_4]
/  src/vga_driver.c:110:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	addl	$1, %eax	/ , x
.L26:
/  src/vga_driver.c:110:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %eax	/ , x
	jbe	.L27	/ ,
/  src/vga_driver.c:113: 	for(size_t y = 0u; y < VGA_HEIGHT - 1; y++) {
	movl	$0, %ecx	/ , y
	jmp	.L28	/ 
.L29:
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	movl	terminal_buffer, %ebx	/  terminal_buffer, terminal_buffer.15_7
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	leal	5(%ecx,%ecx,4), %esi	/ , tmp119
	sall	$4, %esi	/ , tmp119
	movl	%esi, %edx	/  tmp119, tmp120
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	addl	%eax, %edx	/  x, _10
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	leal	(%ebx,%edx,2), %esi	/ , _12
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	leal	(%ecx,%ecx,4), %edi	/ , tmp124
	sall	$4, %edi	/ , tmp124
	movl	%edi, %edx	/  tmp124, tmp125
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	addl	%eax, %edx	/  x, _14
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	leal	(%ebx,%edx,2), %edx	/ , _16
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	movzwl	(%esi), %ebx	/  *_12, _17
/  src/vga_driver.c:115: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y + 1) * VGA_WIDTH + x];
	movw	%bx, (%edx)	/  _17, *_16
/  src/vga_driver.c:114: 		for(size_t x = 0u; x < VGA_WIDTH; x++) {
	addl	$1, %eax	/ , x
.L30:
/  src/vga_driver.c:114: 		for(size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %eax	/ , x
	jbe	.L29	/ ,
/  src/vga_driver.c:113: 	for(size_t y = 0u; y < VGA_HEIGHT - 1; y++) {
	addl	$1, %ecx	/ , y
.L28:
/  src/vga_driver.c:113: 	for(size_t y = 0u; y < VGA_HEIGHT - 1; y++) {
	cmpl	$23, %ecx	/ , y
	ja	.L40	/ ,
/  src/vga_driver.c:114: 		for(size_t x = 0u; x < VGA_WIDTH; x++) {
	movl	$0, %eax	/ , x
	jmp	.L30	/ 
.L40:
/  src/vga_driver.c:118:     if (terminal_downward_history_size == 0u) {
	movl	terminal_downward_history_size, %ebx	/  terminal_downward_history_size, x
/  src/vga_driver.c:118:     if (terminal_downward_history_size == 0u) {
	testl	%ebx, %ebx	/  x
	je	.L31	/ ,
/  src/vga_driver.c:123:         for(size_t x = terminal_downward_history_size - 1; x > 0u; x--) {
	subl	$1, %ebx	/ , x
/  src/vga_driver.c:123:         for(size_t x = terminal_downward_history_size - 1; x > 0u; x--) {
	jmp	.L35	/ 
.L33:
/  src/vga_driver.c:120: 	    	terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
	pushl	$24	/ 
	pushl	%ebx	/  x
	pushl	terminal_color	/  terminal_color
	pushl	$32	/ 
	call	terminal_putentryat	/ 
/  src/vga_driver.c:119: 	    for(size_t x = 0u; x < VGA_WIDTH; x++) {
	addl	$1, %ebx	/ , x
	addl	$16, %esp	/ ,
.L31:
/  src/vga_driver.c:119: 	    for(size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %ebx	/ , x
	jbe	.L33	/ ,
.L34:
/  src/vga_driver.c:127: 	terminal_row = VGA_HEIGHT - 1;
	movl	$24, terminal_row	/ , terminal_row
/  src/vga_driver.c:128: 	terminal_column = 0u;
	movl	$0, terminal_column	/ , terminal_column
/  src/vga_driver.c:129: }
	leal	-12(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
.L36:
/  src/vga_driver.c:124:             terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
	movl	terminal_downward_history_size, %eax	/  terminal_downward_history_size, tmp133
	subl	$1, %eax	/ , _21
/  src/vga_driver.c:124:             terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
	movl	%eax, terminal_downward_history_size	/  _21, terminal_downward_history_size
/  src/vga_driver.c:124:             terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
	movl	terminal_buffer, %edx	/  terminal_buffer, terminal_buffer
	leal	3840(%edx,%ebx,2), %edx	/ , _25
/  src/vga_driver.c:124:             terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
	movzwl	terminal_downward_history(%eax,%eax), %eax	/  terminal_downward_history[_21], _26
/  src/vga_driver.c:124:             terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x] = terminal_downward_history[--terminal_downward_history_size];
	movw	%ax, (%edx)	/  _26, *_25
/  src/vga_driver.c:123:         for(size_t x = terminal_downward_history_size - 1; x > 0u; x--) {
	subl	$1, %ebx	/ , x
.L35:
/  src/vga_driver.c:123:         for(size_t x = terminal_downward_history_size - 1; x > 0u; x--) {
	testl	%ebx, %ebx	/  x
	jne	.L36	/ ,
	jmp	.L34	/ 
	.size	terminal_scroll_down, .-terminal_scroll_down
	.globl	terminal_putchar
	.type	terminal_putchar, @function
terminal_putchar:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$8, %esp	/ ,
	movl	8(%ebp), %eax	/  c, c
/  src/vga_driver.c:65: 	if (c == '\n') {
	cmpb	$10, %al	/ , c
	je	.L46	/ ,
/  src/vga_driver.c:72:     terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	pushl	terminal_row	/  terminal_row
	pushl	terminal_column	/  terminal_column
	pushl	terminal_color	/  terminal_color
	movsbl	%al, %eax	/  c, c
	pushl	%eax	/  c
	call	terminal_putentryat	/ 
/  src/vga_driver.c:73:     if (++terminal_column == VGA_WIDTH) {
	movl	terminal_column, %eax	/  terminal_column, tmp99
	addl	$1, %eax	/ , _8
/  src/vga_driver.c:73:     if (++terminal_column == VGA_WIDTH) {
	movl	%eax, terminal_column	/  _8, terminal_column
	addl	$16, %esp	/ ,
	cmpl	$80, %eax	/ , _8
	je	.L47	/ ,
.L41:
/  src/vga_driver.c:79: }
	leave	
	ret	
.L46:
/  src/vga_driver.c:66: 		if(++terminal_row == VGA_HEIGHT) {
	movl	terminal_row, %eax	/  terminal_row, tmp98
	addl	$1, %eax	/ , _2
/  src/vga_driver.c:66: 		if(++terminal_row == VGA_HEIGHT) {
	movl	%eax, terminal_row	/  _2, terminal_row
	cmpl	$25, %eax	/ , _2
	je	.L48	/ ,
.L43:
/  src/vga_driver.c:69: 		terminal_column = 0;
	movl	$0, terminal_column	/ , terminal_column
/  src/vga_driver.c:70: 		return;
	jmp	.L41	/ 
.L48:
/  src/vga_driver.c:67: 			terminal_scroll_down();
	call	terminal_scroll_down	/ 
	jmp	.L43	/ 
.L47:
/  src/vga_driver.c:74:         terminal_column = 0;
	movl	$0, terminal_column	/ , terminal_column
/  src/vga_driver.c:75:         if (++terminal_row == VGA_HEIGHT) {
	movl	terminal_row, %eax	/  terminal_row, tmp100
	addl	$1, %eax	/ , _10
/  src/vga_driver.c:75:         if (++terminal_row == VGA_HEIGHT) {
	movl	%eax, terminal_row	/  _10, terminal_row
	cmpl	$25, %eax	/ , _10
	jne	.L41	/ ,
/  src/vga_driver.c:76: 			terminal_scroll_down();
	call	terminal_scroll_down	/ 
	jmp	.L41	/ 
	.size	terminal_putchar, .-terminal_putchar
	.globl	terminal_putchar_color
	.type	terminal_putchar_color, @function
terminal_putchar_color:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	8(%ebp), %ebx	/  c, c
/  src/vga_driver.c:82:     const enum vga_color old_color = terminal_color;
	movl	terminal_color, %esi	/  terminal_color, old_color
/  src/vga_driver.c:83:     terminal_setcolor(color);
	subl	$12, %esp	/ ,
	pushl	12(%ebp)	/  color
	call	terminal_setcolor	/ 
/  src/vga_driver.c:84:     terminal_putchar(c);
	movsbl	%bl, %ebx	/  c, c
	movl	%ebx, (%esp)	/  c,
	call	terminal_putchar	/ 
/  src/vga_driver.c:85:     terminal_setcolor(old_color);
	movl	%esi, (%esp)	/  old_color,
	call	terminal_setcolor	/ 
/  src/vga_driver.c:86: }
	addl	$16, %esp	/ ,
	leal	-8(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
	.size	terminal_putchar_color, .-terminal_putchar_color
	.globl	terminal_write
	.type	terminal_write, @function
terminal_write:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$12, %esp	/ ,
	movl	8(%ebp), %edi	/  text, text
	movl	12(%ebp), %esi	/  size, size
/  src/vga_driver.c:89:     for (size_t i = 0; i < size; i++) {
	movl	$0, %ebx	/ , i
/  src/vga_driver.c:89:     for (size_t i = 0; i < size; i++) {
	jmp	.L52	/ 
.L53:
/  src/vga_driver.c:90:         terminal_putchar(text[i]);
	subl	$12, %esp	/ ,
	movsbl	(%edi,%ebx), %eax	/  *_1, _2
	pushl	%eax	/  _2
	call	terminal_putchar	/ 
/  src/vga_driver.c:89:     for (size_t i = 0; i < size; i++) {
	addl	$1, %ebx	/ , i
	addl	$16, %esp	/ ,
.L52:
/  src/vga_driver.c:89:     for (size_t i = 0; i < size; i++) {
	cmpl	%esi, %ebx	/  size, i
	jb	.L53	/ ,
/  src/vga_driver.c:92: }
	leal	-12(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	terminal_write, .-terminal_write
	.globl	terminal_writestring
	.type	terminal_writestring, @function
terminal_writestring:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%ebx	/ 
	subl	$16, %esp	/ ,
	movl	8(%ebp), %ebx	/  text, text
/  src/vga_driver.c:95:     terminal_write(text, kstrlen(text));
	pushl	%ebx	/  text
	call	kstrlen	/ 
	addl	$8, %esp	/ ,
	pushl	%eax	/  tmp84
	pushl	%ebx	/  text
	call	terminal_write	/ 
/  src/vga_driver.c:96: }
	addl	$16, %esp	/ ,
	movl	-4(%ebp), %ebx	/ ,
	leave	
	ret	
	.size	terminal_writestring, .-terminal_writestring
	.globl	terminal_write_color
	.type	terminal_write_color, @function
terminal_write_color:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%ebx	/ 
	subl	$16, %esp	/ ,
/  src/vga_driver.c:99:     const enum vga_color old_color = terminal_color;
	movl	terminal_color, %ebx	/  terminal_color, old_color
/  src/vga_driver.c:100:     terminal_setcolor(color);
	pushl	16(%ebp)	/  color
	call	terminal_setcolor	/ 
/  src/vga_driver.c:101:     terminal_write(text, size);
	addl	$8, %esp	/ ,
	pushl	12(%ebp)	/  size
	pushl	8(%ebp)	/  text
	call	terminal_write	/ 
/  src/vga_driver.c:102:     terminal_setcolor(old_color);
	movl	%ebx, (%esp)	/  old_color,
	call	terminal_setcolor	/ 
/  src/vga_driver.c:103: }
	addl	$16, %esp	/ ,
	movl	-4(%ebp), %ebx	/ ,
	leave	
	ret	
	.size	terminal_write_color, .-terminal_write_color
	.globl	terminal_writestring_color
	.type	terminal_writestring_color, @function
terminal_writestring_color:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%ebx	/ 
	subl	$16, %esp	/ ,
	movl	8(%ebp), %ebx	/  text, text
/  src/vga_driver.c:106:     terminal_write_color(text, kstrlen(text), color);
	pushl	%ebx	/  text
	call	kstrlen	/ 
	addl	$12, %esp	/ ,
	pushl	12(%ebp)	/  color
	pushl	%eax	/  tmp85
	pushl	%ebx	/  text
	call	terminal_write_color	/ 
/  src/vga_driver.c:107: }
	addl	$16, %esp	/ ,
	movl	-4(%ebp), %ebx	/ ,
	leave	
	ret	
	.size	terminal_writestring_color, .-terminal_writestring_color
	.globl	terminal_scroll_up
	.type	terminal_scroll_up, @function
terminal_scroll_up:
/  src/vga_driver.c:132:     if(terminal_upward_history_size == 0u) return;
	cmpl	$0, terminal_upward_history_size	/ , terminal_upward_history_size
	je	.L72	/ ,
/  src/vga_driver.c:131: void terminal_scroll_up(void) {
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
/  src/vga_driver.c:133:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	movl	$0, %eax	/ , x
	jmp	.L63	/ 
.L64:
/  src/vga_driver.c:134:         terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
	movl	terminal_buffer, %edx	/  terminal_buffer, terminal_buffer
	leal	3840(%edx,%eax,2), %ecx	/ , _5
/  src/vga_driver.c:134:         terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
	movl	terminal_downward_history_size, %edx	/  terminal_downward_history_size, terminal_downward_history_size.25_6
	leal	1(%edx), %ebx	/ , tmp112
	movl	%ebx, terminal_downward_history_size	/  tmp112, terminal_downward_history_size
/  src/vga_driver.c:134:         terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
	movzwl	(%ecx), %ecx	/  *_5, _8
/  src/vga_driver.c:134:         terminal_downward_history[terminal_downward_history_size++] = terminal_buffer[(VGA_HEIGHT - 1) * VGA_WIDTH + x];
	movw	%cx, terminal_downward_history(%edx,%edx)	/  _8, terminal_downward_history[terminal_downward_history_size.25_6]
/  src/vga_driver.c:133:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	addl	$1, %eax	/ , x
.L63:
/  src/vga_driver.c:133:     for(size_t x = 0u; x < VGA_WIDTH; x++) {
	cmpl	$79, %eax	/ , x
	jbe	.L64	/ ,
/  src/vga_driver.c:136: 	for(size_t y = VGA_HEIGHT - 1; y > 0u; y--) {
	movl	$24, %esi	/ , y
	jmp	.L65	/ 
.L66:
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	movl	terminal_buffer, %ecx	/  terminal_buffer, terminal_buffer.27_9
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	leal	(%esi,%esi,4), %ebx	/ , tmp116
	movl	%ebx, %eax	/  tmp116, tmp116
	sall	$4, %eax	/ , tmp116
	addl	%edx, %eax	/  x, _11
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	leal	-160(%ecx,%eax,2), %ebx	/ , _14
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	leal	(%ecx,%eax,2), %eax	/ , _16
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	movzwl	(%ebx), %ecx	/  *_14, _17
/  src/vga_driver.c:138: 			terminal_buffer[y * VGA_WIDTH + x] = terminal_buffer[(y - 1) * VGA_WIDTH + x];
	movw	%cx, (%eax)	/  _17, *_16
/  src/vga_driver.c:137: 		for(size_t x = 0; x < VGA_WIDTH; x++) {
	addl	$1, %edx	/ , x
.L67:
/  src/vga_driver.c:137: 		for(size_t x = 0; x < VGA_WIDTH; x++) {
	cmpl	$79, %edx	/ , x
	jbe	.L66	/ ,
/  src/vga_driver.c:136: 	for(size_t y = VGA_HEIGHT - 1; y > 0u; y--) {
	subl	$1, %esi	/ , y
.L65:
/  src/vga_driver.c:136: 	for(size_t y = VGA_HEIGHT - 1; y > 0u; y--) {
	testl	%esi, %esi	/  y
	je	.L75	/ ,
/  src/vga_driver.c:137: 		for(size_t x = 0; x < VGA_WIDTH; x++) {
	movl	$0, %edx	/ , x
	jmp	.L67	/ 
.L75:
/  src/vga_driver.c:141:     for(size_t x = 1u; x < VGA_WIDTH + 1; x++) {
	movl	$1, %edx	/ , x
	jmp	.L68	/ 
.L69:
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movl	terminal_downward_history_size, %eax	/  terminal_downward_history_size, tmp126
	subl	$1, %eax	/ , _19
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movl	%eax, terminal_downward_history_size	/  _19, terminal_downward_history_size
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movl	$80, %ecx	/ , tmp121
	subl	%edx, %ecx	/  x, _21
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movl	terminal_buffer, %ebx	/  terminal_buffer, terminal_buffer
	leal	(%ebx,%ecx,2), %ecx	/ , _23
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movzwl	terminal_upward_history(%eax,%eax), %eax	/  terminal_upward_history[_19], _24
/  src/vga_driver.c:142:         terminal_buffer[VGA_WIDTH - x] = terminal_upward_history[--terminal_downward_history_size];
	movw	%ax, (%ecx)	/  _24, *_23
/  src/vga_driver.c:141:     for(size_t x = 1u; x < VGA_WIDTH + 1; x++) {
	addl	$1, %edx	/ , x
.L68:
/  src/vga_driver.c:141:     for(size_t x = 1u; x < VGA_WIDTH + 1; x++) {
	cmpl	$80, %edx	/ , x
	jbe	.L69	/ ,
/  src/vga_driver.c:144: 	terminal_row = VGA_HEIGHT - 1;
	movl	$24, terminal_row	/ , terminal_row
/  src/vga_driver.c:145: 	terminal_column = 0u;
	movl	$0, terminal_column	/ , terminal_column
/  src/vga_driver.c:146: }
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
.L72:
	ret	
	.size	terminal_scroll_up, .-terminal_scroll_up
	.local	terminal_downward_history_size
	.comm	terminal_downward_history_size,4,4
	.local	terminal_upward_history_size
	.comm	terminal_upward_history_size,4,4
	.local	terminal_downward_history
	.comm	terminal_downward_history,4000,32
	.local	terminal_upward_history
	.comm	terminal_upward_history,4000,32
	.local	terminal_buffer
	.comm	terminal_buffer,4,4
	.local	terminal_color
	.comm	terminal_color,4,4
	.local	terminal_column
	.comm	terminal_column,4,4
	.local	terminal_row
	.comm	terminal_row,4,4
	.ident	"GCC: (GNU) 10.2.0"

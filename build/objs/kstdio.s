	.file	"kstdio.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/kstdio.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/kstdio.o -Og -Wall -Wextra
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
	.type	has_color, @function
has_color:
/  src/kstdio.c:6:     if(format_size < 2u) return -1;
	cmpl	$1, %edx	/ , tmp93
	jbe	.L20	/ ,
/  src/kstdio.c:8:     if(format[0u] == '%') {
	cmpb	$37, (%eax)	/ , *format_6(D)
	jne	.L21	/ ,
/  src/kstdio.c:9:         switch(format[1u]) {
	movzbl	1(%eax), %eax	/  MEM[(const char *)format_6(D) + 1B], _2
	subl	$48, %eax	/ , tmp87
	cmpb	$22, %al	/ , tmp87
	ja	.L3	/ ,
	movzbl	%al, %eax	/  tmp87, tmp88
	jmp	*.L5(,%eax,4)	/ 
	.section	.rodata
	.align 4
	.align 4
.L5:
	.long	.L22
	.long	.L19
	.long	.L18
	.long	.L17
	.long	.L16
	.long	.L15
	.long	.L14
	.long	.L13
	.long	.L12
	.long	.L11
	.long	.L3
	.long	.L3
	.long	.L3
	.long	.L3
	.long	.L3
	.long	.L3
	.long	.L3
	.long	.L10
	.long	.L9
	.long	.L8
	.long	.L7
	.long	.L6
	.long	.L4
	.text
.L3:
	movl	$-1, %eax	/ , <retval>
	ret	
.L19:
/  src/kstdio.c:13:                 return VGA_COLOR_BLUE;
	movl	$1, %eax	/ , <retval>
	ret	
.L18:
/  src/kstdio.c:15:                 return VGA_COLOR_GREEN;
	movl	$2, %eax	/ , <retval>
	ret	
.L17:
/  src/kstdio.c:17:                 return VGA_COLOR_CYAN;
	movl	$3, %eax	/ , <retval>
	ret	
.L16:
/  src/kstdio.c:19:                 return VGA_COLOR_RED;
	movl	$4, %eax	/ , <retval>
	ret	
.L15:
/  src/kstdio.c:21:                 return VGA_COLOR_MAGENTA;
	movl	$5, %eax	/ , <retval>
	ret	
.L14:
/  src/kstdio.c:23:                 return VGA_COLOR_BROWN;
	movl	$6, %eax	/ , <retval>
	ret	
.L13:
/  src/kstdio.c:25:                 return VGA_COLOR_LIGHT_GREY;
	movl	$7, %eax	/ , <retval>
	ret	
.L12:
/  src/kstdio.c:27:                 return VGA_COLOR_DARK_GREY;
	movl	$8, %eax	/ , <retval>
	ret	
.L11:
/  src/kstdio.c:29:                 return VGA_COLOR_LIGHT_BLUE;
	movl	$9, %eax	/ , <retval>
	ret	
.L10:
/  src/kstdio.c:31:                 return VGA_COLOR_LIGHT_GREEN;
	movl	$10, %eax	/ , <retval>
	ret	
.L9:
/  src/kstdio.c:33:                 return VGA_COLOR_LIGHT_CYAN;
	movl	$11, %eax	/ , <retval>
	ret	
.L8:
/  src/kstdio.c:35:                 return VGA_COLOR_LIGHT_RED;
	movl	$12, %eax	/ , <retval>
	ret	
.L7:
/  src/kstdio.c:37:                 return VGA_COLOR_LIGHT_MAGENTA;
	movl	$13, %eax	/ , <retval>
	ret	
.L6:
/  src/kstdio.c:39:                 return VGA_COLOR_LIGHT_BROWN;
	movl	$14, %eax	/ , <retval>
	ret	
.L4:
/  src/kstdio.c:41:                 return VGA_COLOR_WHITE;
	movl	$15, %eax	/ , <retval>
	ret	
.L20:
/  src/kstdio.c:6:     if(format_size < 2u) return -1;
	movl	$-1, %eax	/ , <retval>
	ret	
.L21:
/  src/kstdio.c:45:     return -1;
	movl	$-1, %eax	/ , <retval>
	ret	
.L22:
/  src/kstdio.c:11:                 return VGA_COLOR_BLACK;
	movl	$0, %eax	/ , <retval>
/  src/kstdio.c:46: }
	ret	
	.size	has_color, .-has_color
	.globl	kprintf
	.type	kprintf, @function
kprintf:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$28, %esp	/ ,
	movl	8(%ebp), %esi	/  format, format
/  src/kstdio.c:134:     va_start(pargs, format);
	leal	12(%ebp), %eax	/ , tmp89
	movl	%eax, -12(%ebp)	/  tmp89, pargs
/  src/kstdio.c:136:     const size_t format_len = kstrlen(format);
	pushl	%esi	/  format
	call	kstrlen	/ 
	movl	%eax, %ebx	/  tmp93, format_len
/  src/kstdio.c:137:     for(uint32_t i = 0u; i < format_len; ++i) {
	movl	$0, -16(%ebp)	/ , i
/  src/kstdio.c:137:     for(uint32_t i = 0u; i < format_len; ++i) {
	addl	$16, %esp	/ ,
	jmp	.L24	/ 
.L25:
/  src/kstdio.c:138:         conversion_specifier(format, format_len, &i, &pargs);
	subl	$12, %esp	/ ,
	leal	-12(%ebp), %eax	/ , tmp91
	pushl	%eax	/  tmp91
	leal	-16(%ebp), %ecx	/ , tmp94
	movl	%ebx, %edx	/  format_len,
	movl	%esi, %eax	/  format,
	call	conversion_specifier	/ 
/  src/kstdio.c:137:     for(uint32_t i = 0u; i < format_len; ++i) {
	addl	$1, -16(%ebp)	/ , i
	addl	$16, %esp	/ ,
.L24:
/  src/kstdio.c:137:     for(uint32_t i = 0u; i < format_len; ++i) {
	cmpl	%ebx, -16(%ebp)	/  format_len, i
	jb	.L25	/ ,
/  src/kstdio.c:143:     color_setting = -1;
	movl	$-1, color_setting	/ , color_setting
/  src/kstdio.c:146: }
	movl	$0, %eax	/ ,
	leal	-8(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
	.size	kprintf, .-kprintf
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC0:
	.string	"src/kstdio.c"
.LC1:
	.string	"%4[file: %s :: line: %i]: "
.LC2:
	.string	"Condition Failed: "
.LC3:
	.string	"(*index) < format_size"
	.text
	.type	conversion_specifier, @function
conversion_specifier:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$140, %esp	/ ,
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	cmpl	%edx, (%ecx)	/  format_size, *index_46(D)
	jnb	.L56	/ ,
	movl	%eax, %edi	/  tmp151, format
	movl	%edx, %esi	/  tmp152, format_size
	movl	%ecx, %ebx	/  tmp153, index
/  src/kstdio.c:53:     if(color_setting == -1) {
	cmpl	$-1, color_setting	/ , color_setting
	je	.L57	/ ,
.L30:
/  src/kstdio.c:60:     if(format[*index] == '%') {
	movl	(%ebx), %edx	/  *index_46(D), _6
/  src/kstdio.c:60:     if(format[*index] == '%') {
	movzbl	(%edi,%edx), %eax	/  *_7, _8
/  src/kstdio.c:60:     if(format[*index] == '%') {
	cmpb	$37, %al	/ , _8
	je	.L58	/ ,
/  src/kstdio.c:122:         if(color_setting == -1) {
	movl	color_setting, %edx	/  color_setting, color_setting.13_33
/  src/kstdio.c:122:         if(color_setting == -1) {
	cmpl	$-1, %edx	/ , color_setting.13_33
	je	.L59	/ ,
/  src/kstdio.c:125:             terminal_putchar_color(format[*index], color_setting);
	subl	$8, %esp	/ ,
	pushl	%edx	/  color_setting.13_33
	movsbl	%al, %eax	/  _8, _8
	pushl	%eax	/  _8
	call	terminal_putchar_color	/ 
	addl	$16, %esp	/ ,
/  src/kstdio.c:128:     return 0;
	movl	$0, %eax	/ , <retval>
.L27:
/  src/kstdio.c:129: }
	leal	-12(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
.L56:
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	subl	$4, %esp	/ ,
	pushl	$51	/ 
	pushl	$.LC0	/ 
	pushl	$.LC1	/ 
	call	kprintf	/ 
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC2	/ 
	call	terminal_writestring_color	/ 
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC3	/ 
	call	terminal_writestring_color	/ 
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$10	/ 
	call	terminal_putchar_color	/ 
/  src/kstdio.c:51:     kassert((*index) < format_size, -1);
	addl	$16, %esp	/ ,
	movl	$-1, %eax	/ , <retval>
	jmp	.L27	/ 
.L57:
/  src/kstdio.c:54:         color_setting = has_color(format, format_size);
	call	has_color	/ 
/  src/kstdio.c:54:         color_setting = has_color(format, format_size);
	movl	%eax, color_setting	/  _3, color_setting
/  src/kstdio.c:55:         if(color_setting != -1) {
	cmpl	$-1, %eax	/ , _3
	je	.L30	/ ,
/  src/kstdio.c:56:             *index += 2u;
	addl	$2, (%ebx)	/ , *index_46(D)
	jmp	.L30	/ 
.L58:
/  src/kstdio.c:61:         if(((*index)+1) < format_size) {
	leal	1(%edx), %eax	/ , _9
/  src/kstdio.c:61:         if(((*index)+1) < format_size) {
	cmpl	%esi, %eax	/  format_size, _9
	jnb	.L53	/ ,
/  src/kstdio.c:67:             switch (format[(*index) + 1]) {
	movzbl	1(%edi,%edx), %eax	/  *_10, _11
	subl	$65, %eax	/ , tmp134
	cmpb	$55, %al	/ , tmp134
	ja	.L32	/ ,
	movzbl	%al, %eax	/  tmp134, tmp135
	jmp	*.L34(,%eax,4)	/ 
	.section	.rodata
	.align 4
	.align 4
.L34:
	.long	.L45
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L43
	.long	.L42
	.long	.L41
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L54
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L45
	.long	.L32
	.long	.L44
	.long	.L40
	.long	.L43
	.long	.L42
	.long	.L41
	.long	.L32
	.long	.L40
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L32
	.long	.L39
	.long	.L38
	.long	.L37
	.long	.L32
	.long	.L32
	.long	.L36
	.long	.L32
	.long	.L35
	.long	.L32
	.long	.L32
	.long	.L54
	.text
.L44:
/  src/kstdio.c:70:                     if(color_setting == -1) {
	movl	color_setting, %eax	/  color_setting, color_setting.4_12
/  src/kstdio.c:70:                     if(color_setting == -1) {
	cmpl	$-1, %eax	/ , color_setting.4_12
	je	.L60	/ ,
/  src/kstdio.c:73:                         terminal_putchar_color(va_arg(*variadic_args, int), color_setting);
	movl	8(%ebp), %edi	/  variadic_args, tmp158
	movl	(%edi), %edx	/  *variadic_args_53(D), D.1168
	leal	4(%edx), %ecx	/ , tmp140
	movl	%ecx, (%edi)	/  tmp140, *variadic_args_53(D)
	movl	(%edx), %edx	/  MEM[(int *)_85], _17
/  src/kstdio.c:73:                         terminal_putchar_color(va_arg(*variadic_args, int), color_setting);
	subl	$8, %esp	/ ,
	pushl	%eax	/  color_setting.4_12
	movsbl	%dl, %eax	/  _17, _17
	pushl	%eax	/  _17
	call	terminal_putchar_color	/ 
	addl	$16, %esp	/ ,
.L47:
/  src/kstdio.c:75:                     *index += 1u;
	addl	$1, (%ebx)	/ , *index_46(D)
/  src/kstdio.c:76:                     return 1;
	movl	$1, %eax	/ , <retval>
	jmp	.L27	/ 
.L60:
/  src/kstdio.c:71:                         terminal_putchar(va_arg(*variadic_args, int));
	movl	8(%ebp), %eax	/  variadic_args, tmp156
	movl	(%eax), %eax	/  *variadic_args_53(D), D.1166
	leal	4(%eax), %edx	/ , tmp138
	movl	8(%ebp), %edi	/  variadic_args, tmp157
	movl	%edx, (%edi)	/  tmp138, *variadic_args_53(D)
/  src/kstdio.c:71:                         terminal_putchar(va_arg(*variadic_args, int));
	subl	$12, %esp	/ ,
	movsbl	(%eax), %eax	/  MEM[(int *)_88], _13
	pushl	%eax	/  _13
	call	terminal_putchar	/ 
	addl	$16, %esp	/ ,
	jmp	.L47	/ 
.L36:
/  src/kstdio.c:78:                     if(color_setting == -1) {
	movl	color_setting, %eax	/  color_setting, color_setting.7_22
/  src/kstdio.c:78:                     if(color_setting == -1) {
	cmpl	$-1, %eax	/ , color_setting.7_22
	je	.L61	/ ,
/  src/kstdio.c:81:                         terminal_writestring_color(va_arg(*variadic_args, char*), color_setting);
	movl	8(%ebp), %edi	/  variadic_args, tmp162
	movl	(%edi), %edx	/  *variadic_args_53(D), D.1172
	leal	4(%edx), %ecx	/ , tmp143
	movl	%ecx, (%edi)	/  tmp143, *variadic_args_53(D)
	movl	(%edx), %edx	/  MEM[(char * *)_76], _25
/  src/kstdio.c:81:                         terminal_writestring_color(va_arg(*variadic_args, char*), color_setting);
	subl	$8, %esp	/ ,
	pushl	%eax	/  color_setting.7_22
	pushl	%edx	/  _25
	call	terminal_writestring_color	/ 
	addl	$16, %esp	/ ,
.L49:
/  src/kstdio.c:83:                     *index += 1u;
	addl	$1, (%ebx)	/ , *index_46(D)
/  src/kstdio.c:84:                     return 2;
	movl	$2, %eax	/ , <retval>
	jmp	.L27	/ 
.L61:
/  src/kstdio.c:79:                         terminal_writestring(va_arg(*variadic_args, char*));
	movl	8(%ebp), %eax	/  variadic_args, tmp160
	movl	(%eax), %eax	/  *variadic_args_53(D), D.1170
	leal	4(%eax), %edx	/ , tmp142
	movl	8(%ebp), %edi	/  variadic_args, tmp161
	movl	%edx, (%edi)	/  tmp142, *variadic_args_53(D)
/  src/kstdio.c:79:                         terminal_writestring(va_arg(*variadic_args, char*));
	subl	$12, %esp	/ ,
	pushl	(%eax)	/  MEM[(char * *)_79]
	call	terminal_writestring	/ 
	addl	$16, %esp	/ ,
	jmp	.L49	/ 
.L40:
/  src/kstdio.c:87:                     number = va_arg(*variadic_args, int);
	movl	8(%ebp), %eax	/  variadic_args, tmp164
	movl	(%eax), %eax	/  *variadic_args_53(D), D.1174
	leal	4(%eax), %edx	/ , tmp144
	movl	8(%ebp), %edi	/  variadic_args, tmp165
	movl	%edx, (%edi)	/  tmp144, *variadic_args_53(D)
	movl	(%eax), %eax	/  MEM[(int *)_82], _28
/  src/kstdio.c:87:                     number = va_arg(*variadic_args, int);
	cltd
/  src/kstdio.c:88:                     kint_to_string(number, str, 128);
	pushl	$128	/ 
	leal	-152(%ebp), %ecx	/ , tmp145
	pushl	%ecx	/  tmp145
	pushl	%edx	/  number
	pushl	%eax	/  number
	call	kint_to_string	/ 
/  src/kstdio.c:89:                     if(color_setting == -1) {
	movl	color_setting, %eax	/  color_setting, color_setting.10_29
/  src/kstdio.c:89:                     if(color_setting == -1) {
	addl	$16, %esp	/ ,
	cmpl	$-1, %eax	/ , color_setting.10_29
	je	.L62	/ ,
/  src/kstdio.c:92:                         terminal_writestring_color(str, color_setting);
	subl	$8, %esp	/ ,
	pushl	%eax	/  color_setting.10_29
	leal	-152(%ebp), %eax	/ , tmp147
	pushl	%eax	/  tmp147
	call	terminal_writestring_color	/ 
	addl	$16, %esp	/ ,
.L51:
/  src/kstdio.c:94:                     *index += 1u;
	addl	$1, (%ebx)	/ , *index_46(D)
/  src/kstdio.c:95:                     return 3;
	movl	$3, %eax	/ , <retval>
	jmp	.L27	/ 
.L62:
/  src/kstdio.c:90:                         terminal_writestring(str);
	subl	$12, %esp	/ ,
	leal	-152(%ebp), %eax	/ , tmp146
	pushl	%eax	/  tmp146
	call	terminal_writestring	/ 
	addl	$16, %esp	/ ,
	jmp	.L51	/ 
.L35:
/  src/kstdio.c:102:                     return 6;
	movl	$6, %eax	/ , <retval>
	jmp	.L27	/ 
.L42:
/  src/kstdio.c:105:                     return 7;
	movl	$7, %eax	/ , <retval>
	jmp	.L27	/ 
.L43:
/  src/kstdio.c:108:                     return 8;
	movl	$8, %eax	/ , <retval>
	jmp	.L27	/ 
.L45:
/  src/kstdio.c:111:                     return 9;
	movl	$9, %eax	/ , <retval>
	jmp	.L27	/ 
.L41:
/  src/kstdio.c:114:                     return 10;
	movl	$10, %eax	/ , <retval>
	jmp	.L27	/ 
.L39:
/  src/kstdio.c:116:                     return 11;
	movl	$11, %eax	/ , <retval>
	jmp	.L27	/ 
.L37:
/  src/kstdio.c:118:                     return 12;
	movl	$12, %eax	/ , <retval>
	jmp	.L27	/ 
.L32:
/  src/kstdio.c:128:     return 0;
	movl	$0, %eax	/ , <retval>
	jmp	.L27	/ 
.L38:
/  src/kstdio.c:67:             switch (format[(*index) + 1]) {
	movl	$4, %eax	/ , <retval>
	jmp	.L27	/ 
.L54:
/  src/kstdio.c:100:                     return 5;
	movl	$5, %eax	/ , <retval>
	jmp	.L27	/ 
.L59:
/  src/kstdio.c:123:             terminal_putchar(format[*index]);
	subl	$12, %esp	/ ,
	movsbl	%al, %eax	/  _8, _8
	pushl	%eax	/  _8
	call	terminal_putchar	/ 
	addl	$16, %esp	/ ,
/  src/kstdio.c:128:     return 0;
	movl	$0, %eax	/ , <retval>
	jmp	.L27	/ 
.L53:
	movl	$0, %eax	/ , <retval>
	jmp	.L27	/ 
	.size	conversion_specifier, .-conversion_specifier
	.globl	color_setting
	.data
	.align 4
	.type	color_setting, @object
	.size	color_setting, 4
color_setting:
	.long	-1
	.ident	"GCC: (GNU) 10.2.0"

	.file	"kstring.c"
/  GNU C17 (GCC) version 10.2.0 (i686-elf)
/ 	compiled by GNU C version 9.3.0, GMP version 6.2.0, MPFR version 4.0.2, MPC version 1.1.0, isl version isl-0.22.1-GMP

/  GGC heuristics: --param ggc-min-expand=100 --param ggc-min-heapsize=131072
/  options passed:  -fpreprocessed build/objs/kstring.i -mtune=generic
/  -march=pentiumpro -auxbase-strip build/objs/kstring.o -Og -Wall -Wextra
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
	.string	"src/kstring.c"
.LC1:
	.string	"%4[file: %s :: line: %i]: "
.LC2:
	.string	"Condition Failed: "
.LC3:
	.string	"str != NULL"
	.text
	.globl	kstrlen
	.type	kstrlen, @function
kstrlen:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	subl	$8, %esp	/ ,
	movl	8(%ebp), %edx	/  str, str
/  src/kstring.c:4:     kassert(str != NULL, 0);
	testl	%edx, %edx	/  str
	je	.L7	/ ,
/  src/kstring.c:6:     size_t len = 0;
	movl	$0, %eax	/ , <retval>
	jmp	.L2	/ 
.L7:
/  src/kstring.c:4:     kassert(str != NULL, 0);
	subl	$4, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC0	/ 
	pushl	$.LC1	/ 
	call	kprintf	/ 
/  src/kstring.c:4:     kassert(str != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC2	/ 
	call	terminal_writestring_color	/ 
/  src/kstring.c:4:     kassert(str != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC3	/ 
	call	terminal_writestring_color	/ 
/  src/kstring.c:4:     kassert(str != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$10	/ 
	call	terminal_putchar_color	/ 
/  src/kstring.c:4:     kassert(str != NULL, 0);
	addl	$16, %esp	/ ,
	movl	$0, %eax	/ , <retval>
	jmp	.L1	/ 
.L4:
/  src/kstring.c:8:         len++;
	addl	$1, %eax	/ , <retval>
.L2:
/  src/kstring.c:7:     while (str[len])
	cmpb	$0, (%edx,%eax)	/ , *_1
	jne	.L4	/ ,
.L1:
/  src/kstring.c:10: }
	leave	
	ret	
	.size	kstrlen, .-kstrlen
	.section	.rodata.str1.1
.LC4:
	.string	"lhs != NULL && rhs != NULL"
	.text
	.globl	kstrcmp
	.type	kstrcmp, @function
kstrcmp:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%esi	/ 
	pushl	%ebx	/ 
	movl	8(%ebp), %ebx	/  lhs, lhs
	movl	12(%ebp), %esi	/  rhs, rhs
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	testl	%ebx, %ebx	/  lhs
	sete	%dl	/ , tmp93
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	testl	%esi, %esi	/  rhs
	sete	%al	/ , tmp94
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	orb	%al, %dl	/  tmp94, tmp93
	jne	.L16	/ ,
/  src/kstring.c:15: 	size_t index = 0u;
	movl	$0, %eax	/ , index
.L9:
/  src/kstring.c:16: 	while (lhs[index]) {
	movzbl	(%ebx,%eax), %edx	/  *_6, _7
/  src/kstring.c:16: 	while (lhs[index]) {
	testb	%dl, %dl	/  _7
	je	.L17	/ ,
/  src/kstring.c:17: 		if (lhs[index] == rhs[index]) {
	movzbl	(%esi,%eax), %ecx	/  *_4, _5
/  src/kstring.c:17: 		if (lhs[index] == rhs[index]) {
	cmpb	%dl, %cl	/  _7, _5
	jne	.L11	/ ,
/  src/kstring.c:18: 			index++;
	addl	$1, %eax	/ , index
	jmp	.L9	/ 
.L16:
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	subl	$4, %esp	/ ,
	pushl	$13	/ 
	pushl	$.LC0	/ 
	pushl	$.LC1	/ 
	call	kprintf	/ 
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC2	/ 
	call	terminal_writestring_color	/ 
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$.LC4	/ 
	call	terminal_writestring_color	/ 
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	addl	$8, %esp	/ ,
	pushl	$4	/ 
	pushl	$10	/ 
	call	terminal_putchar_color	/ 
/  src/kstring.c:13:     kassert(lhs != NULL && rhs != NULL, 0);
	addl	$16, %esp	/ ,
	movl	$0, %eax	/ , <retval>
	jmp	.L8	/ 
.L11:
/  src/kstring.c:19: 		} else if (lhs[index] < rhs[index]) {
	jle	.L14	/ ,
/  src/kstring.c:20: 			return -1;
	movl	$-1, %eax	/ , <retval>
	jmp	.L8	/ 
.L17:
/  src/kstring.c:25: 	return 0;
	movl	$0, %eax	/ , <retval>
.L8:
/  src/kstring.c:26: }
	leal	-8(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%ebp	/ 
	ret	
.L14:
/  src/kstring.c:22: 			return 1;
	movl	$1, %eax	/ , <retval>
	jmp	.L8	/ 
	.size	kstrcmp, .-kstrcmp
	.globl	__moddi3
	.globl	__divdi3
	.globl	kint_to_string
	.type	kint_to_string, @function
kint_to_string:
	pushl	%ebp	/ 
	movl	%esp, %ebp	/ ,
	pushl	%edi	/ 
	pushl	%esi	/ 
	pushl	%ebx	/ 
	subl	$28, %esp	/ ,
	movl	8(%ebp), %esi	/  input, input
	movl	12(%ebp), %edi	/  input, input
/  src/kstring.c:30:     if(0u < ret_size && input < 0u) {
	cmpl	$0, 20(%ebp)	/ , ret_size
	setne	%dl	/ , tmp107
/  src/kstring.c:30:     if(0u < ret_size && input < 0u) {
	movl	%edi, %eax	/  input, tmp108
	shrl	$31, %eax	/ , tmp108
/  src/kstring.c:30:     if(0u < ret_size && input < 0u) {
	testb	%al, %dl	/  tmp108, tmp107
	je	.L26	/ ,
/  src/kstring.c:31:         string_ret[index++] = '-';
	movl	16(%ebp), %eax	/  string_ret, tmp137
	movb	$45, (%eax)	/ , *string_ret_29(D)
/  src/kstring.c:32:         input *= -1;
	negl	%esi	/  input
	adcl	$0, %edi	/ , input
	negl	%edi	/  input
/  src/kstring.c:31:         string_ret[index++] = '-';
	movl	$1, -28(%ebp)	/ , %sfp
	jmp	.L21	/ 
.L26:
/  src/kstring.c:29:     size_t index = 0u;
	movl	$0, -28(%ebp)	/ , %sfp
	jmp	.L21	/ 
.L30:
/  src/kstring.c:39:             string_ret[index++] = current_char;
	movl	16(%ebp), %ecx	/  string_ret, tmp139
	movb	%bl, (%ecx,%eax)	/  _6, *_7
/  src/kstring.c:39:             string_ret[index++] = current_char;
	leal	1(%eax), %eax	/ , index
	movl	%eax, -28(%ebp)	/  index, %sfp
.L20:
/  src/kstring.c:41:     } while(input);
	movl	%edi, %edx	/  input, tmp124
	orl	%esi, %edx	/  tmp123, tmp124
	je	.L29	/ ,
.L21:
/  src/kstring.c:35:         size_t current_input = input % 10u;
	pushl	$0	/ 
	pushl	$10	/ 
	pushl	%edi	/  input
	pushl	%esi	/  input
	call	__moddi3	/ 
	addl	$16, %esp	/ ,
	movl	%eax, %ebx	/  tmp127, _4
/  src/kstring.c:36:         input /= 10u;
	pushl	$0	/ 
	pushl	$10	/ 
	pushl	%edi	/  input
	pushl	%esi	/  input
	call	__divdi3	/ 
	addl	$16, %esp	/ ,
	movl	%eax, %esi	/  tmp126, input
	movl	%edx, %edi	/ , input
/  src/kstring.c:37:         const char current_char = current_input + 48u;
	addl	$48, %ebx	/ , _6
/  src/kstring.c:38:         if(index < ret_size) {
	movl	-28(%ebp), %eax	/  %sfp, index
	cmpl	20(%ebp), %eax	/  ret_size, index
	jnb	.L20	/ ,
	jmp	.L30	/ 
.L29:
/  src/kstring.c:42:     if(index < ret_size) {
	movl	-28(%ebp), %eax	/  %sfp, index
	cmpl	20(%ebp), %eax	/  ret_size, index
	jnb	.L22	/ ,
/  src/kstring.c:43:         string_ret[index] = '\0';
	movl	16(%ebp), %edi	/  string_ret, tmp145
	movb	$0, (%edi,%eax)	/ , *_8
.L22:
/  src/kstring.c:45:     const size_t start_index = (string_ret[0u] == '-') ? 1u : 0u;
	movl	16(%ebp), %eax	/  string_ret, tmp147
	cmpb	$45, (%eax)	/ , *string_ret_29(D)
	je	.L31	/ ,
	movl	$0, %esi	/ , iftmp.2_21
.L23:
/  src/kstring.c:46:     for(size_t i = 0u; i < index/2u; ++i) {
	movl	$0, %eax	/ , i
	movl	%esi, -32(%ebp)	/  iftmp.2_21, %sfp
	movl	16(%ebp), %esi	/  string_ret, string_ret
/  src/kstring.c:46:     for(size_t i = 0u; i < index/2u; ++i) {
	jmp	.L24	/ 
.L31:
/  src/kstring.c:45:     const size_t start_index = (string_ret[0u] == '-') ? 1u : 0u;
	movl	$1, %esi	/ , iftmp.2_21
	jmp	.L23	/ 
.L25:
/  src/kstring.c:47:         const char temp = string_ret[i+start_index];
	movl	-32(%ebp), %edi	/  %sfp, iftmp.2_21
	leal	(%eax,%edi), %edx	/ , _10
/  src/kstring.c:47:         const char temp = string_ret[i+start_index];
	addl	%esi, %edx	/  string_ret, _11
/  src/kstring.c:47:         const char temp = string_ret[i+start_index];
	movzbl	(%edx), %edi	/  *_11, temp
/  src/kstring.c:48:         string_ret[i+start_index] = string_ret[index-i-1u];
	movl	-28(%ebp), %ecx	/  %sfp, _12
	subl	%eax, %ecx	/  i, _12
/  src/kstring.c:48:         string_ret[i+start_index] = string_ret[index-i-1u];
	leal	-1(%esi,%ecx), %ecx	/ , _14
/  src/kstring.c:48:         string_ret[i+start_index] = string_ret[index-i-1u];
	movzbl	(%ecx), %ebx	/  *_14, tmp150
	movb	%bl, (%edx)	/  tmp150, *_11
/  src/kstring.c:49:         string_ret[index-i-1u] = temp;
	movl	%edi, %ebx	/  temp, temp
	movb	%bl, (%ecx)	/  temp, *_14
/  src/kstring.c:46:     for(size_t i = 0u; i < index/2u; ++i) {
	addl	$1, %eax	/ , i
.L24:
/  src/kstring.c:46:     for(size_t i = 0u; i < index/2u; ++i) {
	movl	-28(%ebp), %ebx	/  %sfp, _16
	shrl	%ebx	/  _16
/  src/kstring.c:46:     for(size_t i = 0u; i < index/2u; ++i) {
	cmpl	%eax, %ebx	/  i, _16
	ja	.L25	/ ,
/  src/kstring.c:51: }
	leal	-12(%ebp), %esp	/ ,
	popl	%ebx	/ 
	popl	%esi	/ 
	popl	%edi	/ 
	popl	%ebp	/ 
	ret	
	.size	kint_to_string, .-kint_to_string
	.ident	"GCC: (GNU) 10.2.0"

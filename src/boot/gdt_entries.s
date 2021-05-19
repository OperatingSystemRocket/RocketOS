section .text
extern gdt_entries_ptr
global gdt_load
gdt_load:
    push eax
    lgdt [gdt_entries_ptr]
    jmp 0x08:.loaded_cs
.loaded_cs:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    pop eax
    ret


global flush_tss
flush_tss:
	mov ax, (5 * 8) | 3 ; fifth 8-byte selector. Or with 3 to set the RPL (requested privilege level).
	ltr ax
	ret


global jump_usermode
extern test_user_function
jump_usermode:
	mov ax, (4 * 8) | 3 ; ring 3 data with bottom 2 bits set for ring 3
	mov ds, ax
	mov es, ax 
	mov fs, ax 
	mov gs, ax ; SS is handled by iret

	; set up the stack frame iret expects
	mov eax, esp
	push (4 * 8) | 3 ; data selector
	push eax ; current esp
	pushf ; eflags
	push (3 * 8) | 3 ; code selector (ring 3 code with bottom 2 bits set for ring 3)
	push test_user_function ; instruction address to return to
	iret


section .data
gdt:
.null:
	dq 0x0
.ring0_code:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 10011010b
	db 11001111b
	db 0x00
.ring0_data:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 10010010b
	db 11001111b
	db 0x00
.ring3_code:
	dw 0xffff
	dw 0x0000
	db 0x00
	db 11111010b
	db 11011111b
	db 0b
.ring3_data:
    dw 0xffff
    dw 0x0000
    db 0x00
    db 11110010b
	db 11011111b
	db 0b
.tss:
    resb 8 ; allocate space for tss, don't initialize
.end:
.descriptor:
	dw .end - gdt - 1
	dd gdt
ring0_code_segment equ gdt.ring0_code - gdt
ring0_data_segment equ gdt.ring0_data - gdt
ring3_code_segment equ gdt.ring3_code - gdt
ring3_data_segment equ gdt.ring3_data - gdt


section .text
global gdt_load
gdt_load:
    push eax
    lgdt [gdt.descriptor]
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


global install_tss
extern write_tss
install_tss:
	push gdt.tss
	call write_tss
	ret





; .text
; .globl loadPageDirectory
; loadPageDirectory:
; push %ebp
; mov %esp, %ebp
; mov 8(%esp), %eax
; mov %eax, %cr3
; mov %ebp, %esp
; pop %ebp
; ret


section .text
global loadPageDirectory
loadPageDirectory:
push ebp
mov ebp, esp
mov eax, [esp + 8]
mov cr3, eax
; mov esp, ebp
pop ebp
ret



; .text
; .globl enablePaging
; enablePaging:
; push %ebp
; mov %esp, %ebp
; mov %cr0, %eax
; or $0x80000000, %eax
; mov %eax, %cr0
; mov %ebp, %esp
; pop %ebp
; ret


section .text
global enablePaging
enablePaging:
push ebp
mov ebp, esp
mov eax, cr0
or eax, 80000000h
mov cr0, eax
mov esp, ebp
pop ebp
ret

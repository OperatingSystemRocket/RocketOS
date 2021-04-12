section .text
global get_faulting_address
get_faulting_address:
    mov eax, cr2
    ret

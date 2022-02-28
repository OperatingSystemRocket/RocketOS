section .text
global get_faulting_address
get_faulting_address:
    mov eax, cr2
    ret


global outb
outb:
    xchg edx, eax
    out dx, al
    ret

global outl
outl:
    xchg edx, eax
    out dx, eax
    ret


global inb
inb:
    mov edx, eax
    in al, dx
    ret

global inl
inl:
    mov edx, eax
    in eax, dx
    ret

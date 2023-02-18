%define KERNEL_OFFSET 0xC0000000
%define PAGE_SIZE 0x1000

%define V2P(a) ((a) - KERNEL_OFFSET)
%define P2V(a) ((a) + KERNEL_OFFSET)

section .bss
align PAGE_SIZE
resb PAGE_SIZE
BootStack:

section .text
global _start
_start:
    cli

    mov edi, eax
    mov esi, ebx

    ; Set up a known stack
    mov esp, (V2P(BootStack))

    extern boot_page_directory
    mov eax, (V2P(boot_page_directory))
    mov cr3, eax

    ; Set CR0.PG
    ; enabling Paging
    mov eax, cr0
    or eax, 1<<31
    mov cr0, eax

    ; Jump to kernel address space
    lea eax, [upper_memory]
    jmp eax

global upper_memory
upper_memory:
    ; Move stack pointer to kernel space
    mov eax, KERNEL_OFFSET
    add esp, eax

    ; Remove identity mapping
    xor eax, eax
    mov [boot_page_directory], eax

    ; Update page tables
    mov eax, cr3
    mov cr3, eax

    ; Jump to kernel_main()
    extern kernel_main
    mov eax, (kernel_main)
    push esi
    push edi
    call eax

    hlt
    jmp $

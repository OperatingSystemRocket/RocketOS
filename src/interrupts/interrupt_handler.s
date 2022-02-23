%macro save_context 0
    push ebp
    push edi
    push esi
    push edx
    push ecx
    push ebx
    push eax
%endmacro

%macro restore_context 0
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
%endmacro

%macro restore_kernel_segments 0
    push eax
    mov ds, eax
    mov es, eax
    mov es, eax
    mov gs, eax
    pop eax
%endmacro

%macro create_irq 1
    push eax
    push %1

    jmp irq_common_handler
%endmacro

section .text
global irq_common_handler
irq_common_handler:
    push ebp
    mov ebp, esp

    push eax
    push edx

    mov eax, esp

    extern system_call
    call system_call

    add esp, 8

    pop ebp

    iret


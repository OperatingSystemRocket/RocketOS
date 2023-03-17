section .text

global arch_save_context
arch_save_context:
    mov [eax + (0 * 4)], ebx
    mov [eax + (1 * 4)], edx
    mov [eax + (2 * 4)], esi
    mov [eax + (3 * 4)], edi
    mov [eax + (4 * 4)], ebp
    mov ecx, [esp]
    mov [eax + (5 * 4)], ecx ; eip
    lea ecx, [esp+8]
    mov [eax + (6 * 4)], ecx ; esp
    xor ecx, ecx
    ret

global arch_restore_context
arch_restore_context:
    mov esp, [eax + (6 * 4)]
    mov ecx, [eax + (5 * 4)] ; load eip into ecx
    mov ebp, [eax + (4 * 4)]
    mov edi, [eax + (3 * 4)]
    mov esi, [eax + (2 * 4)]
    mov edx, [eax + (1 * 4)]
    mov ebx, [eax + (0 * 4)]
    jmp ecx


global arch_enter_tasklet
arch_enter_tasklet:
    pop eax
    pop ecx
    jmp ecx


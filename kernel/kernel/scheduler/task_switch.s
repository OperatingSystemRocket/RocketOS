%macro restore_context_light 0
    pop eax
    pop ebx
    pop ecx
    pop edx
    pop esi
    pop edi
    pop ebp
%endmacro

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



section .text
extern print_place

global init_task_switch
init_task_switch:
; Switch to the new CR3
    call print_place
    mov eax, esp
    add eax, 4
    mov ecx, [eax]
    push ecx
    push 0
    extern get_process_cr3
    call get_process_cr3
    add esp, 4
    pop ecx
;    mov cr3, eax

    push ecx
    push 0
    extern get_context_address
    call get_context_address
    add esp, 4
    pop ecx
    mov esp, [eax]

    restore_context_light

    add esp, 8

    iret


global task_switch
task_switch:
    mov eax, esp
    add eax, 8
    mov edx, [eax]
    sub eax, 4
    mov ecx, [eax]

    mov eax, ss
    push eax
    mov eax, esp
    push eax

    pushfd
    mov eax, cs
    push eax
    lea eax, [resume_eip]
    push eax

    push 0

    push 0

    save_context

    push edx
    push ecx
    call get_context_address
    pop ecx
    pop edx
    mov [eax], esp


    push edx
    push ecx
    call get_process_cr3
    pop ecx
    pop edx
;    mov cr3, eax

    push edx
    push ecx
    call get_context_address
    pop ecx
    pop edx
    mov esp, [eax]

    restore_context

    add esp, 8

    iret

resume_eip:
    call print_place
    add esp, 8
    call print_place
    ret

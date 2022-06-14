section .text

extern scheduler_kill_current_process

global init_task_switch
init_task_switch:
    ; stack registers
    mov ebp, [eax + (6 * 4)]
    mov esp, [eax + (9 * 4)]

    ; eflags
    mov ecx, [eax + (8 * 4)]
    push ecx
    popfd

    ; eip
    lea ecx, [scheduler_kill_current_process]
    push ecx
    mov ecx, [eax + (7 * 4)]
    push ecx

    ; general purpose registers
    mov edi, [eax + (5 * 4)]
    mov esi, [eax + (4 * 4)]
    mov edx, [eax + (3 * 4)]
    mov ecx, [eax + (2 * 4)]
    push ecx ; avoid trashing ecx when we grab cr3, since we can't set cr3 from memory directly
    mov ebx, [eax + (1 * 4)]
    mov ecx, [eax + (10 * 4)] ; set ecx to the cr3 value
    mov eax, [eax + (0 * 4)]

    ; cr3
    mov cr3, ecx
    pop ecx ; restore our proper ecx register value we saved earlier

    ret


global save_current_task
save_current_task:
    ; stack registers
    mov [eax + (6 * 4)], ebp
    mov [eax + (9 * 4)], esp

    ; save ecx so we can trash it safely within this function
    mov [ebp + (2 * 4)], ecx

    ; eflags
    pushfd
    pop ecx
    mov [eax + (8 * 4)], ecx

    ; eip
    lea ecx, [resume_eip]
    mov [eax + (7 * 4)], ecx

    ; general purpose registers
    mov [eax + (5 * 4)], edi
    mov [eax + (4 * 4)], esi
    mov [eax + (3 * 4)], edx
    mov [eax + (1 * 4)], ebx
    mov [eax + (0 * 4)], eax

    ; cr3
    mov ecx, cr3
    mov [eax + (10 * 4)], ecx

resume_eip:
    ret


extern pic_send_eoi

global resume_task
resume_task:
    mov ebp, [eax + (6 * 4)]
    mov esp, [eax + (9 * 4)]

    ; eflags
    mov ecx, [eax + (8 * 4)]
    push ecx
    popfd

    ; eip and iret setup
    pushf
    push cs
    mov ecx, [eax + (7 * 4)]
    push ecx

    ; general purpose registers
    mov edi, [eax + (5 * 4)]
    mov esi, [eax + (4 * 4)]
    mov edx, [eax + (3 * 4)]
    mov ebx, [eax + (1 * 4)]

    mov ecx, [eax + (2 * 4)]
    push ecx ; avoid trashing ecx when we grab cr3, since we can't set cr3 from memory directly
    mov ecx, [eax + (10 * 4)] ; set ecx to the cr3 value

    mov eax, [eax + (0 * 4)]

    ; cr3
    mov cr3, ecx
    pop ecx ; restore our proper ecx register value we saved earlier

    push eax
    mov eax, 1
    call pic_send_eoi
    pop eax

    iret









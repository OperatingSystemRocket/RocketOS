section .text
global save_current_task
save_current_task:
    push ecx
    mov [eax + (0*4)], ebp
    mov [eax + (1*4)], esp
    lea ecx, [done]
    mov [eax + (2*4)], ecx
global done
done:
    pop ecx
    ret


global load_task
extern pic_send_eoi
load_task:
    mov ebp, [eax + (0*4)]
    mov esp, [eax + (1*4)]

    push eax
    mov eax, 1
    call pic_send_eoi
    pop eax

    sti
    pushf
    push cs
    mov ecx, [eax + (2*4)]
    push ecx

    iret


global resume_task
resume_task:
    mov ebp, [eax + (0*4)]
    mov esp, [eax + (1*4)]

    mov ecx, [eax + (2*4)]
    push ecx

    ret


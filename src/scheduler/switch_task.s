section .text
extern example_function_task
extern new_stack
extern pic_send_eoi
global switch_to_example_task
switch_to_example_task:
    mov esp, new_stack+4096
    mov ebp, new_stack

    mov eax, 1
    call pic_send_eoi

    sti
    pushf
    push cs
    push example_function_task

    iret


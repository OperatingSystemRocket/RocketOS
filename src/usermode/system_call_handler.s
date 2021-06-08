section .text
global trigger_interrupt
trigger_interrupt:
int 128
ret

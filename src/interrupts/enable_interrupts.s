section .text
global enable_interrupts
enable_interrupts:
sti
ret

global disable_interrupts
disable_interrupts:
cli
ret

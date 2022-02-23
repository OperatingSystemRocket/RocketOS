section .text
global print_int
print_int:
push ebp
mov ebp, esp
push edx
mov edx, 0
int 128
pop edx
pop ebp
ret


global print
print:
push ebp
mov ebp, esp
push edx
mov edx, 1
int 128
pop edx
pop ebp
ret


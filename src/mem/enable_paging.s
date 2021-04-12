section .text
global loadPageDirectory
loadPageDirectory:
push eax
mov eax, [esp + 8]
mov cr3, eax
pop eax
ret


section .text
global enablePaging
enablePaging:
push eax
mov eax, cr0
or eax, 80000000h
mov cr0, eax
pop eax
ret


section .text
global enable_ring0_write_protect
enable_ring0_write_protect:
push eax
mov eax, cr0
or eax, 10000h
mov cr0, eax
pop eax
ret

section .text
global load_page_directory
load_page_directory:
push eax
mov eax, [esp + 8]
mov cr3, eax
pop eax
ret


global get_current_cr3
get_current_cr3:
mov eax, cr3


global enable_paging
enable_paging:
push eax
mov eax, cr0
or eax, 80000000h
mov cr0, eax
pop eax
ret


global enable_ring0_write_protect
enable_ring0_write_protect:
push eax
mov eax, cr0
or eax, 10000h
mov cr0, eax
pop eax
ret

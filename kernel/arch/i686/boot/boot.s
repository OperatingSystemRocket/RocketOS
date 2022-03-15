MULTIBOOT2_HEADER_MAGIC equ 0xe85250d6
GRUB_MULTIBOOT_ARCHITECTURE_I386 equ 0
MULTIBOOT_HEADER_TAG_FRAMEBUFFER equ 5
MULTIBOOT_HEADER_TAG_OPTIONAL equ 1
MULTIBOOT_HEADER_TAG_END equ 0
STACK_SIZE equ 0x4000

section .text

global start
global _start
start:
_start:
        jmp     multiboot_entry

        align  8

multiboot_header:
        dd   MULTIBOOT2_HEADER_MAGIC
        dd   GRUB_MULTIBOOT_ARCHITECTURE_I386
        dd   multiboot_header_end - multiboot_header
        dd   -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))

framebuffer_tag_start:  
        dw MULTIBOOT_HEADER_TAG_FRAMEBUFFER
        dw MULTIBOOT_HEADER_TAG_OPTIONAL
        dd framebuffer_tag_end - framebuffer_tag_start
        dd 1024
        dd 768
        dd 32
framebuffer_tag_end:
        dw MULTIBOOT_HEADER_TAG_END
        dw 0
        dd 0
multiboot_header_end:
multiboot_entry:
        mov    esp, (stack_bottom + STACK_SIZE)

        push   0
        popf

        push   ebx
        push   eax

        extern kernel_main
        call  kernel_main

loop:   hlt
        jmp     loop

        align 16
        global stack_bottom
        stack_bottom:
        resb STACK_SIZE ; 16 KiB
        global stack_top
        stack_top:

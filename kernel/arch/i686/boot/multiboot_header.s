MULTIBOOT2_HEADER_MAGIC equ 0xe85250d6
GRUB_MULTIBOOT_ARCHITECTURE_I386 equ 0
MULTIBOOT_HEADER_TAG_FRAMEBUFFER equ 5
MULTIBOOT_HEADER_TAG_OPTIONAL equ 1
MULTIBOOT_HEADER_TAG_END equ 0
STACK_SIZE equ 0x4000


section .multiboot
align 0x8
multiboot_header: ; TODO: don't have the stack and other static data sections inside section .text
    dd MULTIBOOT2_HEADER_MAGIC
    dd GRUB_MULTIBOOT_ARCHITECTURE_I386
    dd multiboot_header_end - multiboot_header
    dd -(MULTIBOOT2_HEADER_MAGIC + GRUB_MULTIBOOT_ARCHITECTURE_I386 + (multiboot_header_end - multiboot_header))

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

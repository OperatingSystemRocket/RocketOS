%define PAGE_SIZE 0x1000
%define V2P(a) ((a) - KERNEL_OFFSET)
%define PAGE_PRESENT 0x001
%define PAGE_WRITE 0x002
%define PAGE_GLOBAL 0x100
%define ENTRIES_PER_PT 1024
%define KERNEL_OFFSET 0xC0000000


section .data
align PAGE_SIZE
boot_page_table:
    %assign addr 0
    %rep 1024
        dd addr | PAGE_PRESENT
        %assign addr (addr + 0x1000)
    %endrep
global boot_page_directory
boot_page_directory:
    %rep ENTRIES_PER_PT
        dd (boot_page_table - 0xC0000000) + PAGE_PRESENT
    %endrep


%define PAGE_SIZE 0x1000
%define PAGE_PRESENT 0x001
%define PAGE_WRITE 0x002
%define PAGE_GLOBAL 0x100
%define ENTRIES_PER_PT 1024
%define ENTRIES_PER_PD 1024
%define KERNEL_OFFSET 0xC0000000
%define V2P(a) ((a) - KERNEL_OFFSET)


section .data
global boot_page_table
align PAGE_SIZE
boot_page_table:
    %assign addr 0
    %rep ENTRIES_PER_PT
        dd addr | PAGE_PRESENT | PAGE_WRITE
        %assign addr (addr + 0x1000)
    %endrep
global boot_page_directory
boot_page_directory:
    %rep ENTRIES_PER_PD
        dd (boot_page_table - KERNEL_OFFSET) + PAGE_WRITE + PAGE_PRESENT
    %endrep


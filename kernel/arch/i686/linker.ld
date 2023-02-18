
ENTRY(_start)

KERNEL_OFFSET = 0xC0000000;
KERNEL_START = 0x00100000; /*0x10000;*/

SECTIONS
{
    . = KERNEL_START + KERNEL_OFFSET;

    .text : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        immutable_data_start = .;
        text_start = .;
        *(.multiboot)
        *(.text)
        text_end = .;
    }
    . = ALIGN(4K);
    .rodata : AT(ADDR(.rodata) - KERNEL_OFFSET)
    {
        rodata_start = .;
        *(.rodata)
        rodata_end = .;
        immutable_data_end = .;
    }

    . = ALIGN(4K);
    .data : AT(ADDR(.data) - KERNEL_OFFSET)
    {
        mutable_data_start = .;
        data_start = .;
        *(.data)
        data_end = .;
    }
    . = ALIGN(4K);
    .bss : AT(ADDR(.bss) - KERNEL_OFFSET)
    {
        bss_start = .;
        *(.COMMON)
        *(.bss)
        bss_end = .;
        mutable_data_end = .;
    }

    /* TODO: should be `kernel_end`*/
    endkernel = .; /* define address of extern variable to mark the end of the kernel address space */
}

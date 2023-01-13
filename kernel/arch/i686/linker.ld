
ENTRY(_start)

KERNEL_OFFSET = 0xC0000000;
KERNEL_START = 0x10000;

/* TODO: rename linker variables to be consistent and more readable */

SECTIONS
{
    . = KERNEL_START + KERNEL_OFFSET;
    /* TODO: should be `kernel_start`*/
    /* startkernel = .; */
    /* text_start = .; */
    .text : AT(ADDR(.text) - KERNEL_OFFSET)
    {
        *(.multiboot)
        *(.text)
    }
    /* text_end = .; */
    /* rodata_start = .; */
    .rodata : AT(ADDR(.rodata) - KERNEL_OFFSET)
    {
        *(.rodata)
    }
    /* rodata_end = .; */
    /*. = ALIGN(4K);*/
    /* mutable_data_start = .; */
    .data : AT(ADDR(.data) - KERNEL_OFFSET)
    {
        *(.data)
    }
    .bss : AT(ADDR(.bss) - KERNEL_OFFSET)
    {
        *(.COMMON)
        *(.bss)
    }
    /* mutable_data_end = .; */
    /* TODO: should be `kernel_end`*/
	endkernel = .; /*define address of extern variable to mark the end of the kernel address space*/
}

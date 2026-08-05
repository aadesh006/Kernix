ENTRY(_start)
OUTPUT_FORMAT("binary")
SECTIONS
{
    /* Use 0x100000 instead of 1M to be safe */
    . = 0x100000;

    /* NOTE: The space before the colon (.text :) is important! */
    .text :
    {
        *(.text)
    }

    .rodata :
    {
        *(.rodata)
    }

    .data :
    {
        *(.data)
    }

    .bss :
    {
        *(COMMON)
        *(.bss)
    }

    .asm :
    {
        *(.asm)
    }

    . = ALIGN(4096);
    _kernel_end = .;
}
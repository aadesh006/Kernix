ENTRY(_start)
OUTPUT_FORMAT(elf32-i386)

SECTIONS
{
    . = 0x100000;

    /* MUST BE FIRST FOR GRUB */
    .multiboot ALIGN(4) :
    {
        *(.multiboot)
    }

    .text ALIGN(4) :
    {
        *(.text)
    }

    .rodata ALIGN(4) :
    {
        *(.rodata)
    }

    .data ALIGN(4) :
    {
        *(.data)
    }

    .bss ALIGN(4) :
    {
        *(COMMON)
        *(.bss)
    }
}
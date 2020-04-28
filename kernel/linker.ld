ENTRY(_start)

KERNEL_VMA = 0xC0000000;

SECTIONS
{
    . = KERNEL_VMA + 1M;

    _kernel_start = .;

    .text ALIGN(4K) : AT(ADDR(.text) - KERNEL_VMA)
    {
        *(.multiboot)
        *(.text)
    }

   .rodata ALIGN(4K) : AT(ADDR(.rodata) - KERNEL_VMA)
   {
       *(.rodata)
   }

   .data ALIGN(4K) : AT(ADDR(.data) - KERNEL_VMA)
   {
        *(.data)
   }

   .bss ALIGN(4K) : AT(ADDR(.bss) - KERNEL_VMA)
   {
       *(COMMON)
       *(.bss)
       *(.bootstrap_stack)
   }

    _kernel_end = .;

    /DISCARD/ :
    {
        *(.comment)
    }
}

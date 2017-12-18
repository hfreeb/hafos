#include <drivers/vga/textmode.h>
#include <stdio.h>
#include <arch/i386/gdt/gdt.h>
#include <arch/i386/idt/idt.h>

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    gdt_init();
    idt_init();

    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello! \n");

    asm volatile ("int $0x3");
}

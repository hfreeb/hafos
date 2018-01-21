#include <drivers/vga/textmode.h>
#include <drivers/timer/timer.h>
#include <drivers/ps2/keyboard.h>
#include <stdio.h>
#include <arch/i386/gdt/gdt.h>
#include <arch/i386/int/idt.h>
#include <arch/i386/int/irq.h>

static int loop = 1;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    gdt_init();
    irq_init();
    idt_init();

    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello, terminal!\n");

    timer_install();
    keyboard_install();

    asm volatile ("sti");


    for (;;);
}

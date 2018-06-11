#include <multiboot.h>
#include <drivers/textmode.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>
#include <stdio.h>
#include <hafos/gdt.h>
#include <hafos/mm.h>
#include <hafos/idt.h>
#include <hafos/irq.h>

extern uint32_t kernel_end;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
void kernel_main(multiboot_info_t *mboot_header, uint32_t mboot_magic) {
    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        printf("Error: Not booted by a compliant bootloader.\n");
        return;
    }

    gdt_install();

    if (!mm_install(mboot_header)) {
        return;
    }

    irq_init();
    idt_init();

    timer_install();
    keyboard_install();

    asm volatile ("sti");

    for (;;);
}

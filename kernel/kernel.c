#include <drivers/vga/textmode.h>
#include <arch/i386/gdt.h>

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    gdt_init();

    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello!");
}

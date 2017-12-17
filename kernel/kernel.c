#include <drivers/vga/textmode.h>

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello!");
}

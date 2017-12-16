#include <drivers/vga/textmode.h>

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    textmode_init(80, 25);

    textmode_write_string("Hello!", textmode_make_attrib(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
}

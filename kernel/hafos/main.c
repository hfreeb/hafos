#include "io/textmode.h"

#include "kprint/kprint.h"

void kernel_main(void)
{
	textmode_init(textmode_make_attrib(VGA_COLOR_GREEN, VGA_COLOR_BLACK));

	textmode_write_string("Hello, kernel World!\n");
        kprintf("Hello %x\n", (unsigned int) &kernel_main);
}

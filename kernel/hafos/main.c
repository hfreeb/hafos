#include "textmode.h"

void kernel_main(void)
{
	textmode_init(textmode_make_attrib(VGA_COLOR_GREEN, VGA_COLOR_BLACK));
 
	textmode_write_string("Hello, kernel World!\n");
}

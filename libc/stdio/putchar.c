#include <stdio.h>
#include <stdint.h>

#ifdef __is_libk
#include <drivers/textmode.h>
#endif

int putchar(int ch) {
#ifdef __is_libk
    char c = (char) ch;
    textmode_putchar(c);
#endif
}

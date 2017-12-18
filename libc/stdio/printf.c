#include <stdio.h>

int printf(const char* format, ...) {
    va_list parameters;
    va_start(parameters, format);

    int out = vprintf(format, parameters);

    va_end(parameters);

    return out;
}

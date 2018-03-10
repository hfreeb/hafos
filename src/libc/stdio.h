#pragma once

#include <stdarg.h>

#define EOF (-1)

int putchar(int ch);

int vprintf(const char* format, va_list vlist);

int printf(const char* format, ...);

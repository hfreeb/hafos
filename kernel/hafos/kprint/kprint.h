#pragma once

#include <stdarg.h>

int kvprintf(const char *fmt, va_list vlist);

int kprintf(const char* format, ...);

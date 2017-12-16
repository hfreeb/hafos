#pragma once

#include <stddef.h>

void* memset(void* dest, int ch, size_t count);

void* memmove(void* dest, const void* src, size_t count);

size_t strlen(const char* str);

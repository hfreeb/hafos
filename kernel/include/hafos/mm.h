#pragma once

#include <multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

// Linear allocator
uintptr_t kmalloc(size_t size, bool aligned);

void framing_install(uintptr_t kernel_base, uintptr_t kernel_end, uintptr_t mmap_addr, size_t mmap_length);

void *frame_alloc(bool kernel, bool writeable);

void frame_free(void *ptr);

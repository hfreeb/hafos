#pragma once

#include <multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void paging_install(void);

void framing_init(uintptr_t kernel_base, uintptr_t kernel_end, uintptr_t mmap_addr, size_t mmap_length);

void *frame_alloc(void);

void frame_free(void *ptr);

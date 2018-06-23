#pragma once

#include <stdbool.h>
#include <multiboot.h>

void mm_install(void);

void *frame_alloc(void);

void frame_free(void *ptr);

#pragma once

#include <multiboot.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

//Linear allocator
uintptr_t kmalloc(size_t size);

struct __attribute__((packed)) page { 
    bool present:1,
    bool rw: 1,
    bool user: 1,
    bool write_through: 1,
    bool cache_disabled: 1,
    bool accessed: 1,
    bool dirty: 1,
    bool pat: 1,
    bool global: 1,
    uint32_t unused: 3,
    uint32_t frame: 20
};

struct page_table {
    struct page *pages[1024];
};

struct page_directory {
    struct page_table *tables[1024];
};

void paging_install(uintptr_t kernel_base, uintptr_t kernel_end, uintptr_t mmap_addr, size_t mmap_length);

void frame_alloc(page_t *page, bool kernel, bool writeable);

void frame_free(page_t *page);

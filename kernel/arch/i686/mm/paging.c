#include <hafos/mm.h>

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#define PAGE_BITMAP_LENGTH 16384 // (0x400 * 0x400) / 64

extern void paging_load_directory(uintptr_t);
extern void paging_enable(void);

struct __attribute__((packed)) page { 
    bool present: 1;
    bool rw: 1;
    bool user: 1;
    bool write_through: 1;
    bool cache_disabled: 1;
    bool accessed: 1;
    bool dirty: 1;
    bool pat: 1;
    bool global: 1;
    uint32_t unused: 3;
    uint32_t frame: 20;
};

struct page_table {
    struct page pages[1024];
};

struct page_directory {
    struct page_tables *tables[1024];
};

unsigned long page_bitmap[PAGE_BITMAP_LENGTH];
struct page_directory *kernel_directory;

inline __attribute__((always_inline))
size_t get_page_id(uintptr_t addr) {
    return addr >> 12;
}

bool get_page_status(size_t id) {
    return (page_bitmap[id / 64] >> id % 64) & 1UL;
}

void set_page_status(size_t id, bool val) {
    if (val) {
        page_bitmap[id / 64] |= 1UL << (id % 64);
    } else {
        page_bitmap[id / 64] &= ~(1UL << (id % 64));
    }
}

void set_range_status(uintptr_t addr_start, uintptr_t addr_end, bool val) {
    for (size_t page = get_page_id(addr_start); page <= get_page_id(addr_end); ++page) {
        set_page_status(page, val);
    }
}

void *frame_alloc(bool kernel, bool writeable) {
    for (size_t i = 0; i < PAGE_BITMAP_LENGTH * 64; ++i) {
        if (!get_page_status(i)) {
            set_page_status(i, true);
            //TODO: Get page
            page->frame = i;
            page->present = 1
            page->rw = writeable ? 1 : 0
            page->user = kernel ? 0 : 1 
        }
    } 

    return;
}

void frame_free(void *ptr) {
    set_page_status(get_page_id(ptr), false);
    // Set page data for MMU
}

void framing_install(uintptr_t kernel_base, uintptr_t kernel_end, uintptr_t mmap_addr, size_t mmap_length) {
    kernel_directory = (struct page_directory *) kmalloc(0x1000, true);
    memset(kernel_directory, 0, 0x1000);
    for (int i = 0; i < 0x400; ++i) {
        struct page_table *table = (struct page_table *) kmalloc(0x1000, true);
        memset(table, 0, 0x1000);
        for (int j = 0; j < 0x400; ++j) {
            uintptr_t addr = (i * 0x400 + j) * 0x1000; 
            struct page p = table->pages[j];

            if ((addr >= kernel_base && addr <= kernel_end) || )
        }

        // SUPERVISOR | WRITE_ENABLED | PRESENT
        kernel_directory->tables[i] = ((uint32_t) table) | 3;
    }

    paging_load_directory((uintptr_t) kernel_directory);
    paging_enable();

    // Page frame allocation setup
    for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mmap_addr;
            (unsigned long) mmap < mmap_addr + mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
        set_range_status(mmap->addr, (uintptr_t) mmap + mmap->len, true);
    }

    set_range_status(kernel_base, kernel_end, true);
}

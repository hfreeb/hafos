#include <hafos/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

//TODO: 16384 or 32768?
#define PAGE_BITMAP_LENGTH 32768

unsigned long page_bitmap[PAGE_BITMAP_LENGTH];

inline __attribute__((always_inline))
size_t get_page_id(uintptr_t addr) {
    return addr >> 12;
}

bool get_page_status(size_t id) {
    return (page_bitmap[id / 64] >> id % 64) & 1U;
}

void set_page_status(size_t id, bool val) {
    page_bitmap[id / 64] ^= (-page_bitmap[id / 64] ^ val) & (1UL << (id % 64));
}

void set_range_status(uintptr_t addr_start, uintptr_t addr_end, bool val) {
    for (size_t page = get_page_id(addr_start); page <= get_page_id(addr_end); ++page) {
        set_page_status(page, val);
    }
}

void framing_init(uintptr_t kernel_base, uintptr_t kernel_end, uintptr_t mmap_addr, size_t mmap_length) {
    for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mmap_addr;
            (unsigned long) mmap < mmap_addr + mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
        set_range_status(mmap->addr, (uintptr_t) mmap + mmap->len, true);
    }

    set_range_status(kernel_base, kernel_end, true);
}

/*
bool mm_install(multiboot_info_t *mboot_header) {
    if (mboot_header -> flags & (1 << 6) == 0) {
        printf("Error: No Multiboot memory map was provided.\n");
        return false;
    }

    mmap_addr = mboot_header->mmap_addr;
    mmap_length = mboot_header->mmap_length;

    for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mboot_header->mmap_addr;
            (unsigned long) mmap < mboot_header->mmap_addr + mboot_header->mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        printf ("base_addr = 0x%08X, length = 0x%08X, type = 0x%X\n",
                (unsigned int) (mmap->addr & 0xFFFFFFFF),
                (unsigned int) (mmap->len & 0xFFFFFFFF),
                (unsigned int) mmap->type);
    }

    return true;
}*/

void *frame_alloc(void) {
    for (size_t i = 0; i < PAGE_BITMAP_LENGTH * 64; ++i) {
        if (!get_page_status(i)) {
            printf("%d is not taken\n", i);
            set_page_status(i, true);
            return (void *) (i << 12);
        }
    } 

    return 0;
}

void frame_free(void *ptr) {
    set_page_status(get_page_id((uintptr_t) ptr), false);
}

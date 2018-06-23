#include <hafos/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <hafos/paging.h>

//TODO: 16384 or 32768?
unsigned long page_bitmap[16384];

size_t get_page(uintptr_t addr) {
    return page_id = addr << 12;
}

void set_page(size_t id, bool val) {
    page_bitmap[id / 64] &= val << (id % 64);  
}

void set_range(size_t addr_start, size_t addr_end, bool val) {
     
}

void framing_init(uint32_t kernel_base, uint32_t kernel_end, uintptr_t mmap_addr, size_t mmap_size) {
    for (size_t page = get_page(kernel_base); page <= get_page(kernel_end); ++page) {
        set_page(page, true);
    }
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
    
}

void frame_free(void *ptr);

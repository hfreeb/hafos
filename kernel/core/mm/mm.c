#include <hafos/mm.h>

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <hafos/paging.h>

extern uint32_t kernel_base;
extern uint32_t kernel_end;

static uintptr_t mmap_addr;
static size_t mmap_length;

bool mm_install(multiboot_info_t *mboot_header) {
    if (mboot_header -> flags & (1 << 6) == 0) {
        printf("Error: No Multiboot memory map was provided.\n");
        return false;
    }

    mmap_addr = mboot_header->mmap_addr;
    mmap_length = mboot_header->mmap_length;

    paging_install();

    /*
    for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mboot_header->mmap_addr;
            (unsigned long) mmap < mboot_header->mmap_addr + mboot_header->mmap_length;
            mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

        printf ("base_addr = 0x%08X, length = 0x%08X, type = 0x%X\n",
                (unsigned int) (mmap->addr & 0xFFFFFFFF),
                (unsigned int) (mmap->len & 0xFFFFFFFF),
                (unsigned int) mmap->type);
    }*/

    return true;
}

#include <hafos/mm.h>

size_t frame_count;
uint64_t *frame_bitmap;

void paging_install(uint32_t mem_size, uintptr_t mmap_addr, size_t mmap_length) {
    frame_count = mem_size / 4; //mem_size is in KiB, and one page is 4KiB 
    frame_bitmap = (uint64_t *) kmalloc(frame_count / 64);
    memset(frame_bitmap, 0, frame_count / 64);

    //Allocate kernel directory

    //Maybe don't use multiboot? https://github.com/klange/toaruos/blob/60016e6e2b15e925234fa63a49e1b497d32d383f/kernel/core/mem.c
    for (multiboot_memory_map_t *mmap = (multiboot_memory_map_t *) mmap_addr; (unsigned long) mmap < mmap_addr + mmap_length; mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {
        //TODO: If type != 1 (available), set frame to used in bitmap
    }
}

#include <hafos/mm.h>

#include <stdbool.h>

extern void *kernel_end;
uintptr_t current_addr = (uintptr_t) &kernel_end;

uintptr_t kmalloc(size_t size, bool aligned) {
    if (aligned && (current_addr & 0x00000FFF)) {
        current_addr &= 0xFFFFF000;
        current_addr += 0x1000;
    }

    //TODO: giving physical address
    uintptr_t addr = current_addr;
    current_addr += size;
    return addr;
}
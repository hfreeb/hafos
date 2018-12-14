#include <hafos/mm.h>

extern uintptr_t kernel_end;
uintptr_t current_addr = (uintptr_t) &kernel_end;

uintptr_t kmalloc(size_t size) {
    //TODO: aligning flag
    //TODO: giving physical address
    uintptr_t addr = current_addr;
    current_addr += size;
    return addr;
}

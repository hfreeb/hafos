#include <hafos/paging.h>
#include <stdint.h>

extern void paging_load_directory(uintptr_t);
extern void paging_enable(void);

static uint32_t page_directory[1024] __attribute__((aligned(4096)));
static uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void paging_install(void) {
    for (int i = 0; i < 1024; ++i) {
        //SUPERVISOR | WRITE_ENABLED | NOT_PRESENT
        page_directory[i] = 0x00000002;
    }

    for (int i = 0; i < 1024; ++i) {
        //SUPERVISOR | WRITE_ENABLED | PRESENT
        first_page_table[i] = (i * 0x1000) | 3;
    }

    page_directory[0] = ((uint32_t) first_page_table) | 3;

    paging_load_directory(page_directory);
    paging_enable();
}

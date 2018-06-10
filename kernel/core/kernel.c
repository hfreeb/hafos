#include <multiboot.h>
#include <drivers/textmode.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>
#include <stdio.h>
#include <hafos/gdt.h>
#include <hafos/paging.h>
#include <hafos/idt.h>
#include <hafos/irq.h>

extern uint32_t kernel_end;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
void kernel_main(multiboot_info_t *mbi, uint32_t mb_magic) {
    gdt_install();
    paging_install();
    irq_init();
    idt_init();

    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello, terminal!\n");

    if (mbi->flags & (1 << 6)) {
        multiboot_memory_map_t *mmap;

        printf ("mmap_addr = 0x%X, mmap_length = 0x%X\n", (unsigned) mbi->mmap_addr, (unsigned) mbi->mmap_length);

        for (mmap = (multiboot_memory_map_t *) mbi->mmap_addr;
                (unsigned long) mmap < mbi->mmap_addr + mbi->mmap_length;
                mmap = (multiboot_memory_map_t *) ((unsigned long) mmap + mmap->size + sizeof (mmap->size))) {

            printf ("base_addr = 0x%08X, length = 0x%08X, type = 0x%X\n",
                    (unsigned int) (mmap->addr & 0xFFFFFFFF),
                    (unsigned int) (mmap->len & 0xFFFFFFFF),
                    (unsigned int) mmap->type);
        }
    }

    timer_install();
    keyboard_install();

    asm volatile ("sti");


    for (;;);
}

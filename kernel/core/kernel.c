#include <multiboot.h>
#include <drivers/textmode.h>
#include <drivers/timer.h>
#include <drivers/keyboard.h>
#include <hafos/gdt.h>
#include <hafos/mm.h>
#include <hafos/idt.h>
#include <hafos/irq.h>
#include <hafos/log.h>

extern uintptr_t kernel_base;
extern uintptr_t kernel_end;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
void kernel_main(multiboot_info_t *mboot_header, uint32_t mboot_magic) {
    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    log(LOG_STATUS_OK, "Textmode intiailized.");

    if (mboot_magic != MULTIBOOT_BOOTLOADER_MAGIC) {
        log(LOG_STATUS_ERROR, "Not booted by a compliant bootloader.");
        return;
    }

    gdt_install();
    log(LOG_STATUS_OK, "GDT installed.");

    paging_install();
    log(LOG_STATUS_OK, "Paging installed.");

    if ((mboot_header->flags & (1 << 6)) == 0) {
        log(LOG_STATUS_ERROR, "No Multiboot memory map was provided.");
        return;
    }
    framing_init(kernel_base, kernel_end, mboot_header->mmap_addr, mboot_header->mmap_length);
    log(LOG_STATUS_OK, "Page frame allocator initialized.");

    irq_init();
    log(LOG_STATUS_OK, "IRQ initialized.");
    idt_init();
    log(LOG_STATUS_OK, "IDT initialized.");

    timer_install();
    log(LOG_STATUS_OK, "Timer installed.");
    keyboard_install();
    log(LOG_STATUS_OK, "Keyboard installed");

    asm volatile ("sti");

    log(LOG_STATUS_OK, "Boot successful.");

    for(;;);
}

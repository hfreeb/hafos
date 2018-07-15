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
    textmode_init(textmode_make_attrib(VGA_COLOR_WHITE, VGA_COLOR_BLACK));
    textmode_write_string_attrib(textmode_make_attrib(VGA_COLOR_YELLOW, VGA_COLOR_BLACK), "[Hafos v0.1]\n\n");
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

    //TODO: Implement puts
    textmode_write_string_attrib(textmode_make_attrib(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK), "\nTesting colours:\n");
    for (int i = 0; i < 16; ++i) {
        textmode_putchar_attrib(textmode_make_attrib(VGA_COLOR_BLACK, i), ' ');
    }
    textmode_write_string("\n\n");

    textmode_write_string_attrib(textmode_make_attrib(VGA_COLOR_LIGHT_BLUE, VGA_COLOR_BLACK), "==== Roadmap ====\n- Heap\n- VFS\n- Initial ramdisk\n- Task switching\n- User mode execution\n- EXT2\n\n");

    asm volatile ("sti");

    log(LOG_STATUS_OK, "Boot successful.");

    for(;;);
}

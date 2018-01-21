#include <drivers/vga/textmode.h>
#include <stdio.h>
#include <arch/i386/gdt/gdt.h>
#include <arch/i386/int/idt.h>
#include <arch/i386/int/irq.h>

//static int wait_count = 0;
//static bool should_spam = 1;

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void keyboard_int_handler(struct int_state state) {
    printf("Keyboard interrupt!");
//    should_spam = 0;
}

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_isr_handler(struct int_state state) {
    printf("handling int $0x3!\n");
}

#pragma GCC diagnostic ignored "-Wmissing-prototypes"
void kernel_main(void) {
    gdt_init();
    irq_init();
    idt_init();

    int_register_handler(0x03, kernel_isr_handler); 
    int_register_handler(0x21, keyboard_int_handler); 
    textmode_init(textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));

    textmode_write_string("Hello! \n");

    asm volatile ("int $0x3");
    asm volatile ("int $0x5");

    printf("Done interrupts");

/*    while (should_spam) {
        printf("Waiting! @%d\n", wait_count); 

        wait_count++;
    }*/
}

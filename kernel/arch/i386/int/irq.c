#include "irq.h"

#include <drivers/serial/io.h>

#define PIC1         0x20
#define PIC1_OFFSET  0x20
#define PIC1_COMMAND PIC1
#define PIC1_DATA    (PIC1 + 1)

#define PIC2         0xA0
#define PIC2_OFFSET  0x28
#define PIC2_END     0x2F
#define PIC2_COMMAND PIC2
#define PIC2_DATA    (PIC2 + 1)

#define PIC_EOI      0x20

#define ICW1_INIT    0x10
#define ICW1_ICW4    0x01

void irq_init(void) {
    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4); //ICW1: Starts the init sequence
    io_wait();
    outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
    io_wait();

    outb(PIC1_DATA, PIC1_OFFSET); //ICW2: Master PIC vector offset
    io_wait();
    outb(PIC2_DATA, PIC2_OFFSET); //ICW2: Slave PIC vector offset
    io_wait();
    
    outb(PIC1_DATA, 0x04); //ICW3: Tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
    io_wait();
    outb(PIC2_DATA, 0x02); //ICW3: Tell slave PIC its case identity (0000 0010)
    io_wait();

    outb(PIC1_DATA, 0x01);
    io_wait(); 
    outb(PIC2_DATA, 0x01);
    io_wait();
    outb(PIC1_DATA, 0x00);
    io_wait();
    outb(PIC1_DATA, 0x00);
    io_wait();
}

bool irq_check(uint8_t interrupt) {
    return interrupt >= PIC1_OFFSET && interrupt <= PIC2_END;
}

void irq_ack(uint8_t interrupt) {
    if (interrupt >= PIC2_OFFSET) {
        outb(PIC2_COMMAND, PIC_EOI);
    }
    
    outb(PIC1_COMMAND, PIC_EOI);
}

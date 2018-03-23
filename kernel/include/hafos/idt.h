#pragma once

#include <stdint.h>

struct int_state {
    uint32_t ds; //Data segment selector
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; //Registers
    uint32_t int_no, err_code; //Interrupt number
    uint32_t eip, cs, eflags, useresp, ss; //Pushed by CPU
};

typedef void (*int_handler_t)(struct int_state*);

void idt_init(void);

void idt_install(uint8_t interrupt, int_handler_t handler);

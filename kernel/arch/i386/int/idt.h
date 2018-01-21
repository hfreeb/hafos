#pragma once

#include <stdint.h>

void idt_init(void);

void idt_register_isr(uint8_t index, uint32_t base);

#pragma once

#include <hafos/idt.h>
#include <stdint.h>
#include <stdbool.h>

void irq_init(void);

void irq_install(uint8_t interrupt, int_handler_t handler);

void irq_eoi(uint8_t interrupt);

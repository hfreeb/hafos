#pragma once

#include "interrupt.h"
#include <stdint.h>
#include <stdbool.h>

void irq_register_handler(uint8_t interrupt, int_handler_t handler);

void irq_init(void);

bool irq_check(uint8_t interrupt);

void irq_ack(uint8_t interrupt);

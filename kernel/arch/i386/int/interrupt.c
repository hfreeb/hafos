#include "interrupt.h"

#include "irq.h"
#include <stdio.h>

static int_handler_t handlers[128];

void int_register_handler(uint32_t interrupt, int_handler_t handler) {
    handlers[interrupt] = handler;
}

void isr_handler(struct int_state *state) {
    if (irq_check(state->int_no)) {
        irq_ack(state->int_no);
    }

    if (handlers[state->int_no]) {
        int_handler_t handler = handlers[state->int_no];
        handler(state);
    }
}

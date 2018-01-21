#include "timer.h"

#include <stdio.h>
#include <arch/i386/int/interrupt.h>
#include <drivers/serial/io.h>

#define TIMER_INPUT_CLOCK 1193180
#define TIMER_COMMAND_REG 0x43
#define TIMER_ZERO_DATA   0x40     

#define TIMER_COUNTER_ZERO     0 << 6
#define TIMER_LSB_THEN_MSB     3 << 4
#define TIMER_MODE_SQUARE_WAVE 3 << 1

static uint64_t timer_ticks = 0;

void timer_handler(struct int_state *state) {
    timer_ticks++;
}

void timer_install(void) {
    //Set timer to use 100Hz clock
    int divisor = TIMER_INPUT_CLOCK / 100;
    outb(TIMER_COMMAND_REG, TIMER_COUNTER_ZERO | TIMER_LSB_THEN_MSB | TIMER_MODE_SQUARE_WAVE);
    outb(TIMER_ZERO_DATA, divisor & 0xFF);
    outb(TIMER_ZERO_DATA, divisor >> 8);

    int_register_handler(32, timer_handler); 
}

void timer_wait(uint64_t ticks) {
    uint64_t req_ticks = timer_ticks + ticks;

    while(timer_ticks < req_ticks);
}

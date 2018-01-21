#include "timer.h"

#include <stdio.h>
#include <arch/i386/int/interrupt.h>

static int timer_ticks = 0;

void timer_handler(struct int_state state) {
    timer_ticks++;

    printf("One tick has passed.\n");
}

void timer_install(void) {
   int_register_handler(32, timer_handler); 
}

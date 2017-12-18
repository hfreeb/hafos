#include "isr.h"

#include <stdio.h>

void isr_handler(struct int_state state) {
    printf("Recieved interrupt 0x%x", state.int_no);
}

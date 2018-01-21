#pragma once

#include <stdint.h>

void timer_install(void);

void timer_wait(uint64_t ticks);

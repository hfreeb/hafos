#pragma once

#include <stdint.h>

enum log_status {
    LOG_STATUS_OK,
    LOG_STATUS_ERROR
};

void log(enum log_status status, const char *msg);

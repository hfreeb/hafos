#pragma once

#include <stdio.h>

#define LOG_STATUS_OK      "OK"
#define LOG_STATUS_WARNING "WARNING"
#define LOG_STATUS_ERROR   "ERROR" 

void log(const char *status, const char *msg);

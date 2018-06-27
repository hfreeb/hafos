#include <hafos/log.h>

void log(const char *status, const char *msg) {
    printf("[%s] %s\n", status, msg);
}

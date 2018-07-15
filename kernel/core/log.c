#include <hafos/log.h>

#include <drivers/textmode.h>
#include <stdio.h>

void log(enum log_status status, const char *msg) {
    textmode_putchar('[');

    if (status == LOG_STATUS_OK) {
        textmode_write_string_attrib(
                textmode_make_attrib(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK),
                "OK"
        );
    } else if (status == LOG_STATUS_ERROR) {
        textmode_write_string_attrib(
                textmode_make_attrib(VGA_COLOR_LIGHT_RED, VGA_COLOR_BLACK),
                "ERROR"
        );
    } else {
        textmode_write_string_attrib(
                textmode_make_attrib(VGA_COLOR_LIGHT_GRAY, VGA_COLOR_BLACK), 
                "INVALID"
        );
    }

    printf("] %s\n", msg);
}

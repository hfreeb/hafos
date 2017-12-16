#include "textmode.h"

#include <string.h>

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t vga_width;
static size_t vga_height;
static size_t vga_row;
static size_t vga_column;
static uint16_t* vga_buffer;

uint8_t textmode_make_attrib(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t textmode_make_entry(char c, uint8_t attrib) {
    return (uint16_t) c | (uint16_t) attrib << 8;
}

void textmode_init(size_t width, size_t height) {
     vga_row = 0;
     vga_column = 0;
     vga_width = width;
     vga_height = height;
     vga_buffer = VGA_MEMORY;
     textmode_clear();
}

void textmode_clear() {
    memset(vga_buffer, 0, vga_width * vga_height);
}

void textmode_put_entry_at(uint16_t entry, size_t x, size_t y) {
    vga_buffer[y * vga_width + x] = entry;
}

void textmode_scroll(void) {
    memmove(vga_buffer, vga_buffer + vga_width, vga_width * (vga_height - 1) * sizeof(*vga_buffer));

    memset(vga_buffer, 0, vga_width);
}

void textmode_putchar(char c, uint8_t attrib) {
    if (c == '\n') {
        if (vga_row == vga_height - 1) {
            textmode_scroll();
        } else {
            ++vga_row;
        }

        vga_column = 0;
        return;
    }

    uint16_t entry = textmode_make_entry(c, attrib);
    textmode_put_entry_at(entry, vga_column, vga_row);

    ++vga_column;
    if (vga_column == vga_width) {
        vga_column = 0;
        if (vga_row == vga_height - 1) {
            textmode_scroll();
        } else {
            ++vga_row;
        }
    }
}

void textmode_write(const char* data, size_t size, uint8_t attrib) {
    for (size_t i = 0; i < size; ++i) {
        textmode_putchar(data[i], attrib);
    }
}
void textmode_write_string(const char* data, uint8_t attrib) {
    textmode_write(data, strlen(data), attrib);
}

#include "textmode.h"

#include <string.h>
#include <drivers/serial/io.h>

#define VGA_WIDTH  80
#define VGA_HEIGHT 25

#define CMD_PORT      0x03D4
#define DATA_PORT     0x03D5
#define HIGH_BYTE_CMD 0x0E
#define LOW_BYTE_CMD  0x0F

static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t vga_row;
static size_t vga_column;
static uint8_t vga_attrib;
static uint16_t* vga_buffer;

uint8_t textmode_make_attrib(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

uint16_t textmode_make_entry(char c, uint8_t attrib) {
    return (uint16_t) c | (uint16_t) attrib << 8;
}

void textmode_init(uint8_t attrib) {
     vga_row = 0;
     vga_column = 0;
     vga_attrib = attrib;
     vga_buffer = VGA_MEMORY;
     textmode_clear();
}

void textmode_clear() {
    uint16_t entry = textmode_make_entry(' ', vga_attrib);

    for (int y = 0; y < VGA_HEIGHT; ++y) {
        for (int x = 0; x < VGA_WIDTH; ++x) {
            textmode_put_entry_at(entry, x, y);            
        }
    }
}

void textmode_set_attrib(uint8_t attrib) {
    vga_attrib = attrib;
}

void textmode_put_entry_at(uint16_t entry, size_t x, size_t y) {
    vga_buffer[y * VGA_WIDTH + x] = entry;
}

void textmode_scroll(void) {
    for (int row = 0; row < VGA_HEIGHT; ++row) {
        for (int col = 0; col < VGA_WIDTH; ++col) {
            if (row == VGA_HEIGHT - 1) {
                vga_buffer[VGA_WIDTH * row + col] = textmode_make_entry(' ', vga_attrib);
            } else {
                vga_buffer[VGA_WIDTH * row + col] = vga_buffer[VGA_WIDTH * (row + 1) + col];
            }
        }
    }
}

void textmode_set_cursor(size_t x, size_t y) {
    uint16_t pos = y * VGA_WIDTH + x;
    outb(CMD_PORT, HIGH_BYTE_CMD);
    outb(DATA_PORT, (pos >> 8) & 0x00FF);
    outb(CMD_PORT, LOW_BYTE_CMD);
    outb(DATA_PORT, pos & 0x00FF);
}

void textmode_putchar(char c) {
    if (c == '\n') {
        if (vga_row == VGA_HEIGHT - 1) {
            textmode_scroll();
        } else {
            ++vga_row;
        }

        vga_column = 0;
        textmode_set_cursor(vga_column, vga_row);
        return;
    }

    uint16_t entry = textmode_make_entry(c, vga_attrib);
    textmode_put_entry_at(entry, vga_column, vga_row);

    ++vga_column;
    if (vga_column == VGA_WIDTH) {
        vga_column = 0;
        if (vga_row == VGA_HEIGHT - 1) {
            textmode_scroll();
        } else {
            ++vga_row;
        }
    }

    textmode_set_cursor(vga_column, vga_row);
}

void textmode_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; ++i) {
        textmode_putchar(data[i]);
    }
}
void textmode_write_string(const char* data) {
    textmode_write(data, strlen(data));
}

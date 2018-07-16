#pragma once

#include <stdint.h>
#include <stddef.h>

enum vga_color {
	VGA_COLOR_BLACK = 0,
	VGA_COLOR_BLUE = 1,
	VGA_COLOR_GREEN = 2,
	VGA_COLOR_CYAN = 3,
	VGA_COLOR_RED = 4,
	VGA_COLOR_MAGENTA = 5,
	VGA_COLOR_BROWN = 6,
	VGA_COLOR_LIGHT_GRAY = 7,
	VGA_COLOR_DARK_GRAY = 8,
	VGA_COLOR_LIGHT_BLUE = 9,
	VGA_COLOR_LIGHT_GREEN = 10,
	VGA_COLOR_LIGHT_CYAN = 11,
	VGA_COLOR_LIGHT_RED = 12,
	VGA_COLOR_LIGHT_MAGENTA = 13,
	VGA_COLOR_YELLOW = 14,
	VGA_COLOR_WHITE = 15,
};
 
uint8_t textmode_make_attrib(enum vga_color fg, enum vga_color bg);
uint16_t textmode_make_entry(char c, uint8_t attrib);

void textmode_init(uint8_t attrib);
void textmode_clear(void);

void textmode_set_attrib(uint8_t attrib);

void textmode_put_entry_at(uint16_t entry, size_t x, size_t y);

void textmode_scroll(void);
void textmode_backspace(void);

void textmode_set_cursor(size_t x, size_t y);

void textmode_putchar_attrib(uint8_t attrib, char c);
void textmode_putchar(char c);

void textmode_write(uint8_t attrib, const char* data, size_t size);
void textmode_write_string_attrib(uint8_t attrib, const char* data);
void textmode_write_string(const char* data);

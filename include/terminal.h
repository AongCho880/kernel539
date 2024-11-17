#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"
#include "keyboard.h"

#define MAX_BUFFER_LINES 1024       // Adjust this as needed for terminal history
#define VGA_WIDTH  80              // Define VGA_WIDTH here as a constant
#define VGA_HEIGHT 25             // Define VGA_HEIGHT here as a constant

// VGA text mode color constants
enum vga_color {
    VGA_BLACK = 0,
    VGA_BLUE = 1,
    VGA_GREEN = 2,
    VGA_CYAN = 3,
    VGA_RED = 4,
    VGA_MAGENTA = 5,
    VGA_BROWN = 6,
    VGA_LIGHT_GREY = 7,
    VGA_DARK_GREY = 8,
    VGA_LIGHT_BLUE = 9,
    VGA_LIGHT_GREEN = 10,
    VGA_LIGHT_CYAN = 11,
    VGA_LIGHT_RED = 12,
    VGA_LIGHT_MAGENTA = 13,
    VGA_LIGHT_BROWN = 14,
    VGA_WHITE = 15,
};

// Terminal functions
void terminal_initialize(void);
void terminal_setcolor(uint8_t color);
void terminal_scroll_up(void);
void terminal_scroll_down(void);
void terminal_update_display(void);
void terminal_putchar(char c);
void terminal_writestring(const char* data);
void terminal_writeint(int num);
void terminal_writehex(uint32_t num);
void terminal_writechar(char c);

// Color and VGA entry functions
uint8_t make_color(enum vga_color fg, enum vga_color bg);
uint16_t make_vgaentry(char c, uint8_t color);

// Keyboard and terminal buffer configuration
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Scroll and display-related extern declarations (for use across other files)
extern size_t scroll_offset;      // Current scroll offset for the terminal
extern size_t total_lines;        // Total lines currently in the terminal buffer
extern uint16_t line_buffer[MAX_BUFFER_LINES][VGA_WIDTH];  // Consistent array size

#endif /* TERMINAL_H */

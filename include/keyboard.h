// keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "types.h" // Include this if you have custom definitions for uint8_t and size_t
#include "terminal.h"

// IO ports
#define KEYBOARD_DATA_PORT 0x60

// Keyboard scan codes (basic set)
#define SC_ENTER     0x1C
#define SC_BACKSPACE 0x0E
#define SC_CMD_BUF_SIZE 256

// Command buffer
extern char cmd_buffer[SC_CMD_BUF_SIZE];
extern size_t cmd_buffer_pos;

// Function declarations
void keyboard_handler();
void process_command();  // Assuming process_command() is implemented elsewhere
void terminal_putchar(char c);  // Assuming terminal_putchar() is implemented elsewhere

#endif // KEYBOARD_H

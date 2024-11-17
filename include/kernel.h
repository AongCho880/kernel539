#ifndef KERNEL_H
#define KERNEL_H

#include "keyboard.h"
#include "terminal.h"

// Keyboard scan codes (basic set)
#define SC_ENTER     0x1C
#define SC_BACKSPACE 0x0E
#define SC_CMD_BUF_SIZE 256

// IO ports
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Command buffer size
#define SC_CMD_BUF_SIZE 256

// Kernel command processing
void process_command();
void exit_kernel();

// Main kernel function
void kernel_main();

// Utility functions
size_t strlen(const char* str);
int strcmp(const char* s1, const char* s2);

// IO functions for port-based communication
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ( "inb %1, %0" : "=a"(ret) : "Nd"(port) );
    return ret;
}

static inline void outb(uint16_t port, uint8_t val) {
    asm volatile ( "outb %0, %1" : : "a"(val), "Nd"(port) );
}

#endif // KERNEL_H

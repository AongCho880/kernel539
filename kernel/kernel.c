#include "../include/kernel.h"


// Command buffer
char cmd_buffer[SC_CMD_BUF_SIZE];
size_t cmd_buffer_pos = 0;


// String functions
size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}


int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}



// Command processing
void process_command() {
    cmd_buffer[cmd_buffer_pos] = '\0';
    terminal_writestring("\n");

    if (strcmp(cmd_buffer, "help") == 0) {
        terminal_writestring("Available commands: \n");
        terminal_writestring("  help     - Show this help message \n");
        terminal_writestring("  clear    - Clear the screen \n");
        terminal_writestring("  color    - Change text color (cycles through colors) \n");
        terminal_writestring("  about    - About 539kernel \n");
        terminal_writestring("  exit     - Exit and halt the system \n");
    }
    else if (strcmp(cmd_buffer, "clear") == 0) {
        terminal_initialize();
    }
    else if (strcmp(cmd_buffer, "color") == 0) {
        static uint8_t current_color = VGA_WHITE;
        current_color = (current_color + 1) % 16;
        terminal_setcolor(make_color(current_color, VGA_BLACK));
    }
    else if (strcmp(cmd_buffer, "about") == 0) {
        terminal_writestring("539kernel - A simple educational kernel \n");
        terminal_writestring("Version 1.0 \n");
        terminal_writestring("Features: \n");
        terminal_writestring("- Basic command line interface \n");
        terminal_writestring("- Keyboard input \n");
        terminal_writestring("- Color text support \n");
        terminal_writestring("- Screen scrolling \n");
    }
    else if (strcmp(cmd_buffer, "exit") == 0) {
        exit_kernel();
    }
    else if (cmd_buffer_pos > 0) {
        terminal_writestring("Unknown command: ");
        terminal_writestring(cmd_buffer);
        terminal_writestring("\nType 'help' for available commands \n");
    }

    terminal_writestring("\n\n> ");
    cmd_buffer_pos = 0;
}

// Exit Kernel
void exit_kernel() {
    terminal_initialize();
    terminal_writestring("System halted. Press reset to restart.\n");
    while (1) {
        asm volatile ("hlt");
    }
}



// Main kernel function
void kernel_main() {
    terminal_initialize();
    terminal_writestring("Welcome to 539kernel!\n");
    terminal_writestring("Type 'help' for available commands\n");
    terminal_writestring("\n> ");

    // Simple command loop
    while (1) {
        // Check keyboard status
        if (inb(KEYBOARD_STATUS_PORT) & 1) {
            keyboard_handler();
        }
    }
}
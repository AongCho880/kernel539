#include "../include/keyboard.h"  // Include the keyboard header file
#include "../include/terminal.h"   // Include terminal functions

// IO ports
#define KEYBOARD_DATA_PORT 0x60

// Keyboard scan codes (basic set)
#define SC_ENTER     0x1C
#define SC_BACKSPACE 0x0E
#define SC_UP_ARROW  0x48   // Up Arrow scancode
#define SC_DOWN_ARROW 0x50  // Down Arrow scancode
#define SC_CMD_BUF_SIZE 256

// Command buffer
extern char cmd_buffer[SC_CMD_BUF_SIZE];  // Declaring it as external since it's defined elsewhere (in kernel.c or other files)
extern size_t cmd_buffer_pos;             // Declaring it as external

// Port input function
static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Port output function
static inline void outb(uint16_t port, uint8_t val) {
    asm volatile("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Keyboard handling function
void keyboard_handler() {
    uint8_t scancode = inb(KEYBOARD_DATA_PORT);  // Read the scancode from the keyboard data port

    // Ignore key releases (scancode with high bit set)
    if (scancode & 0x80) 
        return;

    // Handle special keys
    switch (scancode) {
        case SC_ENTER:
            process_command();  // Process the command in the buffer
            cmd_buffer_pos = 0; // Reset the buffer position after processing
            break;              // Added missing break statement

        case SC_BACKSPACE:
            if (cmd_buffer_pos > 0) {
                cmd_buffer_pos--;  // Move buffer position back
                cmd_buffer[cmd_buffer_pos] = '\0';  // Remove the last character from buffer

                // Handle the backspace in the terminal by moving the cursor back
                terminal_putchar('\b');
                terminal_putchar(' ');  // Clear the character on the screen
                terminal_putchar('\b');  // Move the cursor back again
            }
            break;

        case SC_UP_ARROW:
            terminal_scroll_up();   // Scroll up when the up arrow is pressed
            break;

        case SC_DOWN_ARROW:
            terminal_scroll_down(); // Scroll down when the down arrow is pressed
            break;

        default: {
            // Convert scancode to ASCII (basic mapping)
            static const char scancode_ascii[] = {
                0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
                '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
                0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
                0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
                '*', 0, ' '
            };

            // Check bounds and add character to command buffer
            if (scancode < sizeof(scancode_ascii) && scancode_ascii[scancode] && cmd_buffer_pos < SC_CMD_BUF_SIZE - 1) {
                char c = scancode_ascii[scancode];
                cmd_buffer[cmd_buffer_pos++] = c;
                terminal_putchar(c);  // Display the character
            }
        }
    }
}

#include "../include/terminal.h"
#include "../include/keyboard.h"

// Terminal dimensions and VGA memory address
static uint16_t* const VGA_MEMORY = (uint16_t*)0xB8000;

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

// Scrollable terminal state
size_t scroll_offset = 0;        // Tracks visible scroll offset
size_t total_lines = 0;          // Total lines currently in buffer history

// Line history buffer for scroll functionality
uint16_t line_buffer[MAX_BUFFER_LINES][VGA_WIDTH]; // Circular buffer for terminal history

// Function to create a VGA entry with character and color
uint16_t make_vgaentry(char c, uint8_t color) {
    return (uint16_t)c | (uint16_t)color << 8;
}

// Function to create a color byte from foreground and background color
uint8_t make_color(enum vga_color fg, enum vga_color bg) {
    return fg | bg << 4;
}

// Terminal initialization
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(VGA_WHITE, VGA_BLACK);
    terminal_buffer = VGA_MEMORY;
    scroll_offset = 0;
    total_lines = 0;

    // Initialize VGA memory and line buffer with spaces
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vgaentry(' ', terminal_color);
        }
    }

    // Initialize line buffer with spaces
    for (size_t y = 0; y < MAX_BUFFER_LINES; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            line_buffer[y][x] = make_vgaentry(' ', terminal_color);
        }
    }
}

// Set terminal color
void terminal_setcolor(uint8_t color) {
    terminal_color = color;
}

// Scroll terminal up by one line
void terminal_scroll_up() {
    if (scroll_offset + VGA_HEIGHT < total_lines) {
        scroll_offset++;
        terminal_update_display();
    }
}

// Scroll terminal down by one line
void terminal_scroll_down() {
    if (scroll_offset > 0) {
        scroll_offset--;
        terminal_update_display();
    }
}

// Update the display based on scroll offset
void terminal_update_display() {
    size_t visible_row = 0;

    // Copy lines starting from scroll_offset
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        size_t buffer_row = (scroll_offset + y) % MAX_BUFFER_LINES;
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[visible_row * VGA_WIDTH + x] = line_buffer[buffer_row][x];
        }
        visible_row++;
    }
}

// Adjust `terminal_putchar` to add lines to `line_buffer` sequentially
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;

        if (++terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1; // Keep the cursor at the last row
            total_lines = total_lines < MAX_BUFFER_LINES ? total_lines + 1 : MAX_BUFFER_LINES;
            terminal_scroll_up();  // Auto-scroll when the row limit is exceeded
        }
        return;
    }

    // Calculate index for terminal buffer and line buffer
    const size_t index = terminal_row * VGA_WIDTH + terminal_column;
    terminal_buffer[index] = make_vgaentry(c, terminal_color);
    line_buffer[total_lines % MAX_BUFFER_LINES][terminal_column] = make_vgaentry(c, terminal_color);

    if (++terminal_column == VGA_WIDTH) {
        terminal_column = 0;
        
        if (++terminal_row >= VGA_HEIGHT) {
            terminal_row = VGA_HEIGHT - 1;
            total_lines = total_lines < MAX_BUFFER_LINES ? total_lines + 1 : MAX_BUFFER_LINES;
            terminal_scroll_up();  // Auto-scroll when full
        }
    }
}


// Print a string to the terminal
void terminal_writestring(const char* data) {
    for (size_t i = 0; data[i] != '\0'; i++) {
        terminal_putchar(data[i]);
    }
}

// Write an integer as string to the terminal
void terminal_writeint(int num) {
    char buffer[10];
    int i = 0;

    if (num == 0) {
        terminal_putchar('0');
        return;
    }
    while (num > 0) {
        buffer[i++] = (num % 10) + '0';
        num /= 10;
    }
    for (int j = i - 1; j >= 0; j--) {
        terminal_putchar(buffer[j]);
    }
}

// Write a hexadecimal value to the terminal
void terminal_writehex(uint32_t num) {
    const char hex_chars[] = "0123456789ABCDEF";
    terminal_putchar('0');
    terminal_putchar('x');

    for (int i = 7; i >= 0; i--) {
        terminal_putchar(hex_chars[(num >> (i * 4)) & 0xF]);
    }
}

// Print a single character to the terminal
void terminal_writechar(char c) {
    terminal_putchar(c);
}

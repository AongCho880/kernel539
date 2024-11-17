ASM=nasm
CC=gcc
LD=ld

# Flags for assembler, compiler, and linker
ASMFLAGS=-f elf32
CFLAGS=-m32 -ffreestanding -O2 -c -Wall -Wextra -fno-exceptions -fno-stack-protector -nostdlib -nostdinc
LDFLAGS=-m elf_i386 -T linker.ld -nostdlib

# Object files
OBJECTS= boot/boot.o kernel/kernel.o modules/keyboard.o modules/terminal.o
# Default target to build the kernel binary
all: 539kernel.bin

# Link object files into the final binary
539kernel.bin: $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o 539kernel.bin

# Compile C source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly source files into object files
%.o: %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

# Clean up build files
clean:
	rm -f *.o $(OBJECTS) 539kernel.bin

# Declare phony targets
.PHONY: all clean

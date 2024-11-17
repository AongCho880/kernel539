#!/bin/bash

# Setup script for 539kernel development environment
echo "Setting up development environment for 539kernel..."

# Update package lists
sudo apt update

# Install necessary development tools
echo "Installing required packages..."
sudo apt install -y \
    build-essential \
    nasm \
    qemu-system-x86 \
    xorriso \
    grub-pc-bin \
    grub-common \
    gcc-multilib \
    gdb

# Create project directory structure
echo "Creating project directory structure..."
mkdir -p ~/539kernel
cd ~/539kernel

# Create a script to build and run the kernel
cat > build_and_run.sh << 'EOF'
#!/bin/bash

# Clean any previous builds
make clean

# Build the kernel
make

# Check if build was successful
if [ -f "539kernel.bin" ]; then
    echo "Kernel built successfully!"
    echo "Starting QEMU..."
    qemu-system-i386 -kernel 539kernel.bin
else
    echo "Build failed!"
    exit 1
fi
EOF

# Make the script executable
chmod +x build_and_run.sh

# Create a GDB initialization script
cat > .gdbinit << 'EOF'
set architecture i386
target remote localhost:1234
break kernel_main
continue
EOF

# Create debug script
cat > debug.sh << 'EOF'
#!/bin/bash
qemu-system-i386 -kernel 539kernel.bin -s -S &
gdb 539kernel.bin
EOF

chmod +x debug.sh

# Create directory for tests
mkdir -p tests

# Create a basic test script
cat > tests/test_vga.sh << 'EOF'
#!/bin/bash
qemu-system-i386 -kernel 539kernel.bin -nographic -monitor none -serial file:serial.log &
QEMU_PID=$!
sleep 2
kill $QEMU_PID
if grep -q "Welcome to 539kernel" serial.log; then
    echo "VGA Test Passed!"
else
    echo "VGA Test Failed!"
fi
rm serial.log
EOF

chmod +x tests/test_vga.sh

echo "Setup complete! Project structure created at ~/539kernel"
echo "To build and run the kernel:"
echo "1. Copy your kernel source files to ~/539kernel"
echo "2. Run ./build_and_run.sh"
echo ""
echo "To debug the kernel:"
echo "1. Run ./debug.sh"
echo ""
echo "To run tests:"
echo "1. cd tests"
echo "2. ./test_vga.sh"

## **Kernel539 Project Set-Up Instructions**

### Clone the project from GitHub
> `git clone https://github.com/AongCho880/kernel539.git`

### Environment Set-Up
- Goto the project directory and run the following commnad using terminal
> `./setup_539kernel.sh`

### Build the project
> `make`

### Run the kernel
> `qemu-system-i386 -kernel 539kernel.bin`

### Clean the project file
> `make clean`
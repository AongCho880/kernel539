; Multiboot header
section .multiboot
align 4
    dd 0x1BADB002    ; Magic number
    dd 0             ; Flags
    dd -(0x1BADB002) ; Checksum

; Kernel entry point
section .text
global _start
extern kernel_main

_start:
    cli              ; Disable interrupts
    mov esp, stack_top
    call kernel_main
    hlt              ; Halt the CPU

; Stack section
section .bss
align 16
stack_bottom:
    resb 16384       ; 16 KB for stack
stack_top:

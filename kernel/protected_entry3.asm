[bits 32]
[extern kernel_main]

;+0x400 the second part of the bootloader
global _start
_start:

call kernel_main ; Calls the C function. The linker will know where it is placed in memory

; jmp $


idle_loop:
    hlt         ; halt CPU until the next interrupt
    jmp idle_loop



; %include "kernel/interrupt32.asm"
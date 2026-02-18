[BITS 32]

global _start
extern kernel_main

_start:
    cli                 ; 🔴 CRITICAL: disable interrupts immediately
    mov esp, stack_top  ; safe stack

    call kernel_main

.hang:
    cli
    hlt
    jmp .hang

section .bss
    resb 16384
stack_top:

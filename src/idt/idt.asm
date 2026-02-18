section .asm

global idt_load
idt_load:
    push ebp
    mov ebp, esp
    mov ebx, [ebp+8]
    lidt [ebx]
    pop ebp
    ret
    
section .asm

global isr0
extern isr0_handler

isr0:
    cli
    pushad
    call isr0_handler
    popad
    sti
    iret

global irq0
extern irq0_handler

irq0:
    cli
    pushad
    call irq0_handler
    popad
    sti
    iret

global irq1
extern irq1_handler

irq1:
    cli
    pushad
    call irq1_handler
    popad
    sti
    iret

[BITS 32]

extern irq1_handler
global irq1_stub

irq1_stub:
    pusha                ; save all registers
    call irq1_handler    ; MUST match your C function name
    popa                 ; restore registers
    iretd                ; CRITICAL: interrupt return, NOT ret

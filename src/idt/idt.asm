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

global isr14
extern isr14_handler

isr14:
    cli
    pushad
    call isr14_handler
    popad
    add esp, 4      ; pop the CPU-pushed error code — iret does NOT expect it on the stack
    sti
    iret
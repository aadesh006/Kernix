#include <stdint.h>
#include "pic.h"

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void pic_remap()
{
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Start initialization sequence (ICW1)
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2: remap offsets
    outb(PIC1_DATA, 0x20); // Master PIC -> interrupts 32-39
    outb(PIC2_DATA, 0x28); // Slave PIC  -> interrupts 40-47

    // ICW3: setup cascading
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // ICW4: environment info
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Restore saved masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}
#include <stdint.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

extern void outb(uint16_t port, uint8_t val);
extern uint8_t inb(uint16_t port);

void pic_remap()
{
    uint8_t a1 = inb(PIC1_DATA);
    uint8_t a2 = inb(PIC2_DATA);

    // Start initialization sequence
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // Remap offsets
    outb(PIC1_DATA, 0x20); // IRQ0 -> INT 32
    outb(PIC2_DATA, 0x28); // IRQ8 -> INT 40

    // Setup cascading
    outb(PIC1_DATA, 0x04);
    outb(PIC2_DATA, 0x02);

    // Environment info
    outb(PIC1_DATA, 0x01);
    outb(PIC2_DATA, 0x01);

    // Restore masks
    outb(PIC1_DATA, a1);
    outb(PIC2_DATA, a2);
}

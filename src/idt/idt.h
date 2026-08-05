#ifndef IDT_H
#define IDT_H
#include <stdint.h>

struct idt_desc
{
    uint16_t offset_1; //offsets bits 0-15
    uint16_t selector; // selector that in our GDT
    uint8_t zero; //Does nothing, unused set to zero
    uint8_t type_attr; //Decriptor type and attribute
    uint16_t offset_2; //Offset bits 16-31
}__attribute__((packed));

struct idtr_desc
{
    uint16_t limit; //Size of the descriptor table -1
    uint32_t base; // Base add of the start of the interrupt descriptor table

}__attribute__((packed));

void idt_init();
void isr14_handler(void); 

#endif
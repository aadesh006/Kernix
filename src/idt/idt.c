#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"
#include "../io/io.h"
#include "../keyboard/scancode.h"
#include "../keyboard/keyboard.h"



struct idt_desc idt_descriptors[GPOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;

static uint32_t timer_ticks = 0;

void irq0_handler()
{
    timer_ticks++;
    outb(0x20, 0x20);
    //print(".");

}

extern void keyboard_interrupt_handler();

void irq1_handler()
{
    keyboard_interrupt_handler();
}


extern void idt_load(struct idtr_desc* ptr);
void idt_zero()
{
    print("Divide by Zero Error!");
}

void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];

    desc->offset_1 = (uint32_t) address & 0x0000FFFF;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;
    desc->type_attr = 0x8E;      // Present | Ring0 | 32-bit interrupt gate
    desc->offset_2 = (uint32_t) address >> 16;
}


extern void irq1_stub();

void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));

    idtr_descriptor.limit = sizeof(idt_descriptors) - 1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    // Keyboard IRQ = 33 after PIC remap
    idt_set(33, irq1_stub);

    idt_load(&idtr_descriptor);
}

void isr0_handler(){
    panic("Divide by Zero");
}
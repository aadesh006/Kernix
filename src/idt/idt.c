#include "idt.h"
#include "../config.h"
#include "../memory/memory.h"
#include "../kernel.h"
#include "../io/io.h"
#include "../keyboard/scancode.h"
#include "../keyboard/keyboard.h"

struct idt_desc idt_descriptors[GPOS_TOTAL_INTERRUPTS];
struct idtr_desc idtr_descriptor;
uint32_t timer_ticks = 0;

void irq0_handler()
{
    timer_ticks++;
    outb(0x20, 0x20);
    //print(".");

}

static inline uint64_t rdtsc(void) {
    uint32_t lo, hi;
    __asm__ volatile ("rdtsc" : "=a"(lo), "=d"(hi));
    return ((uint64_t)hi << 32) | lo;
}

static uint64_t last_irq1_cycles = 0;

void irq1_handler()
{
    uint64_t entry = rdtsc();
    last_irq1_cycles = entry; // stash it

    uint8_t scancode = inb(0x60);
    if (!(scancode & 0x80))
    {
        char c = scancode_to_ascii[scancode];
        if (c) keyboard_push(c);
    }
    outb(0x20, 0x20);
}

void isr14_handler(void)
{
    uint32_t faulting_address;
    __asm__ volatile ("mov %%cr2, %0" : "=r"(faulting_address));

    print("\n*** PAGE FAULT ***\n");
    print("Faulting address: 0x");
    print_hex(faulting_address);
    print("\n");

    panic("Unhandled page fault\n");
}

extern void idt_load(struct idtr_desc* ptr);
void idt_zero()
{
    print("Divide by Zero Error!");
}

void idt_set(int interrupt_no, void* address)
{
    struct idt_desc* desc = &idt_descriptors[interrupt_no];
    desc->offset_1 = (uint32_t) address & 0x0000ffff;
    desc->selector = KERNEL_CODE_SELECTOR;
    desc->zero = 0;
    desc->type_attr = 0x8E; // present, ring 0, interrupt gate
    desc->offset_2 = (uint32_t) address >> 16;
}


void idt_init()
{
    memset(idt_descriptors, 0, sizeof(idt_descriptors));
    idtr_descriptor.limit = sizeof(idt_descriptors) -1;
    idtr_descriptor.base = (uint32_t) idt_descriptors;

    idt_set(0,idt_zero);
    extern void isr14();
    idt_set(14, isr14);

    //Load the interrupt
    idt_load(&idtr_descriptor);

    extern void irq0();
    idt_set(0x20, irq0);

    extern void irq1();
    idt_set(0x21, irq1);

}

void isr0_handler(){
    panic("Divide by Zero");
}
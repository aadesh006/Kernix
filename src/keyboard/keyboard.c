#include "../idt/idt.h"
#include "../kernel.h"
#include "keyboard.h"
#include "../io/io.h"

static char buffer[KEYBOARD_BUFFER_SIZE];
static uint32_t head = 0;
static uint32_t tail = 0;

void keyboard_init()
{
    head = 0;
    tail = 0;
}

void keyboard_push(char c)
{
    uint32_t next = (head + 1) % KEYBOARD_BUFFER_SIZE;

    if (next == tail)
        return; // buffer full

    buffer[head] = c;
    head = next;
}

bool keyboard_has_char()
{
    return head != tail;
}

char keyboard_pop()
{
    if (head == tail)
        return 0;

    char c = buffer[tail];
    tail = (tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}


static const char scancode_map[] = {
    0,  27, '1','2','3','4','5','6','7','8','9','0','-','=', '\b',
    '\t','q','w','e','r','t','y','u','i','o','p','[',']','\n',
    0,  'a','s','d','f','g','h','j','k','l',';','\'','`',
    0,  '\\','z','x','c','v','b','n','m',',','.','/',
};

void keyboard_interrupt_handler()
{
    uint8_t scancode = inb(0x60);

    // Ignore key release events
    if (scancode & 0x80)
    {
        outb(0x20, 0x20); // EOI
        return;
    }

    char c = scancode_map[scancode];

    if (c)
    {
        keyboard_push(c);
    }

    // Send End Of Interrupt to PIC
    outb(0x20, 0x20);
}
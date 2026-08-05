#ifndef KERNEL_H
#define KERNEL_H
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

void kernel_main();
void print(const char* str);
void print_hex(uint32_t val);
void panic(const char* msg);
struct heap* kernel_get_heap(void);

void terminal_writechar(char c, char colour);
void terminal_backspace();
extern uint32_t _kernel_end;

#endif
#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

#include "idt/idt.h"
#include "io/io.h"
#include "keyboard/keyboard.h"
#include "terminal/line.h"
#include "shell/shell.h"
#include "terminal/terminal.h"
#include "terminal/vga.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;

uint16_t terminal_make_char(char c, char colour)
{
    return (colour << 8) | (uint8_t)c;
}

void terminal_putchar(int x, int y, char c, char colour)
{
    video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c, colour);
}

void terminal_writechar(char c, char colour)
{
    if (c == '\n')
    {
        terminal_row++;
        terminal_col = 0;
        return;
    }

    terminal_putchar(terminal_col, terminal_row, c, colour);
    terminal_col++;

    if (terminal_col >= VGA_WIDTH)
    {
        terminal_col = 0;
        terminal_row++;
    }
}

void terminal_intialize()
{
    video_mem = (uint16_t*)0xB8000;
    terminal_row = 0;
    terminal_col = 0;

    for (int y = 0; y < VGA_HEIGHT; y++)
    {
        for (int x = 0; x < VGA_WIDTH; x++)
        {
            terminal_putchar(x, y, ' ', 0);
        }
    }
}

void terminal_backspace()
{
    if (terminal_col == 0)
        return;

    terminal_col--;
    terminal_putchar(terminal_col, terminal_row, ' ', VGA_LIGHT_GREEN);
}

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

void print(const char* str)
{
    for (size_t i = 0; i < strlen(str); i++)
        terminal_writechar(str[i], VGA_LIGHT_GREEN);
}

void panic(const char* msg)
{
    terminal_intialize();
    print("KERNEL PANIC\n");
    print(msg);

    while (1)
        __asm__ volatile ("hlt");
}

//KERNEL ENTRY POINT

void kernel_main()
{
    terminal_intialize();
    print("Hello World! Welcome to GPOS\n");

    idt_init();

    keyboard_init();
    line_reset();

    print("> ");

    while (1)
    {
        if (keyboard_has_char())
        {
            char c = keyboard_pop();

            if (c == '\b')
            {
                terminal_backspace();
                line_feed(c);
                continue;
            }

            terminal_writechar(c, VGA_LIGHT_GREEN);

            if (line_feed(c))
            {
                terminal_writechar('\n', VGA_LIGHT_GREEN);
                shell_execute(line_get());
                line_reset();
                print("> ");
            }
        }
        else
        {
            __asm__ volatile ("hlt");
        }
    }
}
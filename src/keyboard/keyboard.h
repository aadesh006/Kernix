#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_BUFFER_SIZE 256

void keyboard_init();
void keyboard_push(char c);
bool keyboard_has_char();
char keyboard_pop();
void keyboard_interrupt_handler();

#endif
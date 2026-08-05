#include "shell.h"
#include "../kernel.h"
#include "../memory/memory.h"
#include "../memory/heap/heap.h"
#include "../terminal/terminal.h"

int strncmp(const char* s1, const char* s2, int n);

void shell_execute(const char* command)
{
    if (command[0] == '\0')
        return;

    // echo command
    if (strncmp(command, "echo ", 5) == 0)
    {
        print(command + 5);
        print("\n");
        return;
    }

    // help command
    if (strncmp(command, "help", 4) == 0)
    {
        print("Available commands:\n");
        print("help    - Show commands\n");
        print("clear   - Clear screen\n");
        print("echo    - Print text\n");
        print("version - Kernel info\n");
        print("memtest - Test heap allocator\n");
        print("pagetest - Trigger deliberate page fault\n");
        return;
    }

    // clear command
    if (strncmp(command, "clear", 5) == 0)
    {
        terminal_intialize();
        return;
    }

    if (strncmp(command, "version", 7) == 0)
    {
        print("Kernix Kernel v0.1\n");
        print("32-bit\n");
        return;
    }

    // memtest command — proves the heap allocator is live
    if (strncmp(command, "memtest", 7) == 0)
    {
        struct heap* heap = kernel_get_heap();

        void* a = heap_malloc(heap, 8192);
        print(a ? "alloc a (8192 bytes): ok\n" : "alloc a: FAILED\n");

        void* b = heap_malloc(heap, 4096);
        print(b ? "alloc b (4096 bytes): ok\n" : "alloc b: FAILED\n");

        heap_free(heap, a);
        heap_free(heap, b);
        print("freed both\n");
        return;
    }

    if (strncmp(command, "pagetest", 8) == 0)
    {
        print("Triggering deliberate page fault...\n");
        volatile uint32_t* bad_ptr = (uint32_t*)0x10000000;
        *bad_ptr = 0xDEAD; // should page-fault here
        print("If you see this, the address was mapped — pick a different one\n");
        return;
    }

    print("Unknown command\n");
}
#include "shell.h"
#include "../kernel.h"
#include "../memory/memory.h"
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
        print("help  - Show commands\n");
        print("clear - Clear screen\n");
        print("echo  - Print text\n");
        print("version - Kernel info\n");
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

    print("Unknown command\n");
}

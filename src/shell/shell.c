#include "shell.h"
#include "../kernel.h"
#include "../memory/memory.h"
#include "../terminal/terminal.h"

int strncmp(const char* s1, const char* s2, int n);

void shell_execute(const char* command)
{
    if (command[0] == '\0')
        return;

    if (strncmp(command, "echo ", 5) == 0)
    {
        // Print everything after "echo "
        print(command + 5);
        print("\n");
        return;
    }

    if (strncmp(command, "help", 4) == 0)
    {
        print("Commands:\nhelp\nclear\necho\n");
        return;
    }

    if (strncmp(command, "clear", 5) == 0)
    {
        terminal_intialize();
        return;
    }

    print("Unknown command\n");
}

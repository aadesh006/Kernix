#include "memory.h"

void* memset(void* ptr, int c, size_t size)
{
    char* c_ptr = (char*)ptr;
    for (int i = 0; i < size; i++)
    {
        c_ptr[i] = (char) c;         
    }
    return ptr;
    
}

int strncmp(const char* s1, const char* s2, int n)
{
    for (int i = 0; i < n; i++)
    {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0')
        {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

#include <stdio.h>

unsigned syscall2(unsigned w0, unsigned w1)
{
    asm volatile (
        "   mov %%esp, %%ecx    ;"
        "   mov $1f, %%edx      ;"
        "   sysenter            ;"
        "1:                     ;"
        : "+a" (w0) : "S" (w1) : "ecx", "edx", "memory");
    return w0;
}

unsigned syscall3(unsigned w0, unsigned w1, unsigned w2)
{
    asm volatile (
        "   mov %%esp, %%ecx    ;"
        "   mov $1f, %%edx      ;"
        "   sysenter            ;"
        "1:                     ;"
        : "+a" (w0) : "S" (w1), "D" (w2) : "ecx", "edx", "memory");
    return w0;
}

void write_to_terminal(const void *data, unsigned len)
{
	syscall3(1, (unsigned)data, len);
}

void print(const char *string)
{
    unsigned len = 0;

    while (string[len] != '\0')
        len++;

    write_to_terminal(string, len);
}

void *brk(void *address)
{
    return (void*)syscall2(3, (unsigned)address);
}
#include "syscall.h"
#include <stdint.h>
#include <stdlib/stdio.h>
#include <idt/idt.h>
#include <errno.h>

void init_syscalls()
{
        idt_install(0x80, &isr128);
}

int sys_write(char *str)
{
        if(!str) return -1;
        printf("%s", str);
        return 0; 
}

int invalid_syscall()
{
        printf("\ninvalid syscall!\n");
        return ERR_BAD_SYSCALL;
}

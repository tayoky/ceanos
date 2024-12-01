#include <stdint.h>
#include "kernel.h"
#include <stdlib/stdio.h>
#include <errno.h>

uint16_t die(const char *die_msg, uint16_t return_value)
{
        __printf("%s\n", die_msg);
        return return_value;
}

void panic(const char *panic) 
{
        __printf("kernel panic: %s\n", panic);
        for(;;){
                asm("hlt");
        }
}


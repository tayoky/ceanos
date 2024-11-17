#include <stdint.h>
#include <kernel.h>
#include <stdlib/stdio.h>

uint16_t die(const char *die_msg)
{
        printf("%s\n", die_msg);
}
void panic(const char *panic) 
{
        printf("panic: %s\n", panic);
        for(;;)
}
#ifndef __SYS__
#define __SYS__

#include <stdint.h>

uint16_t die(const char *die_msg, uint16_t return_value);
void panic(const char *panic);

#endif
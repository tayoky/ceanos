#ifndef __SYS__
#define __SYS__

#include <stdint.h>

uint16_t die(const char *die_msg);
void panic(const char *panic);

#endif
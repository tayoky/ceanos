#ifndef __ceanos__
#define __ceanos__

#include <stdint.h>

#define VERSION "0.0.4-rc1"

uint16_t die(const char *die_msg, uint16_t return_value);
void panic(const char *panic);

#endif

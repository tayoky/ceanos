#ifndef IO_H
#define IO_H

#include "stdint.h"  

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint16_t inw(uint16_t port);

#endif  // IO_H

#pragma once

#include <stdint.h>

uint8_t inb(uint16_t port);
void outb(uint16_t port, uint8_t data);
uint16_t inw(uint16_t port);
volatile void outl(uint16_t port, uint32_t data); 


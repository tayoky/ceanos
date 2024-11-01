#pragma once

#include "stdint.h"

#ifndef UTIL_h
#define UTIL_H

#define CEIL_DIV(a,b) (((a + b) - 1)/b)

void memset(void *dest, char val, uint32_t count);
void* memmove(void* dest, const void* src, uint32_t len);
void outPortB(uint16_t Port, uint8_t Value);
char inPortB(uint16_t port);
void insl(uint16_t port, void* addr, int count); 
void outw(unsigned short port, unsigned short value);
void shutdown(uint16_t port, uint16_t value);

struct InterruptRegisters{
    uint32_t cr2;
    uint32_t ds;
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    uint32_t int_no, err_code;
    uint32_t eip, csm, eflags, useresp, ss;
};

#endif //util.h

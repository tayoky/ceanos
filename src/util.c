/* UTILS FOR CEANOS */

#include "stdint.h"
#include "util.h"
#include "timer.h"
#include "stdlib/stdio.h"

void memset(void *dest, char val, uint32_t count){
    char *temp = (char*) dest;
    for (; count != 0; count --){
        *temp++ = val;
    }
}

void outPortB(uint16_t Port, uint8_t Value){
    asm volatile ("outb %1, %0" : : "dN" (Port), "a" (Value));
}

char inPortB(uint16_t port){
    char rv;
    asm volatile("inb %1, %0": "=a"(rv):"dN"(port));
    return rv;
}

void outw(unsigned short port, unsigned short value) {
    asm volatile (
        "outw %0, %1"
        : // no output operands
        : "a"(value), "Nd"(port)
    );
}

void shutdown(uint16_t port, uint16_t value) {
    outw(port, value); 
}

void insl(uint16_t port, void* addr, int count) {
    asm volatile ("cld; rep insl" 
                  : "=D" (addr), "=c" (count) 
                  : "d" (port), "0" (addr), "1" (count) 
                  : "memory");
}


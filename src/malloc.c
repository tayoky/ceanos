/* MALLOC.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */

#include "malloc.h"
#include "stdint.h"

uint32_t mem_start = 0x100000;      //kernel memory address


void mem_copy(uint8_t *src, uint8_t *des, int bytes) {
    int i;
    for (i=0;i<bytes;i++) {
        *(des+i) = *(src+i);
    }

}
void mem_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = (uint8_t *)dest;

    for (; len != 0; len--) *temp++ = val;

}

uint32_t ceanoc(uint32_t size, int align, uint32_t *paddr) {
    if (align == 1 && (mem_start & 0xFFFFF000)) {
        mem_start &= 0xFFFFF000;
        mem_start += 0x100000;
    }

    if (paddr) *paddr = mem_start;

    uint32_t ret = mem_start;
    mem_start+=size;
    return ret;
}


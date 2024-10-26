#include "malloc.h"
#include "stdint.h"

uint32_t mem_start = 0x100000;   
uint32_t mem_end = 0x200000;     

typedef struct MemBlock {
    uint32_t size;
    struct MemBlock *next;
} MemBlock;

MemBlock *free_list = NULL;  

void mem_copy(uint8_t *src, uint8_t *dest, int bytes) {
    for (int i = 0; i < bytes; i++) {
        *(dest + i) = *(src + i);
    }
}

void mem_set(uint8_t *dest, uint8_t val, uint32_t len) {
    uint8_t *temp = dest;
    while (len--) *temp++ = val;
}

uint32_t ceanoc(uint32_t size, int align, uint32_t *paddr) {
    if (align == 1 && (mem_start & 0xFFFFF000)) {
        mem_start = (mem_start & 0xFFFFF000) + 0x1000;
    }

    MemBlock *prev = NULL, *curr = free_list;
    while (curr != NULL) {
        if (curr->size >= size) {
            if (prev) prev->next = curr->next;
            else free_list = curr->next;

            if (paddr) *paddr = (uint32_t)curr;
            return (uint32_t)curr;
        }
        prev = curr;
        curr = curr->next;
    }

    if (paddr) *paddr = mem_start;
    uint32_t ret = mem_start;
    mem_start += size;

    return ret;
}

void ceanoc_free(void *ptr, uint32_t size) {
    if (!ptr) return;

    MemBlock *block = (MemBlock *)ptr;
    block->size = size;
    block->next = free_list;
    free_list = block;
}

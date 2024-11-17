#ifndef __MALLOC_H__
#define __MALLOC_H__

#include <types.h>
#include <stdint.h>

void kmallocInit(uint32_t heapSize);
void changeHeapSize(int newSize);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif

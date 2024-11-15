#ifndef MALLOC_H
#define MALLOC_H

void kmallocInit(uint32_t heapSize);
void changeHeapSize(int newSize);
void* kmalloc(size_t size);
void kfree(void* ptr);

#endif

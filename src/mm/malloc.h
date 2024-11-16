#ifndef MALLOC_H
#define MALLOC_H

void kmallocInit(uint32_t heapSize);
void changeHeapSize(int newSize);
void* kmalloc(size_t size);
void kfree(void* ptr);
void init_heap(void *heap_start, size_t heap_size); 
void memUnmapPage(uint32_t virtualAddr); 

#endif

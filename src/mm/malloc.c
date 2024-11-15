#include <stdint.h>
#include <util.h>
#include "mem.h"
#include "malloc.h"

static uint32_t heapStart;
static uint32_t heapSize;
static uint32_t threshold;
static bool kmallocInitalized = false;

typedef struct memory_block {
    bool free;               
    uint64_t size;          
    struct memory_block *next; 
} memory_block;

static memory_block *free_list = NULL;

void init_heap(void *heap_start, size_t heap_size) {
    free_list = (memory_block *)heap_start;
    free_list->free = true;
    free_list->size = heap_size;
    free_list->next = NULL;
}

void kmallocInit(uint32_t initialHeapSize)
{
    heapStart = KERNEL_MALLOC;   
    heapSize = 0;
    threshold = 0;
    kmallocInitalized = true;

    changeHeapSize(initialHeapSize);
    *((uint32_t*)heapStart) = 0; 
}

void changeHeapSize(int newSize)
{
    int oldPageTop = CEIL_DIV(heapSize, 0x1000); 
    int newPageTop = CEIL_DIV(newSize, 0x1000);

    if (newPageTop > oldPageTop) {
        int diff = newPageTop - oldPageTop;

        for (int i = 0; i < diff; i++) {
            uint32_t phys = pmmAllocPageFrame();  
            memMapPage(KERNEL_MALLOC + oldPageTop * 0x1000 + i * 0x1000, phys, PAGE_FLAG_WRITE); 
        }
    }

    heapSize = newSize; 
}

void* kmalloc(size_t size)
{
    memory_block *current = free_list;
    while (current) {
        if (current->free && current->size >= size) {
            current->free = false;
            
            // Allocate physical pages if necessary
            uint32_t requiredPages = CEIL_DIV(size, 0x1000);
            uint32_t currentAddr = (uint32_t)(current + 1);

            for (uint32_t i = 0; i < requiredPages; i++) {
                uint32_t physAddr = pmmAllocPageFrame();  
                memMapPage(currentAddr + (i * 0x1000), physAddr, PAGE_FLAG_WRITE);
            }

            return (void *)(current + 1); 
        }
        current = current->next;
    }
    return NULL; 
}

void kfree(void* ptr)
{
    if (!ptr) return;
    
    memory_block *block = (memory_block *)ptr - 1;
    block->free = true;

    uint32_t totalPages = CEIL_DIV(block->size, 0x1000);
    uint32_t currentAddr = (uint32_t)(block + 1);

    for (uint32_t i = 0; i < totalPages; i++) {
        uint32_t physAddr = currentAddr + (i * 0x1000);
        memUnmapPage(currentAddr + (i * 0x1000)); 
    }

    memory_block *current = free_list;
    while (current && current->next) {
        if (current->free && current->next->free) {
            current->size += current->next->size;  
            current->next = current->next->next;
        }
        current = current->next;
    }
}

void memUnmapPage(uint32_t virtualAddr)
{
    uint32_t* pageDir = memGetCurrentPageDir();
    uint32_t pdIndex = virtualAddr >> 22;
    uint32_t ptIndex = (virtualAddr >> 12) & 0x3FF;

    uint32_t* pt = REC_PAGETABLE(pdIndex);
    pt[ptIndex] = 0;  
    invalidate(virtualAddr); 
}

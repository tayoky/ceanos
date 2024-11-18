#include <stdint.h>
#include <util.h>
#include "mem.h"
#include "malloc.h"
#include <errno.h>

static uint32_t heapStart;
static uint32_t heapSize;
static uint32_t threshold;
static bool kmallocInitialized = false;

#define TYPE_FREE 56
#define TYPE_ALLOCATED 87

struct kmalloc_header_struct;

typedef struct kmalloc_header_struct {
    size_t length;
    struct kmalloc_header_struct *prev;
    struct kmalloc_header_struct *next;
    char flag;
} kmalloc_header;

kmalloc_header* first_memory_segment;

void kmallocInit(uint32_t initialHeapSize) {
    kmalloc_header first_header;
    first_memory_segment = &first_header;


    heapStart = KERNEL_MALLOC;   

    heapSize = 0;
    threshold = 0;

    changeHeapSize(initialHeapSize);

    first_memory_segment = KERNEL_MALLOC;
    first_memory_segment->next = NULL;
    first_memory_segment->length = heapSize - sizeof(kmalloc_header);
    first_memory_segment->prev = NULL;
    first_memory_segment->flag = TYPE_FREE;

    kmallocInitialized = true;
}

void changeHeapSize(int newSize) {
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
        //search until find and good segment or find last segment
        kmalloc_header *current_segment = first_memory_segment;

        while ((current_segment->flag  == TYPE_ALLOCATED ) || (current_segment->length < size)) {

            //if last segment we need to make kernel space bigger
            //TODO: implement this
            //for the moment lets just say there isn't enought memory
            if(!current_segment->next) {
                die("not enough memory ! todo: make kernel space bigger", ERR_NOT_ENOUGH_MEMORY);
            }
            current_segment = current_segment->next;
        }

        //now we have found a good segement
        //if the segment is big then we cut it
        if(current_segment->length > size + sizeof(kmalloc_header)){
            kmalloc_header *new_segment;
            new_segment = (kmalloc_header *)((uintptr_t)current_segment + size + sizeof(kmalloc_header));

            //set the lenght
            new_segment->length = current_segment->length - (sizeof(kmalloc_header) + size);
            current_segment->length = size;

            //mark the new segment as free
            new_segment->flag = TYPE_FREE;

            //set pointer
            new_segment->prev = current_segment;
            new_segment->next = current_segment->next;
            current_segment->next = new_segment;
            if(new_segment->next) new_segment->next->prev = new_segment;
        }

        //mark as used
        current_segment->flag = TYPE_ALLOCATED;

        //now return the pointer
        return current_segment + sizeof(kmalloc_header);    
}

void kfree(void* ptr){
    if(!ptr)return;
    kmalloc_header *header = (uintptr_t)((uintptr_t) ptr - (uintptr_t)sizeof(kmalloc_header));
    //if not alloacted do nothing
    if(header->flag != TYPE_ALLOCATED)return;
    
    //mark as free
    header->flag = TYPE_FREE;

    //if next free merge
    if(header->next && header->next->flag == TYPE_FREE){
        //merge
        header->length += header->next->length + sizeof(kmalloc_header);
        header->next = header->next->next;
        if(header->next) header->next->prev = header;

    }
    
    if(header->prev && header->prev->flag == TYPE_FREE){
        //merge
        header->prev->length += header->length + sizeof(kmalloc_header);
        if(header->next) header->next->prev = header->prev;
        header->prev->next = header->next;
        
    }
}

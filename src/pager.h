#include "stdint.h"
#include "vga.h"

#ifndef __PAGER__H
#define __PAGER__H

#define PS 4096    
#define PTE 1024   
#define PDS 1024   

typedef struct {
    uint32_t present : 1;
    uint32_t rw : 1;       
    uint32_t user : 1;     
    uint32_t accessed : 1; 
    uint32_t dirty : 1;    
    uint32_t unused : 7;   
    uint32_t frame : 20;   
} __attribute__((packed)) pte;

typedef struct {
    uint32_t present : 1;
    uint32_t rw : 1;       
    uint32_t user : 1;     
    uint32_t accessed : 1; 
    uint32_t unused : 7;   
    uint32_t table_addr : 20; 
} __attribute__((packed)) pde_t;

void init_pager();
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();
    
uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_pager() {
    //set each entry to not present
    int i;
    for(i = 0; i < 1024; i++)
    {
        // This sets the following flags to the pages:
        //   Supervisor: Only kernel-mode can access them
        //   Write Enabled: It can be both read from and written to
        //   Not Present: The page table is not present
        page_directory[i] = 0x00000002;
    }
    // holds the physical address where we want to start mapping these pages to.
    // in this case, we want to map these pages to the very beginning of memory.
    unsigned int j;

    //we will fill all 1024 entries in the table, mapping 4 megabytes
    for(j = 0; j < 1024; j++)
    {
        // As the address is page aligned, it will always leave 12 bits zeroed.
        // Those bits are used by the attributes 
        first_page_table[j] = (j * 0x1000) | 3; // attributes: supervisor level, read/write, present.
    } 

    page_directory[0] = ((unsigned int)first_page_table) | 3;

    loadPageDirectory(page_directory);
    enablePaging();

    print("paging enabled\n");
}

#endif // __PAGER__H

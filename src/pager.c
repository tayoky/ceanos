#include "pager.h"
#include "stdlib/stdio.h"

uint32_t page_directory[1024] __attribute__((aligned(4096)));
uint32_t first_page_table[1024] __attribute__((aligned(4096)));

void init_pager() {
    int i;
        for(i = 0; i < PDS; i++)
        {
            page_directory[i] = 0x00000002;
        }

    unsigned int j;

        for(j = 0; j < PTE; j++)
        {
            first_page_table[j] = (j * 0x1000) | 3; 
        } 

    page_directory[0] = ((unsigned int)first_page_table) | 3;

    loadPageDirectory(page_directory);
    enablePaging();

    printf("paging enabled\n");
}


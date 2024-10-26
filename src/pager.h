#include "stdint.h"
#include "vga.h"

#ifndef __PAGER__H
#define __PAGER__H

#define PS 4096    
#define PTE 1024   
#define PDS 1024   

void init_pager();
extern void loadPageDirectory(unsigned int*);
extern void enablePaging();

#endif // __PAGER__H

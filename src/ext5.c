#include "ext5.h"
#include "stdint.h"
#include "stdlib/stdio.h"

void init_ext5(struct ext5_superblock extsb) {
    extsb.s_inodes_count = 1;

    printf("ext5 filesystem initialized\n");
}

int read_block(uint32_t block_num, uint8_t buffer) {
         
}

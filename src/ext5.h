#include "pager.h"
#include "stdint.h"
#include "stdlib/stdio.h"

#define SUPERBLOCK_OFFSET 1024
#define BLOCK_SIZE 4096

#define READ_SUCCESS 0
#define READ_ERROR -1

struct ext5_superblock {
    uint32_t s_inodes_count;        // total number of inodes
    uint32_t s_blocks_count;        // total number of blocks
    uint32_t s_free_blocks_count;   // number of free blocks
    uint32_t s_free_inodes_count;   // number of free inodes
    uint32_t s_block_size;          // block size (in bytes)
    uint32_t s_inode_size;          // Size of each inode
    uint32_t s_blocks_per_group;    // number of blocks per block group
    uint32_t s_inodes_per_group;    // number of inodes per block group
}__attribute__((packed));

struct ext5_inode {
    uint16_t i_mode;          
    uint32_t i_size;          
    uint32_t i_atime;         
    uint32_t i_mtime;         
    uint32_t i_blocks;        
    uint32_t i_block[15];     
} __attribute__((packed));

struct ext5_block {
    uint32_t block_num;            // block number (index in the filesystem)
    uint8_t data[4096];            // data payload (adjust size according to block size)
} __attribute__((packed));

struct ext5_directory {
    uint32_t dir_num;        // directory number 
    char name[255];          // name of the directory 
    uint8_t name_len;        // length of the name
} __attribute__((packed));

struct ext5_root {
    uint32_t root_inode_num;   
    uint32_t root_block_num;   
} __attribute__((packed));

int read_block(uint32_t block_num, uint8_t buffer); 
void init_ext5(struct ext5_superblock);


#include "fat.h"
#include <stdint.h>
#include <stdlib/stdio.h>
#include <mm/malloc.h>

FAT32_BootSector boot_sector;

void load_boot_sector() {
    read_sectors_ATA_PIO((uint8_t*)&boot_sector, 0, 1);

    if (boot_sector.bytes_per_sector != 512) {
        debugf("[FAT32] Invalid sector size. Expected 512 bytes, found %d bytes.\n", boot_sector.bytes_per_sector);
        return;
    }
    if (boot_sector.fs_type[0] != 'F' || boot_sector.fs_type[1] != 'A' || boot_sector.fs_type[2] != 'T' || boot_sector.fs_type[3] != '3' || boot_sector.fs_type[4] != '2') {
        debugf("[FAT32] Not a valid FAT32 file system.\n");
        return;
    }
    debugf("[FAT32] Boot sector loaded successfully.\n");
}

uint32_t fat_start, data_start;

void init_fat32_params() {
    if (boot_sector.fat_size_32 == 0) {
        debugf("[FAT32] Invalid FAT size (FAT32). Cannot initialize parameters.\n");
        return;
    }

    fat_start = boot_sector.reserved_sectors;
    data_start = fat_start + (boot_sector.num_fats * boot_sector.fat_size_32);

    if (fat_start >= boot_sector.total_sectors_32 || data_start >= boot_sector.total_sectors_32) {
        debugf("[FAT32] Invalid FAT or data region start locations.\n");
        return;
    }

    debugf("[FAT32] FAT parameters initialized. FAT starts at sector %d, data at sector %d.\n", fat_start, data_start);
}

uint32_t get_next_cluster(uint32_t cluster) {
    uint32_t fat_offset = cluster * 4;
    uint32_t fat_sector = fat_start + (fat_offset / boot_sector.bytes_per_sector);
    uint32_t offset_within_sector = fat_offset % boot_sector.bytes_per_sector;

    uint8_t buffer[512]; 
    read_sectors_ATA_PIO(buffer, fat_sector, 1);

    uint32_t next_cluster = *(uint32_t*)&buffer[offset_within_sector] & 0x0FFFFFFF; 
    if (next_cluster >= 0x0FFFFFF8) {
        return 0;
    }

    return next_cluster;
}

void read_cluster(uint8_t *buffer, uint32_t cluster) {
    uint32_t lba = data_start + ((cluster - 2) * boot_sector.sectors_per_cluster);
    read_sectors_ATA_PIO(buffer, lba, boot_sector.sectors_per_cluster); 
}

void init_fat() {
    uint32_t fat_size = boot_sector.fat_size_32;  
    uint32_t fat_table_start = fat_start;

    uint8_t *fat_buffer = kmalloc(fat_size * boot_sector.num_fats);
    if (fat_buffer == NULL) {
        debugf("[FAT32] Memory allocation failed for FAT table.\n");
        return;
    }

    for (int i = 0; i < boot_sector.num_fats; i++) {
        read_sectors_ATA_PIO(fat_buffer + (i * fat_size), fat_table_start + (i * fat_size), fat_size);
    }

    debugf("[FAT32] FAT table initialized successfully.\n");
}

void read_directory(uint32_t cluster) {
    uint8_t buffer[512 * boot_sector.sectors_per_cluster]; 
    read_cluster(buffer, cluster); 

    FAT32_DirEntry *entries = (FAT32_DirEntry*)buffer; 

    for (int i = 0; i < (512 * boot_sector.sectors_per_cluster) / sizeof(FAT32_DirEntry); i++) {
        FAT32_DirEntry *entry = &entries[i];

        if (entry->name[0] == 0x00) break;  
        if (entry->name[0] == 0xE5) continue; 

        printf("Entry: %.11s\n", entry->name);
    }
}

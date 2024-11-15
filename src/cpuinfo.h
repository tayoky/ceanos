#pragma once 

#include <stdint.h>

typedef struct {
    char vendor[13];
    uint32_t model;
    uint32_t family;
    uint32_t stepping;
    uint32_t extended_model;
    uint32_t extended_family;
} CPUinfo;

void get_cpu_info(CPUinfo* cpu_info); 
void print_cpuinfo();

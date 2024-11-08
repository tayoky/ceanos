#include "syscall.h"
#include <stdint.h>
#include <stdlib/stdio.h>
#include <idt/idt.h>

size_t   syscalls[MAX_SYSCALLS] = {0};
uint32_t syscall_count = 0;

void register_syscall(uint32_t id, void *handler)
{
        if (id > MAX_SYSCALLS)
        {
                debugf("[syscalls] SYSCALL NUMBER EXCEDEED LIMIT !!! %d/", MAX_SYSCALLS, "\n");
                asm volatile("hlt");
        }

        if (syscalls[id])
        {
                debugf("[syscalls] found duplicate syscall !");
                asm volatile("hlt");
        }

        syscalls[id] = (size_t)handler;
        syscall_count++;
}

uint64_t syscall_handler(uint32_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4)
{
        switch (syscall_number) {
                case SYSCALL_PRINT:
                        printf((char *)arg1);
                        return 0;
                case SYSCALL_EXIT:
                        asm volatile("hlt"); 
                        return arg1;
                default:
                        debugf("[syscalls] invalid syscall id: %d\n", syscall_id);
                        return -1;
        } 
}

#include "syscall.h"
#include <stdint.h>
#include <stdlib/stdio.h>

size_t   syscalls[MAX_SYSCALLS] = {0};
uint32_t syscall_count = 0;

void register_syscall(uint32_t id, void *handler)
{
        if (id > MAX_SYSCALLS)
        {
                debugf("[syscalls] SYSCALL NUMBER EXCEDEED LIMIT !!! %d/" MAX_SYSCALLS "\n");
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

typedef uint64_t (*syscall_handler)(uint64_t a1, uint64_t a2, uint64_t a3,
                                    uint64_t a4, uint64_t a5, uint64_t a6);

uint64_t syscall_handler()
{


}

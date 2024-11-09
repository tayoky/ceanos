#include <context.h>   
#include <stdint.h>

typedef struct {
    uint32_t eax, ebx, ecx, edx, esi, edi, ebp;   // General-purpose registers
    uint32_t esp, eip;                             // Stack pointer and instruction pointer
    uint32_t cs, ds, es, fs, gs;                   // Segment registers
} context_t;

void switch_context()
{

        asm volatile(
                "pusha\n"        
                :                 
                :                  
                : "%eax", "%ecx", "%edx", "%ebx", "%esp", "%ebp", "%esi", "%edi"  
        );


}

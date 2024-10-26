.section .text
.globl loadPageDirectory
.globl enablePaging

loadPageDirectory:
    push %ebp               
    mov %esp, %ebp          # set up stack frame
    mov 8(%ebp), %eax       # load the argument into eax
    mov %eax, %cr3          
    pop %ebp                
    ret                      

enablePaging:
    push %ebp                
    mov %esp, %ebp          # set up stack frame
    mov %cr0, %eax          # load cr0 into eax
    or $0x80000000, %eax    # set the PG bit
    mov %eax, %cr0          
    pop %ebp                 
    ret                      

/* KERNEL.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */

/* main entry point of the ceanos kernel */

#include "vga.h"
#include "stdint.h"
#include "gdt.h"
#include "idt.h"
#include "timer.h"
#include "util.h"
#include "stdlib/stdio.h"
#include "keyboard.h"
#include "osfunc.h"
#include "io.h"
#include "vga_types.h"
#include "malloc.h"
#include "pager.h"
#include "ext5.h"

void main(void);
char prompt[2] = "$ ";   

// initialize all important stuff, like idt, gdt, etc

static inline void init_all(void) {
    gdt_init();
        init_pager();
    idt_init();
        timer_init();
    keyboard_init();
        struct ext5_superblock sb;
        init_ext5(sb);
    sleep(500);
    Reset();

    // this disables the cursor
    outb(VGA_CRT_IC, 0x0A);              
    outb(VGA_CRT_DC, 0x20);       
}

void main(void){
    init_all();
    
    printf("##welcome to ceanos##\n");            // this part will probably be cleared and replaced with something
         sleep(50);                                              
    printf("current os version: v0.0.3-alpha\n"); // else in the future, for now it will just print a message and
         sleep(50);                                             
    printf("ceanos~%s",prompt);                  // initialize the shell
    
    set_screen_color(0x0F);                      // 0x0F = white on black
    
    // test //
        /*char *ptr = (char *)0x00000000;

        *ptr = 'a';

        printf("%c\n", *ptr); */

    while(1);   
}

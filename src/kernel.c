/* KERNEL.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */

/* main entry point of the ceanos kernel */

#include "vga.h"
#include "stdint.h"
#include "gdt/gdt.h"
#include "idt/idt.h"
#include "timer.h"
#include "util.h"
#include "stdlib/stdio.h"
#include "keyboard.h"
#include "osfunc.h"
#include "io.h"
#include "vga_types.h"
#include "malloc.h"
#include "paging.h"
#include "multiboot.h"
#include "mem.h"

void main(uint32_t magic, struct multiboot_info* boot);
char prompt[2] = "$ ";

// initialize all important stuff, like idt, gdt, etc

static inline void init_all(void)
{
    vga_disable_cursor();
        gdt_init();
    idt_init();
        timer_init();
    keyboard_init();

    sleep(500);
    Reset();
}

void main(uint32_t magic, struct multiboot_info* boot)
{
    init_all();
        uint32_t mod1 = *(uint32_t*)(boot->mods_addr + 4);
        uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;

        initMemory(boot->mem_upper * 1024, physicalAllocStart);
        kmallocInit(0x1000);
    printf("##welcome to ceanos##\n");            // this part will probably be cleared and replaced with something
        sleep(50);
    printf("current os version: v0.0.3-alpha\n"); // else in the future, for now it will just print a message and
        sleep(50);
    printf("ceanos~%s", prompt);                  // initialize the shell

    set_screen_color(0x0F);                       // 0x0F = white on black

    while(1);
}

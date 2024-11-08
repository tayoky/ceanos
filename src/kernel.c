/* KERNEL.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */
        
/* main entry point of the ceanos kernel */

/* SYS */

#include <sys/syscall.h>
#include <sys/process.h>
#include <sys/scheduler.h>
#include <multiboot.h>
#include <timer.h>

/* DESCRIPTOR TABLES */

#include <gdt/gdt.h>
#include <idt/idt.h>

/* DRIVERS  */

#include <drivers/ata.h>
#include <drivers/video/vga/vga.h>
#include <drivers/keyboard/keyboard.h>
#include <drivers/video/vga/vga_types.h>

//#include <drivers/generic/acpi.h> 
//#include <drivers/generic/pci.h>

/* MEMORY */

#include <mm/malloc.h>
#include <mm/paging.h>
#include <mm/mem.h>

/* FILE SYSTEMS*/

#include <fs/fat.h>

/* STDLIB */

#include <stdlib/stdio.h>

/* UTILS */

#include <stdint.h>
#include <util.h>
#include <osfunc.h>
#include <io.h>

// actual code

void main(uint32_t magic, struct multiboot_info* boot);
char prompt[2] = "$ ";
int safe_mode = 0;


void check_boot_params(struct multiboot_info *mbi)
{
	if (mbi->flags & 0x00000002) {
		char *cmdline = (char *) mbi->cmdline;
		if (strstr(cmdline, "safe_mode=1")) {
			safe_mode = 1;
		}
	}
}


// initialize all important stuff, like idt, gdt, etc

static inline void init_all(void)
{
	vga_disable_cursor();
	gdt_init();
	idt_init();
	timer_init();
	keyboard_init();
	dump_registers();
	sleep(500);
	Reset();
}

void enable_default()
{
	init_all();
	printf("##welcome to ceanos##\n");            // this part will probably be cleared and replaced with something
	printf("current os version: v0.0.3-alpha\n"); // else in the future, for now it will just print a message and
	printf("ceanos~%s", prompt);                  // initialize the shell

	set_screen_color(0x0F);
}

void enable_safe()
{
	init_all();

	printf("##welcome to ceanos##\n");
	printf("SAFE MODE\n");
	printf("current os version: v0.0.3-alpha\n");
	printf("safemode~%s", prompt);

	set_screen_color(0x0F);
}

void main(uint32_t magic, struct multiboot_info* boot)
{
	struct multiboot_info *mbi = (struct multiboot_info *) boot;

	check_boot_params(mbi);

	if (safe_mode == 1) {
		printf("safe mode is enabled !\n");
		enable_safe();
	} else {
		enable_default();
	}
    
	//calculate physical memory start for kernel heap
	uint32_t mod1 = *(uint32_t*)(boot->mods_addr + 4);
	uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
	initMemory(boot->mem_upper * 1024, physicalAllocStart);
	kmallocInit(0x1000);

	while(1);
}

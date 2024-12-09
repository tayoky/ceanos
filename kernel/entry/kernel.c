//@file kernel.c
//@brief entry point of ceanos kernel
//@author ceanvalds

/* SYSTEM */

#include <sys/call/syscall.h>
#include <sys/proc/process.h>
#include <sys/proc/scheduler.h>
#include <multiboot.h>
#include <timer.h>
#include "kernel.h" 

/* DESCRIPTOR TABLES */

#include <gdt.h>
#include <idt.h>

/* DRIVERS  */

#include <drivers/ata.h>
#include <drivers/video/vga/vga.h>
#include <drivers/kb/kbps2.h>
#include <drivers/video/vga/vga_types.h>

#include <drivers/devices/serial.h>
#include <drivers/devices/dev.h>

//#include <drivers/generic/acpi.h> 
//#include <drivers/generic/pci.h>

/* MEMORY */

#include <mm/malloc.h>
#include <mm/paging.h>
#include <mm/mem.h>

/* FILE SYSTEMS */

#include <fs/fat.h>
#include <fs/vfs.h>
#include <fs/tmpfs.h>

/* STDLIB */

#include <stdlib/stdio.h>
#include <stdlib/string.h>

/* UTILS */

#include <stdint.h>
#include <util.h>
#include <io.h>

/* OPTIONS */
#include "config.h"

// actual code

void main(uint32_t magic, struct multiboot_info* boot);
char prompt[2] = "% ";
int safe_mode = 0;

bool debug_mode;

void check_boot_params(struct multiboot_info *boot)
{
	struct multiboot_info *mbi = (struct multiboot_info *) boot;

	if (mbi->flags & 0x00000002) {
		char *cmdline = (char *) mbi->cmdline;
		if (strstr(cmdline, "safe_mode=1")) {
			debugf("[boot] safe_mode=1\n");
			safe_mode = 1;
		} else {
			debugf("[boot] safe_mode=0\n");
		}
	}
}

#define __ceanos__

// Initialize all the important stuff, like idt, gdt, etc

static void init_mm(struct multiboot_info* boot)
{
	//calculate physical memory start for kernel heap
	uint32_t mod1 = *(uint32_t*)(boot->mods_addr + 4);
	uint32_t physicalAllocStart = (mod1 + 0xFFF) & ~0xFFF;
	initMemory(boot->mem_upper * 1024, physicalAllocStart);
	kmallocInit(0x4000);
    	debugf("[mm] memory done!\n");
}

static void init_tables()
{
	gdt_init();
	idt_init();
}

static void init_all(struct multiboot_info* boot)
{
	#ifdef DEBUG
	debug_mode = true;
	#endif

	vga_disable_cursor();
        
        init_tables();

	check_boot_params(boot);

	timer_init();
	keyboard_init();
	init_mm(boot);

	vfs_init();
	init_tmpfs();

        /* init_devices(); */

	/* vfs_node *open_folder = kopen("/");
	vfs_mkdir(open_folder, "test", 0777);
	vfs_close(open_folder); */

	printf("[ceanos] everything done ! booting shortly...\n");
	
	sleep(300);
	Reset();
}

void enable_default(struct multiboot_info* boot)
{
	init_all(boot);

	printf("##welcome to ceanos##\n");            // This part will probably be cleared and replaced with something
	printf("current os version: %s\n", VERSION);  // else in the future, like loading a shell executable, but for now
	printf("ceanos%s", prompt);		      // it will just print a message and initialize the "shell"

	set_screen_color(0x0F);
}

void enable_safe(struct multiboot_info* boot)
{
	init_all(boot);

	printf("##welcome to ceanos##\n");
	printf("SAFE MODE\n");
	printf("current os version: %s\n", VERSION);
	printf("safemode%s", prompt);

	set_screen_color(0x0F);
}

void main(uint32_t magic, struct multiboot_info* boot)
{
	if (safe_mode == 1) {
		printf("safe mode is enabled !\n");
		enable_safe(boot);
	} else {
		enable_default(boot);
	}
                
	while(1) {
		// TODO: Add a way to check for stack overflows and other errors that the ISR can't handle
	};
}

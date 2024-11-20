/* SHELL.C */
/* Copyright (c) @asdasda3456 2024 - 2024 */

#include <drivers/video/vga/vga.h>
#include <stdint.h>
#include <osfunc.h>
#include <cpuinfo.h>
#include <util.h>
#include <strings.h>
#include <drivers/keyboard/keyboard.h>
#include <stdlib/stdio.h>
#include <fs/vfs.h>
#include <fs/fat.h>
#include <kernel.h>
#include <fs/tmpfs.h>
#include <mm/malloc.h>

static inline void process_cmd(const char *tex)
{
	if (strcmp("clear", tex) != 0 || strcmp("cls", tex) != 0) 
        {
		Reset();
	}
        else if (strcmp("help", tex) != 0) 
        {
		_term_help();
	}
        else if (strcmp("version", tex) != 0 || strcmp("ver", tex) != 0) 
        {
		printf("ceanos %s", VERSION);
	}
        else if (strcmp("fetch", tex) != 0) 
        {
		_get_sysinfo();
	}
        else if (strcmp("exit", tex) != 0) 
        {
		printf("halting...");
		sleep(1);
		halt();
	}
        else if (strcmp("shutdown", tex) != 0 ) 
        {
		printf("shutting down...\n");
		printf("not actually doing that because ACPI does not exist yet\n");
	}
        else if (strcmp("compdate", tex) != 0 ) 
        {
		printf("date: " __DATE__ "\nat: " __TIME__);
	}
	else if (strcmp("memseg", tex) != 0) {
		debug_mem_graph();
	}
	else if(strcmp("ls", tex) != 0) {
		vfs_node *open_folder = kopen("/");
		
		if (open_folder == NULL) {
			printf("Error: Could not open the root folder\n");
			return;
		}

		uint32_t index = 0;
		struct dirrent *ret;
		do
		{
			ret = vfs_readdir(open_folder,index);
			if(ret != NULL)printf("%s\n", ret->name);
			kfree(ret);
			index++;
		} while (ret != NULL);
		
		
		vfs_close(open_folder);
	}
        else {
		printf("%s isn't a valid command", tex);
	}
}

void run_term(const char *tex)
{
	if (tex != NULL) {
		int has_non_space = 0;

		for (int i = 0; tex[i] != '\0'; i++) {
			if (tex[i] != ' ' && tex[i] != '\t' && tex[i] != '\n' && tex[i] != '\r') {
				has_non_space = 1;
				break;
			}
		}

		if (has_non_space) {
			process_cmd(tex);
		} else {
			//nothing
		}
	}
}

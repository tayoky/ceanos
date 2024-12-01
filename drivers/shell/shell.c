#include <drivers/video/vga/vga.h>
#include <stdint.h>
#include <cpuinfo.h>
#include <util.h>
#include <drivers/kb/kbps2.h>
#include <lib/stdlib/stdio.h>
#include <fs/vfs.h>
#include <fs/fat.h>
#include <kernel/kernel.h>
#include <fs/tmpfs.h>
#include <mm/malloc.h>
#include <timer.h>

static inline void process_cmd(const char *tex)
{
	if (__strcmp("clear", tex) == 0 || __strcmp("cls", tex) == 0 || __strcmp("clear;", tex) == 0)  
        {
		Reset();
	}
        else if (__strcmp("help", tex) == 0) 
        {
		_term_help();
	}
        else if (__strcmp("version", tex) == 0 || __strcmp("ver", tex) == 0) 
        {
		__printf("ceanos %s", VERSION);
	}
        else if (__strcmp("fetch", tex) == 0) 
        {
		_get_sysinfo();
	}
        else if (__strcmp("exit", tex) == 0) 
        {
		__printf("halting...");
		sleep(1);
		asm("hlt");
	}
        else if (__strcmp("shutdown", tex) == 0 ) 
        {
		__printf("shutting down...\n");
		__printf("not actually doing that because ACPI does not exist yet\n");
	}
        else if (__strcmp("compdate", tex) == 0 ) 
        {
		__printf("date: " __DATE__ "\nat: " __TIME__);
	}
	else if (__strcmp("memseg", tex) == 0) {
		debug_mem_graph();
	}
	else if(__strcmp("ls", tex) == 0 || __strcmp("Dir;", tex) == 0 || __strcmp("dir", tex) == 0) {
		vfs_node *open_folder = kopen("/");

		if (open_folder == NULL) {
			__printf("error: could not open the root (/) folder\n");
			return;
		}

		uint32_t index = 0;
		struct dirrent *ret;
		do
		{
			ret = vfs_readdir(open_folder,index);
			if(ret != NULL)__printf("%s\n", ret->name);
			kfree(ret);
			index++;
		} while (ret != NULL);

		vfs_close(open_folder);
	}
        else {
		__printf("%s isn't a valid command", tex);
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


void _term_help()
{
	__printf("commands:\n");

	__printf("====================\n");
	__printf("=     GENERIC      =\n");
	__printf("====================\n");

	__printf("  clear     - clears the screen\n");
        __printf("  cls       - same as 'clear'\n");
	__printf("  help      - prints this message\n");
	__printf("  version   - displays os version\n");
	__printf("  exit      - exits the operating system\n");
	__printf("  shutdown  - shuts down the system.\n");
	__printf("  compdate  - displays the compilation date.\n");
	__printf("  fetch     - display system information\n");

	__printf("====================\n");
	__printf("=     DEBUG        =\n");
	__printf("====================\n");

	__printf("  memseg    - display mm debug info");
}

inline void _get_sysinfo()
{
	__printf("\nOS: CeanOS\n");
	__printf("Kernel version: 0x0000004 \n");
	__printf("shell: bosh (bobo shell)\n");
	__printf("os bosh version: v0.1 \n");

	__printf("build date: ");
	__printf(__DATE__);
	__printf("\n");

	print_cpuinfo();
}


#include <osfunc.h>
#include <stdint.h>
#include <drivers/keyboard/keyboard.h>
#include <cpuinfo.h>
#include <util.h>
#include <stdlib/stdio.h>

extern char press;
extern char text[100];

void _term_help()
{
	printf("commands:\n");

	printf("====================\n");
	printf("=     GENERIC      =\n");
	printf("====================\n");

	printf("  clear     - clears the screen\n");
        printf("  cls       - same as 'clear'\n");
	printf("  help      - prints this message\n");
	printf("  version   - displays os version\n");
	printf("  exit      - exits the operating system\n");
	printf("  shutdown  - shuts down the system.\n");

	printf("====================\n");
	printf("=     DEBUG        =\n");
	printf("====================\n");

	printf("  compdate  - displays the compilation date.\n");
	printf("  fetch     - display system information\n");
	printf("  dumpreg   - dump current state of the registers\n");
	printf("  testpanic - invalid opcode test panic ");
}

inline void _get_sysinfo()
{
	printf("\nOS: CeanOS\n");
	printf("Kernel version: 0xFDF4E3 \n");
	printf("shell: bosh (bobo shell)\n");
	printf("os bosh version: v0.1 \n");

	printf("build date: ");
	printf(__DATE__);
	printf("\n");

	print_cpuinfo();
}

void halt()
{
	asm("hlt");
}

void itoa(int value, char* str, int base)
{
	char* ptr = str;
	char* ptr1 = str;
	char tmp_char;
	int tmp_value;

	if (value < 0 && base == 10) {
		*ptr++ = '-';
		value = -value;
	}

	do {
		tmp_value = value;
		value /= base;
		*ptr++ = "0123456789ABCDEF"[tmp_value - value * base];
	} while (value);

	*ptr-- = '\0';

	while (ptr1 < ptr) {
		tmp_char = *ptr;
		*ptr-- = *ptr1;
		*ptr1++ = tmp_char;
	}
}


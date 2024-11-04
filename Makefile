CFLAGS = -m32 -ffreestanding -w -g -I src/ -Wl,-z,noexecstack  
LDFLAGS = -m elf_i386 

all:
	#### Assembly ####
	# boot #
	nasm -f elf32 src/boot.s -o build/boot.o
	# other assembly #
	nasm -f elf32 src/gdt/gdt.s -o build/gdts.o
	nasm -f elf32 src/idt/idt.s -o build/idt.o
	### Kernel #####
	gcc $(CFLAGS) -c src/kernel.c -o build/kernel.o
	gcc $(CFLAGS) -c src/stdlib/stdio.c -o build/stdio.o
	gcc $(CFLAGS) -c src/vga.c -o build/vga.o
	gcc $(CFLAGS) -c src/gdt/gdt.c -o build/gdt.o
	gcc $(CFLAGS) -c src/timer.c -o build/timer.o
	gcc $(CFLAGS) -c src/util.c -o build/util.o
	gcc $(CFLAGS) -c src/idt/idt.c -o build/idts.o
	gcc $(CFLAGS) -c src/keyboard.c -o build/keyboard.o
	gcc $(CFLAGS) -c src/cpuinfo.c -o build/cpuinfo.o
	gcc $(CFLAGS) -c src/osfunc.c -o build/osfunc.o
	gcc $(CFLAGS) -c src/shell.c -o build/shell.o
	gcc $(CFLAGS) -c src/strings.c -o build/strings.o
	gcc $(CFLAGS) -c src/io.c -o build/io.o
	gcc $(CFLAGS) -c src/malloc.c -o build/malloc.o
	gcc $(CFLAGS) -c src/mem.c -o build/mem.o
	### else #####
	ld -m elf_i386 -T linker.ld -o kernel build/boot.o build/kernel.o build/vga.o build/gdts.o build/gdt.o build/idts.o build/idt.o build/util.o build/timer.o build/stdio.o build/keyboard.o build/cpuinfo.o build/strings.o build/osfunc.o build/shell.o build/io.o build/malloc.o build/mem.o 
	mv kernel ceanos/boot/kernel
	grub-mkrescue -o build/ceanos.iso ceanos/
	qemu-system-x86_64 -drive format=raw,file=build/ceanos.iso 
debug:
	qemu-system-x86_64 build/ceanos.iso -d int,cpu_reset -no-reboot -no-shutdown
debug_no_dump:
	qemu-system-x86_64 build/ceanos.iso -no-reboot -no-shutdown -monitor stdio
log:
	qemu-system-x86_64 build/ceanos.iso -d int -D qemu_log.txt 

CFLAGS = -m32 -ffreestanding -fno-stack-protector -w -g -I src/
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
	gcc $(CFLAGS) -c src/drivers/video/vga/vga.c -o build/vga.o
	gcc $(CFLAGS) -c src/gdt/gdt.c -o build/gdt.o
	gcc $(CFLAGS) -c src/timer.c -o build/timer.o
	gcc $(CFLAGS) -c src/util.c -o build/util.o
	gcc $(CFLAGS) -c src/idt/idt.c -o build/idts.o
	gcc $(CFLAGS) -c src/drivers/keyboard/keyboard.c -o build/keyboard.o
	gcc $(CFLAGS) -c src/cpuinfo.c -o build/cpuinfo.o
	gcc $(CFLAGS) -c src/osfunc.c -o build/osfunc.o
	gcc $(CFLAGS) -c src/shell.c -o build/shell.o
	gcc $(CFLAGS) -c src/strings.c -o build/strings.o
	gcc $(CFLAGS) -c src/io.c -o build/io.o
	gcc $(CFLAGS) -c src/mm/malloc.c -o build/malloc.o
	gcc $(CFLAGS) -c src/mm/mem.c -o build/mem.o
	gcc $(CFLAGS) -c src/drivers/storage/ahci.c -o build/ahci.o
	gcc $(CFLAGS) -c src/drivers/ata.c -o build/ata.o
	gcc $(CFLAGS) -c src/sys/syscall.c -o build/syscall.o
	#gcc $(CFLAGS) -c src/drivers/generic/acpi.c -o build/acpi.o
	### else #####
	ld -m elf_i386 -T linker.ld -o kernel build/boot.o build/kernel.o build/vga.o build/gdts.o build/gdt.o build/idts.o build/idt.o build/util.o build/timer.o build/stdio.o build/keyboard.o build/cpuinfo.o build/strings.o build/osfunc.o build/shell.o build/io.o build/malloc.o build/mem.o build/ahci.o build/ata.o build/syscall.o # build/acpi.o
	mv kernel ceanos/boot/kernel
	dd if=/dev/zero of=ceanos.iso bs=1M count=100
	mkfs.fat -F32 ceanos.iso
	sudo losetup /dev/loop0 ceanos.iso
	mkdir /mnt/disk
	sudo mount /dev/loop0 /mnt/disk
	sudo grub-install --force --target=i386-pc --boot-directory=/mnt/disk/boot/ /dev/loop0
	cp -r ceanos/boot/kernel /mnt/disk/boot/
	sudo cp ceanos/boot/grub/grub.cfg /mnt/disk/boot/grub
	sudo umount /mnt/disk
	sudo losetup -d /dev/loop0
	rm -rf /mnt/disk	
	qemu-system-x86_64 -drive format=raw,file=build/ceanos.iso 
debug:
	qemu-system-x86_64 build/ceanos.iso -d int,cpu_reset -no-reboot -no-shutdown
debug_no_dump:
	qemu-system-x86_64 build/ceanos.iso -no-reboot -no-shutdown -monitor stdio
log:
	qemu-system-x86_64 build/ceanos.iso -d int -D qemu_log.txt 

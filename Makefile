CFLAGS = -m32 -ffreestanding -fno-stack-protector -nostdlib -w -g -I src/
CC = gcc
LDFLAGS = -m elf_i386 
NASM = nasm
NASMFLAGS = -f elf32
LOOPDEV = loop4

#get all c files
C_SRC = $(shell find . -type f -name "*.c")

#get all assmebly files
ASM_SRC = $(shell find . -type f -name "*.s")

#.o files 
OBJ = ${ASM_SRC:.s=.o} ${C_SRC:.c=.o}

all: ceanos.iso
	qemu-system-x86_64 -drive format=raw,file=ceanos.iso
ceanos.iso : ceanos/boot/kernel/vmcean
	### else #####
	dd if=/dev/zero of=ceanos.iso bs=1M count=100
	mkfs.fat -F32 ceanos.iso
	sudo losetup /dev/${LOOPDEV} ceanos.iso
	sudo mkdir /mnt/disk
	sudo mount /dev/${LOOPDEV} /mnt/disk
	sudo grub-install --force --target=i386-pc --boot-directory=/mnt/disk/boot/ /dev/${LOOPDEV}
	sudo cp -r ceanos/boot/kernel /mnt/disk/boot/
	sudo cp ceanos/boot/grub/grub.cfg /mnt/disk/boot/grub
	sudo mkdir /mnt/disk/usr
	sudo mkdir /mnt/disk/bin
	sudo cp -r ceanos/test.txt /mnt/disk/usr
	sudo cp -r ceanos/test.txt /mnt/disk/
	sudo umount /mnt/disk
	sudo losetup -d /dev/${LOOPDEV}
	sudo rm -rf /mnt/disk
ceanos/boot/kernel/vmcean : ${OBJ}
	ld -m elf_i386 -o vmcean -T linker.ld ${OBJ}
	mv vmcean ceanos/boot/kernel
debug:
	qemu-system-x86_64 build/ceanos.iso -d int,cpu_reset -no-reboot -no-shutdown
debug_no_dump:
	qemu-system-x86_64 build/ceanos.iso -no-reboot -no-shutdown -monitor stdio
log:
	qemu-system-x86_64 build/ceanos.iso -d int -D qemu_log.txt 
clean:
	rm -f **/*.o **/**/*.o
#sudo losetup -d /dev/${LOOPDEV}
#	sudo umount /mnt/disk
	sudo rm -rf /mnt/disk
	sudo losetup -d /dev/${LOOPDEV}
#for C files
%.o : %.c
	${CC} ${CFLAGS} -o $@ -c $^
#for ASM files
%.o : %.s
	${NASM} $< ${NASMFLAGS} -o $@
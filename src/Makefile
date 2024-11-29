CFLAGS = -ffreestanding -fno-builtin -fno-stack-protector -c -g
IFLAG = -I ../ -I ./interrupts -I ./
LDFLAGS = -T linker.ld

kernel:
#create object files, gcc and nasm

	i386-elf-gcc $(CFLAGS) $(IFLAG) kernel.c 			-o kernel.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) util.c				-o util.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) paging.c 			-o paging.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) ./interrupts/idt.c 	-o idt.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) keyboard.c 			-o keyboard.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) gdt.c 				-o gdt.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) process.c 			-o process.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) timer.c 			-o timer.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) memory.c 			-o memory.o
	i386-elf-gcc $(CFLAGS) $(IFLAG) disk-driver.c 		-o disk-driver.o


#i386-elf-gcc $(CFLAGS) $(IFLAG) testkernel.c -o testkernel.o

	nasm -f elf32 boot.asm 					-o boot.o
	nasm -f elf32 print.asm 				-o print.o
	nasm -f elf32 ./interrupts/idt.asm  	-o idtasm.o
	nasm -f elf32 paging.asm				-o pagingasm.o
	nasm -f elf32 gdt.asm					-o gdtasm.o
	nasm -f elf32 mutex.asm					-o mutex.o


#link all object files into elf 32-bit executable format
	i386-elf-ld $(LDFLAGS) -o kernel boot.o kernel.o util.o print.o idt.o idtasm.o keyboard.o gdt.o gdtasm.o process.o timer.o memory.o disk-driver.o mutex.o

##paging.o pagingasm.o, inte inkluderade i linking

	mv kernel ../skibidi/boot/kernel
#make disk image
	grub-mkrescue -o ../skibidi.iso ../skibidi/

clean:
	rm -rf *.o

emu:
#emulate
	qemu-system-i386 -vga cirrus -drive file=../skibidi.iso,format=raw,if=ide

emu-debug:
	qemu-system-i386 ../skibidi.iso -vga std -s -S
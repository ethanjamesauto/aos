CC = g++
LDFLAGS = -Ttext 7e00 -m i386pe
ARGS = -fomit-frame-pointer -fno-pie -m32 -ffreestanding -c
TOOLCHAIN-PREFIX = 

#for macOS
#LDFLAGS = -Ttext 7e00
#TOOLCHAIN-PREFIX = i386-elf-

all: os-image.img

run: all
	qemu-system-x86_64 -drive format=raw,file=os-image.img

clean:
	rm -f *.dump *.o *.bin *.tmp *.img
#	del *.dump *.o *.bin *.tmp *.img

os-image.img: boot_sect.bin entry_point.o kernel.o memory.o memory_manager.o writer.o key_manager.o parse_command.o
	$(TOOLCHAIN-PREFIX)ld $(LDFLAGS) -o os_built.o entry_point.o kernel.o memory.o memory_manager.o writer.o key_manager.o parse_command.o
#	ld -T NUL -m i386pe -o os_built.o -Ttext 7e00 entry_point.o kernel.o memory.o memory_manager.o writer.o key_manager.o parse_command.o
	$(TOOLCHAIN-PREFIX)objdump -M intel -d os_built.o > os_built.dump
	$(TOOLCHAIN-PREFIX)objcopy -O binary os_built.o kernel.bin
#	copy /b boot_sect.bin+kernel.bin+padding os-image.img
	cat boot_sect.bin kernel.bin > os-image.img
	dd if=/dev/null of=os-image.img bs=1 count=0 seek=1474560

boot_sect.bin: bootloader.asm macros.asm
	nasm -o boot_sect.bin bootloader.asm

entry_point.o: entry_point.asm memory_config.asm
	nasm entry_point.asm -f elf32 -o entry_point.o
#	objdump -M intel -D entry_point.o > entry_point.dump

memory.o: memory.asm memory_config.asm
	nasm memory.asm -f elf32 -o memory.o
#	objdump -M intel -D memory.o > memory.dump

memory_manager.o: memory_manager.cpp memory_manager.h
	$(TOOLCHAIN-PREFIX)$(CC) $(ARGS) memory_manager.cpp -o memory_manager.o
#	objdump -M intel -D memory_manager.o > memory_manager.dump

kernel.o: kernel.cpp kernel.h writer.h ioport.h memory_manager.h parse_command.h
	$(TOOLCHAIN-PREFIX)$(CC) $(ARGS) kernel.cpp -o kernel.o
#	objdump -M intel -D kernel.o > kernel.dump

writer.o: writer.cpp writer.h ioport.h
	$(TOOLCHAIN-PREFIX)$(CC) $(ARGS) writer.cpp -o writer.o
#	objdump -M intel -D writer.o > writer.dump
	
key_manager.o: key_manager.cpp key_manager.h ioport.h
	$(TOOLCHAIN-PREFIX)$(CC) $(ARGS) key_manager.cpp -o key_manager.o
#	objdump -M intel -D key_manager.o > key_manager.dump

parse_command.o: parse_command.cpp parse_command.h writer.h
	$(TOOLCHAIN-PREFIX)$(CC) $(ARGS) parse_command.cpp -o parse_command.o
#	objdump -M intel -D parse_command.o > parse_command.dump

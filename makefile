CXX := g++
ASM := nasm
LD := ld

CXXFLAGS := -fomit-frame-pointer -fno-pie -m32 -ffreestanding -c
ASMFLAGS := -f elf32
LDFLAGS := -T os.ld -m i386pe
DEPFLAGS = -MT $@ -MD -MP -MF $(DEPDIR)/$*.Td

TOOLCHAIN-PREFIX := 

#for macOS
#LDFLAGS := -T os.ld
#TOOLCHAIN-PREFIX := i386-elf-

BIN:= os-image.img

SRCS := \
	src/entry_point.asm src/memory.asm \
	src/kernel.cpp src/key_manager.cpp src/memory_manager.cpp src/writer.cpp src/parse_command.cpp

BUILDDIR := build
OBJDIR := $(BUILDDIR)/objects
DEPDIR := $(BUILDDIR)/dependencies

OBJS := $(patsubst %, $(OBJDIR)/%.o,$(basename $(SRCS)))
DEPS := $(patsubst %, $(DEPDIR)/%.d,$(basename $(SRCS)))

$(shell mkdir -p $(dir $(OBJS)) >/dev/null)
$(shell mkdir -p $(dir $(DEPS)) >/dev/null)

COMPILE.asm = $(ASM) $(ASMFLAGS) -o $@
COMPILE.cc = $(TOOLCHAIN-PREFIX)$(CXX) $(DEPFLAGS) $(CXXFLAGS) -c -o $@

PRECOMPILE =
POSTCOMPILE =

all: $(BIN)

run: all
	qemu-system-x86_64 -drive format=raw,file=$(BIN)

clean:
	rm -f *.dump.asm *.img
	rm -r $(BUILDDIR)

$(BIN): $(BUILDDIR)/boot_sect.bin $(OBJS)
	$(TOOLCHAIN-PREFIX)$(LD) $(LDFLAGS) -o $(BUILDDIR)/os-built.elf $(OBJS)
	$(TOOLCHAIN-PREFIX)objdump -M intel -d $(BUILDDIR)/os-built.elf > os-built.dump.asm
	$(TOOLCHAIN-PREFIX)objcopy -O binary $(BUILDDIR)/os-built.elf $(BUILDDIR)/kernel.bin
	cat $(BUILDDIR)/boot_sect.bin $(BUILDDIR)/kernel.bin > $(BIN)
	dd if=/dev/null of=$(BIN) bs=1 count=0 seek=1474560

$(BUILDDIR)/boot_sect.bin: src/bootloader.asm macros.asm
	nasm -o $(BUILDDIR)/boot_sect.bin src/bootloader.asm 

$(OBJDIR)/%.o: %.cpp
$(OBJDIR)/%.o: %.cpp $(DEPDIR)/%.d
	$(PRECOMPILE)
	$(COMPILE.cc) $<
	$(POSTCOMPILE)

$(OBJDIR)/%.o: %.asm
	$(COMPILE.asm) $<

.PRECIOUS: $(DEPDIR)/%.d
$(DEPDIR)/%.d: ;

-include $(DEPS)

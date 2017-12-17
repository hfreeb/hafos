SYSROOT = ../sysroot

include ../make.config

KERNEL := $(SYSROOT)$(BOOT_DIR)/epos.kernel
ARCH_DIR := arch/$(ARCH)

SRC_FILES := $(shell find -type f -name "*.c")
HDR_FILES := $(shell find -type f -name "*.h")
ASM_FILES := $(shell find -type f -name "*.S")
DEBUG_FILES := $(shell find -type f -name "*.d")

KERNEL_OBJS := $(patsubst %.c, %.o, $(SRC_FILES))
KERNEL_OBJS += $(patsubst %.S, %.o, $(ASM_FILES))

LIBS := -nostdlib -lk -lgcc

.PHONY: all clean iso run

all: $(KERNEL)

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp --parents $(HDR_FILES) $(SYSROOT)$(INCLUDE_DIR)/

install-kernel: $(KERNEL)
	mkdir -p $(SYSROOT)$(BOOT_DIR)/
	cp $(KERNEL) $(SYSROOT)$(BOOT_DIR)

clean: 
	rm -f $(KERNEL) $(ISO) $(KERNEL_OBJS) $(DEBUG_FILES)

$(KERNEL): install-headers $(KERNEL_OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)/linker.ld -o $@ $(CFLAGS) $(KERNEL_OBJS) $(LIBS)
	grub-file --is-x86-multiboot $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL) $(SYSROOT)$(BOOT_DIR)/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(SYSROOT)

run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

cleanrun: run clean

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

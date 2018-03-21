SYSROOT = sysroot

TARGET := i686-elf
ARCH := i686

INCLUDE_DIR := /usr/include
BOOT_DIR := /boot
LIB_DIR := /usr/lib

ISO := hafos-$(ARCH).iso

CROSS_DIR ?= ~/opt/cross/bin
CC := $(CROSS_DIR)/$(TARGET)-gcc --sysroot=$(SYSROOT) -isystem=$(INCLUDE_DIR)
AR := $(CROSS_DIR)/$(TARGET)-ar

WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-declarations -Wredundant-decls \
            -Wnested-externs -Winline -Wno-long-long -Wstrict-prototypes

CFLAGS += -std=gnu11 -ffreestanding -fno-builtin $(WARNINGS)
CPPFLAGS += -Iinclude

LIBK := $(SYSROOT)$(LIB_DIR)/libk.a
KERNEL_DIR := kernel/
KERNEL := $(SYSROOT)$(BOOT_DIR)/hafos.kernel
ARCH_DIR := $(KERNEL_DIR)/arch/$(ARCH)

KERNEL_SRC_DIRS := $(KERNEL_DIR)/core $(KERNEL_DIR)/drivers $(ARCH_DIR)
KERNEL_SRC_FILES := $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.c")
KERNEL_ASM_FILES := $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.S")
KERNEL_DEBUG_FILES := $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.d")

KERNEL_INCLUDE := $(KERNEL_DIR)/include

KERNEL_OBJS := $(patsubst %.c, %.o, $(KERNEL_SRC_FILES))
KERNEL_OBJS += $(patsubst %.S, %.o, $(KERNEL_ASM_FILES))

LIBS := -nostdlib -lk -lgcc

.PHONY: all clean iso run

all: kernel-install

kernel-install-headers:

kernel-headers:
	rm -r $(SYSROOT)$(INCLUDE_DIR)/
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp --parents $(HDR_FILES) $(SYSROOT)$(INCLUDE_DIR)/
	$(MAKE) -C ../libc install-headers

kernel-install: $(KERNEL) libk-install
	mkdir -p $(SYSROOT)$(BOOT_DIR)
	cp $(KERNEL) $(SYSROOT)$(BOOT_DIR)
	
clean: 
	rm -f $(KERNEL) $(ISO) $(KERNEL_OBJS) $(KERNEL_DEBUG_FILES)

$(KERNEL): install-headers $(LIBK) $(KERNEL_OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)/linker.ld -o $@ $(CFLAGS) $(CPPFLAGS) $(KERNEL_OBJS) $(LIBS)
	grub-file --is-x86-multiboot $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL) $(SYSROOT)$(BOOT_DIR)/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(SYSROOT)

run: $(ISO)
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)

cleanrun: run clean

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

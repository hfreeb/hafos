SYSROOT = sysroot

TARGET := i686-elf
ARCH := i686

INCLUDE_DIR := /usr/include
BOOT_DIR := /boot
LIB_DIR := /usr/lib

LIBK := libk.a
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
KERNEL_DIR := kernel
KERNEL := $(SYSROOT)$(BOOT_DIR)/hafos.kernel
ARCH_DIR := $(KERNEL_DIR)/arch/$(ARCH)

KERNEL_SRC_DIRS := $(KERNEL_DIR)/core $(KERNEL_DIR)/drivers $(ARCH_DIR)
KERNEL_SRC_FILES := $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.c")
KERNEL_ASM_FILES := $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.S")
KERNEL_DBG_FILES ?= $(shell find $(KERNEL_SRC_DIRS) -type f -name "*.d")

KERNEL_INCLUDE_DIR := $(KERNEL_DIR)/include

KERNEL_OBJS := $(patsubst %.c, %.o, $(KERNEL_SRC_FILES))
KERNEL_OBJS += $(patsubst %.S, %.o, $(KERNEL_ASM_FILES))

KERNEL_LIBS := -nostdlib -lk -lgcc

LIBX_DIR := libc
LIBK_CPPFLAGS += $(CPPFLAGS) -D__is_libk

LIBX_HDR_FILES := $(shell find $(LIBX_DIR) -type f -name "*.h")
LIBX_SRC_FILES := $(shell find $(LIBX_DIR) -type f -name "*.c")
LIBX_ASM_FILES := $(shell find $(LIBX_DIR) -type f -name "*.S")
LIBX_DBG_FILES ?= $(shell find $(LIBX_DIR) -type f -name "*.d")

LIBC_OBJS := $(patsubst %.c, %.o, $(LIBX_SRC_FILES))
LIBC_OBJS += $(patsubst %.c, %.o, $(LIBX_ASM_FILES))

LIBK_OBJS := $(LIBC_OBJS:.o=.libk.o)

.PHONY: all kernel-headers kernel-install iso run clean
.SUFFIXES: .o .libk.o .c .S

all: kernel-install

libk-headers:	
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp $(LIBX_HDR_FILES) $(SYSROOT)$(INCLUDE_DIR)/

libk-install: $(LIBK)

$(LIBK): libk-headers kernel-headers $(LIBK_OBJS)
	mkdir -p $(SYSROOT)$(LIB_DIR)/
	$(AR) rcs $@ $(LIBK_OBJS)

kernel-headers:
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp -r $(KERNEL_INCLUDE_DIR)/* $(SYSROOT)$(INCLUDE_DIR)/

kernel-install: $(KERNEL)
	
$(KERNEL): kernel-headers libk-install $(KERNEL_OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)/linker.ld -o $@ $(CFLAGS) $(CPPFLAGS) $(KERNEL_OBJS) $(KERNEL_LIBS)
	grub-file --is-x86-multiboot $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL) $(SYSROOT)$(BOOT_DIR)/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(SYSROOT)

run: $(ISO)
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)

clean: 
	rm -rf $(KERNEL) $(ISO) $(KERNEL_OBJS) $(KERNEL_DBG_FILES) $(SYSROOT)$(INCLUDE_DIR)/ $(SYSROOT)$(LIB_DIR)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(LIBK_CPPLFAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(LIBK_CPPLFAGS)

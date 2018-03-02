SYSROOT = ../sysroot

include ../make.config

LIBK := $(SYSROOT)$(LIB_DIR)/libk.a
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
	rm -r $(SYSROOT)$(INCLUDE_DIR)/
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp --parents $(HDR_FILES) $(SYSROOT)$(INCLUDE_DIR)/
	$(MAKE) -C ../libc install-headers

install-kernel: $(KERNEL)
	mkdir -p $(SYSROOT)$(BOOT_DIR)/
	cp $(KERNEL) $(SYSROOT)$(BOOT_DIR)

clean: 
	rm -f $(KERNEL) $(ISO) $(KERNEL_OBJS) $(DEBUG_FILES)

$(KERNEL): install-headers $(LIBK) $(KERNEL_OBJS) $(ARCH_DIR)/linker.ld
	$(CC) -T $(ARCH_DIR)/linker.ld -o $@ $(CFLAGS) $(CPPFLAGS) $(KERNEL_OBJS) $(LIBS)
	grub-file --is-x86-multiboot $(KERNEL)

iso: $(ISO)

$(ISO): $(KERNEL) $(SYSROOT)$(BOOT_DIR)/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(SYSROOT)

run: $(ISO)
	qemu-system-i386 $(QEMU_FLAGS) -cdrom $(ISO)

cleanrun: run clean

$(LIBK):
	$(MAKE) -C ../libc install-libk

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

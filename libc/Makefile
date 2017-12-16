SYSROOT := ../sysroot

include ../make.config

ARCH_DIR := arch/$(ARCH)

LIBC := libc.a
LIBK := libk.a

LIBK_CFLAGS += $(CFLAGS)
LIBK_CPPFLAGS += $(CPPFLAGS) -D__is_libk

SRC_FILES := $(shell find -type f -name "*.c")
HDR_FILES := $(shell find -type f -name "*.h")
ASM_FILES := $(shell find -type f -name "*.S")
DEBUG_FILES := $(shell find -type f -name "*.d")

LIBC_OBJS := $(patsubst %.c, %.o, $(SRC_FILES))
LIBC_OBJS += $(patsubst %.S, %.o, $(ASM_FILES))

LIBK_OBJS := $(LIBC_OBJS:.o=.libk.o)

.PHONY: all clean install-headers install-libk
.SUFFIXES: .o .libk.o .c. S

all: install

install: install-headers install-libk

install-headers:
	mkdir -p $(SYSROOT)$(INCLUDE_DIR)/
	cp --parents $(HDR_FILES) $(SYSROOT)$(INCLUDE_DIR)/

install-libk: $(LIBK)
	mkdir -p $(SYSROOT)$(LIB_DIR)/
	cp $(LIBK) $(SYSROOT)$(LIB_DIR)/

clean: 
	rm -f $(SYSROOT)$(LIB_DIR)/$(LIBK) $(SYSROOT)$(LIB_DIR)/$(LIBC) $(LIBK) $(LIBC) $(LIBK_OBJS) $(LIBC_OBJS) $(DEBUG_FILES)

$(LIBK): $(LIBK_OBJS)
	$(AR) rcs $@ $(LIBK_OBJS)

.c.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.S.o:
	$(CC) -MD -c $< -o $@ $(CFLAGS) $(CPPFLAGS)

.c.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

.S.libk.o:
	$(CC) -MD -c $< -o $@ $(LIBK_CFLAGS) $(LIBK_CPPFLAGS)

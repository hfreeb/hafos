.set KERNEL_VMA, 0xC0000000

# Multiboot header constants
.set ALIGN,    1 << 0
.set MEMINFO,  1 << 1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bootstrap_stack, "aw", @nobits
stack_bottom:
.skip 16384
stack_top:

.section .bss, "aw", @nobits
.align 4096
boot_page_directory:
.skip 4096
boot_page_table1:
.skip 4096

.section .text
.global _start
.type _start, @function
_start:
    movl $(boot_page_table1 - KERNEL_VMA), %edi
    movl $0, %esi
    movl $1024, %ecx

1:
    cmpl $(_kernel_start - KERNEL_VMA), %esi
    jl 2f
    cmpl $(_kernel_end - KERNEL_VMA), %esi
    jge 3f

    movl %esi, %edx
    # NOTE: This maps all the kernel pages as (present and) writable, .rodata and .text probably shouldn't be
    orl $0x003, %edx
    movl %edx, (%edi)
2:
    addl $4096, %esi
    addl $4, %edi
    loop 1b
3:
    # Map VGA video memory to 0xC03FF000 (0x003 = present and writable)
    movl $(0x000B8000 | 0x003), boot_page_table1 - KERNEL_VMA + 1023 * 4

    # Map page table to 0x00000000 and KERNAL_VMA
    movl $(boot_page_table1 - KERNEL_VMA + 0x003), boot_page_directory - KERNEL_VMA + 0 # Identity map
    movl $(boot_page_table1 - KERNEL_VMA + 0x003), boot_page_directory - KERNEL_VMA + 768 * 4 # Higher half map

    # Set cr3 to the address of the boot_page_directory
    movl $(boot_page_directory - KERNEL_VMA), %ecx
    movl %ecx, %cr3

    # Enable paging and the write-protect bit
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half
    lea 4f, %ecx
    jmp *%ecx

4:
    # Unmap identity mapping
    movl $0, boot_page_directory + 0

    # Reload crc3 to force a TLB flush
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Set up stack
    mov $stack_top, %esp

    # Enter the kernel
    call kernel_main

    cli
hang:
    hlt
    jmp hang
.end:

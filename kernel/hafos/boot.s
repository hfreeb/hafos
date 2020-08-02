.set KERNEL_VMA, 0xC0000000

.set PAGE_PRESENT, 1 << 0
.set PAGE_RW,      1 << 1

.set CR0_PAGING,        1 << 31
.set CR0_WRITE_PROTECT, 1 << 16

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
    # at this point we haven't jumped to the higher half,
    # so we need to be using the physical address rather than
    # the virtual address, so subtract KERNEL_VMA.

    # map the first 1023 pages (the 1024th is used for the VGA text buffer)
    movl $(boot_page_table1 - KERNEL_VMA), %edi
    movl $0, %esi
    movl $1023, %ecx

map_page:
    cmpl $(_kernel_start - KERNEL_VMA), %esi
    jl map_skip
    cmpl $(_kernel_end - KERNEL_VMA), %esi
    jge map_end

    movl %esi, %edx
    # NOTE: this maps all the kernel pages as writable,
    # .rodata and .text probably shouldn't be
    orl $(PAGE_PRESENT | PAGE_RW), %edx
    movl %edx, (%edi)
map_skip:
    addl $4096, %esi
    addl $4, %edi
    loop map_page
map_end:
    # map VGA video memory to 0xC03FF000
    movl $(0x000B8000 | PAGE_PRESENT | PAGE_RW), boot_page_table1 - KERNEL_VMA + 1023 * 4

    # map page table to 0x00000000 and KERNAL_VMA
    # the identity map is only required initially when paging is enabled
    # this is unmapped once we have jumped to the higher half
    movl $((boot_page_table1 - KERNEL_VMA) + PAGE_PRESENT + PAGE_RW), boot_page_directory - KERNEL_VMA + 0 # Identity map
    movl $((boot_page_table1 - KERNEL_VMA) + PAGE_PRESENT + PAGE_RW), boot_page_directory - KERNEL_VMA + 768 * 4 # Higher half map

    # set the page directory
    movl $(boot_page_directory - KERNEL_VMA), %ecx
    movl %ecx, %cr3

    # enable paging and the write-protect bit
    movl %cr0, %ecx
    orl $(CR0_PAGING | CR0_WRITE_PROTECT), %ecx
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

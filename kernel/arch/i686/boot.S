.section .bss

.align 16
stack_bottom:
.skip 32768 # 32KiB
stack_top:

.section .text

.global start
.type start, @function

start:
    cli

    movl $stack_top, %esp
    # Ensure the stack is 16 byte aligned
    and $-16, %esp

    call _init

    pushl %eax # Multiboot header magic
    pushl %ebx # Multiboot header pointer
    call kernel_main

    cli
hang:
    hlt
    jmp hang

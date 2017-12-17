.section .bss

.align 16
stack_bottom:
.skip 16384 # 16KiB
stack_top:

.section .text

.global start
.type start, @function

start:
    movl $stack_top, %esp

    call _init

    call kernel_main
    hlt

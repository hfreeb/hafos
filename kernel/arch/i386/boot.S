.section .text

.global start
.type start, @function

start:
    call kernel_main
    hlt


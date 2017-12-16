.section .init
.global _init
.type _init, @function

_init:
    push %ebp
    movl %esp, %ebp
    //crtbegin.o's .init goes here

.section .fini
.global _fini
.type _fini @function
_fini:
    pushl %ebp
    movl %esp, %ebp
    //crtbegin.o's .fini goes here

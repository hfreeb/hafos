.section .init
    //crtend.o's .init goes here
    popl %ebp
    ret

.section .fini
    //crtend.o's .fini goes here
    popl %ebp
    ret

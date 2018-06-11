.section .text

//TODO: Use inline assembly & probably in a header
.global paging_load_directory
paging_load_directory:
    push %ebp
    mov %esp, %ebp
    mov 8(%esp), %eax
    mov %eax, %cr3
    mov %ebp, %esp
    pop %ebp
    ret

.global paging_enable
paging_enable:
    push %ebp
    mov %esp, %ebp
    mov %cr0, %eax
    or $0x80000000, %eax
    mov %eax, %cr0
    mov %ebp, %esp
    pop %ebp
    ret

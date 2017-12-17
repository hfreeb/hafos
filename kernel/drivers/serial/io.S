.section .text

.global outb

// 8(%esp): data byte
// 4(%esp): I/O port
outb:
    movb 8(%esp), %al
    movw 4(%esp), %dx
    outb %al, %dx

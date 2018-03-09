.section .text

.extern idt_register_isr
.extern idt_handle_isr

.global idt_flush

idt_flush:
    mov 4(%esp), %eax
    lidt (%eax)
    ret

isr_common_stub:
    pusha //Push all registers to the stack

    mov %ds, %ax
    push %eax

    //Load the kernel  segment descriptors
    mov $0x10, %ax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    //Call ISR handler
    push %esp
    call idt_handle_isr

    add $4, %esp

    pop %eax
    mov %ax, %ds
    mov %ax, %es
    mov %ax, %fs
    mov %ax, %gs

    popa //Restore registers

    //Clean up pushed error code and ISR number
    add $8, %esp
    sti
    iret

.macro ISR index, err=0
    jmp isr_register_\index

    isr_\index:
        cli
    .if !\err
        push $0
    .endif
        push $\index
        jmp isr_common_stub

    isr_register_\index:
        push $isr_\index
        push $\index
        call idt_register_isr
        add $8, %esp
.endm 

.macro ISR_ERR index
    ISR \index, 1
.endm

.global idt_register_stubs

idt_register_stubs:
    ISR 0
    ISR 1
    ISR 2
    ISR 3
    ISR 4
    ISR 5
    ISR 6
    ISR 7
    ISR_ERR 8
    ISR 9
    ISR_ERR 10
    ISR_ERR 11
    ISR_ERR 12
    ISR_ERR 13
    ISR_ERR 14
    ISR 15
    ISR 16
    ISR 17
    ISR 18
    ISR 19
    ISR 20
    ISR 21
    ISR 22
    ISR 23
    ISR 24
    ISR 25
    ISR 26
    ISR 27
    ISR 28
    ISR 29
    ISR 30
    ISR 31
    //PIC IRQs start
    ISR 32
    ISR 33
    ISR 34
    ISR 35
    ISR 36
    ISR 37
    ISR 38
    ISR 39
    ISR 40
    ISR 41
    ISR 42
    ISR 43
    ISR 44
    ISR 45
    ISR 46
    ISR 47
    //PIC IRQs end
    ISR 127

section .asm

global idt_load
idt_load:
    push ebp
    mov ebp, esp ; context switch, moves c frame to stack and jumps to assembly function

    mov ebx, [ebp + 8]
    lidt [ebx]

    pop ebp
    ret
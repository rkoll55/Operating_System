[BITS 32]

global _start
global problem
extern kernel_main

CODE_SEG equ 0x08
DATA_SEG equ 0x10

_start:
	mov ax, DATA_SEG
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	mov ebp, 0x00200000
	mov esp, ebp

	in al, 0x92
	or al, 2
	out 0x92, al

    ;remap master PIC
    mov al, 00010001b
    out 0x20, al ;master pic
    mov al, 0x20 ; interrupt 20 for master
    out 0x21, al
    mov al, 00000001b
    out 0x21, al
    ; end of remapping
    sti
	call kernel_main
	jmp $

problem:
    mov eax, 0
    div eax
times 512-($ - $$) db 0

ORG 0x7c00 
BITS 16

CODE_SEG equ gdt_code - gdt_start
DATA_SEG equ gdt_data - gdt_start

_start:
	jmp short start
	nop

times 33 db 0

start:
	jmp 0:step2


step2:
	cli ; clear interrupts

	mov ax, 0x00
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00 ; set up stack pointer	
	sti ; enable interrupts

.load_protected:
	cli
	lgdt[gdt_descriptor]
	mov eax, cr0
	or eax, 0x1
	mov cr0, eax
	jmp CODE_SEG:load32
	
;GDT
gdt_start:
gdt_null:
	dd 0x0
	dd 0x0

;offset 0x8
gdt_code:
	dw 0xfff;
	dw 0; base 0 to 15 bits
	db 0 ; base 16 - 23 bits
	db 0x9a
	db 11001111b
	db 0

gdt_data: ; DS, SS, ES, FS, GS
	dw 0xfff;
	dw 0; base 0 to 15 bits
	db 0 ; base 16 - 23 bits
	db 0x92
	db 11001111b
	db 0

gdt_end:

gdt_descriptor:
	dw gdt_end - gdt_start -1
	dd gdt_start 

[BITS 32]
load32:
    mov eax, 1
    mov ecx, 100
    mov edi, 0x0100000 ;1 megabyte
    call ata_lba_read
    jmp CODE_SEG:0x0100000

ata_lba_read:
    mov ebx, eax, ;backup LNA? send highert 8 bits of LBA to hard disk controller
    shr eax, 24
    or eax, 0xE0; selects master drive
    mov dx, 0x1F6
    out dx, al

    ; send total sectors to read
    mov eax, ecx
    mov dx, 0x1F2
    out dx, al

    ; send more bits of the LBA
    mov eax, ebx
    mov dx, 0x1F3
    out dx, al

    ; send last 8 bits of lba
    mov dx, 0x1F4
    mov eax, ebx
    shr eax, 8
    out dx, al

    ;send upper 16 bits of LBA
    mov dx, 0x1F5
    mov eax, ebx
    shr eax, 16
    out dx, al

    mov dx, 0x1f7
    mov al, 0x20
    out dx, al

    ; time to read all sectors into memory
.next_sector:
    push ecx

;check for read
.try_again:
    mov dx, 0x1f7
    in al, dx
    test al, 8
    jz .try_again

; read 256 words at a time to memory
    mov ecx, 256
    mov dx, 0x1F0
    rep insw ; reads from port and stores in port in edi (repeats 256 times)
    pop ecx

    loop .next_sector
    ret

times 510-($ - $$) db 0
dw 0xAA55


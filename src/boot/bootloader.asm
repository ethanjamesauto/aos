bits 16
org 0x7c00

%define num_sectors 64
%include "macros.asm"

jmp boot
times 3 - ($-$$) nop
times 62 - ($-$$) db 0

boot:
xor ax, ax ; zero our segment registers
mov es, ax
mov ds, ax
mov bx, past_bootsector

mov ah, 0x02 ; read sectors
mov al, num_sectors ; number of sectors to read
mov ch, 0 ; cylinder 0
mov cl, 2 ; sector 2 (1 after the boot sector)
mov dh, 0 ; head 0
int 0x13 ; call BIOS disk interrupt

jc disk_error ; if the carry bit is set, then an error occured
jmp init

disk_error: ; print out an exclamation mark and hang
print_char '!'
jmp $

init:

;load VESA info to es:di (0:500)
mov ax, 0x4f00
mov bx, 0
mov es, bx
mov di, 0x500
int 0x10

mov ax, 0x2401
int 0x15 ; enable A20 bit
video_mode 0x03

cli
lgdt [gdt_pointer] ; load the gdt table
mov eax, cr0 
or eax, 0x1 ; set the protected mode bit on special CPU reg cr0
mov cr0, eax
jmp code_seg:main ; long jump to the code segment

bits 32
main:
mov ax, data_seg
mov ds, ax
mov es, ax
mov fs, ax
mov gs, ax
mov ss, ax

jmp past_bootsector

gdt_start:
dq 0x0

gdt_code:
dw 0xFFFF
dw 0x0
db 0x0
db 10011010b
db 11001111b
db 0x0

gdt_data:
dw 0xFFFF
dw 0x0
db 0x0
db 10010010b
db 11001111b
db 0x0

gdt_end:

gdt_pointer:
dw gdt_end - gdt_start
dd gdt_start

code_seg equ gdt_code - gdt_start
data_seg equ gdt_data - gdt_start

times 510 - ($-$$) db 0
dw 0xaa55

past_bootsector:

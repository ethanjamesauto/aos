%define parLoc 18

print_hex_func:
pusha
mov bp, sp
mov bx, [bp + parLoc]

.check_zero:
cmp bx, 0
jne .start
mov ax, 0x0e30
int 0x10
jmp .done

.start:
cmp bx, 0
je .print
mov ax, bx
and ax, 0x0f
cmp ax, 0x0a
jl .num
add ax, 0x27
.num:
add ax, 0x30
push ax
shr bx, 4
jmp .start

.print:
cmp bp, sp
je .done
pop ax
mov ah, 0x0e
int 0x10
jmp .print

.done:
mov bx, [bp - 2]
mov sp, bp
popa
ret

print_string_func:
pusha
mov bp, sp
mov si, [bp + parLoc]

.loop:
mov ax, [cs:si]
inc si
cmp al, 0
je .done
mov ah, 0x0e
int 0x10
jmp .loop

.done:
mov sp, bp
popa
ret

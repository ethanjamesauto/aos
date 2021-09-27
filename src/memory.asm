%include "memory_config.asm"

bits 32

global alloc_static
global port_byte_in
global port_byte_out
global port_word_in
global port_word_out
global memtest
global test_asm

alloc_static:
mov eax, [MEMORY_START]
add eax, [esp + 4]
mov [MEMORY_START], eax
sub eax, [esp + 4]
ret

port_byte_in:
push ebp
mov ebp, esp
push edx

xor eax, eax
mov edx, [ebp + 8]
in al, dx

pop edx
mov esp, ebp
pop ebp
ret

port_byte_out:
push ebp
mov ebp, esp
push edx
push eax

mov edx, [ebp + 8]
mov eax, [ebp + 12]
out dx, al

pop eax
pop edx
mov esp, ebp
pop ebp
ret

port_word_in:
push ebp
mov ebp, esp
push edx

xor eax, eax
mov edx, [ebp + 8]
in ax, dx

pop edx
mov esp, ebp
pop ebp
ret

port_word_out:
push ebp
mov ebp, esp
push edx
push eax

mov edx, [ebp + 8]
mov eax, [ebp + 12]
out dx, ax

pop eax
pop edx
mov esp, ebp
pop ebp
ret

memtest:
mov eax, [esp + 4]
mov dword[eax], 0x12345678
._if:
cmp dword[eax], 0x12345678
je ._then
._else:
mov eax, 0
jmp ._done
._then:
mov eax, 1
._done:
ret

test_asm:
push ebp
mov ebp, esp
push edx
push ebx

xor edx, edx
mov ax, 6247
mov bx, 1924
mul bx
shl edx, 16
mov dx, ax
mov eax, edx

pop ebx
pop edx
mov esp, ebp
pop ebp
ret

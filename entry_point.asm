%include "memory_config.asm"

bits 32

extern start

kernel_entry:
mov ebp, STACK_START ; set up the new stack
mov esp, ebp
mov dword[MEMORY_START], MEMORY_START + 4 ; move the "free memory pointer" to its initial location
call start
jmp $


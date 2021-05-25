%macro print_hex 1
push %1
call print_hex_func
add sp, 2
%endmacro

%macro print_string 1
push %1
call print_string_func
add sp, 2
%endmacro

%macro print_char 1
mov ah, 0x0e
mov al, %1
int 0x10
%endmacro

%macro move_cursor 2
mov ah, 0x02
mov bh, 0
mov dh, %1
mov dl, %2
int 0x10
%endmacro

%macro video_mode 1
mov ah, 0
mov al, %1
int 0x10
%endmacro
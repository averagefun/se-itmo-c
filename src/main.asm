%include "src/lib.inc"
%include "src/colon.inc"
%include "src/dict.inc"
%include "src/words.inc"

%define buf_size 256
global _start


section .data

buf_ovflow_msg: db "Error: buffer overflow!", 0xA, 0
not_found_msg: db "Error: key not found!", 0xA, 0


section .bss

buf_addr: resb buf_size


section .text

_start:
    .read_buf:
        mov rdi, buf_addr
        mov rsi, buf_size
        call read_line

        test rax, rax
        jz .buf_ovflow

    .search_key:
        mov rdi, buf_addr
        mov rsi, dict_pointer 
        call find_word

        test rax, rax
        jz .not_found

        lea rdi, [rax+8]    ;   rdi refers to key
        push rdi
        call string_length
        pop rdi

        lea rdi, [rdi+rax+1] ;  rdi refers to value
        call print_string

    .exit:
        call print_newline
        call exit

    .buf_ovflow:
        mov rdi, buf_ovflow_msg
        call print_error_string
        call exit

    .not_found:
        mov rdi, not_found_msg
        call print_error_string
        call exit


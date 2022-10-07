%include "src/lib.inc"
global find_word


section .text

; function find_word(string_pointer: rdi, dict_pointer: rsi): {
;   if (string in dict) {
;       return key_address: rax 
;   } else {
;       return 0: rax
;   }
find_word:
    push rbx        ;   save rbx as callee-saved register
    mov rbx, rsi    ;   we'l use rbx as dict_pointer

    .loop
        lea rsi, [rbx+8]    ;   rsi refers to key
        call string_equals
        test rax, rax
        jnz .exit
        mov rbx, qword[rbx] ;   rbx refers to next dict pair
        test rbx, rbx
        jnz .loop         

    .not_found
        xor rbx, rbx

    .exit
        mov rax, rbx    ;   rbx (dict_pointer) -> rax
        pop rbx         ;   restor rbx from stack
        ret


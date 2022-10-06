global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global read_char
global read_word
global parse_uint
global parse_int
global string_copy

section .text
 
; Принимает код возврата и завершает текущий процесс
; function exit(exit_code: rdi)
exit: 
    xor rax, 60     ; 60 -> rax, sys_exit code for syscall
    syscall     ; system call
    ret 

; Принимает указатель на нуль-терминированную строку, возвращает её длину
; function string_length(string_address: rdi) -> str_len: rax
string_length:
    xor rax, rax    ; clear rax before start counting chars

    .loop:
        cmp byte[rdi+rax], 0   ; cmp current char with 0 (terminated sign)
	    je .end     ; if 0 -> jump to exit
	    inc rax     ; otherwise increment rax, that counting chars
	    jmp .loop   ; jump to start of cycle

    .end:
        ret     ; return from function

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
; function print_string(string_address: rdi)
print_string:
    call string_length  ;   string_length(string_address) -> rax
    mov rdx, rax        ;   rax -> rdx (now rdx contains string length)
                        ;   so rdx contains amount of bytes to write in syscall

    mov rax, 1  ; 1 -> rax, write code for syscall
    mov rsi, rdi  ; rdi -> rsi, now rsi contains string_address
    mov rdi, 1  ; 1 -> rdi, descriptor (stdout)
    syscall
    ret

; Принимает код символа и выводит его в stdout
; function print_char(char_code: rdi)
print_char:
    push rdi        ;   rdi -> stack, so rsp addressing to memory, that contains char_code
    mov rsi, rsp    ; rsp -> rsi, so rdi addressing to memory, that contains char_code
    pop rdi         ; pop increase rsp, but not clear the char_code

    mov rdx, 1  ; 1 -> rdx (rdx contains amount of bytes to write: in that case 1)
    mov rax, 1  ; 1 -> rax, write code for syscall
    mov rdi, 1  ; 1 -> rdi, descriptor (stdout)
    syscall
    ret

; Переводит строку (выводит символ с кодом 0xA)
; function print_newline()
print_newline:
    mov rdi, 0xA    ; 0xA -> rdi, so rdi contains new_line symbol
    call print_char ; print_char(0xA)
    ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
; function print_uint(8_byte_number: rdi)
print_uint:
    mov r8, rbx     ;   save rbx register value (callee-saved)
    mov r9, rsp     ;   save rsp register value (callee-saved)

    mov rbx, 10    ;   base of decimal system
    mov rax, rdi    ;   rdi -> rax, so rax contains 8_byte_number
    push 0  ;   0 -> stack, means end of the string to print

    .loop:
        xor rdx, rdx    ;   clear rdx because we want (rdx rax) = (0 rax)
        div rbx  ;   (rdx rax) / rbx = integer_part -> rax, remainder -> rdx
        add rdx, '0'    ;   covert last digit to ASCII sign
        
        dec rsp  ;  rsp = rsp - 1
        mov byte[rsp], dl   ;   put the last digit (dl) to top of the stack
        cmp rax, 0  ;   if (rax == 0) -> exit the loop
        je .end
        jmp .loop

    .end:
        mov rdi, rsp

        mov rbx, r8     ;   restore rbx value
        push r9         ;   save r9 in stack (bacuse call can change r9, but must save rsp)

        call print_string   ;   print_string(rdi)
        
        pop rsp          ;   restore r9 value and write to rsp, so restore rsp value
        ret

; Выводит знаковое 8-байтовое число в десятичном формате 
; function print_int(8_byte_signed_number: rdi)
print_int:
    test rdi, rdi   ;   set flags according to the value in rdi (we are interested in SF)
    jns .uint       ;   if positive value -> jumo to print unsinged value
    
    neg rdi         ;   if negative value -> get negative
    push rdi        ;   save rdi (contains number) at stack

    mov rdi, '-'    ;   
    call print_char ;   print_char('-': rdi)

    pop rdi
    .uint: call print_uint  ;   print_uint(8_byte_number: rdi)

    mov rdi, r12     ;   restore rdi value
    ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
; function string_equals(str1_address: rdi, str2_address, rsi) -> str1.equals(str2): rax
string_equals:
    xor rax, rax    ;   0 -> rax, so by default strings not equal

    .loop:
        mov r8b, byte[rdi]  ;   str1[rdi] -> r8b
        cmp r8b, byte[rsi]  ;   compare str1[rdi] and str2[rsi]
        jne .end            ;   if str1[rdi] != str2[rsi] -> go to .end
        cmp r8b, 0          ;   compare str1[rdi] and 0 (end of the string)
        je .equals          ;   if str1[rdi] == 0 -> go to .equals
        inc rdi             ;   rdi++
        inc rsi             ;   rsi++
        jmp .loop           ;   loop for all chars in strings

    .equals:
        mov rax, 1  ;   1 -> rax, string eqauls if all symbols equals and strings have same length

    .end:
        ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
; function read_char() -> readed_char: rax
read_char:
    push 0          ; 0 -> stack, so rsp addressing to memory, that will contain readed_char
    mov rsi, rsp    ; rsp -> rsi, so rdi addressing to memory, that will contain readed_char

    mov rdx, 1      ; 1 -> rdx (rdx contains amount of bytes to read: in that case 1)
    xor rax, rax    ; 0 -> rax, read code for syscall
    xor rdi, rdi    ; 0 -> rdi, descriptor (stdout)

    syscall
    pop rax         ; readed_char -> rax
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор
;
; function read_word(buf_addr: rdi, buf_size: rsi) -> {
;    if (success)
;       return buf_affr: rax, word_size: rdx
;    else
;       return 0: rax
; }
read_word:
    push rdi                ; save buf_addr to stack
    push rbx                ; save rbx to stack
    mov rbx, rdi            ; rdi -> rbx, rbx contains buf_addr
                            ;   we'l use rbx as buffer pointer
    push r13                ; save r13 to stack, we'l use r13 as word chars counter
    push r12                ; save r12 value, we use r12 to keep buf_size
    mov r12, rsi            ; rsi -> r12

    .start_loop:
        call read_char      ; read_char() -> readed_char: rax
        cmp rax, 0x20       ; check for space
        je .start_loop      ; if (space) -> jump to .start_loop
        cmp rax, 0x9        ; check for tab
        je .start_loop      ; if (tab) -> jump to .start_loop
        cmp rax, 0xA        ; check for lf
        je .start_loop      ; if (lf) -> jump to .start_loop

    .loop:
        cmp rax, 0x20       ; if readed_char == space 
        je .end             ;   -> jump to .end
        cmp rax, 0xA        ; if readed_char == lf (line feed) 
        je .end             ;   -> jump to .end
        cmp rax, 0x0        ; if readed_char == 0 
        je .end             ;   -> jump to .end

        inc r13             ; count one char

        cmp r13, r12        ; compare word_size with buf_size
        je .buf_ovflow      ; if word_size == buf_size -> failed because word_size <= buf_size - 1 

        mov byte[rbx], al   ; readed_char -> buf[rdi] 

        call read_char      ; read_char() -> readed_char: rax
        inc rbx             ; inc buf pointer
        jmp .loop 

    .end:
        mov byte[rbx], 0    ; null-terminator at the end of string
        mov rdx, r13        ; r13 -> rdx, so rdx contains word_size
        pop r12             ; restore r12 value from stack
        pop r13             ; restore r13 value from stack
        pop rbx             ; restore rbx from stack
        pop rax             ; buf_addr -> rax
        ret

    .buf_ovflow:
        pop r12             ; restore r12 value from stack
        pop r13             ; restore r13 value from stack
        pop rbx             ; restore rbx from stack
        pop rax             ; buf_addr -> rax
        xor rax, rax        ; 0 -> rax, buffer overflow exception
        ret

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
;
; function parse_uint(string_address: rdi) -> {
;    if (success)
;       return number: rax, number_len: rdx
;    else
;       return 0: rdx
; }
parse_uint:
    mov r10, 10     ;   base of decimal system
    xor rax, rax    ;   0 -> rax, rax - number accumulator
    xor rcx, rcx    ;   0 -> rcx, rcx - number_len counter

    .loop:
        xor r8, r8          ; clear r8
        mov r8b, byte[rdi]  ; char at address rdi -> r8

        cmp r8, '9'         ; check, that char is a digit
        jg .end
        cmp r8, '0'
        jl .end 
        sub r8, '0'         ; convert r8 char to int
        
        mul r10             ; rax * r10 -> (rdx rax), slice left by one digit
        add rax, r8         ; add last digit to rax

        inc rdi             ; rdi++, so take new char   
        inc rcx             ; rcx++ or number_len++
        jmp .loop   
    .end:
        mov rdx, rcx        ; number_len -> rdx
        ret

; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
;
; function parse_int(string_address: rdi) -> {
;    if (success)
;       return number: rax, number_len(with sign): rdx
;    else
;       return 0: rdx
; }
parse_int:
    push r12    ;   save r12 value in stack
    xor r12, r12    ;   r12 = 0 it's like sign flag

    xor r8, r8
    mov r8b, byte[rdi]  ;   check for sign

    cmp r8b, '-'        ;   if we have '-' 
    je .negative_before_uint  ; -> jump to neg_bf_uint

    .uint: call parse_uint

    cmp r12, 1  ;   check for negative
    je .negative_after_uint

   .end:
        pop r12     ;   restore r12 from stack
        ret

   .negative_before_uint:
        inc rdi     ;   rdi++, skip sign
        mov r12, 1  ;   r12 = 1, mean negative number
        jmp .uint

   .negative_after_uint:
        neg rax     ;   get negative number
        inc rdx     ;   rdx++ because number_len now increase by '-' sign
        jmp .end

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
;
; function string_copy(string_addr: rdi, buf_addr: rsi, buf_size: rdx) -> {
;    if (success)
;       return str_len: rax
;    else
;       return 0: rax
; }
string_copy:
    push rdi    ;   save all caller-saved registers
    push rsi
    push rdx

    call string_length  ;   str_len -> rax
    
    pop rdx
    pop rsi
    pop rdi

    cmp rax, rdx    ;   compare str_len and buf_size
    jge .buf_ovflow ;   if str_len >= buf_size (+null-terminated) -> jump to buf_ovflow

    xor rcx, rcx    ;   rcx - buf and string index
    .loop:
        xor r8, r8
        mov r8b, byte[rdi+rcx]  ;   str[rcx] -> r8
        mov byte[rsi+rcx], r8b  ;   r8 -> buf[rcx]

        cmp r8b, 0  ;   compare char to null-terminated
        je .end

        inc rcx
        jmp .loop

    .end:
        ret

    .buf_ovflow:
        xor rax, rax
        jmp .end

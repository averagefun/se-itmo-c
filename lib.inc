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
        cmp byte [rdi+rax], 0   ; cmp current char with 0 (terminated sign)
	    je .end     ; if 0 -> jump to exit
	    inc rax     ; otherwise increment rax, that counting chars
	    jmp .loop   ; jump to start of cycle

    .end:
        ret     ; return from function

; Принимает указатель на нуль-терминированную строку, выводит её в stdout
; function print_string(string_address: rdi)
print_string:
    call string_length  ;   len(str) -> rax
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
print_newline:
    xor rax, rax
    ret

; Выводит беззнаковое 8-байтовое число в десятичном формате 
; Совет: выделите место в стеке и храните там результаты деления
; Не забудьте перевести цифры в их ASCII коды.
print_uint:
    xor rax, rax
    ret

; Выводит знаковое 8-байтовое число в десятичном формате 
print_int:
    xor rax, rax
    ret

; Принимает два указателя на нуль-терминированные строки, возвращает 1 если они равны, 0 иначе
string_equals:
    xor rax, rax
    ret

; Читает один символ из stdin и возвращает его. Возвращает 0 если достигнут конец потока
read_char:
    xor rax, rax
    ret 

; Принимает: адрес начала буфера, размер буфера
; Читает в буфер слово из stdin, пропуская пробельные символы в начале, .
; Пробельные символы это пробел 0x20, табуляция 0x9 и перевод строки 0xA.
; Останавливается и возвращает 0 если слово слишком большое для буфера
; При успехе возвращает адрес буфера в rax, длину слова в rdx.
; При неудаче возвращает 0 в rax
; Эта функция должна дописывать к слову нуль-терминатор

read_word:
    ret
 

; Принимает указатель на строку, пытается
; прочитать из её начала беззнаковое число.
; Возвращает в rax: число, rdx : его длину в символах
; rdx = 0 если число прочитать не удалось
parse_uint:
    xor rax, rax
    ret




; Принимает указатель на строку, пытается
; прочитать из её начала знаковое число.
; Если есть знак, пробелы между ним и числом не разрешены.
; Возвращает в rax: число, rdx : его длину в символах (включая знак, если он был) 
; rdx = 0 если число прочитать не удалось
parse_int:
    xor rax, rax
    ret 

; Принимает указатель на строку, указатель на буфер и длину буфера
; Копирует строку в буфер
; Возвращает длину строки если она умещается в буфер, иначе 0
string_copy:
    xor rax, rax
    ret

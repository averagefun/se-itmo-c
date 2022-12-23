default rel
extern _GLOBAL_OFFSET_TABLE_
global image_apply_sepia_asm_impl

section .rodata
rtone: dd 0.299
gtone: dd 0.587
btone: dd 0.114

rbias: dd 49.
gbias: dd -14.
bbias: dd -56.

colormax: dd 255.
colormin: dd 0.

section .text

; int image_apply_sepia_asm_impl(rdi: source_img_data*, rsi: new_img_data*)
image_apply_sepia_asm_impl:
  .loop:
    ; each color - 1 byte

    ; mov 8 bytes (g3, b3, r2, g2, b2, r1, g1, b1) to rax
    mov rax, [rdi]

    ; r8d will keep 4 green pixels (g3, g2, g1, g4)
    xor r8, r8
    mov rcx, rax
    shr rcx, 32
    and ecx, 0xFF000000
    or r8d, ecx

    mov rcx, rax
    shr rcx, 16
    and ecx, 0x00FF0000
    or r8d, ecx

    mov rcx, rax
    and ecx, 0x0000FF00
    or r8d, ecx
    ; r8d = (g3, g2, g1, -)

    ; r9d will keep 4 blue pixels (b3, b2, b1, b4)
    xor r9, r9
    mov rcx, rax
    shr rcx, 24
    and ecx, 0xFF000000
    or r9d, ecx

    mov rcx, rax
    shr rcx, 8
    and ecx, 0x00FF0000
    or r9d, ecx

    mov rcx, rax
    shl rcx, 8
    and ecx, 0x0000FF00
    or r9d, ecx
    ; r9d = (b3, b2, b1, -)

    ; r10d will keep 4 red pixels (r2, r1, r4, r3)
    xor r10, r10
    mov rcx, rax
    shr rcx, 16
    and ecx, 0xFF000000
    or r10d, ecx

    mov rcx, rax
    and ecx, 0x00FF0000
    or r10d, ecx
    ; r10d = (r1, r2, -, -)

    ; mov 4 bytes (r4, g4, b4, r3) to eax
    mov eax, [rdi+8]

    mov ecx, eax
    shr ecx, 16
    and ecx, 0x000000FF
    or r8d, ecx
    ; r8d = (g3, g2, g1, g4)

    mov ecx, eax
    shr ecx, 8
    and ecx, 0x000000FF
    or r9d, ecx
    ; r9d = (b3, b2, b1, b4)

    mov ecx, eax
    shr ecx, 16
    and ecx, 0x0000FF00
    or r10d, ecx

    mov ecx, eax
    and ecx, 0x000000FF
    or r10d, ecx
    ; r10d = (r2, r1, r4, r3)

    ; -----------------------

    ; r8d = (g3, g2, g1, g4)
    ; r9d = (b3, b2, b1, b4)
    ; r10d = (r2, r1, r4, r3)

    ; vector operations

    movq xmm0, r8
    movq xmm1, r9
    movq xmm2, r10
    pxor xmm3, xmm3

    punpcklbw xmm0, xmm3
    punpcklbw xmm1, xmm3
    punpcklbw xmm2, xmm3
    punpcklwd xmm0, xmm3
    punpcklwd xmm1, xmm3
    punpcklwd xmm2, xmm3

    cvtdq2ps xmm0, xmm0
    cvtdq2ps xmm1, xmm1
    cvtdq2ps xmm2, xmm2

    movq xmm3, [gtone]
    movq xmm4, [btone]
    movq xmm5, [rtone]
    
    punpckldq xmm3, xmm3
    punpckldq xmm4, xmm4
    punpckldq xmm5, xmm5
    punpcklqdq xmm3, xmm3
    punpcklqdq xmm4, xmm4
    punpcklqdq xmm5, xmm5

    mulps xmm3, xmm0
    mulps xmm4, xmm1
    mulps xmm5, xmm2

    addps xmm3, xmm4
    addps xmm3, xmm5

    ; xmm3 = rtone * r + gtone * g + btone * b
    pxor xmm4, xmm4
    pxor xmm5, xmm5

    movq xmm4, [gbias]
    movq xmm5, [bbias]
    movq xmm6, [rbias]
    
    punpckldq xmm4, xmm4
    punpckldq xmm5, xmm5
    punpckldq xmm6, xmm6
    punpcklqdq xmm4, xmm4
    punpcklqdq xmm5, xmm5
    punpcklqdq xmm6, xmm6

    ; max -> 255
    movd xmm7, [colormax]
    ; min -> 0
    movd xmm8, [colormin]

    punpckldq xmm7, xmm7
    punpckldq xmm8, xmm8
    punpcklqdq xmm7, xmm7
    punpcklqdq xmm8, xmm8

    addps xmm4, xmm3
    ; minps xmm4, xmm7
    ; maxps xmm4, xmm8

    addps xmm5, xmm3
    ; minps xmm5, xmm7
    ; maxps xmm5, xmm8

    addps xmm6, xmm3
    ; minps xmm6, xmm7
    ; maxps xmm6, xmm8

    cvtps2dq xmm4, xmm4
    cvtps2dq xmm5, xmm5
    cvtps2dq xmm6, xmm6

    packssdw xmm4, xmm4
    packssdw xmm5, xmm5
    packssdw xmm6, xmm6
    packuswb xmm4, xmm4
    packuswb xmm5, xmm5
    packuswb xmm6, xmm6

    movd r8d, xmm4
    movd r9d, xmm5
    movd r10d, xmm6

    ; -----------------------

    ; r8d = (g3, g2, g1, g4)
    ; r9d = (b3, b2, b1, b4)
    ; r10d = (r2, r1, r4, r3)

    xor rax, rax
    mov ecx, r8d
    shr ecx, 24
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r9d
    shr ecx, 24
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r10d
    shr ecx, 24
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r8d
    shr ecx, 16
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r9d
    shr ecx, 16
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r10d
    shr ecx, 16
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r8d
    shr ecx, 8
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r9d
    shr ecx, 8
    and ecx, 0x000000FF
    or rax, rcx

    ; rax = (g3, b3, r2, g2, b2, r1, g1, b1)
    mov [rsi], rax

    xor rax, rax
    mov ecx, r10d
    shr ecx, 8
    and ecx, 0x000000FF
    or rax, rcx
    shl eax, 8

    mov ecx, r8d
    and ecx, 0x000000FF
    or rax, rcx
    shl eax, 8

    mov ecx, r9d
    and ecx, 0x000000FF
    or rax, rcx
    shl rax, 8

    mov ecx, r10d
    and ecx, 0x000000FF
    or rax, rcx

    ; eax = (r4, g4, b4, r3)
    mov [rsi+8], eax

    add rdi, 12
    add rsi, 12

    dec rdx
    jnz .loop
    mov rax, 40

    ret
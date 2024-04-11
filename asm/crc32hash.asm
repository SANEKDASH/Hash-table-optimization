section .text

global asm_CRC32Hash

asm_CRC32Hash:
; Entering function
    push rbp
    mov rbp, rsp

    xor rax, rax
; Hash start value
    mov eax, 0xffffffff

    jmp .HashTest
.HashCycle:
    crc32 eax, byte [rdi]

    inc rdi
.HashTest:
    cmp byte [rdi], 0
    ja .HashCycle

; Leaving function
    mov rsp, rbp
	pop rbp

	ret

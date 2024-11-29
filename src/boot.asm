BITS 32

section .text
    ALIGN 4
    DD 0x1BADB002
    DD 0x3
    DD -(0x1BADB002 + 0x3)
    dd 0
    dd 0
    dd 0
    dd 0 
    dd 0

    dd 0
    dd 1280 ; width
    dd 1024; height
    dd 16 ; bpp

;define MULTIBOOT_INFO_VBE_INFO                 0x00000800
;define MULTIBOOT_INFO_FRAMEBUFFER_INFO         0x00001000


global start
extern kmain

start:
    cli
    mov esp, stack_space
    push ebx
    push eax
    call kmain
    hlt

HaltKernel:
    cli
    hlt
    jmp HaltKernel

section .bss
RESB 8192
stack_space:
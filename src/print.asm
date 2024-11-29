BITS 32

global print
section .text

print:
    ; Function prologue
    push ebp            ; Save the base pointer
    mov ebp, esp       ; Set the base pointer to the current stack pointer

    ; Access arguments
    mov ebx, [ebp + 8] ; Get the first argument (int a)
    mov ecx, [ebp + 12]; Get the second argument (int b)
    mov edx, [ebp + 16]; Get the third argument (char c)

    ; Function body

    ;calculate video memory location of coordinate (a,b) in VGA-text mode and move it to ebx
    imul ecx, 80
    add ecx, ebx
    imul ecx, 2
    add ecx, 0xb8000
    
    mov eax, edx        ;character which is in edx moved to eax
    mov ah, 0x0f        ;font
    
    mov [ecx], ax       ;video memory physical location was stored in ecx

    ; Function epilogue
    mov esp, ebp    ; Restore the stack pointer         
    pop ebp         ; Restore the base pointer

    ret             ; Return from the function

section .data

val80: db 80
val2: db 2
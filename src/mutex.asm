BITS 32

global mutex_spinlock
global mutex_unlock

;todo syscall för sleep varje spin?

mutex_spinlock:
    enter 0,0
    mov al, 1
    mov ebx,[ebp+8]
test_lock:
    xchg al, [ebx]
    test al, al 
    jnz test_lock
    leave
    ret

mutex_unlock:
    enter 0,0
    mov al, 0
    mov ebx,[ebp+8]
    mov [ebx], al
    leave
    ret
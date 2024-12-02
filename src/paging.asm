BITS 32

global kernelPageDir
section .align4KB

kernelPageDir:
    dd 1

pagedirs:

RESB 4096*128

pagetables:

RESB 4096*128

;stuff
;align med linker script


.code

public __disable_wp

__disable_wp proc
    cli
    mov rax, cr0
    and rax, 0FFFEFFFFh
    mov cr0, rax
    sti                        
    ret
__disable_wp endp


public __enable_wp

__enable_wp proc
    cli
    mov rax, cr0
    or  rax, 00010000h
    mov cr0, rax
    sti
    ret
__enable_wp endp

END
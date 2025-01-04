.code

public ExitBootServicesWrapper

extern RetExitBootServices : qword
extern ExitBootServicesHook : proc

ExitBootServicesWrapper proc
    mov rax, [rsp]
    mov RetExitBootServices, rax
    jmp ExitBootServicesHook
ExitBootServicesWrapper endp

END
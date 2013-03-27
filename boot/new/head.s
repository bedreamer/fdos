org 07c00h

[SECTION .s16]
[BITS 16]
    jmp                 0x9000:0x0000
    nop
times 497-($-$$) db 0
setup_sects             db      2048-512
root_flags              dw      0
syssize                 dd      0
ram_size                dw      0
vid_mode                dw      0
root_dev                dw      0
boot_flag               dw      0xAA55


;setup.s
;	jialianglije@126.com
;	bedreamer@163.com
[SECTION .s16]
[BITS 16]
org 0x90200
_setup:
    jmp                 loader_start_wrap
header                  dd   "HdrS" ;魔数标签，其值为"HdrS"
version                 dw   0
realmode_swtch          dd   0
start_sys_seg           dw   0
kernel_version          dw   0
type_of_loader          db   0
loadflags               db   0
setup_move_size         dw   0
code32_start            dd      0x00100000
ramdisk_image           dd   0
ramdisk_size            dd   0
bootsect_kludge         dd   0
heap_end_ptr            dw   0
ext_loader_ver          db   0
ext_loader_type         db   0
cmd_line_ptr            dd   0
ramdisk_max             dd   0
kernel_alignment        dd   0
relocatable_kernel      db   0
min_alignment           db   0
pad3                    dw   0
cmdline_size            dd   0
hardware_subarch        dd   0
hardware_subarch_data   dq   0
payload_offset          dd   0
payload_length          dd   0
setup_data              dq   0
pref_address            dq   0
init_size               dd   0
loader_start_wrap:
    jmp     loader_start
DA_32       EQU	4000h
DA_LIMIT_4K EQU	8000h
DA_DRW      EQU	92h
DA_CR       EQU	9Ah
%macro Descriptor 3
	dw	%2 & 0FFFFh
	dw	%1 & 0FFFFh
	db	(%1 >> 16) & 0FFh
	dw	((%2 >> 8) & 0F00h) | (%3 & 0F0FFh)
	db	(%1 >> 24) & 0FFh
%endmacro
[SECTION .gdt]
GDT_BASE: Descriptor    0,     0   , 0
GDT_CODE: Descriptor    0, 0FFFFFh , DA_CR |DA_32|DA_LIMIT_4K
GDT_DATA: Descriptor    0, 0FFFFFh , DA_DRW|DA_32|DA_LIMIT_4K

GDT_LEN equ $ - GDT_BASE
GDT_PTR dw GDT_LEN
gdt_ptr dd 0x9000*0x10+GDT_BASE

GDT_C   equ GDT_CODE - GDT_BASE
GDT_D   equ GDT_DATA - GDT_BASE

loader_start:
	lgdt 	[GDT_PTR]
    mov     edx, [GDT_PTR]
	cli
	in 		al, 92h
	or 		al, 00000010b
	out 	92h, al
	mov 	eax, cr0
	or 		eax, 1
	mov 	cr0, eax
    mov     ecx, 0xFFFFFFFF
    jmp     $
	jmp 	dword 0x08:0x00100000 
; bzImage kernel code been loaded at 0x00100000 by grub.
times 2048-(2568-2048)-($-$$)        db      0	; do not modify there

; start.s
; bedreamer@163.com
global _start, kstacktop, _kstack, _kintstack
global kintstacktop, _kexpstack, kexpstacktop
global __inb, __inw, __indw
global __outb, __outw, __outdw
global __divide_error, __single_step_exception
global __overflow, __bounds_check, __inval_opcode
global __double_fault, __copr_seg_overrun
global __stack_exception, __general_protection
global __copr_error, __inval_tss, __segment_not_present
global __copr_not_available, __nmi, __breakpoint_exception
global __clock_hwint00, __keyboard_hwint01
global __second_serial_hwint03, __first_serial_hwint04
global __floppy_hwint06, __printer_hwint07
global __realtime_clock_hwint08, __cascade_hwint02
global __irq_2_redirected_hwint09, __irq_10_hwint10
global __irq_11_hwint11, __XT_winchester_hwint05
global __PS_2_mourse_hwint12, __FPU_exception_hwint13
global __IDE0_hwint14, __IDE1_hwint15, __page_fault
global __asm_sys_call
extern x86_irq_handle, x86_exp_handle
extern x86_main, doshift, gdt_ptr, idt_ptr

[SECTION .bss]
_kstack:
    resb    1024 * 2
kstacktop:

[SECTION .bss]
_kintstack:
    resb    1024 * 2
kintstacktop:

[SECTION .bss]
_kexpstack:
    resb    1024 * 2
kexpstacktop:

[section .text]
[BITS 32]
_start:
    call    doshift
    lgdt    [gdt_ptr]
    jmp     dword 0x08:.1
.1:
    mov     esp, kstacktop
    mov     ax, 0x10
    mov     ds, ax
    mov     es, ax
    mov     fs, ax
    mov     gs, ax
    mov     ss, ax
    lidt    [idt_ptr]
    jmp     x86_main

__inb:
    push    ebp
    mov    	ebp,esp
    push    edx
    xor    	eax,eax
    xor    	edx,edx
    mov    	dx,word [ebp+8]
    in    	al,dx
    pop    	edx
    leave
    ret

__inw:
    push    ebp
    mov    	ebp,esp
    push    edx
    xor    	eax,eax
    mov    	dx,word [ebp+8]
    in    	ax,dx
    pop    	edx
    leave
    ret

__indw:
    push    ebp
    mov    	ebp,esp
    push    edx
    xor    	eax,eax
    mov    	dx,word [ebp+8]
    in    	eax,dx
    pop    	edx
    leave
    ret

__outb:
	push	ebp
	mov		ebp,esp
	push	edx
	xor		eax,eax
	xor		edx,edx
	mov		al,byte [ebp+8]
	mov		dx,word [ebp+12]
	out		dx,al
	pop		edx
	leave
	ret

__outw:
	push	ebp
	mov		ebp,esp
	push	edx
	xor		eax,eax
	xor		edx,edx
	mov		ax,word [ebp+8]
	mov		dx,word [ebp+12]
	out		dx,ax
	pop		edx
	leave
	ret

__outdw:
	push	ebp
	mov		ebp,esp
	push	edx
	xor		eax,eax
	xor		edx,edx
	mov		eax,dword [ebp+8]
	mov		dx,word [ebp+12]
	out		dx,eax
	pop		edx
	leave
	ret

[SECTION .text]
__divide_error:
__single_step_exception:
__nmi:
__breakpoint_exception:
__overflow:
__bounds_check:
__inval_opcode:
__copr_not_available:
__double_fault:
__copr_seg_overrun:
__inval_tss:
__segment_not_present:
__stack_exception:
__general_protection:
__page_fault:
__copr_error:
    cli
    hlt

; some case could set cpu to interrupt mode
; user mode  --->  int mode ( easist  )
; core mode  --->  int mode ( easy    )
; int  mode  --->  int mode ( complexe)
; sys  mode  --->  int mode ( normal  )
%macro DEAL_INT 1
    cli
    push  %1
    call  x86_irq_handle
    add   esp, 4
    iret
%endmacro

[SECTION .text]
__clock_hwint00:
    DEAL_INT 0x20
__keyboard_hwint01:
    DEAL_INT 0x21
__cascade_hwint02:
    DEAL_INT 0x22
__second_serial_hwint03:
    DEAL_INT 0x23
__first_serial_hwint04:
    DEAL_INT 0x24
__XT_winchester_hwint05:
    DEAL_INT 0x25
__floppy_hwint06:
    DEAL_INT 0x26
__printer_hwint07:
    DEAL_INT 0x27
__realtime_clock_hwint08:
    DEAL_INT 0x28
__irq_2_redirected_hwint09:
    DEAL_INT 0x29
__irq_10_hwint10:
    DEAL_INT 0x2A
__irq_11_hwint11:
    DEAL_INT 0x2B
__PS_2_mourse_hwint12:
    DEAL_INT 0x2C
__FPU_exception_hwint13:
    DEAL_INT 0x2D
__IDE0_hwint14:
    DEAL_INT 0x2E
__IDE1_hwint15:
    DEAL_INT 0x2F

[SECTION .text]
__asm_sys_call:


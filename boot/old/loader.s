;loader.s
; +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
; Copyright LiJie.<jilianglijie@126.com>
; 2011-10-30 12:22:27
; 2012-01-12 11:36:58
; Monday, June 11, 2012 10:20:45 CST 
;	+ 修改了内核文件的加载方式，在loader中加载两个文件，一个是KSYSTEM，具体干什么还没想清楚，后面
;	+ 应该能用到，一个是KFOOL内核文件
;	+ KSYSTEM =====>  2000:0000 ~ 5000:FFFF		256K
;		+ KSYSTEM是一个tar压缩文件，包含一些内核在启动时需要的驱动等模块文件.
;	+ KFOOL   =====>  6000:0000 ~ 8000:FFFF		192K
; +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
%include "segdef.i"
%include "protectmode.i"
org OFFSET_LDR_FILE_SECTOR	;
jmp loader_start			; 9000:0000h
%include "floppy.s"
%include "string.s"

[SECTION .s16]
[BITS 16]
loader_start:
	mov 	ax,0x1000
	mov 	ss,ax
	mov 	sp,0xFFFE
	mov 	ax,cs
	mov 	ds,ax
	mov 	es,ax
%if 0 ; don't load ksystem.tar this time.
	push	8
	push	sysfilename
	call	_asm_findfile
	add		sp,4
	cmp		ax,0
	je		.error1
	push	bp
	mov		bp,sp
	sub		sp,8
	mov		word [bp-2],0		; int i=0;
	mov		word [bp-4],ax		; int secnum=ax;
	mov		word [bp-6],0x0000	; int offset=0x0000;
	mov		word [bp-8],0x2000	; int seg=0x2000;
.lj:
	cmp		word [bp-2],8
	jge		.break				; if i > 0
	push	2
	push	64
	push	word [bp-4]
	push	word [bp-6]
	push	word [bp-8]
	call	readwritesector
	add		sp,2*5
	cmp		ax,1
	jne		.error2
	mov		ax,word [bp-2]
	and		ax,0x0001
	cmp		ax,0x0001
	jne		.1					; if 0x0002 == 0x0002&i
	add		word [bp-8],0x1000	; seg+=0x1000
	mov		word [bp-6],0x0000	; offset=0
	jmp		.2
.1:								; if 0x0002 != 0x0002&i
	mov		word [bp-6],0x8000	; offset=0x8000
.2:
	add		word [bp-4],64
	inc		word [bp-2]
	mov		word [bp-32],0
;.de
;	cmp		word [bp-32],0
;	jne		.test
;	jmp		.de
;.test:
	jmp		.lj
.break:
	add		sp,8
	mov		sp,bp
	pop		bp
%endif
	;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
	; 系统文件加载完成，寻找核心文件并将其加载到内存
	push	8
	push	corefilename
	call	_asm_findfile
	add		sp,4
	cmp		ax,0
	je		.error3
	push	bp
	mov		bp,sp
	sub		sp,8
	mov		word [bp-2],0		; int i=0;
	mov		word [bp-4],ax		; int secnum=ax;
	mov		word [bp-6],0x0000	; int offset=0x0000;
	mov		word [bp-8],0x6000	; int seg=0x6000;
.lj2:
	cmp		word [bp-2],6
	jge		.break2				; if i > 0
	push	2
	push	64
	push	word [bp-4]
	push	word [bp-6]
	push	word [bp-8]
	call	readwritesector
	add		sp,2*5
	cmp		ax,1
	jne		.error4
	mov		ax,word [bp-2]
	and		ax,0x0001
	cmp		ax,0x0001
	jne		.12					; if 0x0002 == 0x0002&i
	add		word [bp-8],0x1000	; seg+=0x1000
	mov		word [bp-6],0x0000	; offset=0
	jmp		.22
.12:							; if 0x0002 != 0x0002&i
	mov		word [bp-6],0x8000	; offset=0x8000
.22:
	add		word [bp-4],64
	inc		word [bp-2]
	jmp		.lj2
.break2:
	add		sp,8
	mov		sp,bp
	pop		bp
	jmp		.ready
.error1:
	mov		ax,1
	hlt
.error2:
	mov		ax,2
	hlt
.error3:
	mov		ax,3
	hlt
.error4:
	mov		ax,4
	hlt
	jmp		$
.ready:
; 准备进入保护模式
	lgdt 	[GDT_PTR]
	cli
	in 		al,92h
	or 		al,00000010b
	out 	92h,al
	mov 	eax,cr0
	or 		eax,1
	mov 	cr0,eax
	jmp 	dword SelectorCode32:(SEG_LDR_FILE_SECTOR * 0x10 + CODE32_START)
; end 16 bit code.

sysfilename 	db "KSYSTEM "		; 打包的文件
corefilename	db "KFOOL   " 		; 核心文件名
filenamelen		dw 8

;=======================================================================================
; start 32 bit code.
[SECTION .s32]
ALIGN 32
[BITS 32]
CODE32_START:
	mov		ax,SelectorVideo
	mov		gs,ax
	mov		ax,SelectorNormal
	mov		ds,ax
	mov		es,ax
	mov		fs,ax
	mov		ax,SelectorStack
	mov		ss,ax
	mov		esp,0x2FFFE

	call 	ReplaceKernel
	mov		eax,[SEG_KRL_FILE_SECTOR*16+0x18]		; eax = pELF -> e_entry

	mov		dword [SEG_LDR_FILE_SECTOR*16+KernelEntry],eax
	jmp		dword [cs:SEG_LDR_FILE_SECTOR*16+KernelEntry]
;	jmp		SelectorCode32:0x10610
	jmp 	$
Code32Len equ $ - CODE32_START

; now replace the kernel file.
ReplaceKernel:
	xor		esi,esi
	mov		cx,word [SEG_KRL_FILE_SECTOR*16+0x2c]	; ecx = pELFHdr -> e_phnum
	movzx	ecx,cx
	mov		esi,[SEG_KRL_FILE_SECTOR*16+0x1c]		; esi = pELFHdr -> e_phoff
	add		esi ,SEG_KRL_FILE_SECTOR*16				; esi = KernelBase * 16 + pELFHdr -> e_phoff
.docopy:
	mov		eax,[esi+0]
	cmp		eax,0
	jz		.NoAction
	push	dword [esi + 0x10]				; size
	mov		eax,[esi+0x04]					;
	add		eax,SEG_KRL_FILE_SECTOR*16
	push	eax								; src
	push	dword [esi+0x08]				; dst
	call	MemCpy							;
	add		esp,12							;
.NoAction:	
	add		esi,0x20						; esi += pELFHdr -> e_phentsize
	dec		ecx
	jnz		.docopy
	ret

; void* MemCpy(void* es:pDest, void* ds:pSrc, int iSize);
; ------------------------------------------------------------------------
MemCpy:
	push	ebp
	mov		ebp, esp
	push	esi
	push	edi
	push	ecx
	mov		edi, [ebp + 8]		; Destination
	mov		esi, [ebp + 12]		; Source
	mov		ecx, [ebp + 16]		; Counter
.1:
	cmp		ecx, 0				;
	jz		.2					;
	mov		al, [ds:esi]		; 
	inc		esi					; 

	mov		byte [es:edi], al	; 
	inc		edi					; 
	dec		ecx					; 
	jmp		.1					; 
.2:
	mov		eax, [ebp + 8]		;
	pop		ecx
	pop		edi
	pop		esi
	mov		esp, ebp
	pop		ebp
	ret		
; MemCpy-------------------------------------------------------------

;#####################################################################################################

[SECTION .data]
KernelEntry:	dw	0

[SECTION .gdt]
;							SegBaseAddr(32 bit)	SegLimit(20 bit)		Attribute(12 bit)
GDT_BASE: Descriptor 				0	,		  0		,		0
GDT_CODE32: Descriptor				0	,	 0FFFFFh	,	DA_CR |DA_32|DA_LIMIT_4K
GDT_NORMAL: Descriptor				0	,	 0FFFFFh	,	DA_DRW|DA_32|DA_LIMIT_4K
GDT_VIDEO: Descriptor			0B8000h	,	 00FFFFh	,	DA_DRW|DA_DPL3
GDT_STACK: Descriptor				0	,	 0FFFFFh	,	DA_DRW|DA_32|DA_LIMIT_4K
GDT_CODE32_USER: Descriptor			0	,	 0FFFFFh	,	DA_CR |DA_32|DA_LIMIT_4K|DA_DPL3
GDT_DATE32_USER: Descriptor			0	,	 0FFFFFh	,	DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3
GDT_STACK32_USER: Descriptor		0	,	 0FFFFFh	,	DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3

GDT_LEN equ $ - GDT_BASE
GDT_PTR dw GDT_LEN
	    dd SEG_LDR_FILE_SECTOR*0x10 + GDT_BASE

SelectorCode32	equ	GDT_CODE32 - GDT_BASE
SelectorNormal 	equ	GDT_NORMAL - GDT_BASE
SelectorVideo	equ	GDT_VIDEO  - GDT_BASE + SA_RPL3
SelectorStack	equ	GDT_STACK  - GDT_BASE

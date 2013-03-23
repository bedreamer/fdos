;boot.s
;	jialianglije@126.com
;	bedreamer@163.com

org 07c00h

jmp 		_boot_start

%include "fat12.i"
%include "segdef.i"
%include "string.s"
%include "floppy.s"

_boot_start:
	mov		ax,0x2000
	mov 	ss,ax
	mov 	sp,0xfffE
	mov 	ax,0x0000 
	mov 	ds,ax
	mov 	es,ax
	sub 	sp,2
	call 	reset
	add 	sp,2

	mov 	ax,0
	mov 	ds,ax
	push 	word 8
	push 	word LoaderName	; ds == 0x0000
	call 	_asm_findfile
	add 	sp,4
	cmp 	ax,0
	je 		_noloader
;_asm_readwritefloopysector(word address,word offset,word sectornum,word sectorcnt);
	push 	bp
	push 	es
	push 	word read_floopy
	push 	word 0x10
	push 	word ax
	push 	word OFFSET_LDR_FILE_SECTOR
	push 	word SEG_LDR_FILE_SECTOR
	call 	readwritesector
	add 	sp,10
	pop 	es
	pop 	bp
;;;;;;;;;;;;;;;;;;;;;;
	jmp 	SEG_LDR_FILE_SECTOR:OFFSET_LDR_FILE_SECTOR
_noloader:
	mov 	ax,cs
	mov 	es,ax
	mov 	ah,19
	mov 	bp,LoaderLoadFaile
	mov 	cx,word [LoaderLoadFailelen]
	mov 	dx,0x0000
	mov 	al,1
	mov 	bh,0
	mov	 	bl,0x0c
	int 	0x10
	hlt
	jmp	 	$

LoaderLoadFaile db "unable find loader."
LoaderLoadFailelen dw LoaderLoadFailelen-LoaderLoadFaile
LoaderName db 'FOOLLDR '

times 510-($-$$) db 0	; do not modify there
dw 0xaa55				; do not modify there
























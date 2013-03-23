;output.s

%ifndef BOOT___
;设置当前光标位置
;_asm_setcursorpos(word16 _x,word16 _y)
_asm_setcursorpos:
	push	bp
	mov	bp,sp
	pusha
	mov	ah,2
	mov	bh,0
	mov	dh,byte [bp+6]
	cmp	dh,025
	jae	_SETCURSOR_INVALID_PARAM
	mov	dl,byte [bp+4]
	cmp	dl,080
	jae	_SETCURSOR_INVALID_PARAM
	int	10h
_SETCURSOR_INVALID_PARAM:
	popa
	mov	sp,bp
	pop	bp
	ret

;显示一个字符
;word _asm_outputchar(word16 ch,word16 _color)
_asm_outputchar:
	push bp
	mov bp,sp
	pusha
	mov al,byte [bp+4]
	mov ah,14
	mov bl,byte [bp+6]
	mov bh,0
	int 10h
	popa
	mov sp,bp
	pop bp
	ret

; 显示一个空格符
; void _asm_outputspace()
_asm_outputspace:
	pusha
	mov ah,14
	mov al,' '
	mov bx,0x000F
	int 10h
	popa
	ret

;显示一个字符串
;void _asm_outputstring(word16 sct,word16 offset,word16 len,word16 color,word16 x,word16 y)
_asm_outputstring:
	push si
	mov si,sp
	push es
	push bp
	pusha
	mov es,word [ss:si+4]
	mov cx,word [ss:si+8]
	mov dh,byte [ss:si+14]
	mov dl,byte [ss:si+12]
	mov bh,0
	mov al,1
	mov bl,byte [ss:si+10]
	mov bp,word [ss:si+6]
	mov ah,0x13
	int 10h
	popa
	pop bp
	pop es
	mov sp,si
	pop si
	ret

; 输出回车符
; _asm_output_return()
returnstr: db 10
_asm_output_return:
	pusha
	push word 0
	push word 13
	call _asm_outputchar
	add sp,4
	push word 0
	push word 10
	call _asm_outputchar
	add sp,4
	popa
	ret

; 以字符模式在当前光标位置显示一个16进制数
; 最大值为0xffff 65535
;_asm_outputdigit16(word16 )
hexcode:
	db "0123456789ABCDEF"
_asm_outputdigit16:
	push	bp
	mov	bp,sp
	pusha
;====================== 3	
	xor	eax,eax
	mov	eax,0	
	mov	ax,[bp+4]
	mov	si,hexcode
	shr	ax,3*4
	and	eax,0x0000000F
	add	si,ax
	xor	ax,ax
	mov	al,byte [cs:si]
	push	word 0x0f
	push	ax
	call _asm_outputchar
	add	sp,4
;======================= 2
	xor	eax,eax
	mov	eax,0	
	mov	ax,[bp+4]
	mov	si,hexcode
	shr	ax,2*4
	and	eax,0x0000000F
	add	si,ax
	xor	ax,ax
	mov	al,byte [cs:si]
	push	word 0x0f
	push	ax
	call _asm_outputchar
	add	sp,4
;====================== 1
	xor	eax,eax
	mov	eax,0	
	mov	ax,[bp+4]
	mov	si,hexcode
	shr	ax,4
	and	eax,0x0000000F
	add	si,ax
	xor	ax,ax
	mov	al,byte [cs:si]
	push	word 0x0f
	push	ax
	call _asm_outputchar
	add	sp,4
;======================= 0
	xor	eax,eax
	mov	eax,0	
	mov	ax,[bp+4]
	mov	si,hexcode
	and	eax,0x0000000F
	add	si,ax
	xor	ax,ax
	mov	al,byte [cs:si]
	push	word 0x0f
	push	ax
	call _asm_outputchar
	add	sp,4
;==========end==========			
	popa
	mov	sp,bp
	pop	bp
	ret

%endif
















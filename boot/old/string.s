; string.s

%if 0
; 比较两个字符串的大小,相同返回1不相同返回0
;word _asm_str_cmp(ds:offset,es:offset,word len)
;				[bp+4]	[bp+6]	[bp+8] 
_asm_str_cmp:
	push 	bp
	mov 	bp,sp
	push 	si
	push 	di
	push 	cx
	mov 	di,word [bp+4]	; string1
	mov 	si,word [bp+6]	; string2
	mov 	cx,word [bp+8]	; length
.strcmp:
	mov 	ah,byte [ds:si]
	mov 	al,byte [es:di]
	cmp 	ah,al
	jne 	.noteque
	inc 	si
	inc 	di
	loop 	.strcmp
	mov 	ax,1
	jmp 	.endstrcmp
.noteque:
	mov 	ax,0
.endstrcmp:
	pop 	cx
	pop 	di
	pop 	si
	mov 	sp,bp
	pop 	bp
	ret
%endif

%if 1
; 比较两个字符串的大小,相同返回1不相同返回0
;
;word _asm_str_cmp(word section,word offset,word section,word offset,word len)
;						[bp+4]		[bp+6]		[bp+8] 		[bp+10]		[bp+12]
_asm_str_cmp:
	push 	bp
	mov 	bp,sp
	push	ds
	push	es	
	push	si
	push	di
	push	dx
		
	mov		dx,[bp+4]
	mov		ds,dx		
	mov		si,[bp+6]
	mov		dx,[bp+8]
	mov		es,dx
	mov		di,[bp+10]
		
	xor		dx,dx
	jmp 	.jmpuhere1
.jumphere3:
	mov		al,byte [ds:si]
	mov		dh,byte [es:di]

%if 0
	pusha
	mov		ah,14
	mov		al,byte [ds:si]
	mov		bl,0x0c
	mov		bh,0
	int		10h
	mov		ah,14
	mov		al,byte [es:di]
	mov		bl,0x0c
	mov		bh,0
	int		10h
	popa
%endif
	
	cmp 	dh,al
	jz 		.jumphere2
	mov 	ax,0x0
	jmp 	.endhere
.jumphere2:
	inc 	si
	inc 	di
	dec 	word [bp+0xc]
.jmpuhere1:
	cmp 	word [bp+0xc],0
	jg 		.jumphere3
	mov 	ax,0x1
.endhere:
	pop		dx
	pop		di
	pop		si
	pop		es
	pop		ds
	mov		sp,bp
	pop 	bp
	ret
%endif	
	

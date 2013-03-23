; floopy.s
; Copyright �1�70�1�78 2011 LiJie. All rights reserved.
;�1�7�1�7�Մ1�7�1�7�1�7�0�9�1�7�1�7�1�7�1�7�1�8�1�7�1�7�1�7
read_floopy equ 02h
write_floopy equ 03h
floopopbufsection equ 0x1000
floopopbufoffset  equ 0x0000

%ifdef OBJ_LIB
global readwritesector
global reset
extern _asm_outputchar
%endif ;OBJ_LIB

;16bit
; �1�7�1�7�Մ1�7�1�7�1�7�1�7
; readwritesector(word sction,word offset,word sectornum,word sectorcnt,word opp)
;push sectornum
;push offset
;push address
;�1�7�1�7�1�7�1�7�1�7�0�6�1�7�1�7�1�7�1�7�0�3�1�7
; ah = 02h
; ch = �1�7�1�7�1�7�1�7�1�7�1�7 �1�7�1�7�1�7�0�7�1�7�0�0�1�7
; dh = �1�7�1�7�0�5�1�7�1�7
; es:bx�1�7�1�7�1�7�1�9�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�7
; dl = 0h
; al = �0�8�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
; cl = �1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
; 		        		    |---> �1�7�1�7�1�7�1�7�1�7�1�7ch = �1�7�1�7 >> 1
;	        	|---�1�7�1�7	al--|		
;  �1�7�1�7�1�7�1�7�1�7�1�7 / 18 --|	        |---> �1�7�1�7�0�5�1�7�1�7dh = �1�7�1�7 & 1
;	        	|---�1�7�1�7�1�7�1�7	ah---> �1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7cl = �1�7�1�7�1�7�1�7 + 1
readwritesector:
	push 	bp
	mov 	bp,sp
	sub 	sp,2
	push 	bp
	push 	es
	pusha
	mov 	ax,[bp+8] ;�1�7�1�7�1�7�1�7�1�7�1�7
	mov 	dl,012h;18�1�7�1�7�1�7�1�7�1�7�1�7
	div 	dl  ;�1�7�0�8�1�7�1�7�1�7�1�7�1�7AL�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7AH�1�7�1�7
	mov 	cl,ah
	inc 	cl ;�1�7�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7
	mov 	ch,al
	shr 	ch,1;�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7�1�7
	and 	al,1
	mov 	dh,al ;�1�7�1�7�1�7�0�0�1�7�0�5�1�7�1�7
	mov 	es,[bp+4] ;�1�7�ń1�7�0�7 used with di
	mov 	bx,[bp+6] ;�0�1�1�7�0�1�1�7�0�7
	mov 	ah,02h
	mov 	dl,0h
RepeatRead: ;�1�7�1�7�0�0�1�7�1�7�1�7�1�7�1�7�0�3�1�7�1�7�1�7�1�7�1�7�1�7�1�7�0�9�1�7�0�0
	cmp 	byte [bp-2],0x21	;�1�7�1�2�1�732�1�7�1�7
	je 	READ_FLOOPY_ERROR
	inc 	byte [bp-2]
%if 0
	push 	word 0x88
	push 	word '*'
	call 	_asm_outputchar
	add 	sp,4
%endif
	mov 	ah,[bp+12]
	mov 	al,[bp+10]
	int 	13h
	jc 	RepeatRead
	jmp 	READ_FLOOPY_NOERROR
READ_FLOOPY_ERROR:
	popa
	mov 	ax,0
	jmp 	NODE______
READ_FLOOPY_NOERROR:
	popa
	mov 	ax,1
NODE______:
	pop 	es
	pop 	bp
	mov 	sp,bp
	pop 	bp
	ret

;reset(void)
reset:
	pusha
	mov 	byte ah,00h
	mov 	byte dl,0h	
	int 	13h
	popa
	ret

; word _asm_findfile(ds:offset,word len)
_asm_findfile:
	push 	bp
	mov 	bp,sp
	sub 	sp,8
	mov 	word [bp-2],0x0080
	mov 	word [bp-4],floopopbufoffset
	push 	es
	push 	ds
	mov		ax,floopopbufsection
	mov		es,ax
	push 	word read_floopy
	push 	word 0x02
	push 	word 0x13
	push 	word floopopbufoffset
	push 	word floopopbufsection
	call 	readwritesector
	add 	sp,10
.findnexttable:
	cmp 	word [bp-2],0
	je 		.notfound
	push 	word [bp+6]
	push	word [bp+4]
	push	cs
	push	word [bp-4]
	push	es
	call 	_asm_str_cmp
	add 	sp,10
	add 	word [bp-4],0x0020				; offset += 32
	dec 	word [bp-2]						; tablecnt --
	cmp		ax,0
	je		.findnexttable
.foundit:
	mov 	di,word [bp-4]
	sub		di,0x20
	add 	di,0x1A
	mov 	ax,word [es:di]	
	add 	ax,31
	mov		dx,word [es:di]
	jmp 	.foundend
.notfound:
	mov 	ax,0
.foundend:
	pop		ds
	pop 	es
	mov 	sp,bp
	pop 	bp
	ret

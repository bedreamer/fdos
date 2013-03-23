; floopy.s
; Copyright �0�8 2011 LiJie. All rights reserved.
;��д���̵����غ���
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
; ��д����
; readwritesector(word sction,word offset,word sectornum,word sectorcnt,word opp)
;push sectornum
;push offset
;push address
;�����Ĵ����ֳ�
; ah = 02h
; ch = ������ ���ŵ�ţ�
; dh = ��ͷ��
; es:bx���ݻ�������ַ
; dl = 0h
; al = Ҫ���������
; cl = ��ʼ������
; 		        		    |---> ������ch = �� >> 1
;	        	|---��	al--|		
;  ������ / 18 --|	        |---> ��ͷ��dh = �� & 1
;	        	|---����	ah---> ��ʼ������cl = ���� + 1
readwritesector:
	push 	bp
	mov 	bp,sp
	sub 	sp,2
	push 	bp
	push 	es
	pusha
	mov 	ax,[bp+8] ;������
	mov 	dl,012h;18������
	div 	dl  ;�̴�����AL������������AH��
	mov 	cl,ah
	inc 	cl ;������ʼ������
	mov 	ch,al
	shr 	ch,1;����������
	and 	al,1
	mov 	dh,al ;���ô�ͷ��
	mov 	es,[bp+4] ;�ε�ַ used with di
	mov 	bx,[bp+6] ;ƫ�Ƶ�ַ
	mov 	ah,02h
	mov 	dl,0h
RepeatRead: ;��ȡ�����͵�������¶�ȡ
	cmp 	byte [bp-2],0x21	;�ݴ�32��
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

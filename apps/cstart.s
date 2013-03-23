; C start-up code.
global _start
extern exit
extern main

_start:
	push	ebp
	mov		ebp,esp

	push	eax	; argv
	push	ecx	; argc
	call	main
	add		esp,8

	push	eax
	call	exit

	hlt
	leave	;; can not be here.
	ret
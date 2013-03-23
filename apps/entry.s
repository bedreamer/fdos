;int syscall(int eax,int ebx,int ecx,int edx,int edi,int esi);
global syscall

syscall:
	push	ebp
	mov		ebp,esp
	mov		eax,dword [ebp+0x08]
	mov		ebx,dword [ebp+0x0C]
	mov		ecx,dword [ebp+0x10]
	mov		edx,dword [ebp+0x14]
	mov		edi,dword [ebp+0x18]
	mov		esi,dword [ebp+0x1C]
	int		0x80
	leave
	ret

;--------------------------------------------------------------------------------------
; segment define
; Email: jilianglijie@126.com
;		 bedreamer@163.com
; segdef.inc
;--------------------------------------------------------------------------------------
%if 0
SEG_BOOT_SECTOR				equ		0x0000		; Boot sector will be loaded here by BIOS
SEG_LDR_FILE_SECTOR			equ		0x8000		; file FOOLLDR will be loaded here by bootsector
OFFSET_LDR_FILE_SECTOR		equ		0x0000		; offset of file FOOLLDR
SEG_KRL_FILE_SECTOR			equ		0x6000		; fool's kernel file will be loaded here by loader
OFFSET_KRL_FILE_SECTOR		equ		0x0000		; offset of file fool's kernel
SEG_LRL_SECTOR				equ		0x1000		; fool's kernel will be replaced here by loader
;floopopbufsection 			equ 	0x0000		; floopy buffer
;floopopbufoffset  			equ 	0x2000		; floopy buffer
%else
SEG_BOOT_SECTOR				equ		0x0000		; Boot sector will be loaded here by BIOS
; LOADER被加载的段地址为0x9000因此LOADER最大为64K
SEG_LDR_FILE_SECTOR			equ		0x9000
; LOADER偏移为0
OFFSET_LDR_FILE_SECTOR		equ		0x0000
SEG_KRL_FILE_SECTOR			equ		0x6000		; fool's kernel file will be loaded here by loader
OFFSET_KRL_FILE_SECTOR		equ		0x0000		; offset of file fool's kernel
SEG_LRL_SECTOR				equ		0x1000		; fool's kernel will be replaced here by loader
%endif
; 这样分配后内存如下
;           范围				 占用			大小
; 0000:0000 ~ 0000:FFFF 	BIOS			64K
; 1000:0000 ~ 1000:7FFF 	读写缓冲区		32K
; 1000:8000 ~ 1000:FFFF 	LOADER堆栈		32K
; 2000:0000 ~ 8000:FFFF 	空闲				448K
;		+ 2000:0000 ~ 5000:FFFF		存放系统文件.tar		256K
;		+ 6000:0000 ~ 8000:FFFF		内核文件				192K
; 9000:0000 ~ 9000:FFFF 	LOADER代码		64K
; A000:0000 ~ F000:FFFF 	BIOS			64K
SEG_LDR_SS	equ		0x1000
SP_LDR		equ		0xFFFE




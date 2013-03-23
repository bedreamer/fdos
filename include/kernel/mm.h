/*
 *	mm.h
 *	bemaster@163.com
 *	Wednesday, June 13, 2012 03:16:57 CST 
 */
#ifndef _MM_
#define _MM_

#define PAGE_KER 0x00
#define PAGE_USR 0x01
#define PAGE_ROE 0x00
#define PAGE_RW  0x01

#define INVALID_PAGE  0x00000000UL
void *kmalloc(size_t, uint);
void  kfree(void *, size_t);
void *vmalloc(size_t, uint);
void  vmfree(void *, size_t);

#endif /*_MM_*/

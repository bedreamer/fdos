/*
 *	stddef.h
 *	bedreamer@163.com
 */
#ifndef _STDDEF_
#define _STDDEF_

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;
typedef unsigned int size_t;
typedef unsigned short wchar_t;

#define NULL ((void*)0)
#define isdigit(ch) (ch>='0'&&ch<='9'?1:0)
#define ishex(ch) (((ch>='0'&&ch<='9')||(ch>='a'&&ch<='f')||(ch>='A'&&ch<='F'))?1:0)
#define isxdigit ishex
#define islower(ch) (ch>='a'&&ch<='z'?1:0)
#define isupper(ch) (ch>='A'&&ch<='Z'?1:0)

#define INT_MAX	2147483647
#define INT_MIN (-INT_MAX-1)
#define LONGLONG_MAX 9223372036854775807
#define LONGLONG_MIN (-LONGLONG_MAX-1)

typedef struct
{
	char* this;
	int offset;
}var_list;
#define va_start(v,l) {\
						v.this=(char*)(&l);\
						v.this+=sizeof(int);\
						v.offset=0;\
					}
#define va_end(v)
#define va_arg(v,l)	*(l*)var_arg(&v,sizeof(l))

#endif /* _STDDEF_ */


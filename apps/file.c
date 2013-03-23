/*
 * file.c
 * bedreamer@163.com
 * 关于stdin,stdout,stderr的说明:
 * stdin,stdout有独立的缓冲区，stderr没有缓冲区，
 * 标准输入以字节输入，对不同的输入函数采用不同的输入结束判断例如getchar和scanf的
 * 判别条件分别为输入一个可打印字符或回车键和回车键或空格键中断。
 */
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <foolstd.h>

static FILE fslot[256]={{.io_index=-1}};
FILE *stdin=NULL,*stdout=NULL,*stderr=NULL;
unsigned short stdin_buf[1024];
unsigned short stdout_buf[25*80];

FILE* fopen(const char *filaneme,const char *mode)
{
	return NULL;
}

int fread(char *ptr,size_t bn,size_t bs,FILE *fp)
{
	return 0;
}

/*从标准输入设备获得一个字符，包括，退格，回车，空格
 * 不缓冲直接返回.
 */
int getchar(void)
{
	int ch;

	if (NULL==stdin)
	{
		int fd = open("/dev/stdin",MODE_READWRITE);
		if (-1==fd) return 0;

		stdin = & fslot[fd];
		stdin->io_base = stdin_buf;
		stdin->io_index = _stdin;
		stdin->io_ptr = stdin_buf;
		stdin->io_size = 1024;
		stdin->io_t_size = 0;

	}
	if (stdin->io_t_size >= stdin->io_size) 
		return -1;

	read(_stdin,(char*)&ch,1);
	ch &= 0x000000FF;

	return ch;
}

/*输入一个字符串
 */
int gets(char *ptr)
{
	int ch=0,len=0;
	char *p;

	for (;;len++)
	{
		ch = getchar();
		putchar((char)ch);
		if (3 == ch && len > 0)  /*收到回车*/
		{
			int i;

			p = stdin->io_base;
			for (i=0;i<len;i++)
			{
				*ptr ++ = p[i];
				p[i] = '\0';
			}
			memset(stdin->io_base,0,1024);
			stdin->io_ptr = stdin->io_base;
			stdin->io_t_size = 0;
			return len;
		}
		if ((' ' <= ch && '~' >= ch) || '\t' == ch ){
			if (stdin->io_size > stdin->io_t_size )
			{
				*(char*)stdin->io_ptr ++ = (char)ch;
				stdin->io_t_size ++;
			}
		}
		if (4 == ch){ // 退格键
			if (stdin->io_ptr > stdin->io_base ){
				stdin->io_t_size --;
				stdin->io_ptr -= 1;
				*(char*)stdin->io_ptr = '\0';
			}
		}
	}
	return len;
}

/*从标准输入设备输入指定格式的字符串.*/
int scanf(const char *fmt,...)
{
	return 0;
}

/*在标准输出设备上输出一个字符，若这个字符是换行符则刷新输出缓冲区
 * 否则等待下一个换行或者直到缓冲区满后刷新。
 */
int putchar(char ch)
{
	if (NULL==stdout) 
	{
		int fd = open("/dev/stdout",MODE_READWRITE);
		if (-1!=fd)
		{
			stdout = & fslot[fd];
			stdout->io_base = stdout_buf;
			stdout->io_index = _stdout;
			stdout->io_ptr = stdout_buf;
			stdout->io_size = 80*25*sizeof(unsigned short);
			stdout->io_t_size = 0;

			unsigned int i;
			unsigned short *p = stdout->io_base;

			for (i=0;i<25*80;i++,p++)
				*p= 0x0700;			// 如果不初始化颜色会导致输出颜色错误.
		}
		else return 0;
	}

	if (stdout->io_t_size>=stdout->io_size / 2)
	{ // buffer full
		write(_stdout,stdout->io_base,stdout->io_t_size);
		stdout->io_ptr = stdout->io_base;
		stdout->io_t_size = 0;
	}

	if ((' ' <= ch && '~' >= ch) || '\t' == ch ) 
	{
		unsigned short *p = stdout->io_ptr;

		*p++ = 0x0700|ch;

		stdout->io_ptr = p;

		stdout->io_t_size ++;
		if (stdout->io_t_size >= stdout->io_size / 2)
			goto refresh;
		return 1;
	}
	else if ('\n' ==ch) 
	{ // 刷新缓冲区.
		unsigned short *p = stdout->io_ptr;

		*p++ = 0x0700|ch;

		stdout->io_ptr = p;

		stdout->io_t_size ++;
	}
	else { // ignore
		return 0;
	}

refresh:
	write(_stdout,stdout->io_base,stdout->io_t_size);
	stdout->io_ptr = stdout->io_base;
	stdout->io_t_size = 0;
	return 1;
}

/* 将字符串输出到标准输出设备.
 */
unsigned int puts(const char *str)
{
	const char *p = str;
	while ('\0'!=*str)
	{
		if (0==putchar(*str++)) 
			break;
	}
	return (unsigned int)str-(unsigned int)p;
}

/* 将字符串格式化输出到标准输出设备.
 */
int printf(const char *fmt,...)
{
	var_list va;
	int len;
	char buf[1024];

	va_start(va,fmt);
	len=vsprintf(buf,fmt,va);
	buf[len]='\0';

	return puts(buf);
}

int fwrite(char *ptr,size_t bn,size_t bs,FILE *fp);
int fseek(FILE *fp,size_t offset,int base);
size_t ftell(FILE *fp);
int fclose(FILE *fp);

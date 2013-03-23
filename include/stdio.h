/*
 *	stdio.h
 *	bedreamer@163.com
 *	Tuesday, June 12, 2012 10:39:59 CST 
 */
#ifndef _STDIO_
#define _STDIO_
#define _CRT_

#	ifndef _STDDEF_
		#include <stddef.h>
#	endif // _STDDEF_

/*文件I/O描述符
 * @io_size: size of this I/O buffer.
 * @io_index: index of this file description.
 * @io_t_size: current size of this I/O buffer.
 * @io_base: base address of this buffer;
 * @io_ptr: current I/O pointer.
 */
struct IO_FILE
{
	size_t io_size;
	size_t io_index;
	size_t io_t_size;
	void *io_base;
	void *io_ptr;
};
typedef struct IO_FILE FILE;

/*standar I/O stream.*/
extern FILE *stdin;
extern FILE *stdout;
extern FILE *stderr;
#define _stdin 	(0)
#define _stdout (1)
#define _stderr (2)

#ifndef EOF
	#define EOF (-1)
#endif // EOF

extern _CRT_ FILE* fopen(const char *,const char *);

extern _CRT_ int fread(char *,size_t,size_t,FILE *);
extern _CRT_ int getchar(void);
extern _CRT_ int gets(char *);
extern _CRT_ int scanf(const char *,...);

extern _CRT_ int fwrite(char *,size_t,size_t,FILE *);
extern _CRT_ int putchar(char );
extern _CRT_ unsigned int puts(const char *);
extern _CRT_ int printf(const char *,...);

extern _CRT_ int fseek(FILE *,size_t,int);
extern _CRT_ size_t ftell(FILE *);
extern _CRT_ int fclose(FILE *);

extern _CRT_ int flush(FILE *);

/*键映射*/
#define VK_RETURN		13
#define VK_TAB			9
#define VK_BACK			8

#endif /*_STDIO_*/

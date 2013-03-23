/*
 *	stdlib.h
 *	bedreamer@163.com
 */
#ifndef _STDLIB_INCLUDE_
#define _STDLIB_INCLUDE_

#ifndef _STDDEF_
	#include <stddef.h>
#endif /* _STDDEF_ */

#ifndef _STD_STRING_INCLUDE_H_
	#include <string.h>
#endif /* _STD_STRING_INCLUDE_H_ */

extern int atoi(const char * strz);
extern int itoa(char *buf,int dec);
extern int itox(char *buf,int dec);
extern void itob(char *buf,int data);
extern void* var_arg(var_list* vl,unsigned int size);
extern int vsprintf(char *buf,const char *fmt,var_list vl);
extern int sprintf(char *buf,const char *fmt,...);
/*将字符串中指定位置的节点分离出来，返回是否是最后一个节点,起始位置必须为'/'*/
extern int detach_node(unsigned int,const char *,char *,int *,unsigned int);


#endif /* _STDLIB_INCLUDE_ */

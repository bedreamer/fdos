/*
 *	tty.h
 *	bedreamer@163.com
 *	Friday, June 01, 2012 11:49:28 CST
 */
#ifndef _CONSOLE_
#define _CONSOLE_
#include <stddef.h>

/*从设备号*/
#define SLAVE_NUM_TTY1		0x00
#define SLAVE_NUM_TTY2		0x01
#define SLAVE_NUM_TTY3		0x02
#define SLAVE_NUM_TTY4		0x03
#define SLAVE_NUM_STDIN		0x04
#define SLAVE_NUM_STDOUT	0x05
#define SLAVA_NUM_STDERR	0x06

/* 控制台描述结构
 */
struct console_struct
{
};

#endif /*_CONSOLE_*/


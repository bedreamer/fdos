/*
 *	printk.h
 *	bedreamer@163.com
 *	Copyright (c) fool
 */
#ifndef _KPRINTF_
#define _KPRINTF_

int printk(const char *fmt,...);
int kprintf(const char *fmt,...);
#define DBG(fmt, ...)
#define TRC(fmt, ...)
#define INF(fmt, ...)
#define MSG(fmt, ...) printk(fmt, VAR_ARGS)

#endif /*_KPRINTF_*/

/*
 *	printk.c
 *	bedreamer@163.com
 *	Copyright (c) fool
 */
#include <kernel/kernel.h>

char kbuf[1024]={0};
char kx=0,ky=0;
short *pvga=(short*)0xb8000;
extern int vsprintf(char *buf, const char *fmt, var_list vl);
extern int sprintf(char *buf, const char *fmt, ...);

int vprintk(char *buf);
void setcursor(int x, int y);
int printk(const char *fmt, ...)
{
	var_list va;
	int len;
	va_start(va, fmt);
	//get_spinlock(klock);
	if (0!=kx){ kx=0, ky++;}
	len=vsprintf(kbuf, fmt, va);
	kbuf[len]='\0';
	vprintk(kbuf);
	//vprintk("\n");
	//release_spinlock(klock);
	return len;
}

int kprintf(const char *fmt, ...)
{
	var_list va;
	int len;
	va_start(va, fmt);
	//get_spinlock(klock);
	len=vsprintf(kbuf, fmt, va);
	kbuf[len]='\0';
	vprintk(kbuf);
	//release_spinlock(klock);
	return len;
}

int vprintk(char *buf)
{
	int i=0;
	while(*buf){
		pvga[ky*80+kx]=*buf | 0x0700;
		switch(*buf)
		{
			case '\n':
			{
				pvga[ky*80+kx]='\0';
				kx=0, ky++;
			}break;
			case '\t':
			{
				memset((char*)&pvga[ky*80+kx], 0, (4-kx%4)==0?4:(4-kx%4));
				kx+=(4-kx%4)==0?4:(4-kx%4);
			}break;
			default:
				kx++;
			break;
		}
		buf++;
		if (80<=kx) {
			kx=0,ky++;
		}
		//setcursor(kx,ky);
	}
	if (24<=ky)
	{
	 	//outb(0x03d4, 0x0c);
		//outb(0x03d5, (80*(ky-25))>>8&0xff);
		//outb(0x03d4, 0x0d);
		//outb(0x03d5, (80*(ky-25))&0xff);
	}
	return i;
}

void setcursor(int x,int y)
{
	outb(0x03d4, 0x0e);
	outb(0x03d5, ((y*2*80+x*2)/2)>>8&0xff);
	outb(0x03d4, 0x0f);
	outb(0x03d5, ((y*2*80+x*2)/2)&0xff);
}

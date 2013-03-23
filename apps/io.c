/*
 * foolstd.c
 * bedreamer@163.com
 */
#include <stddef.h>
#include <foolstd.h>

int open(const char *filename,int mode)
{
	return syscall(0x00020001,(int)filename,mode,0,0,0);
}

size_t write(int fd,const char *buf,size_t size)
{
	return syscall(0x00020002,fd,(int)buf,size,0,0);
}

size_t read(int fd,char *buf,size_t size)
{
	return syscall(0x00020003,fd,(int)buf,size,0,0);
}

size_t ioctl(int fd,int cmd,int param)
{
	return 0;
}

size_t seek(int fd,int offset,int base)
{
	return 0;
}

int close(int fd)
{
	return 0;
}

/*
 *	foolstd.h
 *	system call interface.
 *	bemaster@163.com
 */
#ifndef FOOL_STD_
#define FOOL_STD_

#	ifndef _STDDEF_
		#include <stddef.h>
#	endif // _STDDEF_

typedef size_t pid_t;

/*open mode and open conditions directions.*/
#define MODE_RO				0x00000001
#define MODE_READWRITE		0x00000002
#define MODE_CREATE			0x00000004
#define MODE_CREATEALWAYS	0x00000008
#define MODE_PRIVATE		0x10000000

/*seek mode*/
#define SEEK_SET			0x00000000	/*seek from start.*/
#define SEEK_CURR			0x00000001	/*seek from current location.*/
#define SEEK_TAIL			0x00000002	/*seek from end.*/

/*max filename len.*/
#define MAX_PATH_LEN		260
/*max filenam node len*/
#define MAX_NAME_NODE_LEN	14
/*max directory deep*/
#define MAX_PATH_DEEP		8

/*entry of all syscall.*/
extern int syscall(int eax,int ebx,int ecx,int edx,int edi,int esi);

extern int exec(char *filename,int argc,char **argv);
extern int waitchild(void);
extern pid_t getpid(void);
extern pid_t getppid(void);
extern void exit(int);

extern int open(const char *filename,int mode);
extern size_t write(int fd,const char *buf,size_t size);
extern size_t read(int fd,char *buf,size_t size);
extern size_t ioctl(int fd,int cmd,int param);
extern size_t seek(int fd,int offset,int base);
extern int close(int fd);

#endif /*FOOL_STD_*/

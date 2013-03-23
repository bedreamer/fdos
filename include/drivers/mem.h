/*
 *	mem.h
 *	bebeamer@163.com
 *	Saturday, June 23, 2012 01:52:26 CST 
 */
#ifndef _MEM_
#define _MEM_

/*对外接口*/
void mem_init(void);
int mem_open(struct kfile*,struct kinode*);
int mem_ioctl(struct kfile *,int param1,int param2);
size_t mem_seek(struct kfile *,size_t offset,int base);
size_t mem_read(struct kfile *,_core_ void *,size_t offset,size_t cnt);
size_t mem_write(struct kfile *,_core_ const void *,size_t offset,size_t cnt);

#endif /*_MEM_*/

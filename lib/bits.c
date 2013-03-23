/*
 *	bits.c
 *	bedreamer@163.com
 *	Tuesday, June 05, 2012 11:42:42 CST 
 *	位相关操作
 */
#include <stddef.h>

/*将指定位设置为1*/
void bitset(size_t index,volatile void *buf)
{
	(*(unsigned char*)(buf+index/8))|=(1<<(index%8));
}

/*将指定位设为0*/
void bitclean(size_t index,volatile void *buf)
{
	(*(unsigned char*)(buf+index/8))&=(~(1<<(index%8)));
}

/*读取指定位,该位为高则返回1否则返回0*/
int bitread(size_t index,volatile void *buf)
{
	buf=(void*)(((*(unsigned char*)(buf+index/8))>>(index%8)));
	return (int)buf;
}

/*将指定位取反*/
void bitrevers(size_t index,volatile void *buf)
{
	(*(unsigned char* )(buf+index/8))^=(1<<(index%8));
}

/*测试0并置位*/
int test0andset(size_t index,volatile void *buf)
{
	if (0==bitread(index,buf)){
		bitset(index,buf);
		return 1;
	}
	return 0;
}
/*测试1并消位*/
int test1andset(size_t index,volatile void *buf)
{
if (1==bitread(index,buf)){
		bitclean(index,buf);
		return 1;
	}
	return 0;
}

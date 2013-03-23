/*
 *	bits.h
 *	bedreamer@163.com
 *	Tuesday, June 05, 2012 11:42:42 CST 
 *	位相关操作
 */
#ifndef _BITS_
#define _BITS_

/*将指定位设置为1*/
extern void bitset(size_t index,volatile void *buf);
/*将指定位设为0*/
extern void bitclean(size_t index,volatile void *buf);
/*读取指定位,该位为高则返回1否则返回0*/
extern int bitread(size_t index,volatile void *buf);
/*将指定位取反*/
extern void bitrevers(size_t index,volatile void *buf);
/*测试0并置位**/
extern int test0andset(size_t index,volatile void *buf);
/*测试1并消位**/
extern int test1andset(size_t index,volatile void *buf);


#endif /*_BITS_*/

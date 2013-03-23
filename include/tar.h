/*
 *	tar.h
 *	bedreamer@163.com
 *	Monday, June 11, 2012 09:14:39 CST 
 */
#ifndef _TAR_H_
#define _TAR_H_

/*tar 文件头.*/
struct tar_head_struct{
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char chksum[8];
	char typeflag;
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char unkown[12];
};

#endif /*_TAR_H_*/



/*
 *	string.c
 *	bedreamer@163.com
 */
#include <stddef.h>

#define byte unsigned char

size_t memcpy(void *des,const void *src,size_t cnt)
{
	auto void* tmp = des;
	while (cnt--)
		*(byte*)des++ = *(byte*)src++;
	return (size_t)des-(size_t)tmp;
}

size_t memmove(void * des,const void * src,size_t cnt)
{
	auto void *tmp = des;
	if (des<src){
		while (cnt--)
			*(byte*)des ++ = *(byte*)src++;
		return (size_t)des - (size_t)tmp;
	}else if (des>src){
		while (cnt--)
			*(byte*)(des+cnt-1) = *(byte*)(src+cnt-1);
		return (size_t)tmp - (size_t)src;
	}else return 0;
}

size_t __memset(char * des,int tom,size_t cnt)
#define memset __memset
{
	auto void *tmp = des;
	while (cnt--)
		*(byte*)des++ = (byte)tom;
	return (size_t)des - (size_t)tmp;
}

size_t strcat(char * des,const char * src)
{
	auto char *tmp;
	while (*des++);
	tmp = des;
	while (*src)
		*des++ = *src ++;
	return (size_t)des-(size_t)tmp;
}

int strcmp(const char * des,const char * src)
{
	while ((*des)&&(*src)&&0==*des-(*src))
		des ++,src++;
	return *des - *src;
}

size_t strcpy(char * des,const char * src)
{
	auto char *tmp = des;
	while (*src)
		*des ++ = *src++;
	return (size_t)des - (size_t)tmp;
}

size_t strlen(const char* des)
{
	auto const char *tmp =des;
	while (*des++);
	return (size_t)des-(size_t)tmp;
}

int strncmp(const char * des,const char * src,size_t max_)
{
	while ((*des)&&(*src)&&0==*des-(*src)&&max_--)
		des ++,src++;
	return *des - *src;
}

size_t strncpy(char *des,const char *src,size_t max_)
{
	auto const char *tmp = des;
	while (*src&&max_--)
		*des++=*src++;
	return (size_t)des-(size_t)tmp;
}

size_t strnlen(const char *des,size_t max_)
{
	auto const char *tmp = des;
	while (*des&& max_--)
		des ++;
	return (size_t)des-(size_t)tmp;
}

const char * strstr(const char * des,const char * src)
{
	auto const char *tmpd = NULL,*tmps = src;
	while (*des&&*src){
		if (*des++ == *tmps++){
			tmpd = tmps-1;
			while (*tmps && *des ++ == *tmps ++);
			if (NULL == tmps )
				return tmpd;
			else{
				tmps = src;
				break;
			}
		}
	}
	return NULL;
}

size_t tolower(char * des)
{
	auto char* tmp = des;
	while (*des){
		*des = *des >= 'A' && *des < 'Z' ? *des + 'A' - 'a' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

size_t tonlower(char * des,size_t max_)
{
	auto char* tmp = des;
	while (*des&&max_--){
		*des = *des >= 'A' && *des < 'Z' ? *des + 'A' - 'a' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

size_t tonupper(char * des,size_t max_)
{
	auto char* tmp = des;
	while (*des&&max_--){
		*des = *des >= 'a' && *des < 'z' ? *des + 'a' - 'A' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

char toupper(char des)
{
	des = des >= 'a' && des < 'z' ? des + 'a' - 'A' : des;
	return des;
}

/*宽字符操作函数*/
size_t wstrcat(wchar_t * des,const wchar_t * src)
{
	auto wchar_t *tmp;
	while (*des++);
	tmp = des;
	while (*src)
		*des++ = *src ++;
	return (size_t)des-(size_t)tmp;
}

int wstrcmp(const wchar_t * des,const wchar_t * src)
{
	while ((*des)&&(*src)&&0==*des-(*src))
		des ++,src++;
	return *des - *src;
}

size_t wstrcpy(wchar_t * des,const wchar_t * src)
{
	auto wchar_t *tmp = des;
	while (*src)
		*des ++ = *src++;
	return (size_t)des - (size_t)tmp;
}

size_t wstrlen(const wchar_t* des)
{
	auto const wchar_t *tmp =des;
	while (*des++);
	return (size_t)des-(size_t)tmp;
}

int wstrncmp(const wchar_t * des,const wchar_t * src,size_t max_)
{
	while ((*des)&&(*src)&&0==*des-(*src)&&max_--)
		des ++,src++;
	return *des - *src;
}

size_t wstrncpy(wchar_t *des,const wchar_t *src,size_t max_)
{
	auto const wchar_t *tmp = des;
	while (*src&&max_--)
		*des++=*src++;
	return (size_t)des-(size_t)tmp;
}

size_t wstrnlen(const wchar_t *des,size_t max_)
{
	auto const wchar_t *tmp = des;
	while (*des&& max_--)
		des ++;
	return (size_t)des-(size_t)tmp;
}

const wchar_t * wstrstr(const wchar_t * des,const wchar_t * src)
{
	auto const wchar_t *tmpd = NULL,*tmps = src;
	while (*des&&*src){
		if (*des++ == *tmps++){
			tmpd = tmps-1;
			while (*tmps && *des ++ == *tmps ++);
			if (NULL == tmps )
				return tmpd;
			else{
				tmps = src;
				break;
			}
		}
	}
	return NULL;
}

size_t wtolower(wchar_t * des)
{
	auto wchar_t* tmp = des;
	while (*des){
		*des = *des >= 'A' && *des < 'Z' ? *des + 'A' - 'a' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

size_t wtonlower(wchar_t * des,size_t max_)
{
	auto wchar_t* tmp = des;
	while (*des&&max_--){
		*des = *des >= 'A' && *des < 'Z' ? *des + 'A' - 'a' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

size_t wtonupper(wchar_t * des,size_t max_)
{
	auto wchar_t* tmp = des;
	while (*des&&max_--){
		*des = *des >= 'a' && *des < 'z' ? *des + 'a' - 'A' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

size_t wtoupper(wchar_t * des)
{
	auto wchar_t* tmp = des;
	while (*des){
		*des = *des >= 'a' && *des < 'z' ? *des + 'a' - 'A' : *des;
		des ++;
	}
	return (size_t)des - (size_t)tmp;
}

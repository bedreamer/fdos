/*
 * Init.c
 * bedramer@163.com
 * 
 */
#include <stdio.h>
#include <foolstd.h>


int main()
{
	int len,i=0;

	char buf[1024];

	buf[0] = 0;
	buf[1023] = 0;

	printf("Hello World!\n");
	printf("buffer address: 0x%x %s\n",buf);

	while (1)
	{
		memset(buf,0,1024);
		len=gets(buf);
		printf("%s\n",buf);
	}
	return 0;
}

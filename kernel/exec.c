/*
 *	system call
 *	exec
 *	bedreamer@163.com
 *	Monday, July 02, 2012 02:46:09 CST 
 */
#include <kernel/kernel.h>
#include <kernel/elf.h>
#include <kernel/schedu.h>
#include <kernel/kmodel.h>

/*执行完整路径名的文件*/
pid_t do_execl(_ci const char *filename,_ci const char param)
{
	return 0;
}

/*从环境变量中选取合适的路径执行*/
pid_t do_execv(_ci const char *filename,_ci const char param)
{
	return 0;
}

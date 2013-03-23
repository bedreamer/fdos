/*
 *	signal.c
 *	bedreamer@163.com
 *	Saturday, June 02, 2012 10:17:24 CST
 *
 */
#include <kernel/kernel.h>
#include <kernel/schedu.h>
#include <kernel/signal.h>
#include <kernel/mm.h>

/*create mutex object.*/
struct mutex* createmutex(const char *mutexname,bool owner)
{
	return NULL;
}

/*destroy mutex object.*/
int destroymutex(struct mutex *pmt)
{
	return 0;
}

/*wait for mutex object.*/
int waitformutex(struct mutex *pmt)
{
	if (1==pmt->_mutex){
		pmt->_mutex--;
		pmt->ptsk=tsk_running;
		return 1;
	} else {
		return 0;
	}
}

/*release mutex object.*/
int releasemutex(struct mutex *pmt)
{
	if (tsk_running!=pmt->ptsk){
		printk("0x%x may be dangerours:(",releasemutex);
		return 0;
	} else {
		return 0;
	}
}

int wait(struct semaphore *psp)
{
	return 0;
}

int signal(struct semaphore *psp)
{
	return 0;
}




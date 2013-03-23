/**************************************************************
 *	signal.h
 *	bedreamer@163.com
 *	Monday, May 28, 2012 12:46:22 CST
 *	see http://www.cnblogs.com/Huayuan/archive/2012/05/01/2477686.html
 ****************************************************************/
#ifndef _SIGNAL_H_
#define _SIGNAL_H_

/*  spin lock.
 *	@ _lck : seed for spin lock.
 */
struct spin_lock
{
	volatile int _lck;
};
#define SPIN_LOCKED		0xCCCCCCCC
#define SPIN_UNLOCKED	0x00000000

/*lock the spin lock.*/
#define spinlock(lock) (lock)._lck=SPIN_LOCKED
/*unlock the spin lock.*/
#define spinunlock(lock) (lock)._lck=SPIN_UNLOCKED
/*initialize the spin lock.*/
#define spinlock_init(lock) spinunlock(lock)
/*get the spin lock.*/
#define get_spinlock(lock) do {} while(SPIN_LOCKED==(lock)._lck);spinlock(lock)
/*release the spin lock.*/
//#define release_spinlock(lock) do{} while(SPIN_UNLOCKED==(lock)._lck);spinunlock(lock)
#define release_spinlock(lock) do{spinunlock(lock);} while(SPIN_LOCKED==(lock)._lck);

/*
 *	waiter list of mutext.
 *	@ ptsk: thread struct pointer of mutex waiter.
 *	@ mwlst: list of waiter. 
 */
struct mutex_waiter{
	struct kthread_struct *ptsk;
	struct list_head mwlst;
};

/*
 *	mutex lock
 *	互斥锁用在读写互斥，写写互斥中.
 *	@ mlst: 这一类互斥量链表.
 *	@ pmw: 等待这个资源的队列.
 *	@ mutex_waitercnt: 等待该信号的用户数量.
 *	@ mutexname: 互斥量名称可为空.
 *	@ _mutext: 若为0访问该临界值的线程将被挂起_mutext@[1,0].
 *	@ ptsk: 当前拥有该互斥量的任务.
 *	#使用规则:
 *	1.同一线程不能重复等待，获取到互斥锁后不能再等待.
 *	2.不能重复释放,获得一次对应的释放一次.
 *	3.谁拿锁谁释放.
 */
struct mutex {
	struct list_head mlst;
	struct mutex_waiter* pmw;
	size_t mutex_waitercnt;
	char *mutexname;
	volatile int _mutex;
	struct task_struct *ptsk;	/*owner who tack this mutex now.*/
};

static inline void init_mutex(struct mutex *pm,char *mname)
{
	list_ini(pm->mlst);
	pm->pmw=NULL;
	pm->mutex_waitercnt=0;
	pm->mutexname=mname;
	pm->_mutex=1;
	pm->ptsk=NULL;
}

/*create mutext object.*/
struct mutex* createmutex(const char *,bool);
/*get mutex object.*/
struct mutex *getmutex(const char *);
/*destroy mutext object.*/
int destroymutex(struct mutex *pmt);
/*wait for mutex object.*/
int waitformutex(struct mutex *pmt);
/*release mutex objext.*/
int releasemutex(struct mutex *pmt);//V +

/*
 *	信号量
 */
struct semaphore{
	volatile int value;
};
int wait(struct semaphore *psp);
#define _P(psp) wait(psp)
int signal(struct semaphore *psp);
#define _V(psp) signal(psp)

struct message{
	struct list_head mlst;
};

#endif /*_SIGNAL_H_*/



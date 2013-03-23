/*
 *	schedu.h
 *	bedreamer@163.com
 *	Thursday, May 31, 2012 02:04:29 CST 
 */
#ifndef _SCHEDU_
#define _SCHEDU_

#include <kernel/kernel.h>
#include <kernel/kmodel.h>

/*thread is creating*/
#define STATUS_THREAD_CREATING			0x0001
/*thread created but not running*/
#define STATUS_THREAD_CREATED			0x0002
/*times up hang-up*/
#define STATUS_THREAD_HANGUP			0x0004
/*I/O block*/
#define STATUS_THREAD_IOHANGUP			0x0008
/*running.*/
#define STASTUS_THREAD_RUNNING			0x0010
/*get a QUIT signal but not quit.*/
#define STATUS_THREAD_SIGNAL_KILL		0x0020
/*quiting.*/
#define STATUS_THREAD_KILLING			0x0040
/*already quit*/
#define STATUS_THREAD_KILLED			0x0080

/*thread switch stack buffer.*/
struct stack_regs_struct
{
	_u32		gs;			//0
	_u32		fs;			//1
	_u32		es;			//2
	_u32		ds;			//3
	_u32		edi;		//4
	_u32		esi;		//5
	_u32		ebp;		//6
	_u32		kernel_esp;	//7
	_u32		ebx;		//8
	_u32		edx;		//9
	_u32		ecx;		//10
	_u32		eax;		//11
	_u32		eip;		//12
	_u32		cs;			//13
	_u32		eflags;		//14
	_u32		esp;		//15
	_u32		ss;			//16
};

struct task_struct;
/*TCB
 *@th_ptsk:pointer to current task.
 *@th_regs:registers.
 *@th_status:thread status.
 *@th_id:pid of this thread.
 *@t_pid:pid of parent thread.
 *@th_tckremain:tick remain.
 *@th_tckused:tick used total.
 *@th_kstack: kernel stack.
 */
struct kthread_struct 
{
	_u32 	th_kstack[0x400]; // 4K byte,1024 items,不要修改这个成员的位置
	struct stack_regs_struct th_regs;

	_u16	th_status;
	pid_t 	th_id;
	pid_t	t_pid;

	_u32	th_tckremain;
	_u64	th_tckused;

	struct task_struct *th_ptsk;
};

/* PCB.
 *@t_cr3: task cr3 register.
 *@t_thrd: task thread array.
 *@t_threadcnt: thread count of task.
 *@t_lst: task list node.
 *@t_name: task name.
 *@t_pmsgq: message quene.
 *@t_msglck: message quene lock.
 *@t_ppid: pareant task pid.
 *@t_pid: main pid of this task.
 *@t_level: task level[-2,-1,0,1,2].
 *@t_pparent: parent task struct ptr.
 *@t_fcnt: how many file opend.
 *@t_flck: task file table lock.
 *@t_file: opend file index array.
 */
struct task_struct
{
	size_t t_cr3;
	struct kthread_struct *t_thrd[K_MAX_THREAD_CNT_PPS];
	size_t t_threadcnt;

	struct list_head t_lst;
	char t_name[32];

	struct message *t_pmsgq;
	struct spin_lock t_msglck;

	pid_t	t_ppid;
	pid_t	t_pid;	
	int		t_level;

	struct task_struct *t_pparent;

	size_t t_fcnt;
	struct spin_lock t_flck;
	struct file *t_file[K_MAX_FILE_OPEN_PPS];
};

/*main schedu program.*/
extern void kschedu(void);
extern void kscheduinit(void);

/*current running thread.*/
extern struct kthread_struct *thr_running;
#define curr_tid (thr_running->th_id)
/*current running task.*/
extern struct task_struct * tsk_running;
#define curr_pid (tsk_running->t_pid)
extern struct task_struct Init;
extern struct kthread_struct Pinit;

extern void file_threadregister(struct stack_regs_struct *,size_t,size_t,int,const char *argv[]);
extern pid_t do_execl(_ci const char *,_ci const char param);
extern pid_t do_execv(_ci const char *,_ci const char param);

#endif /*_SCHEDU_*/

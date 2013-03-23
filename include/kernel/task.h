#ifndef _TASK_H_
#define _TASK_H_

#include <config.h>
#include <kernel/io.h>

struct task_context;

struct thread_context
{
    struct list_head thrd_lst;
    struct task_context *ptsk;
    pid_t  thrd_id;
    pid_t  thrd_pid;

    struct cpu_regs thrd_context;
};

struct task_context
{
    struct thread_context *mainthrd;
    struct file *[NR_FILE];
};

#endif /*_TASK_H_*/

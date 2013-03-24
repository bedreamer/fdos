/*
 * system.h
 *  
 * Copyright (C) 2012, 2013 LiJie. 
 */
#ifndef _X86_SYSTEM_
#define _X86_SYSTEM_

#include <stddef.h>

/*about type define. */
typedef unsigned int pid_t;
typedef unsigned int dev_t;
typedef unsigned int time_t;
typedef unsigned int date_t;
typedef int foff_t;

/*i386 CPU registers struct. */
struct cpu_regs
{
    u32  gs, fs, es, ds;
    u32  edi, esi, ebp;
    u32  kernel_esp;
    u32  ebx, edx, ecx, eax;
    u32  eip, cs, eflags;
    u32  esp, ss;
    u32  cr3;
};

extern size_t __memcopy(char *, const char *, size_t);
#define memcpy(des, src, cnt) \
   __memcopy(des, (const char*)src, (size_t)cnt)
extern size_t __memset(char *, int, size_t);
#define memset(des, f, cnt) \
   __memset(des, f, (size_t)cnt)
extern size_t __strcpy(char *, const char *);
#define strcpy(des, src) \
   __strcpy(des, (const char*)src)
extern size_t __strncpy(char *, const char *, size_t);
#define strncpy(des, src, cnt) \
   __strncpy(des, (const char*)src, (size_t)cnt)
extern int __strcmp(const char *, const char *);
#define strcmp(str1, str2) \
   __strcmp((const char*)str1, (const char*)str2)

/*about IRQ contral. */
#define cli() __asm volatile("cli;\n")
#define sti() __asm volatile("sti;\n")
#define save_flags(x)  \
   __asm volatile("pushfl ; popl %0":"=r" (x)::"memory")
#define restore_flags(x) \
   __asm volatile("pushl %0 ; popfl"::"r" (x):"memory")

/*about PIO. */
extern u8  __inb (u16);
#define inb(port) __inb((u16)port)
extern u16 __inw (u16);
#define inw(port) __inw((u16)port)
extern u32 __indw(u16);
#define indw(port) __indw((u16)port)
extern void __outb(u8, u16);
#define outb(d, p) __outb((u8)d, (u16)p)
extern void __outw(u16, u16);
#define outw(d, p) __outw((u16)d, (u16)p)
extern void __outdw(u32, u16);
#define outdw(d, p) __outdw((u32)d, (u16)p)

/*about CPU. */
extern void __save_cpu_context(struct cpu_regs *);
#define save_cpu_context(c) \
   __save_cpu_context(c)
extern void __restor_cpu_context(struct cpu_regs *);
#define restore_cpu_context(c) \
   __restore_cpu_context(c)
extern u64  __read_cpu_counter(void);
#define read_cpu_counter __read_cpu_counter

/*about page manage unit. */
extern void memory_startup();
extern void paging_startup();
extern u32  __alloc_free_page(void);
#define alloc_free_page __alloc_free_page
extern void __free_page(u32);
#define free_page(p) __free_page(p)
extern u32 t_memsize, t_mavlsize;
extern u32 t_pagecnt, t_pavlcnt, t_pcommitcnt;

/*about early printf function. */
extern int __early_printk(const char *, ...);

#define IRQCNT      256UL
#define VCT_HW      1
#define VCT_EXP     2
#define VCT_SYSCALL 3
typedef union
{
   int (*irqhandle)(u8);
   int (*exphandle)(u8,int,u32,u16,u32);
}irqhandle;
extern int __register_irq(u8, irqhandle, u8);
#define register_irq(vct, handle, type) \
   __register_irq(vct, handle, type)
extern int __unregister_irq(u8, irqhandle, u8);
#define unregister_irq(vct, handle, type) \
   __unregister_irq(vct, handle, type)

/**
  arch reconfigure code. 
  will be excuted by fdos kernel as callback methord. 
  ** purpos
   add arch relation node into system. 
 */
extern void arch_x86_reconfig(void);

/*entry for fdos kernel code. */
extern void start_kernel(char *);
#endif /*_X86_SYSTEM_*/



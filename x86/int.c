/**
 * int.c
 * interrupt procedure of X86 based on.
 * cuplision@163.com
 * 2013/01/27, SUN, 10:43.
 * Copyright (C) 2012, 2013. PRC, LiJie.
 */
#include "system.h"
#include "x86.h"

#define UNREGISTED 0x00
#define REGISTED   0xFF
struct irq_struct
{ 
    irqhandle prc;
    volatile u32 call_cnt;
    u8  type;
    volatile u8  registed;
}irqvct[IRQCNT];
int x86_unexpt_handle(u8 vct);
struct irq_struct vct[IRQCNT];

int x86_irq_unhandle(u8 vct)
{
    return 0;
}

int x86_exp_unhandle
(
    u8 vct, 
    int ercode, 
    u16 cs, 
    u32 eip, 
    u32 eflags){
 
   return 0;
}

int x86_irq_handle(u8 vct)
{
    if (irqvct[vct].registed)
       return irqvct[vct].prc.irqhandle(vct);
    else return x86_irq_unhandle(vct);
}

int x86_exp_handle(u8 vct, int ercode, u16 cs, u32 eip, u32 eflags)
{
    if (irqvct[vct].registed)
       return irqvct[vct].prc.exphandle(
              vct, ercode, cs, eip, eflags);
    else return x86_exp_unhandle(
              vct, ercode, cs, eip, eflags);
}

int ___register_exp_irq(u8 vn, irqhandle prc)
{
    if (UNREGISTED==irqvct[vn].registed) return 0;
    else irqvct[vn].registed = REGISTED;
    irqvct[vn].type = VCT_HW;
    irqvct[vn].prc.irqhandle = prc.irqhandle;
    irqvct[vn].call_cnt = 0;
    return 1;
}

int ___register_hw_irq(u8 vn, irqhandle prc)
{
    if (UNREGISTED==irqvct[vn].registed) return 0;
    else irqvct[vn].registed = REGISTED;
    irqvct[vn].type = VCT_EXP;
    irqvct[vn].prc.exphandle = prc.exphandle;
    irqvct[vn].call_cnt = 0;
    return 1;
}

int ___register_syscall_irq(u8 vn, irqhandle prc)
{
    if (UNREGISTED==irqvct[vn].registed) return 0;
    else irqvct[vn].registed = REGISTED;
    irqvct[vn].type = VCT_SYSCALL;
    irqvct[vn].prc.irqhandle = prc.irqhandle;
    irqvct[vn].call_cnt = 0;
    return 1;
}

int (*___register_irq[])(u8 vn, irqhandle prc)={
    ___register_exp_irq, ___register_hw_irq,
    ___register_syscall_irq };

int __register_irq(u8 vn, irqhandle prc, u8 type)
{
    return 0;
}

int __unregister_irq(u8 vn, irqhandle prc, u8 type)
{
    return 0;
} 



/*
 * x86mm.c 
 * x86 page manage uint.
 * 02/02/2013 23:53:10 
 * Copyright (C) 2012, 2013 LiJie.
 */
#include "system.h"

// total memory size.
u32 t_memsize=0;
// avilable memort size.
u32 t_mavlsize=0;

// total page count.
u32 t_pagecnt=0;
// free page count.
u32 t_pavlcnt=0;
// commited page count.
u32 t_pcommitcnt=0;

u32  __alloc_free_page(void)
{
    return 0;
}

void __free_page(u32 pn)
{
}

void memory_startup()
{
}

void paging_startup()
{
}

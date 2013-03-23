/*
 *	time.c
 *	dedreamer@163.com
 *	COMS time module
 */
#include <kernel/kernel.h>
#include <kernel/time.h>

void getsystime(struct sys_time *st)
{
	st->t_sec=asm_readcmos(CMOS_REG_SEC);
	st->t_sec=((st->t_sec&0xF0)>>4)*10+(st->t_sec&0x0F);
	st->t_min=asm_readcmos(CMOS_REG_MIN);
	st->t_min=((st->t_min&0xF0)>>4)*10+(st->t_min&0x0F);
	st->t_hour=asm_readcmos(CMOS_REG_HOUR);
	st->t_hour=((st->t_hour&0xF0)>>4)*10+(st->t_hour&0x0F);
	st->t_weekday=asm_readcmos(CMOS_REG_WEEKDAY);
	st->t_day=asm_readcmos(CMOS_REG_DAY);
	st->t_day=((st->t_day&0xF0)>>4)*10+(st->t_day&0x0F);
	st->t_month=asm_readcmos(CMOS_REG_MONTH);
	st->t_month=((st->t_month&0xF0)>>4)*10+(st->t_month&0x0F);
	st->t_year=asm_readcmos(CMOS_REG_YEAR);
	st->t_year=((st->t_year&0xF0)>>4)*10+(st->t_hour&0x0F);
}

time_t gettime(void)
{
	return MAKETIME(asm_readcmos(CMOS_REG_HOUR),asm_readcmos(CMOS_REG_MIN),asm_readcmos(CMOS_REG_SEC));
}

date_t getdate(void)
{
	return MAKEDATE(asm_readcmos(CMOS_REG_YEAR),asm_readcmos(CMOS_REG_MONTH),asm_readcmos(CMOS_REG_DAY),asm_readcmos(CMOS_REG_WEEKDAY));
}

/*
 *	x86.c
 *	bedreamer@163.com
 *	Sunday, May 27, 2012 11:36:21 CST 
 *	Copyright (C) 2012, 2013 - fool
 *
 *  02/02/2013 22:38:31 Reconstruct code struct by fool.
 */
#   include "system.h"
#   include "x86.h"

char kernel_cmdline[]="root=/dev/hda1;tty=/dev/ttyUSB0;debug;";
struct seg_descriptor gdt[GDT_CNT];
struct gate_struct idt[IDT_CNT];
union dptr idt_ptr;
union dptr gdt_ptr;

/**
 * do Initialize based on X86 platform then
 * jump to kernel_main.  
 */
void x86_main()
{
    void irqrouter_init(void);
    extern void early_memory_init(void);

    irqrouter_init();
//    early_memory_init();
    cli();
	start_kernel(kernel_cmdline); /* Can't return.*/

	while (1) ;
}

void mkdescriptor
(
    struct seg_descriptor* pDescrip,
    u32 base, 
    u32 limit, 
    u16 attribute){
	pDescrip -> limit_low = limit & 0x0FFFF;
	pDescrip -> base_low  = base & 0x0FFFF;
	pDescrip -> base_mid  = ( base >> 16 ) & 0x0FF;
	pDescrip -> attrib1	  = attribute & 0xFF;
	pDescrip -> limit_high_attr2 =
       ((limit>>16)& 0x0F) | ((attribute>>8)&0xF0);
	pDescrip -> base_high = (base>>24)&0x0FF;
}

/*转移GDT，IDT*/
void doshift()
{
    idt_ptr.ptr.limit=IDT_CNT*sizeof(struct gate_struct)-1;
    idt_ptr.ptr.u.gbase=idt;
    gdt_ptr.ptr.limit=sizeof(struct seg_descriptor)*GDT_CNT-1;
    gdt_ptr.ptr.u.sbase=gdt;

 	mkdescriptor(&gdt[0],0x00000000,0x00000000,
     0x0000);// NULL descriptor
	mkdescriptor(&gdt[1],0x00000000,0x000FFFFF,
     DA_CR |DA_32|DA_LIMIT_4K); // kernel code.
	mkdescriptor(&gdt[2],0x00000000,0x000FFFFF,
     DA_DRW|DA_32|DA_LIMIT_4K); // kernel data.
	mkdescriptor(&gdt[3],0x00000000,0x000FFFFF,
     DA_CR |DA_32|DA_LIMIT_4K|DA_DPL3); // user code.
	mkdescriptor(&gdt[4],0x00000000,0x000FFFFF,
     DA_DRW|DA_32|DA_LIMIT_4K|DA_DPL3); // user data.
}

void init_idt_desc
(
    u8 vector,
    u8 desc_type,
    void (*irqhandle)(void),
    u8 prvilege){
	struct gate_struct *pgate = &idt[vector];
	u32 base = (u32)irqhandle;
	pgate->offset_low = base & 0xFFFF;
	pgate->selector = CORE_CS;
	pgate->dcount = 0;
    pgate->attr = desc_type | (prvilege << 5);
	pgate->offset_hight = (base >> 16) & 0xFFFF;
	return;	
}

extern void __divide_error(void);
extern void __single_step_exception(void);
extern void __nmi(void);
extern void __breakpoint_exception(void);
extern void __overflow(void);
extern void __bounds_check(void);
extern void __inval_opcode(void);
extern void __copr_not_available(void);
extern void __double_fault(void);
extern void __copr_seg_overrun(void);
extern void __inval_tss(void);
extern void __segment_not_present(void);
extern void __stack_exception(void);
extern void __general_protection(void);
extern void __page_fault(void);
extern void __copr_error(void);
extern void __clock_hwint00(void);
extern void __keyboard_hwint01(void);
extern void __cascade_hwint02(void);
extern void __second_serial_hwint03(void);
extern void __first_serial_hwint04(void);
extern void __XT_winchester_hwint05(void);
extern void __floppy_hwint06(void);
extern void __printer_hwint07(void);
extern void __realtime_clock_hwint08(void);
extern void __irq_2_redirected_hwint09(void);
extern void __irq_10_hwint10(void);
extern void __irq_11_hwint11(void);
extern void __PS_2_mourse_hwint12(void);
extern void __FPU_exception_hwint13(void);
extern void __IDE0_hwint14(void);
extern void __IDE1_hwint15(void);
extern void __asm_sys_call(void);
void irqrouter_init(void)
{
	init_idt_desc(
        INT_VCT_DIVIDE,		
		DA_386IGate,
        __divide_error,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_DEBUG,		
		DA_386IGate, 
        __single_step_exception,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_NMI,			
		DA_386IGate,
        __nmi,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_BREAKPOINT,	
		DA_386IGate,
        __breakpoint_exception,
        PRIVILEGE_USER);
	init_idt_desc(
        INT_VCT_OVERFLOW,		
		DA_386IGate,
        __overflow,
        PRIVILEGE_USER);
	init_idt_desc(
        INT_VCT_BOUNDS,		
		DA_386IGate,
        __bounds_check,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_INVAL_OP,		
		DA_386IGate,
        __inval_opcode,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_COPROC_NOT,	
		DA_386IGate,
        __copr_not_available,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_DOUBLE_FAULT,	
		DA_386IGate,
        __double_fault,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_COPROC_SEG,	
		DA_386IGate,
        __copr_seg_overrun,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_INVAL_TSS,		
		DA_386IGate,
        __inval_tss,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_SEG_NOT,		
		DA_386IGate,
        __segment_not_present,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_STACK_FAULT,	
		DA_386IGate,
        __stack_exception,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_PROTECTION,	
		DA_386IGate,
        __general_protection,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_PAGE_FAULT,	
	    DA_386IGate,
        __page_fault,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_COPROC_ERR,	
	    DA_386IGate,
        __copr_error,
        PRIVILEGE_KRNL);

	init_idt_desc(
        INT_VCT_IRQ0 + 0,	
		DA_386IGate,
        __clock_hwint00,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 1,	
		DA_386IGate,
        __keyboard_hwint01,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 2,	
		DA_386IGate,
        __cascade_hwint02,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 3,	
		DA_386IGate,
        __second_serial_hwint03,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 4,	
		DA_386IGate,
        __first_serial_hwint04,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 5,	
		DA_386IGate,
        __XT_winchester_hwint05,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 6,	
		DA_386IGate,
        __floppy_hwint06,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ0 + 7,	
		DA_386IGate,
        __printer_hwint07,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 0,	
		DA_386IGate,
        __realtime_clock_hwint08,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 1,	
		DA_386IGate,
        __irq_2_redirected_hwint09,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 2,	
		DA_386IGate,
        __irq_10_hwint10,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 3,	
		DA_386IGate,
        __irq_11_hwint11,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 4,	
		DA_386IGate,
        __PS_2_mourse_hwint12,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 5,	
		DA_386IGate,
        __FPU_exception_hwint13,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 6,	
		DA_386IGate,
        __IDE0_hwint14,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_IRQ8 + 7,	
		DA_386IGate,
        __IDE1_hwint15,
        PRIVILEGE_KRNL);
	init_idt_desc(
        INT_VCT_SYSCALL,					
		DA_386IGate,
        __asm_sys_call,
        PRIVILEGE_USER);

    // Initlize 8259A IRQ contraller. 
	outb(INT_M_CTL,		0x11);				// Master 8259, ICW1.
	outb(INT_S_CTL,		0x11);				// Slave  8259, ICW1.
	outb(INT_M_CTLMASK,	INT_VCT_IRQ0);	    // Master 8259, ICW2.
	outb(INT_S_CTLMASK,	INT_VCT_IRQ8);	    // Slave  8259, ICW2.
	outb(INT_M_CTLMASK,	0x4);				// Master 8259, ICW3. 
	outb(INT_S_CTLMASK,	0x2);				// Slave  8259, ICW3. 
	outb(INT_M_CTLMASK,	0x1);				// Master 8259, ICW4.
	outb(INT_S_CTLMASK,	0x1);				// Slave  8259, ICW4.
	outb(INT_M_CTLMASK,	0x01);				// Master 8259, OCW1. 
	outb(INT_S_CTLMASK,	0x00);				// Slave  8259, OCW1. 
}

void arch_x86_reconfig(void)
{
}






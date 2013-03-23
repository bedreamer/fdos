#   ifndef _SYSTEM_H_
#   define _SYSTEM_H_

#   ifndef _ARCH_x86_
#   error "Wrong config, Abort!!!"
#   else
#   define GDT_CNT  6
#   define IDT_CNT  256

#   define CORE_CS  0x08
#   define CORE_DS  0x10
#   define USER_CS  0x1B
#   define USER_DS  0x23
#   define CORE_TSS 0x28

#   define SA_RPL_MASK  0xFFFC
#   define SA_RPL0      0
#   define SA_RPL1      1
#   define SA_RPL2      2
#   define SA_RPL3      3
#   define SA_TI_MASK   0xFFFB
#   define SA_TIG       0
#   define SA_TIL       4
#   define DA_32        0x4000
#   define DA_LIMIT_4K  0x8000
#   define DA_DPL0      0x00
#   define DA_DPL1      0x20
#   define DA_DPL2      0x40
#   define DA_DPL3      0x60
#   define DA_DR        0x90
#   define DA_DRW       0x92
#   define DA_DRWA      0x93
#   define DA_C         0x98
#   define DA_CR        0x9A
#   define DA_CCO       0x9C
#   define DA_CCOR      0x9E
#   define DA_LDT       0x82
#   define DA_TaskGate  0x85
#   define DA_386TSS    0x89
#   define DA_386CGate  0x8C
#   define DA_386IGate  0x8E
#   define DA_386TGate  0x8F
#   define PRIVILEGE_KRNL    0
#   define PRIVILEGE_TASK    1
#   define PRIVILEGE_USER    3
#   define RPL_KRNL    SA_RPL0
#   define RPL_TASK    SA_RPL1
#   define RPL_USER    SA_RPL3

struct seg_descriptor
{
    u16 limit_low;
    u16 base_low;
    u8  base_mid;
    u8  attrib1;
    u8  limit_high_attr2;
    u8  base_high;
};

struct gate_struct
{
    u16    offset_low;
    u16    selector;
    u8     dcount;
    u8     attr;
    u16    offset_hight;
};

#   pragma pack(1)
union dptr
{
    unsigned char d[6];
    struct { u16 limit;
      union {
       struct seg_descriptor *sbase;
       struct gate_struct *gbase;
       }u;
    }ptr;
};
#   pragma pack()

/* 8259A interrupt controller ports. */
#define	INT_M_CTL       0x20
#define	INT_M_CTLMASK   0x21
#define	INT_S_CTL       0xA0
#define	INT_S_CTLMASK   0xA1
#define INT_MS_EOI	    0x20
#define	INT_VCT_IRQ0    0x20
#define	INT_VCT_IRQ8    0x28
#define eoi_m() outb(INT_M_CTL,INT_MS_EOI)
#define eoi_s() outb(INT_S_CTL,INT_MS_EOI)

/* interrupt vector */
#define	INT_VCT_DIVIDE			0x0
#define	INT_VCT_DEBUG			0x1
#define	INT_VCT_NMI				0x2
#define	INT_VCT_BREAKPOINT		0x3
#define	INT_VCT_OVERFLOW        0x4
#define	INT_VCT_BOUNDS			0x5
#define	INT_VCT_INVAL_OP        0x6
#define	INT_VCT_COPROC_NOT      0x7
#define	INT_VCT_DOUBLE_FAULT    0x8
#define	INT_VCT_COPROC_SEG		0x9
#define	INT_VCT_INVAL_TSS		0xA
#define	INT_VCT_SEG_NOT			0xB
#define	INT_VCT_STACK_FAULT		0xC
#define	INT_VCT_PROTECTION		0xD
#define	INT_VCT_PAGE_FAULT		0xE
#define	INT_VCT_COPROC_ERR		0xF
#define INT_VCT_IRQ0            0x20
#define INT_VCT_IRQ8            0x28
#define INT_VCT_SYSCALL         0x80

#define CLOCK_HWINT0				INT_VCT_IRQ0 + 0
#define KEYBOARD_HWINT1				INT_VCT_IRQ0 + 1
#define CASCADE_HWINT2				INT_VCT_IRQ0 + 2
#define SECOND_SERIAL_HWINT3		INT_VCT_IRQ0 + 3
#define FIRST_SERIAL_HWINT4			INT_VCT_IRQ0 + 4
#define XT_WINCHERSTER_HWINT5		INT_VCT_IRQ0 + 5
#define FLOPPY_HWINT6				INT_VCT_IRQ0 + 6
#define PRINTER_HWINT7				INT_VCT_IRQ0 + 7
#define REALTIME_CLOCK_HWINT8		INT_VCT_IRQ8 + 0
#define IRQ2_REDIRECTED_HWINT9		INT_VCT_IRQ8 + 1
#define IRQ10_REDIRECTED_HWINT10	INT_VCT_IRQ8 + 2
#define IRQ11_REDIRECTED_HWINT11	INT_VCT_IRQ8 + 3
#define PS2_MOURSE_HWINT12			INT_VCT_IRQ8 + 4
#define FPU_EXCEPTION_HWINT13		INT_VCT_IRQ8 + 5
#define IDE0_INT					INT_VCT_IRQ8 + 6
#define IDE1_INT					INT_VCT_IRQ8 + 7

#   endif /*_ARCH_X86_*/
#   endif  /*_SYSTEM_H_*/

/*
 * vga.h
 * Thu 08 Nov 2012 02:17:03 PM CST 
 * cuplision@163.com
 * VGA 操作使用默认的文本模式，因此在操作显卡时按照双字节操作
 */
#ifndef _VGA_
#define _VGA_

#include <kernel/kernel.h>
#include <kernel/kmodel.h>

#define VGA_TXT_BASE		0x000B8000	/*文本模式基地址*/
#define VGA_DEFAULT_BASE	VGA_TXT_BASE
#define VGA_GRIPH_BASE		0x000A0000	/*图形模式基地址*/
#define VGA_TXT_MAX_ADDR	0x00BFFFF	/*文本模式下能访问的最大地址*/
extern int vga_startup(void);
extern void vga_cleanup(void);
extern int vga_open(struct file *,struct inode *);
extern int vga_close(struct file *,struct inode *);
extern int vga_read(struct file *,_user_out_ char *,foff_t,_user_out_ foff_t *,int cnt);
extern int vga_write(struct file *,_user_in_ const char *,foff_t,_user_out_ foff_t *,int cnt);
extern int vga_ioctl(struct file *,int cmd,int param);

/*VGA操作命令*/
#define VGA_CMD_SET_RESET		0x00000001		/*清空输出缓冲区*/
#define VGA_CMD_SET_DIS_LINE		0x00000003		/*设置显示起始行号*/
#define VGA_CMD_ERASE_LINES		0x00000004		/*擦除指定行后的若干行*/
	#define MAKE_ERASE_PARAM(start,cnt) 		((((start)&0x0000FFFF)<<16)|((cnt)&0x0000FFFF))
	#define GET_ERASE_PARAM_START(param) 		((param)>>16)
	#define GET_ERASE_PARAM_CNT(param) 		((param)&0x0000FFFF)
#define VGA_CMD_SET_CURSOR		0x00000005		/*设置光标位置*/
	#define MAKE_POSITION(x,y)	((0xFFFF0000&(y<<16))|(x&0x0000FFFF))
	#define GET_POS_X(pos)		((pos)&0x0000FFFF)
	#define GET_POS_Y(pos)		((pos)>>16)
#define VGA_CMD_SET_CURSOR_HIDE	0x00000006	/*隐藏光标*/
#define VGA_CMD_SET_CURSOR_DIS	0x00000007		/*显示光标*/

extern int vga_kread(struct inode *,_core_out_ char *,foff_t,_core_out_ foff_t *,int cnt);
extern int vga_kwrite(struct inode *,_core_in_ const char *,foff_t,_core_out_ foff_t *,int cnt);

/* VGA基本参数
 * @ vga_mem_base: VGA显存基地址,总为0xB8000
 * @ c_x_y: 当前光标所在位置
 * @ c_max_line: 最多有多少行
 * @ c_dis_line: 当前在屏幕最上方显示的是第几行
 */
struct vga_private
{
	unsigned short *vga_mem_base;
	unsigned int c_x_y;
	unsigned short c_max_line;

	unsigned short c_dis_line;
	unsigned short c_cur_status;
#define VGA_CURSOR_STATUS_HIDE	0x0001
#define VGA_CURSOR_STATUS_SHOW	0x0002
};
/*VGA 参数*/
#define VGA_MAX_LINE		204		/*0xB8000~0xBFFF */
#define VGA_MAX_DIS_LINE 179		/*显示的*/
#define VGA_MAX_WIDTH	80		/*宽度为80		 */
static inline int line_to_addr(unsigned short line,unsigned short ** p)
{
	if (line>VGA_MAX_LINE) return INVALID;
	*p = (unsigned short *)VGA_DEFAULT_BASE;
	*p = line * 80;
	return 1;
}

int vga_reset(struct vga_private *);
int vga_eraselines(struct vga_private *,unsigned short,unsigned short);
int vga_erasearea(struct vga_private *,unsigned short,unsigned short);
int vga_setdisline(struct vga_private *,unsigned short);
int vga_setcursorpos(struct vga_private *,unsigned int);
int vga_hidecursor(struct vga_private *);
int vga_showcursor(struct vga_private *);

/*保留给printk使用*/
extern struct vga_private vga_pivat;

/*VGA 操作端口定义*/
#define VGA_REG_ADDR				0x3D4
#define VGA_REG_DATA				0x3D5
#define VGA_INDEX_DIS_ADDR_HIGH	0x0C
#define VGA_INDEX_DIS_ADDR_LOW		0x0D
#define VGA_INDEX_CURSOR_HIGH		0x0E
#define VGA_INDEX_CURSOR_LOW		0x0F

#endif /*_VGA_*/
















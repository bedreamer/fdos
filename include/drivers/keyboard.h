/*
 *	keyboard.h
 *	bedreamer@163.com
 *	Thursday, May 31, 2012 07:49:22 CST 
 */
#ifndef _KEYBOARD_
#define _KEYBOARD_

#define _8042OUTPUTBUF	0x60
#define _8042INPUTBYF	0x60
#define _8042STATUSREG	0x64
#define _8042CTLREG		0x64
#define _8042CMDMOURSE	0xD4

/*Intel 8042命令*/
#define _8042CMD_SETMOURSE_1X1_				0xE6/*设置鼠标比例为1：1*/
#define _8042CMD_SETMOURSE_2X1_				0xE7/*设置鼠标比例为2：1*/
#define _8042CMD_SETMOURSE_RESOLUTION		0xE8/*设置鼠标分辨率*/
#define _8042CMD_GETMOURSE_INFO				0xE9/*获取鼠标信息*/
#define _8042CMD_WRITE_LED					0xDE/*写键盘LED*/
#define _8042CMD_DETECT_REFLECT				0xEE/*诊断回声*/
#define _8042CMD_SETSWITCHSCANCODE			0xF0/*设置／获取交替扫描码*/
#define _8042CMD_GETKEYBORADID				0xF2/*读键盘ID*/
#define _8042CMD_GETMOURSEID				0xF2/*读鼠标ID*/
#define _8042CMD_SETREPEATINPUT				0xF3/*设置重复键入信息*/
#define _8042CMD_SETMOUSESIMPLE				0xF3/*设置鼠标采样率*/
#define _8042CMD_KEYBOARDOPEN				0xF4/*键盘开放*/
#define _8042CMD_SETDEFAULTDISABLEKEY		0xF5/*设置默认和禁止键盘*/
#define _8042CMD_SETDEFAULTDISABLEMOUESE	0xF6/*设置默认和禁止鼠标*/
#define _8042CMD_SETALLREPEAT				0xF7/*将所有的键设为重复按键*/
#define _8042CMD_SETALLPRESSORRELEASE		0xF8/*设置所有按键为按键/释放*/
#define _8042CMD_SETALLPRESS				0xF9/*设置所有键为按键*/
#define _8042CMD_SETALLREPEATPRESS			0xFA/*将所有的键设为重复按键/按键*/
#define _8042CMD_SETONEREPEAT				0xFB/*将某个按键设为重复按键*/
#define _8042CMD_SETKEYTRASLATE				0xFC/*将某个键设为按键/翻译*/
#define _8042CMD_SETONLYPRESS				0xFD/*将某个键设为仅按键*/
#define _8042CMD_RESEND						0xFE/*重新发送*/
#define _8042CMD_RESTART					0xFF/*键盘重启*/
#define _8042CMD_MOURSERESTART				0xFF/*鼠标重启*/

void init8042(void);
_u8 _8042readscancode(void);
void _8042cmdmourse(void);
void _8042setresolution1x1(void);
void _8042setresolution2x1(void);
#define RESOLUTION_25	0
#define RESOLUtION_50	1
#define RESOLUTION_100	2
#define RESOLUTION_200	3
void _8042setresolution(_u8);
#define MOURSE_ENABLE		0x00100000
#define MOURSE_LBTNDOWN		0x00040000
#define MOURSE_RBTNDOWN		0x00010000
#define MOURSE_DPI			0x00000300
#define MOURSE_RESOLUTION	0x000000FF
#define mourseenable(_u32_) (MOURSE_ENABLE&_u32_)>>20
#define mourselbtndown(_u32_) (MOURSE_LBTNDOWN&_u32_)>>18
#define mourserbtndown(_u32_) (MOURSE_RBTNDOWN&_u32_)>>16
#define moursedpi(_u32_) (MOURSE_DPI&_u32_)>>8
#define mourseresolution(_u32_) (MOURSE_RESOLUTION&_u32_)
_u32 _8042getmourseinfo(void);
#define LEDCAPSLOCKON	0x04
#define LEDNUMLOCKON	0x02
#define LEDSCROLLOCKON	0x01
void _8042setled(_u8 led);
_u8 _8042detect(void);
void _8042keyboardenable(void);
void _8042mourseenable(void);

/************************************************************************/
/*                          Macros Declaration                          */
/************************************************************************/
#define	KB_IN_BYTES		32			/* size of keyboard input buffer */
#define MAP_COLS		3			/* Number of columns in keymap */
#define NR_SCAN_CODES	0x80		/* Number of scan codes (rows in keymap) */

#define FLAG_BREAK		0x0080		/* Break Code			*/
#define FLAG_EXT		0x0100		/* Normal function keys	*/
#define FLAG_SHIFT_L	0x0200		/* Shift key			*/
#define FLAG_SHIFT_R	0x0400		/* Shift key			*/
#define FLAG_CTRL_L		0x0800		/* Control key			*/
#define FLAG_CTRL_R		0x1000		/* Control key			*/
#define FLAG_ALT_L		0x2000		/* Alternate key		*/
#define FLAG_ALT_R		0x4000		/* Alternate key		*/
#define FLAG_PAD		0x8000		/* keys in num pad		*/

#define MASK_RAW	0x01FF		/* raw key value = code passed to tty & MASK_RAW
					   				the value can be found either in the keymap column 0
					   				or in the list below */

/* Special keys */
#define ESC			(0x01 + FLAG_EXT)	/* Esc		*/
#define TAB			(0x02 + FLAG_EXT)	/* Tab		*/
#define ENTER		(0x03 + FLAG_EXT)	/* Enter	*/
#define BACKSPACE	(0x04 + FLAG_EXT)	/* BackSpace*/

#define GUI_L		(0x05 + FLAG_EXT)	/* L GUI	*/
#define GUI_R		(0x06 + FLAG_EXT)	/* R GUI	*/
#define APPS		(0x07 + FLAG_EXT)	/* APPS	*/

/* Shift, Ctrl, Alt */
#define SHIFT_L		(0x08 + FLAG_EXT)	/* L Shift	*/
#define SHIFT_R		(0x09 + FLAG_EXT)	/* R Shift	*/
#define CTRL_L		(0x0A + FLAG_EXT)	/* L Ctrl	*/
#define CTRL_R		(0x0B + FLAG_EXT)	/* R Ctrl	*/
#define ALT_L		(0x0C + FLAG_EXT)	/* L Alt	*/
#define ALT_R		(0x0D + FLAG_EXT)	/* R Alt	*/

/* Lock keys */
#define CAPS_LOCK	(0x0E + FLAG_EXT)	/* Caps Lock	*/
#define	NUM_LOCK	(0x0F + FLAG_EXT)	/* Number Lock	*/
#define SCROLL_LOCK	(0x10 + FLAG_EXT)	/* Scroll Lock	*/

/* Function keys */
#define F1			(0x11 + FLAG_EXT)	/* F1		*/
#define F2			(0x12 + FLAG_EXT)	/* F2		*/
#define F3			(0x13 + FLAG_EXT)	/* F3		*/
#define F4			(0x14 + FLAG_EXT)	/* F4		*/
#define F5			(0x15 + FLAG_EXT)	/* F5		*/
#define F6			(0x16 + FLAG_EXT)	/* F6		*/
#define F7			(0x17 + FLAG_EXT)	/* F7		*/
#define F8			(0x18 + FLAG_EXT)	/* F8		*/
#define F9			(0x19 + FLAG_EXT)	/* F9		*/
#define F10			(0x1A + FLAG_EXT)	/* F10		*/
#define F11			(0x1B + FLAG_EXT)	/* F11		*/
#define F12			(0x1C + FLAG_EXT)	/* F12		*/

/* Control Pad */
#define PRINTSCREEN	(0x1D + FLAG_EXT)	/* Print Screen	*/
#define PAUSEBREAK	(0x1E + FLAG_EXT)	/* Pause/Break	*/
#define INSERT		(0x1F + FLAG_EXT)	/* Insert	*/
#define DELETE		(0x20 + FLAG_EXT)	/* Delete	*/
#define HOME		(0x21 + FLAG_EXT)	/* Home		*/
#define END			(0x22 + FLAG_EXT)		/* End	*/
#define PAGEUP		(0x23 + FLAG_EXT)	/* Page Up	*/
#define PAGEDOWN	(0x24 + FLAG_EXT)	/* Page Down*/
#define UP			(0x25 + FLAG_EXT)		/* Up	*/
#define DOWN		(0x26 + FLAG_EXT)	/* Down		*/
#define LEFT		(0x27 + FLAG_EXT)	/* Left		*/
#define RIGHT		(0x28 + FLAG_EXT)	/* Right	*/

/* ACPI keys */
#define POWER		(0x29 + FLAG_EXT)	/* Power	*/
#define SLEEP		(0x2A + FLAG_EXT)	/* Sleep	*/
#define WAKE		(0x2B + FLAG_EXT)	/* Wake Up	*/

/* Num Pad */
#define PAD_SLASH	(0x2C + FLAG_EXT)	/* /		*/
#define PAD_STAR	(0x2D + FLAG_EXT)	/* *		*/
#define PAD_MINUS	(0x2E + FLAG_EXT)	/* -		*/
#define PAD_PLUS	(0x2F + FLAG_EXT)	/* +		*/
#define PAD_ENTER	(0x30 + FLAG_EXT)	/* Enter	*/
#define PAD_DOT		(0x31 + FLAG_EXT)	/* .		*/
#define PAD_0		(0x32 + FLAG_EXT)	/* 0		*/
#define PAD_1		(0x33 + FLAG_EXT)	/* 1		*/
#define PAD_2		(0x34 + FLAG_EXT)	/* 2		*/
#define PAD_3		(0x35 + FLAG_EXT)	/* 3		*/
#define PAD_4		(0x36 + FLAG_EXT)	/* 4		*/
#define PAD_5		(0x37 + FLAG_EXT)	/* 5		*/
#define PAD_6		(0x38 + FLAG_EXT)	/* 6		*/
#define PAD_7		(0x39 + FLAG_EXT)	/* 7		*/
#define PAD_8		(0x3A + FLAG_EXT)	/* 8		*/
#define PAD_9		(0x3B + FLAG_EXT)	/* 9		*/
#define PAD_UP		PAD_8				/* Up		*/
#define PAD_DOWN	PAD_2				/* Down		*/
#define PAD_LEFT	PAD_4				/* Left		*/
#define PAD_RIGHT	PAD_6				/* Right	*/
#define PAD_HOME	PAD_7				/* Home		*/
#define PAD_END		PAD_1				/* End		*/
#define PAD_PGUP	PAD_9				/* Page Up	*/
#define PAD_PGDOWN	PAD_3			/* Page Down*/
#define PAD_INS		PAD_0				/* Ins		*/
#define PAD_MID		PAD_5				/* Middle key*/
#define PAD_DEL		PAD_DOT				/* Del		*/

/*控制按键掩码*/
#define CAPS_LOCKED 		(keystatus|=0x80000000)
#define CAPS_UNLOCK 		(keystatus&=0x7FFFFFFF)
#define CHECK_CAPS(key)  	(key&0x800000000)

#define LSHIFT_HOLD 		(keystatus|=0x40000000)
#define LSHIFT_UNHOLD		(keystatus&=0xBFFFFFFF)
#define CHECK_LSHIFT(key)	(key&=0x40000000)

#define LCTL_HOLD			(keystatus|=0x20000000)
#define LCTL_UNHOLD			(keystatus&=0xDFFFFFFF)
#define CHECK_LCTL(key)		(key&=0x20000000)

#define LALT_HOLD			(keystatus|=0x10000000)
#define LALT_UNHOLD			(keystatus&=0xEFFFFFFF)
#define CHECK_LALT(key)		(key&=0x10000000)

#define RSHIFT_HOLD 		(keystatus|=0x04000000)
#define RSHIFT_UNHOLD		(keystatus&=0xFBFFFFFF)
#define CHECK_RSHIFT(key)	(key&=0x04000000)

#define RCTL_HOLD			(keystatus|=0x02000000)
#define RCTL_UNHOLD			(keystatus&=0xFDFFFFFF)
#define CHECK_RCTL(key)		(key&=0x02000000)

#define RALT_HOLD			(keystatus|=0x01000000)
#define RALT_UNHOLD			(keystatus&=0xFEFFFFFF)
#define CHECK_RALT(key)		(key&=0x01000000)

extern _u32 keymap[NR_SCAN_CODES * MAP_COLS];

/*控制键的make code.*/
#define MAKE_CODE_CAPS_LCK		0x3A
#define MAKE_CODE_L_SHIFT		0x2A
#define MAKE_CODE_R_SHIFT		0x36
#define MAKE_CODE_L_CTL			0x1D
#define MAKE_CODE_R_CTL			0x8D
#define MAKE_CODE_L_ALT			0x38
#define MAKE_CODE_R_ALT			0xE0

/*控制键的break code.*/
#define BREAK_CODE_L_SHIFT		0xAA
#define BREAK_CODE_R_SHIFT		0xB6
#define BREAK_CODE_L_CTL		0x9D
#define BREAK_CODE_L_ALT		0xB8

/*其他功能键*/
#define MAKE_CODE_ESC			0x01
#define MAKE_CODE_TAB			0x0F
#define MAKE_CODE_ENTER			0x1C
#define MAKE_CODE_F1			0x3B
#define MAKE_CODE_F2			0x3C
#define MAKE_CODE_F3			0x3D
#define MAKE_CODE_F4			0x3E
#define MAKE_CODE_F5			0x3F
#define MAKE_CODE_F6			0x40
#define MAKE_CODE_F7			0x41
#define MAKE_CODE_F8			0x42
#define MAKE_CODE_F9			0x43
#define MAKE_CODE_F10			0x44
#define MAKE_CODE_F11			0x57
#define MAKE_CODE_F12			0x58
#define MAKE_CODE_PGUP			0x49
#define MAKE_CODE_PGDWN			0x51
#define MAKE_CODE_LEFT			0x4B
#define MAKE_CODE_UP			0x48
#define MAKE_CODE_RIGHT			0x4D
#define MAKE_CODE_DOWN			0x50

#endif /*_KEYBOARD_*/

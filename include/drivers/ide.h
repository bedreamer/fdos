/*
 *	ide.h
 *	bedreamer@163.com
 *	Monday, May 28, 2012 03:59:21 CST 
 */
#ifndef _HD_
#define _HD_

#include <stddef.h>
#include <kernel/kernel.h>

#define SECTOR_SIZE					512
#define MAX_PARTATION_CNT			256
#define IDE_MAX_IODELAY				0x20000

/*ide device status*/
#define IDES_BUSY					0x80		// Busy (see 6.2.3)
#define IDES_READY					0x40		// Device Ready (see 6.2.8)
#define IDES_FAULT					0x20		// Device Fault (see 6.2.7)
#define IDES_STREAMERROR			IDES_FAULT	// Stream Error (see 6.2.11)
#define IDES_DEFERREDWRITEERROR	0x10		// Deferred Write Error (see 6.2.6)
#define IDES_REQESTDATA				0x08		// Data Request (see 6.2.5)
#define IDES_ALIGNERROR				0x04		// Obsolete or Alignment Error (see 6.2.2)
#define IDES_DATAAVAILABLE			0x02		// Obsolete or Sense Data Available (see 6.2.10)
#define IDES_CHECKCONDITION			0x01		// Check Condition (see 6.2.4) or Error (see 6.2.9)
/*ide device error.*/								//7:4 Sense Key (see 6.3.12)
#define IDEE_CRC_ERROR				0x80		//7 Interface CRC (see 6.3.10)
#define IDEE_TABLEWRONG				0x40		//6 Uncorrectable Error (see 6.3.13)
#define IDEE_OBSOLETE5				0x20		//5 Obsolete
#define IDEE_IDNOTFOUND				0x10		//4 ID Not Found (see 6.3.6)
#define IDEE_OBSOLETE3				0x80		//3 Obsolete
#define IDEE_ABORT					0x40		//2 Abort (see 6.3.2)
#define IDEE_ENDM					0x20		//1 End of Media (see 6.3.5)
#define IDEE_ILLEGALLEN				0x10		//0 Illegal Length Indicator (see 6.3.7) or
												//Command Completion Time Out (see 6.3.4) or
												//Media Error (see 6.3.11) or
												//Attempted Partial Range Removal (see 6.3.3) or
												//Insufficient NV Cache space (see 6.3.9) or
												//Insufficient LBA Range Entries Remaining (see 6.3.8)
/*ide device commands.*/
#define IDEC_RESET					0x08		//reset ide device.
#define IDEC_READBUFFER				0xE4		//read ide device buffer.
#define IDEC_WRITEBUFFER			0xE8		//write ide device buffer.
#define IDEC_READSECTORS			0x20		//read ide device sectors.
#define IDEC_WRITESECTORS			0x30		//write ide device sectors.
#define IDEC_SLEEP					0xE6		//set ide device sleep.
#define IDEC_IDENTIFY				0xEC		//indentify.
#define IDEC_IDENTIFY_PACKET		0xA1		//indentify packet device.
#define IDEC_DEV_DIAGNOSTIC			0x90		//diagnostic the device.

/*register for read.*/
#define REG_FOR_READ	0xCC00
/*register for write*/
#define REG_FOR_WRITE	0x00CC
/* IDE contral ioport */
struct ide_ctl_ioport 
{
	int  reg_indentify;
	_u16 reg_data;
	union{
		_u16 reg_error;
		_u16 reg_features;
	}reg_ef;
	_u16 reg_count;
	_u16 reg_lba_low;
	_u16 reg_lba_mid;
	_u16 reg_lba_high;
	_u16 reg_device;
	union{
		_u16 reg_status;
		_u16 reg_cmd;
	}reg_sc;
	union{
	_u16 reg_altstatus;
	_u16 reg_dev_ctl;
	}reg_ad;
};

/*max count of IDE device.*/
#define MAX_IDE_DEVICE		64
/*mask for primary device.*/
#define IDE_PRIMARY_MASK	0x1000
/*mask for secondy device.*/
#define IDE_SECONDY_MASK	0x0100
/*mask for master device.*/
#define IDE_MASTER_MASK		0x0010
/*mask for slave device.*/
#define IDE_SLAVE_MASK		0x0001
/*IDE device partition struct
 *@devnum : device number.
 *@devname: device name. /device_hd1 etc.
 *@lba_start: device start LBA.
 *@lba_end: device end LBA.
 *@lba_cnt: device LBA count.
 *@ide_packet_index: packet index.
 *@ide_packet_name: IDE device packet name.
 *@reg_for_device: indentify for master and slave device.
 *@ide_dev_mask: ide device mask.
 *@ide_port_r: device read contral port.
 *@ide_port_w: device write contral port.
 */
struct ide_device_status 
{
	size_t devnum;
	size_t lba_start;
	size_t lba_end;
	size_t lba_cnt;
	_u16 ide_dev_mask;
	const struct ide_ctl_ioport *ide_port_r;
	const struct ide_ctl_ioport *ide_port_w;
};

/*hard disk command struct.*/
struct ide_cmd_struct 
{
	_u8 reg_features;
	_u8 reg_count;
	_u8 reg_lba_low;
	_u8 reg_lba_mid;
	_u8 reg_lba_high;
	_u8 reg_device;
	_u8 reg_cmd;
	_u8 reg_dev_ctl;
};
#define MAKEDEV(lbahigh,drv) ((drv<<4)|((lbahigh)&0x0F)|0xA0)

/*hard disk I/O interface.*/
extern void ide_init(void);

/*mask for IDE device interrupt.*/
#define IDE_NO_INT		0x0000
#define IDE_GET_INT		0x00FF
#define IDE_WAIT_INT	0xFF00

//#define IDE_DEBUG

#endif /*_HD_*/


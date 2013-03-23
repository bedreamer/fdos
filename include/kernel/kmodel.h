/*
 *	kmodel.h
 *	bedreamer@163.com
 *	Wed 07 Nov 2012 11:08:11 PM CST 
 *  内核抽象对象的定义.
 *	抽象后内核具有如下特性:
 *	#	支持同时挂载最大26个块设备,编号 0x00,0x01,...,0x19。
 *	#	最多支持64类设备,每类设备最多支持15个从设备
 *	#	继续采用UNIX抽象方式，设备即文件，所有对设备的访问都通过对文件的访问完成。
 *	#	设备节点处于 0:/dev/。
 *	#	添加对目录的支持。
 *	#	添加对超级块的支持，提供统一标准。
 *	#	每个进程对多可以打开256个文件，即有256个文件插槽。
 *	#	每个进程最多可创建64个线程。
 *	#	添加模块支持。
 *	#	内核中名称采用宽字符表示.
 *	#	根目录结构如下
 *		0:\
 *		  +--dev/
 *        .    +--null
 *        .    +--zero
 *        .    +--random
 *        .    +--stdin
 *        .    +--stdout
 *        .    +--stderr
 *        .    +--tty1
 *        .    +--tty2
 *        .    +--tty3
 *        .    +--tty4
 *        +--etc/
 *        .    +--environment
 *		  +--sbin/
 *		  .    +--Init
 *		  .	   +--mshell
 *	#	文件名格式 [0-26]:\{0-9a-zA-Z}*
 *	#	为常用的数据结构创建缓存池实现快速分配释放常用数据结构有
 *		+--struct dir;
 *		+--struct inode;
 *		+--struct file;
 *	#	调整键盘驱动功能，将功能单一化，键盘驱动仅进行设备检测，读取键盘扫描码,功能键的解析
 * 		键盘其他扫描码解析放到上层应用来做.
 */
#ifndef _KMODEL_
#define _KMODEL_

/*无效的句柄*/
#define INVALID					-1
#define VALID					 1
#define EOF						-1

/*标准输入输出定义*/
#define stdin 					0x00
#define stdout 					0x01
#define stderr					0x02

/*文件打开模式*/
#define MODE_READ				0x00000001UL
#define MODE_READWRITE			0x00000002UL
#define MODE_CREATE				0x00000004UL
#define MODE_CREATEALWAYS		0x00000008UL
#define MODE_IOCTL				0x00000010UL

/*设备挂载模式*/
#define MOUNT_RO				0x00000001UL
#define MOUNT_RW				0x00000002UL

/*节点名最长为14个字节.*/
#define K_MAX_LEN_NODE_NAME		14
/*文件最长路径*/
#define K_MAX_PATH_LEN			128

/* 节点属性
 * @ i_name: 节点名
 * @ t_create: 节点创建时间
 * @ d_create: 节点创建日期
 * @ t_lastaccess: 节点最后访问时间
 * @ d_lastaccess: 节点最后访问日期
 * @ i_size: 节点大小,块设备表示具有的LBA个数
 * @ i_type: 该节点代表的文件类型
 * @ i_devnum: 若该节点为设备则表示设备号
 */
struct itemattrib
{
	char i_name[K_MAX_LEN_NODE_NAME];
	time_t t_create;
	date_t d_create;
	time_t t_lastaccess;
	date_t d_lastaccess;
	size_t i_size;
	unsigned int i_type;
#define ITYPE_ALIVE		0x10000000
#define ITYPE_ARCHIVE	(ITYPE_ALIVE|0x00000001)
#define ITYPE_DIR		(ITYPE_ALIVE|0x00000002)
#define ITYPE_DEVICE	(ITYPE_ALIVE|0x00000010)
#define ITYPE_CHAR_DEV	(ITYPE_DEVICE|0x00000004)
#define ITYPE_BLK_DEV	(ITYPE_DEVICE|0x00000008)
	dev_t i_devnum;
};

//#define _KERNEL_
//#ifdef _KERNEL_
/*最多支持同时挂载26个块设备*/
#define K_MNT_MAX_BLK_DEV		26
/*最多支持64个设备*/
#define K_MAX_DEV_SUPPORT		64
/*最多支持16个从设备*/
#define K_MAX_SLAVE_DEV_SUPPORT	16
/*系统所在块设备编号*/
#define K_SYS_BLK_DEV_NUM		0x00
/*每个进程可打开的最大文件数*/
#define K_MAX_FILE_OPEN_PPS		256
/*每个进程可创建线程的最大数*/
#define K_MAX_THREAD_CNT_PPS	64
/*文件系统种类*/
#define K_FS_CATORAY_CNT		256
/*块设备卷标长度*/
#define K_LABLE_MAX_LEN			16
/*块设备读写大小*/
#define SECTOR_SIZE				512

#include <kernel/kernel.h>
#include <kernel/schedu.h>

/*模块结构定义
 * @ module_startup: 模块初始化时调用.
 * @ module_cleanup: 模块清除时使用.
 * @ m_lst: 模块链表节点.
 */
struct module
{
	int (*module_startup)(void);
	void (*module_cleanup)(void);
	struct list_head m_lst;
};

/*所有者结构
 * @ ptsk: 所有者的任务结构指针
 * @ pthsk: 所有者的线程指针
 */
struct owner_struct
{
	struct task_struct *ptsk;
	struct kthread_struct *pthsk;
};

/* 节点通用信息
 * @ i_attrib: 节点属性
 * @ i_private: 文件系统或设备节点私有数据
 * @ i_rcnt: 节点被引用次数
 * @ i_volnum: 若为文件则表示节点所在卷号
 * @ d_parent: 父目录节点指针
 * @ i_root: 节点所在卷
 * @ d_op: 目录操作接口
 * @ f_op: 作为字符设备或普通文件的接口
 */
struct itemdata
{
	struct itemattrib i_attrib;
	void *i_private;
	unsigned int i_rcnt;
	unsigned char i_volnum; /* 0 ~ 25 */

	struct dir *d_parent;
	struct mntpnt_struct *i_root;

	struct dir_op *d_op;
	struct file_op *f_op;
};

/*在内核中表示一个打开的文件,可以是设备，也可以是普通文件.
 * @ f_pi: 指向打开的文件节点。
 * @ f_owner: 打开这个文件的进程信息。
 * @ f_mode: 打开模式
 * @ f_private: TTY驱动保留
 * @ f_lst: inode链表节点
 */
struct file
{
	struct inode *f_pi;
	struct owner_struct f_owner;
	unsigned int f_mode;

	void *f_private;	/*For TTY driver.*/

	struct list_head f_lst;
};

/*内核文件节点,可以是普通文件也可以是设备
 * @ i_brothers: 兄弟节点.
 * @ i_data: 节点通用信息.
 * @ f_lst: 引用该节点的链表
 * @ lck_f_lst: f_lst的访问控制锁
 */
struct inode
{
	struct list_head i_brother;
	struct itemdata i_data;

	struct file *f_lst;
	struct spin_lock lck_f_lst;
};

/*目录抽象结构
 * @ d_child: 子目录头指针
 * @ lck_d_child: 子目录链表操作锁
 * @ i_child: 子文件头指针
 * @ lck_i_child: 子文件链表操作锁
 * @ d_brother: 目录链表节点.
 * @ d_data: 文件节点通用信息.
 */
struct dir
{
	struct spin_lock lck_d_child;
	struct dir *d_child;

	struct spin_lock lck_i_child;
	struct inode *i_child;

	struct list_head d_brother;
	struct itemdata d_data;
};

/*超级块抽象结构
 * @ v_lable: 卷标
 * @ v_start: 卷起始LBA地址
 * @ v_cnt: 卷有效LBA地址个数
 */
struct superblk
{
	char v_lable[K_LABLE_MAX_LEN];
	size_t v_lbastart;
	size_t v_lbacnt;
};

/*文件系统结构定义
 * @ fs_id: 文件系统ID，0～255
 * @ fs_name: 文件系统名
 * @ f_op: 文件操作接口.
 * @ d_op: 文件夹操作接口.
 * @ mount: 挂载函数.
 * @ umount: 卸载函数.
 */
struct fs_struct
{
	unsigned char fs_id;
	const char *fs_name;

	struct file_op *f_op;
	struct dir_op *d_op;

	int (*mkfs)(struct inode *,const char *);
	int (*mount)(struct inode *,struct itemdata *,void **);
	int (*umount)(struct inode *,void **);
};
extern int register_fs(struct fs_struct *);

/* 块设备挂载点结构
 * @ mnt_num: 挂载目标设备的号码, 0 ~ 31
 * @ mnt_dev: 挂载设备的指针
 * @ mnt_pfs: 挂载分区的文件系统指针
 * @ mnt_rcnt: 该挂载点被引用的次数,打开一个文件加1,关闭一个文件减1
 * @ mnt_mode: 挂载模式
 * @ mnt_root: 挂载的根目录
 * @ mnt_superblk: 超级快通用信息
 * @ mnt_private: 文件系统私有数据
 */
struct mntpnt_struct
{
	unsigned char mnt_num;
	unsigned char mnt_flg;
#define MNT_DONE	0x01
	struct inode *mnt_dev;
	struct fs_struct *mnt_pfs;

	unsigned int mnt_rcnt;
	unsigned int mnt_mode;

	struct dir mnt_root;
	struct superblk mnt_superblk;
	void *mnt_private;
};

/*文件操作接口。
 * @ open : 打开一个文件（设备）
 * @ closr: 关闭一个文件（设备）
 * @ read: 读取文件数据到用户地址空间
 * @ write: 从用户地址空间写数据到文件
 * @ ioctl: 对设备的控制
 * @ kread: 将数据读到内核地址空间
 * @ kwrite: 将内核地址空间的数据写回文件
 */
struct file_op
{
	int (*open)(struct file *,struct inode *);
	int (*close)(struct file *,struct inode *);

	int (*read)(struct file *,_uo char *,foff_t,_uo foff_t *,int);
	int (*write)(struct file *,_ui const char *,foff_t,_uo foff_t *,int);
	int (*ioctl)(struct file *,int,int);

	/*内核方法按照块大小进行传递(512 bytes)*/
	int (*kread)(struct itemdata *,_co char *,foff_t,int);
	int (*kwrite)(struct itemdata *,_ci const char *,foff_t,int);
};

/*目录操作接口，块设备有效
 * 相对来说这个结构是针对内核提供的功能.
 * @ mknode: 在指定目录中创建新节点
 * @ touch: 在指定目录中创建非文件节点
 * @ mkdir: 在指定目录中创建目录节点
 * @ rm : 删除非目录节点
 * @ rmdir: 删除目录节点
 * @ rename: 重命名节点
 * @ opendir: 仅限于打开一个文件夹节点
 * @ closedir: 仅限于关闭一个打开的文件夹节点
 * @ openinode: 打开一个非文件夹节点
 * @ closeinode: 关闭一个非文件夹节点
 * @ readitem: 按索引查找节点
 * @ readattrib: 读取指定节点的属性
 */
struct dir_op
{
	int (*mknode)(struct dir *,struct itemattrib *,_ci const char *);

	int (*touch)(struct dir *,_co struct itemattrib *,_ci const char *);
	int (*mkdir)(struct dir *,_co struct itemattrib *,_ci const char *);

	int (*rm   )(struct dir *,_ci const char *);
	int (*rmdir)(struct dir *,_ci const char *);

	int (*rename)(struct dir *,struct itemattrib *,_ci const char *);

	int (*opendir)(struct dir *,_co struct itemdata *,_ci const char *);
	int (*closedir)(struct dir *,struct itemdata *);

	int (*openinode)(struct dir *,_co struct inode *,_ci const char *);
	int (*closeinode)(struct dir *,_co struct inode *);

	int (*readitem)(struct dir *,_co struct itemattrib *,int);
	int (*readattrib)(struct dir *,_co struct itemattrib *,_ci const char *);
};
/*检查内核中是否存在指定的文件夹节点*/
extern int dir_check_coredir(struct dir *,_co struct dir **,_ci const char *);
/*检查内核中是否存在指定的非文件夹节点*/
extern int dir_check_coreinode(struct dir *,_co struct inode **,_ci const char *);
/*检查内核中是否存在指定节点*/
extern int dir_check_coreitem(struct dir *,_co struct itemdata **,_ci const char *);
/*检查文件系统中是否存在指定的节点*/
extern int dir_check_fsitem(struct dir *,_co struct itemattrib *,_ci const char *);
/*检查文件系统中是否存在指定的文件夹节点*/
extern int dir_check_fsdir(struct dir *,_co struct itemattrib *,_ci const char *);
/*检查文件系统中是否存在指定的非文件夹节点*/
extern int dir_check_fsinode(struct dir *,_co struct itemattrib *,_ci const char *);
/*检查系统中是否存在指定的节点*/
extern int dir_chech_item(struct dir *,_co struct itemattrib *,_ci const char *);
/*检查系统中是否存在指定的文件夹节点*/
extern int dir_check_dir(struct dir *,_co struct itemattrib *,_ci const char *);
/*检查系统中是否存在指定的非文件夹节点*/
extern int dir_check_inode(struct dir *,_co struct itemattrib *,_ci const char *);
/*执行空间分配，初始化相关数据,但不添加节点到内核节点树中.到这一步节点已经打开成功.*/
extern int dir_do_opendir(struct dir *,_co struct dir **,_ci const char *);
/*打开非文件夹节点，如果节点不存在则失败*/
extern int dir_do_openinode(struct dir *,_co struct inode **,_ci const char *);
/*打开文件节点，若文件节点不存在则创建节点*/
extern int dir_do_opennewinode(struct dir *,_co struct inode **,_ci const char *);
/*打开成功后需要执行该函数将节点添加到内核节点树中.*/
extern int dir_opendir_done(struct dir *,_ci struct dir *);
/*将打开的节点添加到内核节点树中*/
extern int dir_openinode_done(struct dir *,_ci struct inode *);
/*将没有被引用的目录空间释放,匹配opendir中的分配操作.*/
extern int dir_close_dir(struct dir *,_ci struct dir *);
/*将没有被引用的节点空间释放*/
extern int dir_close_inode(struct dir *,_ci struct inode *);
/*返回打开文件的结果*/
extern int sys_do_open(struct dir *,unsigned int,_ci const char *);
/*检查任务是否打开过指定文件*/
extern int sys_check_taskfile(struct task_struct *,const struct inode *);
/*设置任务打开的文件*/
extern int sys_set_taskfile(struct task_struct *,struct file *);
/*打开指定的文件节点*/
extern int sys_do_openfile(struct inode *,struct file **,unsigned int);

/* ioport region.
 *@start: region port start.
 *@end: region port ended.
 *@cnt: region port count.
 *@pmdl: which module hold this region.
 *@rlst: port region list node.
 */
struct ioport_region 
{
	unsigned short start;
	unsigned short end;
	unsigned short cnt;
	struct module_struct *pmdl;
	struct list_head rlst;
};

/*check if the region already registerd.*/
int ioport_region_check(unsigned short,unsigned short);
/*register an ioport region.*/
int ioport_region_register(struct ioport_region *);
/*unregister an ioport region.*/
struct ioport_region *ioport_region_unregister(unsigned short,unsigned short);

/*device number is made by major and slave number.
 *	+-------------+-----------------------+
 *	31           20                       0
 *	major : [31:20]
 *	slave : [19: 0]
 */
#define MAKEDEVNUM(major,slave) (((major)<<20)|((slave)&0x000FFFFF))
#define GETMAJOR(devnum) (devnum>>20)
#define GETSLAVE(devnum) (devnum&0x000FFFFF)

/*supported device major number pre-defination.*/
#define MAJOR_ZERO		0x000	/*zero device 		#0:/dev/zero	*/
#define MAJOR_RAM		0x001	/*ram device  		#0:/dev/ram		*/
#define MAJOR_KB		0x002	/*keyboard device 	#0:/dev/kb		*/
#define MAJOR_MS		0x003	/*mouse device 		#0:/dev/ms		*/
#define MAJOR_VGA		0x004	/*VGA device		#0:/dev/vga		*/
#define MAJOR_HDA		0x005	/*Hard disk device	#0:/dev/hda		*/
#define MAJOR_HDB		0x006	/*Hard disk device	#0:/dev/hdb		*/
#define MAJOR_HDC		0x007	/*Hard disk device	#0:/dev/hdc		*/
#define MAJOR_HDD		0x008	/*Hard disk device	#0:/dev/hdd		*/
#define MAJOR_TTY		0x009	/*TTY 设备*/
#define MAJOR_NULL		0x020	/*Null device		#0:/dev/null	*/
#define MAJOR_MAX		MAJOR_NULL
#define SLAVE_MAX		0x10	/*从设备号最大值*/

/*设备基本描述结构
 * @ dev_num: 设备号，（主设备号<<20|0）
 * @ dev_name: 设备名称
 */
struct device_base
{
	dev_t dev_num;
	const char *dev_name;
};

/*主设备结构定义
 * @ slave_dev: 从设备表
 * @ dev: 主设备
 * @ dev_cnt: 设备总数
 */
struct device
{
	struct device_base *slave_dev[K_MAX_SLAVE_DEV_SUPPORT];
	size_t dev_cnt;
};

/*设备驱动定义
 * @ dev_major_num: 驱动对应设备的的主设备号.
 * @ f_op: 驱动操作接口函数
 * @ d_op: 驱动操作接口函数
 */
struct driver
{
	unsigned int dev_major_num;
	struct file_op *f_op;
	struct dir_op *d_op;
};
extern int register_device(struct device_base *sdev);
extern struct device_base * unregister_device(dev_t devnum);
extern int register_driver(struct driver *);
extern struct driver *unregister_driver(unsigned int major);
extern struct device_base * find_device(dev_t devnum);
extern struct driver * search_driver(unsigned int major);
extern struct driver * find_driver(dev_t devnum);
/*将设备节点添加到内核中*/
extern struct inode *load_driver(dev_t devnum,const char *devnodename);
/*将设备节点从内核中卸载*/
extern int unload_driver(dev_t devnum);

extern int model_startup(void);
extern int module_init(void);

extern struct module *module_head;
extern unsigned int moudule_cnt;
extern struct fs_struct *fs_table[K_FS_CATORAY_CNT];
extern struct mntpnt_struct devroot[K_MNT_MAX_BLK_DEV];
extern struct device device_head[K_MAX_DEV_SUPPORT];
extern struct driver *driver_head[K_MAX_DEV_SUPPORT];

#define MODULE_INIT(modulename) extern int modulename##_startup(void);modulename##_startup();
#define DO_MODULE_INIT_END }while(0);
#define DO_MODULE_INIT_START do {

//#endif /*KERNEL_*/
#endif /*_KMODEL_*/

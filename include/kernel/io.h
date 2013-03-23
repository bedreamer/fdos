/**
  Copyright (C) 2012, 2013 Lijie.
  <cuplision@163.com>
 */
#ifndef _IO_H_
#define _IO_H_

/* kernel return value fail. */
#define INVALID (-1)

/* open file mode. */
#define MODE_RO    1UL
#define MODE_RW    2UL
#define MODE_IOCTL 4UL

/* max file could be open */
#define NR_FILE   256  // 256 * 4 = 1K

/* standar I/O, error*/
#define stdin  0
#define stdout 1
#define stderr 2

/* inode type defination. */
#define I_FILE      0x01UL
#define I_DIR       0x02UL
#define I_CDEV      0x03UL
#define I_BDEV      0x04UL
/* FIXME. not implement yet. */
#define I_LINK      0x05UL

/* mounte mode */
#define MNT_RO      0x01UL
#define MNT_RW      0x02UL

/**
  device num.
  device number make up with master number and slave number.
  devnum:  [31:16] ---> master number.
           [15: 0] ---> slave number.
  example:
    devnum = 0x00010000:
      master number: 0x0001
      slave number: 0x0000
*/
#define MK_DEVNUM(m, s) ((((m)&0xFFFF)<<16)|((s)&0xFFFF))
#define DEVNUM_M(dn)    (((dn)>>16)&0xFFFF)
#define DEVNUM_S(dn)    ((dn)&0xFFFF)

#ifdef _KERNEL_

#define MAX_FS_NAME_LEN   16
#define MAX_DEV_NODE_LEN  MAX_FS_NAME_LEN
#define MAX_NODE_LEN      MAX_DEV_NODE_LEN

/* mask for metadata modifier. */
#define MASK_NAME 0x00000001  /* i_name */
#define MASK_LAC  0x00000002  /* lstacc */
#define MASK_SIZE 0x00000004  /* i_size */

/* fs read write-able attribute. */
#define FS_RO    0x01UL
#define FS_RW    0x02UL

struct inode;
struct file;
struct device;
struct cdev;
struct bdev;
struct device_driver;
struct mntpt_struct;
struct fops_struct;
struct iops_struct;
struct sbop_struct;
struct fs_struct;
typedef unsigned int f_mode;

/**
  filesystem struct. 
  @ fsid:  filesystem ID.
  @ fsattrib: indicate filesystem could be write or not.
  @ fsname: filesystem name.
  @ fops: filesystem driver file operate interfaces.
  @ iops: filesystem dir operate interfaces.
  @ sbop: filesystem super-block operate interfaces.
  @ nr_cnt: counter of filesystem referenced, reserved by kernel, 
            must initialize with 0.
 */
struct fs_struct
{
    unsigned char fsid;
    unsigned char fsattrib;
    const    char fsname[MAX_FS_NAME_LEN];
    const struct  fops_struct *fops;
    const struct  iops_struct *iops;
    const struct  sbop_struct *sbop;

    size_t nr_cnt;
};

/**
  mount point struct
  relation filesystem with device.
  @ fs: which filesystem was used.
  @ idir: which directory has been mounted.
  @ idev: which device has been mounted.
  @ mnt_mode: mounte mode, could be MNT_RO or MNT_RW, 
              depends filesystem attribute also.
  @ io_nr: how many inode opened. 
  @ fo_nr: how many file opened.
  @ mntlst: mount point list. 
 */
struct mntpt_struct
{
    struct fs_struct *fs;
    struct inode *idir;
    struct inode *idev;
    unsigned int mnt_mode;

    size_t io_nr;
    size_t fo_nr;
    struct list_head mntlst;
};

/**
  device struct.
 */
struct device
{
   dev_t devnum;
   const char *devname;

   struct inode *idev;
};

/**
  char device struct.
 */
struct cdev
{
    struct device *dev;
    struct fops_struct *fops;
};

/**
  block device struct. 
 */
struct bdev
{
    struct device dev;
};

/**
  device driver struct
 */
struct device_driver
{
};

/**
  inode metadata.
  @ i_name: node name. 
  @ t_create: time of node been created.
  @ d_create: date of node been created.
  @ t_lstacc: time of node been accessed last.
  @ d_lstacc: date of node been accessed last.
  @ i_size:
        FIXEME: ...
  @ i_type: inode type defination, coulde be one of
       I_FILE, I_DIR, I_CDEV, I_BDEV, I_LINK.
  @ i_devnum: device number, avaiable if i_type is 
    I_CDEV or I_BDEV only.
 */
struct metadata
{
    char i_name[MAX_NODE_LEN];
	time_t t_create; /* can not be modify since created. */
	date_t d_create; /* can not be modify since created. */
	time_t t_lstacc;
	date_t d_lstacc;
	size_t i_size;
	unsigned int i_type; /* same as t_create. */
	dev_t i_devnum;      /* same as t_create. */

#if 0 /* FIXME. maybe I'm not understandar this yet. */
    int  i_user, i_pr;
    int  i_grp, i_pr;
#endif
};

/**
  kernel file opened descripte struct. 
 */
struct file
{
    struct inode *fd;
    struct list_head flst;

    struct list_head flock;

    unsigned int o_mode;
    const struct fops_struct *fops;

    void *f_priva;
};

/**
  kernel file descripte struct.
  inode maybe a general file, directory, char device or block device.
 */
struct inode
{
    struct medadata idata;

    struct file *flst;
    int r_cnt;

    struct file *f_lock;
    unsigned int lockmask;

    struct inode *i_parent;
    struct inode *i_root;

    const struct iops_struct *iops;
    const struct fops_struct *fops;

    void *i_priva;
};

/**
  kenrel opened file I/O operate function struct. 
 */
struct fops_struct
{
    /* for user applation. */
    int (*open  )(struct inode *, struct file *, unsigned int mode);
    int (*close )(struct file *);
    int (*flush )(struct file *);
    int (*ioctl )(struct file *, int cmd, int param);
    int (*seek  )(struct file *, int offset, int base);
    int (*read  )(struct file *, _uo char *, foff_t, _uo foff_t *);
    int (*write )(struct file *, _ui char *, foff_t, _uo foff_t *);

    int (*lockme)(struct file *, int mask);
    int (*unlock)(struct file *);

    /* for kernel. */
    int (*kread )(struct inode *, _co char *, foff_t, _co foff_t *);
    int (*kwrite)(struct inode *, _ci char *, foff_t, _co foff_t *);
};

/**
  kernel inode I/O operate function struct.

  @mknode: make a node in core inode tree and filesystem if possiable. 
  @mkdir:  make a directory inode. 
  @touch:  make a file inode.
  @opendir: open a directory if exsit.
  @closedir: close a directory inode. 
  @rmnode: remove node from this directory. 
  @rmdir: remove emputy directory node from this directory.
  @rm: remove file from this directory. 
  @find: find a node in this directory. return a inode struct if exsit. 
  @lookup: look up is there a node exsit. JUST CHECK. 
  @update: update inode information.

 */
struct iops_struct
{
    struct inode *(*mknode)(struct inode *, dev_t, unsigned int);
    struct inode *(*mkdir )(struct inode *, const char *nodename);
    struct inode *(*touch )(struct inode *, const char *nodename);

    struct inode *(*opendir)(struct inode *, _ci const char *nodename);
    int (*closedir)(struct inode *);

    int (*rmnode )(struct inode *, _ci const char *nodename);
    int (*rmdir  )(struct inode *, _ci const char *nodename);
    int (*rm     )(struct inode *, _ci const char *nodename);

    struct inode *(*find  )(struct inode *, _ci const char *nodename);
    int (*lookup )(struct inode *, _co struct metadata *, _ci const char *nodename);

    int (*update )(struct inode *, const struct metadata *, unsigned int mask);
};

/**
  super block operate function struct.
 */
struct sbops_struct
{
    int (*fsmount )(struct inode *dev, struct inode *dir, unsigned int mode);
    int (*fsumount)(void);
};

#endif /*_KERNEL_*/
#endif /*_IO_H_*/



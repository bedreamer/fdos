/*
 *	kio.c
 *	bedreamer@163.com
 *	Monday, May 28, 2012 03:49:07 CST
 */
#include <kernel/kernel.h>
#include <kernel/signal.h>
#include <kernel/mm.h>
#include <kernel/schedu.h>
#include <kernel/kmodel.h>

/*支持的挂载点描述.*/
struct mntpnt_struct devroot[K_MNT_MAX_BLK_DEV]={{0}};

/*模块链表*/
struct module *module_head=NULL;
/*模块链表操作锁*/
struct spin_lock lck_module_head={0};
/*模块个数*/
unsigned int moudule_cnt=0;

/*文件系统信息*/
struct fs_struct *fs_table[K_FS_CATORAY_CNT]={0};

/*设备表*/
struct device device_head[K_MAX_DEV_SUPPORT]={{{0}}};
struct spin_lock lck_device={SPIN_UNLOCKED};
/*驱动表*/
struct driver *driver_head[K_MAX_DEV_SUPPORT]={NULL};

/*范围链表头.*/
struct ioport_region *irhead=NULL;
/*范围链表读写锁*/
struct spin_lock irlck={._lck=SPIN_UNLOCKED};

/*注册文件系统*/
int register_fs(struct fs_struct *fs)
{
	if (NULL==fs) return INVALID;
	if (K_FS_CATORAY_CNT<=fs->fs_id) return INVALID;
	if (NULL!=fs_table[fs->fs_id]) return INVALID;
	fs_table[fs->fs_id] = fs;
	return VALID;
}

/*check if the region already registerd.*/
int ioport_region_check(unsigned short start,unsigned short end)
{
	int result=0;
	get_spinlock(irlck);
	if (NULL==irhead) {
		result=0;
		goto out;
	}
	struct ioport_region *p=irhead;
	do {
		if (start >= p->start &&
		    start <= p->end ) {
		    result =1;
		    break;
		}
		if (end >= p->start && 
		    end <= p->end ) {
		    result =1;
		    break;
		}
		p=list_load(struct ioport_region,rlst,&(p->rlst));
	} while (p!=irhead);
out:
	release_spinlock(irlck);
	return result;
}

/*register an ioport region.*/
int ioport_region_register(struct ioport_region *pio)
{
	int result=0;

	if (NULL==pio) return 0;
	result=ioport_region_check(pio->start,pio->end);
	if (1==result) return 0;

	get_spinlock(irlck);
	list_ini(pio->rlst);

	if (NULL==irhead) {
		irhead=pio;
		goto out;
	}

	list_inserttail(&(irhead->rlst),&(pio->rlst));
out:
	release_spinlock(irlck);
	return 1;
}

/*unregister an ioport region.*/
struct ioport_region * 
ioport_region_unregister(unsigned short start,unsigned short end)
{
	int result=0;

	result=ioport_region_check(start,end);
	if (0==result) return NULL; /*region not registerd.*/

	get_spinlock(irlck);
	if (NULL==irhead) {
		result=0;
		goto out;
	}

	struct ioport_region *p=irhead,*pt=NULL;
	do {
		if (start == p->start &&
		    end == p->end ) {
		    pt=p;
		    break;
		}
		p=list_load(struct ioport_region,rlst,&(p->rlst));
	} while (p!=irhead);
out:
	release_spinlock(irlck);
	return pt;
}

/*只检查存在与内核中的文件夹节点*/
int dir_check_coredir(struct dir *pdir,_co struct dir **ppdir,_ci const char *name)
{
	if (NULL==pdir||NULL==ppdir||NULL==name||NULL==pdir->d_child) return INVALID;

	get_spinlock(pdir->lck_d_child);
	struct list_head *ph = &(pdir->d_child->d_brother);
	struct list_head *set= &(pdir->d_child->d_brother);
	struct dir *ths=NULL;
	do 
	{
		ths = list_load(struct dir,d_brother,ph);
		int result = 
			strncmp(ths->d_data.i_attrib.i_name,name,K_MAX_LEN_NODE_NAME);
		if (0!=result) ph = ph->next;
		else goto found;
	} while (ph!=set);

	*ppdir = NULL;
	release_spinlock(pdir->lck_d_child);
	return INVALID;
found:
	release_spinlock(pdir->lck_d_child);
	*ppdir = ths;
	return VALID;
}

/*只检查存在与内核中的非文件加节点*/
int dir_check_coreinode(struct dir *pdir,_co struct inode **ppi,_ci const char *name)
{
	if (NULL==pdir||NULL==ppi||NULL==name||NULL==pdir->i_child) return INVALID;

	get_spinlock(pdir->lck_i_child);
	struct list_head *ph = &(pdir->i_child->i_brother);
	struct list_head *set= &(pdir->i_child->i_brother);
	struct inode *ths=NULL;;
	do 
	{
		ths = list_load(struct inode,i_brother,ph);
		int result = 
			strncmp(ths->i_data.i_attrib.i_name,name,K_MAX_LEN_NODE_NAME);
		if (0!=result) ph = ph->next;
		else goto found;
	} while (ph!=set);
	* ppi = NULL;
	release_spinlock(pdir->lck_i_child);
	return INVALID;
found:
	release_spinlock(pdir->lck_i_child);
	* ppi = ths;
	return VALID;
}

/*检查内核中是否存在指定节点*/
int dir_check_coreitem(struct dir *pdir,_co struct itemdata **ppitm,_ci const char *nodename)
{
	if (NULL==pdir||NULL==ppitm||NULL==nodename) return INVALID;
	struct dir *ppdir=NULL;
	struct inode *pinode=NULL;

	int result = dir_check_coredir(pdir,&ppdir,nodename);
	if (INVALID!=result)
	{
		*ppitm = &(ppdir -> d_data);
		return VALID;
	}
	result = dir_check_coreinode(pdir,&pinode,nodename);
	if (INVALID!=result)
	{
		*ppitm = &(pinode -> i_data);
		return VALID;
	}
	return INVALID;
}

/*检查文件系统中是否存在指定的节点，若存在则返回节点信息.*/
int dir_check_fsitem(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	if (NULL==pdir || NULL==pitr||NULL==nodename) return INVALID;
	if (NULL==pdir->d_data.d_op||NULL==pdir->d_data.d_op->readattrib) return INVALID;
	return pdir->d_data.d_op->readattrib(pdir,pitr,nodename);
}

/*检查文件系统中是否存在指定的文件夹节点*/
int dir_check_fsdir(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	int result = dir_check_fsitem(pdir,pitr,nodename);
	if (INVALID==result) return INVALID;
	return (pitr->i_type & ITYPE_DIR) ? VALID : INVALID;
}

/*检查文件系统中是否存在指定的非文件夹节点*/
int dir_check_fsinode(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	int result = dir_check_fsitem(pdir,pitr,nodename);
	if (INVALID==result) return INVALID;
	return (pitr->i_type & ITYPE_DIR) ? INVALID : VALID;
}

/*检查系统中是否存在指定的节点*/
int dir_check_item(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	struct itemdata *pitm=NULL;
	int result = dir_check_coreitem(pdir,&pitm,nodename);
	if (INVALID!=result&&NULL!=pitm) 
	{
		memcpy(pitr,&(pitm->i_attrib),sizeof(struct itemattrib));
		return VALID;
	}return dir_check_fsitem(pdir,pitr,nodename);
}

/*检查系统中是否有文件夹节点*/
int dir_check_dir(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	struct dir *ppdir=NULL;
	if (NULL==pdir||NULL==pitr||NULL==nodename) return INVALID;
	int result = dir_check_coredir(pdir,&ppdir,nodename);
	if (INVALID==result) return dir_check_fsdir(pdir,pitr,nodename);
	if (NULL==ppdir) return INVALID;
	memcpy(pitr,&(ppdir->d_data.i_attrib),sizeof(struct itemattrib));
	return VALID;
}

/*检查非文件夹节点*/
int dir_check_inode(struct dir *pdir,_co struct itemattrib *pitr,_ci const char *nodename)
{
	struct inode *pin=NULL;
	if (NULL==pdir||NULL==pitr||NULL==nodename) return INVALID;

	int result = dir_check_coreinode(pdir,&pin,nodename);
	if (INVALID==result) return dir_check_fsinode(pdir,pitr,nodename);
	if (NULL==pin) return INVALID;

	memcpy(pitr,&(pin->i_data.i_attrib),sizeof(struct itemattrib));
	return VALID;
}

/*打开新的文件夹节点,若内核中已经存在则返回该节点*/
int dir_do_opendir(struct dir *pdir,_co struct dir **ppdir,_ci const char *nodename)
{
	if (NULL==pdir||NULL==ppdir||NULL==nodename) return INVALID;

	int result = dir_check_coredir(pdir,ppdir,nodename);
	if (INVALID!=result&&NULL!=ppdir) return VALID;

	struct itemattrib ditm={{0}};
	result = dir_check_fsdir(pdir,&ditm,nodename);
	if (INVALID==result) return INVALID;
	if (NULL==pdir->d_data.d_op||NULL==pdir->d_data.d_op->opendir) return INVALID;

	struct dir *p = dir_cache_alloc();
	if (NULL==p) return INVALID;

	spinlock_init(p->lck_d_child);
	spinlock_init(p->lck_i_child);
	list_ini(p->d_brother);
	p->d_child = NULL,p->i_child = NULL;
	p->d_data.d_op = pdir -> d_data.d_op;

	p->d_data.i_rcnt = 0;
	p->d_data.i_volnum = pdir->d_data.i_volnum;
	p->d_data.d_parent=pdir;
	p->d_data.i_root=pdir->d_data.i_root;

	result = pdir->d_data.d_op->opendir(pdir,&(p->d_data),nodename);
	if (INVALID==result) goto faile;
	*ppdir = p;
	return VALID;
faile:
	*ppdir = NULL;
	dir_cache_free(p);
	return INVALID;
}

/*打开非文件夹节点，如果节点不存在则失败*/
int dir_do_openinode(struct dir *pdir,_co struct inode **ppi,_ci const char *nodename)
{
	if (NULL==pdir||NULL==ppi||NULL==nodename) return INVALID;

	int result = dir_check_coreinode(pdir,ppi,nodename);
	if (INVALID!=result) return VALID;

	struct itemattrib ditm={{0}};
	result = dir_check_fsinode(pdir,&ditm,nodename);
	if (INVALID==result) return INVALID;
	if (NULL==pdir->d_data.d_op||NULL==pdir->d_data.d_op->openinode) return INVALID;

	struct inode *p = inode_cache_alloc();
	if (NULL==p) return INVALID;

	p->i_data.i_rcnt = 0;
	p->i_data.i_volnum = pdir->d_data.i_volnum;
	p->i_data.d_parent=pdir;
	p->i_data.i_root=pdir->d_data.i_root;
	list_ini(p->i_brother);
	p->f_lst = NULL;
	spinlock_init(p->lck_f_lst);

	result = pdir->d_data.d_op->openinode(pdir,p,nodename);
	if (INVALID==result) goto faile;
	*ppi = p;
	return VALID;
faile:
	*ppi = NULL;
	inode_cache_free(p);
	return INVALID;
}

/*打开文件节点，若文件节点不存在则创建节点*/
int dir_do_opennewinode(struct dir *pdir,_co struct inode **ppi,_ci const char *nodename)
{
	if (NULL==pdir||NULL==ppi||NULL==nodename) return INVALID;
	if (NULL==pdir->d_data.d_op||NULL==pdir->d_data.d_op->touch) return INVALID;
	if (NULL==pdir->d_data.d_op||NULL==pdir->d_data.d_op->openinode) return INVALID;

	struct itemattrib itm={{0}};
	int result = pdir->d_data.d_op->touch(pdir,&itm,nodename);
	if (INVALID==result) return INVALID;
	return dir_do_openinode(pdir,ppi,nodename);
}

/*打开成功后需要执行该函数将节点添加到内核节点树中.*/
int dir_opendir_done(struct dir *pdir,_ci struct dir *ppdir)
{
	if (NULL==pdir||NULL==ppdir) return INVALID;
	struct itemdata *pitm=NULL;
	int result = dir_check_coreitem(pdir,&pitm,ppdir->d_data.i_attrib.i_name);
	if (INVALID!=result) 
		return pitm==&(ppdir->d_data)?VALID:INVALID;

	get_spinlock(pdir->lck_d_child);
	if (NULL==pdir->d_child) 
	{
		pdir->d_child = ppdir;
		goto done;
	}
	ppdir -> d_data.i_rcnt ++;
	list_inserttail(&(pdir->d_child->d_brother),&(ppdir->d_brother));
done:
	release_spinlock(pdir->lck_d_child);
	return VALID;
}

/*将需要打开的非文件节点添加到内核节点目录*/
int dir_openinode_done(struct dir *pdir,_ci struct inode *pi)
{
	if (NULL==pdir||NULL==pi) return INVALID;
	struct itemdata *pitm=NULL;
	int result = dir_check_coreitem(pdir,&pitm,pi->i_data.i_attrib.i_name);
	if (INVALID!=result) 
		return pitm==&(pi->i_data)?VALID:INVALID;

	get_spinlock(pdir->lck_i_child);
	if (NULL==pdir->i_child) 
	{
		pdir->i_child = pi;
		goto done;
	}
	list_inserttail(&(pdir->i_child->i_brother),&(pi->i_brother));
done:
	release_spinlock(pdir->lck_i_child);
	return VALID;
}

/*将没有被引用的目录空间释放,匹配opendir中的分配操作.
 * 目录被关闭的条件是,内核中没有文件引用该节点
 */
int dir_close_dir(struct dir *pdir,_ci struct dir *ppdir)
{
	if (NULL==pdir||NULL==ppdir) return INVALID;
	if (pdir !=  ppdir->d_data.d_parent ) return INVALID;
	struct dir *p=NULL;
	int result = dir_check_coredir(pdir,&p,ppdir->d_data.i_attrib.i_name);
	if (INVALID==result) return INVALID;
	if (ppdir!=p) return INVALID;

	get_spinlock(pdir->lck_d_child);
	if (ppdir->d_brother.next==ppdir->d_brother.pre)
	{
		if (pdir->d_child == ppdir)
		{
			pdir->d_child = NULL;
			pdir->d_data.i_rcnt --;
		}
		else
		{
			printk("Core crashd!");
			goto error;
		}
	}
	else
	{
		struct list_head *plst = ppdir->d_brother.next;
		list_remove(&(ppdir->d_brother));
		p = list_load(struct dir,d_brother,plst);
		pdir->d_child = p;
		pdir->d_data.i_rcnt --;
	}
	result = VALID;
	if (NULL!=pdir->d_data.d_op&&NULL!=pdir->d_data.d_op->closedir)
		result = pdir->d_data.d_op->closedir(pdir,&(ppdir->d_data));
	dir_cache_free(ppdir);

	/*在这里直接释放空间，如果有野指针指向这里可以被系统检测到*/
	memset(ppdir,0,sizeof(struct dir));

	release_spinlock(pdir->lck_d_child);
	return result;
error:
	release_spinlock(pdir->lck_d_child);
	return INVALID;
}

/*关闭节点，若节点的引用链表不为空则失败*/
int dir_close_inode(struct dir *pdir,_ci struct inode *pi)
{
	if (NULL==pdir||NULL==pi) return INVALID;
	if (pdir !=  pi->i_data.d_parent ) return INVALID;
	struct inode *p=NULL;
	int result = dir_check_coreinode(pdir,&p,pi->i_data.i_attrib.i_name);
	if (INVALID==result) return INVALID;
	if (pi!=p) return INVALID;

	get_spinlock(pdir->lck_i_child);	
	if (NULL!=pi->f_lst) goto error;

	if (pi->i_brother.next==pi->i_brother.pre)
	{
		if (pdir->i_child == pi)
		{
			pdir->i_child = NULL;
			pdir->d_data.i_rcnt --;
		}
		else
		{
			printk("Core crashd!");
			goto error;
		}
	}
	else
	{
		struct list_head *plst = pi->i_brother.next;
		list_remove(&(pi->i_brother));
		p = list_load(struct inode,i_brother,plst);
		pdir->i_child = p;
		pdir->d_data.i_rcnt --;
	}
	result = VALID;
	if (NULL!=pdir->d_data.d_op&&NULL!=pdir->d_data.d_op->closeinode)
		result = pdir->d_data.d_op->closeinode(pdir,pi);
	inode_cache_free(pi);

	/*在这里直接释放空间，如果有野指针指向这里可以被系统检测到*/
	memset(pi,0,sizeof(struct inode));

	release_spinlock(pdir->lck_i_child);
	return result;
error:
	release_spinlock(pdir->lck_i_child);
	return INVALID;
}

/*ufilename start with '/'
 * 现在还没有添加对长文件名的支持，支持的最长文件节点名长为K_MAX_LEN_NODE_NAME
 */
int sys_do_open(struct dir *root,unsigned int mode,const char *ufilename)
{
	if (NULL==root||NULL==ufilename) return INVALID;
	char nodename[K_MAX_LEN_NODE_NAME+1]={0};
	int i=0,flg=0,fd=-1;
	int result = detach_node(i++,ufilename,nodename,&flg,K_MAX_LEN_NODE_NAME);
	struct dir *subdir=root,*p=NULL;
	struct inode *pi=NULL;
	struct file *pf=NULL;

	while (1==flg)
	{
		result = dir_do_opendir(subdir,&p,nodename);
		if (INVALID==result) goto error;
		subdir = p;
		result = detach_node(i++,ufilename,nodename,&flg,K_MAX_LEN_NODE_NAME);
	}
	/*现在subdir是nodename的上级目录*/
	result = dir_do_openinode(subdir,&pi,nodename);
	if (INVALID==result) goto error;

	fd = sys_check_taskfile(tsk_running,pi);
	if (INVALID!=fd) 
		return mode==tsk_running->t_file[fd]->f_mode ? fd : INVALID;
	result = sys_do_openfile(pi,&pf,mode);
	if (INVALID==result) goto error;
	return sys_set_taskfile(tsk_running,pf);
error:
	return INVALID;
}

/*检查任务是否打开过指定文件*/
int sys_check_taskfile(struct task_struct *ptsk,const struct inode *pi)
{
	int i,fd=INVALID;

	get_spinlock(ptsk->t_flck);
	for (i=0;i<K_MAX_FILE_OPEN_PPS;i++)
		if (NULL!=ptsk->t_file[i]&&pi==ptsk->t_file[i]->f_pi)
		{
			fd = i;
			break;
		}
	release_spinlock(ptsk->t_flck);
	return fd;
}

/*设置任务打开的文件*/
int sys_set_taskfile(struct task_struct *ptsk,struct file *pf)
{
	int i,fd=INVALID;

	get_spinlock(ptsk->t_flck);
	if (ptsk->t_fcnt>=K_MAX_FILE_OPEN_PPS) goto end;
	for (i=3;i<K_MAX_FILE_OPEN_PPS;i++)
		if (NULL==ptsk->t_file[i])
		{
			ptsk->t_file[i] = pf;
			ptsk->t_fcnt ++;
			fd = i;
			break;
		}
end:
	release_spinlock(ptsk->t_flck);
	return fd;

}

/*打开指定的节点*/
int sys_do_openfile(struct inode *pi,struct file **ppf,unsigned int mode)
{
	if (NULL==pi||NULL==ppf) return INVALID;
	struct file *pf = file_cache_alloc();
	if (NULL==pf) return INVALID;
	pf->f_pi = pi;
	pf->f_owner.ptsk = tsk_running;
	pf->f_owner.pthsk = thr_running;
	pf->f_private = NULL;
	pf->f_mode = mode;
	list_ini(pf->f_lst);
	if (NULL==pi->i_data.f_op) goto done;
	if (NULL==pi->i_data.f_op->open) goto done;
	int result = pi->i_data.f_op->open(pf,pi);
	if (INVALID!=result) goto done;
	file_cache_free(pf);
	*ppf = NULL;

	return INVALID;
done:
	*ppf = pf;
	get_spinlock(pi->lck_f_lst);
	if (NULL==pi->f_lst)
	{
		pi->f_lst = pf;
		goto out;
	}
	list_inserttail(&(pi->f_lst->f_lst),&(pf->f_lst));
out:
	release_spinlock(pi->lck_f_lst);
	return VALID;
}

int register_device(struct device_base *dev)
{
	int result = VALID; unsigned int major,slave;

	if (NULL==dev) return INVALID;

	major = GETMAJOR(dev->dev_num);
	slave = GETSLAVE(dev->dev_num);

	get_spinlock(lck_device);
	if (MAJOR_MAX < major || SLAVE_MAX < slave||
		NULL!=device_head[major].slave_dev[slave])  
	{
		result = INVALID;
		goto done;
	}
	else
	{
		device_head[major].dev_cnt ++;
		device_head[major].slave_dev[slave] = dev;
		result = VALID;
	}
done:
	release_spinlock(lck_device);
	return result;
}

struct device_base * unregister_device(dev_t devnum)
{
	struct device_base *pdev = NULL;
	unsigned int major,slave;

	get_spinlock(lck_device);

	major = GETMAJOR(devnum);
	if (MAJOR_MAX<major) goto done;
	slave = GETSLAVE(devnum);
	if (SLAVE_MAX<slave) goto done;

	pdev = device_head[major].slave_dev[slave];
	if (NULL==pdev) goto done;
	device_head[major].slave_dev[slave] = NULL;
	device_head[major].dev_cnt --;
done:
	release_spinlock(lck_device);
	return pdev;
}

int register_driver(struct driver *drv)
{
	if (NULL==drv) return INVALID;
	unsigned int major = drv->dev_major_num;
	if (MAJOR_MAX<major) return INVALID;
	if (NULL!=driver_head[major]) return INVALID;
	driver_head[major] = drv;
	return VALID;
}

struct driver *unregister_driver(unsigned int major)
{
	struct driver *pdrv=NULL;
	if (MAJOR_MAX<major) return NULL;
	pdrv = driver_head[major];
	driver_head[major] = NULL;
	return pdrv;
}

struct device_base * find_device(dev_t devnum)
{
	unsigned int major,slave;
	major = GETMAJOR(devnum);
	slave = GETSLAVE(devnum);
	
	if (MAJOR_MAX<major || SLAVE_MAX < slave ) return NULL;
	return device_head[major].slave_dev[slave];
}

struct driver * search_driver(unsigned int major)
{
	if (MAJOR_MAX<major ) return NULL;
	return driver_head[major];
}

struct driver * find_driver(dev_t devnum)
{
	return search_driver(GETMAJOR(devnum));
}

/*将设备节点添加到内核中*/
struct inode *load_driver(dev_t devnum,const char *devnodename)
{
	return NULL;
}

/*将设备节点从内核中卸载*/
int unload_driver(dev_t devnum)
{
	return INVALID;
}

int model_startup(void)
{
	return INVALID;
}

/*系统驱动模块初始化*/
int module_init(void)
{
	DO_MODULE_INIT_START
	DO_MODULE_INIT_END
	return VALID;
}


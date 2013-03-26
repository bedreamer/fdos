/**
  pci.h
  Copyright (C) 2012, 2013 Lijie.
  < cuplision@163.com >
 */
#ifndef _PCI_H_
#define _PCI_H_
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <kernel/list.h>

#define PCI_DEV_VEN_ID  0x00
#define PCI_STAT_CMD    0x04
#define PCI_CLASS_REV   0x08

/**
  PCI Agent config registers.

 +31------------24+23------------16+15--------------8+7------------------0+  offset
 +           Device ID             +             Vendor ID                +  0x00
 +----------------+----------------+-----------------+--------------------+
 +             Status              +             Command                  +  0x04
 +----------------+----------------+-----------------+--------------------+
 +                    Class Code                     +    Revision ID     +  0x08
 +----------------+----------------+-----------------+--------------------+
 +      BIST      +  Header Type   +  Latency Timer  +  Cache Line Size   +  0x0C
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 0                          +  0x10
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 1                          +  0x14
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 2                          +  0x18
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 3                          +  0x1C
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 4                          +  0x20
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 5                          +  0x24
 +----------------+----------------+-----------------+--------------------+
 +                         Cardbus CIS Pointer                            +  0x28
 +----------------+----------------+-----------------+--------------------+
 +           Subsystem ID          +          Subsystem Vendor ID         +  0x2C
 +----------------+----------------+-----------------+--------------------+
 +                         Expansion ROMBase Address                      +  0x30
 +----------------+----------------+-----------------+--------------------+
 +                     Reserved                      +Capabilities Pointer+  0x34
 +----------------+----------------+-----------------+--------------------+
 +                               Reserved                                 +  0x38
 +----------------+----------------+-----------------+--------------------+
 +    MAX_Lat     +     Min_Gnt    +  Interrupt Pin  +   Interrupt Line   +  0x3C
 +----------------+----------------+-----------------+--------------------+



  PCI Bridge config registers.

 +31------------24+23------------16+15--------------8+7------------------0+  offset
 +           Device ID             +             Vendor ID                +  0x00
 +----------------+----------------+-----------------+--------------------+
 +             Status              +             Command                  +  0x04
 +----------------+----------------+-----------------+--------------------+
 +                    Class Code                     +    Revision ID     +  0x08
 +----------------+----------------+-----------------+--------------------+
 +      BIST      +  Header Type   +  Latency Timer  +  Cache Line Size   +  0x0C
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 0                          +  0x10
 +----------------+----------------+-----------------+--------------------+
 +                       Base Address Rigister 1                          +  0x14
 +----------------+----------------+-----------------+--------------------+
 +    Secondary   +   Subordinate  +     Secondary   +       Primary      +
 +  Laetncy Timer +    Bus Number  +    Bus Number   +     Bus Number     +  0x18
 +----------------+----------------+-----------------+--------------------+
 +        Secondary Status         +    I/O Limit    +      I/O Base      +  0x1C
 +----------------+----------------+-----------------+--------------------+
 +          Memory Limit           +            Memory Base               +  0x20
 +----------------+----------------+-----------------+--------------------+
 +    Prefetchable Memory Limit    +       Prefetchable Memory Base       +  0x24
 +----------------+----------------+-----------------+--------------------+
 +                  Prefetchable Base Upper 32 Bits                       +  0x28
 +----------------+----------------+-----------------+--------------------+
 +                  Prefetchable Limit Upper 32 Bits                      +  0x2C
 +----------------+----------------+-----------------+--------------------+
 +     I/O Limit Upper 16 Bits     +       I/O Base Upper 16 Bits         +  0x30
 +----------------+----------------+-----------------+--------------------+
 +                     Reserved                      +Capabilities Pointer+  0x34
 +----------------+----------------+-----------------+--------------------+
 +                     Expansion ROMBase Address                          +  0x38
 +----------------+----------------+-----------------+--------------------+
 +          Bridge Control         +  Interrupt Pin  +   Interrupt Line   +  0x3C
 +----------------+----------------+-----------------+--------------------+
 */
struct pci_bus;

struct pci_config_struct
{
    unsigned short id_vendor;
    unsigned short id_device;
    unsigned short cmd;
    unsigned short status;
    unsigned int rev_id:8;
    unsigned int class_id:24;
}__align(char);

struct pci_dev
{
    unsigned char n_bus, n_dev, n_func;
    struct pci_config_struct config;

    struct pci_bus *p_bus;
    struct list_head devlst;
};

struct pci_bus
{
    unsigned char n_bus;

    struct pci_dev *devs;

    struct pci_bus *p_bus;
    struct list_head buslst;
};

struct pci_class_struct
{
	unsigned char	baseclass ;
	unsigned char	subclass ;
	unsigned char	progif ;
	const char *	basedesc ;
	const char *	subdesc ;
	const char *	progdesc ;
};

extern struct pci_class_struct pcs[];
extern char *classname[];

#define PCI_CFG_ADDR 0xCF8
#define PCI_CFG_DATA 0xCFC
#define PCI_INVALID_ID 0xFFFFFFFF
void pci_bus_init(void);
void pci_bus_scan(unsigned char);
void pci_dev_scan(unsigned char, unsigned char);
void pci_bridge_scan(unsigned char, unsigned char, unsigned char);
int pci_func_scan(unsigned char, unsigned char, unsigned char);
void pci_bus_probe(unsigned char);
void pci_dev_probe(unsigned char, unsigned char);
void pci_dev_ioremap(struct pci_dev *, unsigned int, unsigned int);
unsigned int pci_alloc_memory(unsigned int);

static inline void __pci_cfg_read_dw(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned char offset, unsigned int *dw)
{
    unsigned int ioa0;

    ioa0 = ( 1 << 31 ) | ( n_bus << 16 ) | ( n_dev << 11 ) | ( n_func << 8 ) | offset ;
    outdw( ioa0, PCI_CFG_ADDR );
    * dw = indw( PCI_CFG_DATA );
}

static inline void __pci_cfg_write_dw(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned char offset, unsigned int dw)
{
    unsigned int ioa0;

    ioa0 = ( 1 << 31 ) | ( n_bus << 16 ) | ( n_dev << 11 ) | ( n_func << 8 ) | offset ;
    outdw( ioa0, PCI_CFG_ADDR );
    outdw( dw,  PCI_CFG_DATA );
}

#define __pci_exsit __pci_get_id
static inline unsigned int __pci_get_id(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw;

    __pci_cfg_read_dw(n_bus, n_dev, n_func, 0x00, &dw);

    return dw;
}

static inline unsigned int __pci_get_classid_rev(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw;

    __pci_cfg_read_dw(n_bus, n_dev, n_func, 0x08, &dw);

    return dw;
}
#define __pci_get_classid(n_bus, n_dev, n_func) ((__pci_get_classid_rev(n_bus, n_dev, n_func)) >> 8)
#define __pci_get_revid(n_bus, n_dev, n_func)   ((__pci_get_classid_rev(n_bus, n_dev, n_func)) & 0xFF)

static inline unsigned int __pci_is_bridge(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw;

    dw = __pci_get_classid(n_bus, n_dev, n_func);

#ifndef _PCI_DEBUG_
    printk("[%02X:%02X.%X]--> %s", n_bus, n_dev, n_func, classname[0x1F&(dw>>16)]);
#endif

    return (0x06==(dw>>8)) ? 0 : 1;
}

static inline void pci_cfg_read_dw(struct pci_dev *d, unsigned char offset, unsigned int *dw)
{
    __pci_cfg_read_dw(d->n_bus, d->n_dev, d->n_func, offset, dw);
}

static inline void pci_cfg_write_dw(struct pci_dev *d, unsigned char offset, unsigned int dw)
{
    __pci_cfg_write_dw(d->n_bus, d->n_dev, d->n_func, offset, dw);
}
#endif /*_PCI_H_*/

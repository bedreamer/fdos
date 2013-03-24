/**
  pci.h
  Copyright (C) 2012, 2013 Lijie.
  < cuplision@163.com >
 */
#ifndef _PCI_H_
#define _PCI_H_
#include <kernel/list.h>

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

struct pci_dev
{
    unsigned char n_bus, n_dev, n_func;

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

#define PCI_CFG_ADDR 0xCF8
#define PCI_CFG_DATA 0xCFC
void pci_bus_init(void);
void pci_bus_scan(unsigned char);
void pci_dev_scan(unsigned char, unsigned char);
void pci_func_scan(unsigned char, unsigned char, unsigned char);
void pci_bus_probe(unsigned char);
void pci_dev_probe(unsigned char, unsigned char);
void pci_dev_ioremap(struct pci_dev *, unsigned int, unsigned int);
void pci_cfg_read_dw(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int *);
void pci_cfg_write_dw(unsigned char, unsigned char, unsigned char, unsigned char, unsigned int);
unsigned int pci_alloc_memory(unsigned int);

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

#endif /*_PCI_H_*/

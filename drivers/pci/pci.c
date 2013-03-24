/**
  pci.c
  Copyright (C) 2012, 2013 Lijie.
  < cuplision@163.com >
 */
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <drivers/pci.h>

static unsigned int pci_addr=0xFFFFFFFF;
struct pci_bus* root_bus;

void pci_bus_init(void)
{
    unsigned int n=0;

    for (; n <= 0xFF ; n++ )
        pci_bus_scan(n);
    memset((char *)0xFC000000, 64*1024*1024, '*');
}

void pci_bus_scan(unsigned char n_bus)
{
    unsigned int n=0;

    for (; n <= 0x1F ; n++ )
        pci_dev_scan(n_bus, n);
}

void pci_dev_scan(unsigned char n_bus, unsigned char n_dev)
{
    unsigned int n=0;

    for (; n < 0x08 ; n++ )
        pci_func_scan(n_bus, n_dev, n);
}

int pci_func_scan(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw1, bar[4], len[4], i, off[4]={0x10, 0x14, 0x18, 0x1c};
    unsigned int mask[4]={0x0000FFFC, 0x0000FFFC, 0xFFFFFFF0, 0xFFFFFFF0};
    pci_cfg_read_dw(n_bus, n_dev, n_func, 0x00, &dw1);
    if ( dw1 == 0xFFFFFFFF ) return -1;

    printk("{[%02X:%02X.%X-<%04X:%04X>]}", n_bus, n_dev, n_func, dw1 & 0xFFFF, dw1 >> 16);
    for (i=0; i<4; i++) {
        pci_cfg_read_dw (n_bus, n_dev, n_func, off[i], &bar[i]);
        bar[i] &= mask[i];
    }
    for (i=0; i<4; i++) {
        pci_cfg_write_dw(n_bus, n_dev, n_func, off[i], 0xFFFFFFFF);
        pci_cfg_read_dw (n_bus, n_dev, n_func, off[i], &len[i]);
    }

    for (i=0; i<4; i++) {
        if (0x00000000==bar[i]) continue;
        if (! (len[i] & 0x00000001) ) {
            len[i] &= 0xFFFFFFF0;
            len[i]  = ~ len[i] + 1;
            printk("    MEMORY BAR%d:0x%08X, LEN: 0x%08X Byte", i, bar[i], len[i]);
         } else {
            len[i] &= 0xFFFFFFFC;
            len[i]  = ~ len[i] + 1;
            len[i] &= 0x0000FFFF;
            printk("    I/O BAR%d:   0x%08X, LEN: 0x%08X", i, bar[i], len[i]);
       }
    }

    return 0;
}

void pci_bus_probe(unsigned char n_bus)
{
}

void pci_dev_probe(unsigned char n_bus, unsigned char n_dev)
{
}

void pci_dev_ioremap(struct pci_dev *dev, unsigned int sar, unsigned int size)
{
}

void pci_cfg_read_dw(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned char offset, unsigned int *dw)
{
    unsigned int ioa0;

    ioa0 = ( 1 << 31 ) | ( n_bus << 16 ) | ( n_dev << 12 ) | ( n_func << 8 ) | offset ;
    outdw( ioa0, PCI_CFG_ADDR );
    * dw = indw( PCI_CFG_DATA );
}

void pci_cfg_write_dw(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned char offset, unsigned int dw)
{
    unsigned int ioa0;

    ioa0 = ( 1 << 31 ) | ( n_bus << 16 ) | ( n_dev << 12 ) | ( n_func << 8 ) | offset;
    outdw( ioa0, PCI_CFG_ADDR );
    outdw( dw,  PCI_CFG_DATA );
}

unsigned int pci_alloc_memory(unsigned int size)
{
    size = pci_addr - size + 1;
    pci_addr = size - 1;
    return size;
}


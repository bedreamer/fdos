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

#ifdef _PCI_DEBUG_
void pcidev_print_someinfo(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned int id)
{
    unsigned int dw1=id, bar[4], len[4], i, off[4]={0x10, 0x14, 0x18, 0x1c};
    unsigned int mask[4]={0x0000FFFC, 0x0000FFFC, 0xFFFFFFF0, 0xFFFFFFF0};

#if 0
    dw1 = __pci_exsit(n_bus, n_dev, n_func);
    if ( PCI_INVALID_ID == dw1) return;
#endif

    printk("{[%02X:%02X.%X-<%04X:%04X>]}", n_bus, n_dev, n_func, dw1 & 0xFFFF, dw1 >> 16);
    for (i=0; i<4; i++) {
        __pci_cfg_read_dw (n_bus, n_dev, n_func, off[i], &bar[i]);
        bar[i] &= mask[i];
    }
    for (i=0; i<4; i++) {
        __pci_cfg_write_dw(n_bus, n_dev, n_func, off[i], 0xFFFFFFFF);
        __pci_cfg_read_dw (n_bus, n_dev, n_func, off[i], &len[i]);
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
}
#else
#define pcidev_print_someinfo(bus, dev, func, id)
#endif

void pci_bus_init(void)
{
    unsigned int n=0;

    for (; n <= 0xFF ; n++ )
        pci_bus_scan(n);
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

void pci_bridge_scan(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw;
#ifndef _PCI_DEBUG_
    printk("Bridge Device.");
#endif
    __pci_cfg_write_dw(n_bus, n_dev, n_func, 0x18, 0xFFFF0000|n_bus );
}

int pci_func_scan(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int id, dw;

    dw = __pci_get_id(n_bus, n_dev, n_func);
    if ( PCI_INVALID_ID == dw ) return 0;

#ifdef _PCI_DEBUG_
    id = __pci_get_classid(n_bus, n_dev, n_func);
    printk("Device Class. %08X", id);
#endif

    if ( __pci_is_bridge(n_bus, n_dev, n_func) ) {
#if 1
        pci_bridge_scan(n_bus, n_dev, n_func);
#endif
        //return dw;
    }

    pcidev_print_someinfo(n_bus, n_dev, n_func, dw);
    return dw;
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

unsigned int pci_alloc_memory(unsigned int size)
{
    size = pci_addr - size + 1;
    pci_addr = size - 1;
    return size;
}


/**
  pci.c
  Copyright (C) 2012, 2013 Lijie.
  < cuplision@163.com >
 */
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <drivers/pci.h>

unsigned int pci_addr=0xFFFFFFFF;

struct pci_bus* root_bus;

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

    for (; n <= 0x08 ; n++ )
        pci_func_scan(n_bus, n_dev, n);
}

void pci_func_scan(unsigned char n_bus, unsigned char n_dev, unsigned char n_func)
{
    unsigned int dw;
    pci_cfg_read_dw(n_bus, n_dev, n_func, 0x00, &dw);
    if ( dw != 0xFFFFFFFF )
        printk("{[%02X:%02X.%X-<%04X:%04X>]}", n_bus, n_dev, n_func, dw & 0xFFFF, dw >> 16);
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

    ioa0 = ( 1 << 31 ) | ( n_bus << 16 ) | ( n_dev << 12 ) | ( n_func << 8 );
    outdw( ioa0, PCI_CFG_ADDR );
    * dw = indw( PCI_CFG_DATA );
}

void pci_cfg_write_dw(unsigned char n_bus, unsigned char n_dev, unsigned char n_func, unsigned char offset, unsigned int *dw)
{
}


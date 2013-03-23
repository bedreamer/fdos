/*
 *	kmain.c
 *	bedreamer@163.com
 *	Thu 02 Aug 2012 02:56:09 PM CST 
 *	provide a fast malloc and free methord.
 */
#include <kernel/kernel.h>
#include <kernel/printk.h>
#include <drivers/pci.h>

char *root_dev=NULL;
char *root_tty=NULL;
int   root_debug=0;

void paras_cmdline(char *cmdline);

/*#error version is : _VERSION_
*/void start_kernel(char *cmdline)
{
    kprintf("%s\n", VERSION);

    pci_bus_init();
    printk("kernel DONE");

    /* can't be here. */
    while (1)
    {
    }
}

void paras_cmdline(char *cmdline)
{
}



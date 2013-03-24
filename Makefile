# Makefile for fdos.
# < cuplision@163.com>

# config file, auto been generated
include .config
# define exported values.
include .exports

KERNELFILE=KFOOL
# RELEASE=0 release
# RELEASE=1 debug
# RELEASE=2 demon
RELEASE=2
VMDIR=/home/cup/VBoxVMs/fdos/fdos.vbox
CC=gcc
AS=nasm
LD=ld
AR=ar
MAKE=make
MAKEPARAM=--no-print-directory
#MAKEPARAM=
OMIT=-
PWD=pwd
WORKDIR=$(shell pwd)
IMGFILE=$(WORKDIR)/floppy.img
Q=@
#Q=
CFLAGS=-D_KERNEL_ -D_VERSION_=$(RELEASE) -I$(WORKDIR)/$(ARCH)\
        -I$(WORKDIR)/include -Wall -ffreestanding \
       -fno-builtin -fno-builtin-function -fno-stack-protector\
         -z nodefaultlib -I$(WORKDIR)/include/drivers -c 
CCFLAGS+=-D_ARCH_$(ARCH)_
CBUILDIN=-D_BUILDIN_ $(CFLAGS)
# used for some directory.
CPFLAGS=
ASFLAGS+=-I $(WORKDIR)/include/ -I $(WORKDIR)/$(ARCH) -f elf
BINASFLAGS=
# build-in objects.
y-objs=
yobjs-list:=$(WORKDIR)/.yobjs
# module objects.
m-objs=
mobjs-list:=$(WORKDIR)/.mobjs
# separate objects.
s-objs=
sobjs-list:=$(WORKDIR)/.sobjs
# binary objects.
b-objs=
PHONY+=y-objs m-objs s-objs b-objs
EXPORTS+=KERNELFILE VERSION CC AS LD AR MAKE MAKEPARAM OMIT \
        PWD ARCH WORKDIR IMGFILE Q CFLAGS CCFLAGS CMODULE \
        CBUILDIN ASFLAGS BINASFLAGS CPFLAGS yobjs-list mobjs-list\
         sobjs-list
SEP-DIRS=kernel lib $(ARCH)
# must be the last one.
LAST-DIR=boot
SUB-DIRS=drivers fs
BOOT_LOADER:=boot/old/boot.bin boot/old/loader.bin

all: _all

PHONY+=all _all

# FIXME: maybe there is a better way to refresh objects-list files.
_all:
	$(Q)echo ''> $(yobjs-list);echo ''> $(mobjs-list);echo ''> $(sobjs-list);
	$(Q)for d in $(SEP-DIRS) $(SUB-DIRS) $(LAST-DIR); do\
	   $(MAKE) $(MAKEPARAM) -C $$d all;\
     done
PHONY+=all
clean:
	$(Q)$(OMIT)rm $(WORKDIR)/$(KERNELFILE) $(yobjs-list) $(mobjs-list) $(sobjs-list) $(IMGFILE) $(BOOT_LOADER) 2>/dev/null;
	$(Q)$(OMIT)for d in $(SEP-DIRS) $(SUB-DIRS) $(LAST-DIR); do\
        $(MAKE) $(MAKEPARAM) -C $$d clean 2>/dev/null;done;
PHONY+=clean
image:
	$(Q)$(MAKE) $(MAKEPARAM) -C boot boot-img
PHONY+=image
debug:
	$(Q)$(OMIT)virtualbox --startvm $(VMDIR) --dbg &
	$(Q)$(OMIT)echo "Debuger starting....."

PHONY+=debug
DOEXPORTS=$(EXPORTS)

export $(EXPORTS) DOEXPORTS
.PHONY: $(PHONY)








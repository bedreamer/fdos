# FIXME. I want make the Makefile clear.
$(FATFS-CONFIG)-objs:=fat.ko
$(FATFS-CONFIG)-deps:=link.o fat.o open.o read.o write.o dir.o ioctl.o close.o iname.o mount.o umount.o
#
# m-objs:=fat.ko   --->  will be generated
# m-deps:=link.o fat.o open.o read.o write.o dir.o ioctl.o close.o iname.o mount.o umount.o
#
# y-objs:=fat.ko   --->  will not be generated
# y-deps:=link.o fat.o open.o read.o write.o dir.o ioctl.o close.o iname.o mount.o umount.o
#
# s-objs:=fat.ko   --->  will not be generated
# s-deps:=link.o fat.o open.o read.o write.o dir.o ioctl.o close.o iname.o mount.o umount.o
#
include $(WORKDIR)/Rule.make

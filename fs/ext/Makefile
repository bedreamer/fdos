# FIXME. I want make the Makefile clear.
SRCS=$(wildcard *.c)
OBJS=$(patsubst %.c,%.o,$(SRCS))

$(EXTFS-CONFIG)-objs:=ext.ko
$(EXTFS-CONFIG)-deps:=$(OBJS)
include $(WORKDIR)/Rule.make

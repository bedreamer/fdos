# Makefile rule of fdos.
# if you need a sub-dir Makefile, you can define
# `sub-didrs:=$(somedirs)` in your Makefile before include this rule file.
# define `m-objs:=$(module)` to build a kernel module, depends on m-deps
# so you need define the other value named`m-deps:=$(some-objects` also.
include $(WORKDIR)/.config

# this is for sub-dirs Make rule.
ifdef sub-dirs
suball:=sub_all
subclean:=sub_clean

sub_all:
	$(Q)$(OMIT)for d in $(sub-dirs); do $(MAKE) $(MAKEPARAM) -C $$d all;done;
PHONY+=sub_all

sub_clean:
	$(Q)$(OMIT)for d in $(sub-dirs); do $(MAKE) $(MAKEPARAM) -C $$d clean;done;
PHONY+=sub_clean

endif

ifdef m-objs
CPFLAGS+=-D_MODULE_
endif

# for module complire.
$(m-objs) : $(m-deps)

all : $(s-deps) $(y-deps) $(m-objs) $(suball)
	$(Q)$(OMIT)for o in $(y-deps); do PWD=`pwd`; echo $$PWD/$$o >> $(yobjs-list); done;
	$(Q)$(OMIT)for o in $(s-deps); do PWD=`pwd`; echo $$PWD/$$o >> $(sobjs-list); done
PHONY+=all

clean: $(subclean)
	$(Q)$(OMIT)echo "    RM        `pwd`/{m-objs m-deps y-deps s-deps}"
	$(Q)$(OMIT)rm build-in.a $(m-objs) $(m-deps) $(y-deps) $(s-deps);
PHONY+=clean

%.o : %.c Makefile
	$(Q)echo "    CC        `pwd`/$<"
	$(Q)$(CC) $(CFLAGS) $(CCFLAGS) $(CPFLAGS) -o $@ $<

%.o : %.s Makefile
	$(Q)echo "    NASM        `pwd`/$<"
	$(Q)$(AS) $(ASFLAGS) -o $@ $<

%.bin : %s Makefile
	$(Q)echo "    NASM [BIN]  `pwd`/$<"
	$(Q)$(AS) -o $@ $<

# kernel module file is a share libary file.
%.ko : $(m-deps) Makefile
	$(Q)echo "    LD [M]   $@"
	$(Q)$(LD) --shared $(m-deps) -o $@
	$(Q)echo "`pwd`/$@" >> $(mobjs-list); done

# used for multi level Makefile.
.PHONY:$(PHONY)

# you can export some private value in sub-dir also.
# usage: EXPORT+=SOME-VALUE
export $(EXPORT)

# for sub-dir enviroment.
export $(DOEXPORTS)

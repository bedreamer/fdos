# Makefile for boot image.
use-boot:=new
boot-files:=./$(use-boot)/head.bin
loader-file:=./$(use-boot)/setup.bin
nothing:=this_make
sub-dirs:=$(use-boot)

this_make:$(WORKDIR)/setup $(WORKDIR)/vmfdos
	$(Q)cat $(boot-files) $(loader-file) > $(WORKDIR)/setup.bin
	$(Q)$(WORKDIR)/build $(WORKDIR)/setup.bin $(WORKDIR)/vmfdos > $(WORKDIR)/bzImage
	$(Q)echo "    MAKE      bzImage";

$(WORKDIR)/setup:./$(use-boot)/head.bin ./$(use-boot)/setup.bin
	$(Q)echo "    MAKE      setup";
	cat $^ > $@

$(WORKDIR)/vmfdos:$(WORKDIR)/$(KERNELFILE)
	$(Q)echo "    MAKE      vmfdos";
	objcopy -O binary $^ $@

# FIXME. You may need root permision.
boot-img: $(WORKDIR)/$(KERNELFILE)
	$(Q)echo "    MAKE      image.";
	$(Q)dd if=$(boot-files) of=$(IMGFILE) bs=512 count=1 2>/dev/null
	$(Q)dd if=/dev/zero of=$(IMGFILE) seek=1 bs=512 count=2879 oflag=append 2>/dev/null
	$(Q)sudo rm .tmp -rf
	$(Q)mkdir .tmp
	$(Q)true
	$(Q)sudo mount -o loop $(IMGFILE) .tmp
	$(Q)sudo cp $(loader-file) .tmp/FOOLLDR
	$(Q)sudo cp $(WORKDIR)/$(KERNELFILE) .tmp/
	$(Q)sync
	$(Q)sudo umount .tmp
	$(Q)true
	$(Q)echo "+-----------------------------------------------------------------------------"
	$(Q)echo "+   Build $(IMGFILE) done." 
	$(Q)echo "+   `date -R`"
	$(Q)echo "+-----------------------------------------------------------------------------"

boot-loader: 
	$(Q)$(MAKE) -C old all 2>/dev/null;

cleanme:
	$(Q)$(OMIT) if [ -e $(WORKDIR)/$(KERNELFILE) ];then $(OMIT)$(RM) $(WORKDIR)/$(KERNELFILE) 2>/dev/null;fi;
	$(Q)$(OMIT) true

$(WORKDIR)/$(KERNELFILE):$(yobjs-list) $(sobjs-list)
	$(Q)echo "    LD        $(KERNELFILE)";
	$(Q)$(LD) -o $(WORKDIR)/$(KERNELFILE) -Ttext=0x00100000 -Map=../fdos.map `cat $(yobjs-list) | uniq` `cat $(sobjs-list) | uniq`
	$(Q)echo "> Build $(KERNELFILE) done. `date -R`"

.PHONY : boot-loader boot-img cleanme
include $(WORKDIR)/Rule.make

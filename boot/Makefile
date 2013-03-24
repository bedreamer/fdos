# Makefile for boot image.
use-boot:=old
boot-files:=./$(use-boot)/boot.bin
loader-file:=./$(use-boot)/loader.bin
nothing:=boot-img
sub-dirs:=old

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
	$(Q)sudo umount .tmp
	$(Q)true
	$(Q)echo "+-----------------------------------------------------------------------------"
	$(Q)echo "+   Build $(IMGFILE) done." 
	$(Q)echo "+   `date -R`"
	$(Q)echo "+-----------------------------------------------------------------------------"

boot-loader: 
	$(Q)$(MAKE) -C old all 2>/dev/null

cleanme:
	$(Q)$(OMIT)$(RM) $(WORKDIR)/$(KERNELFILE)

$(WORKDIR)/$(KERNELFILE):cleanme
	$(Q)echo "    LD        $(KERNELFILE)";
	$(Q)$(LD) -o $(WORKDIR)/$(KERNELFILE) `cat $(yobjs-list)` `cat $(sobjs-list)`
	$(Q)echo "> Build $(KERNELFILE) done. `date -R`"

.PHONY : boot-loader boot-img cleanme
include $(WORKDIR)/Rule.make

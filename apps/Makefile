CC =gcc
LD =ld
CCFLAGS =-c -I../include -Wall -ffreestanding -fno-builtin \
	-fno-builtin-function -fno-stack-protector -z nodefaultlib

Init: Init.o
	$(LD) --Map=Init.map ../lib/.out/stdlib.o ../lib/.out/string.o ../lib/.out/file.o ../lib/.out/io.o ../lib/.out/ps.o ../lib/.out/entry.o  ../lib/.out/cstart.o $^ -o $@
 
Init.o: Init.c
	$(CC) $(CCFLAGS) -o $@ $^

clean:
	rm Init.o Init

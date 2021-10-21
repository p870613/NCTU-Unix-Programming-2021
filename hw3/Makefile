CC = gcc
ASM64 = yasm -f elf64 -DYASM -D__x86_64__ -DPIC
CFLAG = -g -Wall -fno-stack-protector -nostdlib

BUILD_SO = libmini.so

all: libmini64.o libmini.o start.o
	ld -share -o $(BUILD_SO) libmini64.o libmini.o
%.o: %.asm
	$(ASM64) $< -o $@
%.o: %.c
	$(CC) -c -fPIC $(CFLAG) $< -o $@

test:
	make -C testcase

clean:
	rm -rf *.o
	rm -rf $(BUILD_SO)




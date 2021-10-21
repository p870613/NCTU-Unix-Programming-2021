CC = gcc
CFLAG = -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI 
LD = ld
LDFLAG = -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2

START = ../start.o
TESTCASE = alarm1 alarm2 alarm3 jmp1 
#//jmp2 sig1
all: $(TESTCASE)


%.o: %.c
	$(CC) $(CFLAG) $< -o $@
	$(LD) $(LDFLAG) -o $* $@ ../start.o -L. -L.. -lmini

clean:
	rm -rf *.o
	rm -rf $(TESTCASE)


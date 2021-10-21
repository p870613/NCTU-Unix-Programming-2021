CXX = gcc
CFLAGS = -g 
SHARE_FLAG = -shared -fPIC
FILE_NAME = main.c
EXE_NAME = logger
SO_NAME = logger.so

all: so
	$(CXX) -o $(EXE_NAME) -g $(FILE_NAME)
	

so: print logger.c
	$(CXX) -o $(SO_NAME) $(SHARE_FLAG) logger.c print_log.c -ldl 

print:
	$(CXX) $(CFLAG) -c -o print.o print_log.c

run: 
	LD_PRELOAD=logger.so ./logger
clean:
	rm -rf $(EXE_NAME)
	rm -rf $(SO_NAME)
	rm -rf print.o

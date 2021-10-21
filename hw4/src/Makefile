CC = g++
BUILD = ./hw4
CFLAG = -g -Wall 
SRC = $(wildcard ./*.cpp)
OBJ = $(SRC:.cpp=.o )
LIBS = -lelf -lcapstone

all: $(OBJ)
	$(CC) $(CFLAG) -o $(BUILD) $(OBJ) $(LIBS)

%.o: %.cpp
	$(CC) $(CFLAG) -c $^ -o $@ $(LIBS)

clean:
	rm -rf *.o
	rm -rf hw4

CC = g++
CFLAGS = -g -Wall
LIB_PATH = -L../Matrix/lib -lmatrix
INC_PATH = -I../Matrix/include
BIN = runall clean

all: $(BIN)

runall: ALL.o
	$(CC) $(INC_PATH) -o $@ $^ $(LIB_PATH) 
	
ALL.o: ALL.cpp	
	$(CC) -c $(INC_PATH) $< -o $@ 

clean:
		rm -f *.o
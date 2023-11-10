CC=gcc
#CFLAGS=-Wall -O1 -msse3
CFLAGS= -O3 -march=native
OBJ=poly.o benchmark.o timing/clock.o statistics/cpe.o statistics/fcyc.o statistics/lsquare.o
LDFLAGS=-lm
# phony targets will always be remade, so a file named "clean"
# won't prevent the clean target from running
.PHONY: all clean run
EXE=driver

all: $(EXE)

$(EXE): $(OBJ)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJ) $(LDFLAGS)

run: $(EXE)
	./$(EXE)

clean:
	rm -f $(EXE) $(OBJ)


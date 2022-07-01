CC := g++
CFLAGS := -g -Wall -O2

.PHONY: all clean

all: benchmark

benchmark: lib/main.o
	$(CC) -o $@ lib/main.o

lib/main.o: lib/main.cpp
	$(CC) $(CFLAGS) -c lib/main.cpp -o lib/main.o

clean:
	rm $(LIB_SRC)/*.o

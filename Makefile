CC := g++
CFLAGS := -g -Wall -O2

.PHONY: all clean

all: benchmark

benchmark: lib/main.o lib/benchmark.o lib/util.o lib/bubblesort.o
	$(CC) -o $@ lib/main.o lib/benchmark.o lib/util.o lib/bubblesort.o

lib/main.o: lib/main.cpp
	$(CC) $(CFLAGS) -c lib/main.cpp -o lib/main.o

lib/benchmark.o: lib/benchmark.cpp
	$(CC) $(CFLAGS) -c lib/benchmark.cpp -o lib/benchmark.o

lib/util.o: lib/util.cpp
	$(CC) $(CFLAGS) -c lib/util.cpp -o lib/util.o

lib/bubblesort.o: lib/methods/bubblesort.cpp
	$(CC) $(CFLAGS) -c lib/methods/bubblesort.cpp -o lib/bubblesort.o

clean:
	rm lib/*.o

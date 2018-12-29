CC=g++
CFLAGS=-I.
LIBS=-lstdc++

DEPS = kmvconverter.h
OBJ = main.cpp kmvconverter.cpp

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

kmvconverter: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

.PHONY: clean

clean:
	rm -f *.o kmvconverter


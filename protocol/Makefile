CC=gcc
CFLAGS=-Wall
EXEC=protocol

all: $(EXEC)

protocol: protocol_primitives.o protocol_instance.o main.o
		$(CC) -o $@ $^

%.o: %.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)

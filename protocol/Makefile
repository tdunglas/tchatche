CC=gcc
CFLAGS=-Wall
LDFLAGS=-lm
EXEC=protocol.exe

all: $(EXEC)

protocol.exe: protocol_primitives.o protocol_instance.o main.o
		$(CC) $(LDFLAGS) -o $@ $^

%.o: %.c
		$(CC) -o $@ -c $< $(CFLAGS)

clean:
		rm -rf *.o

mrproper: clean
		rm -rf $(EXEC)

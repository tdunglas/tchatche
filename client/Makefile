CC=gcc
CFLAGS=-Wall
EXEC=tchatche

all: $(EXEC)

tchatche: file1.o file2.o
        $(CC) -o $@ $^

%.o: %.c
        $(CC) -o $@ -c $< $(CFLAGS)

clean:
        rm -rf *.o

mrproper: clean
        rm -rf $(EXEC)

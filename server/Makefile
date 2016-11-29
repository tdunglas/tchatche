CC=gcc
CFLAGS=-Wall
EXEC=server
CFGLIB=`pkg-config --cflags --libs glib-2.0`


all: $(EXEC)
	
server: function.o server.o
	$(CC) -o $(EXEC) function.o server.o  $(CFGLIB) $(CFLAGS)

function.o: function.c function.h
	$(CC) -o function.o   -c function.c $(CFGLIB) $(CFLAGS)

server.o: server.c
	$(CC) -o server.o -c server.c $(CFGLIB) $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

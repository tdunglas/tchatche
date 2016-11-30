CC=gcc
CFLAGS=-Wall
EXEC=server
CFGLIB=`pkg-config --cflags --libs glib-2.0`


all: $(EXEC)
	
server: function.o server.o
	$(CC) -o $(EXEC) $^  $(CFGLIB) $(CFLAGS)

function.o: function.c function.h
	$(CC) -o $@ -c $< $(CFGLIB) $(CFLAGS)

server.o: server.c
	$(CC) -o $@ -c $^ $(CFGLIB) $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

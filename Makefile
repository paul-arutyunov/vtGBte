# Put your compiler name here 
CC = gcc
CFLAGS = -I$(CURDIR)

PREFIX = "/usr/local"
                   # By default

.PHONY: all clean install uninstall

all: gbt

draw.o: src/draw.c
	$(CC) $(CFLAGS) -c -o draw.o src/draw.c -lncurses

file.o: src/file.c
	$(CC) $(CFLAGS) -c -o file.o src/file.c -lncurses

main.o: src/main.c
	$(CC) $(CFLAGS) -c -o  main.o src/main.c -lncurses

str.o: src/str.c
	$(CC) $(CFLAGS) -c -o str.o src/str.c -lncurses

gbt: main.o str.o draw.o file.o
	$(CC) $(CFLAGS) -o gbt main.o draw.o str.o file.o -lncurses

clean:
	rm -rf gbt *.o

install:
	install ./gbt $(PREFIX)/bin

uninstall:
	rm -rf $(PREFIX)/bin/gbt

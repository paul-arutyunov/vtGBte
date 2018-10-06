# Put your compiler name here 
COMPILER = gcc

PREFIX = "/usr/local"
                   # By default

.PHONY: all clean install uninstall

all: gbt

idrawing.o: idraw.c
	$(COMPILER) -c -o idrawing.o idraw.c -lncurses

file.o: file.c
	$(COMPILER) -c -o file.o file.c -lncurses

gbt.o: gbt.c
	$(COMPILER) -c -o gbt.o gbt.c -lncurses

str.o: str.c
	$(COMPILER) -c -o str.o str.c -lncurses

gbt: gbt.o str.o idrawing.o file.o
	$(COMPILER) -o gbt gbt.o idrawing.o str.o file.o -lncurses

clean:
	rm -rf gbt *.o

install:
	install ./gbt $(PREFIX)/bin

uninstall:
	rm -rf $(PREFIX)/bin/gbt

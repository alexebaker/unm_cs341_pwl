all: pwl

clean:
	rm pwl

pwl: pwl.c pwl.h
	gcc -Wall -ansi -pedantic -D_BSD_SOURCE -o pwl pwl.c


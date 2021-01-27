CC=gcc
CFLAGS=-Wall -O2 -m32
OBJS=tiny.o csapp.o
tiny: $(OBJS)
    $(CC) $(CFLAGS) -o stiny $(OBJS)
tiny.o:
    tiny.c csapp.h
csapp.o:
    csapp.c csapp.h
clean: rm -f *~ *.o tiny



tiny1: tiny.o csapp.o
        gcc -o csapp tiny.o csapp.o
csapp.o: csapp.c csapp.h
        gcc -c csapp.c csapp.h
tiny.o: csapp.h tiny.c
        gcc -c csapp.h tiny.c
		rgss
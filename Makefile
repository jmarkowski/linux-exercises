CC = gcc
CFLAGS = -Wall -O0 -ggdb

PROG += checklen
PROG += daemon
PROG += envarg
PROG += errno
PROG += exec
PROG += exit
PROG += exitstatus
PROG += fdvsfp
PROG += fork
PROG += getenv
PROG += mymalloc
PROG += mutexes
PROG += pid
PROG += resinfo
PROG += signals
PROG += sleep
PROG += sysinfo
PROG += tee
PROG += termcolors
PROG += threads
PROG += userinfo

SRC = $(addsuffix .c, $(PROG))
OBJ = $(addsuffix .o, $(PROG))

all: $(OBJ)

threads.o: threads.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

mutexes.o: mutexes.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

getenv.o: getenv.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean

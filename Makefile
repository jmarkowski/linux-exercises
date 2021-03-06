CC = gcc -std=gnu11
CFLAGS = -Wall -O0 -ggdb

PROG += checklen
PROG += cp
PROG += daemon
PROG += envarg
PROG += errno
PROG += exec
PROG += exit
PROG += exitstatus
PROG += fdvsfp
PROG += fork
PROG += getenv
PROG += ipc
PROG += mymalloc
PROG += mutexes
PROG += pid
PROG += pipe
PROG += popen
PROG += resinfo
PROG += signals
PROG += sleep
PROG += sysinfo
PROG += tee
PROG += termcolors
PROG += terminal-io-attr
PROG += threads
PROG += ttyinfo
PROG += userinfo

SRC = $(addsuffix .c, $(PROG))
OBJ = $(addsuffix .o, $(PROG))

all: $(OBJ)

ipc.o: ipc.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

threads.o: threads.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

mutexes.o: mutexes.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

getenv.o: getenv.c
	$(CC) $(CFLAGS) -pthread -o $@ $<

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f $(OBJ)

.PHONY: all clean

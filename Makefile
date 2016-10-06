CC = gcc
CFLAGS = -Wall

PROG += checklen
PROG += envarg
PROG += exit
PROG += exitstatus
PROG += fdvsfp
PROG += fork
PROG += mymalloc
PROG += resinfo
PROG += sysinfo
PROG += tee
PROG += userinfo

SRC = $(addsuffix .c, $(PROG))
OBJ = $(addsuffix .o, $(PROG))

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean

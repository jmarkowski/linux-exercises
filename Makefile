CC = gcc
CFLAGS = -Wall

PROG += checklen
PROG += env
PROG += exit
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

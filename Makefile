CC = gcc
CFLAGS = -Wall

PROG += env
PROG += checklen
PROG += fdvsfp
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

CC = gcc
CFLAGS = -Wall

PROG += tee
PROG += checklen
PROG += fdvsfp
PROG += userinfo
PROG += sysinfo

SRC = $(addsuffix .c, $(PROG))
OBJ = $(addsuffix .o, $(PROG))

all: $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f *.o

.PHONY: all clean

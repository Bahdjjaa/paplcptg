CC=cc
TARGET=tons
CFLAGS=
CDFLAGS=-Wall -Wextra -g

HFILES=\
	lcol.h\

OBJS=\
	lcol.o\
	tons.o\

.PHONY: all default clean

all: default

default: ${TARGET}

${TARGET}: ${OBJS}
	${CC} ${OBJS} ${CFLAGS} ${CDFLAGS} -o $@

${OBJS}: ${HFILES}

.SUFFIXES: .c .o
.c.o:
	${CC} -c $< ${CFLAGS} ${CDFLAGS} -o $@

clean:
	rm -f ${OBJS} ${TARGET}

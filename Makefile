all: directories netcopy

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

.PHONY: directories clean

netcopy: src/netcopy.c
	gcc $< -o ${BIN_DIR}/$@

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

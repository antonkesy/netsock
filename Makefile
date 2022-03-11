all: directories ncp

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

.PHONY: directories clean

ncp: src/netcopy.c src/arguments/arguments.c src/network/network.c
	gcc $^ -o ${BIN_DIR}/$@

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

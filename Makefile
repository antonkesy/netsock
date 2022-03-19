all: directories netsock

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

BUILD_O=${CC} ${CFLAGS} -c $< -o ${BIN_DIR}/$@

NS_SRC=src/netsock.c src/arguments/arguments.c src/network/network.c src/network/send/send.c src/network/recv/recv.c
NS_DEP_O= ${BIN_DIR}/netsock.o ${BIN_DIR}/arguments.o ${BIN_DIR}/recv.o ${BIN_DIR}/send.o ${BIN_DIR}/network.o


.PHONY: directories clean

netsock: netsock.o arguments.o recv.o send.o network.o
	${CC} ${CFLAGS} ${NS_DEP_O} -o ${BIN_DIR}/$@

netsock.o: src/netsock.c
	${BUILD_O}

arguments.o: src/arguments/arguments.c
	${BUILD_O}

recv.o: src/network/recv/recv.c
	${BUILD_O}

send.o: src/network/send/send.c
	${BUILD_O}

network.o: src/network/network.c
	${BUILD_O}

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

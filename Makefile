all: directories ncp tests

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

NCP_SRC=src/netcopy.c src/arguments/arguments.c src/network/network.c
TEST_SRC=test/test.c test/util/test_socket.c test/util/test_runner.c test/util/test_file.c

.PHONY: directories clean

ncp: ${NCP_SRC}
	${CC} ${CFLAGS} $^ -o ${BIN_DIR}/$@

tests: ${TEST_SRC} ncp
	${CC} ${CFLAGS} ${TEST_SRC} -o ${BIN_DIR}/$@

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

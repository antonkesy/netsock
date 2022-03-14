all: directories netcopy tests

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

NCP_SRC=src/netcopy.c src/arguments/arguments.c src/network/network.c src/network/send/send.c src/network/recv/recv.c
TEST_SRC=test/test.c test/util/socket/test_socket.c test/util/runner/test_runner.c test/util/file/test_file.c

.PHONY: directories clean

netcopy: ${NCP_SRC}
	${CC} ${CFLAGS} $^ -o ${BIN_DIR}/$@

tests: ${TEST_SRC} netcopy
	${CC} ${CFLAGS} ${TEST_SRC} -o ${BIN_DIR}/$@

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

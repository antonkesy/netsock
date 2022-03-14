all: directories netcopy tests

CC=gcc
CFLAGS=-Werror -Wall

BIN_DIR=bin
DIRS=${BIN_DIR}

BUILD_O=${CC} ${CFLAGS} -c $< -o ${BIN_DIR}/$@

NCP_SRC=src/netcopy.c src/arguments/arguments.c src/network/network.c src/network/send/send.c src/network/recv/recv.c
TEST_SRC=test/test.c test/util/socket/test_socket.c test/util/runner/test_runner.c test/util/file/test_file.c
NCP_DEP_O=${BIN_DIR}/arguments.o ${BIN_DIR}/recv.o ${BIN_DIR}/send.o ${BIN_DIR}/network.o


.PHONY: directories clean

netcopy: main_netcopy.o arguments.o recv.o send.o network.o
	${CC} ${CFLAGS} ${BIN_DIR}/main_netcopy.o ${NCP_DEP_O} -o ${BIN_DIR}/$@

main_netcopy.o: src/netcopy.c
	${BUILD_O} -DSTANDALONE

netcopy.o: src/netcopy.c
	${BUILD_O}

arguments.o: src/arguments/arguments.c
	${BUILD_O}

recv.o: src/network/recv/recv.c
	${BUILD_O}

send.o: src/network/send/send.c
	${BUILD_O}

network.o: src/network/network.c
	${BUILD_O}

tests: ${TEST_SRC} netcopy
	${CC} ${CFLAGS} ${TEST_SRC} -o ${BIN_DIR}/$@

directories:
	mkdir -p ${DIRS}

clean:
	-rm ${DIRS} -drf

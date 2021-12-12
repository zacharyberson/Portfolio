CC = gcc
CFLAGS= -Wall
DEPS = lib.h bitwise.h cryptolib.h libDisk.h error_codes.h
SRC = main.c bitwise.c cryptolib.c libDisk.c

libDisk: $(SRC) $(DEPS)
	$(CC) $(CFLAGS) -o libDisk $(SRC)
	cp blankDisk vDisk

clean:
	rm -f libDisk *.debug *.out vDisk

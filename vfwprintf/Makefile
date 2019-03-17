
CC=gcc
CFLAGS=-Wno-switch -D_AMIGA
LIBFLAGS=-fPIC
DEFINES=
LDFLAGS=-noixemul

all: vfwprintf.o
	$(AR) rs libvfwprintf.a vfwprintf.o

test: test.o vfwprintf.o


clean:
	@delete test test.o vfwprintf.o libvfwprintf.a
	
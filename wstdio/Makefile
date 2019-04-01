
CC=gcc
CFLAGS=-Wno-switch -D_AMIGA
LIBFLAGS=-fPIC
DEFINES=
LDFLAGS=-noixemul

all: vfwprintf.o vswprintf.o others.o
	$(AR) rs libwstdio.a vfwprintf.o vswprintf.o others.o

test: test.o vfwprintf.o vswprintf.o others.o


clean:
	@delete test test.o vfwprintf.o vswprintf.o others.o libwstdio.a
	
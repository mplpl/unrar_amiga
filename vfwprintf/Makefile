
CC=gcc
CFLAGS=-Wno-switch -D_AMIGA
LIBFLAGS=-fPIC
DEFINES=
LDFLAGS=-noixemul

all: vfwprintf.o fvwrite.o wsetup.o makebuf.o vswprintf.o others.o
	$(AR) rs libvfwprintf.a vfwprintf.o fvwrite.o wsetup.o makebuf.o vswprintf.o others.o

test: test.o vfwprintf.o fvwrite.o wsetup.o makebuf.o vswprintf.o others.o


clean:
	@delete test test.o vfwprintf.o fvwrite.o wsetup.o makebuf.o vswprintf.o others.o libvfwprintf.a
	
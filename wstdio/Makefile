
CC=gcc
ifeq ($(shell uname),MorphOS)
CC=ppc-morphos-gcc-4
endif
LIBFLAGS=-fPIC
DEFINES=
LDFLAGS=

ifeq ($(shell uname),AROS)
CFLAGS=-Wno-switch -D_AMIGA -I. -D_STDC_NOINLINE_WCTOMB -fshort-wchar
OBJECTS=vfwprintf.o vswprintf.o others.o fake_file.o wcrtomb.o wcschr.o wcscmp.o wcscpy.o wcslen.o wcsncmp.o wcsncpy.o wcspbrk.o wcsrchr.o wmemchr.o wmemset.o wmemmove.o wctomb.o btowc.o fwide.o fputwc.o towupper.o towlower.o
else
CFLAGS=-Wno-switch -D_AMIGA -I.
OBJECTS=vfwprintf.o vswprintf.o others.o fake_file.o
endif

all: $(OBJECTS)
	$(AR) rs libwstdio.a $(OBJECTS)	

test: test.o $(OBJECTS)


clean:
	@delete test test.o libwstdio.a $(OBJECTS)
	
CXX=c++
LIBFLAGS=-fPIC
DEFINES=

ifeq ($(shell uname),AROS)
CXXFLAGS=-O2 -Wno-switch -D_AMIGA -fshort-wchar
LDFLAGS=-L../wstdio  -L../utf8proc
LIBS=../unrarsrc/unicode.o -lwstdio -lutf8proc -liconv
else
CXXFLAGS=-O2 -Wno-switch -D_AMIGA
LDFLAGS=-noixemul -L../wstdio  -L../utf8proc
LIBS=../unrarsrc/unicode.o -lwstdio -lutf8proc
endif

LINK=$(CXX)
	
all: unrar_locale.o
	$(AR) rs libunrarlocale.a unrar_locale.o

test: test.o unrar_locale.o
	$(LINK) -o test $(LDFLAGS) test.o unrar_locale.o $(LIBS)


clean:
	@delete test test.o unrar_locale.o libunrarlocale.a
	

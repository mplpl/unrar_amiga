CXX=c++
CXXFLAGS=-Wno-switch -D_AMIGA
LIBFLAGS=-fPIC
DEFINES=
LDFLAGS=-noixemul -L../vfwprintf 
LIBS=-lvfwprintf

LINK=$(CXX)
	
all: unrar_locale.o
	$(AR) rs libunrarlocale.a unrar_locale.o

test: test.o unrar_locale.o
	$(LINK) -o test $(LDFLAGS) test.o unrar_locale.o $(LIBS)


clean:
	@delete test test.o unrar_locale.o libunrarlocale.a
	
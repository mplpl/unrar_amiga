MAKEDIR=makedir
RM=delete

ifeq ($(PLATFORM),)
PLATFORM=$(shell uname)
endif

ifeq ($(PLATFORM),AmigaOS)
ifeq ($(shell uname -m),ppc)
PLATFORM=AmigaOS4
else
PLATFORM=AmigaOS3
endif
endif

LIBFLAGS=-fPIC
DEFINES=
CXXFLAGS=-O2 -Wno-switch -D_AMIGA
LDFLAGS=-L../wstdio -L../utf8proc
LIBS=../unrarsrc/unicode.o -lwstdio -lutf8proc

ifeq ($(PLATFORM),MorphOS)
CXX=ppc-morphos-g++-10
AR=ppc-morphos-ar
CXXFLAGS+= -noixemul
LDFLAGS+= -noixemul
AMIGA=1
endif

ifeq ($(PLATFORM),WarpOS)
CXX=ppc-morphos-g++-9
AR=ppc-morphos-ar
CFLAGS=--specs=warpup -D__warpos__
CXXFLAGS=--specs=warpup -D__warpos__
LDFLAGS+= --specs=warpup -noixemul
AMIGA=1
endif

ifeq ($(PLATFORM),AmigaOS4)
CXX=ppc-amigaos-c++
AR=ppc-amigaos-ar
LDFLAGS+= -noixemul
AMIGA=1
endif

ifeq ($(PLATFORM),AROS)
CXX=i386-aros-g++
AR=ar
CXXFLAGS+= -fshort-wchar
LIBS+= -liconv
AMIGA=1
endif

ifeq ($(PLATFORM),AmigaOS3)
CXX=m68k-amigaos-g++
AR=m68k-amigaos-ar
CXXFLAGS+= -D__amigaos3__ -I../wstdio -I../wstdio/amigaos3
AMIGA=1
endif

ifeq ($(PLATFORM),Mini)
CXX=m68k-amigaos-g++
AR=m68k-amigaos-ar
CXXFLAGS+= -D__amigaos3__ -D__mini__ -I../wstdio -I../wstdio/amigaos3
LIBS=../unrarsrc/unicode.o -lwstdio
AMIGA=1
endif

ifeq ($(AMIGA),)
$(error Amiga platform not detected - use "PLATFORM=" parameter to pass one of: MorphOS, AmigaOS4, AROS, AmigaOS3, WarpOS, Mini)
endif

LINK=$(CXX)
	
all: unrar_locale.o
	$(AR) rs libunrarlocale.a unrar_locale.o

test: test.o unrar_locale.o
	$(LINK) -o test $(LDFLAGS) test.o unrar_locale.o $(LIBS)


clean:
	-@rm -f test test.o unrar_locale.o libunrarlocale.a
	
cat:
	-@$(RM) /catalogs all
	@$(MAKEDIR) /catalogs
	@$(MAKEDIR) /catalogs/polski 
	@$(MAKEDIR) /catalogs/italiano 
	@$(MAKEDIR) /catalogs/español 
	@$(MAKEDIR) /catalogs/russian 
	@$(MAKEDIR) /catalogs/français 
	@$(MAKEDIR) /catalogs/deutsch
	-flexcat unrar.cd unrar_polish.ct catalog=/catalogs/polski/unrar.catalog
	-flexcat unrar.cd unrar_italian.ct catalog=/catalogs/italiano/unrar.catalog
	-flexcat unrar.cd unrar_spanish.ct catalog=/catalogs/español/unrar.catalog
	-flexcat unrar.cd unrar_russian.ct catalog=/catalogs/russian/unrar.catalog
	-flexcat unrar.cd unrar_french.ct catalog=/catalogs/français/unrar.catalog
	-flexcat unrar.cd unrar_german.ct catalog=/catalogs/deutsch/unrar.catalog

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
LDFLAGS=
CFLAGS=-Wno-switch -D_AMIGA -I.
OBJECTS=vfwprintf.o vswprintf.o others.o fake_file.o

ifeq ($(PLATFORM),MorphOS)
CC=ppc-morphos-gcc-4
AR=ppc-morphos-ar
AMIGA=1
endif

ifeq ($(PLATFORM),WarpOS)
CC=ppc-morphos-gcc-9
AR=ppc-morphos-ar
CFLAGS=--specs=warpup -D__warpos__
LDFLAGS=--specs=warpup
AMIGA=1
endif

ifeq ($(PLATFORM),AmigaOS4)
CC=ppc-amigaos-gcc
AR=ppc-amigaos-ar
AMIGA=1
endif

ifeq ($(PLATFORM),AROS)
CC=i386-aros-gcc
AR=ar
CFLAGS+= -D_STDC_NOINLINE_WCTOMB -fshort-wchar
OBJECTS+= wcrtomb.o wcschr.o wcscmp.o wcscpy.o wcslen.o wcsncmp.o wcsncpy.o wcspbrk.o \
wcsrchr.o wmemchr.o wmemset.o wmemmove.o wctomb.o btowc.o fwide.o fputwc.o towupper.o \
towlower.o getpass.o readpassphrase.o
AMIGA=1
endif

ifeq ($(PLATFORM),AmigaOS3)
CC=m68k-amigaos-gcc
AR=m68k-amigaos-ar
CFLAGS+= -D__amigaos3__ -Iamigaos3
OBJECTS+= wcrtomb.o wcschr.o wcscmp.o wcscpy.o wcslen.o wcsncmp.o wcsncpy.o wcspbrk.o \
wcsrchr.o wmemchr.o wmemset.o wmemmove.o wctomb.o btowc.o fwide.o fputwc.o towupper.o \
towlower.o mbrtowc.o
AMIGA=1
endif

ifeq ($(PLATFORM),Mini)
CC=m68k-amigaos-gcc
AR=m68k-amigaos-ar
LINK=m68k-amigaos-g++
CFLAGS+= -D__amigaos3__ -Iamigaos3 -D__mini__
OBJECTS+= wcrtomb.o wcschr.o wcscmp.o wcscpy.o wcslen.o wcsncmp.o wcsncpy.o wcspbrk.o \
wcsrchr.o wmemchr.o wmemset.o wmemmove.o wctomb.o btowc.o fwide.o fputwc.o towupper.o \
towlower.o mbrtowc.o getpass.o readpassphrase.o truncate.o iconv_mock.o utf8proc_mock.o
AMIGA=1
LDFLAGS=-noixemul
endif

ifeq ($(AMIGA),)
$(error Amiga platform not detected - use "PLATFORM=" parameter to pass one of: MorphOS, AmigaOS4, AROS, AmigaOS3, WarpOS, Mini)
endif

libwstdio.a: $(OBJECTS)
	$(AR) rs libwstdio.a $(OBJECTS)

test: test.o libwstdio.a
	$(LINK) $(LDFLAGS) -o test test.o libwstdio.a

clean:
	-@rm -f test test.o libwstdio.a $(OBJECTS)

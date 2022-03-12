MAKEDIR=makedir
COPY=copy
RM=delete
RENAME=rename
VERSION=6.11

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


$(info Building for $(PLATFORM))

ifeq ($(PLATFORM),AmigaOS3)
BINNAME=unrar_aos
AMIGA=1
endif


ifeq ($(PLATFORM),AmigaOS4)
BINNAME=unrar_aos4
AMIGA=1
endif


ifeq ($(PLATFORM),AROS)
BINNAME=unrar_aros
AMIGA=1
endif


ifeq ($(PLATFORM),MorphOS)
BINNAME=unrar_mos
AMIGA=1
endif


ifeq ($(PLATFORM),WarpOS)
BINNAME=unrar_wos
AMIGA=1
endif

ifeq ($(PLATFORM),Mini)
BINNAME=unrar_aos_mini
AMIGA=1
endif

ifeq ($(AMIGA),)
$(error Amiga platform not detected - use "PLATFORM=" parameter to pass one of: MorphOS, AmigaOS4, AROS, AmigaOS3, WarpOS, Mini)
endif

TOPTARGETS := all clean

ifeq ($(PLATFORM),Mini)
SUBDIRS := wstdio locale unrarsrc
else
SUBDIRS := utf8proc wstdio locale unrarsrc
endif

all: $(TOPTARGETS)
	cp unrarsrc/unrar $(BINNAME)

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

dist: cat
	-@$(MAKEDIR) dist
	-@$(RM) dist/unrar_$(VERSION) all
	-@$(RM) dist/UnRAR-$(VERSION).lha
	@$(MAKEDIR) dist/unrar_$(VERSION)
	$(COPY) UnRAR.readme dist/unrar_$(VERSION)
	$(COPY) unrar_mos dist/unrar_$(VERSION)/unrar_mos
	$(COPY) unrar_aos4 dist/unrar_$(VERSION)/unrar_aos4
	$(COPY) unrar_aros dist/unrar_$(VERSION)/unrar_aros
	$(COPY) unrar_aos dist/unrar_$(VERSION)/unrar_aos
	#$(COPY) unrar_wos dist/unrar_$(VERSION)/unrar_wos
	$(COPY) unrar_aos_mini dist/unrar_$(VERSION)/unrar_aos_mini
	$(COPY) catalogs dist/unrar_$(VERSION)/catalogs all
	$(COPY) license.txt dist/unrar_$(VERSION)
	$(COPY) license_newlib.txt dist/unrar_$(VERSION)
	$(COPY) license_utf8proc.txt dist/unrar_$(VERSION)
	$(COPY) license_libiconv.txt dist/unrar_$(VERSION)
	$(COPY) license_getpass.txt dist/unrar_$(VERSION)
	@cd dist; lha -r a UnRAR-$(VERSION).lha unrar_$(VERSION)
	$(RM) dist/unrar_$(VERSION) all
	$(COPY) UnRAR.readme dist/UnRAR-$(VERSION).readme

cat:
	${MAKE} -C locale cat
	
.PHONY: $(TOPTARGETS) $(SUBDIRS) dist cat

MAKEDIR=makedir
COPY=copy
RM=delete
VERSION=5.80

ifneq ($(PLATFORM),)
export PLATFORM=$(PLATFORM)
endif

TOPTARGETS := all clean

SUBDIRS := utf8proc wstdio locale unrarsrc

all: $(TOPTARGETS)
	cp unrarsrc/unrar .

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

dist:
	-@$(RM) dist all
	@$(MAKEDIR) dist
	@$(MAKEDIR) dist/unrar_$(VERSION)
	$(COPY) UnRAR.readme dist/unrar_$(VERSION)
	$(COPY) unrar dist/unrar_$(VERSION)/unrar_mos
	$(COPY) unrar_aos4 dist/unrar_$(VERSION)/unrar_aos4
	$(COPY) unrar_aros dist/unrar_$(VERSION)/unrar_aros
	$(COPY) unrar_aos3 dist/unrar_$(VERSION)/unrar_aos3
	$(COPY) catalogs dist/unrar_$(VERSION)/catalogs all
	$(COPY) license.txt dist/unrar_$(VERSION)
	$(COPY) license_newlib.txt dist/unrar_$(VERSION)
	$(COPY) license_utf8proc.txt dist/unrar_$(VERSION)
	$(COPY) license_libiconv.txt dist/unrar_$(VERSION)
	$(COPY) license_getpass.txt dist/unrar_$(VERSION)
	@cd dist; lha -r a UnRAR-5.80.lha unrar_5.80
	$(RM) dist/unrar_$(VERSION) all
	$(COPY) UnRAR.readme dist/UnRAR-$(VERSION).readme

.PHONY: $(TOPTARGETS) $(SUBDIRS) dist

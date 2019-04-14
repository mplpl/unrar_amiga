MAKEDIR=makedir
COPY=copy
RM=delete

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
	@$(MAKEDIR) dist/unrar_5.70
	$(COPY) UnRAR.readme dist/unrar_5.70
	$(COPY) unrar dist/unrar_5.70/unrar_mos
	$(COPY) unrar_aos4 dist/unrar_5.70/unrar_aos4
	$(COPY) catalogs dist/unrar_5.70/catalogs all
	$(COPY) license.txt dist/unrar_5.70
	$(COPY) license_newlib.txt dist/unrar_5.70
	$(COPY) license_utf8proc.txt dist/unrar_5.70
	@$(COPY) Makefile.dist dist/Makefile
	make -C dist
	$(RM) dist/Makefile
	$(RM) dist/unrar_5.70 all
	$(COPY) UnRAR.readme dist/UnRAR-5.70.readme

.PHONY: $(TOPTARGETS) $(SUBDIRS) dist

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
	@$(MAKEDIR) dist/unrar
	$(COPY) UnRAR.readme dist/unrar
	$(COPY) unrar dist/unrar/unrar_mos
	$(COPY) unrar_aos4 dist/unrar/unrar_aos4
	$(COPY) catalogs dist/unrar/catalogs all
	$(COPY) locale/unrar.cd dist/unrar
	@$(COPY) Makefile.dist dist/Makefile
	make -C dist
	$(RM) dist/Makefile
	$(RM) dist/unrar all
	$(COPY) UnRAR.readme dist

.PHONY: $(TOPTARGETS) $(SUBDIRS) dist

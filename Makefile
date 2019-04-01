
TOPTARGETS := all clean

SUBDIRS := utf8proc wstdio locale unrarsrc

all: $(TOPTARGETS)
	cp unrarsrc/unrar .

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

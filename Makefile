
TOPTARGETS := all clean

SUBDIRS := utf8proc vfwprintf unrar_src

all: $(TOPTARGETS)
	cp unrar_src/unrar .

$(TOPTARGETS): $(SUBDIRS)
$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

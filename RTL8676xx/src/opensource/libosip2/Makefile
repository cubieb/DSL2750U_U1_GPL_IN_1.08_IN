include $(ROOTDIR)/BaseVar.mk
.PHONY: all build clean

SRCDIR=$(CURDIR)/libosip2-3.2.0

all: configure build


configure:
	cd $(SRCDIR); \
	./configure --prefix=$(TMPDISTDIR) --host=${TARGET} --enable-static;
	touch configure

build:
	make -C $(SRCDIR) all install

clean:
	cd $(SRCDIR); \
	make clean

install:
	make -C $(SRCDIR) install

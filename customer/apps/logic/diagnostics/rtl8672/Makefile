ifeq "$(ROOTDIR)" ""
	export ROOTDIR=$(shell while true; do if [ -f BaseVar.mk ]; then pwd;exit; else cd ..;fi;done;)
endif

include $(ROOTDIR)/.config
include $(ROOTDIR)/BaseVar.mk

CFLAGS= -I$(BUILDDIR) -I$(BUILDDIR)/include -I$(BUILDDIR)/kernel/${TBS_KERNEL_VERSION}/include -I$(BUILDDIR)/apps/include -I$(CURDIR)
LDFLAGS= #-lssap -L$(BUILDDIR)/apps/ssap/lib -L$(TMPDISTDIR)/lib -lscew -lexpat -lz

ifeq "$(ROOTDIR)" "y"
CFLAGS += -DDG_DEBUG
endif

EXT-y :=
EXT-y += ping
EXT-y += oam

EXT_FILE+=$(foreach T,$(EXT-y),extensions/ext_$(T).c)
EXT_OBJS+=$(foreach T,$(EXT-y),extensions/ext_$(T).o)


all: diagtest

diagtest: diag_main.o extensions/initext.o $(EXT_OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

extensions/initext.o: extensions/initext.c
extensions/initext.c: Makefile $(EXT_OBJS)
	echo "" > $@
	for i in $(EXT-y); do \
		echo "extern void $${i}_init(void);" >> $@; \
	done
	echo "void init_extensions(void)" >> $@
	echo "{" >> $@
	for i in $(EXT-y); do \
		echo "	$${i}_init();" >> $@; \
	done
	echo "}" >> $@

clean: $(EXTRA_CLEANS)
	rm -f extensions/initext.c
	@find . -name '*.[ao]' -o -name '*.so' | xargs rm -f

distclean: clean
	@rm -f TAGS `find . -name '*~' -o -name '.*~'` `find . -name '*.rej'` `find . -name '*.d'` .makefirst

install:
	cp diagtest $(FSROOT)/sbin/diagtest
	$(STRIP) $(FSROOT)/sbin/diagtest

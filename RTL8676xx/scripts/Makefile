# ===========================================================================
# Kernel configuration targets
# These targets are used from top-level makefile
#
CC = $(HOSTCC)
PWD = $(shell pwd)
LXDIALOG =$(PWD)/lxdialog
OBJS = conf mconf
all: $(OBJS)

check-lxdialog  := $(LXDIALOG)/check-lxdialog.sh

CFLAGS = $(shell $(check-lxdialog) -ccflags)
LDFLAGS   = $(shell $(check-lxdialog) -ldflags $(HOSTCC))
ifeq ($(shell uname -s),Darwin)
HOST_LOADLIBES  += -lncurses
endif

HOST_EXTRACFLAGS += -DLOCALE

lxdialog := $(LXDIALOG)/checklist.o $(LXDIALOG)/util.o $(LXDIALOG)/inputbox.o $(LXDIALOG)/textbox.o $(LXDIALOG)/yesno.o $(LXDIALOG)/menubox.o

conf-objs	:= conf.o  zconf.tab.o
mconf-objs	:= mconf.o zconf.tab.o $(lxdialog)
kxgettext-objs	:= kxgettext.o zconf.tab.o

zconf.tab.o: lex.zconf.c zconf.hash.c
mconf: $(mconf-objs)
conf:  $(conf-objs)
clean:
	rm -f $(mconf-objs) $(OBJS) $(conf-objs) $(kxgettext-objs)


ifdef LKC_GENPARSER

$(obj)/zconf.tab.c: $(src)/zconf.y
$(obj)/lex.zconf.c: $(src)/zconf.l
$(obj)/zconf.hash.c: $(src)/zconf.gperf

%.tab.c: %.y
	bison -l -b $* -p $(notdir $*) $<
	cp $@ $@_shipped

lex.%.c: %.l
	flex -L -P$(notdir $*) -o$@ $<
	cp $@ $@_shipped

%.hash.c: %.gperf
	gperf < $< > $@
	cp $@ $@_shipped

endif


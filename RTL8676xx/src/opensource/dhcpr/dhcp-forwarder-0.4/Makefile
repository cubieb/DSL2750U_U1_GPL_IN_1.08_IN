# include ../../../../../Rules.make

# export PATH := $(TI_uclibc_path):$(shell echo -n $(PATH))

# all: build install

# dhcp_forwarder_temp = $(TI_build_home)/develop/dhcp_forwarder/__install



build:
	CFLAGS="-O0 -DNDEBUG=0" ./configure --disable-dietlibc --prefix= --build=i386-linux --host=mipsel-linux
	$(MAKE)

#  The above distclean doesn't work. I am adding some workaround
#  Murali
	-rm -rf src/.deps
	-rm -rf test/.deps
	-rm -rf .deps
	-rm -rf doc/dhcp-fwd.1
	-rm -rf test/Makefile
	-rm -rf m4/Makefile
	-rm -rf src/*.o
	-rm -rf Makefile
	-rm -rf config.h
	-rm -rf config.log
	-rm -rf config.status
	-rm -rf dhcp-forwarder.spec
	-rm -rf dhcp-fwd
	-rm -rf stamp-h1


